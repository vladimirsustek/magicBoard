/*
 * nvm_app.h
 *
 *  Created on: Dec 29, 2022
 *      Author: 42077
 */

#ifndef INC_NVM_APP_H_
#define INC_NVM_APP_H_

#include "eeprom_25aa1024.h"

/* ------------------------------------------------------------------*/
/* ---------------------EEPROM APPLICATION LAYOUT--------------------*/
/* Application's predefined addresses where "SSID","password" are stored
 * distance between addresses and therefore space is 128 bytes what is
 * enough for 32 maximal bytes SSID and maximal 64 bytes password*/
#define EEPROM_WIFI_ADR_0		(uint32_t)(0x01FD00)
#define EEPROM_WIFI_ADR_1		(uint32_t)(0x01FD80)
#define EEPROM_WIFI_ADR_2		(uint32_t)(0x01FE00)
#define EEPROM_WIFI_ADR_3		(uint32_t)(0x01FE80)

/* 4-bytes for sys_state storage */
#define EEPROM_SYS_STATE_ADR	(uint32_t)(0x01FF00)

typedef struct sys_state_flags
{
	uint8_t rdaEnabled : 1, /* To be non-volatile */
			espEnabled : 1, /* To be non-volatile */

			espConnected : 1, /* Being checked on runtime, no reason for non-volatile */
			eepromFunctional : 1, /* Being checked on runtime, no reason for non-volatile */

	        dummy0xA : 4; /* To be non-volatile 0b1010 */
}sys_state_flags_t;

typedef struct sys_state
{
	uint16_t radioFreq;
	uint8_t radioVolm;
	sys_state_flags_t states;
}sys_state_t;


/* to be used as extern*/
sys_state_t systemGlobalState;

uint16_t CmdNVMWriteBytes(const uint8_t* const cmd, const uint16_t lng);
uint16_t CmdNVMReadBytes(const uint8_t* const cmd, const uint16_t lng);
uint8_t* NVM_GetWIfi(uint32_t address, uint32_t offset, uint8_t* pSSIDpassword);
uint32_t NVM_GetSystemState(void);
uint32_t NVM_SetSystemState(void);

#endif /* INC_NVM_APP_H_ */
