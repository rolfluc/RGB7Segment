#pragma once
#include <stdint.h>

void InitTimer();
void SendTimerDMA(uint32_t* buffer, uint16_t count);