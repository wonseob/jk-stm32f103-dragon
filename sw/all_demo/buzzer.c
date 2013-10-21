
#include "hw_config.h"
#include "buzzer.h"


void bsp_buzzer_gpio_init(void) 
{
	GPIO_InitTypeDef GPIO_InitStructure;						 					
	
  	GPIO_InitStructure.GPIO_Pin =  BUZZER_OUT_PIN;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  	GPIO_Init(BUZZER_OUT_PORT, &GPIO_InitStructure);
}

void bsp_buzzer_init(void) 
{
	bsp_buzzer_gpio_init();
}

void buzzer_on(void) 
{
	// TR is active low.
	GPIO_ResetBits(BUZZER_OUT_PORT, BUZZER_OUT_PIN);	
}

void buzzer_off(void) 
{
	GPIO_SetBits(BUZZER_OUT_PORT, BUZZER_OUT_PIN);	
}

