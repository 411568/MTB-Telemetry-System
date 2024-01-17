#include "setup_menu.h"

void enterMenu()
{
	ST7565_clear();
	ST7565_drawstring(40, 0, "SETTINGS");

	char menu_items[4][20] = {"Set time", "Set travel", "Set brakes", "Exit"};
	uint8_t cursor_position = 0;
	char str_temp[20];
	uint8_t exit = 0;

	while(exit == 0)
	{
		ST7565_clear();
		ST7565_drawstring(40, 0, "SETTINGS");

		// Print menu items
		for(uint8_t i = 0; i < 4; i++)
		{
			if (cursor_position == i)
			{
				  strcpy(str_temp, "-> ");
				  strcat(str_temp, menu_items[i]);
				  ST7565_drawstring(0, i+2, str_temp);
			}
			else
			{
				ST7565_drawstring(0, i+2, menu_items[i]);
			}
		}


		// Button inputs
		if (HAL_GPIO_ReadPin(SW2_GPIO_Port, SW2_Pin) == GPIO_PIN_RESET) // button down
		{
			HAL_Delay(100);
			if (HAL_GPIO_ReadPin(SW2_GPIO_Port, SW2_Pin) == GPIO_PIN_RESET)
			{
				if(cursor_position < 3)
				{
					cursor_position++;
				}
				else
				{
					cursor_position = 0;
				}
			}
		}
		else if (HAL_GPIO_ReadPin(SW3_GPIO_Port, SW3_Pin) == GPIO_PIN_RESET) // button up
		{
			HAL_Delay(100);
			if (HAL_GPIO_ReadPin(SW3_GPIO_Port, SW3_Pin) == GPIO_PIN_RESET)
			{
				if(cursor_position > 0)
				{
					cursor_position--;
				}
				else
				{
					cursor_position = 3;
				}
			}
		}
		else if (HAL_GPIO_ReadPin(SW1_GPIO_Port, SW1_Pin) == GPIO_PIN_RESET) // enter
		{
			HAL_Delay(100);
			if (HAL_GPIO_ReadPin(SW1_GPIO_Port, SW1_Pin) == GPIO_PIN_RESET)
			{
				if (cursor_position == 3)
					exit = 1;
				else if (cursor_position == 2)
					;//setup_brakes(); TODO
				else if (cursor_position == 1)
					;//setup_suspension(); TODO
				else if (cursor_position == 0)
					setupTime();
			}
		}

		ST7565_display();
	}
}


void setupTime()
{
	uint8_t exit = 0;
	uint8_t cursor_position = 0;
	char str[30] = "";
	char str_temp[30] = "";

	RTC_TimeTypeDef time_temp;
	RTC_DateTypeDef date_temp;

	HAL_RTC_GetTime(&hrtc, &time_temp, RTC_FORMAT_BIN);
	HAL_RTC_GetDate(&hrtc, &date_temp, RTC_FORMAT_BIN);

	uint8_t hour = time_temp.Hours;
	uint8_t minute = time_temp.Minutes;
	uint8_t second = time_temp.Seconds;

	memset(str, '\0', sizeof(str));
	memset(str_temp, '\0', sizeof(str_temp));

	sprintf(str, "%02u", hour);
	strcat(str_temp, str);
	strcat(str_temp, " : ");
	sprintf(str, "%02u", minute);
	strcat(str_temp, str);
	strcat(str_temp, " : ");
	sprintf(str, "%02u", second);
	strcat(str_temp, str);

	while(exit == 0)
	{
		ST7565_clear();
		ST7565_drawstring(30, 0, "TIME SETUP");

		memset(str, '\0', sizeof(str));
		memset(str_temp, '\0', sizeof(str_temp));

		sprintf(str, "%02u", hour);
		strcat(str_temp, str);
		strcat(str_temp, " : ");
		sprintf(str, "%02u", minute);
		strcat(str_temp, str);
		strcat(str_temp, " : ");
		sprintf(str, "%02u", second);
		strcat(str_temp, str);

		ST7565_drawstring(20, 3, str_temp);

		// Button inputs
		if (HAL_GPIO_ReadPin(SW2_GPIO_Port, SW2_Pin) == GPIO_PIN_RESET) // button down
		{
			HAL_Delay(100);
			if (HAL_GPIO_ReadPin(SW2_GPIO_Port, SW2_Pin) == GPIO_PIN_RESET)
			{
				if (cursor_position == 0)
				{
					if (hour > 0)
						hour--;
				}
				else if (cursor_position == 1)
				{
					if (minute > 0)
						minute--;
				}
				else
				{
					if (second > 0)
						second--;
				}
			}
		}
		else if (HAL_GPIO_ReadPin(SW3_GPIO_Port, SW3_Pin) == GPIO_PIN_RESET) // button up
		{
			HAL_Delay(100);
			if (HAL_GPIO_ReadPin(SW3_GPIO_Port, SW3_Pin) == GPIO_PIN_RESET)
			{
				if (cursor_position == 0)
				{
					if (hour < 23)
						hour++;
				}
				else if (cursor_position == 1)
				{
					if (minute < 59)
						minute++;
				}
				else
				{
					if (second < 59)
						second++;
				}
			}
		}
		else if (HAL_GPIO_ReadPin(SW1_GPIO_Port, SW1_Pin) == GPIO_PIN_RESET) // button enter
		{
			HAL_Delay(100);
			if (HAL_GPIO_ReadPin(SW1_GPIO_Port, SW1_Pin) == GPIO_PIN_RESET)
			{
				cursor_position++;
				if(cursor_position >= 3)
				{
					// Write to RTC registers on exit
					time_temp.Hours = hour;
					time_temp.Minutes = minute;
					time_temp.Seconds = second;

					HAL_RTC_SetTime(&hrtc, &time_temp, RTC_FORMAT_BIN);
					HAL_RTC_SetDate(&hrtc, &date_temp, RTC_FORMAT_BIN);

					exit = 1;
				}
			}
		}

		ST7565_display();
	}
}
