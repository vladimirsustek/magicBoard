/*
 * fontDefs.h
 *
 *  Created on: Jan 26, 2023
 *      Author: 42077
 */

#ifndef FONTDEFS_H_
#define FONTDEFS_H_

#include <stdint.h>

typedef struct FONT_CHAR
{
	uint32_t width;
	uint32_t offset;
} FONT_CHAR_INFO;

typedef struct S_FONT_INFO
{
	uint8_t height;
	uint8_t start;
	uint8_t end;
	uint8_t width;
	const uint8_t * descriptors;
	const uint8_t * bitmaps;
}FONT_INFO;

#define ASCII_OFFSET (uint32_t)(33)

#endif /* FONTDEFS_H_ */
