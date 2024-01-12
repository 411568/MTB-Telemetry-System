#include "HMC5883L.h"

void HMC5883L_initialize(void)
{
	uint8_t RegSettingA = HMC5883l_Enable_A;
	uint8_t RegSettingB = HMC5883l_Enable_B;
	uint8_t RegSettingMR = HMC5883l_MR;
	HAL_I2C_Mem_Write(&hi2c3, HMC5883l_ADDRESS, 0x00 , 1, &RegSettingA , 1, 100);
	HAL_I2C_Mem_Write(&hi2c3, HMC5883l_ADDRESS, 0x01 , 1, &RegSettingB , 1, 100);
	HAL_I2C_Mem_Write(&hi2c3, HMC5883l_ADDRESS, 0x02 , 1, &RegSettingMR , 1, 100);
}

uint16_t HMC5883L_get_X(uint8_t sensor)
{
	uint8_t DataX[2];
	uint16_t Xaxis = 0;

	// Read the register
	if(sensor == 0)
		HAL_I2C_Mem_Read(&handler_1_HMC, HMC5883l_ADDRESS, HMC5883l_ADD_DATAX_MSB_MULTI, 1, DataX, 2, 100);
	else
		HAL_I2C_Mem_Read(&handler_2_HMC, HMC5883l_ADDRESS, HMC5883l_ADD_DATAX_MSB_MULTI, 1, DataX, 2, 100);

	Xaxis = ((DataX[1]<<8) | DataX[0]);
	return Xaxis;
}

uint16_t HMC5883L_get_Y(uint8_t sensor)
{
	uint8_t DataY[2];
	uint16_t Yaxis = 0;

	// Read the register
	if(sensor == 0)
		HAL_I2C_Mem_Read(&handler_1_HMC, HMC5883l_ADDRESS, HMC5883l_ADD_DATAY_MSB_MULTI, 1, DataY, 2, 100);
	else
		HAL_I2C_Mem_Read(&handler_2_HMC, HMC5883l_ADDRESS, HMC5883l_ADD_DATAY_MSB_MULTI, 1, DataY, 2, 100);

	Yaxis = ((DataY[1]<<8) | DataY[0]);
	return Yaxis;
}

uint16_t HMC5883L_get_Z(uint8_t sensor)
{
	uint8_t DataZ[2];
	uint16_t Zaxis = 0;

	// Read the register
	if(sensor == 0)
		HAL_I2C_Mem_Read(&handler_1_HMC, HMC5883l_ADDRESS, HMC5883l_ADD_DATAZ_MSB_MULTI, 1, DataZ, 2, 100);
	else
		HAL_I2C_Mem_Read(&handler_2_HMC, HMC5883l_ADDRESS, HMC5883l_ADD_DATAZ_MSB_MULTI, 1, DataZ, 2, 100);

	Zaxis = ((DataZ[1]<<8) | DataZ[0]);
	return Zaxis;
}






