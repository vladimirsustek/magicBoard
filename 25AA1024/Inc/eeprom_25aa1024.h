/*
 * eeprom_25aa1024.h
 *
 *  Created on: Aug 28, 2022
 *      Author: 42077
 */

#ifndef EEPROM_25AA1024_H_
#define EEPROM_25AA1024_H_

#include "stdint.h"
#include "stdio.h"
#include "string.h"

#include "spi.h"

#define EEPROM_READ				(uint8_t)(0b00000011) // Read data from memory array beginning at the selected address
#define EEPROM_WRITE			(uint8_t)(0b00000010) // Write data to memory array beginning at the selected address
#define EEPROM_WREN				(uint8_t)(0b00000110) // Set the write enable latch (enable write operations)
#define EEPROM_WRDI				(uint8_t)(0b00000100) // Reset the write enable latch (enable write operations)
#define EEPROM_RDSR				(uint8_t)(0b00000101) // Read STATUS register
#define EEPROM_WRSR				(uint8_t)(0b00000001) // Write STATUS register
#define EEPROM_PE				(uint8_t)(0b01000010) // Page erase - erase one page memory array
#define EEPROM_SE				(uint8_t)(0b11011000) // Sector erase - erase one sector in memory array
#define EEPROM_CE				(uint8_t)(0b11000111) // Chip erase - erase all sectors in memory array
#define EEPROM_PDID				(uint8_t)(0b10101011) // Release from Deep Power-Down and Read Electronic signature
#define EEPROM_DPD				(uint8_t)(0b10111001) // Deep Power-Down mode


#define EEPROM_START_ADDRESS	(uint32_t)(0x00000000)
#define EEPROM_STOP_ADDRESS		(uint32_t)(0x0001FFFF)

#define MAX_WRITE_AT_ONCE		(uint16_t)(256u)

#define STATUS_PROTECTION_0		(uint8_t)((0) << 2)
#define STATUS_PROTECTION_1		(uint8_t)((1) << 2)
#define STATUS_PROTECTION_2		(uint8_t)((2) << 2)
#define STATUS_PROTECTION_3		(uint8_t)((3) << 2)

#define STATUS_WIP				(uint8_t)(1)
#define STATUS_WEN				(uint8_t)(2)

#define EEPROM_PAGE_SIZE		(uint16_t)(256u)

uint32_t EEPROM_Init(void);
uint32_t EEPROM_WriteData(uint32_t address, uint8_t* pData, uint16_t Size);
uint32_t EEPROM_ReadData(uint32_t address, uint8_t *pData, uint16_t Size);
uint32_t EEPROM_ReadStatusRegister(uint8_t *ret);
uint32_t EEPROM_WriteStatusRegister(uint8_t value);
#endif /* EEPROM_25AA1024_H_ */
