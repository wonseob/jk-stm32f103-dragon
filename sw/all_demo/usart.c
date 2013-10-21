/*
*********************************************************************************************************
*                                            INCLUDE FILES
*********************************************************************************************************
*/
#include  <stdio.h>
#include  <string.h>
#include  <ctype.h>
#include  <stdlib.h>
#include  <stdarg.h>

#include "stm32f10x.h"
#include "hw_config.h"
#include "queue.h"
#include "usart.h"

// setting USART
#define USART_TX_BUFF_CNT	64
#define USART_RX_BUFF_CNT	10

#define USART_TX_DMA_SIZ          64
#define USART_RX_DMA_SIZ          64

q_list_type gbl_qlist_usart1_tx;
q_list_type gbl_qlist_usart1_tx_free;

q_node_type gbl_qdata_usart1_tx[USART_TX_BUFF_CNT];
q_node_type gbl_qdata_usart1_tx_free[USART_TX_BUFF_CNT];

char gbl_usart1_tx_buff[USART_TX_BUFF_CNT][USART_TX_DMA_SIZ];
char gbl_usart1_tx_buff_dma[USART_TX_DMA_SIZ];

q_list_type gbl_qlist_usart1_rx;
q_list_type gbl_qlist_usart1_rx_free;

q_node_type gbl_qdata_usart1_rx[USART_RX_BUFF_CNT];
q_node_type gbl_qdata_usart1_rx_free[USART_RX_BUFF_CNT];

char gbl_usart1_rx_buff[USART_RX_BUFF_CNT][USART_RX_DMA_SIZ];
char gbl_usart1_rx_buff_dma[USART_RX_DMA_SIZ];

static u16 s_usart1_tx_send_cnt = 0;
static u16 s_usart1_tx_q_free_cnt = 0;
static u16 s_usart1_tx_q_send_cnt = 0;

static s8 usart_format_buff[128];

#if 1
typedef struct
{
	u16 wr_idx;
	u16 rd_idx;
	char buffer[USART_RX_DMA_SIZ];
}usart_rx_type;

usart_rx_type gbl_usart1_rx_buff_proc;
#endif

u16 usart1_dma_transfering = FALSE;
//

static usartmode_type s_usart1_mode = usartmodeIRQ;
static usartmode_type s_usart2_mode = usartmodeIRQ;
static usartmode_type s_usart3_mode = usartmodeIRQ;

usart_service_function_type gbl_ar_usart_service[usartServiceFunctionMAX] = 
{
	{usart1ServiceFunction, NULL},
	{usart2ServiceFunction, NULL},
	{usart3ServiceFunction, NULL}
};

/* ------------------------------------------------------------------------------------------------- */
/* BSP USART */
/* ------------------------------------------------------------------------------------------------- */


void init_usart1_buffer(void)
{
	int i;

	q_remove_all(&gbl_qlist_usart1_tx);
	q_remove_all(&gbl_qlist_usart1_tx_free);

	// initialize usart tx queue buffer.
	for(i=0;i<USART_TX_BUFF_CNT;i++)
	{
		gbl_qdata_usart1_tx_free[i].data = &gbl_usart1_tx_buff[i];
		q_add_tail(&gbl_qlist_usart1_tx_free, &gbl_qdata_usart1_tx_free[i]);		
	}

	q_remove_all(&gbl_qlist_usart1_rx);
	q_remove_all(&gbl_qlist_usart1_rx_free);

	// initialize usart rx queue buffer.
	for(i=0;i<USART_RX_BUFF_CNT;i++)
	{
		gbl_qdata_usart1_rx_free[i].data = &gbl_usart1_rx_buff[i];
		q_add_tail(&gbl_qlist_usart1_rx_free, &gbl_qdata_usart1_rx_free[i]);		
	}

	memset(gbl_usart1_tx_buff_dma, 0x00, sizeof(char)*USART_TX_DMA_SIZ);
	memset(gbl_usart1_rx_buff_dma, 0x00, sizeof(char)*USART_RX_DMA_SIZ);
	memset(gbl_usart1_rx_buff_proc.buffer, 0x00, sizeof(char)*USART_RX_DMA_SIZ);	
	gbl_usart1_rx_buff_proc.rd_idx = 0;
	gbl_usart1_rx_buff_proc.wr_idx = 0;

	usart1_dma_transfering = FALSE;
}

void usart_transmit_byte( USART_TypeDef* port, u16 chr)
{
	USART_SendData(port, chr);
	while (USART_GetFlagStatus(port, USART_FLAG_TXE) == RESET)
		;		
}

void usart1_transmit_byte(u16 chr)
{
	usart_transmit_byte(USART1, chr);
}


void usart1_tx_proc(void)
{
	int i;
	u16 data_len;
	// char tx_data[USART_TX_DMA_SIZ];
	q_node_type* q_usart_pkt_ptr;



	if( s_usart1_mode == usartmodeIRQ )
	{
		while( (q_usart_pkt_ptr = q_remove_tail(&gbl_qlist_usart1_tx)) != NULL )
		{
			data_len = q_usart_pkt_ptr->len;
			memcpy(gbl_usart1_tx_buff_dma, q_usart_pkt_ptr->data, q_usart_pkt_ptr->len);

			for(i=0;i<data_len;i++)
			{
				usart_transmit_byte(USART1, (u16)gbl_usart1_tx_buff_dma[i]);
			}
			q_add_tail(&gbl_qlist_usart1_tx_free, q_usart_pkt_ptr);

		}
	}	
	
}


void usart_transmit_string(USART_TypeDef* port, const void *data) 
{
	int i;
	u16 data_len;
	int tx_count;
	const char* usart_data;
	q_node_type* q_usart_pkt_ptr;

	usart_data = (char*)data;
	data_len = strlen(usart_data);

	tx_count = (data_len - 1) / USART_TX_DMA_SIZ + 1;

	INTLOCK();


	if( q_get_count(&gbl_qlist_usart1_tx_free) >= tx_count )
	{
		for(i=0;i<tx_count-1;i++)
		{
			if( (q_usart_pkt_ptr = q_remove_tail(&gbl_qlist_usart1_tx_free)) != NULL )
			{
				memcpy(q_usart_pkt_ptr->data, usart_data, USART_TX_DMA_SIZ);
				q_usart_pkt_ptr->len = USART_TX_DMA_SIZ;
				q_add_tail(&gbl_qlist_usart1_tx, q_usart_pkt_ptr);

				data_len = data_len - USART_TX_DMA_SIZ;
				usart_data += USART_TX_DMA_SIZ;

			}
		}

		if( (q_usart_pkt_ptr = q_remove_tail(&gbl_qlist_usart1_tx_free)) != NULL )
		{
			memcpy(q_usart_pkt_ptr->data, usart_data, data_len);
			q_usart_pkt_ptr->len = data_len;
			q_add_tail(&gbl_qlist_usart1_tx, q_usart_pkt_ptr);

		}

		if( usart1_dma_transfering == FALSE )
			usart1_tx_proc();

			
	}	


	INTFREE();
	
}

void usart1_transmit_string(const void *data)
{
	usart_transmit_string(USART1, data);
}

void usart1_transmit_string_format(const char * szFormat, ... )
{
	va_list varpars;
	int nLen;

	va_start(varpars, szFormat);
	nLen = vsprintf( (char*)usart_format_buff, szFormat, varpars);
	va_end(varpars);

	usart_format_buff[nLen] = 0x00;

	usart_transmit_string(USART1, usart_format_buff);	
}

void bsp_init_irq_usart1(void/*isr_function usart1_isr*/)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	init_usart1_buffer();

	/* Enable the USART1 Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);		

	/* 
	   USART1_REMAP USART1 remapping
	   This bit is set and cleared by software. It controls the mapping of USART1 TX and RX alternate
	   functions on the GPIO ports.

	   0: No remap (TX/PA9, RX/PA10)
	   1: Remap (TX/PB6, RX/PB7)		
	*/
	// GPIO_PinRemapConfig(GPIO_Remap_USART1, ENABLE);	


	/* Configure the GPIO ports( USART1 Transmit and Receive Lines) */
	/* Configure the USART1_Tx as Alternate function Push-Pull */
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	  
	/* Configure the USART1_Rx as input floating */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;   
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	/* Configure the USART1 */
	USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART1, &USART_InitStructure);

	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);

	/* Enable the USART1 */
	USART_Cmd(USART1, ENABLE);	

	s_usart1_mode = usartmodeIRQ;

}



/* ------------------------------------------------------------------------------------------------- */
/* extern USART */
/* ------------------------------------------------------------------------------------------------- */
void USART1_IRQHandler(void)
{
	u16 data_len;
	q_node_type* q_usart_pkt_ptr;
	s8 data[1];
	
	// u16 wr_idx;
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
	{

		data_len = 1;

		if( q_get_count(&gbl_qlist_usart1_rx_free) > 0 )
		{
			if( (q_usart_pkt_ptr = q_remove_tail(&gbl_qlist_usart1_rx_free)) != NULL )
			{
				data[0] = USART_ReceiveData(USART1) & 0xFF;
				memcpy(q_usart_pkt_ptr->data, data, data_len);
				q_usart_pkt_ptr->len = data_len;
				q_add_tail(&gbl_qlist_usart1_rx, q_usart_pkt_ptr);

			}
		}	

		USART_ClearITPendingBit(USART1, USART_IT_RXNE);	  
	}
}


int usart_is_ne(USART_TypeDef* port)
{

	if( port == USART1 )
	{
		//if( gbl_usart1_rx_buff.wr_idx == gbl_usart1_rx_buff.rd_idx )
		if( q_get_count(&gbl_qlist_usart1_rx) == 0 )
			return FALSE;
		else 
		{
			return TRUE;
		}
	}

	return FALSE;
}

int usart1_is_ne(void)
{
	return usart_is_ne(USART1);
}

void* usart1_get_data(void)
{
	u16 data_len;
	q_node_type* q_usart_pkt_ptr;	
	//byte data[1];

	s_usart1_tx_q_free_cnt = q_get_count(&gbl_qlist_usart1_tx_free);
	s_usart1_tx_q_send_cnt = q_get_count(&gbl_qlist_usart1_tx);	


	if( usart_is_ne(USART1) )
	{

		if( (q_usart_pkt_ptr = q_remove_tail(&gbl_qlist_usart1_rx)) != NULL )
		{
			data_len = q_usart_pkt_ptr->len;
			memcpy(gbl_usart1_rx_buff_proc.buffer, q_usart_pkt_ptr->data, q_usart_pkt_ptr->len);


			q_add_tail(&gbl_qlist_usart1_rx_free, q_usart_pkt_ptr);

			return gbl_usart1_rx_buff_proc.buffer;

		}


	}

	return NULL;
}


void bsp_init_irq_usart2(void/*isr_function usart1_isr*/)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;	

	/* Enable the USART2 Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);		

	/* 
	   USART1_REMAP USART1 remapping
	   This bit is set and cleared by software. It controls the mapping of USART1 TX and RX alternate
	   functions on the GPIO ports.

	   0: No remap (TX/PA9, RX/PA10)
	   1: Remap (TX/PB6, RX/PB7)		
	*/
	// GPIO_PinRemapConfig(GPIO_Remap_USART1, ENABLE);	


	/* Configure the GPIO ports( USART1 Transmit and Receive Lines) */
	/* Configure the USART2_Tx as Alternate function Push-Pull */
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	  
	/* Configure the USART2_Rx as input floating */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;   
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	/* Configure the USART2 */
	USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART2, &USART_InitStructure);

	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);

	/* Enable the USART2 */
	USART_Cmd(USART2, ENABLE);	

	s_usart2_mode = usartmodeIRQ;

}



void USART2_IRQHandler(void)
{
	u16 data_len;
	s8 data[1];	

	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
	{

		data_len = 1;
		data[0] = USART_ReceiveData(USART2) & 0xFF;

		// uart2 echo test
		usart2_transmit_byte(data[0]);

		USART_ClearITPendingBit(USART2, USART_IT_RXNE);	  
	}
}

void usart2_transmit_byte(u16 chr)
{
	usart_transmit_byte(USART2, chr);
}

void usart2_transmit_string(char* data)
{
    while(*data != '\0')
	{
        usart2_transmit_byte(*(unsigned char *)data);
        data++;
    }
}


void bsp_init_irq_usart3(void/*isr_function usart1_isr*/)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;	

	/* Enable the USART2 Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);		

	/* 
	   USART1_REMAP USART1 remapping
	   This bit is set and cleared by software. It controls the mapping of USART1 TX and RX alternate
	   functions on the GPIO ports.

	   0: No remap (TX/PA9, RX/PA10)
	   1: Remap (TX/PB6, RX/PB7)		
	*/
	// GPIO_PinRemapConfig(GPIO_Remap_USART1, ENABLE);	


	/* Configure the GPIO ports( USART3 Transmit and Receive Lines) */
	/* Configure the USART3_Tx as Alternate function Push-Pull */
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	  
	/* Configure the USART3_Rx as input floating */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;   
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	/* Configure the USART3 */
	USART_InitStructure.USART_BaudRate = BAUDRATE_485;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;  // USART_StopBits_1
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART3, &USART_InitStructure);

	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);

	/* Enable the USART3 */
	USART_Cmd(USART3, ENABLE);		

	s_usart3_mode = usartmodeIRQ;

}



 

void register_usart_function(usart_register_function_type usart_fn_type, usart_register_function fn)
{
	gbl_ar_usart_service[usart_fn_type].run = fn;
}

void USART3_IRQHandler(void)
{
	u16 data_len;
	s8 data[1];	

	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)
	{

		data_len = 1;
		data[0] = USART_ReceiveData(USART3) & 0xFF;

		// uart3 echo test
		// usart3_transmit_byte(data[0]);

		if( gbl_ar_usart_service[usart3ServiceFunction].run != NULL )
		{
			gbl_ar_usart_service[usart3ServiceFunction].run(data[0]);
		}

		USART_ClearITPendingBit(USART3, USART_IT_RXNE);	  
	}
}

void usart3_transmit_byte(u16 chr)
{
	usart_transmit_byte(USART3, chr);
}

void usart3_transmit_string(char* data)
{
    while(*data != '\0')
	{
        usart3_transmit_byte(*(unsigned char *)data);
        data++;
    }
}

