

#include "hw_config.h"
#include "lcd1602.h"


void LCD1602_GPIO_OUTPUT(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;

  /* Enable GPIOB clock */
  //RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO, ENABLE);
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOE, &GPIO_InitStructure);
}
void LCD1602_GPIO_INPUT(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;

  /* Enable GPIOB clock */
  //RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO, ENABLE);
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOE, &GPIO_InitStructure);

}

void bsp_lcd1602_gpio_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	// LCD EN
	GPIO_InitStructure.GPIO_Pin = LCD1602_EN_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(LCD1602_EN_PORT, &GPIO_InitStructure);

	// LCD RW
	GPIO_InitStructure.GPIO_Pin = LCD1602_RW_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(LCD1602_RW_PORT, &GPIO_InitStructure);

	// LCD RS
	GPIO_InitStructure.GPIO_Pin = LCD1602_RS_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(LCD1602_RS_PORT, &GPIO_InitStructure);

	LCD1602_GPIO_OUTPUT();

 }

void bsp_lcd1602_init(void)
{
	bsp_lcd1602_gpio_init();
	
	LCD1602_sendbyte(iCmd, LCDa_FUNCTION);	// function mode setting 
	while(LCD1602_readBF());
	LCD1602_sendbyte(iCmd, LCDa_ON);		// open the show 
	while(LCD1602_readBF());
	LCD1602_clear();						// clear the screen 
	while(LCD1602_readBF());
	LCD1602_sendbyte(iCmd, LCDa_ENTRY);	// input mode setting 
}



u8 LCDa_DI(void)
{
	u8 data;

	data = (u8)(GPIOE->IDR >> 8);

	return data;
}


/******************************************* 
Function name: LCD1602_readbyte 
Function: From 1602 to read one u8 of data or instructions 
Parameters: DatCmd - when the data for the iDat, when instructions for the iCmd 
Return Value: du8 - read back the data or instruction 
********************************************/ 
u8 LCD1602_readbyte(u8 DatCmd)
{
 	u8 du8;

	if (DatCmd == iCmd)		 // Command operation 
		LCDa_CLR_RS;
	else
		LCDa_SET_RS;
		
	LCDa_SET_RW;	   	  	 // Read
	LCDa_SET_E;

	LCD1602_GPIO_INPUT();	 // data bus is defined as the input 

	du8=LCDa_DI();			 // read data or instructions 
	delay_ms(2);				 // timing adjustment 
	LCDa_CLR_E;	

	LCD1602_GPIO_OUTPUT();	 // restore the output data bus 

	return du8;
}

/******************************************* 
Function name: LCD1602_sendbyte 
Function: 1602 to write a u8 of data or instructions 
Parameters: DatCmd - when the data for the iDat, when instructions for the iCmd 
du8 - 1602 for the write data or instructions 
Return Value: None 
********************************************/ 
void LCD1602_sendbyte(u8 DatCmd, u8 du8)
{
	if (DatCmd == iCmd)		// Command operation 
		LCDa_CLR_RS;
	else
		LCDa_SET_RS;
		
	LCDa_CLR_RW;			// write
	LCDa_SET_E;
	LCDa_DO(du8);		// write data
	delay_ms(2);
	LCDa_CLR_E;	
}

/******************************************* 
Function name: LCD1602_sendstr 
Function: 1602 to the ranks assigned to write a string 
Parameters: Row - the specified line 
Col - the specified column 
ptString - string pointer 
Return Value: None 
********************************************/ 
void LCD1602_Char_Set(u8 Row, u8 Col, u8 *ptString)
{
	switch (Row)
	{
		case 2:
			LCD1602_sendbyte(iCmd, LCDa_L2 + Col); break;	// write line 2 of the specified column 
		default:
			LCD1602_sendbyte(iCmd, LCDa_L1 + Col); break;	// write the first line of the specified column 
	}
	while((*ptString)!='\0')		 // string is not the end of 
	{
		LCD1602_sendbyte(iDat, *ptString++);
	}
}
/******************************************* 
Function name: LCD1602_clear 
Function: clear-screen LCD 1602 
Parameters: None 
Return Value: None 
********************************************/ 
void LCD1602_clear(void)
{
	LCD1602_sendbyte(iCmd,LCDa_CLS);
	delay_ms(4); // clear the screen after the command is written, 2ms delay is necessary!!! 
}
/******************************************* 
Function name: LCD1602_readBF 
Function: clear-screen LCD 1602 
Parameters: None 
Return Value: busy - busy is 1, 0 can receive instruction 
********************************************/ 
u8 LCD1602_readBF(void)
{	  
	u8 busy;
	busy=LCD1602_readbyte(iCmd);		// read back to the BF flag and address 
	if(busy&0x80)						// Busy
		busy=1;
	else	   							// not busy, you can write 
		busy=0;
	return busy;
}

/******************************************* 
Function name: LCD1602_wrCGRAM 
Function: Write data to CGRAM 
Parameters: ptCGRAM - CGRAM data pointer 
Return Value: None 
********************************************/ 
void LCD1602_wrCGRAM(const u8 *ptCGRAM)
{
	u8 i;
	LCD1602_sendbyte(iCmd, LCDa_CGRAM_ADDR);  // write the first address of CGRAM 
	for (i = 0; i < LCDa_CGMAX; i++)		  // write the contents of 64 u8s of CGRAM 
	{
		LCD1602_sendbyte(iDat,ptCGRAM[i]);
	}
}
/******************************************* 
Function name: LCD1602_disCGRAM 
Function: display CGRAM contents of 64 u8s written on two lines of 32 u8s 
Parameters: None 
Return Value: None 
********************************************/ 
void LCD1602_disCGRAM(u8 Row,u8 Col)
{
	u8 i;
	switch (Row)
	{
		case 2:
			LCD1602_sendbyte(iCmd, LCDa_L2 + Col); break;	// write line 2 of the specified column 
		default:
			LCD1602_sendbyte(iCmd, LCDa_L1 + Col); break;	//  write the first line of the specified column 
	}
	for (i = 0;i <4;i++)   // display the contents of the first 32 u8s 
	{	 	
		LCD1602_sendbyte(iDat, i);		
	}
}


