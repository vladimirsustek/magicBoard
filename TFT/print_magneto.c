#include "print_magneto.h"

/* Functions:
 *
 * - printMagneto100()
 * - eraseMagneto100()
 * - printMagneto40()
 * - eraseMagneto40()
 * - printMagneto40Degree()
 * - eraseMagneto40Degree()
 *
 * are customized for 480*320 display which is portrait oriented
 * by default (x dimension shorter than y), but display is flipped
 * to landscape. Each font character is print pixel by pixel using
 * drawPixel(x, y, color) and due to landscape orientation is used
 * as drawPixel(y, x, color).
 *
 * print are always WHITE, erase is basically just print BLACK.
 *
 * printMagneto100() and eraseMagneto100() expects coordinates x, y
 * thus user must calculate, where the character is print and also
 * must count with offset for adjacent (before or after) print symbols.
 *
 *
 * magneto_100ptBitmaps[]
 * magneto_40ptBitmaps[]
 *
 * magneto_100ptDescriptors[]
 * magneto_40ptDescriptors[]
 *
 * */

static uint32_t sPrintMagneto100(uint32_t xOffset, uint32_t yOffset, uint32_t idx, uint32_t color)
{
	uint16_t adrOffset = magneto_100ptDescriptors[idx].offset;
	uint32_t width = magneto_100ptDescriptors[idx].width;
	uint32_t righAlignOffset = MAX_WIDTH - magneto_100ptDescriptors[idx].width;

	/* Get align width in bytes*/
	width = (width % 8) ? width / 8 + 1 : width / 8;

	uint32_t height = (magneto_100ptDescriptors[idx + 1].offset - adrOffset)/width;
	uint8_t inspectedByte = 0;
	uint8_t inspectedBit = 0;

	  for(uint32_t line = 0; line < height; line++)
	  {
		  for(uint32_t pixel = 0; pixel < width*8; pixel++)
		  {
			  if(pixel % 8 == 0)
			  {
				  inspectedByte = magneto_100ptBitmaps[adrOffset + line* width + pixel / 8];
			  }

			  inspectedBit = inspectedByte & (1 << (7 - (pixel % 8)));
			  inspectedBit = inspectedBit >> (7 - (pixel % 8));

			  if(inspectedBit)
			  {
				  drawPixel(height - line + yOffset, xOffset + pixel + righAlignOffset, color);
			  }
		  }
	  }
	  return magneto_100ptDescriptors[idx].width;

}

static uint32_t sPrintMagneto60(uint32_t xOffset, uint32_t yOffset, uint32_t idx, uint16_t color)
{
	uint32_t adrOffset = magneto_60ptDescriptors[idx].offset;
	uint32_t width = magneto_60ptDescriptors[idx].width;

	/* Get align width in bytes*/
	width = (width % 8) ? width / 8 + 1 : width / 8;

	uint32_t height = (magneto_60ptDescriptors[idx + 1].offset - adrOffset)/width;
	uint8_t inspectedByte = 0;
	uint8_t inspectedBit = 0;

	  for(uint32_t line = 0; line < height; line++)
	  {
		  for(uint32_t pixel = 0; pixel < width*8; pixel++)
		  {
			  if(pixel % 8 == 0)
			  {
				  inspectedByte = magneto_60ptBitmaps[adrOffset + line* width + pixel / 8];
			  }

			  inspectedBit = inspectedByte & (1 << (7 - (pixel % 8)));
			  inspectedBit = inspectedBit >> (7 - (pixel % 8));

			  if(inspectedBit)
			  {
				  drawPixel(height - line + yOffset, pixel + xOffset, color);
			  }
		  }
	  }

	  return magneto_60ptDescriptors[idx].width;
}

void printMagnetoComa(void)
{
	  fillCircle(DOT_1Y, DOT_1X, 10, WHITE);
	  fillCircle(DOT_2Y, DOT_2X, 10, WHITE);
}

uint32_t printMagneto100(uint32_t xOffset, uint32_t yOffset, uint32_t idx)
{
	return sPrintMagneto100(xOffset, yOffset, idx, PRINT_COLOR);
}

uint32_t eraseMagneto100(uint32_t xOffset, uint32_t yOffset, uint32_t idx)
{
	return sPrintMagneto100(xOffset, yOffset, idx, ERASE_COLOR);
}

uint32_t printMagneto40(uint32_t xOffset, uint32_t yOffset, uint32_t idx)
{
	return sPrintMagneto60(xOffset, yOffset, idx, PRINT_COLOR);
}

uint32_t eraseMagneto40(uint32_t xOffset, uint32_t yOffset, uint32_t idx)
{
	return sPrintMagneto60(xOffset, xOffset, idx, ERASE_COLOR);
}

uint32_t printMagneto40Degree(uint32_t xOffset, uint32_t yOffset)
{
	fillCircle(yOffset + 70, xOffset + 15, 10, PRINT_COLOR);
	fillCircle(yOffset + 70, xOffset + 15, 4, ERASE_COLOR);
	return 35;
}

uint32_t eraseMagneto40Degree(uint32_t yOffset, uint32_t xOffset)
{
	fillCircle(yOffset + 70, xOffset + 15, 10, ERASE_COLOR);
	return 35;
}
