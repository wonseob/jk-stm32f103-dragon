

#include "hw_config.h"
#include "usart.h"
#include "rs485.h"




void bsp_rs485_gpio_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	GPIO_InitStructure.GPIO_Pin =  RS485_EN_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(RS485_EN_PORT, &GPIO_InitStructure);
	  
	bsp_init_irq_usart3();
}

void rs485_set_rx(void)
{
	// make the MAX485 in the receiving state	
	GPIO_ResetBits(RS485_EN_PORT, RS485_EN_PIN);  
}

void rs485_set_tx(void)
{
	// make the MAX485 in the sending state	
	GPIO_SetBits(RS485_EN_PORT, RS485_EN_PIN);
}

// 9600 baud / 8 data bits / 1 stop bit / odd parity / send on / receive interrupt / Asynchronous Operation
void bsp_rs485_init(void) 
{	
	rs485_set_rx(); 	// make the MAX485 in the receiving state
} 

void rs485_puts(char *data) 
{	

	rs485_set_tx();
	delay_ms(20);

    while(*data != '\0')
	{
		
		delay_ms(50);
        usart3_transmit_byte(*(unsigned char *)data);		
        data++;
		delay_ms(50);
    }

	delay_ms(10);

	rs485_set_rx();	
} 

