
#ifndef __TOUCH_H__
#define __TOUCH_H__

#define TP_CS()  GPIO_ResetBits(GPIOE,GPIO_Pin_5)
#define TP_DCS() GPIO_SetBits(GPIOE,GPIO_Pin_5)

#define PEN  GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_6)
#define NPEN !(0x01&PEN)     //!PEN	

typedef struct 
{
	u16 X0;
	u16 Y0;
	u16 X;
	u16 Y;						   	    
	u8  Key_Sta;
	u8  Key_LSta;

	float xfac;
	float yfac;
	short xoff;
	short yoff;
} Pen_Holder;




#define Key_Down 0x01
#define Key_Up   0x00 

#define CLICK_POINT 0X01   
#define MOVE_LEFT   0X10
#define MOVE_RIGHT  0X20
#define MOVE_UP     0X40
#define MOVE_DOWN   0X80

void bsp_touch_spi_init(void);
void bsp_touch_gpio_init_polling(void);
void bsp_touch_gpio_init_intettupt(void);
void bsp_touch_nvic_init(void);
void bsp_touch_init_polling(void);
void bsp_touch_init_interrupt(void);

void lcd43_touch_adjust_auto(void);
void lcd43_touch_adjust(void);

void lcd70_touch_adjust_auto(void);
void lcd70_touch_adjust(void);
u16 convert2pos(void);
void lcd_touch_interrupt_set(u8 en);
u8 lcd_touch_read_once(void);

void lcd28_touch_adjust(void);
void lcd32_touch_adjust(void);

void EXTI9_5_IRQHandler(void);

#endif



















