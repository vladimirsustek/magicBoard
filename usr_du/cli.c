/*
 * cli.c
 *
 *  Created on: Dec 12, 2022
 *      Author: 42077
 */

#include "cli.h"

#define STM32F767

#ifdef STM32F767
/* Porting Handles - Must be defined accordingly  (valid for STM32Fxxx)*/
extern UART_HandleTypeDef huart3;
extern DMA_HandleTypeDef hdma_usart3_rx;
#define RX_CNT	hdma_usart3_rx.Instance->NDTR
#endif

#ifdef STM32F103
/* Porting Handles - Must be defined accordingly  (valid for STM32Fxxx)*/
extern UART_HandleTypeDef huart3;
extern DMA_HandleTypeDef hdma_usart3_rx;
#define RX_CNT	hdma_usart3_rx.Instance->CNDTR
#endif

static uint8_t buffInt[CLI_BUFF_SIZE + 1];
static uint8_t buffExt[CLI_BUFF_SIZE];
static uint32_t uartX_rx_read_ptr = 0;

#define TERMINATION_CHAR	(uint8_t)('\n')
#define STRING_TERMINATION	(uint8_t)('\0')

void cli_init(void)
{
	HAL_UART_Receive_DMA(&huart3, buffInt, CLI_BUFF_SIZE);
}

void cli_deInit(void)
{
	HAL_UART_AbortReceive(&huart3);
}

cli_t cli_process(void)
{
	static uint32_t intLng = 0;
	cli_t retStr = {NULL, 0};

	while(uartX_rx_read_ptr != CLI_BUFF_SIZE - RX_CNT)
	{

		if(intLng >= CLI_PROCESS_TIMEOUT)
		{
			retStr.pBegin = NULL;
			break;
		}

		buffExt[intLng] = buffInt[uartX_rx_read_ptr];

		if(TERMINATION_CHAR == buffInt[uartX_rx_read_ptr])
		{
			buffExt[intLng+1] = STRING_TERMINATION;
			retStr.length = intLng+1;
			retStr.pBegin = buffExt;
			intLng = 0;
		}
		else
		{
			intLng++;
		}
		uartX_rx_read_ptr = (uartX_rx_read_ptr + (uint32_t)1u) % CLI_BUFF_SIZE;

	}

	return retStr;
}

int _write(int file, char *ptr, int len)
{
	HAL_UART_Transmit(&huart3, (uint8_t*)ptr, len, HAL_MAX_DELAY);
	return len;
}
