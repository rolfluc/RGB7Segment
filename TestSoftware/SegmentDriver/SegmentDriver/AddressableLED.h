#pragma once
#include <stdint.h>
#include "Color.h"

// Need 1 half word, per color bit.
// 1 LED Color is 24 half words, so 48 bytes.
#define BUFFER_SIZE 8
typedef struct 
{
	uint16_t buffer[BUFFER_SIZE];
}colorBuffer;

// Places the respective color at the prescribed location. Assumes the buffer has been allocated.
// Input: Buffer where the data will be stored.
// Input: Color: The RGB/W color intended to be placed into the buffer
// Output: Bytes Written
uint8_t FillColor(uint8_t* buffer, Color color);