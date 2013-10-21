

#ifndef __CAN_H
#define __CAN_H



#define CAN_PORT  	GPIOB
#define CAN_TX_PIN 	GPIO_Pin_9
#define CAN_RX_PIN 	GPIO_Pin_8

void bsp_can_gpio_init(void);
void bsp_can_init(void);
void bsp_can_interrupt_init(void);
int can_polling(void);
int can_interrupt(void);
#endif

