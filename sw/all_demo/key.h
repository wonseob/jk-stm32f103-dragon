
#ifndef  KEY_PRESENT
#define  KEY_PRESENT

#include "hw_config.h"
#include <stm32f10x.h>


typedef enum
{
	key1User = 0x00,
	key2User,
	key3User,
	key4User,
	key5User,
	key6User,
	key7User,
	keyMax
} BSP_KEY_Def;

// polling
#define KEY1_USER_PORT				GPIOC
#define KEY1_USER_PIN				GPIO_Pin_0
#define KEY2_USER_PORT				GPIOC
#define KEY2_USER_PIN				GPIO_Pin_1
#define KEY3_USER_PORT				GPIOC
#define KEY3_USER_PIN				GPIO_Pin_2
#define KEY4_USER_PORT				GPIOC
#define KEY4_USER_PIN				GPIO_Pin_3

// use exti interrupt
#define KEY5_USER_PORT				GPIOC
#define KEY5_USER_PIN				GPIO_Pin_4



// KEY IRQ Pin define
#define KEY5_IRQ_PORT_SOURCE		GPIO_PortSourceGPIOC
#define KEY5_IRQ_PIN_SOURCE			GPIO_PinSource4

// KEY IRQ External Line
#define KEY5_IRQ_EXTI_Line			EXTI_Line4

// KEY IRQ channel
#define KEY5_IRQ_CHANNEL			EXTI4_IRQn


// KEY Value
#define KEY_NONE	0x00
#define KEY_KEY1	0x01
#define KEY_KEY2	0x02
#define KEY_KEY3	0x04
#define KEY_KEY4	0x08
#define KEY_KEY5	0x10



typedef struct
{
	GPIO_TypeDef* gpio_reg;
	u16          pin;
}bsp_key_group_type;


typedef enum
{
	extiKey5ServiceFunction = 0x00,
	extiKeyServiceFunctionMAX
} exti_key_register_function_type;

typedef            void     (*exti_key_register_function)(void);

typedef struct _exti_key_service_function_type
{
	exti_key_register_function_type service_type;
	exti_key_register_function run;
} exti_key_service_function_type;



/* ------------------------------------------------------------------------------------------------- */
/* BSP KEY */
/* ------------------------------------------------------------------------------------------------- */

extern void register_exti_key_function(exti_key_register_function_type exti_key_fn_type, exti_key_register_function fn);
extern void bsp_key_interrupt_init(void);
extern void bsp_key_gpio_init(void);

void EXTI0_IRQHandler(void);
void EXTI2_IRQHandler(void);
void EXTI4_IRQHandler(void);
void EXTI15_10_IRQHandler(void);
	
#endif
