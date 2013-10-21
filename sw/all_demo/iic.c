
#include "hw_config.h"
#include "iic.h"


void IIC_delay_us(u16 num)
{
	u8 i,j;

	for(i=0;i<num;i++)
		for(j=0;j<20;j++);
}

void IIC1_Start(void)
{     
	IIC1_SDA_HIGH();
	IIC1_SCL_HIGH();
	IIC_delay_us(4);
	IIC1_SDA_LOW();
	IIC_delay_us(4);
	IIC1_SCL_LOW();            
}

void IIC1_Stop(void)
{     
	IIC1_SCL_LOW();
	IIC1_SDA_LOW();
	IIC_delay_us(4);
	IIC1_SCL_HIGH();
	IIC1_SDA_HIGH();
	IIC_delay_us(4);
}

u8 IIC1_Wait_Ack(void)
{     
	u16 ucErrTime=0;
	
	IIC1_SDA_HIGH();
	IIC_delay_us(1);
	IIC1_SCL_HIGH();
	IIC_delay_us(1);

	while(IIC1_READ_DATA)
	{
		ucErrTime++;
		if(ucErrTime>2500)
		{
			IIC1_Stop();
			return 1;
		}     
	}

	IIC1_SCL_LOW();

	return 0;
}

void IIC1_Ack(void)
{     
	IIC1_SCL_LOW();
	IIC1_SDA_LOW();
	IIC_delay_us(2);
	IIC1_SCL_HIGH();
	IIC_delay_us(2);
	IIC1_SCL_LOW(); 
}

void IIC1_NAck(void)
{     
	IIC1_SCL_LOW();
	IIC1_SDA_HIGH();
	IIC_delay_us(2);
	IIC1_SCL_HIGH();
	IIC_delay_us(2);
	IIC1_SCL_LOW();
}

void IIC1_Send_Byte(u8 txd)
{     
	u8 t;

	IIC1_SCL_LOW();

	for(t=0;t<8;t++)
	{
		if(txd&0x80)
			IIC1_SDA_HIGH();
		else
			IIC1_SDA_LOW();

		txd<<=1;
		IIC_delay_us(2);
		IIC1_SCL_HIGH();
		IIC_delay_us(2);
		IIC1_SCL_LOW();
		IIC_delay_us(2);
	}
}

u8 IIC1_Read_Byte(unsigned char ack)
{      
	unsigned char i,receive = 0;
	for(i=0;i<8;i++)
	{
		IIC1_SCL_LOW();
		IIC_delay_us(2);
		IIC1_SCL_HIGH();
		receive<<=1;

		if(IIC1_READ_DATA)receive++;

		IIC_delay_us(1);
	}

	if(!ack)
		IIC1_NAck();
	else
		IIC1_Ack();

	return receive;
}

