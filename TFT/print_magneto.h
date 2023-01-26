/*
 * print_magneto.h
 *
 *  Created on: Jan 26, 2023
 *      Author: 42077
 */

#ifndef PRINT_MAGNETO_H_
#define PRINT_MAGNETO_H_

#include <magneto100.h>
#include <magneto60.h>
#include "print_magneto.h"
#include "tft.h"
#include "functions.h"

#define PRINT_COLOR			WHITE
#define ERASE_COLOR 		BLACK

#define TEXT_OFFSET			(130)
#define MAX_X 				(480)
#define CLEARANCE 			(5)
#define MAX_WIDTH 			(92)
#define HEIGHT				(140)
#define COLON_DIST			(20)

#define POS_0 				(CLEARANCE)
#define POS_1 				(MAX_WIDTH + CLEARANCE)
#define POS_2 				(MAX_X - CLEARANCE*4 - MAX_WIDTH*2)
#define POS_3 				(MAX_X - CLEARANCE*2 - MAX_WIDTH)
#define POS_COLON			(MAX_X/2)

#define DOT_1Y				((TEXT_OFFSET+HEIGHT/2)-COLON_DIST)
#define DOT_1X				(POS_COLON)
#define DOT_2Y				((TEXT_OFFSET+HEIGHT/2)+COLON_DIST)
#define DOT_2X  			(POS_COLON-COLON_DIST/2)

uint32_t printMagneto100(uint32_t xOffset, uint32_t yOffset, uint32_t idx);
uint32_t eraseMagneto100(uint32_t xOffset, uint32_t yOffset, uint32_t idx);
uint32_t printMagneto40(uint32_t xOffset, uint32_t yOffset, uint32_t idx);
uint32_t eraseMagneto40(uint32_t xOffset, uint32_t yOffset, uint32_t idx);
uint32_t printMagneto40Degree(uint32_t xOffset, uint32_t yOffset);
uint32_t eraseMagneto40Degree(uint32_t xOffset, uint32_t yOffset);

#endif /* PRINT_MAGNETO_H_ */
