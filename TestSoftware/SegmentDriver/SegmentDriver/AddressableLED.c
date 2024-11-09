#include "AddressableLED.h"
#include <memory.h>

// 100ns per bit, 1.2us, 12 bits per color bit. 8 bits per color, 12 bytes per color


static inline bitBuffer_24 twoBitsToBitBuffer(uint8_t bits)
{
	// 100ns per bit. 
	// 300ns high, 900ns low for low bit  3 bit high, 9 bit low
	// 900ns high, 300ns low for high bit. 9 bit high, 3 bit low
	static const uint8_t bitHigh_1 = 0x0F;
	static const uint8_t bitHigh_2 = 0x0F;
	static const uint8_t bitHigh_3 = 0x08;
	static const uint8_t bitLow_1 = 0x05;
	static const uint8_t bitLow_2 = 0x00;
	static const uint8_t bitLow_3 = 0x00;
	bitBuffer_24 buffer;
	switch (bits)
	{
	case 0x00:
		buffer.one = bitLow_1 << 4 | bitLow_2;
		buffer.two = bitLow_3 << 4 | bitLow_1;
		buffer.three = bitLow_2 << 4 | bitLow_3;
		break;
	case 0x01:
		buffer.one = bitLow_1 << 4 | bitLow_2;
		buffer.two = bitLow_3 << 4 | bitHigh_1;
		buffer.three = bitHigh_2 << 4 | bitHigh_3;
		break;
	case 0x02:
		buffer.one = bitHigh_3 << 4 | bitHigh_2;
		buffer.two = bitHigh_3 << 4 | bitLow_1;
		buffer.three = bitLow_2 << 4 | bitLow_3;
		break;
	case 0x03:
		buffer.one = bitHigh_1 << 4 | bitHigh_2;
		buffer.two = bitHigh_3 << 4 | bitHigh_1;
		buffer.three = bitHigh_2 << 4 | bitHigh_3;
		break;
	default:
		break;
	}
	return buffer;
}

// TODO below is broken.
static colorBuffer colorToBuffer(uint8_t colorByte)
{
	colorBuffer retVal = {0};
	uint8_t* retValBuf = (uint8_t*)&retVal;
 	for (int8_t i = 6; i >= 0; i-=2)
	{
		uint8_t twoBit = 0;
		bitBuffer_24 buffer = twoBitsToBitBuffer((uint8_t)(colorByte & (0x03 << i)) >> i);
		uint8_t bufSize = sizeof(buffer);
		memcpy(retValBuf, (uint8_t*)&buffer, sizeof(buffer));
		retValBuf+=3;
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
	colorBuffer red = colorToBuffer(color.R);
	memcpy(buffer, (uint8_t*)&red, sizeof(colorBuffer));
	buffer += sizeof(colorBuffer);
	writeCount += sizeof(colorBuffer);
	colorBuffer green = colorToBuffer(color.G);
	memcpy(buffer, (uint8_t*)&green, sizeof(colorBuffer));
	buffer += sizeof(colorBuffer);
	writeCount += sizeof(colorBuffer);
	colorBuffer blue = colorToBuffer(color.B);
	memcpy(buffer, (uint8_t*)&blue, sizeof(colorBuffer));
	writeCount += sizeof(colorBuffer);
#ifdef RGBW
	buffer += 4;
	uint32_t white = colorTo4Byte(color.W);
	memcpy(buffer, (uint8_t*)&white, sizeof(white));
	writeCount += 4;
#endif // RGBW
	return writeCount;
}