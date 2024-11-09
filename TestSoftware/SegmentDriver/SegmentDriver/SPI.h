#pragma once
#include <stdbool.h>
#include "stm32f0xx.h"
void InitSPI();
void SendSPIDMA(uint8_t* data, uint16_t bytes);
bool isDMAFinished();