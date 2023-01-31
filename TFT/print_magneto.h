#ifndef PRINT_MAGNETO_H_
#define PRINT_MAGNETO_H_

#include "magneto100.h"
#include "magneto60.h"

#include "print_magneto.h"
#include "tft.h"
#include "functions.h"

#define PRINT_COLOR					WHITE
#define ERASE_COLOR 				BLACK

/* Display text layout for magneto 100 text */
#define MAGNETO_100_LINE			(uint32_t)(130)
#define MAX_X 						(uint32_t)(480)
#define CLEARANCE 					(uint32_t)(5)
#define MAX_WIDTH 					(uint32_t)(92)
#define HEIGHT						(uint32_t)(140)
#define COLON_DIST					(uint32_t)(20)

#define MAGNETO_100_POS_0 			(uint32_t)(CLEARANCE)
#define MAGNETO_100_POS_1 			(uint32_t)(MAX_WIDTH + CLEARANCE)
#define MAGNETO_100_POS_2 			(uint32_t)(MAX_X - CLEARANCE*4 - MAX_WIDTH*2)
#define MAGNETO_100_POS_3 			(uint32_t)(MAX_X - CLEARANCE*2 - MAX_WIDTH)
#define POS_COLON					(uint32_t)(MAX_X/2)

#define DOT_1Y						(uint32_t)((MAGNETO_100_LINE+HEIGHT/2)+COLON_DIST)
#define DOT_1X						(uint32_t)(POS_COLON)
#define DOT_2Y						(uint32_t)((MAGNETO_100_LINE+HEIGHT/2)-COLON_DIST)
#define DOT_2X  					(uint32_t)(POS_COLON-COLON_DIST/2)

/* Magneto 40 defines */
#define MAGNETO_40_START			(uint32_t)(5)
#define MAGNETO_40_LINE_0			(uint32_t)(0)

void printMagnetoComa(void);
uint32_t printMagneto100(uint32_t xOffset, uint32_t yOffset, uint32_t idx);
uint32_t eraseMagneto100(uint32_t xOffset, uint32_t yOffset, uint32_t idx);
uint32_t printMagneto40(uint32_t xOffset, uint32_t yOffset, uint32_t idx);
uint32_t eraseMagneto40(uint32_t xOffset, uint32_t yOffset, uint32_t idx);
uint32_t printMagneto40Degree(uint32_t xOffset, uint32_t yOffset);
uint32_t eraseMagneto40Degree(uint32_t xOffset, uint32_t yOffset);

#endif /* PRINT_MAGNETO_H_ */
