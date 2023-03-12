/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
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
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdbool.h>
#include <string.h>
#include <stdio.h>

#include "cli.h"
#include "nrf24l01p_defines.h"
#include "nrf24l01p_driver.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
typedef struct payload
{
	uint32_t vdda;
	uint32_t temp_ntc;
	uint32_t temp_sens;
}payload_t;
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

/* USER CODE END Variables */
/* Definitions for defaultTask */
osThreadId_t defaultTaskHandle;
const osThreadAttr_t defaultTask_attributes = {
  .name = "defaultTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for nrfCOM */
osThreadId_t nrfCOMHandle;
const osThreadAttr_t nrfCOM_attributes = {
  .name = "nrfCOM",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityHigh,
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void *argument);
void Start_nrfCOM(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of defaultTask */
  defaultTaskHandle = osThreadNew(StartDefaultTask, NULL, &defaultTask_attributes);

  /* creation of nrfCOM */
  nrfCOMHandle = osThreadNew(Start_nrfCOM, NULL, &nrfCOM_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void *argument)
{
  /* USER CODE BEGIN StartDefaultTask */
  /* Infinite loop */
  for(;;)
  {
    osDelay(100);
    HAL_GPIO_TogglePin(LED2_GPIO_Port, LED2_Pin);
  }
  /* USER CODE END StartDefaultTask */
}

/* USER CODE BEGIN Header_Start_nrfCOM */
/**
* @brief Function implementing the nrfCOM thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_Start_nrfCOM */
void Start_nrfCOM(void *argument)
{
  /* USER CODE BEGIN Start_nrfCOM */
	payload_t payload = {0};
	bool break_flag = false;
  /* Infinite loop */
  for(;;)
  {
	  NRF_powerDown();
	  osDelay(105);
	  NRF_powerUp();
	  osDelay(105);

	  DEBUG_PRINT("NRF STATUS 1 attempt: 0x%02x\n", NRF_getSTATUS());
	  DEBUG_PRINT("NRF STATUS 2 attempt: 0x%02x\n", NRF_getSTATUS());
	  DEBUG_PRINT("NRF STATUS 3 attempt: 0x%02x\n", NRF_getSTATUS());

	  NRF_configure(false);

	  NRF_set_W_ACK_PAYLOAD(0, (uint8_t*)"DummyACK", strlen("DummyACK"));

	  for(;;)
	  {

		  NRF_CEactivate();
		  osDelay(1000);
		  NRF_CEdeactivate();
		  uint8_t status = NRF_getSTATUS();

		  if(status & (1 << RX_DR))
		  {
			  NRF_getR_RX_PAYLOAD((uint8_t*)&payload, NRF_getR_RX_PL_WID());
			  DEBUG_PRINT("VDDA %ld\n"
					  "CH0 %ld\n"
					  "SENS %ld\n"
					  "-----------\r\n",
					  payload.vdda,
					  payload.temp_ntc,
					  payload.temp_sens);
			  NRF_setSTATUS(1 << RX_DR);
			  NRF_set_W_ACK_PAYLOAD(0, (uint8_t*)"DummyACK", strlen("DummyACK"));
		  }
		  if(break_flag){
			  break;
		  }
	  }
    osDelay(1);
  }
  /* USER CODE END Start_nrfCOM */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

