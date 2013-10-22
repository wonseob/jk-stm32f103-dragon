

/*
Predefine ¼³Á¤ 
100p
STM32F103VCT6(512KB, 64KB) :  USE_STDPERIPH_DRIVER, STM32F10X_HD
STM32F105VCT6(512KB, 64KB) :  USE_STDPERIPH_DRIVER, STM32F10X_CL
STM32F107VBT6(256KB, 64KB) :  USE_STDPERIPH_DRIVER, STM32F10X_CL
STM32F107VCT6(256KB, 64KB) :  USE_STDPERIPH_DRIVER, STM32F10X_CL
STM32F107VBT6(128KB, 32KB) :  USE_STDPERIPH_DRIVER, STM32F10X_CL

144p
STM32F103ZET6(512KB, 64KB) :  USE_STDPERIPH_DRIVER, STM32F10X_HD
*/

/* Includes ------------------------------------------------------------------*/

#include "stm32f10x.h"
#include "hw_config.h"


int main(void)
{
	return 0;
}

		


#ifdef  USE_FULL_ASSERT
/*******************************************************************************
* Function Name  : assert_failed
* Description    : Reports the name of the source file and the source line number
*                  where the assert_param error has occurred.
* Input          : - file: pointer to the source file name
*                  - line: assert_param error line source number
* Output         : None
* Return         : None
*******************************************************************************/
void assert_failed(uint8_t* file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {}
}
#endif



/******************* (C) COPYRIGHT 2010 STMicroelectronics *****END OF FILE****/
