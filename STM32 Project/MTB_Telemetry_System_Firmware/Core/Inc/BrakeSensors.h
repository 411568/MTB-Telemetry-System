#include "stm32f4xx_hal.h"
#include "main.h"
#include "MiscellaneousFunctions.h"

// SENSOR TO CHOOSE
#define SENSOR_LEFT 0
#define SENSOR_RIGHT 1

// ADC Handler
ADC_HandleTypeDef hadc1;
#define handler_1_brakes hadc1

// Channels definition
#define CHANNEL_LEFT ADC_CHANNEL_0
#define CHANNEL_RIGHT ADC_CHANNEL_1


// Read the brake sensor ADC
uint16_t Brake_Sensor_Read(uint8_t sensor);
