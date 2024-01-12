#include "BrakeSensors.h"


uint16_t Brake_Sensor_Read(uint8_t sensor)
{
	uint16_t ADC_reading = 0;

	if(sensor == SENSOR_LEFT)
	{
		if(HAL_ADC_PollForConversion(&handler_1_brakes, 10) == HAL_OK)
		{
			ADC_reading = HAL_ADC_GetValue(&handler_1_brakes);
		    ADC_SetActiveChannel(&handler_1_brakes, CHANNEL_LEFT);
		    HAL_ADC_Start(&handler_1_brakes);
		}
	}
	else
	{
		if(HAL_ADC_PollForConversion(&handler_1_brakes, 10) == HAL_OK)
		{
			ADC_reading = HAL_ADC_GetValue(&handler_1_brakes);
			ADC_SetActiveChannel(&handler_1_brakes, CHANNEL_RIGHT);
			HAL_ADC_Start(&handler_1_brakes);
		}
	}

	return ADC_reading;
}
