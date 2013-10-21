
#include  <stm32f10x.h>
#include "hw_config.h"
#include "lcd.h" 
#include "lcd43.h"
#include "stdlib.h"
#include "usart.h"	


#define LCD43_DBG_LOG

unsigned int  HDP=479;

unsigned int  HT=531;
unsigned int  HPS=43;
unsigned int  LPS=8;
unsigned char HPW=10;

unsigned int  VDP=271;
unsigned int  VT=288;
unsigned int  VPS=12;
unsigned int  FPS=4;
unsigned char VPW=10;


extern const unsigned char asc2_1206[95][12];
extern const unsigned char asc2_1608[95][16];
extern const unsigned char icon_demo[63720];
extern const unsigned char BIG_ASCII[195][16];

extern unsigned int  POINT_COLOR;
extern unsigned int  BACK_COLOR;

static LCD_OrientationMode_t orientation_mode = LCD43_ORIENTATION_DEFAULT;

// 480 * 272
void lcd43_clear_screen(unsigned int color)
{                    
	unsigned int l=480,w;
	
	lcd_wr_reg(0x002A);	
	lcd_wr_data(0);	    
	lcd_wr_data(0);
	lcd_wr_data(HDP>>8);	    
	lcd_wr_data(HDP&0x00ff);
	lcd_wr_reg(0x002b);	
	lcd_wr_data(0);	    
	lcd_wr_data(0);
	lcd_wr_data(VDP>>8);	    
	lcd_wr_data(VDP&0x00ff);
	lcd_wr_reg(0x002c);
	
	
	while(l--)
	{
	    for(w=0;w<272;w++)
		{    
	      	lcd_wr_data(color);
		}
	} 
}

void bsp_lcd43_init(void)
{
	bsp_lcd_gpio_init();

	delay_ms(10);

	LCD_CS_HIGH();

	// LCD Data(0 ~ 7)
	GPIO_SetBits(GPIOD, GPIO_Pin_14| GPIO_Pin_15 |GPIO_Pin_0 | GPIO_Pin_1);  	 
	GPIO_SetBits(GPIOE, GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10);   

	
	LCD_RESET_LOW();

	LCD_RD_HIGH();
	LCD_WR_HIGH();

	delay_ms(10);
	bsp_fsmc_lcd_init();
	delay_ms(10);

#ifdef LCD43_DBG_LOG
	usart1_transmit_string("\r\nfsmc init\r\n");
#endif

	lcd_rst();

	delay_ms(20);
	delay_ms(20);
	delay_ms(20);
	delay_ms(20);

	LCD_PWM_LIGHT_HIGH();

#ifdef LCD43_DBG_LOG
	usart1_transmit_string("\r\nlcd_rst()\r\n");
#endif

	lcd_wr_reg(0x00E2);	//PLL multiplier, set PLL clock to 120M
	lcd_wr_data(0x0023);	    //N=0x36 for 6.5M, 0x23 for 10M crystal
	lcd_wr_data(0x0002);
	lcd_wr_data(0x0004);
	
	lcd_wr_reg(0x00E0);  // PLL enable
	lcd_wr_data(0x0001);
	delay_ms(10);

	lcd_wr_reg(0x00E0);
	lcd_wr_data(0x0003);

	delay_ms(20);
	delay_ms(20);
	delay_ms(20);
	lcd_wr_reg(0x0001);  // software reset

	delay_ms(20);
	delay_ms(20);
	delay_ms(20);
	lcd_wr_reg(0x00E6);	//PLL setting for PCLK, depends on resolution
	lcd_wr_data(0x0001);
	lcd_wr_data(0x0033);
	lcd_wr_data(0x0032);

	lcd_wr_reg(0x00B0);	//LCD SPECIFICATION
	lcd_wr_data(0x0018);
	lcd_wr_data(0x0000);
	lcd_wr_data((HDP>>8)&0X00FF);  //Set HDP
	lcd_wr_data(HDP&0X00FF);
    lcd_wr_data((VDP>>8)&0X00FF);  //Set VDP
	lcd_wr_data(VDP&0X00FF);
    lcd_wr_data(0x0000);

	lcd_wr_reg(0x00B4);	//HSYNC
	lcd_wr_data((HT>>8)&0X00FF);  //Set HT
	lcd_wr_data(HT&0X00FF);
	lcd_wr_data((HPS>>8)&0X00FF);  //Set HPS
	lcd_wr_data(HPS&0X00FF);
	lcd_wr_data(HPW);			   //Set HPW
	lcd_wr_data((LPS>>8)&0X00FF);  //Set HPS
	lcd_wr_data(LPS&0X00FF);
	lcd_wr_data(0x0000);

	lcd_wr_reg(0x00B6);	//VSYNC
	lcd_wr_data((VT>>8)&0X00FF);   //Set VT
	lcd_wr_data(VT&0X00FF);
	lcd_wr_data((VPS>>8)&0X00FF);  //Set VPS
	lcd_wr_data(VPS&0X00FF);
	lcd_wr_data(VPW);			   //Set VPW
	lcd_wr_data((FPS>>8)&0X00FF);  //Set FPS
	lcd_wr_data(FPS&0X00FF);

//	lcd_wr_reg(0x00BA);
//	lcd_wr_data(0x000F);    //GPIO[3:0] out 1

//	lcd_wr_reg(0x00B8);
//	lcd_wr_data(0x0007);    //GPIO3=input, GPIO[2:0]=output
//	lcd_wr_data(0x0001);    //GPIO0 normal

//	lcd_wr_reg(0x0036); //rotation
	//lcd_wr_data(0x0020);
//	lcd_wr_data(0x0000);
//	lcd_wr_data(0x0060);
//	lcd_wr_data(0x0080);
	//============================================

	//============================================
	// 16bit per pixel
	lcd_wr_reg(0x003A); //rotation
	lcd_wr_data(0x0050);

//		lcd_wr_reg(0x0026); //rotation
//	lcd_wr_data(0x0003);
//		lcd_wr_reg(0x0030); //rotation
//	lcd_wr_data(0x0000);

	//=============================================

	//=============================================
	// rgb565 format
	lcd_wr_reg(0x00F0); //pixel data interface
	lcd_wr_data(0x0003);

	lcd_wr_reg(0x0021);
	
    lcd_wr_reg(0x00BC);
	lcd_wr_data(0x0080);
    lcd_wr_data(0x0080);
    lcd_wr_data(0x0080);
    lcd_wr_data(0x0001);


	delay_ms(5);

	lcd43_clear_screen(GREEN);

	

	lcd_wr_reg(0x0029); //display on

//	lcd_wr_reg(0x00BE); //set PWM for B/L
//	lcd_wr_data(0x0006);
//	lcd_wr_data(0x0008);

	
//	lcd_wr_data(0x0001);
//	lcd_wr_data(0x00f0);
//	lcd_wr_data(0x0000);
//	lcd_wr_data(0x0000);

//	lcd_wr_reg(0x00d0);//
//	lcd_wr_data(0x000d);
				
//	delay_time(65535);


#ifdef LCD43_DBG_LOG
	usart1_transmit_string("\r\nlcd43_clear_screen()\r\n");
#endif


}

void lcd43_clear_area(u16 x,u16 y,u32 len,u32 wid)
{                    
    u32 w;

	lcd_wr_reg(0x002A);	
	lcd_wr_data(x>>8);	    
	lcd_wr_data(x);
	lcd_wr_data((len-1)>>8);	    
	lcd_wr_data((len-1));
    lcd_wr_reg(0x002b);	
	lcd_wr_data(y>>8);	    
	lcd_wr_data(y);
	lcd_wr_data((wid-1)>>8);	    
	lcd_wr_data((wid-1));
	lcd_wr_reg(0x002c);
	w=(len-x)*(wid-y);
	while(w--) lcd_wr_data(0xffff);
}   


void lcd43_draw_point(u16 x, u16 y, u16 color)
{	
	// set column address							       
	lcd_wr_reg(0x002A);				    
	lcd_wr_data(x>>8);
	lcd_wr_data(x);	    			
	lcd_wr_data(x>>8);
	lcd_wr_data(x);	    		
		    
	// set page address
    lcd_wr_reg(0x002b);	
	lcd_wr_data(y>>8);
	lcd_wr_data(y);	    			    
	lcd_wr_data(y>>8);
	lcd_wr_data(y);	   
	 
	// write memory start			
	lcd_wr_reg(0x2c);
	lcd_wr_data(color); 	   	
} 

void lcd43_set_cursor(u16 x, u16 y)
{	
	// set column address							       
	lcd_wr_reg(0x002A);				    
	lcd_wr_data(x>>8);
	lcd_wr_data(x);	    			
	lcd_wr_data(x>>8);
	lcd_wr_data(x);	    		
		    
	// set page address
    lcd_wr_reg(0x002b);	
	lcd_wr_data(y>>8);
	lcd_wr_data(y);	    			    
	lcd_wr_data(y>>8);
	lcd_wr_data(y);
} 

void lcd43_write_ram_prepare(void) 
{
	// write memory start			
	lcd_wr_reg(0x2c);
}

void lcd43_draw_point_big(u16 x,u16 y, u16 color)
{
	lcd43_draw_point(x,y,color);
	lcd43_draw_point(x+1,y,color);
	lcd43_draw_point(x,y+1,color);
	lcd43_draw_point(x+1,y+1,color);
}



void lcd43_show_char(u16 x, u16 y, u8 num, u16 color)
{       
	#define MAX_CHAR43_POSX 464
	#define MAX_CHAR43_POSY 262

    u8 temp;
    u8 pos,t;      

    if(x>MAX_CHAR43_POSX||y>MAX_CHAR43_POSY)
		return;
	
	lcd_wr_reg(0x002A);				    
	lcd_wr_data(x>>8);
	lcd_wr_data(x&0x00ff);
	lcd_wr_data((x+5)>>8);
	lcd_wr_data((x+5)&0x00ff);
    lcd_wr_reg(0x002b);	
	lcd_wr_data(y>>8);
	lcd_wr_data(y&0x00ff);
	lcd_wr_data((y+11)>>8);
	lcd_wr_data((y+11)&0x00ff);
	lcd_wr_reg(0x2c);	 
	num=num-' ';
	for(pos=0;pos<12;pos++)	
	{
	    temp=asc2_1206[num][pos];
	    for(t=0;t<6;t++)
	    {                 
	        if(temp&0x01)
				lcd_wr_data(color);
	        else 
				lcd_wr_data(0xffff);
	        temp>>=1; 
	    }
	}
}  	 

void lcd43_show_char_mode(unsigned int x,unsigned int y,unsigned int num,unsigned int size,unsigned int mode)
{       
//#define MAX_CHAR_POSX 472
//#define MAX_CHAR_POSY 264 
    unsigned int temp;
    unsigned int pos,t;      

    if(x>MAX_CHAR43_POSX||y>MAX_CHAR43_POSY)
		return;
   	lcd_wr_reg(0x002A);	
	lcd_wr_data(x>>8);	    
	lcd_wr_data(x&0x00ff);
	lcd_wr_data(x+(size/2-1)>>8);	    
	lcd_wr_data(x+(size/2-1)&0x00ff);
    lcd_wr_reg(0x002b);	
	lcd_wr_data(y>>8);	    
	lcd_wr_data(y&0x00ff);
	lcd_wr_data((y+size-1)>>8);	    
	lcd_wr_data((y+size-1)&0x00ff);
	lcd_wr_reg(0x002c);

	num=num-' ';
	if(!mode)
	{
		for(pos=0;pos<size;pos++)
		{
			if(size==12)temp=asc2_1206[num][pos];
			else temp=asc2_1608[num][pos];
			for(t=0;t<size/2;t++)
		    {                 
		        if(temp&0x01)
				 // lcd_wr_data(POINT_COLOR);
		          lcd43_draw_point(x+t,y+pos,POINT_COLOR);
				else 
				  //lcd_wr_data(BACK_COLOR); 
				  lcd43_draw_point(x+t,y+pos,BACK_COLOR);    
		        temp>>=1; 
		    }
		}	
	}else
	{
		for(pos=0;pos<size;pos++)
		{
			if(size==12)temp=asc2_1206[num][pos];
			else temp=asc2_1608[num][pos];
			for(t=0;t<size/2;t++)
		    {                 
		        if(temp&0x01)lcd43_draw_point(x+t,y+pos,POINT_COLOR);
		        temp>>=1; 
		    }
		}
	}
	

} 


void lcd43_show_string_mode(unsigned int x,unsigned int y,const unsigned char *p,unsigned int size,unsigned int mode)
{
    while(*p!='\0')
    {   
		    
        if(x>MAX_CHAR43_POSX)
		{
			x=0;
			y+=size;
		}
        
		if(y>MAX_CHAR43_POSY)
		{
			y=x=0;
			lcd43_clear_screen(WHITE);
		}
        
		lcd43_show_char_mode(x,y,*p,size,mode);
        x = x + size/2;
        p++;
    }  
}

void lcd43_show_num(u16 x,u16 y,u32 num, u16 color)
{      
	u32 res;   	   
	u8 t=0,t1=0;   
	res=num;
	if(!num)lcd43_show_char(x,y,'0',color);
	while(res)  						
	{
		res/=10;
		t++;
	}
	t1=t;
	while(t)							
	{
		res=mn(10,t-1); 	 
	    lcd43_show_char(x+(t1-t)*6,y,(num/res)%10+'0', color);
		t--;
	}
	if(t1==3) lcd43_show_char(x+18,y,' ', color);
} 


void lcd43_show_string(u16 x, u16 y, const u8 *p, u16 color)
{         
    while(*p!='\0')
    {       
        if(x>MAX_CHAR43_POSX){x=0;y+=12;}
        if(y>MAX_CHAR43_POSY){y=x=0;lcd43_clear_area(0,0,480,272);}
        lcd43_show_char(x,y,*p,color);
        x+=6;
        p++;
    }  
}

void lcd43_fill_color(u16 x,u16 y, u16 x1, u16 y1, u16 color)
{	
	u16 a,b;
	
	for(a=0; a<(y1-y); a++)
	{
		for(b=0; b<(x1-x); b++)
		{
			lcd43_draw_point(x+b,y+a, color);
		}
	}	  	
}

void lcd43_draw_line(u16 x1, u16 y1, u16 x2, u16 y2, u16 color)
{
	u16 t; 
	int xerr=0, yerr=0, delta_x, delta_y, distance; 
	int incx, incy, uRow, uCol; 

	delta_x=x2-x1; // computation increase of coordinates 
	delta_y=y2-y1; 
	uRow=x1; 
	uCol=y1; 
	if(delta_x>0)incx=1; // establishes on foot the direction 
	else if(delta_x==0)incx=0; // perpendicular line 
	else {incx=-1; delta_x=-delta_x;} 
	if(delta_y>0)incy=1; 
	else if(delta_y==0)incy=0; // level line 
	else {incy=-1; delta_y=-delta_y;} 
	if (delta_x>delta_y) distance=delta_x; // selects the basic increase coordinate axis 
	else distance=delta_y; 
	for (t=0; t<=distance+1; t++) // linedraw output 
	{  
		lcd43_draw_point(uRow, uCol, color); // picture spot 
		xerr+=delta_x; 
		yerr+=delta_y; 
		if(xerr>distance) 
		{ 
			xerr-=distance; 
			uRow+=incx; 
		} 
		if(yerr>distance) 
		{ 
			yerr-=distance; 
			uCol+=incy; 
		} 
	}  
} 

// is assigning the position to draw one to assign the size the circle
// (x, y): Central point
//r: Radius
void lcd43_draw_circle (u8 x0, u16 y0, u8 r, u16 color)
{
	int a, b;
	int di;
	a=0; b=r;	  
	di=3-(r<<1);             // judgment spot position symbol
	while(a<=b)
	{
		lcd43_draw_point(x0-b, y0-a, color);             //3           
		lcd43_draw_point(x0+b, y0-a, color);             //0           
		lcd43_draw_point(x0-a, y0+b, color);             //1       
		lcd43_draw_point(x0-b, y0-a, color);             //7           
		lcd43_draw_point(x0-a, y0-b, color);             //2             
		lcd43_draw_point(x0+b, y0+a, color);             //4               
		lcd43_draw_point(x0+a, y0-b, color);             //5
		lcd43_draw_point(x0+a, y0+b, color);             //6 
		lcd43_draw_point(x0-b, y0+a, color);             
		a++;
		// uses the Bresenham algorithm to make a circle     
		if(di<0)di +=4*a+6;	  
		else
		{
			di+=10+4*(a-b);   
			b--;
		} 
		lcd43_draw_point(x0+a, y0+b, color);
	}
} 

void lcd43_draw_demo_icon(unsigned int xsta,unsigned int ysta,unsigned int xend,unsigned int yend,unsigned char picturenumber)
{                    
    unsigned long n,disp_pix_num;	
    unsigned int temp;
 


	disp_pix_num=(unsigned long)(yend-ysta)*(xend-xsta)*2;



	lcd_wr_reg(0x002A);	
	lcd_wr_data(xsta>>8);	    
	lcd_wr_data(xsta&0x00ff);
	lcd_wr_data(xend-1>>8);	    
	lcd_wr_data(xend-1&0x00ff);
    lcd_wr_reg(0x002b);	
	lcd_wr_data(ysta>>8);	    
	lcd_wr_data(ysta&0x00ff);
	lcd_wr_data(yend-1>>8);	    
	lcd_wr_data(yend-1&0x00ff);
	lcd_wr_reg(0x002c);

	n=0;

	while(n<disp_pix_num)

	 { 
	  temp=(uint16_t)( icon_demo[n+picturenumber*7080]<<8)+icon_demo[n+1+picturenumber*7080];
	  lcd_wr_data(temp);
	  n=n+2;
	 }	 


}
void lcd43_fill_rectangle(unsigned int xsta,unsigned int ysta,unsigned int xend,unsigned int yend,unsigned int color)
{                    
    unsigned long n;

	n=(unsigned long)(yend-ysta+1)*(xend-xsta+1);

 

	lcd_wr_reg(0x002A);	
	lcd_wr_data(xsta>>8);	    
	lcd_wr_data(xsta&0x00ff);
	lcd_wr_data(xend>>8);	    
	lcd_wr_data(xend&0x00ff);
    lcd_wr_reg(0x002b);	
	lcd_wr_data(ysta>>8);	    
	lcd_wr_data(ysta&0x00ff);
	lcd_wr_data(yend>>8);	    
	lcd_wr_data(yend&0x00ff);

	lcd_wr_reg(0x002c);
  

	while(n--)lcd_wr_data(color);  

} 

/*
// void lcd43_draw_line(u16 x1, u16 y1, u16 x2, u16 y2, u16 color)
void LCD_DrawBox(uint16_t Xpos, uint16_t Ypos, uint16_t Height, uint16_t Width, u16 color)
{
	int32_t CurY;

	for (CurY = Ypos; CurY < (Ypos + Height); CurY++) {
		lcd43_draw_line(Xpos, CurY, Width, LCD_LINE_HORIZONTAL);
	}
}
*/


void lcd43_show_4num(unsigned int x,unsigned int y,unsigned int num,unsigned int size,unsigned int mode)
{   
	lcd43_show_char_mode(x,y,(num/1000)+'0',size,mode);
	lcd43_show_char_mode(x+size/2,y,(num/100)%10+'0',size,mode);      							   
    lcd43_show_char_mode(x+size,y,(num/10)%10+'0',size,mode); 
    lcd43_show_char_mode(x+size+size/2,y,num%10+'0',size,mode); 
}



void lcd43_demo(unsigned char M_Type,unsigned char Cnt,unsigned char En)
{
	unsigned int tx;
	unsigned int ty;
		
	ty=10; 
	//lcd_wr_reg(0x0028); //display off
	
	if(M_Type==0)
	{	  
		tx=100*(Cnt%5)+10;//10,90,170			
		
		if((Cnt==5)||(Cnt>5))
			ty=153;	 

		if(En)
		{	  
			POINT_COLOR=WHITE;
			BACK_COLOR=BLUE;
		}else
		{
			POINT_COLOR=BLACK;
			BACK_COLOR=WHITE;
		}

		if( Cnt == 0 )
	  		lcd43_show_string_mode(tx+13, ty+74, "Picture", 16, 0);	
		else if( Cnt == 1 )
	  		lcd43_show_string_mode(tx+13, ty+74, "Thils", 16, 0);	
		else if( Cnt == 2 )
	  		lcd43_show_string_mode(tx+13, ty+74, "Game", 16, 0);	
		else if( Cnt == 3 )
	  		lcd43_show_string_mode(tx+13, ty+74, "Earth", 16, 0);
		else if( Cnt == 4 )
	  		lcd43_show_string_mode(tx+13, ty+74, "Music", 16, 0);
		else if( Cnt == 5 )
	  		lcd43_show_string_mode(tx+13, ty+74, "Clock", 16, 0);
		else if( Cnt == 6 )
	  		lcd43_show_string_mode(tx+13, ty+74, "Apple", 16, 0);
		else if( Cnt == 7 )
	  		lcd43_show_string_mode(tx+13, ty+74, "Radio", 16, 0);
		else if( Cnt == 8 )
	  		lcd43_show_string_mode(tx+13, ty+74, "SMS", 16, 0);

	}
	
	//lcd_wr_reg(0x0029); //display on
	POINT_COLOR=BLACK;
	BACK_COLOR=WHITE;

}

void lcd43_draw_demo_icon_all(void)
{

	unsigned char t;
	unsigned int tx,ty;
	lcd43_clear_screen(WHITE);
	
	
	tx=10;ty=10;
	for(t=0;t<9;t++)
	{	   
					    
		tx = 100*(t%5)+10;//10,90,170,250,330	    
		if((t==5)||(t>5))
			ty=153;	  
		
		lcd43_draw_demo_icon(tx,ty,tx+59,ty+60,t);
		
	
		lcd43_demo(0,t,0);
		
	}
	
}



void lcd43_show_big_num(unsigned int x,unsigned int y,unsigned int num)
{
    unsigned int n,t;
	unsigned int temp;
	unsigned int t1,deadline;

	
	lcd_wr_reg(0x002A);	
	lcd_wr_data(x>>8);	    
	lcd_wr_data(x&0x00ff);
	lcd_wr_data((x+29)>>8);	    
	lcd_wr_data((x+29)&0x00ff);
    lcd_wr_reg(0x002b);	
	lcd_wr_data(y>>8);	    
	lcd_wr_data(y&0x00ff);
	lcd_wr_data((y+59)>>8);	    
	lcd_wr_data((y+59)&0x00ff);
	lcd_wr_reg(0x002c);


	if(num==':')t1=150;
	else if(num=='.')t1=165;
	else if(num=='C')t1=180;
	else t1=15*num;
	deadline=t1+15;
		   
	for(;t1<deadline;t1++)
	{	 
		for(n=0;n<16;n++)
		{
			temp=BIG_ASCII[t1][n];
			for(t=0;t<8;t++)
			{
				if(temp&0x80) 
				   lcd_wr_data(POINT_COLOR); 
				  // 	 lcd43_draw_point(x+t,y+n,POINT_COLOR);
				else 
				    lcd_wr_data(BACK_COLOR);
					// lcd43_draw_point(x+t,y+n,BACK_COLOR);
				
				temp<<=1;
				if(((n%4)==3)&&t==5)break;
			}
		}
	} 

}

void lcd43_set_orientation(LCD_OrientationMode_t m)
{
}

LCD_OrientationMode_t lcd43_get_orientation(void)
{
	return orientation_mode;
}

uint16_t lcd43_getwidth(void)
{
	switch (orientation_mode) {
	case LCD_LANDSCAPE_TOP_DOWN:
	case LCD_LANDSCAPE_BOTTOM_UP:
		return LCD43_HEIGHT_HW;
	case LCD_PORTRAIT_TOP_DOWN:
	case LCD_PORTRAIT_BOTTOM_UP:
	default:
		return LCD43_WIDTH_HW;
	}
}

/**
 * @brief Get height in current LCD orientation
 * @param none
 * @retval height
 */
uint16_t lcd43_getheight(void)
{
	switch (orientation_mode) {
	case LCD_LANDSCAPE_TOP_DOWN:
	case LCD_LANDSCAPE_BOTTOM_UP:
		return LCD43_WIDTH_HW;
	case LCD_PORTRAIT_TOP_DOWN:
	case LCD_PORTRAIT_BOTTOM_UP:
	default:
		return LCD43_HEIGHT_HW;
	}
}

