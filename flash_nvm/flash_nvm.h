#include <stdint.h>
#include <stdbool.h>

#include "stm32f7xx_hal.h"

bool flash_nvm_open();
bool flash_nvm_read(uint32_t offset, uint32_t* pData, uint32_t length);
bool flash_nvm_write(uint32_t offset, uint32_t* pData, uint32_t length);
