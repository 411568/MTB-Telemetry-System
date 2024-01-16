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

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
uint8_t test = 0;
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

  RTC_TimeTypeDef time; // Create time struct
  RTC_DateTypeDef date; // Create date struct

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
		  // TEST
	  	  HAL_Delay(300);
		  ST7565_clear(); // clear the display
		//  ST7565_drawstring(0, 0, "Hello world!");


		  // Read magnetometer data
		  char str[10] = "";
		  char str_temp[20] = "";
		  //HMC5883L_initialize();
		  uint16_t HMC_x_axis_front = HMC5883L_get_X(1);
		  uint16_t HMC_x_axis_rear = HMC5883L_get_X(0);


		  sprintf(str, "%u", HMC_x_axis_front);
		  strcpy(str_temp, "Travel front: ");
		  strcat(str_temp, str);
		  ST7565_drawstring(0, 1, str_temp);

		  sprintf(str, "%u", HMC_x_axis_rear);
		  strcpy(str_temp, "Travel rear: ");
		  strcat(str_temp, str);
		  ST7565_drawstring(0, 2, str_temp);

		  // Read brake sensor ADC
		  uint16_t Brake_left = Brake_Sensor_Read(SENSOR_LEFT);
		  uint16_t Brake_right = Brake_Sensor_Read(SENSOR_RIGHT);

		  sprintf(str, "%u", Brake_left);
		  strcpy(str_temp, "Brake left: ");
		  strcat(str_temp, str);
		  ST7565_drawstring(0, 3, str_temp);

		  sprintf(str, "%u", Brake_right);
		  strcpy(str_temp, "Brake right: ");
		  strcat(str_temp, str);
		  ST7565_drawstring(0, 4, str_temp);

		  // Read battery voltage
		  uint8_t battery_voltage = Read_Battery_Voltage();
		  sprintf(str, "%u", battery_voltage);
		  strcpy(str_temp, "Battery percent: ");
		  strcat(str_temp, str);
		  ST7565_drawstring(0, 5, str_temp);


		  // Read accelerometer and gyroscpe
		  float accel_x = MPU6050_accel_read(Xaxis);
		  float gyro_x = MPU6050_gyro_read(Zaxis);

		  sprintf(str, "%.2f", accel_x);
		  strcpy(str_temp, "Accel: ");
		  strcat(str_temp, str);
		  ST7565_drawstring(0, 6, str_temp);

		  sprintf(str, "%.2f", gyro_x);
		  strcpy(str_temp, "Gyro: ");
		  strcat(str_temp, str);
		  ST7565_drawstring(0, 7, str_temp);

		  // Get current time
		  HAL_RTC_GetTime(&hrtc, &time, RTC_FORMAT_BIN);
		  HAL_RTC_GetDate(&hrtc, &date, RTC_FORMAT_BIN);

		  sprintf(str, "%u", time.Seconds);
		  strcpy(str_temp, "Second: ");
		  strcat(str_temp, str);
		  ST7565_drawstring(0, 0, str_temp);

		  // Interrupt test
		  sprintf(str, "%u", test);
		//  ST7565_drawstring(0, 7, str);

		  // Send data to display
		  ST7565_display();
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
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  // Check which version of the timer triggered this callback
  if (htim == &htim4)
  {
	  if (test == 0)
	  {
		  test = 1;
	  }
	  else
	  {
		  test = 0;
	  }
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

