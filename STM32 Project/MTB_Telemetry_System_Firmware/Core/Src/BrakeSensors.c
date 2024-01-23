#include "BrakeSensors.h"


uint16_t Brake_Sensor_Read(uint8_t sensor)
{
	uint16_t ADC_reading = 0;

	if(sensor == SENSOR_LEFT)
	{

		ADC_SetActiveChannel(&handler_1_brakes, CHANNEL_LEFT);
		HAL_ADC_Start(&handler_1_brakes);
		HAL_ADC_PollForConversion(&handler_1_brakes, HAL_MAX_DELAY);
		ADC_reading = HAL_ADC_GetValue(&handler_1_brakes);
	}
	else if (sensor == SENSOR_RIGHT)
	{
		ADC_SetActiveChannel(&handler_1_brakes, CHANNEL_RIGHT);
		HAL_ADC_Start(&handler_1_brakes);
		HAL_ADC_PollForConversion(&handler_1_brakes, HAL_MAX_DELAY);
		ADC_reading = HAL_ADC_GetValue(&handler_1_brakes);
	}
	else
	{
		ADC_reading = 999;
	}

	HAL_ADC_Stop(&handler_1_brakes);

	// Scale to 0-100 range
	ADC_reading = ADC_reading / 40;

	return ADC_reading;
}
