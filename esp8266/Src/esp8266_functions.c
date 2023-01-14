/*
 * esp8266_functions.c
 *
 *  Created on: Aug 16, 2022
 *      Author: 42077
 */

#include "esp8266_functions.h"

extern UART_HandleTypeDef huart3;

/* Direct access to MDA controlled */
extern char comUsrBuffer[COM_USR_RX_MESSAGES_MAX][ESP_COM_BUFF_LNG + 1];
extern uint32_t comUsrBufferLen[COM_USR_RX_MESSAGES_MAX];
extern uint32_t comUserBufferMsgIdx;
extern uint32_t comUserBufferMsgReadIdx;

#ifndef CMD_METHOD_ATS
#define CMD_METHOD_ATS  "AT"
#endif

#ifndef CMD_ESP8266_ATSET
#define CMD_ESP8266_ATSET "ATST"
#endif

#ifndef CMD_ESP8266_STRWR
#define CMD_ESP8266_STRWR "STWR"
#endif

#define CMD_ESP8266_DELIMITER "_"

#define CMD_ESP8266_HEADER_LNG (uint16_t)(strlen(CMD_METHOD_ATS) + \
								strlen(CMD_ESP8266_DELIMITER) + \
								strlen(CMD_ESP8266_ATSET) + \
								strlen(CMD_ESP8266_DELIMITER))

#ifndef CMD_CUSTOM
#define CMD_CUSTOM			((uint16_t)(0x5A5A))
#endif



uint16_t CmdESPConsoleATCmd(const uint8_t* const cmd, const uint16_t lng)
{
	uint16_t subResult;
	char* auxPtr = (char*)(cmd + CMD_ESP8266_HEADER_LNG);

	subResult = espPort_sendCommand(auxPtr, lng - CMD_ESP8266_HEADER_LNG);
	subResult += ESP_FetchAndPrintResponse();
	subResult = (0 == subResult) ? CMD_CUSTOM : (uint16_t)(-1);

	return subResult;
}

uint16_t CmdESPConsoleWrStr(const uint8_t* const str, const uint16_t lng)
{
	uint16_t subResult;
	char* auxPtr = (char*)(str + CMD_ESP8266_HEADER_LNG);

	subResult = espPort_sendCommand(auxPtr, lng - CMD_ESP8266_HEADER_LNG);
	subResult += ESP_FetchAndPrintResponse();
	subResult = (0 == subResult) ? CMD_CUSTOM : (uint16_t)(-1);

	return subResult;
}


static void printEachMessage(uint32_t enable)
{
	if(!enable) return;
	HAL_UART_Transmit(&huart3,
			(uint8_t*)comUsrBuffer[comUserBufferMsgReadIdx],
			(uint16_t)comUsrBufferLen[comUserBufferMsgReadIdx],
			HAL_MAX_DELAY);
	HAL_UART_Transmit(&huart3, (uint8_t*)("\r\n"), 2, HAL_MAX_DELAY);
}

// WARNING FUNCTION EXPECTS <CR><LF> termination
uint32_t ESP_CheckRX_Blocking(uint32_t timeOut,
		             uint32_t printAllreceived,
					 U32_pFn_pC_pC_U32_pC_pU32 processFn,
					 char * keyWord,
					 char **retStr,
					 uint32_t * retU32)
{
	uint32_t rxResult = ESP_HARD_ERR;
	uint32_t processingResult = ESP_HARD_ERR;

	do
	{
		rxResult = espPort_checkRXBuffer(timeOut);
	}
	while(rxResult == ESP_RX_PENDING /*|| rxResult == ESP_TX_TIMEOUT*/ || rxResult == ESP_RX_SILENT);

	if(rxResult == ESP_OK)
	{
		while(comUserBufferMsgIdx != comUserBufferMsgReadIdx)
		{

			printEachMessage(printAllreceived);

			if(processFn)
			{
				processingResult = processFn(keyWord,
											comUsrBuffer[comUserBufferMsgReadIdx],
											comUsrBufferLen[comUserBufferMsgReadIdx],
											retStr,
											retU32);
			}
			else
			{
				processingResult = ESP_OK;
			}
			comUserBufferMsgReadIdx = (comUserBufferMsgReadIdx + (uint32_t)1u) % COM_USR_RX_MESSAGES_MAX;
		}
	}
	else
	{
		processingResult = rxResult;
	}

	return (processingResult + rxResult);
}


uint32_t ESP_CheckRX_NonBlocking(uint32_t timeOut,
		 uint32_t printAllreceived,
		 U32_pFn_pC_pC_U32_pC_pU32 processFn,
		 char * keyWord,
		 char **retStr,
		 uint32_t * retU32)
{
	uint32_t rxResult = ESP_HARD_ERR;
	uint32_t processingResult = ESP_HARD_ERR;

	if ((rxResult = espPort_checkRXBuffer(timeOut)) == ESP_OK)
	{
		while(comUserBufferMsgIdx != comUserBufferMsgReadIdx)
		{
			printEachMessage(printAllreceived);

			if(processFn)
			{
				processingResult = processFn(keyWord,
											comUsrBuffer[comUserBufferMsgReadIdx],
											comUsrBufferLen[comUserBufferMsgReadIdx],
											retStr,
											retU32);
			}
			else
			{
				processingResult = ESP_OK;
			}
			comUserBufferMsgReadIdx = (comUserBufferMsgReadIdx + (uint32_t)1u) % COM_USR_RX_MESSAGES_MAX;
		}
	}
	else
	{
		processingResult = rxResult;
	}


	return (processingResult + rxResult);
}

uint8_t* ESP_CheckResponse(char *pCmd, uint32_t cmdLng, uint32_t timeOut)
{
	const uint32_t printAllreceived = PRINT_ALL_RECEIVED;
	uint32_t result = ESP_RSP_ERR;
	uint32_t pDummyU32;

	char *pDummyC = NULL;

	result = ESP_CheckRX_Blocking(
			timeOut,
			printAllreceived,
			ESP_CheckForKeyWord,
			pCmd,
			&pDummyC,
			&pDummyU32);

	if (ESP_OK != result)
	{
		pDummyC = NULL;
	}

	return (uint8_t*)pDummyC;
}

uint32_t ESP_FetchAndPrintResponse(void)
{
	const uint32_t printAllreceived = 1;
	const uint32_t delay = 2000;
	uint32_t result = ESP_CheckRX_Blocking(delay, printAllreceived, NULL, NULL, NULL, NULL);
	return result;
}
