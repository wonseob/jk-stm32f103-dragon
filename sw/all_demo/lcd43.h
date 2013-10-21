#ifndef __LCD43_H
#define __LCD43_H


#define LCD43_WIDTH_HW         480
#define LCD43_HEIGHT_HW        272

// #define LCD_ORIENTATION        LCD_PORTRAIT
#define LCD43_ORIENTATION        LCD_LANDSCAPE

#if LCD43_ORIENTATION == LCD_PORTRAIT
#define LCD43_WIDTH              272
#define LCD43_HEIGHT             480
#define LCD43_CHARS_PER_LINE      10
#define LCD43_CHAR_LINES          13
#define LCD43_ENTRY_MODE_DEFAULT 0x1030 /* Entry Mode BGR=1,AM=0,ID1/0=1/1 */
#define LCD43_ORIENTATION_DEFAULT LCD_PORTRAIT_TOP_DOWN
#else
#define LCD43_WIDTH              480
#define LCD43_HEIGHT             272
#define LCD43_CHARS_PER_LINE      15
#define LCD43_CHAR_LINES          10
#define LCD43_ENTRY_MODE_DEFAULT 0x1018 /* Entry Mode BGR=1,AM=1,ID1/0=1/0 */
#define LCD43_ORIENTATION_DEFAULT LCD_LANDSCAPE_TOP_DOWN
#endif

void lcd43_clear_screen(unsigned int color);
void bsp_lcd43_init(void);
void lcd43_clear_area(u16 x,u16 y,u32 len,u32 wid);
void lcd43_draw_point(u16 x, u16 y, u16 color);
void lcd43_set_cursor(u16 x, u16 y);
void lcd43_write_ram_prepare(void) ;
void lcd43_draw_point_big(u16 x,u16 y,u16 color);
void lcd43_show_char(u16 x, u16 y, u8 num, u16 color);
void lcd43_show_char_mode(unsigned int x,unsigned int y,unsigned int num,unsigned int size,unsigned int mode);
void lcd43_show_string_mode(unsigned int x,unsigned int y,const unsigned char *p,unsigned int size,unsigned int mode);
void lcd43_show_num(u16 x,u16 y,u32 num, u16 color);
void lcd43_show_string(u16 x, u16 y, const u8 *p, u16 color);
void lcd43_fill_color(u16 x,u16 y, u16 x1, u16 y1, u16 color);
void lcd43_draw_line(u16 x1, u16 y1, u16 x2, u16 y2, u16 color);
void lcd43_draw_circle (u8 x0, u16 y0, u8 r, u16 color);

void lcd43_draw_demo_icon(unsigned int xsta,unsigned int ysta,unsigned int xend,unsigned int yend,unsigned char picturenumber);
void lcd43_fill_rectangle(unsigned int xsta,unsigned int ysta,unsigned int xend,unsigned int yend,unsigned int color);
void lcd43_show_4num(unsigned int x,unsigned int y,unsigned int num,unsigned int size,unsigned int mode);
void lcd43_demo(unsigned char M_Type,unsigned char Cnt,unsigned char En);
void lcd43_draw_demo_icon_all(void);
void lcd43_show_big_num(unsigned int x,unsigned int y,unsigned int num);

void lcd43_set_orientation(LCD_OrientationMode_t m);						  		 
LCD_OrientationMode_t lcd43_get_orientation(void);
uint16_t lcd43_getwidth(void);
uint16_t lcd43_getheight(void);
#endif  
	 



