#ifndef _SCCB_H
#define _SCCB_H

#include "stm32f10x.h"

#define SCCB_USER_PORT				GPIOE
#define SCCB_SCL_PIN				GPIO_Pin_7
#define SCCB_SDA_PIN				GPIO_Pin_8

#define SCCB_SIO_C		7 // 4 SCL
#define SCCB_SIO_D		8 // 5 SDA

#define SIO_C_SET		{GPIOE->BSRR =(1<<SCCB_SIO_C);}
#define SIO_C_CLR		{GPIOE->BRR = (1<<SCCB_SIO_C);}
#define SIO_D_SET		{GPIOE->BSRR =(1<<SCCB_SIO_D);}
#define SIO_D_CLR		{GPIOE->BRR = (1<<SCCB_SIO_D);}

#define SIO_D_IN	{GPIO_InitStructure.GPIO_Pin = SCCB_SDA_PIN;\
                         GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;\
                         GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;\
                         GPIO_Init(GPIOE, &GPIO_InitStructure); }

#define SIO_D_OUT	 {GPIO_InitStructure.GPIO_Pin = SCCB_SDA_PIN;\
                    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;\
                    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;\
                    GPIO_Init(GPIOE, &GPIO_InitStructure);}

#define SIO_D_STATE	((GPIOE->IDR&(1<<SCCB_SIO_D))==(1<<SCCB_SIO_D))


///////////////////////////////////////////
void DelaySCCB(void);
void InitSCCB(void);
void startSCCB(void);
void stopSCCB(void);
void noAck(void);
u8 SCCBwriteByte(u8 m_data);
u8 SCCBreadByte(void);


#endif /* _SCCB_H */
