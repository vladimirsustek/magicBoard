#include <string.h>
#include <stdio.h>

#define NO_FLASH_BUT_PRINT 0

#include "flash_nvm.h"
/* RM410 - STM32F67 reference manual page 88-90 */
#define FLASH_SB_NVM_BEGIN 			(uint32_t)0x081C0000
#define FLASH_SB_NVM_END			(uint32_t)0x081FFFFF
#define FLASH_SB_NVM_SIZE			(uint32_t)0x40000
#define FLASH_SB_NVM_SECTOR    		(uint32_t)11
#define FLASH_SB_NVM_SECTOR_SIZE  	(uint32_t)0x40000

#define FLASH_DB_NVM_BEGIN 			(uint32_t)0x081E0000
#define FLASH_DB_NVM_END			(uint32_t)0x081FFFFF
#define FLASH_DB_NVM_SIZE			(uint32_t)0x20000
#define FLASH_DB_NVM_SECTOR    		(uint32_t)23
#define FLASH_DB_NVM_SECTOR_SIZE  	(uint32_t)0x20000

#define FLASH_NVM_NB_SECTORS    	(uint32_t)1

/* RM410 - STM32F767 reference manual page 100*/
#define FLASH_OPTION_BYTES_2_ADR (uint32_t)(0x1FFF0008)
#define SINGLE_BANK 			 (uint32_t)1
#define DUAL_BANK 				 (uint32_t)2
#define NBANK 					 (uint32_t)(1 << 13)

#define IS_ADR_4B_ALIGNED(adr)	 (0==(adr)%4)

static uint32_t bank_mode = 0;

bool static flash_valid_range(uint32_t offset, uint32_t bytes)
{
	bool result = false;

	if(bank_mode == SINGLE_BANK)
	{
		if(FLASH_DB_NVM_BEGIN + offset + bytes > FLASH_SB_NVM_END)
		{
			result = false;
		}
		else
		{
			result = true;
		}
	}
	else
	{
		if(FLASH_DB_NVM_BEGIN + offset + bytes > FLASH_DB_NVM_END)
		{
			result = false;
		}
		else
		{
			result = true;
		}
	}

	return result;
}

bool flash_nvm_open()
{
	uint32_t option2 = *((uint32_t*)(0x1FFF0008));

	if(option2 & NBANK)
	{
		bank_mode = SINGLE_BANK;
	}
	else
	{
		bank_mode = DUAL_BANK;
	}
	return true;
}

bool flash_nvm_write(uint32_t offset, uint32_t* pData, uint32_t length)
{
	if(!flash_valid_range(offset, length) || pData == NULL ||
			!IS_ADR_4B_ALIGNED(offset))
	{
		return false;
	}

	/* Erase the flash prior write  ------------------------------ */
	FLASH_EraseInitTypeDef eraseInit;
	HAL_StatusTypeDef result;
	uint32_t sectorError;
	uint32_t adr;

	if(bank_mode == SINGLE_BANK)
	{
		adr = FLASH_SB_NVM_BEGIN;
		eraseInit.Sector = FLASH_SB_NVM_SECTOR;
	}
	else
	{
		adr = FLASH_DB_NVM_BEGIN;
		eraseInit.Sector = FLASH_DB_NVM_SECTOR;
	}

	adr += offset;

	eraseInit.TypeErase = FLASH_TYPEERASE_SECTORS;
	eraseInit.Banks = 0; /* Not used in sector erase */
	/* TODO: add argument to check VDDA voltage */
	eraseInit.VoltageRange = FLASH_VOLTAGE_RANGE_4;
	eraseInit.NbSectors = FLASH_NVM_NB_SECTORS;

	HAL_FLASH_Unlock();

	/* End of erase prior write  ------------------------------ */
	if( HAL_OK != HAL_FLASHEx_Erase(&eraseInit, &sectorError))
	{
		  return HAL_FLASH_GetError ();
	}

	uint32_t idx = 0;
	while(length)
	{
		uint64_t to_write = (uint64_t)pData[idx];

		if(HAL_OK == HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, adr, to_write))
		{
			adr += sizeof(uint32_t);
			idx++;
			length--;
		}
		else
		{
			HAL_FLASH_GetError();
		}

	}
	HAL_FLASH_Lock();

	return true;
}
