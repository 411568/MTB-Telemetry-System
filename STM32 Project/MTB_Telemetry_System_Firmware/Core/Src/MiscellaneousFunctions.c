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
	float battery_voltage = 0.0;
	const float voltage_divider = 0.5;
	const float adc_voltage = 3.3;

	// Get the ADC reading
	ADC_SetActiveChannel(&handler_batt_adc, CHANNEL_BATT_ADC);
	HAL_ADC_Start(&handler_batt_adc);
	HAL_ADC_PollForConversion(&handler_batt_adc, HAL_MAX_DELAY);
	ADC_reading = HAL_ADC_GetValue(&handler_batt_adc);

	// Convert to voltage
	battery_voltage = 10 * ADC_reading / 4096 * adc_voltage * voltage_divider;

	return (uint8_t)(battery_voltage);
}
