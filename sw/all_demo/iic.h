

#ifndef __IIC_H
#define __IIC_H

#define IIC1_SCL_PORT  	GPIOB    
#define IIC1_SCL_PIN  	GPIO_Pin_6  

#define IIC1_SDA_PORT  	GPIOB 
#define IIC1_SDA_PIN  	GPIO_Pin_7

#define IIC1_SCL_HIGH() GPIO_WriteBit(IIC1_SCL_PORT,IIC1_SCL_PIN,Bit_SET)
#define IIC1_SCL_LOW() 	GPIO_WriteBit(IIC1_SCL_PORT,IIC1_SCL_PIN,Bit_RESET)

#define IIC1_SDA_HIGH() GPIO_WriteBit(IIC1_SDA_PORT,IIC1_SDA_PIN,Bit_SET)
#define IIC1_SDA_LOW() 	GPIO_WriteBit(IIC1_SDA_PORT,IIC1_SDA_PIN,Bit_RESET)

#define IIC1_READ_DATA 	GPIO_ReadInputDataBit(IIC1_SDA_PORT,IIC1_SDA_PIN)

void IIC1_Init(void);
void IIC1_Start(void);
void IIC1_Stop(void);
u8 IIC1_Wait_Ack(void);
void IIC1_Ack(void);
void IIC1_NAck(void);
void IIC1_Send_Byte(u8 txd);
u8 IIC1_Read_Byte(unsigned char ack);

#endif
