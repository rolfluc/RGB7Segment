#include <stm32g4xx_hal.h>
#include <stm32_hal_legacy.h>
//#include "SPI.h"
#include "Timer.h"
#include "SegmentInterface.h"
#include "LEDTiming.h"

TIM_HandleTypeDef htim1;

void SysTick_Handler(void)
{
	HAL_IncTick();
	HAL_SYSTICK_IRQHandler();
}

void SystemClock_Config(void)
{
	__HAL_RCC_SYSCFG_CLK_ENABLE();
	__HAL_RCC_PWR_CLK_ENABLE();
	RCC_OscInitTypeDef RCC_OscInitStruct = { 0 };
	RCC_ClkInitTypeDef RCC_ClkInitStruct = { 0 };

	/** Configure the main internal regulator output voltage
	*/
	HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1);

	/** Initializes the RCC Oscillators according to the specified parameters
	* in the RCC_OscInitTypeDef structure.
	*/
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
	RCC_OscInitStruct.HSIState = RCC_HSI_ON;
	RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
	RCC_OscInitStruct.PLL.PLLM = RCC_PLLM_DIV1;
	RCC_OscInitStruct.PLL.PLLN = 8;
	RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
	RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
	RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
	{
		__ASM("BKPT 255");
	}

	/** Initializes the CPU, AHB and APB buses clocks
	*/
	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
	                            | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
	{
		__ASM("BKPT 255");
	}
}

//uint16_t tmpBuffer[4] = { 19, 57, 19, 57};
uint16_t tmpBuffer[4] = { BIT_LOW_COUNTS, BIT_HIGH_COUNTS, BIT_LOW_COUNTS, BIT_HIGH_COUNTS};

int main(void)
{
	HAL_Init();
	SystemClock_Config();

	InitTimer();
	SegmentVal v0 = GetSegmentForInt(3);
	SegmentVal v1 = GetSegmentForInt(6);
	Color c0 = { 0x00, 0x00, 0x00 };
	Color c1 = { 0x00, 0x00, 0x00 };

	for (;;)
	{
		// TODO something dumb but an idea. Right now requires 16 bits, per bit. So for 1 LED, you need 24x16 bits, so 48 bytes per LED.
		// One idea, DMA 8bit->16 bit.
		SendTimerDMA((uint8_t*)&tmpBuffer, 4*sizeof(uint16_t));
		//SetDisplay(v0, c0, v1, c1);
		HAL_Delay(500);
	}
}
