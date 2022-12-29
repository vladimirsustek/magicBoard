/*
 * cmd_leds.c
 *
 *  Created on: Dec 28, 2022
 *      Author: 42077
 */

#include "gpio.h"
#include "cmd_defs.h"

uint16_t CmdGreenLEDSetStat(const uint8_t* const pStrCmd, const uint16_t lng)
{
    if ((CMD_METHOD_LNG + CMD_NAME_LNG +
         CMD_DELIMITER_LNG*2 + CMD_ARG1_LNG+
         CMD_EOL_LNG) != lng)
    {

        return CMD_RET_ERR;
    }

    uint32_t result = CMD_RET_ERR;
    uint8_t state = pStrCmd[CMD_ARG_OFFSET + 0];

    if (state == '1' || state == '0')
    {
    	HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, (GPIO_PinState)(state-'0'));
    	result = CMD_RET_OK;
    }
    return result;
}

uint16_t CmdWhiteLEDSetStat(const uint8_t* const pStrCmd, const uint16_t lng)
{
    if ((CMD_METHOD_LNG + CMD_NAME_LNG +
         CMD_DELIMITER_LNG*2 + CMD_ARG1_LNG+
         CMD_EOL_LNG) != lng)
    {

        return CMD_RET_ERR;
    }

    uint32_t result = CMD_RET_ERR;
    uint8_t state = pStrCmd[CMD_ARG_OFFSET + 0];

    if (state == '1' || state == '0')
    {
    	HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, (GPIO_PinState)(state-'0'));
    	result = CMD_RET_OK;
    }
    return result;
}
