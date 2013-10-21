#ifndef __LCD70_H
#define __LCD70_H

#define LCD_DATA_ADD		*((u16 *)0x60020000)
#define LCD_COMM_ADD		*((u16 *)0x60000000)


//#define Bank1_LCD_D    ((uint32_t)0x60020000)    //disp Data ADDR
//#define Bank1_LCD_C    ((uint32_t)0x60000000)	 //disp Reg ADDR

#define Up2Down2Left2Right  0x01
#define Down2Up2Right2Left	0x02
#define Up2Down2Right2Left	0x00
#define Down2Up2Left2Right	0x03

#define OpenEL()		{s1963gpio = 0x08|s1963gpio; LCD_COMM_ADD = 0xBA; LCD_DATA_ADD = s1963gpio;}
#define CloseEL()		{s1963gpio = 0x07&s1963gpio; LCD_COMM_ADD = 0xBA; LCD_DATA_ADD = s1963gpio;}
#define EnableDITHB()		{s1963gpio = 0x0B&s1963gpio; LCD_COMM_ADD = 0xBA; LCD_DATA_ADD = s1963gpio;}
#define DisableDITHB()		{s1963gpio = 0x04|s1963gpio; LCD_COMM_ADD = 0xBA; LCD_DATA_ADD = s1963gpio;}
#define SetS1963GpioOut()	{		 	     LCD_COMM_ADD = 0xBA; LCD_DATA_ADD = s1963gpio;}

void bsp_lcd70_init(void);
void lcd70_clear_screen(unsigned int color);
void lcd70_draw_point(u16 x, u16 y, u16 color);
void lcd70_show_char(u16 x, u16 y, u8 num, u16 color);
void lcd70_show_char_mode(unsigned int x,unsigned int y,unsigned int num,unsigned int size,unsigned int mode);
void lcd70_show_string_mode(unsigned int x,unsigned int y,const unsigned char *p,unsigned int size,unsigned int mode);
void lcd70_fill_color(u16 x,u16 y,u32 len,u32 wid, u16 color );
void lcd70_draw_point_big(u16 x,u16 y, u16 color);
void lcd70_show_string(u16 x, u16 y, const u8 *p, u16 color);
void lcd70_show_num(u16 x,u16 y,u32 num, u16 color);
void lcd70_draw_line(u16 x1, u16 y1, u16 x2, u16 y2, u16 color);
void lcd70_draw_rectangle(u16 x1, u16 y1, u16 x2, u16 y2, u16 color);
void lcd70_set_area(u16 x1, u16 y1, u16 x2, u16 y2);
void lcd70_set_cursor(u16 x, u16 y);
void lcd70_write_ram_prepare(void);
void lcd70_draw_circle (u8 x0, u16 y0, u8 r, u16 color);
void lcd70_draw_demo_icon(unsigned int xsta,unsigned int ysta,unsigned int xend,unsigned int yend,unsigned char picturenumber);
void lcd70_draw_demo_icon_all(void);
void lcd70_demo(unsigned char M_Type,unsigned char Cnt,unsigned char En);
						  		 
#endif  
	 



