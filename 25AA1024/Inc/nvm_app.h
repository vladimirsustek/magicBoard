/*
 * nvm_app.h
 *
 *  Created on: Dec 29, 2022
 *      Author: 42077
 */

#ifndef INC_NVM_APP_H_
#define INC_NVM_APP_H_

#include "eeprom_25aa1024.h"


#define EEPROM_WIFI_ADR_0		(uint32_t)(0x01FD00)
#define EEPROM_WIFI_ADR_1		(uint32_t)(0x01FD80)
#define EEPROM_WIFI_ADR_2		(uint32_t)(0x01FE00)
#define EEPROM_WIFI_ADR_3		(uint32_t)(0x01FE80)

#define WIFI_ADR_LNG 			EEPROM_WIFI_ADR_1 - EEPROM_WIFI_ADR_0

#define NVM_OK					EEPROM_OK
#define NVM_ERR					EEPROM_ERR

/* 4-bytes for sys_state storage */
#define EEPROM_SYS_STATE_ADR	(uint32_t)(0x01FF00)

typedef enum {
	WIFI_ADR_0 = EEPROM_WIFI_ADR_0,
	WIFI_ADR_1 = EEPROM_WIFI_ADR_1,
	WIFI_ADR_2 = EEPROM_WIFI_ADR_2,
	WIFI_ADR_3 = EEPROM_WIFI_ADR_3,
	SYS_STATE_ADR = EEPROM_SYS_STATE_ADR} nvm_adr_e;

/* ------------------------------------------------------------------*/
/* ---------------------EEPROM APPLICATION LAYOUT--------------------*/


/* Address layout within the 25AA1024 memory
 *
 * 0x000000: 130304 to the next address
 *
 * --- free
 *
 * 0x01FD00: EEPROM_WIFI_ADR_0       128 bytes to next address
 * 0x01FD80: EEPROM_WIFI_ADR_1       128 bytes to next address
 * 0x01FE00: EEPROM_WIFI_ADR_2       128 bytes to next address
 * 0x01FE80: EEPROM_WIFI_ADR_3       128 bytes to next address
 * 0x01FF00: EEPROM_SYS_STATE_ADR    255 bytes to last EEPROM address
 *
 * 767 bytes occupied
 */
/* Application's predefined addresses where "SSID","password" are stored
 * distance between addresses and therefore space is 128 bytes what is
 * enough for 32 maximal bytes SSID and maximal 64 bytes password*/


typedef struct sys_state_flags
{
	uint8_t rdaEnabled : 1,
			espEnabled : 1,
			audioOutEnabled : 1,
			dummyUnused : 1,
	        dummy0xA : 4; /* To be non-volatile 0b1010 */
}sys_state_flags_t;

typedef struct sys_state
{
	uint16_t rda5807mFreq;
	uint8_t rda5807mVolm;
	sys_state_flags_t states;
}sys_state_t;

uint16_t CmdNVMWriteBytes(const uint8_t* const cmd, const uint16_t lng);
uint16_t CmdNVMReadBytes(const uint8_t* const cmd, const uint16_t lng);

uint32_t NVM_GetWIfi(nvm_adr_e adr, uint8_t* pSSIDpassword, uint32_t lng);
uint32_t NVM_SetWifi(nvm_adr_e adr, uint8_t* pSSIDpassword, uint32_t lng);

void NVM_SetRDAEnable(uint8_t en);
void NVM_SetESPEnable(uint8_t en);
void NVM_SetAudioOutEnable(uint8_t en);
void NVM_SetRDAvolume(uint8_t volume);
void NVM_SetRDAfrequency(uint16_t frequency);

uint8_t NVM_GetRDAEnable();
uint8_t NVM_GetESPEnable();
uint8_t NVM_GetAudioOutEnable();
uint8_t NVM_GetRDAvolume();
uint16_t NVM_GetRDAfrequency();

#endif /* INC_NVM_APP_H_ */
