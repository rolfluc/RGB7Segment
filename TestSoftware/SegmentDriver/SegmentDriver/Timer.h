#pragma once
#include <stdint.h>

void InitTimer();
void SendTimerDMA(uint8_t* buffer, uint16_t count);