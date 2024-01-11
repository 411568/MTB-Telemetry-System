/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define BrakeADC1_Pin GPIO_PIN_0
#define BrakeADC1_GPIO_Port GPIOA
#define BrakeADC2_Pin GPIO_PIN_1
#define BrakeADC2_GPIO_Port GPIOA
#define BatteryADC_Pin GPIO_PIN_2
#define BatteryADC_GPIO_Port GPIOA
#define DisplayRSE_Pin GPIO_PIN_3
#define DisplayRSE_GPIO_Port GPIOA
#define DisplayRS_Pin GPIO_PIN_4
#define DisplayRS_GPIO_Port GPIOA
#define DisplayCS_Pin GPIO_PIN_6
#define DisplayCS_GPIO_Port GPIOA
#define SDCS_Pin GPIO_PIN_12
#define SDCS_GPIO_Port GPIOB
#define SW1_Pin GPIO_PIN_10
#define SW1_GPIO_Port GPIOA
#define SW2_Pin GPIO_PIN_11
#define SW2_GPIO_Port GPIOA
#define SW3_Pin GPIO_PIN_12
#define SW3_GPIO_Port GPIOA
#define DRDY1_Pin GPIO_PIN_5
#define DRDY1_GPIO_Port GPIOB
#define DRDY2_Pin GPIO_PIN_6
#define DRDY2_GPIO_Port GPIOB
#define MPU6050INT_Pin GPIO_PIN_7
#define MPU6050INT_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
