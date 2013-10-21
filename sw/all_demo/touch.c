

#include <stdlib.h>
#include <math.h>

#include "hw_config.h"
#include "touch.h" 
#include "lcd.h"
#include "usart.h"
#include "spi.h"
#include "lcd43.h"
#include "lcd70.h"
#include "lcd28.h"
#include "lcd32.h"

#define LCD_TOUCH_DBG_LOG

// ADS7843/7846/UH7843/7846/XPT2046/TSC2046
// 7.0 인치
//#define CMD_RDX 0XD0  //0B10010000 read a differential manner X coordinate
//#define CMD_RDY	0X90  //0B11010000 read Y coordinates with a differential approach
//#define TEMP_RD	0XF0  //0B11110000 read Y coordinates with a differential approach

// 4.3 인치
//#define CMD_RDY 0XD0  //0B10010000 read a differential manner X coordinate
//#define CMD_RDX	0X90  //0B11010000 read Y coordinates with a differential approach

Pen_Holder Pen_Point;

extern unsigned int  POINT_COLOR;
extern unsigned int  BACK_COLOR;

extern tft_lcd_type curr_tft_lcd;
extern Pen_Holder Pen_Point;

u16 demo_selected_pen_color = BLACK;



void bsp_touch_spi_init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure; 
	SPI_InitTypeDef   SPI_InitStructure; 

	/* SPI1 SCK(PA5)、MISO(PA6)、MOSI(PA7) */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	// Touch CS
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOE, &GPIO_InitStructure);	
	
	#if 1
	// SPI1 NSS
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_SetBits(GPIOA, GPIO_Pin_4);
	#endif
	
	/* SPI1 init*/ 
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;					
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;				
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;						
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;					
	// NSS Soft
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;						
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_64;
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;				   
	SPI_InitStructure.SPI_CRCPolynomial = 7;
	SPI_Init(SPI1, &SPI_InitStructure);
	
	/* SPI1 Enable */  
	SPI_Cmd(SPI1,ENABLE);

}

void bsp_touch_gpio_init_polling(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	// Touch interrupt
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(GPIOE, &GPIO_InitStructure);
}

void bsp_touch_gpio_init_intettupt(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	// Touch interrupt
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
	GPIO_Init(GPIOE, &GPIO_InitStructure);	
}

void bsp_touch_nvic_init(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;
	
	/* Configure one bit for preemption priority */
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	
	/* Enable the EXTI9_5 Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn  ;
	
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	GPIO_EXTILineConfig(GPIO_PortSourceGPIOE, GPIO_PinSource6);
	
	/* Configure Key Button EXTI Line to generate an interrupt on falling edge */  
	EXTI_InitStructure.EXTI_Line = EXTI_Line6;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);
}


void bsp_touch_init_polling(void)
{
	bsp_touch_gpio_init_polling();
	bsp_touch_spi_init(); 
}

void bsp_touch_init_interrupt(void)
{
	bsp_touch_gpio_init_intettupt();
	bsp_touch_nvic_init();
	bsp_touch_spi_init(); 
}

unsigned char spi1_write_byte(unsigned char data) 
{ 
	unsigned char Data = 0; 
	
	
	while(SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_TXE)==RESET); 	
	SPI_I2S_SendData(SPI1,data);
	
	while(SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_RXNE)==RESET); 	
	Data = SPI_I2S_ReceiveData(SPI1); 
	
	return Data; 
} 

void lcd28_touch_point_draw(u16 x,u16 y, u16 color)
{
	POINT_COLOR = color;

	lcd28_draw_line(x-12,y,x+13,y); // horizontal line
	lcd28_draw_line(x,y-12,x,y+13); // vertical line
	lcd28_draw_point(x+1,y+1);
	lcd28_draw_point(x-1,y+1);
	lcd28_draw_point(x+1,y-1);
	lcd28_draw_point(x-1,y-1);
	// lcd28_draw_circle(x,y,6); // draw center circle
}

void lcd32_touch_point_draw(u16 x,u16 y, u16 color)
{
	POINT_COLOR = color;

	lcd32_draw_line(x-12,y,x+13,y); // horizontal line
	lcd32_draw_line(x,y-12,x,y+13); // vertical line
	lcd32_draw_point(x+1,y+1);
	lcd32_draw_point(x-1,y+1);
	lcd32_draw_point(x+1,y-1);
	lcd32_draw_point(x-1,y-1);
	// lcd32_draw_circle(x,y,6); // draw center circle
}


void lcd70_touch_point_draw(u16 x,u16 y, u16 color)
{
	lcd70_draw_line(x-12,y,x+13,y, color); // horizontal line
	lcd70_draw_line(x,y-12,x,y+13, color); // vertical line
	lcd70_draw_point(x+1,y+1, color);
	lcd70_draw_point(x-1,y+1, color);
	lcd70_draw_point(x+1,y-1, color);
	lcd70_draw_point(x-1,y-1, color);
	// lcd70_draw_circle(x,y,6, color); // draw center circle
}

void lcd43_touch_point_draw(u16 x,u16 y, u16 color)
{
	lcd43_draw_line(x-12,y,x+13,y, color); // horizontal line
	lcd43_draw_line(x,y-12,x,y+13, color); // vertical line
	lcd43_draw_point(x+1,y+1, color);
	lcd43_draw_point(x-1,y+1, color);
	lcd43_draw_point(x+1,y-1, color);
	lcd43_draw_point(x-1,y-1, color);
	// lcd70_draw_circle(x,y,6, color); // draw center circle
}

// SPI read data
// Read adc value from the 7846/7843/XPT2046/UH7843/UH7846	   
u16 read_xpt2046_spi(u8 CMD)	  
{ 	 
   u16 y=0;
   TP_CS();	                        
   
   delay_us(10);
   spi1_write_byte(CMD);			

   delay_us(10);
   y = spi1_write_byte(0x00);
   y <<= 8;   

   y |= spi1_write_byte(0x00);

   TP_DCS(); 					    								  
   y = y>>3;
   //y = y>>4;
   return (y); 
	
}

// Read the value of a coordinate
// Sequential read READ_TIMES time data, the data in ascending order,
// Then remove the minimum and maximum LOST_VAL number of averaged
#define READ_TIMES 30 // reads
#define LOST_VAL 10	  // discard value
u16 read_xpt2046(u8 xy)
{
	u16 i, j;
	u16 buf[READ_TIMES];
	u16 sum=0;
	u16 temp;

	
	for(i=0;i<READ_TIMES;i++)
	{				 
		buf[i]=read_xpt2046_spi(xy);	    
	}
	

	for(i=0;i<READ_TIMES-1; i++) // sort
	{
		for(j=i+1;j<READ_TIMES;j++)
		{
			if(buf[i]>buf[j]) // ascending order
			{
				temp=buf[i];
				buf[i]=buf[j];
				buf[j]=temp;
			}
		}
	}	  
	sum=0;
	for(i=LOST_VAL;i<READ_TIMES-LOST_VAL;i++)
		sum+=buf[i];
	temp=sum/(READ_TIMES-2*LOST_VAL);

	return temp;   
} 

// Read the coordinates with Filter
// Minimum value of not less than 100.
u8 read_xpt2046_xy(u16 *x,u16 *y)
{
	u16 xtemp,ytemp;		
	
// 7.0 인치
//#define CMD_RDX 0XD0  //0B10010000 read a differential manner X coordinate
//#define CMD_RDY	0X90  //0B11010000 read Y coordinates with a differential approach
//#define TEMP_RD	0XF0  //0B11110000 read Y coordinates with a differential approach

// 4.3 인치
//#define CMD_RDY 0XD0  //0B10010000 read a differential manner X coordinate
//#define CMD_RDX	0X90  //0B11010000 read Y coordinates with a differential approach

	if( curr_tft_lcd == tft43_lcd )
	{	
		xtemp=read_xpt2046(0X90);	
		ytemp=read_xpt2046(0XD0);
	}
	else
	{	
		xtemp=read_xpt2046(0XD0);	
		ytemp=read_xpt2046(0X90);
	}
	
	//xtemp>>=1;	  
#if 0													   
	if(xtemp<100||ytemp<100)
	{
#ifdef LCD_TOUCH_DBG_LOG
		usart1_transmit_string("\r\n Read_ADS read failure");
#endif
		return 0; // read failure
	}
#endif
	*x=xtemp;
	*y=ytemp;

	return 1; // reading success
}

#define ERR_RANGE 50 // error range 
u8 lcd_touch_read_xpt2046_xy(u16 *x,u16 *y) 
{
	u16 x1,y1;
 	u16 x2,y2;
 	u8 flag;    

    flag=read_xpt2046_xy(&x1,&y1);   
    if(flag==0)
	{
		return(0);
	}

    flag=read_xpt2046_xy(&x2,&y2);	   
    if(flag==0)
	{
		return(0);   
	}
#ifdef LCD_TOUCH_DBG_LOG
	usart1_transmit_string_format("\r\n>>x1=%d, x2=%d, y1=%d, y2=%d", x1, x2, y1, y2);
	//usart1_transmit_string_format("\r\n>>g1=%d, g2=%d, g3=%d, g4=%d", x1<x2+ERR_RANGE, x2<x1+ERR_RANGE, y1<y2+ERR_RANGE, y2<y1+ERR_RANGE);
#endif

    if(
		((x2<=x1 && x1<x2+ERR_RANGE)||(x1<=x2 && x2<x1+ERR_RANGE)) // before and after the two samples within the +-50
    	&&
		((y2<=y1 && y1<y2+ERR_RANGE)||(y1<=y2 && y2<y1+ERR_RANGE))
	  )
    {

        *x=(x1+x2)/2;
        *y=(y1+y2)/2;
#ifdef LCD_TOUCH_DBG_LOG
		usart1_transmit_string_format("\r\n>>*x=%d, *y=%d", *x, *y);
#endif
        return 1;
    }
	else
	{ 
#ifdef LCD_TOUCH_DBG_LOG
		usart1_transmit_string("\r\n Read_ADS2 read failure");
#endif
		return 0;	  
	}
} 


// PEN interrupt settings
void lcd_touch_interrupt_set(u8 en)
{
	EXTI_InitTypeDef EXTI_InitStructure;

	if(en)
	{
		/* Configure Key Button EXTI Line to generate an interrupt on falling edge */  
		EXTI_InitStructure.EXTI_Line = EXTI_Line6;
		EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
		EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
		EXTI_InitStructure.EXTI_LineCmd = ENABLE;
		EXTI_Init(&EXTI_InitStructure);

		// EXTI->IMR|=1<<1;     // turn on interrupts on line1 	
	}
		
	else 
	{
		/* Configure Key Button EXTI Line to generate an interrupt on falling edge */  
		EXTI_InitStructure.EXTI_Line = EXTI_Line6;
		EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
		EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
		EXTI_InitStructure.EXTI_LineCmd = DISABLE;
		EXTI_Init(&EXTI_InitStructure);

		// EXTI->IMR&=~(1<<1); // turn off interrupts on line1 	
	}

}

// Read the first coordinates
// Only read once, that release before returning PEN!				   
u8 lcd_touch_read_once(void)
{
	u8 t=0;	    
	lcd_touch_interrupt_set(0); // turn off interrupts
	Pen_Point.Key_Sta=Key_Up;
	if( lcd_touch_read_xpt2046_xy(&Pen_Point.X,&Pen_Point.Y) == 1 )
	{

		while(PEN==0&&t<=250)
		{
			t++;
			delay_ms(10);
		};
	
		lcd_touch_interrupt_set(1); // Open interrupt
		if(t>=250)
		{
	#ifdef LCD_TOUCH_DBG_LOG
			usart1_transmit_string("\r\n Press 2.5s considered invalid");
	#endif
			Pen_Point.Key_Sta=Key_Down;
			return 0; // Press 2.5s considered invalid
		}
		else 
			return 1;
	}
	else
	{
		Pen_Point.Key_Sta=Key_Down;
		lcd_touch_interrupt_set(1); // Open interrupt
#ifdef LCD_TOUCH_DBG_LOG
		usart1_transmit_string("\r\n Read_ADS2 read failure fsf");
#endif
		return 0;
	}
}



void lcd28_touch_adjust(void)
{

	u16 cal_distance = 20;

	u16 pos_temp[4][2]; // coordinates of the cache value
	u8 cnt = 0;
	u16 d1, d2;
	u32 tem1, tem2;
	float fac;
	
	cnt = 0;
	lcd28_clear_screen(WHITE);

	lcd28_touch_point_draw(cal_distance,cal_distance, BLUE); // draw point 3

	Pen_Point.Key_Sta = Key_Up; // elimination of the trigger signal
	Pen_Point.xfac = 0; // xfac used to mark whether or calibrated, so the calibration must be cleared away before! To avoid errors

#ifdef LCD_TOUCH_DBG_LOG
	usart1_transmit_string("\r\n tp-0");
#endif
	
	while (1)
	{
		if (Pen_Point.Key_Sta == Key_Down) // button pressed
		{
#ifdef LCD_TOUCH_DBG_LOG
		usart1_transmit_string("\r\n tp-1");
#endif
			if (lcd_touch_read_once ())// get the value of a single button
			{

				pos_temp[cnt][0] = Pen_Point.X;
				pos_temp[cnt][1] = Pen_Point.Y;

				cnt++;
			

				switch (cnt)
				{
				case 1:
					lcd28_clear_screen(WHITE); // clear the screen
					lcd28_touch_point_draw(240-cal_distance,cal_distance, BLUE); // draw point 2				
					break;
				case 2:
					lcd28_clear_screen(WHITE); // clear the screen
					lcd28_touch_point_draw(cal_distance,320-cal_distance, BLUE); // draw point 3
					break;
				case 3:
					lcd28_clear_screen(WHITE); // clear the screen
					lcd28_touch_point_draw(240-cal_distance,320-cal_distance, BLUE); // draw point 4
					break;
				case 4: // all of the four points have been
					// On the same side
					tem1 = abs (pos_temp[0][0]-pos_temp[1][0]);// x1-x2
					tem2 = abs (pos_temp[0][1]-pos_temp[1][1]);// y1-y2
					tem1 *= tem1;
					tem2 *= tem2;
					d1 = sqrt (tem1 + tem2); // get the distance 1,2
	
					tem1 = abs (pos_temp[2][0]-pos_temp[3][0]);// x3-x4
					tem2 = abs (pos_temp[2][1]-pos_temp[3][1]);// y3-y4
					tem1 *= tem1;
					tem2 *= tem2;
					d2 = sqrt(tem1 + tem2); // get the distance 3,4
					fac = (float) d1/d2;
	
	#ifdef LCD_TOUCH_DBG_LOG
					usart1_transmit_string_format("\r\npos_temp[0][0]=%d", pos_temp[0][0]);
					usart1_transmit_string_format("\r\npos_temp[1][0]=%d", pos_temp[1][0]);
					usart1_transmit_string_format("\r\npos_temp[0][1]=%d", pos_temp[0][1]);
					usart1_transmit_string_format("\r\npos_temp[1][1]=%d", pos_temp[1][1]);
	
					usart1_transmit_string_format("\r\npos_temp[2][0]=%d", pos_temp[2][0]);
					usart1_transmit_string_format("\r\npos_temp[3][0]=%d", pos_temp[3][0]);
					usart1_transmit_string_format("\r\npos_temp[2][1]=%d", pos_temp[2][1]);
					usart1_transmit_string_format("\r\npos_temp[3][1]=%d", pos_temp[3][1]);
					
					usart1_transmit_string_format("\r\n fac1=%4.2f", fac);
	#endif
					if (fac < 0.3 || fac > 1.3 || d1 == 0 || d2 == 0) // fail
					{
						cnt = 0;
						lcd28_clear_screen (WHITE); // clear the screen
						lcd28_touch_point_draw(cal_distance,cal_distance, BLUE); // draw point 3
	#ifdef LCD_TOUCH_DBG_LOG
						usart1_transmit_string_format("\r\n >>1 if (fac < 0.95 || fac > 1.05), d1=%d, d2=%d", fac, d1, d2);
	#endif
						continue;
					}
					
					tem1 = abs (pos_temp[0][0]-pos_temp[2][0]);// x1-x3
					tem2 = abs (pos_temp[0][1]-pos_temp[2][1]);// y1-y3
					tem1 *= tem1;
					tem2 *= tem2;
					d1 = sqrt (tem1 + tem2); // get the distance 1,3
	
	#ifdef LCD_TOUCH_DBG_LOG
					usart1_transmit_string_format("\r\npos_temp[0][0]=%d", pos_temp[0][0]);
					usart1_transmit_string_format("\r\npos_temp[2][0]=%d", pos_temp[2][0]);
					usart1_transmit_string_format("\r\npos_temp[0][1]=%d", pos_temp[0][1]);
					usart1_transmit_string_format("\r\npos_temp[2][1]=%d", pos_temp[2][1]);
	#endif
	
					tem1 = abs (pos_temp[1][0]-pos_temp[3][0]);// x2-x4
					tem2 = abs (pos_temp[1][1]-pos_temp[3][1]);// y2-y4
					tem1 *= tem1;
					tem2 *= tem2;
					d2 = sqrt (tem1 + tem2); // get distance of 2,4
					fac = (float) d1/d2;
	
	#ifdef LCD_TOUCH_DBG_LOG
					usart1_transmit_string_format("\r\npos_temp[1][0]=%d", pos_temp[1][0]);
					usart1_transmit_string_format("\r\npos_temp[3][0]=%d", pos_temp[3][0]);
					usart1_transmit_string_format("\r\npos_temp[1][1]=%d", pos_temp[1][1]);
					usart1_transmit_string_format("\r\npos_temp[3][1]=%d", pos_temp[3][1]);
					
					usart1_transmit_string_format("\r\n fac2=%4.2f", fac);
	#endif
					if (fac < 0.3 || fac > 1.3) // fail
					{
						cnt = 0;
						lcd28_clear_screen (WHITE); // clear the screen
						lcd28_touch_point_draw(cal_distance,cal_distance, BLUE); // draw point 3
	#ifdef LCD_TOUCH_DBG_LOG
						usart1_transmit_string_format("\r\n >>1 if (fac < 0.95 || fac > 1.05)", fac);
	#endif
						continue;
					} // Correct the
	
					// Diagonal equal
					tem1 = abs (pos_temp[1][0]-pos_temp[2][0]);// x1-x3
					tem2 = abs (pos_temp[1][1]-pos_temp[2][1]);// y1-y3
					tem1 *= tem1;
					tem2 *= tem2;
					d1 = sqrt (tem1 + tem2); // get the distance of 1,4
	
	#ifdef LCD_TOUCH_DBG_LOG
					usart1_transmit_string_format("\r\npos_temp[1][0]=%d", pos_temp[1][0]);
					usart1_transmit_string_format("\r\npos_temp[2][0]=%d", pos_temp[2][0]);
					usart1_transmit_string_format("\r\npos_temp[1][1]=%d", pos_temp[1][1]);
					usart1_transmit_string_format("\r\npos_temp[2][1]=%d", pos_temp[2][1]);
	#endif
	
					tem1 = abs (pos_temp[0][0]-pos_temp[3][0]);// x2-x4
					tem2 = abs (pos_temp[0][1]-pos_temp[3][1]);// y2-y4
					tem1 *= tem1;
					tem2 *= tem2;
					d2 = sqrt (tem1 + tem2); // get the distance 2,3
					fac = (float) d1/d2;
	
	#ifdef LCD_TOUCH_DBG_LOG
					usart1_transmit_string_format("\r\npos_temp[0][0]=%d", pos_temp[0][0]);
					usart1_transmit_string_format("\r\npos_temp[3][0]=%d", pos_temp[3][0]);
					usart1_transmit_string_format("\r\npos_temp[0][1]=%d", pos_temp[0][1]);
					usart1_transmit_string_format("\r\npos_temp[3][1]=%d", pos_temp[3][1]);
					
					usart1_transmit_string_format("\r\n fac3=%4.2f", fac);
	#endif
					if (fac < 0.3 || fac > 1.3) // fail
					{
						cnt = 0;
						lcd28_clear_screen(WHITE); // clear the screen
						lcd28_touch_point_draw(cal_distance,cal_distance, BLUE); // draw point 3
	#ifdef LCD_TOUCH_DBG_LOG
						usart1_transmit_string_format("\r\n >>>>>> if (fac < 0.95 || fac > 1.05), d1=%d, d2=%d", fac, d1, d2);
	#endif
						continue;
					} // Correct the
					
					// Results			
					
					Pen_Point.xfac=(float)(240-cal_distance*2)/(pos_temp[1][0]-pos_temp[0][0]);//xfac		 
					Pen_Point.xoff=(240-Pen_Point.xfac*(pos_temp[1][0]+pos_temp[0][0]))/2;//xoff
						  
					Pen_Point.yfac=(float)(320-cal_distance*2)/(pos_temp[2][1]-pos_temp[0][1]);//yfac
					Pen_Point.yoff=(320-Pen_Point.yfac*(pos_temp[2][1]+pos_temp[0][1]))/2;//yoff
	
	
	#ifdef LCD_TOUCH_DBG_LOG
					usart1_transmit_string_format("\r\npos_temp[0][0]=%d", pos_temp[0][0]);
					usart1_transmit_string_format("\r\npos_temp[1][0]=%d", pos_temp[1][0]);
					usart1_transmit_string_format("\r\npos_temp[0][1]=%d", pos_temp[0][1]);
					usart1_transmit_string_format("\r\npos_temp[2][1]=%d", pos_temp[2][1]);
	
					usart1_transmit_string_format("\r\nPen_Point.xfac=%4.2f", Pen_Point.xfac);
					usart1_transmit_string_format("\r\nPen_Point.xoff=%d", Pen_Point.xoff);
					usart1_transmit_string_format("\r\nPen_Point.yfac=%4.2f", Pen_Point.yfac);
					usart1_transmit_string_format("\r\nPen_Point.yoff=%d", Pen_Point.yoff);
	#endif
	
	
					lcd28_clear_screen(WHITE); // clear the screen
	
	
					return; // calibration is completed,
				}  
			}
			else
			{
#ifdef LCD_TOUCH_DBG_LOG
		usart1_transmit_string("\r\n tp-2");
#endif
			}
		}
	}
}

void lcd32_touch_adjust(void)
{

	u16 cal_distance = 20;

	u16 pos_temp[4][2]; // coordinates of the cache value
	u8 cnt = 0;
	u16 d1, d2;
	u32 tem1, tem2;
	float fac;
	
	cnt = 0;
	lcd32_clear_screen(WHITE);

	lcd32_touch_point_draw(cal_distance,cal_distance, BLUE); // draw point 3

	Pen_Point.Key_Sta = Key_Up; // elimination of the trigger signal
	Pen_Point.xfac = 0; // xfac used to mark whether or calibrated, so the calibration must be cleared away before! To avoid errors

#ifdef LCD_TOUCH_DBG_LOG
	usart1_transmit_string("\r\n tp-0");
#endif
	
	while (1)
	{
		if (Pen_Point.Key_Sta == Key_Down) // button pressed
		{
#ifdef LCD_TOUCH_DBG_LOG
		usart1_transmit_string("\r\n tp-1");
#endif
			if (lcd_touch_read_once ())// get the value of a single button
			{

				pos_temp[cnt][0] = Pen_Point.X;
				pos_temp[cnt][1] = Pen_Point.Y;

				cnt++;
			

				switch (cnt)
				{
				case 1:
					lcd32_clear_screen(WHITE); // clear the screen
					lcd32_touch_point_draw(240-cal_distance,cal_distance, BLUE); // draw point 2				
					break;
				case 2:
					lcd32_clear_screen(WHITE); // clear the screen
					lcd32_touch_point_draw(cal_distance,320-cal_distance, BLUE); // draw point 3
					break;
				case 3:
					lcd32_clear_screen(WHITE); // clear the screen
					lcd32_touch_point_draw(240-cal_distance,320-cal_distance, BLUE); // draw point 4
					break;
				case 4: // all of the four points have been
					// On the same side
					tem1 = abs (pos_temp[0][0]-pos_temp[1][0]);// x1-x2
					tem2 = abs (pos_temp[0][1]-pos_temp[1][1]);// y1-y2
					tem1 *= tem1;
					tem2 *= tem2;
					d1 = sqrt (tem1 + tem2); // get the distance 1,2
	
					tem1 = abs (pos_temp[2][0]-pos_temp[3][0]);// x3-x4
					tem2 = abs (pos_temp[2][1]-pos_temp[3][1]);// y3-y4
					tem1 *= tem1;
					tem2 *= tem2;
					d2 = sqrt(tem1 + tem2); // get the distance 3,4
					fac = (float) d1/d2;
	
	#ifdef LCD_TOUCH_DBG_LOG
					usart1_transmit_string_format("\r\npos_temp[0][0]=%d", pos_temp[0][0]);
					usart1_transmit_string_format("\r\npos_temp[1][0]=%d", pos_temp[1][0]);
					usart1_transmit_string_format("\r\npos_temp[0][1]=%d", pos_temp[0][1]);
					usart1_transmit_string_format("\r\npos_temp[1][1]=%d", pos_temp[1][1]);
	
					usart1_transmit_string_format("\r\npos_temp[2][0]=%d", pos_temp[2][0]);
					usart1_transmit_string_format("\r\npos_temp[3][0]=%d", pos_temp[3][0]);
					usart1_transmit_string_format("\r\npos_temp[2][1]=%d", pos_temp[2][1]);
					usart1_transmit_string_format("\r\npos_temp[3][1]=%d", pos_temp[3][1]);
					
					usart1_transmit_string_format("\r\n fac1=%4.2f", fac);
	#endif
					if (fac < 0.3 || fac > 1.3 || d1 == 0 || d2 == 0) // fail
					{
						cnt = 0;
						lcd32_clear_screen (WHITE); // clear the screen
						lcd32_touch_point_draw(cal_distance,cal_distance, BLUE); // draw point 3
	#ifdef LCD_TOUCH_DBG_LOG
						usart1_transmit_string_format("\r\n >>1 if (fac < 0.95 || fac > 1.05), d1=%d, d2=%d", fac, d1, d2);
	#endif
						continue;
					}
					
					tem1 = abs (pos_temp[0][0]-pos_temp[2][0]);// x1-x3
					tem2 = abs (pos_temp[0][1]-pos_temp[2][1]);// y1-y3
					tem1 *= tem1;
					tem2 *= tem2;
					d1 = sqrt (tem1 + tem2); // get the distance 1,3
	
	#ifdef LCD_TOUCH_DBG_LOG
					usart1_transmit_string_format("\r\npos_temp[0][0]=%d", pos_temp[0][0]);
					usart1_transmit_string_format("\r\npos_temp[2][0]=%d", pos_temp[2][0]);
					usart1_transmit_string_format("\r\npos_temp[0][1]=%d", pos_temp[0][1]);
					usart1_transmit_string_format("\r\npos_temp[2][1]=%d", pos_temp[2][1]);
	#endif
	
					tem1 = abs (pos_temp[1][0]-pos_temp[3][0]);// x2-x4
					tem2 = abs (pos_temp[1][1]-pos_temp[3][1]);// y2-y4
					tem1 *= tem1;
					tem2 *= tem2;
					d2 = sqrt (tem1 + tem2); // get distance of 2,4
					fac = (float) d1/d2;
	
	#ifdef LCD_TOUCH_DBG_LOG
					usart1_transmit_string_format("\r\npos_temp[1][0]=%d", pos_temp[1][0]);
					usart1_transmit_string_format("\r\npos_temp[3][0]=%d", pos_temp[3][0]);
					usart1_transmit_string_format("\r\npos_temp[1][1]=%d", pos_temp[1][1]);
					usart1_transmit_string_format("\r\npos_temp[3][1]=%d", pos_temp[3][1]);
					
					usart1_transmit_string_format("\r\n fac2=%4.2f", fac);
	#endif
					if (fac < 0.3 || fac > 1.3) // fail
					{
						cnt = 0;
						lcd32_clear_screen (WHITE); // clear the screen
						lcd32_touch_point_draw(cal_distance,cal_distance, BLUE); // draw point 3
	#ifdef LCD_TOUCH_DBG_LOG
						usart1_transmit_string_format("\r\n >>1 if (fac < 0.95 || fac > 1.05)", fac);
	#endif
						continue;
					} // Correct the
	
					// Diagonal equal
					tem1 = abs (pos_temp[1][0]-pos_temp[2][0]);// x1-x3
					tem2 = abs (pos_temp[1][1]-pos_temp[2][1]);// y1-y3
					tem1 *= tem1;
					tem2 *= tem2;
					d1 = sqrt (tem1 + tem2); // get the distance of 1,4
	
	#ifdef LCD_TOUCH_DBG_LOG
					usart1_transmit_string_format("\r\npos_temp[1][0]=%d", pos_temp[1][0]);
					usart1_transmit_string_format("\r\npos_temp[2][0]=%d", pos_temp[2][0]);
					usart1_transmit_string_format("\r\npos_temp[1][1]=%d", pos_temp[1][1]);
					usart1_transmit_string_format("\r\npos_temp[2][1]=%d", pos_temp[2][1]);
	#endif
	
					tem1 = abs (pos_temp[0][0]-pos_temp[3][0]);// x2-x4
					tem2 = abs (pos_temp[0][1]-pos_temp[3][1]);// y2-y4
					tem1 *= tem1;
					tem2 *= tem2;
					d2 = sqrt (tem1 + tem2); // get the distance 2,3
					fac = (float) d1/d2;
	
	#ifdef LCD_TOUCH_DBG_LOG
					usart1_transmit_string_format("\r\npos_temp[0][0]=%d", pos_temp[0][0]);
					usart1_transmit_string_format("\r\npos_temp[3][0]=%d", pos_temp[3][0]);
					usart1_transmit_string_format("\r\npos_temp[0][1]=%d", pos_temp[0][1]);
					usart1_transmit_string_format("\r\npos_temp[3][1]=%d", pos_temp[3][1]);
					
					usart1_transmit_string_format("\r\n fac3=%4.2f", fac);
	#endif
					if (fac < 0.3 || fac > 1.3) // fail
					{
						cnt = 0;
						lcd32_clear_screen(WHITE); // clear the screen
						lcd32_touch_point_draw(cal_distance,cal_distance, BLUE); // draw point 3
	#ifdef LCD_TOUCH_DBG_LOG
						usart1_transmit_string_format("\r\n >>>>>> if (fac < 0.95 || fac > 1.05), d1=%d, d2=%d", fac, d1, d2);
	#endif
						continue;
					} // Correct the
					
					// Results			
					
					Pen_Point.xfac=(float)(240-cal_distance*2)/(pos_temp[1][0]-pos_temp[0][0]);//xfac		 
					Pen_Point.xoff=(240-Pen_Point.xfac*(pos_temp[1][0]+pos_temp[0][0]))/2;//xoff
						  
					Pen_Point.yfac=(float)(320-cal_distance*2)/(pos_temp[2][1]-pos_temp[0][1]);//yfac
					Pen_Point.yoff=(320-Pen_Point.yfac*(pos_temp[2][1]+pos_temp[0][1]))/2;//yoff
	
	
	#ifdef LCD_TOUCH_DBG_LOG
					usart1_transmit_string_format("\r\npos_temp[0][0]=%d", pos_temp[0][0]);
					usart1_transmit_string_format("\r\npos_temp[1][0]=%d", pos_temp[1][0]);
					usart1_transmit_string_format("\r\npos_temp[0][1]=%d", pos_temp[0][1]);
					usart1_transmit_string_format("\r\npos_temp[2][1]=%d", pos_temp[2][1]);
	
					usart1_transmit_string_format("\r\nPen_Point.xfac=%4.2f", Pen_Point.xfac);
					usart1_transmit_string_format("\r\nPen_Point.xoff=%d", Pen_Point.xoff);
					usart1_transmit_string_format("\r\nPen_Point.yfac=%4.2f", Pen_Point.yfac);
					usart1_transmit_string_format("\r\nPen_Point.yoff=%d", Pen_Point.yoff);
	#endif
	
	
					lcd32_clear_screen(WHITE); // clear the screen
	
	
					return; // calibration is completed,
				}  
			}
			else
			{
#ifdef LCD_TOUCH_DBG_LOG
		usart1_transmit_string("\r\n tp-2");
#endif
			}
		}
	}
}


void lcd43_touch_adjust_auto(void)
{
	//Pen_Point.xfac= -0.20793951;
	u16 pos_temp[4][2];
	
	pos_temp[0][0]=3762;	//x1
	pos_temp[1][0]=442;	//x2
	
	pos_temp[0][1]=3200;   //y1
	pos_temp[2][1]=761;	//y3

	Pen_Point.xfac=(float)440/(pos_temp[1][0]-pos_temp[0][0]);
	Pen_Point.xoff=(480-Pen_Point.xfac*(pos_temp[1][0]+pos_temp[0][0]))/2;
		  
	Pen_Point.yfac=(float)232/(pos_temp[2][1]-pos_temp[0][1]);
	Pen_Point.yoff=(272-Pen_Point.yfac*(pos_temp[2][1]+pos_temp[0][1]))/2;
 
 // Pen_Point.xfac= -0.132530;

 // Pen_Point.xoff= 518;
 // Pen_Point.yfac= -0.095121;
 // Pen_Point.yoff= 324;

 //	Pen_Point.X0=Pen_Point.xfac*Pen_Point.X0+Pen_Point.xoff;
//		Pen_Point.Y0=Pen_Point.yfac*Pen_Point.Y0+Pen_Point.yoff;

//		tempx=Pen_Point.xfac*tempx+Pen_Point.xoff;
//		tempy=Pen_Point.yfac*tempy+Pen_Point.yoff;
}


void lcd43_touch_adjust(void)
{

	u16 cal_distance = 20;

	u16 pos_temp[4][2]; // coordinates of the cache value
	u8 cnt = 0;
	u16 d1, d2;
	u32 tem1, tem2;
	float fac;
	
	cnt = 0;
	lcd43_clear_screen(WHITE);

	lcd43_touch_point_draw(cal_distance,cal_distance, BLUE); // draw point 3

	Pen_Point.Key_Sta = Key_Up; // elimination of the trigger signal
	Pen_Point.xfac = 0; // xfac used to mark whether or calibrated, so the calibration must be cleared away before! To avoid errors

#ifdef LCD_TOUCH_DBG_LOG
	usart1_transmit_string("\r\n tp-0");
#endif
	
	while (1)
	{
		if (Pen_Point.Key_Sta == Key_Down) // button pressed
		{
#ifdef LCD_TOUCH_DBG_LOG
		usart1_transmit_string("\r\n tp-1");
#endif
			if (lcd_touch_read_once ())// get the value of a single button
			{

				pos_temp[cnt][0] = Pen_Point.X;
				pos_temp[cnt][1] = Pen_Point.Y;

				cnt++;
			

				switch (cnt)
				{
				case 1:
					lcd43_clear_screen(WHITE); // clear the screen
					lcd43_touch_point_draw(480-cal_distance,cal_distance, BLUE); // draw point 2				
					break;
				case 2:
					lcd43_clear_screen(WHITE); // clear the screen
					lcd43_touch_point_draw(cal_distance,272-cal_distance, BLUE); // draw point 3
					break;
				case 3:
					lcd43_clear_screen(WHITE); // clear the screen
					lcd43_touch_point_draw(480-cal_distance,272-cal_distance, BLUE); // draw point 4
					break;
				case 4: // all of the four points have been
					// On the same side
					tem1 = abs (pos_temp[0][0]-pos_temp[1][0]);// x1-x2
					tem2 = abs (pos_temp[0][1]-pos_temp[1][1]);// y1-y2
					tem1 *= tem1;
					tem2 *= tem2;
					d1 = sqrt (tem1 + tem2); // get the distance 1,2
	
					tem1 = abs (pos_temp[2][0]-pos_temp[3][0]);// x3-x4
					tem2 = abs (pos_temp[2][1]-pos_temp[3][1]);// y3-y4
					tem1 *= tem1;
					tem2 *= tem2;
					d2 = sqrt(tem1 + tem2); // get the distance 3,4
					fac = (float) d1/d2;
	
	#ifdef LCD_TOUCH_DBG_LOG
					usart1_transmit_string_format("\r\npos_temp[0][0]=%d", pos_temp[0][0]);
					usart1_transmit_string_format("\r\npos_temp[1][0]=%d", pos_temp[1][0]);
					usart1_transmit_string_format("\r\npos_temp[0][1]=%d", pos_temp[0][1]);
					usart1_transmit_string_format("\r\npos_temp[1][1]=%d", pos_temp[1][1]);
	
					usart1_transmit_string_format("\r\npos_temp[2][0]=%d", pos_temp[2][0]);
					usart1_transmit_string_format("\r\npos_temp[3][0]=%d", pos_temp[3][0]);
					usart1_transmit_string_format("\r\npos_temp[2][1]=%d", pos_temp[2][1]);
					usart1_transmit_string_format("\r\npos_temp[3][1]=%d", pos_temp[3][1]);
					
					usart1_transmit_string_format("\r\n fac1=%4.2f", fac);
	#endif
					if (fac < 0.3 || fac > 1.3 || d1 == 0 || d2 == 0) // fail
					{
						cnt = 0;
						lcd70_clear_screen (WHITE); // clear the screen
						lcd70_touch_point_draw(cal_distance,cal_distance, BLUE); // draw point 3
	#ifdef LCD_TOUCH_DBG_LOG
						usart1_transmit_string_format("\r\n >>1 if (fac < 0.95 || fac > 1.05), d1=%d, d2=%d", fac, d1, d2);
	#endif
						continue;
					}
					
					tem1 = abs (pos_temp[0][0]-pos_temp[2][0]);// x1-x3
					tem2 = abs (pos_temp[0][1]-pos_temp[2][1]);// y1-y3
					tem1 *= tem1;
					tem2 *= tem2;
					d1 = sqrt (tem1 + tem2); // get the distance 1,3
	
	#ifdef LCD_TOUCH_DBG_LOG
					usart1_transmit_string_format("\r\npos_temp[0][0]=%d", pos_temp[0][0]);
					usart1_transmit_string_format("\r\npos_temp[2][0]=%d", pos_temp[2][0]);
					usart1_transmit_string_format("\r\npos_temp[0][1]=%d", pos_temp[0][1]);
					usart1_transmit_string_format("\r\npos_temp[2][1]=%d", pos_temp[2][1]);
	#endif
	
					tem1 = abs (pos_temp[1][0]-pos_temp[3][0]);// x2-x4
					tem2 = abs (pos_temp[1][1]-pos_temp[3][1]);// y2-y4
					tem1 *= tem1;
					tem2 *= tem2;
					d2 = sqrt (tem1 + tem2); // get distance of 2,4
					fac = (float) d1/d2;
	
	#ifdef LCD_TOUCH_DBG_LOG
					usart1_transmit_string_format("\r\npos_temp[1][0]=%d", pos_temp[1][0]);
					usart1_transmit_string_format("\r\npos_temp[3][0]=%d", pos_temp[3][0]);
					usart1_transmit_string_format("\r\npos_temp[1][1]=%d", pos_temp[1][1]);
					usart1_transmit_string_format("\r\npos_temp[3][1]=%d", pos_temp[3][1]);
					
					usart1_transmit_string_format("\r\n fac2=%4.2f", fac);
	#endif
					if (fac < 0.3 || fac > 1.3) // fail
					{
						cnt = 0;
						lcd70_clear_screen (WHITE); // clear the screen
						lcd70_touch_point_draw(cal_distance,cal_distance, BLUE); // draw point 3
	#ifdef LCD_TOUCH_DBG_LOG
						usart1_transmit_string_format("\r\n >>1 if (fac < 0.95 || fac > 1.05)", fac);
	#endif
						continue;
					} // Correct the
	
					// Diagonal equal
					tem1 = abs (pos_temp[1][0]-pos_temp[2][0]);// x1-x3
					tem2 = abs (pos_temp[1][1]-pos_temp[2][1]);// y1-y3
					tem1 *= tem1;
					tem2 *= tem2;
					d1 = sqrt (tem1 + tem2); // get the distance of 1,4
	
	#ifdef LCD_TOUCH_DBG_LOG
					usart1_transmit_string_format("\r\npos_temp[1][0]=%d", pos_temp[1][0]);
					usart1_transmit_string_format("\r\npos_temp[2][0]=%d", pos_temp[2][0]);
					usart1_transmit_string_format("\r\npos_temp[1][1]=%d", pos_temp[1][1]);
					usart1_transmit_string_format("\r\npos_temp[2][1]=%d", pos_temp[2][1]);
	#endif
	
					tem1 = abs (pos_temp[0][0]-pos_temp[3][0]);// x2-x4
					tem2 = abs (pos_temp[0][1]-pos_temp[3][1]);// y2-y4
					tem1 *= tem1;
					tem2 *= tem2;
					d2 = sqrt (tem1 + tem2); // get the distance 2,3
					fac = (float) d1/d2;
	
	#ifdef LCD_TOUCH_DBG_LOG
					usart1_transmit_string_format("\r\npos_temp[0][0]=%d", pos_temp[0][0]);
					usart1_transmit_string_format("\r\npos_temp[3][0]=%d", pos_temp[3][0]);
					usart1_transmit_string_format("\r\npos_temp[0][1]=%d", pos_temp[0][1]);
					usart1_transmit_string_format("\r\npos_temp[3][1]=%d", pos_temp[3][1]);
					
					usart1_transmit_string_format("\r\n fac3=%4.2f", fac);
	#endif
					if (fac < 0.3 || fac > 1.3) // fail
					{
						cnt = 0;
						lcd70_clear_screen(WHITE); // clear the screen
						lcd70_touch_point_draw(cal_distance,cal_distance, BLUE); // draw point 3
	#ifdef LCD_TOUCH_DBG_LOG
						usart1_transmit_string_format("\r\n >>>>>> if (fac < 0.95 || fac > 1.05), d1=%d, d2=%d", fac, d1, d2);
	#endif
						continue;
					} // Correct the
					
					// Results			
					
					Pen_Point.xfac=(float)(480-cal_distance*2)/(pos_temp[1][0]-pos_temp[0][0]);//xfac		 
					Pen_Point.xoff=(480-Pen_Point.xfac*(pos_temp[1][0]+pos_temp[0][0]))/2;//xoff
						  
					Pen_Point.yfac=(float)(272-cal_distance*2)/(pos_temp[2][1]-pos_temp[0][1]);//yfac
					Pen_Point.yoff=(272-Pen_Point.yfac*(pos_temp[2][1]+pos_temp[0][1]))/2;//yoff
	
	
	#ifdef LCD_TOUCH_DBG_LOG
					usart1_transmit_string_format("\r\npos_temp[0][0]=%d", pos_temp[0][0]);
					usart1_transmit_string_format("\r\npos_temp[1][0]=%d", pos_temp[1][0]);
					usart1_transmit_string_format("\r\npos_temp[0][1]=%d", pos_temp[0][1]);
					usart1_transmit_string_format("\r\npos_temp[2][1]=%d", pos_temp[2][1]);
	
					usart1_transmit_string_format("\r\nPen_Point.xfac=%4.2f", Pen_Point.xfac);
					usart1_transmit_string_format("\r\nPen_Point.xoff=%d", Pen_Point.xoff);
					usart1_transmit_string_format("\r\nPen_Point.yfac=%4.2f", Pen_Point.yfac);
					usart1_transmit_string_format("\r\nPen_Point.yoff=%d", Pen_Point.yoff);
	#endif
	
	
					lcd43_clear_screen(WHITE); // clear the screen
	
	
					return; // calibration is completed,
				}  
			}
			else
			{
#ifdef LCD_TOUCH_DBG_LOG
		usart1_transmit_string("\r\n tp-2");
#endif
			}
		}
	}
}



void lcd70_touch_adjust_auto(void)
{					
	u16 cal_distance = 100;
	u16 pos_temp[4][2]; // coordinates of the cache value

/*
	pos_temp[0][0]=3084;//3212;//3250;//3300;//3400;//3619;//3019;	//x1
	pos_temp[1][0]=812;//657;//480;//252;//452;	//x2
	
	pos_temp[0][1]=1221;//1042;//800;//602;   //y1
	pos_temp[2][1]=2804;//3063;//3200;//3300;//3490;//3854;	//y3
*/
	pos_temp[0][0]=2885;
	pos_temp[1][0]=865;

	pos_temp[0][1]=1274;
	pos_temp[2][1]=2699;
				
				
					
	Pen_Point.xfac=(float)(800-cal_distance*2)/(pos_temp[1][0]-pos_temp[0][0]);//xfac		 
	Pen_Point.xoff=(800-Pen_Point.xfac*(pos_temp[1][0]+pos_temp[0][0]))/2;//xoff
		  
	Pen_Point.yfac=(float)(480-cal_distance*2)/(pos_temp[2][1]-pos_temp[0][1]);//yfac
	Pen_Point.yoff=(480-Pen_Point.yfac*(pos_temp[2][1]+pos_temp[0][1]))/2;//yoff
}


// Touch screen calibration code
// Get the four calibration parameters
void lcd70_touch_adjust(void)									   
{
	u16 cal_distance = 100;

	u16 pos_temp[4][2]; // coordinates of the cache value
	u8 cnt = 0;
	u16 d1, d2;
	u32 tem1, tem2;
	float fac;
	
	cnt = 0;

	lcd70_clear_screen(WHITE);


	lcd70_touch_point_draw(cal_distance,cal_distance, BLUE); // draw point 3

	Pen_Point.Key_Sta = Key_Up; // elimination of the trigger signal
	Pen_Point.xfac = 0; // xfac used to mark whether or calibrated, so the calibration must be cleared away before! To avoid errors



#ifdef LCD_TOUCH_DBG_LOG
	usart1_transmit_string("\r\n tp-0");
#endif
	
	while (1)
	{
		if (Pen_Point.Key_Sta == Key_Down) // button pressed
		{
#ifdef LCD_TOUCH_DBG_LOG
		usart1_transmit_string("\r\n tp-1");
#endif
			if (lcd_touch_read_once ())// get the value of a single button
			{

				pos_temp[cnt][0] = Pen_Point.X;
				pos_temp[cnt][1] = Pen_Point.Y;

				cnt++;			

				switch (cnt)
				{
				case 1:
					lcd70_clear_screen(WHITE); // clear the screen
					lcd70_touch_point_draw(800-cal_distance,cal_distance, BLUE); // draw point 2				
					break;
				case 2:
					lcd70_clear_screen(WHITE); // clear the screen
					lcd70_touch_point_draw(cal_distance,480-cal_distance, BLUE); // draw point 3
					break;
				case 3:
					lcd70_clear_screen(WHITE); // clear the screen
					lcd70_touch_point_draw(800-cal_distance,480-cal_distance, BLUE); // draw point 4
					break;
				case 4: // all of the four points have been
					// On the same side
					tem1 = abs (pos_temp[0][0]-pos_temp[1][0]);// x1-x2
					tem2 = abs (pos_temp[0][1]-pos_temp[1][1]);// y1-y2
					tem1 *= tem1;
					tem2 *= tem2;
					d1 = sqrt (tem1 + tem2); // get the distance 1,2
	
					tem1 = abs (pos_temp[2][0]-pos_temp[3][0]);// x3-x4
					tem2 = abs (pos_temp[2][1]-pos_temp[3][1]);// y3-y4
					tem1 *= tem1;
					tem2 *= tem2;
					d2 = sqrt(tem1 + tem2); // get the distance 3,4
					fac = (float) d1/d2;
	
	#ifdef LCD_TOUCH_DBG_LOG
					usart1_transmit_string_format("\r\npos_temp[0][0]=%d", pos_temp[0][0]);
					usart1_transmit_string_format("\r\npos_temp[1][0]=%d", pos_temp[1][0]);
					usart1_transmit_string_format("\r\npos_temp[0][1]=%d", pos_temp[0][1]);
					usart1_transmit_string_format("\r\npos_temp[1][1]=%d", pos_temp[1][1]);
	
					usart1_transmit_string_format("\r\npos_temp[2][0]=%d", pos_temp[2][0]);
					usart1_transmit_string_format("\r\npos_temp[3][0]=%d", pos_temp[3][0]);
					usart1_transmit_string_format("\r\npos_temp[2][1]=%d", pos_temp[2][1]);
					usart1_transmit_string_format("\r\npos_temp[3][1]=%d", pos_temp[3][1]);
					
					usart1_transmit_string_format("\r\n fac1=%4.2f", fac);
	#endif
					if (fac < 0.3 || fac > 1.3 || d1 == 0 || d2 == 0) // fail
					{
						cnt = 0;
						lcd70_clear_screen (WHITE); // clear the screen
						lcd70_touch_point_draw(cal_distance,cal_distance, BLUE); // draw point 3
	#ifdef LCD_TOUCH_DBG_LOG
						usart1_transmit_string_format("\r\n >>1 if (fac < 0.95 || fac > 1.05), d1=%d, d2=%d", fac, d1, d2);
	#endif
						continue;
					}
					
					tem1 = abs (pos_temp[0][0]-pos_temp[2][0]);// x1-x3
					tem2 = abs (pos_temp[0][1]-pos_temp[2][1]);// y1-y3
					tem1 *= tem1;
					tem2 *= tem2;
					d1 = sqrt (tem1 + tem2); // get the distance 1,3
	
	#ifdef LCD_TOUCH_DBG_LOG
					usart1_transmit_string_format("\r\npos_temp[0][0]=%d", pos_temp[0][0]);
					usart1_transmit_string_format("\r\npos_temp[2][0]=%d", pos_temp[2][0]);
					usart1_transmit_string_format("\r\npos_temp[0][1]=%d", pos_temp[0][1]);
					usart1_transmit_string_format("\r\npos_temp[2][1]=%d", pos_temp[2][1]);
	#endif
	
					tem1 = abs (pos_temp[1][0]-pos_temp[3][0]);// x2-x4
					tem2 = abs (pos_temp[1][1]-pos_temp[3][1]);// y2-y4
					tem1 *= tem1;
					tem2 *= tem2;
					d2 = sqrt (tem1 + tem2); // get distance of 2,4
					fac = (float) d1/d2;
	
	#ifdef LCD_TOUCH_DBG_LOG
					usart1_transmit_string_format("\r\npos_temp[1][0]=%d", pos_temp[1][0]);
					usart1_transmit_string_format("\r\npos_temp[3][0]=%d", pos_temp[3][0]);
					usart1_transmit_string_format("\r\npos_temp[1][1]=%d", pos_temp[1][1]);
					usart1_transmit_string_format("\r\npos_temp[3][1]=%d", pos_temp[3][1]);
					
					usart1_transmit_string_format("\r\n fac2=%4.2f", fac);
	#endif
					if (fac < 0.3 || fac > 1.3) // fail
					{
						cnt = 0;
						lcd70_clear_screen (WHITE); // clear the screen
						lcd70_touch_point_draw(cal_distance,cal_distance, BLUE); // draw point 3
	#ifdef LCD_TOUCH_DBG_LOG
						usart1_transmit_string_format("\r\n >>1 if (fac < 0.95 || fac > 1.05)", fac);
	#endif
						continue;
					} // Correct the
	
					// Diagonal equal
					tem1 = abs (pos_temp[1][0]-pos_temp[2][0]);// x1-x3
					tem2 = abs (pos_temp[1][1]-pos_temp[2][1]);// y1-y3
					tem1 *= tem1;
					tem2 *= tem2;
					d1 = sqrt (tem1 + tem2); // get the distance of 1,4
	
	#ifdef LCD_TOUCH_DBG_LOG
					usart1_transmit_string_format("\r\npos_temp[1][0]=%d", pos_temp[1][0]);
					usart1_transmit_string_format("\r\npos_temp[2][0]=%d", pos_temp[2][0]);
					usart1_transmit_string_format("\r\npos_temp[1][1]=%d", pos_temp[1][1]);
					usart1_transmit_string_format("\r\npos_temp[2][1]=%d", pos_temp[2][1]);
	#endif
	
					tem1 = abs (pos_temp[0][0]-pos_temp[3][0]);// x2-x4
					tem2 = abs (pos_temp[0][1]-pos_temp[3][1]);// y2-y4
					tem1 *= tem1;
					tem2 *= tem2;
					d2 = sqrt (tem1 + tem2); // get the distance 2,3
					fac = (float) d1/d2;
	
	#ifdef LCD_TOUCH_DBG_LOG
					usart1_transmit_string_format("\r\npos_temp[0][0]=%d", pos_temp[0][0]);
					usart1_transmit_string_format("\r\npos_temp[3][0]=%d", pos_temp[3][0]);
					usart1_transmit_string_format("\r\npos_temp[0][1]=%d", pos_temp[0][1]);
					usart1_transmit_string_format("\r\npos_temp[3][1]=%d", pos_temp[3][1]);
					
					usart1_transmit_string_format("\r\n fac3=%4.2f", fac);
	#endif
					if (fac < 0.3 || fac > 1.3) // fail
					{
						cnt = 0;
						lcd70_clear_screen(WHITE); // clear the screen
						lcd70_touch_point_draw(cal_distance,cal_distance, BLUE); // draw point 3
	#ifdef LCD_TOUCH_DBG_LOG
						usart1_transmit_string_format("\r\n >>>>>> if (fac < 0.95 || fac > 1.05), d1=%d, d2=%d", fac, d1, d2);
	#endif
						continue;
					} // Correct the
					
					// Results					
					Pen_Point.xfac=(float)(800-cal_distance*2)/(pos_temp[1][0]-pos_temp[0][0]);//xfac		 
					Pen_Point.xoff=(800-Pen_Point.xfac*(pos_temp[1][0]+pos_temp[0][0]))/2;//xoff
						  
					Pen_Point.yfac=(float)(480-cal_distance*2)/(pos_temp[2][1]-pos_temp[0][1]);//yfac
					Pen_Point.yoff=(480-Pen_Point.yfac*(pos_temp[2][1]+pos_temp[0][1]))/2;//yoff
	
	
	#ifdef LCD_TOUCH_DBG_LOG
					usart1_transmit_string_format("\r\npos_temp[0][0]=%d", pos_temp[0][0]);
					usart1_transmit_string_format("\r\npos_temp[1][0]=%d", pos_temp[1][0]);
					usart1_transmit_string_format("\r\npos_temp[0][1]=%d", pos_temp[0][1]);
					usart1_transmit_string_format("\r\npos_temp[2][1]=%d", pos_temp[2][1]);
	
					usart1_transmit_string_format("\r\nPen_Point.xfac=%4.2f", Pen_Point.xfac);
					usart1_transmit_string_format("\r\nPen_Point.xoff=%d", Pen_Point.xoff);
					usart1_transmit_string_format("\r\nPen_Point.yfac=%4.2f", Pen_Point.yfac);
					usart1_transmit_string_format("\r\nPen_Point.yoff=%d", Pen_Point.yoff);
	#endif
	
	

					lcd70_clear_screen(WHITE); // clear the screen
	
	/*
	lcd70_touch_point_draw(cal_distance,cal_distance, BLUE); // draw point 3
	lcd70_touch_point_draw(800-cal_distance,cal_distance, BLUE); // draw point 2				
	lcd70_touch_point_draw(cal_distance,480-cal_distance, BLUE); // draw point 3
	lcd70_touch_point_draw(800-cal_distance,480-cal_distance, BLUE); // draw point 4
	*/
	
					return; // calibration is completed,
				}  
			}
			else
			{
#ifdef LCD_TOUCH_DBG_LOG
		usart1_transmit_string("\r\n tp-2");
#endif
			}
		}
	}


}

// Convert results
// According to a touch screen calibration parameters to determine the conversion result stored in X0, Y0 in
u16 convert2pos(void)
{		 	  
	if(lcd_touch_read_xpt2046_xy(&Pen_Point.X,&Pen_Point.Y))
	{
		
		Pen_Point.X0=Pen_Point.xfac*Pen_Point.X+Pen_Point.xoff;
		Pen_Point.Y0=Pen_Point.yfac*Pen_Point.Y+Pen_Point.yoff;  

		//usart1_transmit_string_format("\r\n Pen_Point.X0=%d, Pen_Point.Y0=%d", Pen_Point.X0, Pen_Point.Y0);
		return 1;
	}
	else
		return 0;
}

void EXTI9_5_IRQHandler(void)
{ 
#if 1
	if(EXTI_GetITStatus(EXTI_Line6) != RESET)
	{
		Pen_Point.Key_Sta = Key_Down;

#if 0
		lcd_touch_read_xpt2046_xy(&Pen_Point.X,&Pen_Point.Y);
		usart1_transmit_string_format("\r\n Pen_Point.X0=%d, Pen_Point.Y0=%d", Pen_Point.X, Pen_Point.Y);		
#endif
		
		EXTI_ClearITPendingBit(EXTI_Line6);
	}
#endif
}

