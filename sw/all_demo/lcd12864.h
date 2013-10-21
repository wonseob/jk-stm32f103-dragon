#ifndef __LCD12864_H
#define __LCD12864_H

// ST7920 LCD driver 12864 driver, 3-wire serial mode

/*
Pin Definition: RS (CS )=====> PG0
          RW (SID )====> PG1
          EN (SCLK )===> PG2
          PSB ========> PG3 / / low level for the serial mode
*/

#define LCD12864_EN_PORT  				GPIOD
#define LCD12864_EN_PIN 				GPIO_Pin_4
#define LCD12864_EN_HIGH()     			GPIO_SetBits(LCD12864_EN_PORT, LCD12864_EN_PIN)  
#define LCD12864_EN_LOW()     			GPIO_ResetBits(LCD12864_EN_PORT, LCD12864_EN_PIN)  

#define LCD12864_RW_PORT  				GPIOD
#define LCD12864_RW_PIN 				GPIO_Pin_5
#define LCD12864_RW_HIGH()     			GPIO_SetBits(LCD12864_RW_PORT, LCD12864_RW_PIN)  
#define LCD12864_RW_LOW()     			GPIO_ResetBits(LCD12864_RW_PORT, LCD12864_RW_PIN)  

#define LCD12864_RS_PORT  				GPIOD
#define LCD12864_RS_PIN 				GPIO_Pin_11
#define LCD12864_RS_HIGH()     			GPIO_SetBits(LCD12864_RS_PORT, LCD12864_RS_PIN)  
#define LCD12864_RS_LOW()     			GPIO_ResetBits(LCD12864_RS_PORT, LCD12864_RS_PIN)  

#define LCD12864_PSB_PORT  				GPIOE
#define LCD12864_PSB_PIN 				GPIO_Pin_5
#define LCD12864_PSB_HIGH()     		GPIO_SetBits(LCD12864_RS_PORT, LCD12864_RS_PIN)  
#define LCD12864_PSB_LOW()     			GPIO_ResetBits(LCD12864_RS_PORT, LCD12864_RS_PIN)  


#define xtal    16
#define left    0x18
#define right   0x1c

#define Set_CS()   LCD12864_RS_HIGH()
#define Set_SID()  LCD12864_RW_HIGH()
#define Set_SCLK() LCD12864_EN_HIGH()

#define Clr_CS()   LCD12864_RS_LOW()
#define Clr_SID()  LCD12864_RW_LOW()
#define Clr_SCLK() LCD12864_EN_LOW()


void W_1byte(u8 RW, u8 RS, u8 W_data);
void Write_8bits(u16 W_bits);
void LCD_Init0(void);
void LCD_Init1(void);
void LCD12864_Char_Set(u8 y, u8 x, u8 *p);
void Set_Draw(void);
void Draw_Pic(u8 x, u8 y, const u8 *Draw);
void bsp_lcd12864_gpio_init(void);
void bsp_lcd12864_init(void);
void Lcd_flash(u16 delay_t,u8 times);

#endif



