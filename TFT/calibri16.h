/*
 * calibri16.h
 *
 *  Created on: Jan 26, 2023
 *      Author: 42077
 */

#ifndef CALIBRI16_H_
#define CALIBRI16_H_

#include "fontDefs.h"

uint32_t sPrintCalibri16(uint32_t xOffset, uint32_t yOffset, uint32_t idx, uint32_t color);

extern const uint8_t calibri_16ptBitmaps[];
extern const FONT_CHAR_INFO calibri_16ptDescriptors[];

#endif /* CALIBRI16_H_ */
