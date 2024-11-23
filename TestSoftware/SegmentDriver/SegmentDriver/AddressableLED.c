#include "AddressableLED.h"
#include <memory.h>
#include "LEDTiming.h"


colorBuffer byteToBuffer(uint8_t colorByte)
{
	static const uint16_t bitHigh = BIT_HIGH_COUNTS;
	static const uint16_t bitLow = BIT_LOW_COUNTS;
	colorBuffer buffer;
	// Preserve endianess
	for (int8_t i = 7; i > 0; i--)
	{
		
	}
}

uint8_t FillColor(uint8_t* buffer, Color color)
{
	
	uint8_t writeCount = 0;
	
#ifdef RGBW
#endif // RGBW
	return writeCount;
}