
#ifndef __DAC_H
#define __DAC_H

#define DAC_OUT1_PORT  		GPIOA
#define DAC_OUT1_PIN	 	GPIO_Pin_4

void bsp_dac_gpio_init(void);
void bsp_dac_init(void);


#endif

