

#include  <stm32f10x.h>
#include "key.h"


const bsp_key_group_type key_group[keyMax]=
{
	{KEY1_USER_PORT, KEY1_USER_PIN },
	{KEY2_USER_PORT, KEY2_USER_PIN },
	{KEY3_USER_PORT, KEY3_USER_PIN },
	{KEY4_USER_PORT, KEY4_USER_PIN },
	{KEY5_USER_PORT, KEY5_USER_PIN }

};


exti_key_service_function_type gbl_ar_exti_key_service[extiKeyServiceFunctionMAX] = 
{
	{extiKey5ServiceFunction, NULL}
};


void register_exti_key_function(exti_key_register_function_type exti_key_fn_type, exti_key_register_function fn)
{
	gbl_ar_exti_key_service[exti_key_fn_type].run = fn;
}



void bsp_key_gpio_init(void)
{
	
	EXTI_InitTypeDef EXTI_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;

	/* Configure the GPIO ports */
	GPIO_InitStructure.GPIO_Pin =  KEY1_USER_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(KEY1_USER_PORT, &GPIO_InitStructure);	

	GPIO_InitStructure.GPIO_Pin =  KEY2_USER_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(KEY2_USER_PORT, &GPIO_InitStructure);	

	GPIO_InitStructure.GPIO_Pin =  KEY3_USER_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(KEY3_USER_PORT, &GPIO_InitStructure);	

	GPIO_InitStructure.GPIO_Pin =  KEY4_USER_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(KEY4_USER_PORT, &GPIO_InitStructure);	

	GPIO_InitStructure.GPIO_Pin =  KEY5_USER_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(KEY5_USER_PORT, &GPIO_InitStructure);	


	/* Configure EXTI  to generate an interrupt on falling edge */
	EXTI_InitStructure.EXTI_Line = KEY5_IRQ_EXTI_Line;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
	EXTI_InitStructure.EXTI_LineCmd = DISABLE;
	EXTI_Init(&EXTI_InitStructure);	

		
}

void bsp_key_interrupt_init(void)
{
	EXTI_InitTypeDef EXTI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	/* Connect EXTI */	
	GPIO_EXTILineConfig(KEY5_IRQ_PORT_SOURCE, KEY5_IRQ_PIN_SOURCE);	

	/* Configure EXTI  to generate an interrupt on falling edge */
	EXTI_InitStructure.EXTI_Line = KEY5_IRQ_EXTI_Line;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);

	/* 2 bit for pre-emption priority, 2 bits for subpriority */
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

	NVIC_InitStructure.NVIC_IRQChannel = KEY5_IRQ_CHANNEL;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);	

	/* Clear EXTI Line Pending Bit */	
	EXTI_ClearITPendingBit(KEY5_IRQ_EXTI_Line);	
	/* Enable the Key EXTI line Interrupt */
	NVIC_ClearPendingIRQ(KEY5_IRQ_CHANNEL);		
	
}

void EXTI0_IRQHandler(void)
{

}

void EXTI2_IRQHandler(void)
{

}


void EXTI4_IRQHandler(void)
{
	if(EXTI_GetITStatus(KEY5_IRQ_EXTI_Line) != RESET)
	{		 
		EXTI_ClearITPendingBit(KEY5_IRQ_EXTI_Line);			
		if( gbl_ar_exti_key_service[extiKey5ServiceFunction].run != NULL )
		{
			gbl_ar_exti_key_service[extiKey5ServiceFunction].run();
		}		
	}
}


void EXTI15_10_IRQHandler(void)
{

}

