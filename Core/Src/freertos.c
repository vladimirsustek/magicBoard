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

#include "tft.h"
#include "fonts.h"
#include "functions.h"
#include "print_magneto.h"
#include "calibri16.h"

#include "cli.h"

#include "rtc.h"
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
  .stack_size = 512 * 4,
  .priority = (osPriority_t) osPriorityHigh,
};
/* Definitions for displayTask */
osThreadId_t displayTaskHandle;
const osThreadAttr_t displayTask_attributes = {
  .name = "displayTask",
  .stack_size = 512 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for uartCLI */
osThreadId_t uartCLIHandle;
const osThreadAttr_t uartCLI_attributes = {
  .name = "uartCLI",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */
/* Definitions for wirelessData */
osMessageQueueId_t wirelessDataHandle;
const osMessageQueueAttr_t wirelessData_attributes = {
  .name = "wirelessData"
};

/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void *argument);
void Start_nrfCOM(void *argument);
void Start_displayTask(void *argument);
void Start_uartCLI(void *argument);

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

  /* Create the queue(s) */
  /* creation of wirelessData */
  wirelessDataHandle = osMessageQueueNew (16, sizeof(uint16_t), &wirelessData_attributes);

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  wirelessDataHandle = osMessageQueueNew (1, sizeof(payload_t), &wirelessData_attributes);
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of defaultTask */
  defaultTaskHandle = osThreadNew(StartDefaultTask, NULL, &defaultTask_attributes);

  /* creation of nrfCOM */
  nrfCOMHandle = osThreadNew(Start_nrfCOM, NULL, &nrfCOM_attributes);

  /* creation of displayTask */
  displayTaskHandle = osThreadNew(Start_displayTask, NULL, &displayTask_attributes);

  /* creation of uartCLI */
  uartCLIHandle = osThreadNew(Start_uartCLI, NULL, &uartCLI_attributes);

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
	bool synced = false;
	uint8_t status = 0;
  /* Infinite loop */
  for(;;)
  {
	  NRF_powerDown();
	  osDelay(105);
	  NRF_powerUp();
	  osDelay(105);

	  DEBUG_PRINT("NRF STATUS 1 attempt: 0x%02x\n", NRF_getSTATUS());

	  NRF_configure(false);

	  NRF_set_W_ACK_PAYLOAD(0, (uint8_t*)"DummyACK", strlen("DummyACK"));

	  for(;;)
	  {

		  uint32_t start = osKernelGetTickCount();
		  NRF_CEactivate();
		  do {
			  osDelay(2);
		  } while(osKernelGetTickCount() < start + 4000 && !NRF_getIRQ());

		  NRF_CEdeactivate();

		  status = NRF_getSTATUS();

		  if(status & (1 << RX_DR))
		  {
			  synced = true;
			  NRF_getR_RX_PAYLOAD((uint8_t*)&payload, sizeof(payload_t));
			  osMessageQueuePut(wirelessDataHandle, (void*)&payload, 0, 1);
			  NRF_setSTATUS(1 << RX_DR);
			  NRF_set_W_ACK_PAYLOAD(0, (uint8_t*)"DummyACK", strlen("DummyACK"));
		  }
		  else
		  {
			  synced = false;
		  }

		  if(synced){
			  HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_SET);
			  osDelay(8000);
		  }
		  else
		  {
			  HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_RESET);
			  osDelay(1);
		  }
	  }
    osDelay(1);
  }
  /* USER CODE END Start_nrfCOM */
}

/* USER CODE BEGIN Header_Start_displayTask */
/**
* @brief Function implementing the displayTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_Start_displayTask */
void Start_displayTask(void *argument)
{
  /* USER CODE BEGIN Start_displayTask */
	  RTC_TimeTypeDef previous = {0};
	  RTC_TimeTypeDef current = {0};
	  osStatus_t msg_state;
	  payload_t data;
	  uint32_t prev_vdda;
	  uint32_t tft_Cursor;
	  char str_vdda[16];

	  previous.Hours = 11;
	  previous.Minutes = 11;

	  printMagnetoComa();
  /* Infinite loop */
  for(;;)
  {
	  HAL_RTC_GetTime(&hrtc, &current, RTC_FORMAT_BIN);

	  if(memcmp(&current, &previous, sizeof(RTC_TimeTypeDef)))
	  {
		  /* Print a size-100 numbers - current time usage*/
		  if(previous.Hours/10 != current.Hours/10)
		  {
			  eraseMagneto100(MAGNETO_100_POS_0, MAGNETO_100_LINE, previous.Hours/10);
			  printMagneto100(MAGNETO_100_POS_0, MAGNETO_100_LINE, current.Hours/10);
		  }
		  if(previous.Hours%10 != current.Hours%10)
		  {
			  eraseMagneto100(MAGNETO_100_POS_1, MAGNETO_100_LINE, previous.Hours%10);
			  printMagneto100(MAGNETO_100_POS_1, MAGNETO_100_LINE, current.Hours%10);
		  }
		  if(previous.Minutes/10 != current.Minutes/10)
		  {
			  eraseMagneto100(MAGNETO_100_POS_2, MAGNETO_100_LINE, previous.Minutes/10);
			  printMagneto100(MAGNETO_100_POS_2, MAGNETO_100_LINE, current.Minutes/10);
		  }
		  if(previous.Minutes%10 != current.Minutes%10)
		  {
			  eraseMagneto100(MAGNETO_100_POS_3, MAGNETO_100_LINE, previous.Minutes%10);
			  printMagneto100(MAGNETO_100_POS_3, MAGNETO_100_LINE, current.Minutes%10);
		  }
		  memcpy(&previous, &current, sizeof(RTC_TimeTypeDef));
	  }

	  msg_state = osMessageQueueGet(wirelessDataHandle, (void*)&data, 0, 1);

	  if(msg_state == osOK)
	  {
		  DEBUG_PRINT("temp_ntc %lu\n"
				  "temp_sens %lu\n"
				  "vdda %lu\n"
				  "---------------\r\n",
				  data.temp_ntc,
				  data.temp_sens,
				  data.vdda);

		  if(data.vdda != prev_vdda)
		  {
			  tft_Cursor = MAGNETO_40_START;
			  sprintf(str_vdda, "%lu", prev_vdda);

			  for(uint32_t idx = 0; idx < strlen(str_vdda); idx++)
			  {
				 tft_Cursor += eraseMagneto40(tft_Cursor, MAGNETO_40_LINE_0, str_vdda[idx] - ASCII_OFFSET);
			  }

			  tft_Cursor = MAGNETO_40_START;
			  sprintf(str_vdda, "%lu", data.vdda);

			  for(uint32_t idx = 0; idx < strlen(str_vdda); idx++)
			  {
				  tft_Cursor += printMagneto40(tft_Cursor, MAGNETO_40_LINE_0, str_vdda[idx] - ASCII_OFFSET);
			  }

			  prev_vdda = data.vdda;
		  }
	  }

	  osDelay(1000);
  }
  /* USER CODE END Start_displayTask */
}

/* USER CODE BEGIN Header_Start_uartCLI */
/**
* @brief Function implementing the uartCLI thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_Start_uartCLI */
void Start_uartCLI(void *argument)
{
  /* USER CODE BEGIN Start_uartCLI */
  /* Infinite loop */
  for(;;)
  {
    osDelay(100);
  }
  /* USER CODE END Start_uartCLI */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

