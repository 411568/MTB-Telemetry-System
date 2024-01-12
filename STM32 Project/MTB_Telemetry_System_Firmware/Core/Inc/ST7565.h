/*
 * 	HOW TO USE THIS LIBRARY:
 *
 * 	ST7565_begin(0x7); <- initializes the display
 * 	ST7565_clear();    <- clears the buffer
 * 	Writing / drawing
 * 	ST7565_display();  <- send data to LCD
 */

// Required libraries
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

// uC specific libraries
#include "stm32f4xx.h"
#include "stm32f4xx_it.h"
#include "stm32f4xx_hal_conf.h"
#include "main.h"


// SPI HANDLER
SPI_HandleTypeDef hspi1;
#define handler_1 hspi1



//////////////////////////////////////////////////////////////////////////////

// Pin Definitions - change those to match your project
#define CS_pin DisplayCS_Pin
#define CS_port DisplayCS_GPIO_Port

#define RST_pin DisplayRSE_Pin
#define RST_port DisplayRSE_GPIO_Port

#define RS_pin DisplayRS_Pin
#define RS_port DisplayRS_GPIO_Port

#define GPIO_PIN_SET 1
#define GPIO_PIN_RESET 0

//////////////////////////////////////////////////////////////////////////////

// Bit value macro
#define _BV(bit)(1 << (bit))

// Macro for swapping two uint8_t values
#define swap(a, b) { uint8_t t = a; a = b; b = t; }

//////////////////////////////////////////////////////////////////////////////

// Display setup
#define enablePartialUpdate
#define ST7565_STARTBYTES 0

//////////////////////////////////////////////////////////////////////////////

// Basic defines
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

// Basic display initialization
void ST7565_st7565_init(void);

// Display startup settings including contrast
// 0x07 is usually a good starting point (0x18 for some displays)
void ST7565_begin(uint8_t contrast);

// Send a single command
void ST7565_st7565_command(uint8_t c);

// Send data to the display
void ST7565_st7565_data(uint8_t c);

// Set display brightness
void ST7565_st7565_set_brightness(uint8_t val);

// Doesn't touch the buffer, but clears the display RAM
void ST7565_clear_display(void);

// Clear everything in the uC buffer (doesn't affect the screen output)
void ST7565_clear();

// Send buffer content to display
void ST7565_display();

// Set a single pixel
void ST7565_setpixel(uint8_t x, uint8_t y, uint8_t color);

// Get the value of a single pixel
uint8_t ST7565_getpixel(uint8_t x, uint8_t y);

// Draw a filled circle
void ST7565_fillcircle(uint8_t x0, uint8_t y0, uint8_t r, uint8_t color);

// Draw a circle
void ST7565_drawcircle(uint8_t x0, uint8_t y0, uint8_t r, uint8_t color);

// Draw a rectangle
void ST7565_drawrect(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint8_t color);

// Draw a filled rectangle
void ST7565_fillrect(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint8_t color);

// Bresenham's algorithm for drawing a single line
void ST7565_drawline(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t color);

// Draw a single char in location: x, line
void ST7565_drawchar(uint8_t x, uint8_t line, char c);

// Draw a string
void ST7565_drawstring(uint8_t x, uint8_t line, char* c);

// Draw a string (used for const char*)
void ST7565_drawstring_P(uint8_t x, uint8_t line, const char* c);

// Draw a bitmap under the specified location
void ST7565_drawbitmap(uint8_t x, uint8_t y, const uint8_t* bitmap, uint8_t w, uint8_t h, uint8_t color);

// Send a single byte through SPI
void ST7565_spiwrite(uint8_t c);

// Set a single pixel without updating the bounding box
void ST7565_my_setpixel(uint8_t x, uint8_t y, uint8_t color);

// Get nth element from the 12x16 font array
// How to use it - create an array uint8_t[22] - buffer
// Then use the buffer as a bitmap
void ST7565_get_big_font_bmp(uint8_t number, uint8_t* buffer);
