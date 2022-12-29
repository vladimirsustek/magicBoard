/*
 * cmd_leds.h
 *
 *  Created on: Dec 28, 2022
 *      Author: 42077
 */

#ifndef INC_CMD_LEDS_H_
#define INC_CMD_LEDS_H_

#include "cmd_leds.h"

uint16_t CmdGreenLEDSetStat(const uint8_t* const pStrCmd, const uint16_t lng);
uint16_t CmdWhiteLEDSetStat(const uint8_t* const pStrCmd, const uint16_t lng);

#endif /* INC_CMD_LEDS_H_ */
