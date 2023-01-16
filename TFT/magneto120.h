/*
 * magneto150.h
 *
 *  Created on: Jan 16, 2023
 *      Author: 42077
 */

#ifndef MAGNETO120_H_
#define MAGNETO120_H_

#include <stdint.h>


typedef struct FONT_CHAR
{
	uint32_t width;
	uint32_t offset;
} FONT_CHAR_INFO;

typedef uint8_t uint_8;

extern const FONT_CHAR_INFO magneto_120ptDescriptors[];
extern const uint_8 magneto_120ptBitmaps[];

#endif /* MAGNETO120_H_ */
