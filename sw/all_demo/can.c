

#include "hw_config.h"
#include "usart.h"
#include "can.h"

__IO uint32_t Result = 0; /* for return of the interrupt handling */

void bsp_can_gpio_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	/* Configure CAN pin: TX */
	GPIO_InitStructure.GPIO_Pin = CAN_TX_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(CAN_PORT, &GPIO_InitStructure);

	/* Configure CAN pin: RX */
	GPIO_InitStructure.GPIO_Pin = CAN_RX_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(CAN_PORT, &GPIO_InitStructure);
	
	// GPIO_PinRemapConfig(AFIO_MAPR_CAN_REMAP_REMAP2 , ENABLE);	
	GPIO_PinRemapConfig(GPIO_Remap1_CAN1,ENABLE); 
}


void bsp_can_interrupt_init(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	
	/* Enable CAN1 RX0 interrupt IRQ channel */
	NVIC_InitStructure.NVIC_IRQChannel = USB_LP_CAN1_RX0_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

}

void bsp_can_init(void) 
{
	CAN_InitTypeDef        CAN_InitStructure;
	CAN_FilterInitTypeDef  CAN_FilterInitStructure;

	int is_initialized = 0;

	bsp_can_gpio_init();
	bsp_can_interrupt_init();

	/* CAN register init */
	CAN_DeInit(CAN1);
	CAN_StructInit(&CAN_InitStructure);
	
	/* CAN cell init */

	/* time-triggered prohibited, time-triggered: CAN hardware internal timer is activated, and is used to generate a timestamp  */
	CAN_InitStructure.CAN_TTCM = DISABLE; 

	/* automatic offline prohibit automatic offline: offline automatically exit once the hardware is monitored 128 times 11 recessive bits. Here to be the software set to exit */
	CAN_InitStructure.CAN_ABOM = DISABLE; 

	/* automatic wake prohibit packets to automatically exit hibernation	*/
	CAN_InitStructure.CAN_AWUM = DISABLE; 

	/* packet retransmission, if the error has been passed successfully ended, otherwise only passing time */
	CAN_InitStructure.CAN_NART = DISABLE; 

	/*  Receive FIFO lock - Lock received a new message digest not ,0 - received a new message to cover the previous packet	*/
	CAN_InitStructure.CAN_RFLM = DISABLE; 

	/* send priority 0 --- decided by the identifier 1 --- determines the order by sending a request	*/
	CAN_InitStructure.CAN_TXFP = ENABLE;  

	CAN_InitStructure.CAN_Mode = CAN_Mode_Normal; /* mode	*/

	/* re-synchronization jump width, only can hardware in order to access the register in the initialization mode */
	CAN_InitStructure.CAN_SJW = CAN_SJW_1tq;      
	CAN_InitStructure.CAN_BS1 = CAN_BS1_4tq;      /* period */
	CAN_InitStructure.CAN_BS2 = CAN_BS2_3tq;      /* period */
	CAN_InitStructure.CAN_Prescaler = 45;         /* Baud Rate Prescaler frequency */  
	
	/* Baud rate calculation method  */
	/* CANbps= Fpclk/((BRP+1)*((Tseg1+1)+(Tseg2+1)+1)  calculated here is  CANbps=36000000/(45*(4+3+1))=100kHz */   														  //´Ë´¦Tseg1+1 = CAN_BS1_8tp
	/* Configure general direction:  Tseg1>=Tseg2  Tseg2>=tq; Tseg2>=2TSJW */
	
	if (CAN_Init(CAN1,&CAN_InitStructure) == CANINITFAILED) 		
	{
		/* Initialize time the first set CAN_MCR the initialization bit */
		/* Then view the hardware really set the initialization bit CAN_MSR confirm whether or not to enter the initialization mode */
		is_initialized = 0;
	}

	is_initialized = 1;
		
	/* Configure the the CAN filters */
	/* 32 corresponding to the id */
	/* Stdid [10:0], extid [17:0], ide, rtr */
	/* 16 corresponding to the id */
	/* Stdid [10:0], ide, rtr, extid [17:15] */
	/* General shield mode */
	/* Fifo receiver full interrupt, there is fifo concept that has been taken is the earliest that a data, to be released in order to remove a data */
	/* Often used interrupt */
	/* 1, interruption, ie fifo registered interrupt */
	/* 2, fifo full interrupts */
	/* Fifo full and information to the interrupt that fifo overflow interrupt */
	
	CAN_FilterInitStructure.CAN_FilterNumber=0;     /* filter */
	CAN_FilterInitStructure.CAN_FilterMode=CAN_FilterMode_IdMask;  /* spacious screen mode */
	CAN_FilterInitStructure.CAN_FilterScale=CAN_FilterScale_32bit; /* 32 */
	CAN_FilterInitStructure.CAN_FilterIdHigh=0x0000;  /* The following four are 0, show that does not filter any id */
	CAN_FilterInitStructure.CAN_FilterIdLow=0x0000;
	CAN_FilterInitStructure.CAN_FilterMaskIdHigh=0x0000;
	CAN_FilterInitStructure.CAN_FilterMaskIdLow=0x0000;
	CAN_FilterInitStructure.CAN_FilterFIFOAssignment=0;  /* packets through the filter deposit to fifo0 in the */
	CAN_FilterInitStructure.CAN_FilterActivation=ENABLE;
	CAN_ITConfig(CAN1,CAN_IT_FMP0, ENABLE);   /*  registered interrupt to enter the interrupt the read fifo message function after release message clear interrupt flag */
	CAN_FilterInit(&CAN_FilterInitStructure);

} 

/*******************************************************************************
* Function Name  : can_polling
* Description    : Configures the CAN, transmit and receive by polling.
* Input          : None
* Output         : None
* Return         : PASSED if the reception is well done, FAILED in other case
* Attention		 : None
*******************************************************************************/
int can_polling(void)
{
	CAN_InitTypeDef        CAN_InitStructure;
	CAN_FilterInitTypeDef  CAN_FilterInitStructure;
	CanTxMsg TxMessage;
	CanRxMsg RxMessage;
	uint32_t i = 0;
	uint8_t TransmitMailbox = 0;
	
	/* CAN register init */
	CAN_DeInit(CAN1);
	CAN_StructInit(&CAN_InitStructure);
	
	/* CAN cell init */
	CAN_InitStructure.CAN_TTCM=DISABLE;
	CAN_InitStructure.CAN_ABOM=DISABLE;
	CAN_InitStructure.CAN_AWUM=DISABLE;
	CAN_InitStructure.CAN_NART=DISABLE;
	CAN_InitStructure.CAN_RFLM=DISABLE;
	CAN_InitStructure.CAN_TXFP=DISABLE;
	CAN_InitStructure.CAN_Mode=CAN_Mode_LoopBack;
	CAN_InitStructure.CAN_SJW=CAN_SJW_1tq;
	CAN_InitStructure.CAN_BS1=CAN_BS1_8tq;
	CAN_InitStructure.CAN_BS2=CAN_BS2_7tq;
	CAN_InitStructure.CAN_Prescaler=5;
	CAN_Init(CAN1, &CAN_InitStructure);
	
	/* CAN filter init */
	CAN_FilterInitStructure.CAN_FilterNumber=0;
	CAN_FilterInitStructure.CAN_FilterMode=CAN_FilterMode_IdMask;
	CAN_FilterInitStructure.CAN_FilterScale=CAN_FilterScale_32bit;
	CAN_FilterInitStructure.CAN_FilterIdHigh=0x0000;
	CAN_FilterInitStructure.CAN_FilterIdLow=0x0000;
	CAN_FilterInitStructure.CAN_FilterMaskIdHigh=0x0000;
	CAN_FilterInitStructure.CAN_FilterMaskIdLow=0x0000;
	CAN_FilterInitStructure.CAN_FilterFIFOAssignment=0;
	CAN_FilterInitStructure.CAN_FilterActivation=ENABLE;
	CAN_FilterInit(&CAN_FilterInitStructure);
	
	/* transmit */
	TxMessage.StdId=0x11;
	TxMessage.RTR=CAN_RTR_DATA;
	TxMessage.IDE=CAN_ID_STD;
	TxMessage.DLC=2;
	TxMessage.Data[0]=0xCA;
	TxMessage.Data[1]=0xFE;
	
	TransmitMailbox=CAN_Transmit(CAN1, &TxMessage);
	i = 0;
	while((CAN_TransmitStatus(CAN1, TransmitMailbox) != CANTXOK) && (i != 0xFF))
	{
		i++;
	}
	
	i = 0;
	while((CAN_MessagePending(CAN1, CAN_FIFO0) < 1) && (i != 0xFF))
	{
		i++;
	}
	
	/* receive */
	RxMessage.StdId=0x00;
	RxMessage.IDE=CAN_ID_STD;
	RxMessage.DLC=0;
	RxMessage.Data[0]=0x00;
	RxMessage.Data[1]=0x00;
	CAN_Receive(CAN1, CAN_FIFO0, &RxMessage);
	
	if (RxMessage.StdId!=0x11)
	{
		return DISABLE;  
	}
	
	if (RxMessage.IDE!=CAN_ID_STD)
	{
		return DISABLE;
	}
	
	if (RxMessage.DLC!=2)
	{
		return DISABLE;  
	}
	
	if ((RxMessage.Data[0]<<8|RxMessage.Data[1])!=0xCAFE)
	{
		return DISABLE;
	}
	
	return ENABLE; /* Test Passed */
}

/*******************************************************************************
* Function Name  : NVIC_Configuration
* Description    : Configures the CAN, transmit and receive using interrupt.
* Input          : None
* Output         : None
* Return         : PASSED if the reception is well done, FAILED in other case
* Attention		 : None
*******************************************************************************/
int can_interrupt(void)
{
	CAN_InitTypeDef        CAN_InitStructure;
	CAN_FilterInitTypeDef  CAN_FilterInitStructure;
	CanTxMsg TxMessage;
	uint32_t i = 0;
	
	/* CAN register init */
	CAN_DeInit(CAN1);
	CAN_StructInit(&CAN_InitStructure);
	
	/* CAN cell init */
	CAN_InitStructure.CAN_TTCM=DISABLE;
	CAN_InitStructure.CAN_ABOM=DISABLE;
	CAN_InitStructure.CAN_AWUM=DISABLE;
	CAN_InitStructure.CAN_NART=DISABLE;
	CAN_InitStructure.CAN_RFLM=DISABLE;
	CAN_InitStructure.CAN_TXFP=DISABLE;
	CAN_InitStructure.CAN_Mode=CAN_Mode_LoopBack;
	CAN_InitStructure.CAN_SJW=CAN_SJW_1tq;
	CAN_InitStructure.CAN_BS1=CAN_BS1_8tq;
	CAN_InitStructure.CAN_BS2=CAN_BS2_7tq;
	CAN_InitStructure.CAN_Prescaler=1;
	CAN_Init(CAN1, &CAN_InitStructure);
	
	/* CAN filter init */
	CAN_FilterInitStructure.CAN_FilterNumber=1;
	CAN_FilterInitStructure.CAN_FilterMode=CAN_FilterMode_IdMask;
	CAN_FilterInitStructure.CAN_FilterScale=CAN_FilterScale_32bit;
	CAN_FilterInitStructure.CAN_FilterIdHigh=0x0000;
	CAN_FilterInitStructure.CAN_FilterIdLow=0x0000;
	CAN_FilterInitStructure.CAN_FilterMaskIdHigh=0x0000;
	CAN_FilterInitStructure.CAN_FilterMaskIdLow=0x0000;
	CAN_FilterInitStructure.CAN_FilterFIFOAssignment=CAN_FIFO0;
	CAN_FilterInitStructure.CAN_FilterActivation=ENABLE;
	CAN_FilterInit(&CAN_FilterInitStructure);
	
	/* CAN FIFO0 message pending interrupt enable */ 
	CAN_ITConfig(CAN1, CAN_IT_FMP0, ENABLE);
	
	/* transmit 1 message */
	TxMessage.StdId=0x00;
	TxMessage.ExtId=0x1234;
	TxMessage.IDE=CAN_ID_EXT;
	TxMessage.RTR=CAN_RTR_DATA;
	TxMessage.DLC=2;
	TxMessage.Data[0]=0xDE;
	TxMessage.Data[1]=0xCA;
	CAN_Transmit(CAN1, &TxMessage);
	
	/* initialize the value that will be returned */
	Result = 0xFF;
	   
	/* receive message with interrupt handling */
	i=0;
	while((Result == 0xFF) && (i < 0xFFF))
	{
		i++;
	}
	
	if (i == 0xFFF)
	{
		Result=0;  
	}
	
	/* disable interrupt handling */
	CAN_ITConfig(CAN1, CAN_IT_FMP0, DISABLE);
	
	return (FunctionalState)Result;
}

