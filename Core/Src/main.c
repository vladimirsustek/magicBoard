/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
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
#include <magneto100.h>
#include "main.h"
#include "adc.h"
#include "dac.h"
#include "dma.h"
#include "eth.h"
#include "i2c.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "usb_otg.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>

#include "nrf24l01p_driver.h"
#include "nrf24l01p_driver_B.h"

#include "esp8266_http_server.h"

#include "nvm_app.h"

#include "rda5807m.h"

#include "tft.h"
#include "fonts.h"
#include "functions.h"

#include "cli.h"

#include "cmd_dispatcher.h"

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
uint32_t cursorLine0 = 0;
uint32_t cursorLine1 = 0;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
extern void drawPixel(int16_t x, int16_t y, uint16_t color);
void printMagneto100(uint32_t xOffset, uint32_t yOffset, uint32_t idx);
void _printMagneto100(uint32_t xOffset, uint32_t yOffset, uint32_t idx);
void printMagneto40(uint32_t xOffset, uint32_t yOffset, uint32_t idx);
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
  uint8_t nrf1_tx[33];
  uint8_t nrf1_rx[33] = {0};
  uint8_t nrf2_tx[33];
  uint8_t nrf2_rx[33] = {0};

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
  //MX_DMA_Init();
  //MX_ETH_Init();
  MX_USART3_UART_Init();
  //MX_USB_OTG_FS_USB_Init();
  MX_DAC_Init();
  MX_SPI3_Init();
  MX_TIM3_Init();
  MX_UART7_Init();
  MX_TIM2_Init();
  MX_I2C2_Init();
  MX_I2C4_Init();
  MX_ADC1_Init();
  MX_TIM9_Init();
  /* USER CODE BEGIN 2 */

  reset();
  tft_init(readID());
  fillScreen(BLACK);


#define MAX_X 480
#define CLEARANCE 5
#define MAX_WIDTH 92
#define POS_0 (CLEARANCE)
#define POS_1 (MAX_WIDTH + CLEARANCE)
#define POS_2 (MAX_X - CLEARANCE*4 - MAX_WIDTH*2)
#define POS_3 (MAX_X - CLEARANCE*2 - MAX_WIDTH)


  fillCircle((140+141/2)-20, 240, 10, WHITE);
  fillCircle((140+141/2)+20, 240, 10, WHITE);

  uint8_t hhmm[4] = {0,0,0,1};
  uint8_t hhmmOld[4] = {1,2,3,4};

  while(1)
  {
	  if(hhmm[0] != hhmmOld[0])
	  {
		  _printMagneto100(POS_0, 130, hhmmOld[0]);
		  printMagneto100(POS_0, 130, hhmm[0]);
		  hhmmOld[0] = hhmm[0];
	  }

	  if(hhmm[1] != hhmmOld[1])
	  {
		  _printMagneto100(POS_1, 130, hhmmOld[1]);
		  printMagneto100(POS_1, 130, hhmm[1]);
		  hhmmOld[1] = hhmm[1];
	  }

	  if(hhmm[2] != hhmmOld[2])
	  {
		  _printMagneto100(POS_2, 130, hhmmOld[2]);
		  printMagneto100(POS_2, 130, hhmm[2]);
		  hhmmOld[2] = hhmm[2];
	  }

	  if(hhmm[3] != hhmmOld[3])
	  {
		  _printMagneto100(POS_3, 130, hhmmOld[3]);
		  printMagneto100(POS_3, 130, hhmm[3]);
		  hhmmOld[3] = hhmm[3];
	  }

  }


  printMagneto40(0, 0, '2' - 33);
  printMagneto40(50, 0, '5' - 33);
  printMagneto40(100, 0, '4' - 33);
  printMagneto40(150, 0, 'C' - 33);

  while(1);

  printf("AudioOut %d\r\n", NVM_GetAudioOutEnable());
  printf("ESP %d\r\n", NVM_GetESPEnable());
  printf("RDA %d\r\n", NVM_GetRDAEnable());
  printf("RDAfrequency %d\r\n", NVM_GetRDAfrequency());
  printf("RDAvolume %d\r\n", NVM_GetRDAvolume());

  //ESP_HTTPinit();

  NRF_powerDown();
  NRF_powerDown_B();
  HAL_Delay(500);
  NRF_powerUp();
  NRF_powerUp_B();
  HAL_Delay(500);

  printf("NRF1 STATUS: 0x%02lx\n", NRF_powerCycle(HAL_Delay));
  printf("NRF2 STATUS: 0x%02lx\n", NRF_powerCycle_B(HAL_Delay));

  NRF_configure(true);
  NRF_configure_B(false);

  RDA5807mPowerOn();
  RDA5807mInit(9170, 1);
  RDA5807mMute(0);

  cli_init();

  HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_SET);
  HAL_GPIO_WritePin(AO_PWR_GPIO_Port, AO_PWR_Pin, GPIO_PIN_SET);

  while(1)
  {

	  cli_t cli = cli_process();
	  if(cli.pBegin != NULL &&
			  cli.length != 0 &&
			  cli.length != CLI_PROCESS_TIMEOUT)
	  {
		  CmdDispatch(cli.pBegin, cli.length);
	  }
	  continue;

	  HAL_GPIO_TogglePin(LED1_GPIO_Port, LED1_Pin);
	  HAL_GPIO_TogglePin(LED2_GPIO_Port, LED2_Pin);

	  sprintf((char*)nrf1_tx, "%032ld", HAL_GetTick());
	  sprintf((char*)nrf2_tx, "%032ld", 0x7FFFFFFF - HAL_GetTick());

	  NRF_setW_TX_PAYLOAD(nrf1_tx, 32);
	  NRF_set_W_ACK_PAYLOAD_B(0, nrf2_tx, 32);

	  NRF_CEactivate_B();
	  NRF_CEactivate();
	  HAL_Delay(5);
	  NRF_CEdeactivate_B();
	  NRF_CEdeactivate();

	  if(NRF_getIRQ())
	  {
		  printf("NRF1 IRQ\n");
		  HAL_Delay(50);
	  }

	  if(NRF_getIRQ_B())
	  {
		  printf("NRF2 IRQ\n");
		  HAL_Delay(50);
	  }

	  uint8_t lng1 = NRF_postProcess(0, nrf1_rx);
	  uint8_t lng2 = NRF_postProcess_B(0, nrf2_rx);

	  if(lng1 && lng1 != (uint8_t)(-1))
	  {
		  printf("%s\n", nrf1_rx);
		  HAL_Delay(50);
	  }
	  if(lng2 && lng2 != (uint8_t)(-1))
	  {
		  printf("%s\n", nrf2_rx);
	  }

	  HAL_Delay(100);
  }

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
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

  /** Configure LSE Drive Capability
  */
  HAL_PWR_EnableBkUpAccess();
  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_BYPASS;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 96;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  RCC_OscInitStruct.PLL.PLLR = 2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Activate the Over-Drive mode
  */
  if (HAL_PWREx_EnableOverDrive() != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
void printMagneto100(uint32_t xOffset, uint32_t yOffset, uint32_t idx)
{
	uint32_t adrOffset = magneto_100ptDescriptors[idx].offset;
	uint32_t width = magneto_100ptDescriptors[idx].width;
	uint32_t righAlignOffset = MAX_WIDTH - magneto_100ptDescriptors[idx].width;

	/* Get align width in bytes*/
	width = (width % 8) ? width / 8 + 1 : width / 8;

	uint32_t height = (magneto_100ptDescriptors[idx + 1].offset - adrOffset)/width;
	uint8_t inspectedByte = 0;
	uint8_t inspectedBit = 0;

	  for(uint32_t line = 0; line < height; line++)
	  {
		  for(uint32_t pixel = 0; pixel < width*8; pixel++)
		  {
			  if(pixel % 8 == 0)
			  {
				  inspectedByte = magneto_100ptBitmaps[adrOffset + line* width + pixel / 8];
			  }

			  inspectedBit = inspectedByte & (1 << (7 - (pixel % 8)));
			  inspectedBit = inspectedBit >> (7 - (pixel % 8));

			  if(inspectedBit)
			  {
				  drawPixel(height - line + yOffset, xOffset + pixel + righAlignOffset, WHITE);
			  }
		  }
	  }

}

void _printMagneto100(uint32_t xOffset, uint32_t yOffset, uint32_t idx)
{
	uint32_t adrOffset = magneto_100ptDescriptors[idx].offset;
	uint32_t width = magneto_100ptDescriptors[idx].width;
	uint32_t righAlignOffset = MAX_WIDTH - magneto_100ptDescriptors[idx].width;

	/* Get align width in bytes*/
	width = (width % 8) ? width / 8 + 1 : width / 8;

	uint32_t height = (magneto_100ptDescriptors[idx + 1].offset - adrOffset)/width;
	uint8_t inspectedByte = 0;
	uint8_t inspectedBit = 0;

	  for(uint32_t line = 0; line < height; line++)
	  {
		  for(uint32_t pixel = 0; pixel < width*8; pixel++)
		  {
			  if(pixel % 8 == 0)
			  {
				  inspectedByte = magneto_100ptBitmaps[adrOffset + line* width + pixel / 8];
			  }

			  inspectedBit = inspectedByte & (1 << (7 - (pixel % 8)));
			  inspectedBit = inspectedBit >> (7 - (pixel % 8));

			  if(inspectedBit)
			  {
				  drawPixel(height - line + yOffset, xOffset + pixel + righAlignOffset, BLACK);
			  }
		  }
	  }

}

void printMagneto40(uint32_t xOffset, uint32_t yOffset, uint32_t idx)
{
	uint32_t adrOffset = magneto_60ptDescriptors[idx].offset;
	uint32_t width = magneto_60ptDescriptors[idx].width;

	/* Get align width in bytes*/
	width = (width % 8) ? width / 8 + 1 : width / 8;

	uint32_t height = (magneto_60ptDescriptors[idx + 1].offset - adrOffset)/width;
	uint8_t inspectedByte = 0;
	uint8_t inspectedBit = 0;

	  for(uint32_t line = 0; line < height; line++)
	  {
		  for(uint32_t pixel = 0; pixel < width*8; pixel++)
		  {
			  if(pixel % 8 == 0)
			  {
				  inspectedByte = magneto_60ptBitmaps[adrOffset + line* width + pixel / 8];
			  }

			  inspectedBit = inspectedByte & (1 << (7 - (pixel % 8)));
			  inspectedBit = inspectedBit >> (7 - (pixel % 8));

			  if(inspectedBit)
			  {
				  drawPixel(height - line + yOffset, pixel + cursorLine0, WHITE);
			  }
		  }
	  }

	  cursorLine0 += magneto_60ptDescriptors[idx].width + 2;

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

