#include "print_magneto.h"

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
