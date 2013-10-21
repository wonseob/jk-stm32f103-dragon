
#include "hw_config.h"
#include "dac.h"

#define DAC_DHR8R1_Address      0x40007410
uc8 Escalator8bit[6] = {0x0, 0x33, 0x66, 0x99, 0xCC, 0xFF};

void bsp_dac_gpio_init(void) 
{
	GPIO_InitTypeDef GPIO_InitStructure;						 				
	
	/* Configure DAC channe1 and DAC channel2 outputs pins */
  	GPIO_InitStructure.GPIO_Pin =  DAC_OUT1_PIN;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  	GPIO_Init(DAC_OUT1_PORT, &GPIO_InitStructure);
}

void bsp_dac_init(void) 
{
	DAC_InitTypeDef            DAC_InitStructure;
	DMA_InitTypeDef            DMA_InitStructure;


	bsp_dac_gpio_init();

#if 1
	/* TIM6 Configuration */
	TIM_PrescalerConfig(TIM6, 0xF, TIM_PSCReloadMode_Update);
	TIM_SetAutoreload(TIM6, 0xFF);
	/* TIM6 TRGO selection */
	TIM_SelectOutputTrigger(TIM6, TIM_TRGOSource_Update);

	/* DAC channel1 Configuration */
	DAC_InitStructure.DAC_Trigger = DAC_Trigger_T6_TRGO;
	DAC_InitStructure.DAC_WaveGeneration = DAC_WaveGeneration_None;
	DAC_InitStructure.DAC_OutputBuffer = DAC_OutputBuffer_Disable;
	DAC_Init(DAC_Channel_1, &DAC_InitStructure);

	/* DMA2 channel3 configuration */
	DMA_DeInit(DMA2_Channel3);
	DMA_InitStructure.DMA_PeripheralBaseAddr = DAC_DHR8R1_Address;
	DMA_InitStructure.DMA_MemoryBaseAddr = (u32)&Escalator8bit;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
	DMA_InitStructure.DMA_BufferSize = 6;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	DMA_Init(DMA2_Channel3, &DMA_InitStructure);
	
	/* Enable DMA2 Channel3 */
	DMA_Cmd(DMA2_Channel3, ENABLE);
	
	/* Enable DAC Channel1 */
	DAC_Cmd(DAC_Channel_1, ENABLE);
	
	/* Enable DMA for DAC Channel1 */
	DAC_DMACmd(DAC_Channel_1, ENABLE);
	
	/* TIM6 enable counter */
	TIM_Cmd(TIM6, ENABLE);
#endif

#if 0
	/* DAC channel1 Configuration */
	DAC_InitStructure.DAC_Trigger = DAC_Trigger_Software;
	DAC_InitStructure.DAC_WaveGeneration = DAC_WaveGeneration_Noise;
	DAC_InitStructure.DAC_LFSRUnmask_TriangleAmplitude = DAC_LFSRUnmask_Bits8_0;
	DAC_InitStructure.DAC_OutputBuffer = DAC_OutputBuffer_Enable;
	DAC_Init(DAC_Channel_1, &DAC_InitStructure);

	
	/* Enable DAC Channel1 */
	DAC_Cmd(DAC_Channel_1, ENABLE);
	
	/* Set DAC Channel1 DHR12L register */
	DAC_SetChannel1Data(DAC_Align_12b_L, 0x7FF0);
#endif
}


