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
#include "nvm_app.h"
#include "esp8266_http_server.h"
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
typedef struct Measurement
{
	int32_t ntc2;
	int32_t ntc1;
	int32_t oagp;
	int32_t curr;
	int32_t tmpi;
	int32_t vdda;
	int32_t tmpe;
}Measurement_t;
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
/* Definitions for WIFITask */
osThreadId_t WIFITaskHandle;
const osThreadAttr_t WIFITask_attributes = {
  .name = "WIFITask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};


/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */
/* Definitions for wirelessData */
osMessageQueueId_t wirelessDataHandle;
const osMessageQueueAttr_t wirelessData_attributes = {
  .name = "wirelessData"
};
osMessageQueueId_t wifiDataHandle;
const osMessageQueueAttr_t wifiData_attributes = {
  .name = "wifiData"
};
/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void *argument);
void Start_nrfCOM(void *argument);
void Start_displayTask(void *argument);
void Start_uartCLI(void *argument);
void StartWIFITask(void *argument);

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

  /* creation of wifiData */
  wifiDataHandle = osMessageQueueNew (16, sizeof(uint16_t), &wifiData_attributes);

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  wirelessDataHandle = osMessageQueueNew (1, sizeof(Measurement_t), &wirelessData_attributes);
  wifiDataHandle = osMessageQueueNew (16, sizeof(uint16_t), &wifiData_attributes);
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

  /* creation of WIFITask */
  WIFITaskHandle = osThreadNew(StartWIFITask, NULL, &WIFITask_attributes);

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
    HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, GPIO_PIN_SET);
    osDelay(50);
    HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, GPIO_PIN_RESET);
    osDelay(950);
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
	static Measurement_t payload = {0};
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

		  NRF_CEactivate();
		  osDelay(200);
		  NRF_CEdeactivate();

		  status = NRF_getSTATUS();

		  if(status & (1 << RX_DR))
		  {
			  synced = true;
			  NRF_getR_RX_PAYLOAD((uint8_t*)&payload, sizeof(Measurement_t));


			  DEBUG_PRINT("NTC2 %ld\n", payload.ntc2);
			  DEBUG_PRINT("NTC1 %ld\n", payload.ntc1);
			  DEBUG_PRINT("OAGP %ld\n", payload.oagp);
			  DEBUG_PRINT("CURR %ld\n", payload.curr);
			  DEBUG_PRINT("TMPI %ld\n", payload.tmpi);
			  DEBUG_PRINT("VDDA %ld\n", payload.vdda);
			  DEBUG_PRINT("TMPE %ld\n", payload.tmpe);

			  NRF_setSTATUS(1 << RX_DR | 1 << TX_DS);

			  osMessageQueuePut(wirelessDataHandle, (uint8_t*)&payload, 0, 1);

			  NRF_set_W_ACK_PAYLOAD(0, (uint8_t*)"DummyACK", strlen("DummyACK"));

		  }
		  else
		  {
			  synced = false;
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
	  RTC_DateTypeDef dummy = {0};

	  previous.Hours = 11;
	  previous.Minutes = 11;
	  previous.Seconds = 11;
	  current.Hours = 0;
	  current.Minutes = 0;
	  current.Seconds = 0;

	  bool firstTime = true;
	  osStatus_t msg_state;
	  Measurement_t data;
	  int32_t prev_temp = 0;
	  uint32_t tft_Cursor;
	  char str_vdda[16];

	  printMagnetoComa();

	  NVM_GetHardcodeTime(&current);

  /* Infinite loop */
  for(;;)
  {
	  /* It's a STM HAL bug ... without the reading date
	   * the read time does not work */
	  HAL_RTC_GetDate(&hrtc, &dummy, RTC_FORMAT_BIN);
	  HAL_RTC_GetTime(&hrtc, &current, RTC_FORMAT_BIN);

	  if(memcmp(&current, &previous, sizeof(RTC_TimeTypeDef)) || firstTime)
	  {
		  firstTime = false;
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

	  if(msg_state == osOK && data.tmpe != INT32_MIN)
	  {

		  data.tmpe= data.tmpe / 100;

		  if(data.tmpe != prev_temp)
		  {

			  tft_Cursor = MAGNETO_40_START;
			  sprintf(str_vdda, "%ld.%ld", prev_temp/10, prev_temp%10);

			  for(uint32_t idx = 0; idx < strlen(str_vdda); idx++)
			  {
				 tft_Cursor += eraseMagneto40(tft_Cursor, MAGNETO_40_LINE_0, str_vdda[idx] - ASCII_OFFSET);
			  }

			  tft_Cursor += eraseMagneto40Degree(tft_Cursor, MAGNETO_40_LINE_0);
			  tft_Cursor += eraseMagneto40(tft_Cursor, MAGNETO_40_LINE_0, 'C' - ASCII_OFFSET);

			  tft_Cursor = MAGNETO_40_START;
			  sprintf(str_vdda, "%ld.%ld", data.tmpe/10, data.tmpe%10);

			  for(uint32_t idx = 0; idx < strlen(str_vdda); idx++)
			  {
				  tft_Cursor += printMagneto40(tft_Cursor, MAGNETO_40_LINE_0, str_vdda[idx] - ASCII_OFFSET);
			  }

			  tft_Cursor += printMagneto40Degree(tft_Cursor, MAGNETO_40_LINE_0);
			  tft_Cursor += printMagneto40(tft_Cursor, MAGNETO_40_LINE_0, 'C' - ASCII_OFFSET);


			  prev_temp = data.tmpe;
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

/* USER CODE BEGIN Header_StartWIFITask */
/**
* @brief Function implementing the WIFITask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartWIFITask */
void StartWIFITask(void *argument)
{
  /* USER CODE BEGIN StartWIFITask */

	char *pHTTPReq = NULL;
    uint32_t httpReqLng = 0;

    NVM_SetWifi(WIFI_ADR_0, (uint8_t*)"\"UPCEDFF983\",\"tb6mhkmxW6ee\"",
    		strlen("\"UPCEDFF983\",\"tb6mhkmxW6ee\""));

	ESP_HTTPinit();

  /* Infinite loop */
  for(;;)
  {
      if(ESP_RET_OK == ESP_CheckReceiveHTTP(&pHTTPReq, &httpReqLng))
      {
          ESP_ProcessHTTP(pHTTPReq, httpReqLng);
      }
    osDelay(100);
  }
  /* USER CODE END StartWIFITask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

