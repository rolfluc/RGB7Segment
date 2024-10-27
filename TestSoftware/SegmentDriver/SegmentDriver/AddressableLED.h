#pragma once
#include <stdint.h>
#include "Color.h"

// This is the amount of time 1 bit consumes, i.e. 8 * this number should give how long 1 byte takes to transfer.
// i.e. if SPI clock is 1 Mhz, 1 bit time is 1us, or 1000ns
// SPI clock is 10 Mhz, bit time is 100ns
// 333ns is 3 Mhz clock
// TODO This codebase assumes a minimum SPI clock of 3 Mhz. Might not be terrible good if not 3 Mhz, since 2 color bits are 1 SPI byte. Truncating off 3_33
#define SPI_BIT_TIME_100ns 3
#define BIT_SET_TIME_100ns 9 // TODO bad name.
#define BIT_UNSET_TIME_100ns 3 // TODO bad name.
#define NUM_SPI_BITS_PER_COLOR_BIT ((BIT_SET_TIME_ns + BIT_UNSET_TIME_ns) / SPI_BIT_TIME_ns)

// Places the respective color at the prescribed location. Assumes the buffer has been allocated.
// Input: Buffer where the data will be stored.
// Input: Color: The RGB/W color intended to be placed into the buffer
// Output: Bytes Written
uint8_t FillColor(uint8_t* buffer, Color color);