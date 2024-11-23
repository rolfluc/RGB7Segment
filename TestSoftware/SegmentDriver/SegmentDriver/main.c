#include <stm32g4xx_hal.h>
#include <stm32_hal_legacy.h>
//#include "SPI.h"
#include "Timer.h"
#include "SegmentInterface.h"

TIM_HandleTypeDef htim1;

void SysTick_Handler(void)
{
	HAL_IncTick();
	HAL_SYSTICK_IRQHandler();
}

#if 0
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if (htim->Instance == TIM1) {
		HAL_IncTick();
	}
}

void TIM1_BRK_UP_TRG_COM_IRQHandler(void)
{
	HAL_TIM_IRQHandler(&htim1);
}

HAL_StatusTypeDef HAL_InitTick(uint32_t TickPriority)
{
	RCC_ClkInitTypeDef    clkconfig;
	uint32_t              uwTimclock = 0;
	uint32_t              uwPrescalerValue = 0;
	uint32_t              pFLatency;
	/*Configure the TIM1 IRQ priority */
	HAL_NVIC_SetPriority(TIM1_BRK_UP_TRG_COM_IRQn, TickPriority, 0);

	/* Enable the TIM1 global Interrupt */
	HAL_NVIC_EnableIRQ(TIM1_BRK_UP_TRG_COM_IRQn);

	/* Enable TIM1 clock */
	__HAL_RCC_TIM1_CLK_ENABLE();

	/* Get clock configuration */
	HAL_RCC_GetClockConfig(&clkconfig, &pFLatency);

	/* Compute TIM1 clock */
	uwTimclock = HAL_RCC_GetPCLK1Freq();
	/* Compute the prescaler value to have TIM1 counter clock equal to 1MHz */
	uwPrescalerValue = (uint32_t)((uwTimclock / 1000000U) - 1U);

	/* Initialize TIM1 */
	htim1.Instance = TIM1;

	/* Initialize TIMx peripheral as follow:
	+ Period = [(TIM1CLK/1000) - 1]. to have a (1/1000) s time base.
	+ Prescaler = (uwTimclock/1000000 - 1) to have a 1MHz counter clock.
	+ ClockDivision = 0
	+ Counter direction = Up
	*/
	htim1.Init.Period = (1000000U / 1000U) - 1U;
	htim1.Init.Prescaler = uwPrescalerValue;
	htim1.Init.ClockDivision = 0;
	htim1.Init.CounterMode = TIM_COUNTERMODE_UP;

	if (HAL_TIM_Base_Init(&htim1) == HAL_OK)
	{
		/* Start the TIM time Base generation in interrupt mode */
		return HAL_TIM_Base_Start_IT(&htim1);
	}

	/* Return function status */
	return HAL_ERROR;
}

/**
  * @brief  Suspend Tick increment.
  * @note   Disable the tick increment by disabling TIM1 update interrupt.
  * @param  None
  * @retval None
  */
void HAL_SuspendTick(void)
{
	/* Disable TIM1 update Interrupt */
	__HAL_TIM_DISABLE_IT(&htim1, TIM_IT_UPDATE);
}

/**
  * @brief  Resume Tick increment.
  * @note   Enable the tick increment by Enabling TIM1 update interrupt.
  * @param  None
  * @retval None
  */
void HAL_ResumeTick(void)
{
	/* Enable TIM1 Update interrupt */
	__HAL_TIM_ENABLE_IT(&htim1, TIM_IT_UPDATE);
}
#endif

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

uint16_t tmpBuffer[4] = { 19, 57, 19, 57};

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
		SendTimerDMA((uint8_t*)&tmpBuffer, 4*sizeof(uint16_t));
		//SetDisplay(v0, c0, v1, c1);
		HAL_Delay(500);
	}
}
