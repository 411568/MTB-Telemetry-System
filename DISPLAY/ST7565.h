/*
 * HOW TO USE THIS LIBRARY:
 * 		ST7565_begin(0x7);
 * 		ST7565_clear(); <- clears the buffer
 *
 * 		then you write something etc
 *
 * 		ST7565_display(); <- send to LCD
 */

///must have libs
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

//uC specific
#include "stm32f1xx.h"
#include "stm32f1xx_it.h"
#include "stm32f1xx_hal_conf.h"
#include "main.h"

SPI_HandleTypeDef hspi1;



//////////////////////////////////////////////////////////////////////////////

//pin defines (you can change the values on the right to match your uC)
#define CS_pin CS_Pin
#define CS_port CS_GPIO_Port

#define RST_pin RSE_Pin
#define RST_port RSE_GPIO_Port

#define RS_pin RS_Pin
#define RS_port RS_GPIO_Port

#define GPIO_PIN_SET 1
#define GPIO_PIN_RESET 0


//////////////////////////////////////////////////////////////////////////////

//bit value macro
#define _BV(bit)(1 << (bit))

//swap two uint8_t values macro
#define swap(a, b) { uint8_t t = a; a = b; b = t; }

#define BLACK 1
#define WHITE 0

#define LCDWIDTH 128
#define LCDHEIGHT 64

#define CMD_DISPLAY_OFF   0xAE
#define CMD_DISPLAY_ON    0xAF

#define CMD_SET_DISP_START_LINE  0x40
#define CMD_SET_PAGE  0xB0

#define CMD_SET_COLUMN_UPPER  0x10
#define CMD_SET_COLUMN_LOWER  0x00

#define CMD_SET_ADC_NORMAL  0xA0
#define CMD_SET_ADC_REVERSE 0xA1

#define CMD_SET_DISP_NORMAL 0xA6
#define CMD_SET_DISP_REVERSE 0xA7

#define CMD_SET_ALLPTS_NORMAL 0xA4
#define CMD_SET_ALLPTS_ON  0xA5
#define CMD_SET_BIAS_9 0xA2
#define CMD_SET_BIAS_7 0xA3

#define CMD_RMW  0xE0
#define CMD_RMW_CLEAR 0xEE
#define CMD_INTERNAL_RESET  0xE2
#define CMD_SET_COM_NORMAL  0xC0
#define CMD_SET_COM_REVERSE  0xC8
#define CMD_SET_POWER_CONTROL  0x28
#define CMD_SET_RESISTOR_RATIO  0x20
#define CMD_SET_VOLUME_FIRST  0x81
#define  CMD_SET_VOLUME_SECOND  0
#define CMD_SET_STATIC_OFF  0xAC
#define  CMD_SET_STATIC_ON  0xAD
#define CMD_SET_STATIC_REG  0x0
#define CMD_SET_BOOSTER_FIRST  0xF8
#define CMD_SET_BOOSTER_234  0
#define  CMD_SET_BOOSTER_5  1
#define  CMD_SET_BOOSTER_6  3
#define CMD_NOP  0xE3
#define CMD_TEST  0xF0

//////////////////////////////////////////////////////////////////////////////

void ST7565_st7565_init(void);
void ST7565_begin(uint8_t contrast);
void ST7565_st7565_command(uint8_t c);
void ST7565_st7565_data(uint8_t c);
void ST7565_st7565_set_brightness(uint8_t val);
void ST7565_clear_display(void);
void ST7565_clear();
void ST7565_display();

void ST7565_setpixel(uint8_t x, uint8_t y, uint8_t color);
uint8_t ST7565_getpixel(uint8_t x, uint8_t y);
void ST7565_fillcircle(uint8_t x0, uint8_t y0, uint8_t r, uint8_t color);
void ST7565_drawcircle(uint8_t x0, uint8_t y0, uint8_t r, uint8_t color);
void ST7565_drawrect(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint8_t color);
void ST7565_fillrect(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint8_t color);
void ST7565_drawline(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t color);
void ST7565_drawchar(uint8_t x, uint8_t line, char c);
void ST7565_drawstring(uint8_t x, uint8_t line, char* c);
void ST7565_drawstring_P(uint8_t x, uint8_t line, const char* c);

void ST7565_drawbitmap(uint8_t x, uint8_t y, const uint8_t* bitmap, uint8_t w, uint8_t h, uint8_t color);

void ST7565_spiwrite(uint8_t c);

void ST7565_my_setpixel(uint8_t x, uint8_t y, uint8_t color);

