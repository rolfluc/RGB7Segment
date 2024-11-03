#include "SPI.h"
static bool _dmaRunning = false;

// TODO DMA CB set false above
SPI_HandleTypeDef spi1;
DMA_HandleTypeDef spi1dma;
DMA_Channel_TypeDef sp1dmachannel;

void HAL_SPI_TxCpltCallback(SPI_HandleTypeDef * hspi)
{
	_dmaRunning = false;
}

void SPI1_IRQHandler(void)
{
	HAL_SPI_IRQHandler(&spi1);
}

void DMA1_Channel2_3_IRQHandler(void)
{
	HAL_DMA_IRQHandler(&spi1dma);
}

static void _InitSPIPins()
{
	GPIO_InitTypeDef txPin;
	txPin.Pin = GPIO_PIN_7;
	txPin.Mode = GPIO_MODE_AF_PP;
	txPin.Pull = GPIO_NOPULL;
	txPin.Alternate = GPIO_AF0_SPI1; // TODO confirm
	txPin.Speed = GPIO_SPEED_FREQ_HIGH; // TODO 
	HAL_GPIO_Init(GPIOA, &txPin);
}

static void _InitSPIDMA()
{
	__DMA1_CLK_ENABLE();
	__HAL_RCC_DMA1_CLK_ENABLE();
	spi1dma.Instance = DMA1_Channel3;
	spi1dma.Init.Direction = DMA_MEMORY_TO_PERIPH;
	spi1dma.Init.PeriphInc = DMA_PINC_DISABLE;
	spi1dma.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
	spi1dma.Init.Mode = DMA_NORMAL;
	spi1dma.Init.Priority = DMA_PRIORITY_MEDIUM;
	spi1dma.Init.MemInc = DMA_MINC_ENABLE;
	spi1dma.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
	
	HAL_DMA_Init(&spi1dma);
	__HAL_LINKDMA(&spi1, hdmatx, spi1dma);
	HAL_NVIC_SetPriority(DMA1_Channel2_3_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(DMA1_Channel2_3_IRQn);
}

void InitSPI()
{
	_InitSPIPins();
	__SPI1_CLK_ENABLE();
	__HAL_RCC_SPI1_CLK_ENABLE();
	_InitSPIDMA();
	spi1.Instance = SPI1;
	spi1.Init.Mode = SPI_MODE_MASTER;
	spi1.Init.Direction = SPI_DIRECTION_1LINE;
	spi1.Init.DataSize = SPI_DATASIZE_8BIT;
	spi1.Init.CLKPolarity = SPI_POLARITY_LOW;
	spi1.Init.CLKPhase = SPI_PHASE_2EDGE;
	spi1.Init.NSS = SPI_NSS_SOFT;
	spi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
	spi1.Init.TIMode = SPI_TIMODE_DISABLE;
	spi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
	spi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_4; // TODO this matters a lot here.
	spi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
	spi1.Init.CRCPolynomial = 7;
	spi1.Init.CRCLength = SPI_CRC_LENGTH_DATASIZE;
	spi1.Init.NSSPMode = SPI_NSS_PULSE_ENABLE;
	HAL_SPI_Init(&spi1);
	HAL_NVIC_SetPriority(SPI1_IRQn, 2, 0);
	HAL_NVIC_EnableIRQ(SPI1_IRQn);
}

void SendSPIDMA(uint8_t* data, uint8_t bytes)
{
	HAL_SPI_Transmit_DMA(&spi1, data, bytes);
	_dmaRunning = true;
}

// Returns true when dma Is not running.
bool isDMAFinished()
{
	return !_dmaRunning;
}
