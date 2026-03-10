/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
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
#include "stm32g4xx_hal.h"

#include "stm32g4xx_ll_cordic.h"
#include "stm32g4xx_ll_dac.h"
#include "stm32g4xx_ll_fmac.h"
#include "stm32g4xx_ll_hrtim.h"
#include "stm32g4xx_ll_rcc.h"
#include "stm32g4xx_ll_bus.h"
#include "stm32g4xx_ll_crs.h"
#include "stm32g4xx_ll_system.h"
#include "stm32g4xx_ll_exti.h"
#include "stm32g4xx_ll_cortex.h"
#include "stm32g4xx_ll_utils.h"
#include "stm32g4xx_ll_pwr.h"
#include "stm32g4xx_ll_dma.h"
#include "stm32g4xx_ll_usart.h"
#include "stm32g4xx_ll_gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
//记得在此处包含你用到的外设的头文件
#include "adc.h"
#include "dac.h"
#include "spi.h"
#include "fdcan.h"
#include "hrtim.h"
#include "tim.h"
#include "rtc.h"
#include "cordic.h"
#include "fmac.h"
#include "usart.h"
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
#define INITIAL_PERIOD 14436
#define INITIAL_DEADTIME 250
#define INITIAL_COMPARE INITIAL_PERIOD/2
#define TEST1_Pin LL_GPIO_PIN_13
#define TEST1_GPIO_Port GPIOC
#define B_Pin LL_GPIO_PIN_2
#define B_GPIO_Port GPIOC
#define A_Pin LL_GPIO_PIN_6
#define A_GPIO_Port GPIOA
#define F_Pin LL_GPIO_PIN_10
#define F_GPIO_Port GPIOB
#define E_Pin LL_GPIO_PIN_11
#define E_GPIO_Port GPIOB
#define LFL_Pin LL_GPIO_PIN_6
#define LFL_GPIO_Port GPIOC
#define LFH_Pin LL_GPIO_PIN_7
#define LFH_GPIO_Port GPIOC
#define C_Pin LL_GPIO_PIN_10
#define C_GPIO_Port GPIOA
#define D_C_Pin LL_GPIO_PIN_10
#define D_C_GPIO_Port GPIOC
#define D_Pin LL_GPIO_PIN_7
#define D_GPIO_Port GPIOB
#define TEST2_Pin LL_GPIO_PIN_9
#define TEST2_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */
#define RAMFUNC __attribute__((section(".ccm_text"), used))
#define RAMRO   __attribute__((section(".ccm_ro"), used))
#define RAMVAR  __attribute__((section(".ccm_data"), used))
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
