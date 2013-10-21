

#ifndef  USART_PRESENT
#define  USART_PRESENT

#include <stm32f10x_usart.h>

#define BAUDRATE_485 9600

/* ------------------------------------------------------------------------------------------------- */
/* BSP USART */
/* ------------------------------------------------------------------------------------------------- */
typedef enum
{
	usartmodeDMA,
	usartmodeIRQ,
	usartmodeMAX
} usartmode_type;

typedef enum
{
	usart1ServiceFunction,
	usart2ServiceFunction,
	usart3ServiceFunction,
	usartServiceFunctionMAX
} usart_register_function_type;

typedef            void     (*usart_register_function)(char data);

typedef struct _usart_service_function_type
{
	usart_register_function_type service_type;
	usart_register_function run;
} usart_service_function_type;

/* ------------------------------------------------------------------------------------------------- */
/* function USART */
/* ------------------------------------------------------------------------------------------------- */

void USART1_IRQHandler(void);
void USART2_RQHandler(void);


/* ------------------------------------------------------------------------------------------------- */
/* extern USART */
/* ------------------------------------------------------------------------------------------------- */

extern void init_usart1_buffer(void);				    
extern void usart_transmit_byte( USART_TypeDef* port, u16 chr);
extern void usart1_transmit_byte( u16 chr);
extern void usart1_tx_proc(void);

extern void usart_transmit_string(USART_TypeDef* port, const void *data);
extern void usart1_transmit_string(const void *data);
extern void usart1_transmit_string_format(const char * szFormat, ... );


extern void bsp_init_irq_usart1(void/*isr_function usart1_isr*/);

extern int usart_is_ne(USART_TypeDef* port);
extern int usart1_is_ne(void);
extern void* usart1_get_data(void);

extern void bsp_init_irq_usart2(void/*isr_function usart2_isr*/);
extern void usart2_transmit_byte(u16 chr);
extern void usart2_transmit_string(char* data);

extern void register_usart_function(usart_register_function_type usart_fn_type, usart_register_function fn);
extern void bsp_init_irq_usart3(void/*isr_function usart3_isr*/);
extern void usart3_transmit_byte(u16 chr);
extern void usart3_transmit_string(char* data);

#endif                                                          

/* End of module include.                               */

