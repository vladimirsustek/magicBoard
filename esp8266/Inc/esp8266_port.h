/*
 * esp8266_port.h
 *
 *  Created on: Jul 19, 2022
 *      Author: 42077
 */

#ifndef ESP8266_PORT_H_
#define ESP8266_PORT_H_

#include <string.h>
#include <stdint.h>
#include <stdio.h>

#include "stm32f7xx_hal.h"


#define PLATFORM_DELAY_MS(x)		(HAL_Delay((x)))
#define ESP_COM_BUFF_LNG			(uint32_t)(1024u)

#define ESP_NEVER_VALUE		(uint32_t)(0xA5A5A5A5)

#define ESP_OK						(uint32_t)(0)
#define ESP_COMMAND_BUSY			(uint32_t)(-1)
#define ESP_RX_SILENT				(uint32_t)(-2)
#define ESP_INCORRECT_CRLF			(uint32_t)(-3)
#define ESP_RX_PENDING				(uint32_t)(-4)
#define ESP_TX_TIMEOUT				(uint32_t)(-5)
#define ESP_HARD_ERR				(uint32_t)(-6)
#define ESP_RSP_ERR					(uint32_t)(-7)
#define ESP_TOO_LONG_HTTP			(uint32_t)(-8)
#define ESP_WIFI_FAILED				(uint32_t)(-9)

#define ESP_DONT_CHECK_RESPONSE_P	(char*)(NULL)
#define ESP_DONT_CHECK_RESPONSE_V	(uint32_t)(0)

#define ESP_TIMEOUT_2s				(uint32_t)(2000u)
#define ESP_TIMEOUT_1s				(uint32_t)(1000u)
#define ESP_TIMEOUT_300ms			(uint32_t)(300u)
#define ESP_TIMEOUT_15s				(uint32_t)(15*1000u)

#define COM_USR_RX_MESSAGES_MAX				(uint32_t)(4u)

#define BITS_OF_UART_8N1_TRANSACTION		(uint32_t)(10u)
#define UART_BAUDRATE						(uint32_t)(115200u)
#define INTO_MILISECONDS					(uint32_t)(1000u)
#define TIMEOUT_FOR_LONGEST_TRANSACTION		(uint32_t)((ESP_COM_BUFF_LNG*BITS_OF_UART_8N1_TRANSACTION*INTO_MILISECONDS)/UART_BAUDRATE)

#define TIMETOUT_1SECOND					(uint32_t)(1000u)

/* Either calculated timeout according to to maximal buffer size or fixed */
//#define  TRANSACTION_TIMEOUT				TIMEOUT_FOR_LONGEST_TRANSACTION
#define TRANSACTION_TIMEOUT					TIMETOUT_1SECOND

#define TX_TIMETOUT							TIMETOUT_1SECOND

typedef uint32_t (*U32_pFn_pC_pC_U32_pC_pU32)	(char *, char*, uint32_t, char **, uint32_t *);

uint32_t espPort_comInit(void);
uint32_t espPort_sendCommand(const char* const pStrCmd, const uint32_t lng);
uint32_t espPort_checkRXBuffer(const uint32_t timeout);

uint32_t ESP_Start_TimeTick(void);
void ESP_IncrementTick(void);
void ESP_IncrementTime(void);
uint32_t ESP_GetTick(void);
uint32_t ESP_GetTime(void);
void ESP_SetTime(uint32_t time);

#endif /* ESP8266_PORT_H_ */
