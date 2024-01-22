#include "stm32f4xx_hal.h"

// HMC5883l - ADDRESS
#define HMC5883l_ADDRESS (0x1E << 1)

// CONTROL REG A
#define HMC5883l_Enable_A 0x78//(0b00011000) // 75 Hz measurement rate, no averaging, default mode

// CONTROL REG B
#define HMC5883l_Enable_B 0xA0//(0b11100000)  // 230 gain

// MODE REGISTER
#define HMC5883l_MR (0x00) // continuous operating mode

// HMC5883l - MSB / LSB ADDRESSES
#define HMC5883l_ADD_DATAX_MSB (0x03)
#define HMC5883l_ADD_DATAX_LSB (0x04)
#define HMC5883l_ADD_DATAZ_MSB (0x05)
#define HMC5883l_ADD_DATAZ_LSB (0x06)
#define HMC5883l_ADD_DATAY_MSB (0x07)
#define HMC5883l_ADD_DATAY_LSB (0x08)

// SUM (MSB + LSB) DEFINE
#define HMC5883l_ADD_DATAX_MSB_MULTI (HMC5883l_ADD_DATAX_MSB | 0x80)
#define HMC5883l_ADD_DATAY_MSB_MULTI (HMC5883l_ADD_DATAY_MSB | 0x80)
#define HMC5883l_ADD_DATAZ_MSB_MULTI (HMC5883l_ADD_DATAZ_MSB | 0x80)

// SENSOR TO CHOOSE
#define SENSOR_FRONT 0
#define SENSOR_REAR 1

/* I2C HANDLER
 * Sensors have the same i2c address, so we're going to use two different i2c modules of the stm32.
 * Change this section to match the currently used i2c handler.
 */
I2C_HandleTypeDef hi2c1; // Front sensor
#define handler_1_HMC hi2c1
I2C_HandleTypeDef hi2c2; // Rear sensor
#define handler_2_HMC hi2c2



// Initialize the device - same settings for both sensors
uint8_t HMC5883L_initialize(void);

// Get the X axis data
uint16_t HMC5883L_get_X(uint8_t sensor);

// Get the Y axis data
uint16_t HMC5883L_get_Y(uint8_t sensor);

// Get the Z axis data
uint16_t HMC5883L_get_Z(uint8_t sensor);

// TODO
// Calculate the suspension travel
