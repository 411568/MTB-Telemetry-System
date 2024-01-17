/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "adc.h"
#include "i2c.h"
#include "rtc.h"
#include "spi.h"
#include "tim.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "ST7565.h"
#include "HMC5883L.h"
#include "BrakeSensors.h"
#include "MiscellaneousFunctions.h"
#include <stdio.h>
#include <string.h>
#include "MPU6050.h"
#include "stm32f4xx_hal_rtc.h"
#include "setup_menu.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
uint16_t HMC_x_axis_front;
uint16_t HMC_x_axis_rear;

uint16_t Brake_left;
uint16_t Brake_right;

uint8_t battery_voltage;

float accel_x;
float gyro_x;

RTC_TimeTypeDef time;
RTC_DateTypeDef date;

uint8_t button_pressed = 0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_ADC1_Init();
  MX_I2C1_Init();
  MX_I2C2_Init();
  MX_RTC_Init();
  MX_SPI1_Init();
  MX_SPI2_Init();
  MX_TIM4_Init();
  /* USER CODE BEGIN 2 */

  ST7565_begin(0x7); // Initialize display
  ST7565_clear(); // Clear the display

  HMC5883L_initialize(); // Initialize magnetometers

  uint8_t check = MPU6050_initialize(); // Initialize MPU6050

  if (check == 1)
  {
	  Error_Handler();
  }

  HAL_TIM_Base_Start_IT(&htim4);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  if (button_pressed == 1)
	  {
		  // Disable button interrupt so that it does not interfere with our menu
		  HAL_NVIC_DisableIRQ(EXTI15_10_IRQn);

		  // Go into the menu display function in file setup_menu.c
		  enterMenu();

		  HAL_Delay(200);
		  __HAL_GPIO_EXTI_CLEAR_IT(SW1_Pin); // clear interrupts
		  button_pressed = 0;

		  HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);
	  }
	  else
	  {
	  	  HAL_Delay(100);
		  ST7565_clear(); // clear the display

		  char str[10] = "";
		  char str_temp[20] = "";

		  // Brake lever bar graph
		  Brake_left = (uint16_t)(Brake_left / 2.3);
		  Brake_right = (uint16_t)(Brake_right / 2.3);
		  ST7565_fillrect(0, 0, Brake_left, 10, BLACK);
		  ST7565_fillrect((128-Brake_right), 0, Brake_right, 10, BLACK);
		  ST7565_drawstring(50, 0, "BRAKE");

		  // Suspension percent
		  ST7565_drawstring(48, 2, "TRAVEL");
		  uint8_t suspension_front_1 = HMC_x_axis_front % 10;
		  uint8_t suspension_front_10 = HMC_x_axis_front / 10;
		  uint8_t suspension_rear_1 = HMC_x_axis_rear % 10;
		  uint8_t suspension_rear_10 = HMC_x_axis_rear / 10;
		  uint8_t buffer[22];

		  ST7565_get_big_font_bmp(suspension_front_10, buffer);
		  ST7565_drawbitmap(10, 15, buffer, 11, 16, BLACK);
		  ST7565_get_big_font_bmp(suspension_front_1, buffer);
		  ST7565_drawbitmap(25, 15, buffer, 11, 16, BLACK);

		  ST7565_get_big_font_bmp(suspension_rear_10, buffer);
		  ST7565_drawbitmap(90, 15, buffer, 11, 16, BLACK);
		  ST7565_get_big_font_bmp(suspension_rear_1, buffer);
		  ST7565_drawbitmap(105, 15, buffer, 11, 16, BLACK);

		  // Current time
		  ST7565_drawstring(10, 6, "TIME");
		  sprintf(str, "%02u", time.Hours);
		  strcat(str_temp, str);
		  strcpy(str, ":");
		  strcat(str_temp, str);
		  sprintf(str, "%02u", time.Minutes);
		  strcat(str_temp, str);
		  strcpy(str, ":");
		  strcat(str_temp, str);
		  sprintf(str, "%02u", time.Seconds);
		  strcat(str_temp, str);
		  ST7565_drawstring(0, 7, str_temp);

		  // Battery %
		  ST7565_drawstring(95, 6, "BATT");
		  sprintf(str, "%u", battery_voltage);
		  strcpy(str_temp, "%");
		  strcat(str, str_temp);
		  ST7565_drawstring(105, 7, str);

		  // Send data to display
		  ST7565_display();
	  }
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE2);
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI|RCC_OSCILLATORTYPE_LSE;
  RCC_OscInitStruct.LSEState = RCC_LSE_ON;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

// Timer interrupt
// On every timer interrupt read the current sensor states and write to SD card
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  // Check which version of the timer triggered this callback
  if (htim == &htim4)
  {
	  // Read magnetometer data
	  HMC_x_axis_front = HMC5883L_get_X(1);
	  HMC_x_axis_rear = HMC5883L_get_X(0);

	  // Read brake sensor ADC
	  Brake_left = Brake_Sensor_Read(SENSOR_LEFT);
	  Brake_right = Brake_Sensor_Read(SENSOR_RIGHT);

	  // Read battery voltage
	  battery_voltage = Read_Battery_Voltage();

	  // Read accelerometer and gyroscpe
	  //float accel_x = MPU6050_accel_read(Xaxis);
	  //float gyro_x = MPU6050_gyro_read(Zaxis);

	  // Get current time
	  HAL_RTC_GetTime(&hrtc, &time, RTC_FORMAT_BIN);
	  HAL_RTC_GetDate(&hrtc, &date, RTC_FORMAT_BIN);

	  // Write to SD card
	  // TODO
  }
}


// Button interrupt
// Disables all interrupts, stops main loop, shows basic menu
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	// Check the interrupt source
    if(GPIO_Pin == SW1_Pin)
    {
    	button_pressed = 1;
    }
}


/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

