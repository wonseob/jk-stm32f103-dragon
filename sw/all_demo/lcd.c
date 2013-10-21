
#include  <stm32f10x.h>
#include "hw_config.h"
#include "lcd.h" 

tft_lcd_type curr_tft_lcd = tft43_lcd;
u16 lcd_id = 0x00;	 

unsigned int  POINT_COLOR=RED;
unsigned int  BACK_COLOR=0XFFFF;


/**
  * @brief  Configures the FSMC and GPIOs to interface with the SRAM memory.
  *         This function must be called before any write/read operation
  *         on the SRAM.
  * @param  None 
  * @retval : None
  */
void bsp_fsmc_lcd_init(void)
{
	FSMC_NORSRAMInitTypeDef  FSMC_NORSRAMInitStructure;
	FSMC_NORSRAMTimingInitTypeDef  p;
	
#if 1
	p.FSMC_AddressSetupTime = 0x02;
	p.FSMC_AddressHoldTime = 0x00;
	p.FSMC_DataSetupTime = 0x05;
	p.FSMC_BusTurnAroundDuration = 0x00;
	p.FSMC_CLKDivision = 0x00;
	p.FSMC_DataLatency = 0x00;
	p.FSMC_AccessMode = FSMC_AccessMode_B;
#endif
	
#if 0
	p.FSMC_AddressSetupTime = 0x02;
	p.FSMC_AddressHoldTime = 0x00;
	p.FSMC_DataSetupTime = 0x05;
	p.FSMC_BusTurnAroundDuration = 0x00;
	p.FSMC_CLKDivision = 0x02;
	p.FSMC_DataLatency = 0x100;
	p.FSMC_AccessMode = FSMC_AccessMode_B;
#endif
	
	FSMC_NORSRAMInitStructure.FSMC_Bank = FSMC_Bank1_NORSRAM1;
	FSMC_NORSRAMInitStructure.FSMC_DataAddressMux = FSMC_DataAddressMux_Disable;
	FSMC_NORSRAMInitStructure.FSMC_MemoryType = FSMC_MemoryType_NOR;
	FSMC_NORSRAMInitStructure.FSMC_MemoryDataWidth = FSMC_MemoryDataWidth_16b;
	FSMC_NORSRAMInitStructure.FSMC_BurstAccessMode = FSMC_BurstAccessMode_Disable;
	FSMC_NORSRAMInitStructure.FSMC_WaitSignalPolarity = FSMC_WaitSignalPolarity_Low;
	FSMC_NORSRAMInitStructure.FSMC_WrapMode = FSMC_WrapMode_Disable;
	FSMC_NORSRAMInitStructure.FSMC_WaitSignalActive = FSMC_WaitSignalActive_BeforeWaitState;
	FSMC_NORSRAMInitStructure.FSMC_WriteOperation = FSMC_WriteOperation_Enable;
	FSMC_NORSRAMInitStructure.FSMC_WaitSignal = FSMC_WaitSignal_Disable;
	FSMC_NORSRAMInitStructure.FSMC_ExtendedMode = FSMC_ExtendedMode_Disable;
	FSMC_NORSRAMInitStructure.FSMC_WriteBurst = FSMC_WriteBurst_Disable;
	FSMC_NORSRAMInitStructure.FSMC_ReadWriteTimingStruct = &p;
	FSMC_NORSRAMInitStructure.FSMC_WriteTimingStruct = &p;
	
	
	FSMC_NORSRAMInit(&FSMC_NORSRAMInitStructure); 
	
	delay_ms(100);
	
	/* Enable FSMC Bank1_SRAM Bank */
	FSMC_NORSRAMCmd(FSMC_Bank1_NORSRAM1, ENABLE);  

	delay_ms(100);

	/* Enable the FSMC Clock */
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_FSMC, ENABLE);

	delay_ms(100);

}

void bsp_lcd_gpio_init(void)
{

	GPIO_InitTypeDef GPIO_InitStructure;


	// LCD PWM-LIGHT
	GPIO_InitStructure.GPIO_Pin = LCD_PWM_LIGHT_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(LCD_PWM_LIGHT_PORT, &GPIO_InitStructure);

	

	// LCD Reset
	GPIO_InitStructure.GPIO_Pin = LCD_RESET_PIN;
	GPIO_Init(LCD_RESET_PORT, &GPIO_InitStructure);

	// FSMC_NBL0											 	
	GPIO_InitStructure.GPIO_Pin = LCD_FSMC_NBL0_PIN;
	GPIO_Init(LCD_FSMC_NBL0_PORT, &GPIO_InitStructure); 

	// LCD RD, AFPP
	GPIO_InitStructure.GPIO_Pin = LCD_RD_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(LCD_RD_PORT, &GPIO_InitStructure);

	

	// LCD WR, AFPP
	GPIO_InitStructure.GPIO_Pin = LCD_WR_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(LCD_WR_PORT, &GPIO_InitStructure);

	


	// FSMC LCD Data LCD_D2, LCD_D3, LCD_D13, LCD_D14, LCD_D15, LCD_D0, LCD_D1
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_14 | GPIO_Pin_15;	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOD, &GPIO_InitStructure);

	// FSMC LCD Data LCD_D4, LCD_D5, LCD_D6, LCD_D7, LCD_D8, LCD_D9, LCD_D10, LCD_D11, LCD_D12
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | 
	                            GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | 
	                            GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOE, &GPIO_InitStructure); 

	// LCD CS
	GPIO_InitStructure.GPIO_Pin = LCD_CS_PIN;
	GPIO_Init(LCD_CS_PORT, &GPIO_InitStructure);

	// LCD RS
	GPIO_InitStructure.GPIO_Pin = LCD_RS_PIN;
	GPIO_Init(LCD_RS_PORT, &GPIO_InitStructure);
		 
}

void lcd_rst(void)
{
	LCD_RESET_LOW();
	delay_ms(10);
	delay_ms(10);
	delay_ms(10);
	delay_ms(10);
	delay_ms(10);
    LCD_RESET_HIGH();	
}

void lcd_wr_reg(unsigned int index)
{
	*(__IO uint16_t *) (Bank1_LCD_C)= index;

}

void lcd_wr_cmd(unsigned int index)
{
	*(__IO uint16_t *) (Bank1_LCD_C)= index;

}



void lcd_wr_reg_value(u16 LCD_Reg, u16 LCD_RegValue)
{	
	*(__IO uint16_t *) (Bank1_LCD_C)= LCD_Reg;	
	*(__IO uint16_t *) (Bank1_LCD_D)= LCD_RegValue;
}


unsigned int lcd_rd_data(void)
{
	unsigned int a=0;
	//a=(*(__IO uint16_t *) (Bank1_LCD_D)); 	//Dummy
	//a= *(__IO uint16_t *) (Bank1_LCD_D);  	//H
	//a=a<<8;
	a=*(__IO uint16_t *) (Bank1_LCD_D); //L

	return(a);	
}

void lcd_wr_data(unsigned int val)
{   
	*(__IO uint16_t *) (Bank1_LCD_D)= val; 	
}

void lcd_wr_data_8(unsigned int val)
{
	*(__IO uint16_t *) (Bank1_LCD_D)= val;
}

u32 mn(u8 m,u8 n)
{
	u32 result=1;	 
	while(n--)result*=m;    
	return result;
}

//m ^n function
u32 mypow (u8 m, u8 n)
{
	u32 result=1;	 
	while (n--) result*=m;    
	return result;
}	


uint16_t LCD_RGB2Pixel565(uint8_t r, uint8_t g, uint8_t b)
{
	/* RGB2PIXEL565 from a Macro in Greg Haerr's Nano-X, MPL license */
	return ((((r) & 0xf8) << 8) | (((g) & 0xfc) << 3) | (((b) & 0xf8) >> 3));
}



