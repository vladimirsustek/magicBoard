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
#include "rtc.h"
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

#include "print_magneto.h"
#include "calibri16.h"

#include "stm32f7xx_hal_adc.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define TFT_DEMO 1
#define NRF_DEMO 1
#define EEPROM_DEMO 1
#define FM_RADIO_DEMO 1
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
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

	/* User input structure*/
	cli_t cli = {0};
#if TFT_DEMO
  uint32_t tft_Cursor = 5;
  uint32_t tft_Line = 300;
  char tft_TestStr[]= "I am a demo application for the magiBoard!";
  RTC_TimeTypeDef tft_Time = {0};
#endif

#if NRF_DEMO
  uint8_t nrf_lng1 = 0;
  uint8_t nrf_lng2 = 0;
  uint8_t nrf1_tx[33] = {0};
  uint8_t nrf1_rx[33] = {0};
  uint8_t nrf2_tx[33] = {0};
  uint8_t nrf2_rx[33] = {0};
#endif
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
  MX_ETH_Init();
  MX_USART3_UART_Init();
  MX_USB_OTG_FS_USB_Init();
  MX_DAC_Init();
  MX_SPI3_Init();
  MX_TIM3_Init();
  MX_DMA_Init();
  MX_UART7_Init();
  MX_TIM2_Init();
  MX_I2C2_Init();
  MX_I2C4_Init();
  MX_ADC1_Init();
  MX_TIM9_Init();
  MX_RTC_Init();
  MX_TIM4_Init();
  /* USER CODE BEGIN 2 */

  /* Initiliaze UART-console input */
  cli_init();
  HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_SET);
#if TFT_DEMO
  /* TFT functionality overview*/

  reset();
  tft_init(readID());
  fillScreen(BLACK);

  /* Print a size-16 string - general usage*/
  for(uint32_t i = 0; i < strlen(tft_TestStr); i++)
  {
	  tft_Cursor += sPrintCalibri16(tft_Cursor, tft_Line, tft_TestStr[i] - ASCII_OFFSET, WHITE);
  }

  /* Get time */
  HAL_RTC_GetTime(&hrtc, &tft_Time, RTC_FORMAT_BIN);

  /* Print a size-100 numbers - current time usage*/
  printMagneto100(MAGNETO_100_POS_0, MAGNETO_100_LINE, tft_Time.Hours/10);
  printMagneto100(MAGNETO_100_POS_1, MAGNETO_100_LINE, tft_Time.Hours%10);
  printMagnetoComa();
  printMagneto100(MAGNETO_100_POS_2, MAGNETO_100_LINE, tft_Time.Minutes/10);
  printMagneto100(MAGNETO_100_POS_3, MAGNETO_100_LINE, tft_Time.Minutes%10);

  /* Print a size-40 text - informal usage (temperature)*/
  tft_Cursor = MAGNETO_40_START;
  tft_Cursor += printMagneto40(tft_Cursor, MAGNETO_40_LINE_0, '1' - ASCII_OFFSET);
  tft_Cursor += printMagneto40(tft_Cursor, MAGNETO_40_LINE_0, '1' - ASCII_OFFSET);
  tft_Cursor += printMagneto40(tft_Cursor, MAGNETO_40_LINE_0, '.' - ASCII_OFFSET);
  tft_Cursor += printMagneto40(tft_Cursor, MAGNETO_40_LINE_0, '1' - ASCII_OFFSET);
  tft_Cursor += printMagneto40Degree(tft_Cursor, MAGNETO_40_LINE_0);
  tft_Cursor += printMagneto40(tft_Cursor, MAGNETO_40_LINE_0, 'C' - ASCII_OFFSET);
#endif

#ifdef EEPROM_DEMO
  /* Read - out some NVM EEPROM stored custom data */
  printf("AudioOut %d\r\n", NVM_GetAudioOutEnable());
  printf("ESP %d\r\n", NVM_GetESPEnable());
  printf("RDA %d\r\n", NVM_GetRDAEnable());
  printf("RDAfrequency %d\r\n", NVM_GetRDAfrequency());
  printf("RDAvolume %d\r\n", NVM_GetRDAvolume());
#endif

#ifdef WIFI_DEMO
  /* ESP8226-01 Initialization */
  ESP_HTTPinit();
#endif


#ifdef FM_RADIO_DEMO
  /* Initialize RDA5807M */
  RDA5807mPowerOn();
  RDA5807mInit(9170, 1);
  RDA5807mMute(0);
  /* Enable AUDIO_OUT amplifier */
  HAL_GPIO_WritePin(AO_PWR_GPIO_Port, AO_PWR_Pin, GPIO_PIN_SET);
#endif

#ifdef NRF_DEMO

  /* Initialization of both NRF module */
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
#endif
  while(1)
  {

	  /* Process console users UART input*/
	  cli = cli_process();
	  if(cli.pBegin != NULL &&
			  cli.length != 0 &&
			  cli.length != CLI_PROCESS_TIMEOUT)
	  {
		  CmdDispatch(cli.pBegin, cli.length);
	  }

	  /* Just toggle magicBoard's general purpose LEDS*/
	  HAL_GPIO_TogglePin(LED1_GPIO_Port, LED1_Pin);
	  HAL_GPIO_TogglePin(LED2_GPIO_Port, LED2_Pin);

#ifdef NRF_DEMO

	  /* Generate some payload for both NRFs - use ARM's 1ms tick */
	  sprintf((char*)nrf1_tx, "%032ld", HAL_GetTick());
	  sprintf((char*)nrf2_tx, "%032ld", 0x7FFFFFFF - HAL_GetTick());

	  /* Shift payload in the NRFs*/
	  NRF_setW_TX_PAYLOAD(nrf1_tx, 32);
	  NRF_set_W_ACK_PAYLOAD_B(0, nrf2_tx, 32);

	  /* Start receive, start transmit */
	  NRF_CEactivate_B();
	  NRF_CEactivate();

	  /* Wait long enought for reception and ACK */
	  HAL_Delay(5);

	  /* Deactivate reception and transmission */
	  NRF_CEdeactivate_B();
	  NRF_CEdeactivate();

	  /* Check whether IRQ fired (receive, transmit or error occured)*/
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

	  /* Check receive and transmit states, get potential payload and payload length*/
	  nrf_lng1 = NRF_postProcess(0, nrf1_rx);
	  nrf_lng2 = NRF_postProcess_B(0, nrf2_rx);

	  /* Print payloads when available */
	  if(nrf_lng1 && nrf_lng1 != (uint8_t)(-1))
	  {
		  printf("%s\n", nrf1_rx);
		  HAL_Delay(50);
	  }
	  if(nrf_lng2 && nrf_lng2 != (uint8_t)(-1))
	  {
		  printf("%s\n", nrf2_rx);
	  }
#endif
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
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSI|RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_BYPASS;
  RCC_OscInitStruct.LSIState = RCC_LSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 216;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 9;
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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV4;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_7) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
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

