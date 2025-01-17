#include "AddressableLED.h"
#include <memory.h>
#include "LEDTiming.h"


void byteToBuffer(colorBuffer* buffer, uint8_t colorByte)
{
	
}

void FillColor(PaddedColor* buffer, Color color)
{
	byteToBuffer(&(buffer->R), color.R);
	byteToBuffer(&(buffer->G), color.G);
	byteToBuffer(&(buffer->B), color.B);
}