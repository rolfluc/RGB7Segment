#pragma once
#include <stdint.h>
#include "Color.h"

typedef struct 
{
	uint8_t one;
	uint8_t two;
	uint8_t three;
}bitBuffer_24;

#define BUFFER_SIZE 12
typedef struct 
{
	uint8_t buffer[BUFFER_SIZE];
}colorBuffer;

// Places the respective color at the prescribed location. Assumes the buffer has been allocated.
// Input: Buffer where the data will be stored.
// Input: Color: The RGB/W color intended to be placed into the buffer
// Output: Bytes Written
uint8_t FillColor(uint8_t* buffer, Color color);