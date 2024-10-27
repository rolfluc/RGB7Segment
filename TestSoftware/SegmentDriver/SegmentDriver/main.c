#include <stm32f0xx_hal.h>
#include <stm32_hal_legacy.h>
#include "SPI.h"
#include "SegmentInterface.h"

#ifdef __cplusplus
extern "C"
#endif
void SysTick_Handler(void)
{
	HAL_IncTick();
	HAL_SYSTICK_IRQHandler();
}

int main(void)
{
	HAL_Init();

	__GPIOA_CLK_ENABLE();
	InitSPI();
	SegmentVal v0 = GetSegmentForInt(3);
	SegmentVal v1 = GetSegmentForInt(6);
	Color c0 = { 0x0f, 0x0f, 0x0f };
	Color c1 = { 0x3f, 0x00, 0x00 };

	for (;;)
	{
		SetDisplay(v0, c0, v1, c1);
		HAL_Delay(500);
	}
}
