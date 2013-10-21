
#include  <stm32f10x.h>
#include "hw_config.h"
#include "lcd28.h"
#include "lcd32.h"
#include "stdlib.h"
#include "usart.h"	
#include "lcd.h" 

#define LCD32_DBG_LOG

extern const unsigned char asc2_1206[95][12];
extern const unsigned char asc2_1608[95][16];
extern const unsigned char icon_demo[63720];
extern const unsigned char BIG_ASCII[195][16];

extern unsigned int  POINT_COLOR;
extern unsigned int  BACK_COLOR;

extern u16 lcd_id;	 

void bsp_lcd32_init(void)
{
	bsp_lcd_gpio_init();   	
	delay_ms(10);

	LCD_CS_HIGH();

	// LCD Data(0 ~ 7)
	GPIO_SetBits(GPIOD, GPIO_Pin_14| GPIO_Pin_15 |GPIO_Pin_0 | GPIO_Pin_1);  	 
	GPIO_SetBits(GPIOE, GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10);   
	GPIO_SetBits(GPIOD, GPIO_Pin_14| GPIO_Pin_15 |GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 );  	 
	GPIO_SetBits(GPIOE, GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 |  GPIO_Pin_15);   

	
	LCD_RESET_LOW();

	LCD_RD_HIGH();
	LCD_WR_HIGH();

	delay_ms(10);
	bsp_fsmc_lcd_init();
	delay_ms(10);

#ifdef LCD32_DBG_LOG
	usart1_transmit_string("\r\nfsmc init\r\n");
#endif

	lcd_rst();
	

	

	delay_ms(20);
	delay_ms(20);
	delay_ms(20);
	delay_ms(20);


	//LCD_PWM_LIGHT_HIGH();
	LCD_PWM_LIGHT_LOW();
	//LCD_PWM_LIGHT_HIGH();
	//LCD_PWM_LIGHT_LOW();

#ifdef LCD32_DBG_LOG
	usart1_transmit_string("\r\nlcd_rst()\r\n");
#endif

	delay_ms(5); 
	lcd_wr_reg_value(0x0000,0x0001);
	delay_ms(10); 
	
	// HX8347A device id(Himax ID code)	= 0x0047
	lcd_wr_reg(0x0067);
	lcd_id = lcd_rd_data();
	
	if(lcd_id != 0x0047)
	{		
		// ILI93xx Series device id
		delay_ms(5); 
		lcd_wr_reg(0x0000);
		lcd_id = lcd_rd_data();
	}
	
	usart1_transmit_string_format("LCD ID:0x%x \n", lcd_id);  
	
	
	if(lcd_id==0x9220 )	// ILI9220
	{
		lcd_wr_reg_value(0x0001,0x001B);
		lcd_wr_reg_value(0x0002,0x0700);
		lcd_wr_reg_value(0x0003,0x1030);
		lcd_wr_reg_value(0x0004,0x0000);     
		lcd_wr_reg_value(0x0005,0x0000);
		lcd_wr_reg_value(0x0007,0x0004);      
		lcd_wr_reg_value(0x0008,0x0202);
		lcd_wr_reg_value(0x0009,0x0000);
		lcd_wr_reg_value(0x000B,0x0004);  
		lcd_wr_reg_value(0x000C,0x0003);
		lcd_wr_reg_value(0x000B,0x0004);
		lcd_wr_reg_value(0x0021,0x0000);
		lcd_wr_reg_value(0x0040,0x0000);
		lcd_wr_reg_value(0x0041,0x00EF);
		lcd_wr_reg_value(0x0042,0xDB00);
		lcd_wr_reg_value(0x0043,0xDB00);
		lcd_wr_reg_value(0x0044,0xAF00);
		lcd_wr_reg_value(0x0045,0xDB00);
		delay_ms(10);
		
		//################# void Gamma_Set(void) ####################//
		lcd_wr_reg_value(0x0030,0x0000);  
		lcd_wr_reg_value(0x0031,0x0704);  
		lcd_wr_reg_value(0x0032,0x0004);  
		lcd_wr_reg_value(0x0033,0x0604);  
		lcd_wr_reg_value(0x0034,0x0307);  
		lcd_wr_reg_value(0x0035,0x0103);  
		lcd_wr_reg_value(0x0036,0x0707);  
		lcd_wr_reg_value(0x0037,0x0603);  
		lcd_wr_reg_value(0x0038,0x0000);  
		lcd_wr_reg_value(0x0039,0x0000);
		//############# void Power_Set(void) ################//
		lcd_wr_reg_value(0x0010,0x0000);
		lcd_wr_reg_value(0x0011,0x0000);
		lcd_wr_reg_value(0x0013,0x1413);
		lcd_wr_reg_value(0x0012,0x0008);
		delay_ms(10);
		lcd_wr_reg_value(0x0012,0x0018);
		delay_ms(10);
		lcd_wr_reg_value(0x0010,0x0140);
		delay_ms(20);
		lcd_wr_reg_value(0x0013,0x3414);
		delay_ms(10);
		
		lcd_wr_reg_value(0x0010,0x2140);
		
		//################## void Display_ON(void) ####################//
		lcd_wr_reg_value(0x0010,0x4040);
		lcd_wr_reg_value(0x0007,0x0005);
		delay_ms(4);
		lcd_wr_reg_value(0x0007,0x0025);
		lcd_wr_reg_value(0x0007,0x0027);
		delay_ms(4);
		lcd_wr_reg_value(0x0007,0x0037);
		delay_ms(1);

	}
	else if(lcd_id==0x9325||lcd_id==0x9328)//ILI9325
	{
		lcd_wr_reg_value(0x00e7,0x0010);      
		lcd_wr_reg_value(0x0000,0x0001); // opening internal clock
		lcd_wr_reg_value(0x0001,0x0100);     
		lcd_wr_reg_value(0x0002,0x0700); // power source opening                    
		//lcd_wr_reg_value (0x0003,(1<<3)|(1<<4)); 	//65K RGB
		//DRIVE TABLE (register 03H)
		//BIT3 =AM BIT4:5=ID0:1
		//AM ID0 ID1 FUNCATION
		// 0 0 0 R->L D->U
		// 1 0 0 D->U R->L
		// 0 1 0 L->R D->U
		// 1 1 0 D->U L->R
		// 0 0 1 R->L U->D
		// 1 0 1 U->D R->L
		// 0 1 1 L->R U->D normal uses this.  	
		// 1 1 1 U->D L->R
        lcd_wr_reg_value(0x0003,(1<<12)|(3<<4)|(0<<3)); //65K    
        lcd_wr_reg_value(0x0004,0x0000);                                   
        lcd_wr_reg_value(0x0008,0x0207);	           
        lcd_wr_reg_value(0x0009,0x0000);         
        lcd_wr_reg_value(0x000a,0x0000); //display setting         
        lcd_wr_reg_value(0x000c,0x0001); //display setting          
        lcd_wr_reg_value(0x000d,0x0000); //0f3c          
        lcd_wr_reg_value(0x000f,0x0000);
		// power source disposition
        lcd_wr_reg_value(0x0010,0x0000);   
        lcd_wr_reg_value(0x0011,0x0007);
        lcd_wr_reg_value(0x0012,0x0000);                                                                 
        lcd_wr_reg_value(0x0013,0x0000);                 
        delay_us(5); 
        lcd_wr_reg_value(0x0010,0x1590);   
        lcd_wr_reg_value(0x0011,0x0227);
        delay_us(5); 
        lcd_wr_reg_value(0x0012,0x009c);                  
        delay_us(5); 
        lcd_wr_reg_value(0x0013,0x1900);   
        lcd_wr_reg_value(0x0029,0x0023);
        lcd_wr_reg_value(0x002b,0x000e);
        delay_us(5); 
        lcd_wr_reg_value(0x0020,0x0000);                                                            
        lcd_wr_reg_value(0x0021,0x013f);           
		delay_us(5); 
		// gamma adjustment
        lcd_wr_reg_value(0x0030,0x0007); 
        lcd_wr_reg_value(0x0031,0x0707);   
        lcd_wr_reg_value(0x0032,0x0006);
        lcd_wr_reg_value(0x0035,0x0704);
        lcd_wr_reg_value(0x0036,0x1f04); 
        lcd_wr_reg_value(0x0037,0x0004);
        lcd_wr_reg_value(0x0038,0x0000);        
        lcd_wr_reg_value(0x0039,0x0706);     
        lcd_wr_reg_value(0x003c,0x0701);
        lcd_wr_reg_value(0x003d,0x000f);
        delay_us(5); 
        lcd_wr_reg_value(0x0050,0x0000); // level GRAM reference 
        lcd_wr_reg_value(0x0051,0x00ef); // level GRAM terminated the position                    
        lcd_wr_reg_value(0x0052,0x0000); // vertical GRAM reference                    
        lcd_wr_reg_value(0x0053,0x013f); // vertical GRAM terminated the position  
        
        lcd_wr_reg_value(0x0060,0xa700);        
        lcd_wr_reg_value(0x0061,0x0001); 
        lcd_wr_reg_value(0x006a,0x0000);
        lcd_wr_reg_value(0x0080,0x0000);
        lcd_wr_reg_value(0x0081,0x0000);
        lcd_wr_reg_value(0x0082,0x0000);
        lcd_wr_reg_value(0x0083,0x0000);
        lcd_wr_reg_value(0x0084,0x0000);
        lcd_wr_reg_value(0x0085,0x0000);
      
        lcd_wr_reg_value(0x0090,0x0010);     
        lcd_wr_reg_value(0x0092,0x0000);  
        lcd_wr_reg_value(0x0093,0x0003);
        lcd_wr_reg_value(0x0095,0x0110);
        lcd_wr_reg_value(0x0097,0x0000);        
        lcd_wr_reg_value(0x0098,0x0000);  
        // opens the demonstration establishment    
        lcd_wr_reg_value(0x0007,0x0133);   
        lcd_wr_reg_value(0x0020,0x0000);                                                            
        lcd_wr_reg_value(0x0021,0x013f);
	} 
	else if(lcd_id==0x9320 || lcd_id==0x9300)
	{
		lcd_wr_reg_value(0x00,0x0001);

		delay_us(50);
		
		lcd_wr_reg_value(0x01,0x0100);	//Driver Output Contral.
		lcd_wr_reg_value(0x02,0x0700);	//LCD Driver Waveform Contral.
		delay_us(20); 
		
		lcd_wr_reg_value(0x03,0x1030);	//Entry Mode Set.	
		
		delay_us(20); 
	
		lcd_wr_reg_value(0x04,0x0000);	//Scalling Contral.
		lcd_wr_reg_value(0x08,0x0202);	//Display Contral 2. (0x0207)
		lcd_wr_reg_value(0x09,0x0000);	//Display Contral 3. (0x0000)
		lcd_wr_reg_value(0x0a,0x0000);	//Frame Cycle Contal. (0x0000)
		lcd_wr_reg_value(0x0c,(1<<0));	//Extern Display Interface Contral 1. (0x0000)
		lcd_wr_reg_value(0x0d,0x0000);	//Frame Maker Position.
		lcd_wr_reg_value(0x0f,0x0000);	//Extern Display Interface Contral 2.	    

		delay_ms(20); 
		lcd_wr_reg_value(0x07,0x0101);	//Display Contral.
		delay_ms(20); 								  

		lcd_wr_reg_value(0x10,(1<<12)|(0<<8)|(1<<7)|(1<<6)|(0<<4));	//Power Control 1. (0x16b0)
		delay_us(20); 
		lcd_wr_reg_value(0x11,0x0007);								//Power Control 2. (0x0001)
		delay_us(40); 
		lcd_wr_reg_value(0x12,(1<<8)|(1<<4)|(0<<0));				//Power Control 3. (0x0138)
		delay_us(40); 
		lcd_wr_reg_value(0x13,0x0b00);								//Power Control 4.
		delay_us(20); 
		lcd_wr_reg_value(0x29,0x0000);								//Power Control 7.
		delay_us(20); 
	
		lcd_wr_reg_value(0x2b,(1<<14)|(1<<4));	    
		lcd_wr_reg_value(0x50,0);	//Set X Star
		// level GRAM terminated position Set X End.
		lcd_wr_reg_value(0x51,239);	//Set Y Star
		lcd_wr_reg_value(0x52,0);	//Set Y End.t.
		lcd_wr_reg_value(0x53,319);	//
		delay_us(20); 
	
		lcd_wr_reg_value(0x60,0x2700);	//Driver Output Control.
		lcd_wr_reg_value(0x61,0x0001);	//Driver Output Control.
		lcd_wr_reg_value(0x6a,0x0000);	//Vertical Srcoll Control.
		delay_us(20); 
	
		lcd_wr_reg_value(0x80,0x0000);	//Display Position? Partial Display 1.
		lcd_wr_reg_value(0x81,0x0000);	//RAM Address Start? Partial Display 1.
		lcd_wr_reg_value(0x82,0x0000);	//RAM Address End-Partial Display 1.
		lcd_wr_reg_value(0x83,0x0000);	//Displsy Position? Partial Display 2.
		lcd_wr_reg_value(0x84,0x0000);	//RAM Address Start? Partial Display 2.
		lcd_wr_reg_value(0x85,0x0000);	//RAM Address End? Partial Display 2.
		delay_us(20); 
	
		lcd_wr_reg_value(0x90,(0<<7)|(16<<0));	//Frame Cycle Contral. (0x0013)
		lcd_wr_reg_value(0x92,0x0000);	//Panel Interface Contral 2. (0x0000)
		lcd_wr_reg_value(0x93,0x0001);	//Panel Interface Contral 3.
		lcd_wr_reg_value(0x95,0x0110);	//Frame Cycle Contral. (0x0110)
		lcd_wr_reg_value(0x97,(0<<8));	//
		lcd_wr_reg_value(0x98,0x0000);	//Frame Cycle Contral.	   

		lcd_wr_reg_value(0x07,0x0173);	// (0x0173)
		delay_us(40);
	} 
	else if(lcd_id==0x5408)
	{
#if 1	
		lcd_wr_reg_value(0x01,0x0100);								  
		lcd_wr_reg_value(0x02,0x0700); //LCD Driving Waveform Contral 

		// 세로모드
		//lcd_wr_reg_value(0x03,0x1030); //Entry Mode establishment 	   
		// 가로모드
		lcd_wr_reg_value(0x03,0x1018); 
		
		// indicator top-down increases the pattern automatically from left to right
		//Normal Mode (Window Mode disable)
		//RGB form
		//16 bit data 2 transmission 8 main line establishments
		lcd_wr_reg_value(0x04,0x0000); //Scalling Control register     
		lcd_wr_reg_value(0x08,0x0207); //Display Control 2 
		lcd_wr_reg_value(0x09,0x0000); //Display Control 3	 
		lcd_wr_reg_value(0x0A,0x0000); //Frame Cycle Control	 
		lcd_wr_reg_value(0x0C,0x0000); //External Display Interface Control 1 
		lcd_wr_reg_value(0x0D,0x0000); //Frame Maker Position		 
		lcd_wr_reg_value(0x0F,0x0000); //External Display Interface Control 2 
 		delay_us(20);
		//TFT liquid crystal color image display packing 14
		lcd_wr_reg_value(0x10,0x16B0); //0x14B0 //Power Control 1
		lcd_wr_reg_value(0x11,0x0001); //0x0007 //Power Control 2
		lcd_wr_reg_value(0x17,0x0001); //0x0000 //Power Control 3
		lcd_wr_reg_value(0x12,0x0138); //0x013B //Power Control 4
		lcd_wr_reg_value(0x13,0x0800); //0x0800 //Power Control 5

		// 새로 추가 함
	     //lcd_wr_reg_value(0x0020,0x00ef);
	     //lcd_wr_reg_value(0x0021,0x013f);
		// 새로 추가 함
		
		lcd_wr_reg_value(0x29,0x0009); //NVM read data 2
		lcd_wr_reg_value(0x2a,0x0009); //NVM read data 3
		lcd_wr_reg_value(0xa4,0x0000);	 
		lcd_wr_reg_value(0x50,0x0000); // establishes the operation window X axis to start the row
		lcd_wr_reg_value(0x51,0x00EF); // establishes the operation window X axis conclusion row
		lcd_wr_reg_value(0x52,0x0000); // establishes the operation window the Y axis to start the line
		lcd_wr_reg_value(0x53,0x013F); // establishes the operation window the Y axis conclusion line

		lcd_wr_reg_value(0x60,0x2700); //Driver Output Control  --> original
		//lcd_wr_reg_value(0x0060, 0xA700); // Gate Scan Line
		
		// establishes screen's points as well as the scanning outset line
		lcd_wr_reg_value(0x61,0x0001); //Driver Output Control
		lcd_wr_reg_value(0x6A,0x0000); //Vertical Scroll Control
		lcd_wr_reg_value(0x80,0x0000); //Display Position - Partial Display 1
		lcd_wr_reg_value(0x81,0x0000); //RAM Address Start - Partial Display 1
		lcd_wr_reg_value(0x82,0x0000); //RAM address End - Partial Display 1
		lcd_wr_reg_value(0x83,0x0000); //Display Position - Partial Display 2
		lcd_wr_reg_value(0x84,0x0000); //RAM Address Start - Partial Display 2
		lcd_wr_reg_value(0x85,0x0000); //RAM address End - Partail Display2
		lcd_wr_reg_value(0x90,0x0013); //Frame Cycle Control
		lcd_wr_reg_value(0x92,0x0000);  //Panel Interface Control 2
		lcd_wr_reg_value(0x93,0x0003); //Panel Interface control 3
		lcd_wr_reg_value(0x95,0x0110);  //Frame Cycle Control
		lcd_wr_reg_value(0x07,0x0173);		 
		delay_us(5);
#endif

#if 0
	lcd_wr_reg_value(0x00E3,0x3008);        
	lcd_wr_reg_value(0x00E7,0x0012);
	lcd_wr_reg_value(0x00Ef,0x1231); 	// Set the internal timing;    
	//initializing funciton 1 
         lcd_wr_reg_value(0x00,0x0001);
         lcd_wr_reg_value(0x01, 0x0000); // set SS and SM bit
         lcd_wr_reg_value(0x02, 0x0700); // set 1 line inversion
        // lcd_wr_reg_value(0x03, 0x10B0); // set GRAM write direction and BGR=1.
#if   ID_AM==000       
	     lcd_wr_reg_value(0x0003,0x1000);
#elif ID_AM==001        
	     lcd_wr_reg_value(0x0003,0x1008);      
#elif ID_AM==010  
	     lcd_wr_reg_value(0x0003,0x1010);        
#elif ID_AM==011
	     lcd_wr_reg_value(0x0003,0x1018);
#elif ID_AM==100  
	     lcd_wr_reg_value(0x0003,0x1020);      
#elif ID_AM==101  
	     lcd_wr_reg_value(0x0003,0x1028);      
#elif ID_AM==110  
	     lcd_wr_reg_value(0x0003,0x1030);      
#elif ID_AM==111  
	     lcd_wr_reg_value(0x0003,0x1038);
#endif    
         lcd_wr_reg_value(0x04, 0x0000); // Resize register
         lcd_wr_reg_value(0x08, 0x0404); // set the back porch and front porch
         lcd_wr_reg_value(0x09, 0x0000); // set non-display area refresh cycle ISC[3:0]
         lcd_wr_reg_value(0x0A, 0x0000); // FMARK function
        // lcd_wr_reg_value(0x0C, 0x0000); // RGB interface setting
        // lcd_wr_reg_value(0x0D, 0x0000); // Frame marker Position
        // lcd_wr_reg_value(0x0F, 0x0000); // RGB interface polarity
//Power On sequence //
         lcd_wr_reg_value(0x10, 0x0080); // SAP, BT[3:0], AP, DSTB, SLP, STB
         lcd_wr_reg_value(0x11, 0x0007); // DC1[2:0], DC0[2:0], VC[2:0]
         lcd_wr_reg_value(0x12, 0x0000); // VREG1OUT voltage
         lcd_wr_reg_value(0x13, 0x0000); // VDV[4:0] for VCOM amplitude
	 lcd_wr_reg_value(0x07, 0x0001); // 
		 
         delay_us(10); // Dis-charge capacitor power voltage
         
         lcd_wr_reg_value(0x10, 0x1590); // SAP, BT[3:0], AP, DSTB, SLP, STB
         lcd_wr_reg_value(0x11, 0x0227); // DC1[2:0], DC0[2:0], VC[2:0]
         delay_us(5); // delay_us 50ms
        
         lcd_wr_reg_value(0x12, 0x001c); // Internal reference voltage= Vci;
         delay_us(5); // delay_us 50ms
         lcd_wr_reg_value(0x13, 0x1500); // Set VDV[4:0] for VCOM amplitude
         lcd_wr_reg_value(0x29, 0x0010); // Set VCM[5:0] for VCOMH
         lcd_wr_reg_value(0x2B, 0x000f); // Set Frame Rate
         delay_us(5); // delay_us 50ms
         
#if   ID_AM==000         
	     lcd_wr_reg_value(0x0020,0x00ef);
	     lcd_wr_reg_value(0x0021,0x013f);      
#elif ID_AM==001
	     lcd_wr_reg_value(0x0020,0x00ef);
	     lcd_wr_reg_value(0x0021,0x013f);      
#elif ID_AM==010
	     lcd_wr_reg_value(0x0020,0x0000);
	     lcd_wr_reg_value(0x0021,0x013f);      
#elif ID_AM==011
	     lcd_wr_reg_value(0x0020,0x0000);
	     lcd_wr_reg_value(0x0021,0x013f);       
#elif ID_AM==100
	     lcd_wr_reg_value(0x0020,0x00ef);
	     lcd_wr_reg_value(0x0021,0x0000);      
#elif ID_AM==101  
	     lcd_wr_reg_value(0x0020,0x00ef);
	     lcd_wr_reg_value(0x0021,0x0000);      
#elif ID_AM==110
	     lcd_wr_reg_value(0x0020,0x0000);
	     lcd_wr_reg_value(0x0021,0x0000);      
#elif ID_AM==111
	     lcd_wr_reg_value(0x0020,0x0000);
	     lcd_wr_reg_value(0x0021,0x0000);         
#endif  
// ----------- Adjust the Gamma Curve ----------//
		lcd_wr_reg_value(0x0030,0x0101);
		lcd_wr_reg_value(0x0031,0x0707);
		lcd_wr_reg_value(0x0032,0x0505);
		lcd_wr_reg_value(0x0035,0x0107);
		lcd_wr_reg_value(0x0036,0x0108);
		lcd_wr_reg_value(0x0037,0x0102);
		lcd_wr_reg_value(0x0038,0x0202);
		lcd_wr_reg_value(0x0039,0x0106);
		lcd_wr_reg_value(0x003C,0x0202);
		lcd_wr_reg_value(0x003D,0x0806);
//------------------ Set GRAM area ---------------//
         lcd_wr_reg_value(0x0050, 0x0000); // Horizontal GRAM Start Address
         lcd_wr_reg_value(0x0051, 0x00EF); // Horizontal GRAM End Address
         lcd_wr_reg_value(0x0052, 0x0000); // Vertical GRAM Start Address
         lcd_wr_reg_value(0x0053, 0x013F); // Vertical GRAM Start Address
         lcd_wr_reg_value(0x0060, 0xA700); // Gate Scan Line
         lcd_wr_reg_value(0x0061, 0x0001); // NDL,VLE, REV
         lcd_wr_reg_value(0x006A, 0x0000); // set scrolling line
//-------------- Partial Display Control ---------//
         lcd_wr_reg_value(0x80, 0x0000);
         lcd_wr_reg_value(0x81, 0x0000);
         lcd_wr_reg_value(0x82, 0x0000);
         lcd_wr_reg_value(0x83, 0x0000);
         lcd_wr_reg_value(0x84, 0x0000);
         lcd_wr_reg_value(0x85, 0x0000);
//-------------- Panel Control -------------------//
         lcd_wr_reg_value(0x90, 0x0010);
         lcd_wr_reg_value(0x92, 0x0000);
         lcd_wr_reg_value(0x93, 0x0003);
         lcd_wr_reg_value(0x95, 0x0110);
         lcd_wr_reg_value(0x97, 0x0000);
         lcd_wr_reg_value(0x98, 0x0000);
         lcd_wr_reg_value(0x07, 0x0173); // 262K color and display ON   0133

		 delay_us(5);
#endif		 
	}	
	else if(lcd_id==0x1505)
	{
		// second release on 3/5, luminance is acceptable, water wave appear during camera preview lcd_wr_reg_value(0x0007,0x0000);
        delay_us(5); 
        lcd_wr_reg_value(0x0012,0x011C); //0x011A   why need to set several times?
        lcd_wr_reg_value(0x00A4,0x0001); //NVM	 
        lcd_wr_reg_value(0x0008,0x000F);
        lcd_wr_reg_value(0x000A,0x0008);
        lcd_wr_reg_value(0x000D,0x0008);	    
  		// gamma adjustment
        lcd_wr_reg_value(0x0030,0x0707);
        lcd_wr_reg_value(0x0031,0x0007); //0x0707
        lcd_wr_reg_value(0x0032,0x0603); 
        lcd_wr_reg_value(0x0033,0x0700); 
        lcd_wr_reg_value(0x0034,0x0202); 
        lcd_wr_reg_value(0x0035,0x0002); //? 0x0606
        lcd_wr_reg_value(0x0036,0x1F0F);
        lcd_wr_reg_value(0x0037,0x0707); //0x0f0f 0x0105
        lcd_wr_reg_value(0x0038,0x0000); 
        lcd_wr_reg_value(0x0039,0x0000); 
        lcd_wr_reg_value(0x003A,0x0707); 
        lcd_wr_reg_value(0x003B,0x0000); //0x0303
        lcd_wr_reg_value(0x003C,0x0007); //0x0707
        lcd_wr_reg_value(0x003D,0x0000); //0x1313//0x1f08
        delay_us(5); 
        lcd_wr_reg_value(0x0007,0x0001);
        lcd_wr_reg_value(0x0017,0x0001); // opens the power source
        delay_us(5); 
  		// power source disposition
        lcd_wr_reg_value(0x0010,0x17A0); 
        lcd_wr_reg_value(0x0011,0x0217); //reference voltage VC[2:0] Vciout = 1.00*Vcivl
        lcd_wr_reg_value(0x0012,0x011E); //0x011c //Vreg1out = Vcilvl*1.80 is it the same as Vgama1out?
        lcd_wr_reg_value(0x0013,0x0F00); //VDV[4:0]-->VCOM Amplitude VcomL = VcomH - Vcom Ampl
        lcd_wr_reg_value(0x002A,0x0000);  
        lcd_wr_reg_value(0x0029,0x000A); //0x0001F Vcomh = VCM1[4:0]*Vreg1out gate source voltage??
        lcd_wr_reg_value(0x0012,0x013E); // 0x013C  power supply on
        //Coordinates Control//
        lcd_wr_reg_value(0x0050,0x0000); //0x0e00
        lcd_wr_reg_value(0x0051,0x00EF); 
        lcd_wr_reg_value(0x0052,0x0000); 
        lcd_wr_reg_value(0x0053,0x013F); 
    	//Pannel Image Control//
        lcd_wr_reg_value(0x0060,0x2700); 
        lcd_wr_reg_value(0x0061,0x0001); 
        lcd_wr_reg_value(0x006A,0x0000); 
        lcd_wr_reg_value(0x0080,0x0000); 
    	//Partial Image Control//
        lcd_wr_reg_value(0x0081,0x0000); 
        lcd_wr_reg_value(0x0082,0x0000); 
        lcd_wr_reg_value(0x0083,0x0000); 
        lcd_wr_reg_value(0x0084,0x0000); 
        lcd_wr_reg_value(0x0085,0x0000); 
  		//Panel Interface Control//
        lcd_wr_reg_value(0x0090,0x0013); //0x0010 frenqucy
        lcd_wr_reg_value(0x0092,0x0300); 
        lcd_wr_reg_value(0x0093,0x0005); 
        lcd_wr_reg_value(0x0095,0x0000); 
        lcd_wr_reg_value(0x0097,0x0000); 
        lcd_wr_reg_value(0x0098,0x0000); 
  
        lcd_wr_reg_value(0x0001,0x0100); 
        lcd_wr_reg_value(0x0002,0x0700); 
        lcd_wr_reg_value(0x0003,0x1030); 
        lcd_wr_reg_value(0x0004,0x0000); 
        lcd_wr_reg_value(0x000C,0x0000); 
        lcd_wr_reg_value(0x000F,0x0000); 
        lcd_wr_reg_value(0x0020,0x0000); 
        lcd_wr_reg_value(0x0021,0x0000); 
        lcd_wr_reg_value(0x0007,0x0021); 
        delay_us(20);
        lcd_wr_reg_value(0x0007,0x0061); 
        delay_us(20);
        lcd_wr_reg_value(0x0007,0x0173); 
        delay_us(20);
	}							 
	else if(lcd_id==0x8989)
	{
		lcd_wr_reg_value(0x0000,0x0001); delay_us(5); // opens the crystal oscillator
    	lcd_wr_reg_value(0x0003,0xA8A4); delay_us(5); //0xA8A4
    	lcd_wr_reg_value(0x000C,0x0000); delay_us(5);    
    	lcd_wr_reg_value(0x000D,0x080C); delay_us(5);    
    	lcd_wr_reg_value(0x000E,0x2B00); delay_us(5);    
    	lcd_wr_reg_value(0x001E,0x00B0); delay_us(5);    
    	lcd_wr_reg_value(0x0001,0x2B3F); delay_us(5); // actuation output control 320*240 0x6B3F
    	lcd_wr_reg_value(0x0002,0x0600); delay_us(5); 
    	lcd_wr_reg_value(0x0010,0x0000); delay_us(5); 
    	lcd_wr_reg_value(0x0011,0x6070); delay_us(5); // definition data format 16 color horizontal screen 0x6058
    	lcd_wr_reg_value(0x0005,0x0000); delay_us(5); 
    	lcd_wr_reg_value(0x0006,0x0000); delay_us(5); 
    	lcd_wr_reg_value(0x0016,0xEF1C); delay_us(5); 
    	lcd_wr_reg_value(0x0017,0x0003); delay_us(5); 
    	lcd_wr_reg_value(0x0007,0x0233); delay_us(5); //0x0233       
    	lcd_wr_reg_value(0x000B,0x0000); delay_us(5); 
    	lcd_wr_reg_value(0x000F,0x0000); delay_us(5); // scanning starts the address
    	lcd_wr_reg_value(0x0041,0x0000); delay_us(5); 
    	lcd_wr_reg_value(0x0042,0x0000); delay_us(5); 
    	lcd_wr_reg_value(0x0048,0x0000); delay_us(5); 
    	lcd_wr_reg_value(0x0049,0x013F); delay_us(5); 
    	lcd_wr_reg_value(0x004A,0x0000); delay_us(5); 
    	lcd_wr_reg_value(0x004B,0x0000); delay_us(5); 
    	lcd_wr_reg_value(0x0044,0xEF00); delay_us(5); 
    	lcd_wr_reg_value(0x0045,0x0000); delay_us(5); 
    	lcd_wr_reg_value(0x0046,0x013F); delay_us(5); 
    	lcd_wr_reg_value(0x0030,0x0707); delay_us(5); 
    	lcd_wr_reg_value(0x0031,0x0204); delay_us(5); 
    	lcd_wr_reg_value(0x0032,0x0204); delay_us(5); 
    	lcd_wr_reg_value(0x0033,0x0502); delay_us(5); 
    	lcd_wr_reg_value(0x0034,0x0507); delay_us(5); 
    	lcd_wr_reg_value(0x0035,0x0204); delay_us(5); 
    	lcd_wr_reg_value(0x0036,0x0204); delay_us(5); 
    	lcd_wr_reg_value(0x0037,0x0502); delay_us(5); 
    	lcd_wr_reg_value(0x003A,0x0302); delay_us(5); 
    	lcd_wr_reg_value(0x003B,0x0302); delay_us(5); 
    	lcd_wr_reg_value(0x0023,0x0000); delay_us(5); 
    	lcd_wr_reg_value(0x0024,0x0000); delay_us(5); 
    	lcd_wr_reg_value(0x0025,0x8000); delay_us(5); 
    	lcd_wr_reg_value(0x004f,0);        // line of first site 0
    	lcd_wr_reg_value(0x004e,0);        // row first site 0
	} 
	else if(lcd_id==0x4531)
	{
		lcd_wr_reg_value(0X00,0X0001);   
		delay_us(50);   
		lcd_wr_reg_value(0X10,0X1628);   
		lcd_wr_reg_value(0X12,0X000e); //0x0006    
		lcd_wr_reg_value(0X13,0X0A39);   
		delay_us(10);   
		lcd_wr_reg_value(0X11,0X0040);   
		lcd_wr_reg_value(0X15,0X0050);   
		delay_us(40);   
		lcd_wr_reg_value(0X12,0X001e); //16    
		delay_us(40);   
		lcd_wr_reg_value(0X10,0X1620);   
		lcd_wr_reg_value(0X13,0X2A39);   
		delay_us(10);   
		lcd_wr_reg_value(0X01,0X0100);   
		lcd_wr_reg_value(0X02,0X0300);   
		lcd_wr_reg_value(0X03,0X1030); // change direction   
		lcd_wr_reg_value(0X08,0X0202);   
		lcd_wr_reg_value(0X0A,0X0008);   
		lcd_wr_reg_value(0X30,0X0000);   
		lcd_wr_reg_value(0X31,0X0402);   
		lcd_wr_reg_value(0X32,0X0106);   
		lcd_wr_reg_value(0X33,0X0503);   
		lcd_wr_reg_value(0X34,0X0104);   
		lcd_wr_reg_value(0X35,0X0301);   
		lcd_wr_reg_value(0X36,0X0707);   
		lcd_wr_reg_value(0X37,0X0305);   
		lcd_wr_reg_value(0X38,0X0208);   
		lcd_wr_reg_value(0X39,0X0F0B);   
		lcd_wr_reg_value(0X41,0X0002);   
		lcd_wr_reg_value(0X60,0X2700);   
		lcd_wr_reg_value(0X61,0X0001);   
		lcd_wr_reg_value(0X90,0X0210);   
		lcd_wr_reg_value(0X92,0X010A);   
		lcd_wr_reg_value(0X93,0X0004);   
		lcd_wr_reg_value(0XA0,0X0100);   
		lcd_wr_reg_value(0X07,0X0001);   
		lcd_wr_reg_value(0X07,0X0021);   
		lcd_wr_reg_value(0X07,0X0023);   
		lcd_wr_reg_value(0X07,0X0033);   
		lcd_wr_reg_value(0X07,0X0133);   
		lcd_wr_reg_value(0XA0,0X0000); 
	}	 
	else if(lcd_id==0x0047)	// HX8347A(Himax)
	{
		lcd_wr_reg_value(0x0019,0x0071);//osc setting //71
		delay_ms(10);
		lcd_wr_reg_value(0x0093,0x0000);   //0ff
		lcd_wr_reg_value(0x001B,0x0018);//exit sleep
		delay_ms(10);
		
		lcd_wr_reg_value(0x001B,0x0014);//exit sleep
		delay_ms(50);
		
		
		/***************display setting*******************/
		
		lcd_wr_reg_value(0x0001,0x0006);//display control 3
		delay_ms(10);
		
		//lcd_wr_reg_value(0x0016,0x00c8);//memory access control //c8
		lcd_wr_reg_value(0x0016,0x0008);
		delay_ms(10);
		
		lcd_wr_reg_value(0x0018,0x0000);//memory access control
		delay_ms(10);
		
		lcd_wr_reg_value(0x0023,0x0095);//cycle control  //95
		delay_ms(10);
		
		lcd_wr_reg_value(0x0024,0x0095);//cycle control
		delay_ms(10);
		
		lcd_wr_reg_value(0x0025,0x00ff);//cycle control
		delay_ms(10);
		
		lcd_wr_reg_value(0x0027,0x0002);// BP/FP SETTING
		delay_ms(10);
		
		lcd_wr_reg_value(0x0028,0x0002);// BP/FP SETTING
		delay_ms(10);
		
		lcd_wr_reg_value(0x0029,0x0002);// BP/FP SETTING
		delay_ms(10);
		
		lcd_wr_reg_value(0x002a,0x0002);// BP/FP SETTING
		delay_ms(10);
		
		lcd_wr_reg_value(0x002c,0x0002);// BP/FP SETTING
		delay_ms(10);
		
		lcd_wr_reg_value(0x002d,0x0002);// BP/FP SETTING
		delay_ms(10);
		
		lcd_wr_reg_value(0x003a,0x0000);//cycle control 1
		delay_ms(10);
		
		lcd_wr_reg_value(0x003b,0x0000);//cycle control 2
		delay_ms(10);
		
		lcd_wr_reg_value(0x003c,0x00f0);//cycle control 3
		delay_ms(10);
		
		lcd_wr_reg_value(0x003d,0x0000);//cycle control 4
		delay_ms(20);
		
		
		/**********power setting**********************/
		
		lcd_wr_reg_value(0x0042,0x001f);//BGP control
		delay_ms(50);
		
		lcd_wr_reg_value(0x001c,0x0006);// AP2-0 SETTING 
		delay_ms(30);
		
		lcd_wr_reg_value(0x001D,0x0000);//VLCD SETTING  
		delay_ms(30);
		
		lcd_wr_reg_value(0x001E,0x0000);//VREG3 SETTING  
		delay_ms(30);
		
		lcd_wr_reg_value(0x001F,0x00e);//VREG1 //00  //0e
		delay_ms(30);
		
		lcd_wr_reg_value(0x0020,0x0010);// VGL/VGH SETTING   
		delay_ms(30);
		
		lcd_wr_reg_value(0x0021,0x0011);// DC/DC SETTING   
		delay_ms(30);
		
		lcd_wr_reg_value(0x0043,0x0080);//vomg control
		delay_ms(50);
		
		lcd_wr_reg_value(0x0044,0x0036);  //vcomh control//35
		delay_ms(10);
		
		lcd_wr_reg_value(0x0045,0x000a);//vcom control //0a
		delay_ms(20);
		
		/******************GAMMA SETTING***********************/
		
		lcd_wr_reg_value(0x0046,0x0094);//GAMMA control 1  
		
		lcd_wr_reg_value(0x0047,0x0021);//GAMMA control 2  
		
		lcd_wr_reg_value(0x0048,0x0000);//GAMMA control 3   
		
		lcd_wr_reg_value(0x0049,0x0033);//GAMMA control 4  
		
		lcd_wr_reg_value(0x004A,0x0023);//GAMMA control 5  
		
		lcd_wr_reg_value(0x004B,0x0045);//GAMMA control 6   
		
		lcd_wr_reg_value(0x004C,0x0044);//GAMMA control 7 
		
		lcd_wr_reg_value(0x004D,0x0077);//GAMMA control  8 
		
		lcd_wr_reg_value(0x004E,0x0012);//GAMMA control 9   
		
		lcd_wr_reg_value(0x004F,0x0001);//GAMMA control 10 
		
		lcd_wr_reg_value(0x0050,0x0001);//GAMMA control 11  
		
		lcd_wr_reg_value(0x0051,0x0001);//GAMMA control 12  
		delay_ms(100);
		/****************display on setting**************/
		
		lcd_wr_reg_value(0x0026,0x0004);
		delay_ms(20);
		
		lcd_wr_reg_value(0x0026,0x0024);
		delay_ms(20);
		
		lcd_wr_reg_value(0x0026,0x002c);
		delay_ms(20);
		
		lcd_wr_reg_value(0x0026,0x003c);
		delay_ms(20);
	} 

#ifdef LCD32_DBG_LOG
	usart1_transmit_string("\r\nlcd32_clear_screen()\r\n");
#endif

	// 3.2 lcd backlight on
	LCD_PWM_LIGHT_HIGH(); //--> 3.2 LCD original code	
	delay_ms(20);
	
	lcd32_clear_screen(BLUE);
	//lcd32_draw_rectangle(20, 20, 100, 100);
	//lcd32_fill_color(30, 30, 90, 90, RED);
}

// starts to write GRAM
void lcd32_writeram_prepare(void)
{
	lcd_wr_reg(R34);
}	 

void lcd32_setcursor (u16 Xpos, u16 Ypos)
{
	if(lcd_id == 0x0047)
	{
		lcd_wr_reg_value(0x0002,Xpos>>8);	   // Column address start2
		lcd_wr_reg_value(0x0003,Xpos);	   // Column address start1
		lcd_wr_reg_value(0x0004,Xpos>>8);	   // Column address end2
		lcd_wr_reg_value(0x0005,Xpos);	   // Column address end1
		lcd_wr_reg_value(0x0006,Ypos>>8);	   // Row address start2
		lcd_wr_reg_value(0x0007,Ypos);	   // Row address start1
	    lcd_wr_reg_value(0x0008,Ypos>>8);	   // Row address end2
		lcd_wr_reg_value(0x0009,Ypos);	   // Row address end1
	}
	else
	{
		lcd_wr_reg_value (R32, Xpos);
		lcd_wr_reg_value (R33, Ypos);
	}

	//usart1_transmit_string("\r\n2\r\n");
} 

// 320 * 240
void lcd32_clear_screen(unsigned int color)
{                    

	u32 index=0;      


	if(lcd_id == 0x0047)
	{
		// Setcurosr
		lcd_wr_reg_value(0x0002,0x00);	   // Column address start2
		lcd_wr_reg_value(0x0003,0x00);	   // Column address start1
		lcd_wr_reg_value(0x0004,0x00);	   // Column address end2
		lcd_wr_reg_value(0x0005,0xef);	   // Column address end1
		lcd_wr_reg_value(0x0006,0x00);	   // Row address start2
		lcd_wr_reg_value(0x0007,0x00);	   // Row address start1
	    lcd_wr_reg_value(0x0008,0x01);	   // Row address end2
		lcd_wr_reg_value(0x0009,0x3f);	   // Row address end1
		// Setcurosr
	}
	else
	{
		lcd32_setcursor(0x00,0x0000); // establishment cursor position 
	}


	lcd32_writeram_prepare();     // starts to read in GRAM	 	  

	for (index=0; index<320*240; index++)
	{
		lcd_wr_data(color);    
	}
}

void lcd32_draw_point (u16 x, u16 y)
{
	lcd32_setcursor(x, y); // establishment cursor position 
	lcd_wr_reg(R34); // starts to read in GRAM
	lcd_wr_data(POINT_COLOR); 
} 	

void lcd32_set_window(u16 x1, u16 y1, u16 x2, u16 y2)
{
	if(lcd_id == 0x0047)
	{
		lcd_wr_reg_value(0x0002,x1>>8);	   // Column address start2
		lcd_wr_reg_value(0x0003,x1);	   // Column address start1
		lcd_wr_reg_value(0x0004,x2>>8);	   // Column address end2
		lcd_wr_reg_value(0x0005,x2);	   // Column address end1
		lcd_wr_reg_value(0x0006,y1>>8);	   // Row address start2
		lcd_wr_reg_value(0x0007,y1);	   // Row address start1
	    lcd_wr_reg_value(0x0008,y2>>8);	   // Row address end2
		lcd_wr_reg_value(0x0009,y2);	   // Row address end1
	}
	else
	{
		lcd_wr_reg_value (R80, x1); // horizontal direction GRAM start address
		lcd_wr_reg_value (R81, x2); // horizontal direction GRAM end address
		lcd_wr_reg_value (R82, y1); // vertical direction GRAM start address
		lcd_wr_reg_value (R83, y2); // vertical direction GRAM end address	
	}
}

void lcd32_fill_color(u8 xsta, u16 ysta, u8 xend, u16 yend, u16 color)
{                    
    u32 n;

	// establishment window		
	lcd32_set_window(xsta, ysta, xend, yend);
									
	//lcd_wr_reg_value (R80, xsta); // horizontal direction GRAM start address
	//lcd_wr_reg_value (R81, xend); // horizontal direction GRAM end address
	//lcd_wr_reg_value (R82, ysta); // vertical direction GRAM start address
	//lcd_wr_reg_value (R83, yend); // vertical direction GRAM end address	

	if(lcd_id != 0x0047)
		lcd32_setcursor(xsta, ysta); // establishment cursor position  

	lcd32_writeram_prepare();  // starts to read in GRAM	 	   	   
	n=(u32)(yend-ysta+1)*(xend-xsta+1);    


	while (n--) 
	{
		// demonstration fills color. 
		lcd_wr_data(color);
	}

	// resumes the establishment
	lcd32_set_window(0x0000, 0x0000, 0x00EF, 0x013F);
	//lcd_wr_reg_value (R80, 0x0000); // horizontal direction GRAM start address
	//lcd_wr_reg_value (R81, 0x00EF); // horizontal direction GRAM end address
	//lcd_wr_reg_value (R82, 0x0000); // vertical direction GRAM start address
	//lcd_wr_reg_value (R83, 0x013F); // vertical direction GRAM end address	    
}  

// draws a line
//x1, y1: Beginning coordinates
//x2, y2: End point coordinates  
void lcd32_draw_line (u16 x1, u16 y1, u16 x2, u16 y2)
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
		lcd32_draw_point(uRow, uCol); // picture spot 
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
// picture rectangle
void lcd32_draw_rectangle (u8 x1, u16 y1, u8 x2, u16 y2)
{
	lcd32_draw_line(x1, y1, x2, y1);
	lcd32_draw_line(x1, y1, x1, y2);
	lcd32_draw_line(x1, y2, x2, y2);
	lcd32_draw_line(x2, y1, x2, y2);
}
// is assigning the position to draw one to assign the size the circle
// (x, y): Central point
//r: Radius
void lcd32_draw_circle(u8 x0, u16 y0, u8 r)
{
	int a, b;
	int di;
	a=0; b=r;	  
	di=3-(r<<1);             // judgment spot position symbol
	while(a<=b)
	{
		lcd32_draw_point(x0-b, y0-a);             //3           
		lcd32_draw_point(x0+b, y0-a);             //0           
		lcd32_draw_point(x0-a, y0+b);             //1       
		lcd32_draw_point(x0-b, y0-a);             //7           
		lcd32_draw_point(x0-a, y0-b);             //2             
		lcd32_draw_point(x0+b, y0+a);             //4               
		lcd32_draw_point(x0+a, y0-b);             //5
		lcd32_draw_point(x0+a, y0+b);             //6 
		lcd32_draw_point(x0-b, y0+a);             
		a++;
		// uses the Bresenham algorithm to make a circle     
		if(di<0)di +=4*a+6;	  
		else
		{
			di+=10+4*(a-b);   
			b--;
		} 
		lcd32_draw_point(x0+a, y0+b);
	}
} 
// is assigning the position to demonstrate a character
//x: 0~234
//y: 0~308
//num: Must demonstrate character: ""---> " ~"
//size: Font size 12/16
//mode: The superimposition way (1) is also the non-superimposition way (0)
void lcd32_show_char (u8 x, u16 y, u8 num, u8 size, u8 mode)
{       

#define MAX_CHAR28_POSX 232
#define MAX_CHAR28_POSY 304 

    u8 temp;
    u8 pos, t;      

    if(x>MAX_CHAR28_POSX || y>MAX_CHAR28_POSY)
		return;	    

	// establishment window										
	lcd32_set_window(x, y, x+(size/2-1), y+size-1);

	//lcd_wr_reg_value(R80, x);           // horizontal direction GRAM start address
	//lcd_wr_reg_value(R81, x+(size/2-1)); // horizontal direction GRAM end address
	//lcd_wr_reg_value(R82, y);           // vertical direction GRAM start address
	//lcd_wr_reg_value(R83, y+size-1);    // vertical direction GRAM end address	

	if(lcd_id != 0x0047)
		lcd32_setcursor(x, y);            // establishment cursor position  
	lcd32_writeram_prepare();        // starts to read in GRAM	   
	num = num - ' '; // obtains the displacement value
	if (!mode) // non-superimposition way
	{
		for (pos=0; pos<size; pos++)
		{
			if(size==12)
				temp=asc2_1206[num][pos]; // transfers 1206 typefaces
			else 
				temp=asc2_1608[num][pos];		 // transfers 1608 typefaces

			for (t=0; t<size/2; t++)
		    {                 
		        if(temp&0x01)
				{
					lcd_wr_data(POINT_COLOR);
				} 
				else 
					lcd_wr_data(BACK_COLOR);	        

		        temp>>=1; 
		    }
		}	
	} else//superimposition way
	{
		for (pos=0; pos<size; pos++)
		{
			if(size==12)temp=asc2_1206[num][pos]; // transfers 1206 typefaces
			else temp=asc2_1608[num][pos];		 // transfers 1608 typefaces
			for (t=0; t<size/2; t++)
		    {                 
		        if(temp&0x01)lcd32_draw_point(x+t, y+pos); // draws a spot     
		        temp>>=1; 
		    }
		}
	}	    
	// restores the window size	 
	//lcd_wr_reg_value (R80, 0x0000); // horizontal direction GRAM start address
	//lcd_wr_reg_value (R81, 0x00EF); // horizontal direction GRAM end address
	//lcd_wr_reg_value (R82, 0x0000); // vertical direction GRAM start address
	//lcd_wr_reg_value (R83, 0x013F); // vertical direction GRAM end address
	lcd32_set_window(0x0000, 0x0000, 0x00EF, 0x013F);
}  
			 
// demonstrates 2 numerals
//x, y: Beginning coordinates	 
//len: Digital figure
//size: Font size
//color: Color
//num: Value (0~4294967295);	 
void lcd32_show_num(u8 x, u16 y, u32 num, u8 len, u8 size)
{         	
	u8 t, temp;
	u8 enshow=0;						   
	for (t=0; t<len; t++)
	{
		temp=(num/mypow(10, len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				lcd32_show_char(x+(size/2) *t, y, ' ', size,0);
				continue;
			} else enshow=1; 
		 	 
		}
	 	lcd32_show_char(x+(size/2) *t, y, temp+'0', size,0); 
	}
} 

// demonstration string of character
//x, y: Beginning coordinates  
// *p: String of character start address
// uses 16 typefaces
void lcd32_show_string (u8 x, u16 y, const u8 *p)
{         
    while (*p != '\0')
    {       
        if(x>MAX_CHAR28_POSX) 
		{
			x=0; 
			y+=16;
		}
        
		if(y>MAX_CHAR28_POSY) 
		{
			y=x=0; 
			lcd32_clear_screen(WHITE);
		}

        lcd32_show_char(x, y,*p,16,0);
        x+=8;
        p++;
    }  
}

void lcd32_draw_point_big(u16 x,u16 y)
{
	lcd32_draw_point(x,y);
	lcd32_draw_point(x+1,y);
	lcd32_draw_point(x,y+1);
	lcd32_draw_point(x+1,y+1);
}




