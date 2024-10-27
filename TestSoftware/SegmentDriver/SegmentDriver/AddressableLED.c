#include "AddressableLED.h"
#include <memory.h>

static uint32_t colorTo4Byte(uint8_t colorByte)
{
	static const uint8_t bitHigh = 0x0E;
	static const uint8_t bitLow = 0x08;
	uint32_t retVal = 0;
	for (uint8_t i = 7; i > 0; i--)
	{
		// Check the topmost bit, remove 1 for next call, load the nibble to the right location.
		uint8_t tmpByte = ((colorByte & (1 << i--)) == 0) ? (bitLow << 4) : (bitHigh << 4);
		tmpByte |= ((colorByte & (1 << i)) == 0) ? bitLow : bitHigh;
		retVal |= tmpByte << (i * 4);
	}
	return retVal;
}


uint8_t FillColor(uint8_t* buffer, Color color)
{
	// In the case of RGB, 3 bytes, 8 bits per byte, 24 bits.
	uint8_t writeCount = 0;
	// Operates on the assumption that NUM_SPI_BITS_PER_COLOR_BIT == 4
	// each bit gets converted to a nibble.
	// each byte gets converted to 4 bytes
	uint32_t red = colorTo4Byte(color.R);
	memcpy(buffer, (uint8_t*)&red, sizeof(uint32_t));
	buffer += 4;
	writeCount += 4;
	uint32_t green = colorTo4Byte(color.G);
	memcpy(buffer, (uint8_t*)&green, sizeof(uint32_t));
	buffer += 4;
	writeCount += 4;
	uint32_t blue = colorTo4Byte(color.B);
	memcpy(buffer, (uint8_t*)&red, sizeof(uint32_t));
	writeCount += 4;
#ifdef RGBW
	buffer += 4;
	uint32_t white = colorTo4Byte(color.W);
	memcpy(buffer, (uint8_t*)&white, sizeof(white));
	writeCount += 4;
#endif // RGBW
	return writeCount;
}