/*
 * magneto150.h
 *
 *  Created on: Jan 16, 2023
 *      Author: 42077
 */

#ifndef MAGNETO100_H_
#define MAGNETO100_H_

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

typedef uint8_t uint_8;

extern const FONT_CHAR_INFO magneto_100ptDescriptors[];
extern const uint_8 magneto_100ptBitmaps[];

extern const FONT_CHAR_INFO magneto_60ptDescriptors[];
extern const uint_8 magneto_60ptBitmaps[];

#endif /* MAGNETO100_H_ */
