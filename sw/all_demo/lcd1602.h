

#ifndef __LCD1602_H
#define __LCD1602_H




/*
Pin Definition: RS (CS )=====> PG0
          RW (SID )====> PG1
          EN (SCLK )===> PG2
          PSB ========> PG3 / / low level for the serial mode
*/

#define LCD1602_EN_PORT  				GPIOD
#define LCD1602_EN_PIN 					GPIO_Pin_4
#define LCD1602_EN_HIGH()     			GPIO_SetBits(LCD1602_EN_PORT, LCD1602_EN_PIN)  
#define LCD1602_EN_LOW()     			GPIO_ResetBits(LCD1602_EN_PORT, LCD1602_EN_PIN)  

#define LCD1602_RW_PORT  				GPIOD
#define LCD1602_RW_PIN 					GPIO_Pin_5
#define LCD1602_RW_HIGH()     			GPIO_SetBits(LCD1602_RW_PORT, LCD1602_RW_PIN)  
#define LCD1602_RW_LOW()     			GPIO_ResetBits(LCD1602_RW_PORT, LCD1602_RW_PIN)  

#define LCD1602_RS_PORT  				GPIOD
#define LCD1602_RS_PIN 					GPIO_Pin_11
#define LCD1602_RS_HIGH()     			GPIO_SetBits(LCD1602_RS_PORT, LCD1602_RS_PIN)  
#define LCD1602_RS_LOW()     			GPIO_ResetBits(LCD1602_RS_PORT, LCD1602_RS_PIN)  

#if 0
#define LCDa_CTRL	    		LCD1602_PORT
#define LCDa_CTRL_DDR			LCD1602_DDR
#define LCDa_RS	        		LCD1602_RS_PIN_NO
#define LCDa_RW		    		LCD1602_RW_PIN_NO
#define LCDa_E	        		LCD1602_EN_PIN_NO

#define LCDa_DO		   			PORTA	// output data bus port 
#define LCDa_DI		    			PINA
#define LCDa_DATA_DDR			DDRA
#endif

#define LCDa_DO(x)	   			GPIOE->BSRR = 0xFF000000 | ( x<<8 )
//#define LCDa_DI		    		((u8)GPIOE->IDR)

//#define DataOut(x) GPIOB->BSRR = 0xFF000000 | ( x <<8 ); //PB8-15?

/******************************************************************/

#define LCDa_FUNCTION			0x38   	// LCD mode 8-bit, 2 lines, 5 * 8 characters 
#define LCDa_L1		    		0x80		// first line of address: 0x80 + addr, addr is the number of columns 
#define LCDa_L2		    		0xC0	// second line of address: 0x80 +0 x40 + addr 

#define LCDa_CGRAM_ADDR			0x40		// CGRAM start address 
#define LCDa_CGMAX   			64	    	// CGRAM maximum number of bytes stored 

#define LCDa_SET_RS		    	LCD1602_RS_HIGH()	// three control pins of the control operations 
#define LCDa_SET_RW 			LCD1602_RW_HIGH()
#define LCDa_SET_E  			LCD1602_EN_HIGH()

#define LCDa_CLR_RS 			LCD1602_RS_LOW()
#define LCDa_CLR_RW 			LCD1602_RW_LOW()
#define LCDa_CLR_E  			LCD1602_EN_LOW()






#define iDat	1		// data flag 
#define iCmd	0		// command flag 

#define LCDa_CLS	0x01			// clear the screen 
#define LCDa_HOME	0x02		// address return to origin, does not change the contents of DDRAM 
#define LCDa_ENTRY 	0x06		// set the input mode, the cursor plus, the screen does not move 
#define LCDa_C2L	0x10			// cursor to the left 
#define LCDa_C2R	0x14			// cursor right 
#define LCDa_D2L	0x18			// display left 
#define LCDa_D2R	0x1C		// screen and move

#define LCDa_ON		   	0x0C	// open the show 
#define LCDa_OFF		0x08		// turn off the display 
#define LCDa_CURON		0x0E		// show the cursor 
#define LCDa_CURFLA		0x0F		// open cursor blinking 



void bsp_lcd1602_gpio_init(void);
void bsp_lcd1602_init(void);
u8 LCD1602_readbyte(u8 DatCmd);
void LCD1602_sendbyte(u8 DatCmd, u8 dByte);
void LCD1602_Char_Set(u8 Row, u8 Col, u8 *ptString);
void LCD1602_clear(void);
u8 LCD1602_readBF(void);
void LCD1602_wrCGRAM(const u8 *ptCGRAM);
void LCD1602_disCGRAM(u8 Row,u8 Col);


#endif
