

#ifndef _BUZZER_H_
#define _BUZZER_H_

#define BUZZER_OUT_PORT  	GPIOB
#define BUZZER_OUT_PIN	 	GPIO_Pin_5

void bsp_buzzer_gpio_init(void);
void bsp_buzzer_init(void);
void buzzer_on(void);
void buzzer_off(void);

#endif


