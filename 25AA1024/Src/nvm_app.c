/*
 * nvm_app.c
 *
 *  Created on: Dec 29, 2022
 *      Author: 42077
 */

#include "nvm_app.h"

#define ESP_RESULT_CUSTOM 		(uint16_t)(0x5A5A)
#define EEPROM_R_ERROR			(char*)("EEPROM_R_ERROR\r\n")
#define EEPROM_W_ERROR			(char*)("EEPROM_W_ERROR\r\n")

#define ESP_CMD_ADR 			(uint8_t*)"ADR_"
#define ESP_CMD_LNG 			(uint8_t*)"LNG_"

#define EEPROM_W_CMD_OFFSET		(uint16_t)(strlen("EE_WRBT_ADR_000000_"))
#define EEPROM_R_CMD_OFFSET		(uint16_t)(strlen("EE_RDBT_ADR_000000_LNG_000000_"))

#define IS_NUM(x)   ((x) >= '0' && (x) <= '9')

#ifndef CMD_CUSTOM
#define CMD_CUSTOM			((uint16_t)(0x5A5A))
#endif

#define PLATFORM_DELAY_MS(x)		(HAL_Delay((x)))

sys_state_t systemGlobalState;

/* Helper function */
static uint32_t ExtractValue(const uint8_t* const pKeyWord,
		const uint8_t* const pBUff,
		const uint32_t buffLng,
		uint32_t* val);

#ifdef SHOW_HOW_TO_USE_NVM_GetWifi
/* Supposed to be used with ESP8266, thus AT+CWJAP=*/
	EEPROM_WriteData(EEPROM_WIFI_ADR_0,
			(uint8_t*)"\"WIFI_NAME\",\"WIFI_PASSWORD\"\r\n",
			strlen("\"WIFI_NAME\",\"WIFI_PASSWORD\"\r\n"));
	NVM_GetWIfi(EEPROM_WIFI_ADR_0, strlen("AT+CWJAP="), pWIFI);
	memcpy(pWIFI, "AT+CWJAP=", strlen("AT+CWJAP="));
	printf("%s\r\n", pWIFI);
#endif

uint8_t* NVM_GetWIfi(uint32_t address, uint32_t offset, uint8_t* pSSIDpassword)
{
	uint8_t* result = NULL;
	uint8_t quotationMarkCnt = 0;

	uint32_t subResult = (uint32_t)(-1);
	uint16_t idx = 0;

	if(offset <= 32)
	{
		subResult = EEPROM_ReadData(address, pSSIDpassword + offset, EEPROM_PAGE_SIZE/2);

		for(idx = 0; idx < EEPROM_PAGE_SIZE/2; idx++)
		{
			if (pSSIDpassword[idx + offset] == '\"')
			{
				quotationMarkCnt++;
			}
			if(4 == quotationMarkCnt)
			{
				idx++;
				break;
			}
		}
	}

	if (0 == subResult && 4 == quotationMarkCnt)
	{
		if(!memcmp(pSSIDpassword + offset + idx, "\r\n", 2))
		{
			idx +=2;
			memset(pSSIDpassword + offset + idx, '\0', EEPROM_PAGE_SIZE/2 - offset - idx);
			result = pSSIDpassword;
		}
	}

	return result;
}

uint32_t NVM_GetSystemState(void)
{
	uint32_t subResult;

	subResult = EEPROM_ReadData(EEPROM_SYS_STATE_ADR, (uint8_t*)&systemGlobalState, sizeof(sys_state_t));

	subResult = (0 == subResult) ? 0 : (uint32_t)-1;

	return subResult;
}

uint32_t NVM_SetSystemState(void)
{
	uint32_t subResult;

	subResult = EEPROM_WriteData(EEPROM_SYS_STATE_ADR, (uint8_t*)&systemGlobalState, sizeof(sys_state_t));

	subResult = (0 == subResult) ? 0 : (uint32_t)-1;

	return subResult;
}



uint16_t CmdNVMWriteBytes(const uint8_t* const cmd, const uint16_t lng)
{
	// expected e.g. EEPROM_W_ADR_000000_HelloWorld

	uint16_t payloadLng = lng - EEPROM_W_CMD_OFFSET;

	uint32_t addr = 0;
	uint32_t subResult = (uint32_t)(-1);

	ExtractValue(ESP_CMD_ADR, cmd, lng, &addr);

	subResult = EEPROM_WriteData(
			addr,
			(uint8_t*)cmd + EEPROM_W_CMD_OFFSET,
			payloadLng
			);

	if(0 == subResult)
	{
    	printf("EEPROM_W address %ld = 0x%06lx : %d bytes\r\n",
    			addr,
				addr,
				lng - EEPROM_W_CMD_OFFSET);
	}
	else
	{
    	printf(EEPROM_W_ERROR);
	}

	subResult = (subResult == 0) ? CMD_CUSTOM : (uint16_t)(-1);

	return subResult;
}

uint16_t CmdNVMReadBytes(const uint8_t* const cmd, const uint16_t lng)
{
	// expected e.g. EEPROM_R_ADR_000000_LNG_000012 (To read HelloWorld\r\n)
	// if was previously sucessfully done EEPROM_W_ADR_000000_HelloWorld\r\n

	uint8_t auxBuff[EEPROM_PAGE_SIZE] = {0};

	uint16_t subLng = 0;
	uint32_t addr = 0;
	uint32_t readLng = 0;
	uint32_t subResult = (uint32_t)(-1);

	ExtractValue(ESP_CMD_ADR, cmd, lng, &addr);
	ExtractValue(ESP_CMD_LNG, cmd, lng, &readLng);

	subLng = (readLng > EEPROM_PAGE_SIZE) ? EEPROM_PAGE_SIZE : readLng;

	while(readLng)
	{
		subResult = EEPROM_ReadData(addr, auxBuff, subLng);
    	readLng -= subLng;
    	printf("%s\r\n", (char*)auxBuff);
    	PLATFORM_DELAY_MS(100);
	}

	if(0 != subResult)
	{
    	printf(EEPROM_R_ERROR);
	}

	subResult = (subResult == 0) ? CMD_CUSTOM : (uint16_t)(-1);

	return subResult;
}

uint32_t ExtractValue(const uint8_t* const pKeyWord,
		const uint8_t* const pBUff,
		const uint32_t buffLng,
		uint32_t* val)
{
    const uint8_t* pBrowse = pBUff;

    uint32_t strLng = 0;
    uint32_t value = 0;
	uint32_t decOrder = 1;
	uint32_t keyWordFound = 0;

    /* Is keyword in pBUff ? */
    for (uint32_t idx = 0; idx < buffLng; idx++)
    {
        if (!memcmp(pBrowse, (char*)pKeyWord, strlen((char*)pKeyWord)))
        {
            keyWordFound = 1;
            break;
        }
        (char*)(pBrowse++);
    }

    //if ((!keyWordFound) || ((pBUff - pBrowse) >= buffLng))
    if (!keyWordFound)
    {
        /* Keyword is either not found or there is no data after keyword*/
        *val = 0;
        return 0;
    }

    /* Move pBrowse on the number */
    pBrowse = (uint8_t*)(pBrowse + strlen((char*)pKeyWord));

    for (uint32_t idx = 0; IS_NUM(pBrowse[idx]); idx++)
    {
        strLng++;
    }

    /* Create highest decimal order of the string */
    for (uint32_t idx = 0; idx < strLng - 1; idx++)
    {
        decOrder = decOrder * 10;
    }

    /* Go from left to right through the string and create the int value */
    for (uint32_t idx = 0; idx < strLng; idx++)
    {
        value += (pBrowse[idx] - '0') * decOrder;

        decOrder = decOrder / 10;
    }

    *val = value;

    return keyWordFound;
}
