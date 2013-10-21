
#include "hw_config.h"
#include "iic.h"
#include "at24c02.h"


void bsp_at24c02_gpio_init() 
{
	GPIO_InitTypeDef GPIO_InitStructure;	
	
	//SCL
	GPIO_InitStructure.GPIO_Pin = IIC1_SCL_PIN;  
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
	GPIO_Init(IIC1_SCL_PORT, &GPIO_InitStructure);                                 

	//SDA
	GPIO_InitStructure.GPIO_Pin = IIC1_SDA_PIN;  
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD; 
	GPIO_Init(IIC1_SDA_PORT, &GPIO_InitStructure); 
}


void AT24CXX_Write_Delay(void)
{
	u16 i;
	for(i=0;i<45000;i++);
}

void AT24CXX_Init(void)
{
	bsp_at24c02_gpio_init();
}

u8 AT24CXX_ReadOneByte(u16 ReadAddr)
{
	u8 temp = 0;

	IIC1_Start();

	if(EE_TYPE>AT24C16)
	{
		IIC1_Send_Byte(0xA0);
		IIC1_Wait_Ack();
		IIC1_Send_Byte(ReadAddr>>8);
	}
	else
	{
		IIC1_Send_Byte(0xA0+((ReadAddr/256)<<1));   
	}

	IIC1_Wait_Ack();
	IIC1_Send_Byte(ReadAddr%256);
	IIC1_Wait_Ack();
	IIC1_Start();
	IIC1_Send_Byte(0xA1);
	IIC1_Wait_Ack();
	temp = IIC1_Read_Byte(0);
	IIC1_Stop();

	return temp;
}

void AT24CXX_WriteOneByte(u16 WriteAddr,u8 DataToWrite)
{
	IIC1_Start();
	if(EE_TYPE>AT24C16)
	{
		IIC1_Send_Byte(0xA0);
		IIC1_Wait_Ack();
		IIC1_Send_Byte(WriteAddr>>8);
		IIC1_Wait_Ack(); 
	}
	else
	{
		IIC1_Send_Byte(0xa0 + ((WriteAddr/256<<1)));
	}

	IIC1_Wait_Ack();
	IIC1_Send_Byte(WriteAddr%256);
	IIC1_Wait_Ack();
	IIC1_Send_Byte(DataToWrite);
	IIC1_Wait_Ack();
	IIC1_Stop();
	AT24CXX_Write_Delay();
 
}

void AT24CXX_WriteLenByte(u16 WriteAddr,u32 DataToWrite,u8 Len)
{
	u8 t;
	for(t=0;t<Len;t++)
	{
		AT24CXX_WriteOneByte(WriteAddr+t,(DataToWrite>>(8*t))&0xff);
	}
}

u32 AT24CXX_ReadLenByte(u16 ReadAddr,u8 Len)
{
	u8 t;
	u32 temp = 0;

	for(t=0;t<Len;t++)
	{
		temp<<=8;
		temp+=AT24CXX_ReadOneByte(ReadAddr+Len-t-1);
	}

	return temp;
}

void AT24CXX_Read(u16 ReadAddr,u8 *pBuffer,u16 NumToRead)
{
	while(NumToRead)
	{
		*pBuffer++=AT24CXX_ReadOneByte(ReadAddr++);
		NumToRead--;
	}
}

void AT24CXX_Write(u16 WriteAddr,u8 *pBuffer,u16 NumToWrite)
{
	while(NumToWrite--)
	{
		AT24CXX_WriteOneByte(WriteAddr,*pBuffer);
		WriteAddr++;
		pBuffer++;
	}
}

