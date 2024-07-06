#include "stm32f4xx_hal.h" 
#include "user_diskio_spi.h"



void SPI_Timer_On(uint32_t waitTicks) 
{
    spiTimerTickStart = HAL_GetTick();
    spiTimerTickDelay = waitTicks;
}

uint8_t SPI_Timer_Status() 
{
    return ((HAL_GetTick() - spiTimerTickStart) < spiTimerTickDelay);
}

//					data to send
static BYTE xchg_spi (BYTE dat)
{
	BYTE rxDat;
    HAL_SPI_TransmitReceive(&SD_SPI_HANDLE, &dat, &rxDat, 1, 50);
    return rxDat;
}

// 							pointer to data, number of bytes
static void rcvr_spi_multi (BYTE *buff,	UINT btr)
{
	for(UINT i=0; i<btr; i++) 
	{
		*(buff+i) = xchg_spi(0xFF);
	}
}

#if _USE_WRITE

// 							pointer to data, number of bytes
static void xmit_spi_multi (const BYTE *buff, UINT btx)
{
	HAL_SPI_Transmit(&SD_SPI_HANDLE, buff, btx, HAL_MAX_DELAY);
}
#endif


//						timeout
static int wait_ready (UINT wt)
{
	BYTE d;
	uint32_t waitSpiTimerTickStart;
	uint32_t waitSpiTimerTickDelay;

	waitSpiTimerTickStart = HAL_GetTick();
	waitSpiTimerTickDelay = (uint32_t)wt;

	// Wait until card is ready
	do 
	{
		d = xchg_spi(0xFF);
	} while (d != 0xFF && ((HAL_GetTick() - waitSpiTimerTickStart) < waitSpiTimerTickDelay));

	return (d == 0xFF) ? 1 : 0;
}


static void despiselect (void)
{
	CS_HIGH();		
	xchg_spi(0xFF);	// Dummy clock

}

static int spiselect (void)
{
	CS_LOW();		
	xchg_spi(0xFF);	// Dummy clock
	if (wait_ready(500)) return 1;	// Wait until card is ready

	despiselect();

	return 0;	
}

// 							data buffer pointer, data block length
static int rcvr_datablock (BYTE *buff, UINT btr)
{
	BYTE token;

	SPI_Timer_On(200);

	do 
	{							
		// Wait for DataStart token
		token = xchg_spi(0xFF);
	} while ((token == 0xFF) && SPI_Timer_Status());

	if(token != 0xFE) return 0;

	rcvr_spi_multi(buff, btr);			// Store trailing data to the buffer
	xchg_spi(0xFF); xchg_spi(0xFF);	

	return 1;
}

#if _USE_WRITE

//						pointer to data buffer, token
static int xmit_datablock (const BYTE *buff, BYTE token)
{
	BYTE resp;


	if (!wait_ready(500)) return 0;		// Wait until card is ready

	xchg_spi(token);
	if (token != 0xFD) 
	{				
		// Send data if token is other than StopTran
		xmit_spi_multi(buff, 512);
		xchg_spi(0xFF); xchg_spi(0xFF);	// Dummy CRC

		// Receive response
		resp = xchg_spi(0xFF);				

		if ((resp & 0x1F) != 0x05) return 0;
	}

	return 1;
}
#endif

//			         command index    argument
static BYTE send_cmd (BYTE cmd, DWORD arg)
{
	BYTE n, res;

	if (cmd & 0x80) 
	{	
		// Send a CMD55 prior to ACMD<n>
		cmd &= 0x7F;
		res = send_cmd(CMD55, 0);

		if (res > 1) return res;
	}

	// Select the card and wait 
	if (cmd != CMD12) 
	{
		despiselect();

		if (!spiselect()) return 0xFF;
	}

	// Send command
	xchg_spi(0x40 | cmd);
	xchg_spi((BYTE)(arg >> 24));
	xchg_spi((BYTE)(arg >> 16));
	xchg_spi((BYTE)(arg >> 8));
	xchg_spi((BYTE)arg);
	n = 0x01;	

	// STOP
	if (cmd == CMD0) n = 0x95;
	if (cmd == CMD8) n = 0x87;

	xchg_spi(n);

	// Receive response 
	if (cmd == CMD12) xchg_spi(0xFF);	// Diacard one byte when CMD12

	n = 10;								// Wait for response
	do 
	{
		res = xchg_spi(0xFF);
	} while ((res & 0x80) && --n);

	return res;							// Return received response
}


//							    	drive number
inline DSTATUS USER_SPI_initialize (BYTE drv)
{
	BYTE n, cmd, ty, ocr[4];

	// Check if correct drive
	if (drv != 0) return STA_NOINIT;

	if (Stat & STA_NODISK) return Stat;	

	FCLK_SLOW(); // Set slow clock
	for (n = 10; n; n--) xchg_spi(0xFF);	// Send dummy clocks

	ty = 0;
	if (send_cmd(CMD0, 0) == 1) 
	{
		// Idle state
		SPI_Timer_On(1000);	
						
		// Timeout
		if (send_cmd(CMD8, 0x1AA) == 1) 
		{	
			// SDv2
			for (n = 0; n < 4; n++) ocr[n] = xchg_spi(0xFF);	// Get 32 bit return value

			if (ocr[2] == 0x01 && ocr[3] == 0xAA) 
			{
				while (SPI_Timer_Status() && send_cmd(ACMD41, 1UL << 30)) ;	// Wait for end of initialization with

				if (SPI_Timer_Status() && send_cmd(CMD58, 0) == 0) 
				{
					// check CCS
					for (n = 0; n < 4; n++) ocr[n] = xchg_spi(0xFF);
					ty = (ocr[0] & 0x40) ? CT_SD2 | CT_BLOCK : CT_SD2;	// Card id SDv2
				}
			}
		} 
		else
		{	
			// Not a SDv2 ( SDv1 or MMC )
			if (send_cmd(ACMD41, 0) <= 1) 	
			{	
				ty = CT_SD1; cmd = ACMD41;	// SDv1
			} 
			else 
			{
				ty = CT_MMC; cmd = CMD1;	// MMCv3
			}
			
			while (SPI_Timer_Status() && send_cmd(cmd, 0));		// Wait for end of initialization

			if (!SPI_Timer_Status() || send_cmd(CMD16, 512) != 0)	// Set block length
			{
				ty = 0;
			}
		}
	}

	// Card type
	CardType = ty;
	despiselect();

	if (ty) // Initialized correctly
	{
		FCLK_FAST();
		Stat &= ~STA_NOINIT;	// Clear STA_NOINIT
	} 
	else 
	{
		Stat = STA_NOINIT;
	}

	return Stat;
}


//  							drive number
inline DSTATUS USER_SPI_status (BYTE drv)
{
	if (drv) return STA_NOINIT;	

	// Return drive status
	return Stat;
}


// 							drive number, buffer for data, sector num, number of sectors to read
inline DRESULT USER_SPI_read (BYTE drv,	BYTE *buff,	DWORD sector, UINT count)
{
	if (drv || !count) return RES_PARERR;		// Check parameter
	if (Stat & STA_NOINIT) return RES_NOTRDY;	// Check if ready

	if (!(CardType & CT_BLOCK)) sector *= 512;	// LBA ==> BA conversion

	if (count == 1) 
	{	
		// Single sector read
		if ((send_cmd(CMD17, sector) == 0) && rcvr_datablock(buff, 512)) 
		{
			count = 0;
		}
	}
	else 
	{				
		// Multiple sector read
		if (send_cmd(CMD18, sector) == 0) 
		{
			do 
			{
				if (!rcvr_datablock(buff, 512)) break;

				buff += 512;
			} while (--count);

			// STOP
			send_cmd(CMD12, 0);
		}
	}

	despiselect();

	return count ? RES_ERROR : RES_OK;
}


#if _USE_WRITE
//                             drive number, pointer to data, start secot, number of sectors
inline DRESULT USER_SPI_write (BYTE drv, const BYTE *buff, DWORD sector, UINT count)	
{
	if (drv || !count) return RES_PARERR;		// Check parameter
	if (Stat & STA_NOINIT) return RES_NOTRDY;	// Check drive status
	if (Stat & STA_PROTECT) return RES_WRPRT;	// Check write protect

	if (!(CardType & CT_BLOCK)) sector *= 512;	// LBA ==> BA conversion

	if (count == 1) 
	{	
		// Single sector write
		if ((send_cmd(CMD24, sector) == 0) && xmit_datablock(buff, 0xFE)) 
		{
			count = 0;
		}
	}
	else 
	{		
		// Multiple sector write
		if (CardType & CT_SDC) send_cmd(ACMD23, count);	// Define number of sectors

		if (send_cmd(CMD25, sector) == 0) 
		{	
			// Multiple block write
			do 
			{
				if (!xmit_datablock(buff, 0xFC)) break;
				
				buff += 512;
			} while (--count);

			// STOP
			if (!xmit_datablock(0, 0xFD)) count = 1;
		}
	}

	despiselect();

	return count ? RES_ERROR : RES_OK;
}
#endif


#if _USE_IOCTL

// 							drive number, command, pointer to data
inline DRESULT USER_SPI_ioctl (BYTE drv, BYTE cmd, void *buff)
{
	DRESULT res;
	BYTE n, csd[16];
	DWORD *dp, st, ed, csize;


	if (drv) return RES_PARERR;					// Check parameter
	if (Stat & STA_NOINIT) return RES_NOTRDY;	// Check if drive is ready

	res = RES_ERROR;

	switch (cmd) 
	{
		// Wait for end of internal write process
		case CTRL_SYNC:		
			if (spiselect()) res = RES_OK;
			break;

		// Get drive capacity
		case GET_SECTOR_COUNT:	
			if ((send_cmd(CMD9, 0) == 0) && rcvr_datablock(csd, 16)) 
			{
				if ((csd[0] >> 6) == 1) 
				{	
					//  SDv2
					csize = csd[9] + ((WORD)csd[8] << 8) + ((DWORD)(csd[7] & 63) << 16) + 1;
					*(DWORD*)buff = csize << 10;
				} 
				else 
				{
					// SDv1 of MMC
					n = (csd[5] & 15) + ((csd[10] & 128) >> 7) + ((csd[9] & 3) << 1) + 2;
					csize = (csd[8] >> 6) + ((WORD)csd[7] << 2) + ((WORD)(csd[6] & 3) << 10) + 1;
					*(DWORD*)buff = csize << (n - 9);
				}

				res = RES_OK;
			}
			break;

		// Get erase block size
		case GET_BLOCK_SIZE:
			if (CardType & CT_SD2) 
			{	// SDv2
				if (send_cmd(ACMD13, 0) == 0) 
				{	
					// Read status
					xchg_spi(0xFF);

					if (rcvr_datablock(csd, 16)) 
					{				
						// Read partial block
						for (n = 64 - 16; n; n--) xchg_spi(0xFF);	// Remove 

						*(DWORD*)buff = 16UL << (csd[10] >> 4);

						res = RES_OK;
					}
				}
			} 
			else 
			{			
				// SDv1 or MMC
				if ((send_cmd(CMD9, 0) == 0) && rcvr_datablock(csd, 16)) 
				{
					// Read CSD
					if (CardType & CT_SD1) 
					{
						// SDv1
						*(DWORD*)buff = (((csd[10] & 63) << 1) + ((WORD)(csd[11] & 128) >> 7) + 1) << ((csd[13] >> 6) - 1);
					}
					else 
					{ 
						// MMC	
						*(DWORD*)buff = ((WORD)((csd[10] & 124) >> 2) + 1) * (((csd[11] & 3) << 3) + ((csd[11] & 224) >> 5) + 1);
					}

					res = RES_OK;
				}
			}
			break;

		// Erase block
		case CTRL_TRIM:
			if (!(CardType & CT_SDC)) break;				  	// Check if SDC 
			if (USER_SPI_ioctl(drv, MMC_GET_CSD, csd)) break;	// Get CSD
			if (!(csd[0] >> 6) && !(csd[10] & 0x40)) break;		// Check if can erase 
			dp = buff; st = dp[0]; ed = dp[1];					// Load block

			if (!(CardType & CT_BLOCK)) 
			{
				st *= 512; ed *= 512;
			}
			if (send_cmd(CMD32, st) == 0 && send_cmd(CMD33, ed) == 0 && send_cmd(CMD38, 0) == 0 && wait_ready(30000)) 
			{	
				// Erase block
				res = RES_OK;
			}
			break;

		default:
			res = RES_PARERR;
	}

	despiselect();

	return res;
}
#endif
