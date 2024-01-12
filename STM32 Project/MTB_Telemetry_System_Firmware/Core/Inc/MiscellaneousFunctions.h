#include "stm32f4xx_hal.h"
#include "main.h"

// ADC Handler
ADC_HandleTypeDef hadc1;
#define handler_batt_adc hadc1

#define CHANNEL_BATT_ADC ADC_CHANNEL_3


// Set active ADC channel
void ADC_SetActiveChannel(ADC_HandleTypeDef *hadc, uint32_t AdcChannel);

// Read battery voltage from ADC
uint8_t Read_Battery_Voltage();
