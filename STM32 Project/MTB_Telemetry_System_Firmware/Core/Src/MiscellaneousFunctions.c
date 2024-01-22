#include "MiscellaneousFunctions.h"


void ADC_SetActiveChannel(ADC_HandleTypeDef *hadc, uint32_t AdcChannel)
{
	ADC_ChannelConfTypeDef sConfig = {0};
	sConfig.Channel = AdcChannel;
	sConfig.Rank = 1;
	sConfig.SamplingTime = ADC_SAMPLETIME_84CYCLES;
	if (HAL_ADC_ConfigChannel(hadc, &sConfig) != HAL_OK)
	{
		Error_Handler();
	}
}


uint8_t Read_Battery_Voltage()
{
	uint16_t ADC_reading = 0;
	uint8_t battery_out = 0;
	const uint16_t bat_min = 2172; // minimum battery reading (about 3.5V)

	// Get the ADC reading
	ADC_SetActiveChannel(&handler_batt_adc, CHANNEL_BATT_ADC);
	HAL_ADC_Start(&handler_batt_adc);
	HAL_ADC_PollForConversion(&handler_batt_adc, HAL_MAX_DELAY);
	ADC_reading = HAL_ADC_GetValue(&handler_batt_adc);

	// Return the battery %
	if (ADC_reading < bat_min)
	{
		battery_out = 0;
	}
	else
	{
		battery_out = (ADC_reading - bat_min) / 5;
		if (battery_out > 100) // if the voltage exceeds about 4.15 V (depends on the ADC supply voltage, so it might shift a little)
		{
			battery_out = 100;
		}
	}

	return battery_out;
}