

#include  <stm32f10x.h>
#include "led.h"


const bsp_led_core_group_type led_core_group[ledCoreMax]=
{
	{LED_USER_CORE_PORT, LED_USER_CORE_PIN }
};

const bsp_led_bottom_group_type led_bottom_group[ledBottomMax]=
{
	{LED_USER_BOTTOM1_PORT, LED_USER_BOTTOM1_PIN },
	{LED_USER_BOTTOM2_PORT, LED_USER_BOTTOM2_PIN },
	{LED_USER_BOTTOM3_PORT, LED_USER_BOTTOM3_PIN }
};

void bsp_led_gpio_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	/* Configure the GPIO ports */
	GPIO_InitStructure.GPIO_Pin =  LED_USER_CORE_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(LED_USER_CORE_PORT, &GPIO_InitStructure);	

	/* Configure the GPIO ports */
	GPIO_InitStructure.GPIO_Pin =  LED_USER_BOTTOM1_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(LED_USER_BOTTOM1_PORT, &GPIO_InitStructure);	

	GPIO_InitStructure.GPIO_Pin =  LED_USER_BOTTOM2_PIN;
	GPIO_Init(LED_USER_BOTTOM2_PORT, &GPIO_InitStructure);	

	GPIO_InitStructure.GPIO_Pin =  LED_USER_BOTTOM3_PIN;
	GPIO_Init(LED_USER_BOTTOM3_PORT, &GPIO_InitStructure);	

}

void bsp_led_core_on(BSP_LED_CORE_Def led)
{
	if( led >= ledCoreMax )
		return;

	GPIO_SetBits(led_core_group[led].gpio_reg, led_core_group[led].pin);
}

void bsp_led_core_off(BSP_LED_CORE_Def led)
{
	if( led >= ledCoreMax )
		return;

	GPIO_ResetBits(led_core_group[led].gpio_reg, led_core_group[led].pin);
}

void bsp_led_core_toggle(BSP_LED_CORE_Def led)
{
	s32 pins;
	if( led >= ledCoreMax )
		return;

	pins = GPIO_ReadOutputData(led_core_group[ led ].gpio_reg);
	if ((pins & led_core_group[led].pin) == 0)
	{
		GPIO_SetBits(led_core_group[led].gpio_reg, led_core_group[led].pin);
	}
	else
	{
		GPIO_ResetBits(led_core_group[led].gpio_reg, led_core_group[led].pin);
	}	
}

void bsp_led_bottom_on(BSP_LED_BOTTOM_Def led)
{
	if( led >= ledBottomMax )
		return;

	GPIO_SetBits(led_bottom_group[led].gpio_reg, led_bottom_group[led].pin);
}

void bsp_led_bottom_off(BSP_LED_BOTTOM_Def led)
{
	if( led >= ledBottomMax )
		return;

	GPIO_ResetBits(led_bottom_group[led].gpio_reg, led_bottom_group[led].pin);
}

void bsp_led_bottom_toggle(BSP_LED_BOTTOM_Def led)
{
	s32 pins;
	if( led >= ledBottomMax )
		return;

	pins = GPIO_ReadOutputData(led_bottom_group[ led ].gpio_reg);
	if ((pins & led_bottom_group[led].pin) == 0)
	{
		GPIO_SetBits(led_bottom_group[led].gpio_reg, led_bottom_group[led].pin);
	}
	else
	{
		GPIO_ResetBits(led_bottom_group[led].gpio_reg, led_bottom_group[led].pin);
	}	
}

