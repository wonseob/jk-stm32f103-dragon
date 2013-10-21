#ifndef __LCD_H
#define __LCD_H


typedef enum
{
	tft28_lcd = 0,
	tft32_lcd,
	tft43_lcd,
	tft70_lcd,
	tft_lcd_max
} tft_lcd_type;

typedef enum {
	LCD_PORTRAIT_TOP_DOWN,
	LCD_PORTRAIT_BOTTOM_UP,
	LCD_LANDSCAPE_TOP_DOWN,
	LCD_LANDSCAPE_BOTTOM_UP
} LCD_OrientationMode_t;

#define LCD_PORTRAIT           0
#define LCD_LANDSCAPE          1

#define LCD_LINE_HORIZONTAL     0x00
#define LCD_LINE_VERTICAL       0x01


#define Bank1_LCD_D    ((uint32_t)0x60020000)    //disp Data ADDR
#define Bank1_LCD_C    ((uint32_t)0x60000000)	 //disp Reg ADDR




// LCD PWM-LIGHT
#define LCD_PWM_LIGHT_PORT  		GPIOD
#define LCD_PWM_LIGHT_PIN 			GPIO_Pin_13
#define LCD_PWM_LIGHT_HIGH()     	GPIO_SetBits(LCD_PWM_LIGHT_PORT, LCD_PWM_LIGHT_PIN)  
#define LCD_PWM_LIGHT_LOW()     	GPIO_ResetBits(LCD_PWM_LIGHT_PORT, LCD_PWM_LIGHT_PIN)  

#define LCD_RESET_PORT  			GPIOE
#define LCD_RESET_PIN 				GPIO_Pin_1
#define LCD_RESET_HIGH()     		GPIO_SetBits(LCD_RESET_PORT, LCD_RESET_PIN)  
#define LCD_RESET_LOW()     		GPIO_ResetBits(LCD_RESET_PORT, LCD_RESET_PIN)  

#define LCD_CS_PORT  				GPIOD
#define LCD_CS_PIN 					GPIO_Pin_7
#define LCD_CS_HIGH()     			GPIO_SetBits(LCD_CS_PORT, LCD_CS_PIN)  
#define LCD_CS_LOW()     			GPIO_ResetBits(LCD_CS_PORT, LCD_CS_PIN)  

#define LCD_RD_PORT  				GPIOD
#define LCD_RD_PIN 					GPIO_Pin_4
#define LCD_RD_HIGH()     			GPIO_SetBits(LCD_RD_PORT, LCD_RD_PIN)  
#define LCD_RD_LOW()     			GPIO_ResetBits(LCD_RD_PORT, LCD_RD_PIN)  

#define LCD_WR_PORT  				GPIOD
#define LCD_WR_PIN 					GPIO_Pin_5
#define LCD_WR_HIGH()     			GPIO_SetBits(LCD_WR_PORT, LCD_WR_PIN)  
#define LCD_WR_LOW()     			GPIO_ResetBits(LCD_WR_PORT, LCD_WR_PIN)  

#define LCD_RS_PORT  				GPIOD
#define LCD_RS_PIN 					GPIO_Pin_11
#define LCD_RS_HIGH()     			GPIO_SetBits(LCD_RS_PORT, LCD_RS_PIN)  
#define LCD_RS_LOW()     			GPIO_ResetBits(LCD_RS_PORT, LCD_RS_PIN)  

#define LCD_FSMC_NBL0_PORT  		GPIOE
#define LCD_FSMC_NBL0_PIN 			GPIO_Pin_0

// LCD COLOR
#define RED	 			0XF800
#define GREEN 			0X07E0
#define BLUE 			0X001F  
#define BRED 			0XF81F
#define GRED 			0XFFE0
#define GBLUE			0X07FF
#define WHITE 			0Xffff
#define BLACK 			0X0000
#define BROWN 			0XBC40
#define BRRED 			0XFC07
#define GRAY  			0X8430
#define LGRAY 			0XC618
#define YELLOW   	 	0xFFE0
 
#define FSIZE 16			



void bsp_fsmc_lcd_init(void);
void bsp_lcd_gpio_init(void);
void lcd_rst(void);
void lcd_wr_reg(unsigned int index);
void lcd_wr_reg_value(u16 LCD_Reg, u16 LCD_RegValue);
void lcd_wr_cmd(unsigned int index);
unsigned int lcd_rd_data(void);
void lcd_wr_data(unsigned int val);
void lcd_wr_data_8(unsigned int val);

u32 mn(u8 m,u8 n);
u32 mypow (u8 m, u8 n);

uint16_t LCD_RGB2Pixel565(uint8_t r, uint8_t g, uint8_t b);

#define LCD_WR_REG(x)			lcd_wr_reg(x)
#define LCD_WR_Data(x)			lcd_wr_data(x)	
	  		 
#endif  
	 



