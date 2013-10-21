
#ifndef __ADC_H
#define __ADC_H

#define ADC12_IN8_PORT  	GPIOB
#define ADC12_IN8_PIN	 	GPIO_Pin_0

#define ADC12_IN9_PORT  	GPIOB
#define ADC12_IN9_PIN	 	GPIO_Pin_1

#define ADC12_IN15_PORT  	GPIOC
#define ADC12_IN15_PIN	 	GPIO_Pin_5

void bsp_adc_gpio_init(uint8_t ADC_Channel);
void bsp_adc_init(uint8_t ADC_Channel);


#endif

