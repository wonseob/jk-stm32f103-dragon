/******************** (C) COPYRIGHT 2010 STMicroelectronics ********************
* File Name          : main.c
* Author             : MCD Application Team
* Version            : V3.2.1
* Date               : 04/18/2011
* Description        : 
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

/*
Predefine ¼³Á¤ 
100p
STM32F103VCT6(512KB, 64KB) :  USE_STDPERIPH_DRIVER, STM32F10X_HD
STM32F105VCT6(512KB, 64KB) :  USE_STDPERIPH_DRIVER, STM32F10X_CL
STM32F107VBT6(256KB, 64KB) :  USE_STDPERIPH_DRIVER, STM32F10X_CL
STM32F107VCT6(256KB, 64KB) :  USE_STDPERIPH_DRIVER, STM32F10X_CL
STM32F107VBT6(128KB, 32KB) :  USE_STDPERIPH_DRIVER, STM32F10X_CL

144p
STM32F103ZET6(512KB, 64KB) :  USE_STDPERIPH_DRIVER, STM32F10X_HD
*/

/* Includes ------------------------------------------------------------------*/
#include "stdio.h"
#include "stdlib.h"
#include "math.h"

#include "stm32f10x.h"
#include "usb_lib.h"
#include "hw_config.h"
#include "usb_pwr.h"
#include "main.h"
#include "led.h"
#include "timer.h"
#include "usart.h"
#include "rtc.h"
#include "mmc_sd.h"
#include "lcd.h"
#include "touch.h"
#include "key.h"
#include "nrf24l01.h"
#include "vs1003b.h"
#include "diskio.h"
#include "spi.h"
#include "at24c02.h"
#include "rs485.h"
#include "can.h"
#include "adc.h"
#include "dac.h"
#include "buzzer.h"
#include "lcd43.h"
#include "lcd70.h"
#include "lcd28.h"
#include "lcd32.h"
#include "lcd1602.h"
#include "lcd12864.h"

// fat
#include "ff.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Extern variables ----------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

// setting menu tree.
service_define_type s_menu_level = MainService;
u8 s_current_service = serviceMAX;

static volatile s16 s_blink_cnt = 0;
static volatile s16 s_wait_cnt = 0;

#ifdef NRF24L01_BPS_CHECK
static volatile s16 s_rf_bps_check_cnt = 0;
#endif

static volatile u16 s_key = 0;
static u8 rs485_data_count = 0;

// for mp3 player
//#define BUFF_SIZE  ( 512 )
#define BUFF_SIZE  ( 3092 )
u8 buffer[BUFF_SIZE]; // file copy buffer
	 
struct FileList
{
	u8 filename[65];
	struct FileList *next;
	struct FileList *back;
};
 
typedef struct FileList dnode;
typedef dnode *dlink;

extern uint16_t CAN_ID;
extern uint8_t CAN_DATA0,CAN_DATA1,CAN_DATA2,CAN_DATA3,CAN_DATA4,CAN_DATA5,CAN_DATA6,CAN_DATA7;
extern uint8_t CanFlag,Display;

extern tft_lcd_type curr_tft_lcd;
extern Pen_Holder Pen_Point;
extern u16 demo_selected_pen_color;

extern unsigned int  POINT_COLOR;
extern unsigned int  BACK_COLOR;

FATFS fs;            // Work area (file system object) for logical drive
DIR dirs;
FRESULT res;
FILINFO finfo;
FIL fsrc;      // file objects    

char path[50] = {""}; 
u16 read_count;
char szlcdpath[120];



const main_service_type gbl_main_service[1][serviceMAX] = 
{
	{
		{Ledservice, "[1] LED test\r\n", service_led, '1'},
		{TimerService, "[2] TIMER test\r\n", service_timer, '2'},	
		{RtcService, "[3] RTC test\r\n", service_rtc, '3'},
		{SdService, "[4] SD test\r\n", service_sd, '4'},
		{UsbService, "[5] USB test\r\n", service_usb, '5'},
		{LcdService, "[6] LCD test\r\n", service_lcd, '6'},
		{EepromService, "[7] EEPROM(AT2402) test\r\n", service_eeprom, '7'},
		{KeyService, "[8] KEY test\r\n", service_key, '8'},
		{RfService, "[9] nRF24L01 test\r\n", service_rf, '9'},
		{Mp3Service, "[a] MP3 Play test\r\n", service_mp3, 'a'},

		{Uart2Service, "[b] UART2 test\r\n", service_uart2, 'b'},
		{Rs485Service, "[c] RS485 test\r\n", service_rs485, 'c'},
		{CanService, "[d] CAN test\r\n", service_can, 'd'},
		{AdcService, "[e] ADC test\r\n", service_adc, 'e'},		
		{DacService, "[f] DAC test\r\n", service_dac, 'f'},		
		{BuzzerService, "[g] Buzzer test\r\n", service_buzzer, 'g'},
		{Lcd1602Service, "[h] LCD1602 test\r\n", service_lcd1602, 'h'},
		{Lcd12864Service, "[i] LCD12864 test\r\n", service_lcd12864, 'i'},

		{serviceMAX, 0x00, 0x00, 0x00}	
	}
};

const sub_service_type gbl_service[serviceMAX-1][10] = 
{
	{
		{ledUserOnService, "[1] User LED on\r\n", service_user_led_on, '1'},
		{ledUserOffService, "[2] User LED off\r\n", service_user_led_off, '2'},
		{ledExitService, "[x] Exit led test\r\n", service_exit, 'x'},
		{ledServiceMAX, 0x00, 0x00, 0x00}	
	},	
	{
		{timerTIM2Service, "[1] TIM2 ticktime test\r\n", service_tim2_ticktime, '1'},
		{timerTIM2Stop, "[2] TIM2 blink stop\r\n", service_tim2_stop, '2'},
		{timerExitService, "[x] Exit timer test\r\n", service_exit, 'x'},			
		{timerServiceMAX, 0x00, 0x00, 0x00}	
	},	
	{
		{rtcStartService, "[1] RTC service start\r\n", service_rtc_start, '1'},
		{rtcStopService, "[2] RTC service stop\r\n", service_rtc_stop, '2'},
		{rtcExitService, "[x] Exit RTC test\r\n", service_exit, 'x'},					
		{rtcServiceMAX, 0x00, 0x00, 0x00}	
	},		
	{
		{sdFileListService, "[1] SD SD File List\r\n", service_sd_file_list, '1'},
		{sdExitService, "[x] Exit SD test\r\n", service_exit, 'x'},					
		{sdServiceMAX, 0x00, 0x00, 0x00}	
	},	
	{
		{usbStartService, "[1] USB service start\r\n", service_usb_start, '1'},
		{usbStopService, "[2] USB service stop\r\n", service_usb_stop, '2'},
		{usbExitService, "[x] Exit USB test\r\n", service_exit, 'x'},					
		{usbServiceMAX, 0x00, 0x00, 0x00}	
	},	
	{
		{lcd28Service, "[1] LCD 2.8inch.\r\n", service_lcd28_start, '1'},
		{lcd28TouchService, "[2] LCD 2.8inch touch.\r\n", service_lcd28_touch_start, '2'},

		{lcd32Service, "[3] LCD 3.2inch.\r\n", service_lcd32_start, '3'},
		{lcd32TouchService, "[4] LCD 3.2inch touch\r\n", service_lcd32_touch_start, '4'},

		{lcd43Service, "[5] LCD 4.3inch.\r\n", service_lcd43_start, '5'},
		{lcd43TouchService, "[6] LCD 4.3inch touch.\r\n", service_lcd43_touch_start, '6'},

		{lcd70Service, "[7] LCD 7.0inch.\r\n", service_lcd70_start, '7'},
		{lcd70TouchService, "[8] LCD 7.0inch touch\r\n", service_lcd70_touch_start, '8'},

		{lcdExitService, "[x] Exit LCD test\r\n", service_exit, 'x'},					
		{lcdServiceMAX, 0x00, 0x00, 0x00}	
	},
	{
		{eepromWriteService, "[1] EEPROM(AT2402) write\r\n", service_eeprom_write, '1'},
		{eepromReadService, "[2] EEPROM(AT2402) read\r\n", service_eeprom_read, '2'},
		{eepromExitService, "[x] Exit EEPROM(AT2402) test\r\n", service_exit, 'x'},					
		{eepromServiceMAX, 0x00, 0x00, 0x00}	
	},		
	{
		{keyStartPollingService, "[1] KEY service start(polling & exti)\r\n", service_key_polling_start, '1'},
		{keyStopPollingService, "[2] KEY service stop(polling & exti)\r\n", service_key_polling_stop, '2'},
		{keyExitService, "[x] Exit KEY test\r\n", service_exit, 'x'},					
		{keyServiceMAX, 0x00, 0x00, 0x00}	
	},	
	{
		{rfStartRxService, "[1] nRF24L01 RX service start\r\n", service_rf_start_rx, '1'},
		{rfStartTxService, "[2] nRF24L01 TX service start\r\n", service_rf_start_tx, '2'},
#ifdef NRF24L01_BPS_CHECK		
		{rfStartRxBpsService, "[3] nRF24L01 RX(BPS check) service start\r\n", service_rf_bps_start_rx, '3'},
		{rfStartTxBpsService, "[4] nRF24L01 TX(BPS check) service start\r\n", service_rf_bps_start_tx, '4'},		
		{rfStopService, "[5] nRF24L01 service stop\r\n", service_rf_stop, '5'},
#else
		{rfStopService, "[3] nRF24L01 service stop\r\n", service_rf_stop, '3'},
#endif
		{rfExitService, "[x] Exit nRF24L01 test\r\n", service_exit, 'x'},					
		{rfServiceMAX, 0x00, 0x00, 0x00}	
	},	
	{
		{mp3StartPlayService, "[1] MP3 Play start\r\n", service_mp3play_start, '1'},
		{mp3StopPlayService, "[2] MP3 Play stop\r\n", service_mp3play_stop, '2'},
		{mp3ExitService, "[x] Exit MP3 Play test\r\n", service_exit, 'x'},					
		{mp3ServiceMAX, 0x00, 0x00, 0x00}	
	},	

	{
		{uart2StartService, "[1] UART2 start\r\n", service_uart2_start, '1'},
		{uart2ExitService, "[x] Exit UART2 test\r\n", service_exit, 'x'},					
		{uart2ServiceMAX, 0x00, 0x00, 0x00}	
	},	

	{
		{rs485MasterStartService, "[1] RS485 Master.\r\n", service_rs485_master_start, '1'},
		{rs485SlaveStartService, "[2] RS485 Slave.\r\n", service_rs485_slave_start, '2'},
		{rs485ExitService, "[x] Exit RS485 test\r\n", service_exit, 'x'},					
		{rs485ServiceMAX, 0x00, 0x00, 0x00}	
	},	

	{
		{canLoopbackStartService, "[1] CAN Loopback.\r\n", service_can_loopback_start, '1'},
		{canNormalStartService, "[2] CAN Normal.\r\n", service_can_normal_start, '2'},
		{canExitService, "[x] Exit CAN test\r\n", service_exit, 'x'},					
		{canServiceMAX, 0x00, 0x00, 0x00}	
	},	

	{
		{adc12_15Service, "[1] ADC12 IN15 VR Input.\r\n", service_adc12_15_start, '1'},
		{adc12_9Service, "[2] ADC12 IN9 Input.\r\n", service_adc12_9_start, '2'},
		{adc12_8Service, "[3] ADC12 IN8 Input.\r\n", service_adc12_8_start, '3'},
		{adcExitService, "[x] Exit ADC test\r\n", service_exit, 'x'},					
		{adcServiceMAX, 0x00, 0x00, 0x00}	
	},
	{
		{dac1_noiseService, "[1] DAC1 Noise output.\r\n", service_dac1_noise_start, '1'},
		{dacExitService, "[x] Exit DAC test\r\n", service_exit, 'x'},					
		{dacServiceMAX, 0x00, 0x00, 0x00}	
	},
	{
		{buzzerStartService, "[1] Buzzer Start.\r\n", service_buzzer_start, '1'},
		{buzzerStopService, "[2] Buzzer Stop.\r\n", service_buzzer_stop, '2'},
		{buzzerExitService, "[x] Exit Buzzer test\r\n", service_exit, 'x'},					
		{buzzerServiceMAX, 0x00, 0x00, 0x00}	
	},		
	{
		{lcd1602StartService, "[1] LCD1602 test.\r\n", service_lcd1602_start, '1'},
		{lcd1602ExitService, "[x] Exit LCD1602 test.\r\n", service_exit, 'x'},					
		{lcd1602ServiceMAX, 0x00, 0x00, 0x00}	
	},	
	{
		{lcd12864StartService, "[1] LCD12864 test.\r\n", service_lcd12864_start, '1'},
		{lcd12864ExitService, "[x] Exit LCD12864 test.\r\n", service_exit, 'x'},					
		{lcd12864ServiceMAX, 0x00, 0x00, 0x00}	
	},	


};

// #define LCD43_MP3_VOLUME_UP_DOWN_TEST

#ifdef LCD43_MP3_VOLUME_UP_DOWN_TEST

u32 vs1003b_volume_up = 0;
u32 vs1003b_volume_down = 0;
u8 vs1003b_current_volume = 0x44;

typedef enum { stream_none, stream_from_file, stream_from_memory } stream_type;

typedef struct {
	stream_type type;
	void* base_handler;
	uint8_t* buffer;
	size_t buffer_size;
	size_t buffer_available;
	uint8_t *buffer_ptr;
} stream_handle;

FIL file;

char szbuff[512];

typedef struct
{
	u8  pic_head[2];				//1
	u16 pic_size_l;			    //2
	u16 pic_size_h;			    //3
	u16 pic_nc1;				    //4
	u16 pic_nc2;				    //5
	u16 pic_data_address_l;	    //6
	u16 pic_data_address_h;		//7	
	u16 pic_message_head_len_l;	//8
	u16 pic_message_head_len_h;	//9
	u16 pic_w_l;					//10
	u16 pic_w_h;				    //11
	u16 pic_h_l;				    //12
	u16 pic_h_h;				    //13	
	u16 pic_bit;				    //14
	u16 pic_dip;				    //15
	u16 pic_zip_l;			    //16
	u16 pic_zip_h;			    //17
	u16 pic_data_size_l;		    //18
	u16 pic_data_size_h;		    //19
	u16 pic_dipx_l;			    //20
	u16 pic_dipx_h;			    //21	
	u16 pic_dipy_l;			    //22
	u16 pic_dipy_h;			    //23
	u16 pic_color_index_l;	    //24
	u16 pic_color_index_h;	    //25
	u16 pic_other_l;			    //26
	u16 pic_other_h;			    //27
	u16 pic_color_p01;		    //28
	u16 pic_color_p02;		    //29
	u16 pic_color_p03;		    //30
	u16 pic_color_p04;		    //31
	u16 pic_color_p05;		    //32
	u16 pic_color_p06;		    //33
	u16 pic_color_p07;		    //34
	u16 pic_color_p08;			//35			
}BMP_HEAD;

BMP_HEAD bmp;

typedef struct
{
	u16 x;
	u16 y;
	u8  r;
	u8  g;
	u8  b;		
}BMP_POINT;

BMP_POINT point;

UINT br, bw;         // File R/W count
u32 tx,ty,r_data,g_data,b_data;	




u32 RGB888ToRGB565(u8 r,u8 g,u8 b)
 {return (u32) (r & 0xF8) << 8 | (g & 0xFC) << 3 | (b & 0xF8) >> 3;}	//565

void lcd43_mp3_volume_up_down_test(void)
{
	// FATFS fs;            // Work area (file system object) for logical drive

	//FILINFO finfo;
  	unsigned int count=0;
	int j;
	UINT br;
	bool user_break;
	int len;

	u8 scan_bit1;
	u8 scan_bit2;
	u32 reg_data_lcd = 0x00, lcd_cnt = 0;


	s_current_service = mp3StartPlayService;
	
	//usart1_transmit_string("\r\nservice_mp3play_start()\r\n");

	s_key = KEY_NONE;	

	bsp_key_gpio_init();
	bsp_key_interrupt_init();
	register_exti_key_function(extiKey5ServiceFunction, key5_interrupt_event);

	bsp_lcd43_init();
	bsp_touch_init_interrupt();
	lcd43_touch_adjust_auto();

	// lcd43_draw_demo_icon_all();



	bsp_sd_gpio_init();

	if( disk_initialize(0) == 0 )
		usart1_transmit_string("\r\nSD initialize success.\r\n");
	else
		usart1_transmit_string("\r\nSD initialize failed.\r\n");  

	if(Vs1003_Init())
		usart1_transmit_string("\r\nvs1003b initialize failed..\r\n");
	else
		usart1_transmit_string("\r\nvs1003b initialize successed.\r\n");		


	// Already initialize SPI2 by disk_initialize()
	//  


#if 1
	VS1003B_Fill2048Zero();
#endif
	Vs1003SoftReset();	
	delay_ms(2);        
        

	/*
	// bmp display
	if ( f_mount( 0, &fs ) != FR_OK ) 
	{
		usart1_transmit_string ("\r\n fmount failed \r\n");
	}
	else 
	{
		strcpy(path, "0:/pics");
		scan_files(path, &user_break);
	}
	*/



loop_test:

		//lcd_wr_reg(0x0036); //rotation
		//lcd_wr_data(0x0000);
		//lcd_wr_data(0x0020);
		//lcd_wr_data(0x0080);  --> °¡·Î¼¼·Î ¹Ù²ñ
		//lcd_wr_data(0x00A0);
		//lcd_wr_data(0x0000);
		//delay_ms(500);


	lcd_wr_reg(0x0028); //display off

	f_mount(0,&fs);
	//bmp2lcd("0:/a.bmp");

	res = f_open(&fsrc, "0:/logo.bmp", FA_OPEN_EXISTING | FA_READ);	 //´òÉÏÍ¼Æ¬ÎÄ¼þÃû
	res = f_read(&fsrc, &bmp, sizeof(bmp), &br);
	if((bmp.pic_head[0]=='B')&&(bmp.pic_head[1]=='M'))
	{

	    usart1_transmit_string("BMP Formats\r\n");
		sprintf(szbuff, "size : %d\r\n", (bmp.pic_size_h<<16)|(bmp.pic_size_l));
		usart1_transmit_string(szbuff);

		sprintf(szbuff, "width£º %d\r\n",(bmp.pic_w_h<<16)|(bmp.pic_w_l));	
		usart1_transmit_string(szbuff);

		sprintf(szbuff, "height£º %d\r\n", (bmp.pic_h_h<<16)|(bmp.pic_h_l));		
		usart1_transmit_string(szbuff);

		sprintf(szbuff, "dipx£º %d\r\n",	(bmp.pic_dipx_h<<16)|(bmp.pic_dipx_l));  
		usart1_transmit_string(szbuff);

		sprintf(szbuff, "dipy£º %d\r\n",(bmp.pic_dipy_h<<16)|(bmp.pic_dipy_l));		
		usart1_transmit_string(szbuff);

		sprintf(szbuff, "pic_dip£º %d\r\n", bmp.pic_dip);				
		usart1_transmit_string(szbuff);

		sprintf(szbuff, "pic_bit£º %d\r\n", bmp.pic_bit);	
		usart1_transmit_string(szbuff);


#if 1
		res = f_lseek(&fsrc, ((bmp.pic_data_address_h<<16)|bmp.pic_data_address_l));

		
	
		for(tx=0;tx<bmp.pic_h_l;tx++)
		{
			//delay_ms(1);
			f_read(&fsrc, buffer, (bmp.pic_w_l)*3, &br);
			for(ty=0;ty<bmp.pic_w_l;ty++)
			{

				r_data = *(ty*3 +2+buffer);
				g_data = *(ty*3 +1+buffer);
				b_data = *(ty*3 +0+buffer);	
				
				if(tx<480)
				{
					point.x = tx;
					point.y = ty;
					point.r = r_data;
					point.g = g_data;
					point.b = b_data;
					
					lcd43_set_cursor(point.x ,point.y);
					lcd43_write_ram_prepare();
					//lcd_wr_data((point.r | point.g | point.b));
					lcd_wr_data(RGB888ToRGB565(point.r,point.g,point.b));

                	// lcd43_draw_point(point.x ,point.y,RGB888ToRGB565(point.r,point.g,point.b));
                }		

    		}				
		}
		f_close(&fsrc);	
	}

	lcd_wr_reg(0x0029); //display on
/*

		lcd_wr_reg(0x0036); //rotation
		lcd_wr_data(0x0000);
		delay_ms(500);



	if( lcd_cnt == 0 )
	{
		lcd_wr_reg(0x0036); //rotation
		lcd_wr_data(0x0000);
		delay_ms(500);
		lcd43_clear_screen(GREEN);
		goto loop_test;
	}
	else if( lcd_cnt == 1 )
	{
		lcd_wr_reg(0x0036); //rotation
		lcd_wr_data(0x00f3);
		delay_ms(500);
		lcd43_clear_screen(GREEN);
		goto loop_test;
	}
	else if( lcd_cnt == 2 )
	{
		lcd_wr_reg(0x0036); //rotation
		lcd_wr_data(0x0013);
		delay_ms(500);
		lcd43_clear_screen(GREEN);
		goto loop_test;
	}

	else if( lcd_cnt == 3 )
	{
		lcd_wr_reg(0x0036); //rotation
		lcd_wr_data(0x0001);
		delay_ms(500);
		lcd43_clear_screen(GREEN);
		goto loop_test;
	}
	else if( lcd_cnt == 4 )
	{
		lcd_wr_reg(0x0036); //rotation
		lcd_wr_data(0x0081);
		delay_ms(500);
		lcd43_clear_screen(GREEN);
		goto loop_test;
	}
	else if( lcd_cnt == 5 )
	{
		lcd_wr_reg(0x0036); //rotation
		lcd_wr_data(0x0041);
		delay_ms(500);
		lcd43_clear_screen(GREEN);
		goto loop_test;
	}
*/


	//lcd_cnt++;
#endif


    sprintf(szlcdpath, "Current volume  = %d.", vs1003b_current_volume);
    lcd43_show_string(300,120, szlcdpath, RED);


loop_play:
        
	f_mount(0,&fs);

	if (f_opendir (&dirs, path) == FR_OK) 
  	{
  		//usart0_puts ("\r\n3");
      	while (f_readdir (&dirs, &finfo) == FR_OK)  
    	{
    			//usart0_puts ("\r\n4");
      		if (finfo.fattrib & AM_ARC) 
      		{      			
      			//usart0_puts ("\r\n5");
    			if (!finfo.fname[0])	
      				break;

				//usart1_transmit_string_format ("\r\n file name is: \n %s \n", finfo.fname);
				len = strlen(finfo.fname);
				if( finfo.fname[len-1] == '3' )
				{
					sprintf(szlcdpath, "%s Playing...", finfo.fname);				
					lcd43_show_string(100,120, szlcdpath, RED);
	                                //usart1_transmit_string ("\r\n Playing¡¦");
	
					res = f_open (&fsrc, finfo.fname, FA_OPEN_EXISTING | FA_READ); 

				
				//read_count = 0;



					for (; ;) 
					{
						// delay_ms(1);
	
						if( vs1003b_volume_up == 1) 
						{
							vs1003b_volume_up = 0;
							vs1003b_current_volume = vs1003b_current_volume - 0x1; 
							vs1003b_volume_set((vs1003b_current_volume * 256) + vs1003b_current_volume);
	                                                sprintf(szlcdpath, "Volume up  = %d.        ", vs1003b_current_volume);
							lcd43_show_string(300,120, szlcdpath, RED);
						}
						if( vs1003b_volume_down == 1)
						{
							vs1003b_volume_down = 0;
							vs1003b_current_volume = vs1003b_current_volume + 0x1;
							vs1003b_volume_set((vs1003b_current_volume * 256) + vs1003b_current_volume);
	                                                sprintf(szlcdpath, "Volume down  = %d.        ", vs1003b_current_volume);
							lcd43_show_string(300,120, szlcdpath, RED);
						}
						
						res = f_read (&fsrc, buffer, sizeof(buffer), &br);
						//read_count++;
						// usart1_transmit_string_format ("\r\n read_count=%d\r\n", read_count);
						
						if(res==FR_OK) 
						{
							count=0;
							// delay_ms(1);
							
							while(count<BUFF_SIZE) 
							{					    		
						    	if(VS1003B_NeedData())
								{							
									//Vs1003_DATA_Write(buffer);	
									//count = count + 32;
									
									
									for (j=0; j<32; j++) // each time delivers 32 data
									{
										//if(VS1003B_NeedData())
										{
											MP3_DCS_SET(0); 										
											bsp_readwritebyte_spi2(buffer[count]);
											MP3_DCS_SET(1); 
											
											count++;
										}
									}
									
									
									// usart1_transmit_string_format ("\r\n count=%d\r\n", count);
									
								}
								
								
					    			if (res || br == 0) 
									break;   // error or eof	
							}
						
							// usart0_puts ((char*)buffer);
	
						}					
	
						if (res || br == 0) 
							break;   // error or eof		   
					}				

				
					f_close(&fsrc);
                                
                    goto loop_play;
				}
      		}                      
      	}
  }
  

}
#endif


void wait_10ms(s16 ms_10)
{
	s_wait_cnt = 0;
	while( s_wait_cnt < ms_10 );
}

void timer2_event(void)
{
	// 

	if( s_wait_cnt++ == 0xffff )
		s_wait_cnt = 0;

#ifdef NRF24L01_BPS_CHECK
	if( s_menu_level == RfService && s_current_service == rfStartTxBpsService)	
	{
		s_rf_bps_check_cnt++;
		if( s_rf_bps_check_cnt > 1000 )
		{
			s_rf_bps_check_cnt = 1001;
		}
	}
#endif

	if( (s_menu_level == KeyService && s_current_service == keyStartPollingService) || (s_current_service == mp3StartPlayService))
	{
		gpio_polling_key();
	}		

	// usart1_transmit_string_format("s_blink_cnt=%d\r\n", s_blink_cnt);	

	if( s_blink_cnt++ > 100 )
	{
		s_blink_cnt = 0;		

		if( s_menu_level == TimerService && s_current_service == timerTIM2Service )
		{
			bsp_led_core_toggle(ledUserCore);
		}

	}
}

void rtc_event(void)
{
	if(RTC_GetITStatus(RTC_IT_SEC) != RESET)
	{
		/* Clear the RTC Second interrupt */
		RTC_ClearITPendingBit(RTC_IT_SEC);

		/* Toggle led connected to PC.06 pin each 1s */
		GPIO_WriteBit(GPIOC, GPIO_Pin_6, (BitAction)(1-GPIO_ReadOutputDataBit(GPIOC, GPIO_Pin_6)));

		/* Enable time update */
		if( s_menu_level == RtcService && s_current_service == rtcStartService )
		{
			time_show();
		}

		/* Wait until last write operation on RTC registers has finished */
		RTC_WaitForLastTask();
		/* Reset RTC Counter when Time is 23:59:59 */
		if(RTC_GetCounter() == 0x00015180)
		{
			RTC_SetCounter(0x0);
			/* Wait until last write operation on RTC registers has finished */
			RTC_WaitForLastTask();
		}
	}
}

void welcome(void)
{
	usart1_transmit_string("\r\n*****************************************************************************\r\n");
	usart1_transmit_string("Rabbit Pro Development Board Ver 1.0\r\n");

#ifdef STM32F10X_CL
	// STM32F105, 107 ¿¡¼­´Â PLL¼³Á¤ÀÌ ¾à°£ Æ²¸®´Ù. 
	// ÀÌÈÄ¿¡ PLL¼³Á¤ ½ºÆ®¸µÀ» ¼öÁ¤ÇØ¾ß ÇÑ´Ù.
	usart1_transmit_string("HSE(High Speed External clock signal) Enable\r\n");
	usart1_transmit_string("HCLK(SYSCLK) = 8MHz, PCLK2 = HCLK, PCLK1 = HCLK/2, ADCCLK = PCLK2/2\r\n");
	usart1_transmit_string("PLLCLK = 8MHz / 1 * 9 = 72MHz, USBCLK = PLLCLK / 2 = 48MHz\r\n\r\n");
#else
	usart1_transmit_string("HSE(High Speed External clock signal) Enable\r\n");
	usart1_transmit_string("HCLK(SYSCLK) = 8MHz, PCLK2 = HCLK, PCLK1 = HCLK/2, ADCCLK = PCLK2/2\r\n");
	usart1_transmit_string("PLLCLK = 8MHz / 1 * 9 = 72MHz, USBCLK = PLLCLK / 2 = 48MHz\r\n");
#endif

}

void display_menu()
{
	int i;

	if( s_menu_level == MainService )
	{
		for(i=0;i<serviceMAX;i++)
		{
			if( gbl_main_service[MainService][i].run == 0x00 )
				break;
	
			// wait(5);
			usart1_transmit_string(gbl_main_service[MainService][i].service_string);
			//strcpy(gbl_zz, gbl_service[s_menu_level][i].service_string);
			//strcpy(zz, gbl_service[s_menu_level][i].service_string);
			
		}
	}
	else
	{
		for(i=0;i<serviceMAX;i++)
		{
			if(  gbl_service[s_menu_level-1][i].run == 0x00 )
				break;

			usart1_transmit_string(gbl_service[s_menu_level-1][i].service_string);			
		}	
	}
	
	usart1_transmit_string("\r\n\r\nSelect menu ? \r\n");
	
}


int run_menu_selection(void)
{
	int i;
	char* pdata;
	s8 data;

	pdata = (char*)usart1_get_data();

	if( pdata == 0x00 )
		return 0x00;

	data = pdata[0];

#if 0
	usart1_transmit_string_format("\r\n%c", data);
#endif

	if( s_menu_level == UsbService && s_current_service == usbStartService)
	{
		usb_mouse_control(data);
	}


	if( s_menu_level == MainService )
	{
		for(i=0;i<serviceMAX;i++)
		{
			if( gbl_main_service[MainService][i].run == 0x00 )
				break;

			if( gbl_main_service[MainService][i].cmd == data )
			{
				gbl_main_service[MainService][i].run();
				return 1;
			}
		}	
	
	}
	else
	{
		for(i=0;i<serviceMAX;i++)
		{
			if( gbl_service[s_menu_level-1][i].run == 0x00 )
				break;

			if( gbl_service[s_menu_level-1][i].cmd == data )
			{
				gbl_service[s_menu_level-1][i].run();
				return 1;
			}
		}	
	}


	return 0;
	
}

#undef LCD70_RECTANGLE_TEST
#ifdef LCD70_RECTANGLE_TEST
void lcd70_rectangle_test()
{
	int x1, y1, x2, y2;
	int i, x_depth, y_depth;
	u16 col;

	bsp_lcd70_init();
	bsp_touch_init_interrupt();
	lcd70_touch_adjust_auto();





	while(1)
	{
		x_depth = 5;
		y_depth = 5;
	
		x1 = 0 + x_depth;
		x2 = 800 - x_depth;
		y1 = 0 + y_depth;
		y2 = 480 - y_depth;

		col = RED;

		for(i=0;i<100;i++)
		{
			if(i == 0 )
				col = RED;
			else if(i == 1 )
				col = GREEN;
			else if(i == 2 )
				col = BLUE;
			else if(i == 3 )
				col = WHITE;
			else if(i == 4 )
				col = YELLOW;
			else if(i == 5 )
				col = BROWN;
			else
				col = GRAY;

			lcd70_draw_rectangle(x1, y1, x2, y2, col);
	
			x1 = x1 + x_depth;
			x2 = x2 - x_depth;
			y1 = y1 + y_depth;
			y2 = y2 - y_depth;
	
			if( (x2-x1 <= 20) || (y2-y1 <= 20) )
				break;
		}

		delay_ms(100);
		delay_ms(100);
		delay_ms(100);
		delay_ms(100);
		delay_ms(100);
		delay_ms(100);
		delay_ms(100);
		delay_ms(100);
		delay_ms(100);
		delay_ms(100);

		lcd70_clear_screen(BLACK);	
	}
}
#endif

#undef LCD70_PICTURE_DISPLAY_TEST
#ifdef LCD70_PICTURE_DISPLAY_TEST

typedef enum { stream_none, stream_from_file, stream_from_memory } stream_type;

typedef struct {
	stream_type type;
	void* base_handler;
	uint8_t* buffer;
	size_t buffer_size;
	size_t buffer_available;
	uint8_t *buffer_ptr;
} stream_handle;

FIL file;

char szbuff[512];

typedef struct
{
	u8  pic_head[2];				//1
	u16 pic_size_l;			    //2
	u16 pic_size_h;			    //3
	u16 pic_nc1;				    //4
	u16 pic_nc2;				    //5
	u16 pic_data_address_l;	    //6
	u16 pic_data_address_h;		//7	
	u16 pic_message_head_len_l;	//8
	u16 pic_message_head_len_h;	//9
	u16 pic_w_l;					//10
	u16 pic_w_h;				    //11
	u16 pic_h_l;				    //12
	u16 pic_h_h;				    //13	
	u16 pic_bit;				    //14
	u16 pic_dip;				    //15
	u16 pic_zip_l;			    //16
	u16 pic_zip_h;			    //17
	u16 pic_data_size_l;		    //18
	u16 pic_data_size_h;		    //19
	u16 pic_dipx_l;			    //20
	u16 pic_dipx_h;			    //21	
	u16 pic_dipy_l;			    //22
	u16 pic_dipy_h;			    //23
	u16 pic_color_index_l;	    //24
	u16 pic_color_index_h;	    //25
	u16 pic_other_l;			    //26
	u16 pic_other_h;			    //27
	u16 pic_color_p01;		    //28
	u16 pic_color_p02;		    //29
	u16 pic_color_p03;		    //30
	u16 pic_color_p04;		    //31
	u16 pic_color_p05;		    //32
	u16 pic_color_p06;		    //33
	u16 pic_color_p07;		    //34
	u16 pic_color_p08;			//35			
}BMP_HEAD;

BMP_HEAD bmp;

typedef struct
{
	u16 x;
	u16 y;
	u8  r;
	u8  g;
	u8  b;		
}BMP_POINT;

BMP_POINT point;

UINT br, bw;         // File R/W count
u32 tx,ty,r_data,g_data,b_data;	




u32 RGB888ToRGB565(u8 r,u8 g,u8 b)
 {return (u32) (r & 0xF8) << 8 | (g & 0xFC) << 3 | (b & 0xF8) >> 3;}	//565

void lcd70_bmp_display_test()
{
	UINT br;
	int bmp_index = 0;
	int ly = 480-1;

	s_key = KEY_NONE;	

	bsp_lcd70_init();
	bsp_touch_init_interrupt();
	lcd70_touch_adjust_auto();

	bsp_key_gpio_init();
	bsp_key_interrupt_init();
	register_exti_key_function(extiKey5ServiceFunction, key5_interrupt_event);


	bsp_sd_gpio_init();

	if( disk_initialize(0) == 0 )
		usart1_transmit_string("\r\nSD initialize success.\r\n");
	else
		usart1_transmit_string("\r\nSD initialize failed.\r\n");

	//lcd_wr_reg(0x0028); //display off

	f_mount(0,&fs);	

	while(1)
	{

		if( bmp_index == 0 )
			res = f_open(&fsrc, "0:/70_1.bmp", FA_OPEN_EXISTING | FA_READ);
		else if( bmp_index == 1 )
			res = f_open(&fsrc, "0:/70_2.bmp", FA_OPEN_EXISTING | FA_READ);
		else if( bmp_index == 2 )
			res = f_open(&fsrc, "0:/70_3.bmp", FA_OPEN_EXISTING | FA_READ);

		bmp_index++;
		if( bmp_index > 2 )
			bmp_index = 0;

		res = f_read(&fsrc, &bmp, sizeof(bmp), &br);
		if((bmp.pic_head[0]=='B')&&(bmp.pic_head[1]=='M'))
		{
	
		    usart1_transmit_string("BMP Formats\r\n");
			sprintf(szbuff, "size : %d\r\n", (bmp.pic_size_h<<16)|(bmp.pic_size_l));
			usart1_transmit_string(szbuff);
	
			sprintf(szbuff, "width£º %d\r\n",(bmp.pic_w_h<<16)|(bmp.pic_w_l));	
			usart1_transmit_string(szbuff);
	
			sprintf(szbuff, "height£º %d\r\n", (bmp.pic_h_h<<16)|(bmp.pic_h_l));		
			usart1_transmit_string(szbuff);
	
			sprintf(szbuff, "dipx£º %d\r\n",	(bmp.pic_dipx_h<<16)|(bmp.pic_dipx_l));  
			usart1_transmit_string(szbuff);
	
			sprintf(szbuff, "dipy£º %d\r\n",(bmp.pic_dipy_h<<16)|(bmp.pic_dipy_l));		
			usart1_transmit_string(szbuff);
	
			sprintf(szbuff, "pic_dip£º %d\r\n", bmp.pic_dip);				
			usart1_transmit_string(szbuff);
	
			sprintf(szbuff, "pic_bit£º %d\r\n", bmp.pic_bit);	
			usart1_transmit_string(szbuff);
	
	
	
			res = f_lseek(&fsrc, ((bmp.pic_data_address_h<<16)|bmp.pic_data_address_l));

			// lcd70_clear_screen(RED);	
	
			
		
#if 1
			ly = 480;
			for(ty=0;ty<bmp.pic_h_l;ty++)
			{
				//delay_ms(1);
				f_read(&fsrc, buffer, (bmp.pic_w_l)*3, &br);

				lcd70_set_area(800 ,ly, 0, ly);
				ly--;
				lcd70_write_ram_prepare();

				for(tx=0;tx<bmp.pic_w_l;tx++)
				{
	
					r_data = *(tx*3 +2+buffer);
					g_data = *(tx*3 +1+buffer);
					b_data = *(tx*3 +0+buffer);	
					
					if(tx<800)
					{
						//point.x = tx;
						//point.y = ty;
						point.r = r_data;
						point.g = g_data;
						point.b = b_data;
						
						//lcd_wr_data((point.r | point.g | point.b));
						lcd_wr_data(RGB888ToRGB565(point.r,point.g,point.b));
	
	                	//lcd70_draw_point(point.x ,point.y,RGB888ToRGB565(point.r,point.g,point.b));
	                }		
	
	    		}				
			}
#endif

#if 0
			for(ty=0;ty<bmp.pic_h_l;tx++)
			{
				//delay_ms(1);
				f_read(&fsrc, buffer, (bmp.pic_w_l)*3, &br);
				for(tx=0;tx<bmp.pic_w_l;tx++)
				{
	
					r_data = *(tx*3 +2+buffer);
					g_data = *(tx*3 +1+buffer);
					b_data = *(tx*3 +0+buffer);	
					
					if(tx<800)
					{
						point.x = tx;
						point.y = ty;
						point.r = r_data;
						point.g = g_data;
						point.b = b_data;
						
						lcd70_set_cursor(point.x ,point.y);
						lcd70_write_ram_prepare();
						//lcd_wr_data((point.r | point.g | point.b));
						lcd_wr_data(RGB888ToRGB565(point.r,point.g,point.b));
	
	                	// lcd43_draw_point(point.x ,point.y,RGB888ToRGB565(point.r,point.g,point.b));
	                }		
	
	    		}				
			}
#endif
			
		}


		f_close(&fsrc);	
	
		//lcd_wr_reg(0x0029); //display on
	}

}
#endif



/*******************************************************************************
* Function Name  : main.
* Description    : main routine.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
int main(void)
{
	/* Initialize the Demo */
	bsp_init_rcc();

	bsp_init_gpio();

#ifdef USB_DISCONNECT_CONFIG
	USB_Cable_Config(DISABLE);
#endif

	delay_ms(100);

	bsp_init_interrupt();

	register_timer_function(timer2ServiceFunction, timer2_event);
	register_rtc_function(rtcServiceFunction, rtc_event);

	bsp_init_timer2();

	bsp_init_irq_usart1();
	//bsp_init_dma_usart1();

	// 10msec
	// wait_10ms(1);

	// User LED ON
	bsp_led_core_off(ledUserCore);
	bsp_led_core_on(ledUserCore);

	welcome();
	
	usart1_transmit_string("\r\n*****************************************************************************\r\n");	
	usart1_transmit_string("User led2 ( on )\r\n");
	usart1_transmit_string("Initialize gpio service.\r\n");
	usart1_transmit_string("Start USART1 service on mode interrupt.\r\n");
	usart1_transmit_string_format("BaudRate = %d, Databit = %dbit, StopBits = %d, Parity = no, FlowControl = none\r\n", 115200, 8, 1);	
	usart1_transmit_string("\r\n*****************************************************************************\r\n");		

	display_menu();

	usart1_tx_proc();

	// for lcd direct test
	//bsp_lcd43_init();
	 

	// service_lcd43_start();
	// bsp_lcd70_init();
	//service_lcd43_touch_start();
	// service_lcd70_touch_start();
	//service_lcd70_start();

	//lcd70_draw_point(0, 0, BLUE);

	//service_lcd28_start();
#ifdef LCD43_MP3_VOLUME_UP_DOWN_TEST
	lcd43_mp3_volume_up_down_test();
#endif

#ifdef LCD70_RECTANGLE_TEST
	lcd70_rectangle_test();
#endif

#ifdef LCD70_PICTURE_DISPLAY_TEST
	lcd70_bmp_display_test();
#endif
	
	while( 1 )
	{

		if( run_menu_selection() != 0 )
			display_menu();
	}

}
				  


void service_led(void)
{
	s_menu_level = Ledservice;
	//usart1_transmit_string("\r\nservice_led()\r\n");
}

void service_timer(void)
{
	s_menu_level = TimerService;
	//usart1_transmit_string("\r\nservice_timer()\r\n");	
}

void service_rtc(void)
{
	s_menu_level = RtcService;
	//usart1_transmit_string("\r\nservice_rtc()\r\n");	
}

void service_sd(void)
{
	s_menu_level = SdService;
	//usart1_transmit_string("\r\nservice_sd()\r\n");	
}

void service_usb(void)
{
	s_menu_level = UsbService;
	//usart1_transmit_string("\r\nservice_usb()\r\n");	
}

void service_lcd(void)
{
	s_menu_level = LcdService;
	//usart1_transmit_string("\r\nservice_lcd()\r\n");	
}

void service_eeprom(void)
{
	s_menu_level = EepromService;
	//usart1_transmit_string("\r\nservice_eeprom()\r\n");	
}

void service_key(void)
{
	s_menu_level = KeyService;
	//usart1_transmit_string("\r\nservice_key()\r\n");	
}

void service_rf(void)
{
	s_menu_level = RfService;
	//usart1_transmit_string("\r\nservice_rf()\r\n");	
}

void service_mp3(void)
{
	s_menu_level = Mp3Service;
	//usart1_transmit_string("\r\nservice_mp3()\r\n");	
}

void service_uart2(void)
{
	s_menu_level = Uart2Service;
	//usart1_transmit_string("\r\nservice_uart2()\r\n");		
}

void service_rs485(void)
{
	s_menu_level = Rs485Service;
	//usart1_transmit_string("\r\nservice_rs485()\r\n");	
}

void service_can(void)
{
	s_menu_level = CanService;
	//usart1_transmit_string("\r\nservice_can()\r\n");	
}

void service_adc(void)
{
	s_menu_level = AdcService;
	//usart1_transmit_string("\r\nservice_adc()\r\n");	
}

void service_dac(void)
{
	s_menu_level = DacService;
	//usart1_transmit_string("\r\nservice_dac()\r\n");	
}

void service_buzzer(void)
{
	s_menu_level = BuzzerService;
	//usart1_transmit_string("\r\nservice_buzzer()\r\n");	
}

void service_lcd1602(void)
{
	s_menu_level = Lcd1602Service;
	//usart1_transmit_string("\r\nservice_lcd1602()\r\n");	
}

void service_lcd12864(void)
{
	s_menu_level = Lcd12864Service;
	//usart1_transmit_string("\r\nservice_lcd12864()\r\n");	
}


void service_user_led_on(void)
{
	bsp_led_core_on(ledUserCore);
	bsp_led_bottom_on(ledUserBottom1);
	bsp_led_bottom_on(ledUserBottom2);
	bsp_led_bottom_on(ledUserBottom3);
	//usart1_transmit_string("\r\nservice_user_led_core_on()\r\n");
}

void service_user_led_off(void)
{
	bsp_led_core_off(ledUserCore);
	bsp_led_bottom_off(ledUserBottom1);
	bsp_led_bottom_off(ledUserBottom2);
	bsp_led_bottom_off(ledUserBottom3);
	//usart1_transmit_string("\r\nservice_user_led_core_off()\r\n");
}



void service_tim2_ticktime(void)
{
	usart1_transmit_string("\r\nservice_tim2_ticktime()\r\n");	
	usart1_transmit_string("Start timer2 10msec period.\r\n");
	usart1_transmit_string("User led blink start 1 second period\r\n");

	s_current_service = timerTIM2Service;
}

void service_tim2_stop(void)
{
	//usart1_transmit_string("\r\nservice_tim2_stop()\r\n");	
	usart1_transmit_string("User led blink stop.\r\n");

	s_current_service = timerTIM2Stop;
}



void service_rtc_start(void)
{

	//usart1_transmit_string("\r\nservice_rtc_start()\r\n");	

	bsp_init_rtc();
	// byte cur_time [7] = {0x55, 0x14, 0x10, 0x19, 0x06, 0x05, 0x09}; // seconds, minutes, hours, days, months, weeks, years

	// bsp_ds1302_gpio_init();

	// ds1302_set_time(cur_time);

	// year-month-days week time-minute-seconds
	usart1_transmit_string("\r\n Set 2009-06-19 5 10:14:56 \r\n");		
	usart1_transmit_string("Start RTC Service to uart.\r\n");

	time_show();

	s_current_service = rtcStartService;
}

void service_rtc_stop(void)
{
	//usart1_transmit_string("\r\nservice_rtc_stop()\r\n");	
	usart1_transmit_string("Stop RTC Service to uart.\r\n");

	s_current_service = rtcStopService;
}


void service_sd_file_list(void)
{

	bsp_sd_gpio_init();

	if( disk_initialize(0) == 0 )
		usart1_transmit_string("\r\nSD initialize success.\r\n");
	else
		usart1_transmit_string("\r\nSD initialize failed.\r\n");

  
	usart1_transmit_string("\r\nservice_sd_file_list()\r\n");	

	f_mount(0,&fs);

	//usart1_transmit_string("\r\nf_mount()\r\n");	

	if (f_opendir (&dirs, path) == FR_OK) 
  	{
  		//usart1_transmit_string("\r\nf_opendir()\r\n");	
      	while (f_readdir (&dirs, &finfo) == FR_OK)  
    	{
			if (!finfo.fname[0])	
				break;
			else
				usart1_transmit_string_format("%s\r\n", finfo.fname);	

    		//usart1_transmit_string("\r\nf_readdir()\r\n");	
#if 0
			if ((finfo.fattrib & AM_DIR)) 
				usart1_transmit_string_format("%s", finfo.fname);	

      		if (!(finfo.fattrib & AM_DIR)) 
      		{
      			
        			if (!finfo.fname[0])	
        			{
        				//usart1_transmit_string("\r\nfinfo.fname\r\n");	
          				break;         

					}
        			usart1_transmit_string_format ("\r\n%s", finfo.fname);
      		}  
			else
			{
				break;
			}                    
#endif
      	}


		usart1_transmit_string("\r\n");	
	}

	//sd_size = SD_GetCapacity();

	//usart1_transmit_string_format("\r\nCapacity is %d Bytes\r\n", sd_size);	

	s_current_service = sdFileListService;
}

void service_usb_start(void)
{

#ifdef USB_DISCONNECT_CONFIG
	USB_Cable_Config(ENABLE);
	USB_Init();
	usart1_transmit_string("USB 2.0 FS(Full speed) Initialize.\r\n");
#endif

	usart1_transmit_string("\r\nservice_usb_start()\r\n");		
	usart1_transmit_string("Start USB mouse control.\r\n");
	usart1_transmit_string("Press 'L', 'R', 'U', 'D' Key for control PC mouse cursor.\r\n");

	s_current_service = usbStartService;
}

void service_usb_stop(void)
{
	//usart1_transmit_string("\r\nservice_usb_stop()\r\n");	
	usart1_transmit_string("Stop USB mouse control.\r\n");

	s_current_service = usbStopService;
}





u8 Touch_Key_Pro(u8 type)
{
	u16 tempx=0,tempy=0;
	u8 ml=0,mr=0,mu=0,md=0;
	u8 first=1;	  		 	    					   		 
	do
	{	    
		Pen_Point.Key_Sta=Key_Up;
		if(lcd_touch_read_once()/*Read_Ads7846()*/)
		{	 
			if(first)
			{
				Pen_Point.X0=Pen_Point.X;Pen_Point.Y0=Pen_Point.Y;
				tempx=Pen_Point.X;tempy=Pen_Point.Y;  
				
				first=0;
			}else if(type==2)
			{	  
				if(tempx>Pen_Point.X)ml++;
				else mr++;	   	
				if(tempy>Pen_Point.Y)mu++;
				else md++;
				if(abs(tempx-Pen_Point.X)>700||abs(tempy-Pen_Point.Y)>700)
				{
					ml=mr=mu=md=0;
					tempx=Pen_Point.X=Pen_Point.X0;
					tempy=Pen_Point.Y=Pen_Point.Y0;
					break;
				}   
				tempx=Pen_Point.X;tempy=Pen_Point.Y;
			}
			else if(type==1)break; 
			
		}
		delay_ms(10);
	} while(NPEN||Pen_Point.Key_Sta==Key_Down);

	delay_ms(10);
	Pen_Point.Key_Sta=Key_Up;
 	
	if(abs(tempx-Pen_Point.X0)<=200&&abs(tempy-Pen_Point.Y0)<=200||type<2)	
	{	 
		if(Pen_Point.xfac!=0)
		{
			
			lcd43_fill_rectangle(410,160,479,230,YELLOW);
			lcd43_show_4num(410,160,Pen_Point.X0,16,1);
			lcd43_show_4num(410,190,Pen_Point.Y0,16,1);
			
			Pen_Point.X0=Pen_Point.xfac*Pen_Point.X0+Pen_Point.xoff;
			Pen_Point.Y0=Pen_Point.yfac*Pen_Point.Y0+Pen_Point.yoff;
			

		}
		Pen_Point.X=Pen_Point.X0;Pen_Point.Y=Pen_Point.Y0;
		return CLICK_POINT;
	}	 
	
	if(abs(tempx-Pen_Point.X0)<=500&&abs(tempy-Pen_Point.Y0)<=500)
		return 0;
	else if(Pen_Point.xfac!=0)
	{
		Pen_Point.X0=Pen_Point.xfac*Pen_Point.X0+Pen_Point.xoff;
		Pen_Point.Y0=Pen_Point.yfac*Pen_Point.Y0+Pen_Point.yoff;

		tempx=Pen_Point.xfac*tempx+Pen_Point.xoff;
		tempy=Pen_Point.yfac*tempy+Pen_Point.yoff;
	}
		
	if(abs(ml-mr)>abs(mu-md))
	{
		if(abs(tempx-Pen_Point.X0)>abs(tempy-Pen_Point.Y0))
		{
			if(tempx>Pen_Point.X0)
				return MOVE_RIGHT;
			else 
				return MOVE_LEFT; 
		}else
		{
			if(tempy>Pen_Point.Y0)return MOVE_DOWN;
			else return MOVE_UP;
		}
	}else
	{
		if(abs(tempy-Pen_Point.Y0)>abs(tempx-Pen_Point.X0))
		{	    
			if(tempy>Pen_Point.Y0)return MOVE_DOWN;
			else return MOVE_UP;			 
		}
		else
		{	  
			if(tempx>Pen_Point.X0)return MOVE_RIGHT;
			else return MOVE_LEFT;
		}
	}   	  
}


u8 Is_In_Area(u16 x1,u16 y1,u16 x2,u16 y2)
{
	if(Pen_Point.X<=x2&&Pen_Point.X>=x1&&Pen_Point.Y<=y2&&Pen_Point.Y>=y1)return 1;
	else return 0;
}  

u8 Touch_To_Num(u8 M_Type)
{
	u16 xtemp;
	u16 t,ytemp;
	switch(M_Type)
	{
		case 0:
			xtemp=Touch_Key_Pro(1);
			if(!(xtemp&CLICK_POINT))
				return 0;
			ytemp=10;
			for(t=0;t<9;t++)
			{
				xtemp=100*(t%5)+10;
				//if(t>5)ytemp=216;
				if(t>4)ytemp=153;
				if(Is_In_Area(xtemp,ytemp,xtemp+59,ytemp+90))
				{
				   Pen_Point.X=0;
	               Pen_Point.Y=0;
				//   delay_ms(100);
				   return t+1;

				}
			}
			break;
		case 1:
			xtemp=Touch_Key_Pro(2);
			if(xtemp&CLICK_POINT)
			{
				for(t=0;t<8;t++)
				{	  
					if(Is_In_Area(0,42+t*32,239,42+(t+1)*32))return t+1;
				}
				if(Is_In_Area(0,300,40,319))return 9;
				if(Is_In_Area(200,300,239,319))return 10;
				return 0;
			}
			if(xtemp)
				return xtemp&0xf0;
			break; 
		case 2:
			xtemp=Touch_Key_Pro(1);
			if(xtemp&CLICK_POINT)
			{
				if(Is_In_Area(4,142,28,166))return 1;	    
				else if(Is_In_Area(60,142,84,166))return 2;
				else if(Is_In_Area(96,142,120,166))return 3;
				else if(Is_In_Area(136,142,160,166))return 4;
				else if(Is_In_Area(172,142,196,166))return 5;
				else if(Is_In_Area(212,142,236,166))return 6;

				else if(Is_In_Area(4,206,28,230))return 7;	    
				else if(Is_In_Area(60,206,84,230))return 8;
				else if(Is_In_Area(96,206,120,230))return 9;
				else if(Is_In_Area(136,206,160,230))return 10;
				else if(Is_In_Area(172,206,196,230))return 11;
				else if(Is_In_Area(212,206,236,230))return 12;
															   
				else if(Is_In_Area(0,300,40,319))return 13;
				else if(Is_In_Area(200,300,239,319))return 14;
				return 0;
			}
			break;
		case 3:
			xtemp=Touch_Key_Pro(1);
			if(xtemp&CLICK_POINT)
			{
				if(Is_In_Area(214,72,240,96))return 1;
				else if(Is_In_Area(87,103,153,119))return 2;
				else if(Is_In_Area(8,223,56,247))return 3;
				else if(Is_In_Area(64,223,112,247))return 4;
				else if(Is_In_Area(120,223,168,247))return 5;
				else if(Is_In_Area(178,224,204,276))return 6;
				else if(Is_In_Area(214,224,238,248))return 7;
				else if(Is_In_Area(214,252,238,276))return 8;
					
				else if(Is_In_Area(28,288,52,312))return 9;
				else if(Is_In_Area(108,288,132,312))return 10;
				else if(Is_In_Area(188,288,212,312))return 11;

				else if(Is_In_Area(30,83,210,98))return 12;
															   
				return 0;
			}
		case 4:
			xtemp=Touch_Key_Pro(1);
			if(xtemp&CLICK_POINT)
			{
				for(t=0;t<8;t++)
				{	  
					if(Is_In_Area(0,42+t*32,239,42+(t+1)*32))return t+1;
				}
				if(Is_In_Area(0,300,40,319))return 9;
				if(Is_In_Area(200,300,239,319))return 10;
				return 0;
			}										    
			break;
		case 5:
			xtemp=Touch_Key_Pro(1);
			if(xtemp&CLICK_POINT)
			{
				if(Is_In_Area(37,215,61,239))return 1;
				else if(Is_In_Area(93,215,117,239))return 2;
				else if(Is_In_Area(152,218,203,236))return 3;
					
				else if(Is_In_Area(28,261,52,285))return 4;
				else if(Is_In_Area(108,261,132,285))return 5;
				else if(Is_In_Area(188,261,212,285))return 6;
																		   
				else if(Is_In_Area(0,300,40,319))return 7;
				else if(Is_In_Area(200,300,239,319))return 8;
				return 0;
			}  
			break; 
		case 6:
			xtemp=Touch_Key_Pro(1);
			if(xtemp&CLICK_POINT)
			{
				if(Is_In_Area(111,151,153,169))return 1;
				return 0;
			}  
			break; 	  	    		 
	}
	return 0;
}

void service_lcd28_start(void)
{
	//usart1_transmit_string("\r\nservice_lcd28_start()\r\n");
	
	s_current_service = lcd28Service;	

	curr_tft_lcd = tft28_lcd;	

	bsp_lcd28_init();

	POINT_COLOR=RED;	  
	
	lcd28_show_string(20,50, "--- STM32 ---");	
	lcd28_show_string(20,70, "2.8' TFT LCD TEST");	
	lcd28_show_string(20,90, "Written by JK Electronics");
	lcd28_show_string(20,110, "http://www.deviceshop.net");
	lcd28_show_string(20,130, "Date : 2012/9/5");



}
void service_lcd28_touch_start(void)
{
	//usart1_transmit_string("\r\nservice_lcd28_touch_start()\r\n");
	
	s_current_service = lcd28TouchService;	

	curr_tft_lcd = tft28_lcd;	

	bsp_key_gpio_init();
	bsp_key_interrupt_init();

	register_exti_key_function(extiKey5ServiceFunction, key5_interrupt_event);

	bsp_lcd28_init();
	bsp_touch_init_interrupt();
	lcd28_touch_adjust();

	lcd28_clear_screen(WHITE);

	while(!(s_key & KEY_KEY5))
	{
		if(Pen_Point.Key_Sta==Key_Down)
		{
			lcd_touch_interrupt_set(0);
			do
			{
				Pen_Point.Key_Sta=Key_Up;

				if( convert2pos() == 1 )
				{
					POINT_COLOR=RED;	  
					lcd28_draw_point_big(Pen_Point.X0,Pen_Point.Y0);
				}
				
			} while(PEN==0);
			
			lcd_touch_interrupt_set(1);
		}
		else 
			delay_ms(1);
	}

	service_exit();	
}

void service_lcd32_start(void)
{
	//usart1_transmit_string("\r\nservice_lcd32_start()\r\n");
	
	s_current_service = lcd32Service;	

	curr_tft_lcd = tft32_lcd;	

	bsp_lcd32_init();


	POINT_COLOR=RED;	  
	

	lcd32_show_string(20,50, "--- STM32 ---");	
	lcd32_show_string(20,70, "3.2' TFT LCD TEST");	
	lcd32_show_string(20,90, "Written by JK Electronics");
	lcd32_show_string(20,110, "http://www.deviceshop.net");
	lcd32_show_string(20,130, "Date : 2012/9/5");

}
void service_lcd32_touch_start(void)
{
	//usart1_transmit_string("\r\nservice_lcd32_touch_start()\r\n");
	
	s_current_service = lcd32TouchService;	

	curr_tft_lcd = tft32_lcd;	

	bsp_key_gpio_init();
	bsp_key_interrupt_init();

	register_exti_key_function(extiKey5ServiceFunction, key5_interrupt_event);

	bsp_lcd32_init();
	bsp_touch_init_interrupt();
	lcd32_touch_adjust();

	lcd32_clear_screen(WHITE);

	while(!(s_key & KEY_KEY5))
	{
		if(Pen_Point.Key_Sta==Key_Down)
		{
			lcd_touch_interrupt_set(0);
			do
			{
				Pen_Point.Key_Sta=Key_Up;

				if( convert2pos() == 1 )
				{
					POINT_COLOR=RED;	  
					lcd32_draw_point_big(Pen_Point.X0,Pen_Point.Y0);
				}
				
			} while(PEN==0);
			
			lcd_touch_interrupt_set(1);
		}
		else 
			delay_ms(1);
	}

	service_exit();	
}

void service_lcd43_start(void)
{
	u32 index=0;
	u8 t;

	//usart1_transmit_string("\r\nservice_lcd43_start()\r\n");
	
	s_current_service = lcd43Service;	
	curr_tft_lcd = tft43_lcd;

	s_key = KEY_NONE;	

	bsp_key_gpio_init();
	bsp_key_interrupt_init();

	register_exti_key_function(extiKey5ServiceFunction, key5_interrupt_event);

	bsp_lcd43_init();
	bsp_touch_init_interrupt();
	//lcd43_touch_adjust();
	lcd43_touch_adjust_auto();

	lcd43_draw_demo_icon_all();
	index=0;
	lcd43_demo(0,0,1);
	
	Pen_Point.Key_Sta=Key_Up;
	t=0;

	
	while(!(s_key & KEY_KEY5))
	{					  
	
		if( (Pen_Point.Key_Sta==Key_Down||NPEN) && Pen_Point.Key_LSta )
			t=Touch_To_Num(0);

		if(PEN)
		{
			Pen_Point.Key_LSta=1;	 
			Pen_Point.Key_Sta=Key_Up;
		}    						   
		if(t)
		{ 
			Pen_Point.Key_LSta=0;	   
			lcd43_demo(0,t-1,1);
			if(index!=t-1)
				lcd43_demo(0,index,0);

				index=t-1; 									   
				t=0;
		}

		delay_ms(1);
	}

	service_exit();
}
void service_lcd43_touch_start(void)
{
	u32 color_box_width_x=30;
	u32 color_box_width_y=30;


	s_current_service = lcd43TouchService;	
	curr_tft_lcd = tft43_lcd;

	s_key = KEY_NONE;	


	bsp_key_gpio_init();
	bsp_key_interrupt_init();

	register_exti_key_function(extiKey5ServiceFunction, key5_interrupt_event);

	bsp_lcd43_init();
	bsp_touch_init_interrupt();
	lcd43_touch_adjust();

#if 1

	lcd43_clear_screen(WHITE);
	
	lcd43_fill_color(0,0,color_box_width_x*1,color_box_width_y,RED); 					
	lcd43_fill_color(color_box_width_x*1,0,color_box_width_x*2,color_box_width_y,BLUE); 
	lcd43_fill_color(color_box_width_x*2,0,color_box_width_x*3,color_box_width_y,GREEN); 
	lcd43_fill_color(color_box_width_x*3,0,color_box_width_x*4,color_box_width_y,GRED); 
	lcd43_fill_color(color_box_width_x*4,0,color_box_width_x*5,color_box_width_y,BRED); 
	lcd43_fill_color(color_box_width_x*5,0,color_box_width_x*6,color_box_width_y,BLACK); 	 
	lcd43_show_string(color_box_width_x*7,20,"CLR", RED);


	while(!(s_key & KEY_KEY5))
	{
		if(Pen_Point.Key_Sta==Key_Down)
		{
			lcd_touch_interrupt_set(0);
			do
			{
				Pen_Point.Key_Sta=Key_Up;

				if( convert2pos() == 1 )
				{

					if(Pen_Point.X0<=4096&&Pen_Point.Y0<=4096)
					{	
						lcd43_show_num(color_box_width_x*6+3,7,Pen_Point.X0, RED);
						lcd43_show_num(color_box_width_x*6+3,22,Pen_Point.Y0, RED);
				
						if(Pen_Point.X0>color_box_width_x*7 && Pen_Point.Y0<50)
						{
							//usart1_transmit_string("lcd70_clear_area\r\n");
							lcd43_clear_screen(WHITE);
							
							lcd43_fill_color(0,0,color_box_width_x*1,color_box_width_y,RED); 					
							lcd43_fill_color(color_box_width_x*1,0,color_box_width_x*2,color_box_width_y,BLUE); 
							lcd43_fill_color(color_box_width_x*2,0,color_box_width_x*3,color_box_width_y,GREEN); 
							lcd43_fill_color(color_box_width_x*3,0,color_box_width_x*4,color_box_width_y,GRED); 
							lcd43_fill_color(color_box_width_x*4,0,color_box_width_x*5,color_box_width_y,BRED); 
							lcd43_fill_color(color_box_width_x*5,0,color_box_width_x*6,color_box_width_y,BLACK); 	 
							lcd43_show_string(color_box_width_x*7,18,"CLR", RED);
						}
						else if(Pen_Point.X0>0 && Pen_Point.Y0<50)
						{
							//usart1_transmit_string("ggg\r\n");
						    if(Pen_Point.X0<=color_box_width_x*1) 
								demo_selected_pen_color=RED;
							else if(Pen_Point.X0>color_box_width_x*1 && Pen_Point.X0<=color_box_width_x*2) 
								demo_selected_pen_color=BLUE;
							else if(Pen_Point.X0>color_box_width_x*2 && Pen_Point.X0<=color_box_width_x*3) 
								demo_selected_pen_color=GREEN;
							else if(Pen_Point.X0>color_box_width_x*3 && Pen_Point.X0<=color_box_width_x*4) 
								demo_selected_pen_color=GRED;
							else if(Pen_Point.X0>color_box_width_x*4 && Pen_Point.X0<=color_box_width_x*5) 
								demo_selected_pen_color=BRED;	
							else if(Pen_Point.X0>color_box_width_x*5 && Pen_Point.X0<=color_box_width_x*6) 
								demo_selected_pen_color=BLACK;
						}
						else if(Pen_Point.Y0>60)
						{
							//usart1_transmit_string("draw\r\n");
							lcd43_draw_point_big(Pen_Point.X0,Pen_Point.Y0,demo_selected_pen_color);
						}
					}
				}

				// lcd43_draw_point_big(Pen_Point.X0,Pen_Point.Y0,RED);
			} while(PEN==0);
			
			lcd_touch_interrupt_set(1);
		}
		else 
			delay_ms(1);
	}
#endif

	service_exit();
}


void service_lcd70_start(void)
{
	
	u32 index=0;
	u8 t;
	
	s_current_service = lcd70Service;
	curr_tft_lcd = tft70_lcd;

	s_key = KEY_NONE;	

	bsp_key_gpio_init();
	bsp_key_interrupt_init();

	register_exti_key_function(extiKey5ServiceFunction, key5_interrupt_event);

	bsp_lcd70_init();
	bsp_touch_init_interrupt();
	lcd70_touch_adjust();
	// lcd70_touch_adjust_auto();



	lcd70_draw_demo_icon_all();
	index=0;
	lcd70_demo(0,0,1);
	
	Pen_Point.Key_Sta=Key_Up;
	t=0;

	
	while(!(s_key & KEY_KEY5))
	{					  
	
		if( (Pen_Point.Key_Sta==Key_Down||NPEN) && Pen_Point.Key_LSta )
			t=Touch_To_Num(0);

		if(PEN)
		{
			Pen_Point.Key_LSta=1;	 
			Pen_Point.Key_Sta=Key_Up;
		}    						   
		if(t)
		{ 
			Pen_Point.Key_LSta=0;	   
			lcd70_demo(0,t-1,1);

			if(index!=t-1)
				lcd70_demo(0,index,0);

				index=t-1; 									   
				t=0;
		}

		  	delay_ms(1);
	}		

	service_exit();
}

void service_lcd70_touch_start(void)
{
	u32 color_box_width_x=50;
	u32 color_box_width_y=50;

	s_current_service = lcd70TouchService;
	curr_tft_lcd = tft70_lcd;	



	bsp_key_gpio_init();
	bsp_key_interrupt_init();

	register_exti_key_function(extiKey5ServiceFunction, key5_interrupt_event);

	bsp_lcd70_init();
	bsp_touch_init_interrupt();
	lcd70_touch_adjust();

#if 1

	lcd70_clear_screen(WHITE);
	
	lcd70_fill_color(0,0,color_box_width_x*1,color_box_width_y,RED); 					
	lcd70_fill_color(color_box_width_x*1,0,color_box_width_x*2,color_box_width_y,BLUE); 
	lcd70_fill_color(color_box_width_x*2,0,color_box_width_x*3,color_box_width_y,GREEN); 
	lcd70_fill_color(color_box_width_x*3,0,color_box_width_x*4,color_box_width_y,GRED); 
	lcd70_fill_color(color_box_width_x*4,0,color_box_width_x*5,color_box_width_y,BRED); 
	lcd70_fill_color(color_box_width_x*5,0,color_box_width_x*6,color_box_width_y,BLACK); 	 
	lcd70_show_string(color_box_width_x*7,20,"CLR", RED);


	while(!(s_key & KEY_KEY5))
	{
		if(Pen_Point.Key_Sta==Key_Down)
		{
			lcd_touch_interrupt_set(0);
			do
			{
				Pen_Point.Key_Sta=Key_Up;

				if( convert2pos() == 1 )
				{

					if(Pen_Point.X0<=4096&&Pen_Point.Y0<=4096)
					{	
						lcd70_show_num(color_box_width_x*6+3,7,Pen_Point.X0, RED);
						lcd70_show_num(color_box_width_x*6+3,22,Pen_Point.Y0, RED);
				
						if(Pen_Point.X0>color_box_width_x*7 && Pen_Point.Y0<50)
						{
							//usart1_transmit_string("lcd70_clear_area\r\n");
							lcd70_clear_screen(WHITE);
							
							lcd70_fill_color(0,0,color_box_width_x*1,color_box_width_y,RED); 					
							lcd70_fill_color(color_box_width_x*1,0,color_box_width_x*2,color_box_width_y,BLUE); 
							lcd70_fill_color(color_box_width_x*2,0,color_box_width_x*3,color_box_width_y,GREEN); 
							lcd70_fill_color(color_box_width_x*3,0,color_box_width_x*4,color_box_width_y,GRED); 
							lcd70_fill_color(color_box_width_x*4,0,color_box_width_x*5,color_box_width_y,BRED); 
							lcd70_fill_color(color_box_width_x*5,0,color_box_width_x*6,color_box_width_y,BLACK); 	 
							lcd70_show_string(color_box_width_x*7,18,"CLR", RED);
						}
						else if(Pen_Point.X0>0 && Pen_Point.Y0<50)
						{
							//usart1_transmit_string("ggg\r\n");
						    if(Pen_Point.X0<=color_box_width_x*1) 
								demo_selected_pen_color=RED;
							else if(Pen_Point.X0>color_box_width_x*1 && Pen_Point.X0<=color_box_width_x*2) 
								demo_selected_pen_color=BLUE;
							else if(Pen_Point.X0>color_box_width_x*2 && Pen_Point.X0<=color_box_width_x*3) 
								demo_selected_pen_color=GREEN;
							else if(Pen_Point.X0>color_box_width_x*3 && Pen_Point.X0<=color_box_width_x*4) 
								demo_selected_pen_color=GRED;
							else if(Pen_Point.X0>color_box_width_x*4 && Pen_Point.X0<=color_box_width_x*5) 
								demo_selected_pen_color=BRED;	
							else if(Pen_Point.X0>color_box_width_x*5 && Pen_Point.X0<=color_box_width_x*6) 
								demo_selected_pen_color=BLACK;
						}
						else if(Pen_Point.Y0>60)
						{
							//usart1_transmit_string("draw\r\n");
							lcd70_draw_point_big(Pen_Point.X0,Pen_Point.Y0,demo_selected_pen_color);
						}
					}
				}

				// lcd70_draw_point_big(Pen_Point.X0,Pen_Point.Y0,RED);
			} while(PEN==0);
			
			lcd_touch_interrupt_set(1);
		}
		else 
			delay_ms(1);
	}
#endif


	service_exit();			  	

	 
}

void service_eeprom_write(void)
{
	//usart1_transmit_string("\r\nservice_eeprom_write()\r\n");
	
	s_current_service = eepromWriteService;

	AT24CXX_Init();

	AT24CXX_WriteOneByte (0x01, 0x12); // byte write
	delay_ms (10);
	AT24CXX_WriteOneByte (0x02, 0x13); // byte write
	delay_ms (10);
	AT24CXX_WriteOneByte (0x03, 0x14); // byte write
	delay_ms (10);

	usart1_transmit_string_format("\r\nwritten data 0x%x, 0x%x, 0x%x.\r\n", 0x12, 0x13, 0x14);	
}

void service_eeprom_read(void)
{
	//usart1_transmit_string("\r\nservice_eeprom_read()\r\n");

	s_current_service = eepromReadService;

	AT24CXX_Init();

	usart1_transmit_string_format("0x%x, 0x%x, 0x%x readed.\r\n", AT24CXX_ReadOneByte (0x1), AT24CXX_ReadOneByte (0x2), AT24CXX_ReadOneByte (0x3));
}


void service_key_polling_start(void)
{
	usart1_transmit_string("\r\nservice_key_polling_start()\r\n");
	usart1_transmit_string("Press Key1 ~ 4.\r\n");
	usart1_transmit_string("\r\nservice_key_interrupt_start()\r\n");
	usart1_transmit_string("Press Key5.\r\n");

	s_current_service = keyStartPollingService;

	s_key = KEY_NONE;	

	bsp_key_gpio_init();
	bsp_key_interrupt_init();
	register_exti_key_function(extiKey5ServiceFunction, key5_interrupt_event);
}

void service_key_polling_stop(void)
{
	//usart1_transmit_string("\r\nservice_key_polling_stop()\r\n");

	s_current_service = keyStopPollingService;
}



void gpio_polling_key(void)
{

	if( (!GPIO_ReadInputDataBit(KEY1_USER_PORT, KEY1_USER_PIN )) && !(s_key & KEY_KEY1) )
	{
		usart1_transmit_string("Key1 Pressed.\r\n");
		s_key |= KEY_KEY1;
	}
	else if( (GPIO_ReadInputDataBit(KEY1_USER_PORT, KEY1_USER_PIN )) && (s_key & KEY_KEY1) )
	{
		usart1_transmit_string("Key1 Released.\r\n");
		s_key &= ~KEY_KEY1;
	}

	if( (!GPIO_ReadInputDataBit(KEY2_USER_PORT, KEY2_USER_PIN )) && !(s_key & KEY_KEY2) )
	{
		usart1_transmit_string("Key2 Pressed.\r\n");
		s_key |= KEY_KEY2;
	}
	else if( (GPIO_ReadInputDataBit(KEY2_USER_PORT, KEY2_USER_PIN )) && (s_key & KEY_KEY2) )
	{
		usart1_transmit_string("Key2 Released.\r\n");
		s_key &= ~KEY_KEY2;
	}

	if( (!GPIO_ReadInputDataBit(KEY3_USER_PORT, KEY3_USER_PIN )) && !(s_key & KEY_KEY3) )
	{
		usart1_transmit_string("Key3 Pressed.\r\n");
		s_key |= KEY_KEY3;
#ifdef LCD43_MP3_VOLUME_UP_DOWN_TEST
		vs1003b_volume_up = 1;
#endif
	}
	else if( (GPIO_ReadInputDataBit(KEY3_USER_PORT, KEY3_USER_PIN )) && (s_key & KEY_KEY3) )
	{
		usart1_transmit_string("Key3 Released.\r\n");
		s_key &= ~KEY_KEY3;
	}

	if( (!GPIO_ReadInputDataBit(KEY4_USER_PORT, KEY4_USER_PIN )) && !(s_key & KEY_KEY4) )
	{
		usart1_transmit_string("Key4 Pressed.\r\n");
		s_key |= KEY_KEY4;
#ifdef LCD43_MP3_VOLUME_UP_DOWN_TEST
		vs1003b_volume_down = 1;
#endif

	}
	else if( (GPIO_ReadInputDataBit(KEY4_USER_PORT, KEY4_USER_PIN )) && (s_key & KEY_KEY4) )
	{
		usart1_transmit_string("Key4 Released.\r\n");
		s_key &= ~KEY_KEY4;
	}

}

void key5_interrupt_event(void)
{	
	wait_10ms(1);

	// usart1_transmit_string("key5_interrupt_event\r\n");
	
	if( (!GPIO_ReadInputDataBit(KEY5_USER_PORT, KEY5_USER_PIN )) && !(s_key & KEY_KEY5) )
	{
		usart1_transmit_string("Key5 Pressed.\r\n");
		s_key |= KEY_KEY5;
	}
	else if( (GPIO_ReadInputDataBit(KEY5_USER_PORT, KEY5_USER_PIN )) && (s_key & KEY_KEY5) )
	{
		usart1_transmit_string("Key5 Released.\r\n");
		s_key &= ~KEY_KEY5;
	}
}


void service_rf_start_rx(void)
{
	u8 recv_buf[33];
	
	//usart1_transmit_string("\r\nservice_rf_start_rx()\r\n");

	s_current_service = rfStartRxService;

	bsp_key_gpio_init();
	bsp_key_interrupt_init();

	register_exti_key_function(extiKey5ServiceFunction, key5_interrupt_event);

	bsp_nrf24l01_init();

	delay_ms(10);

	if( nRF24L01_Check() == 1 )
	{
		usart1_transmit_string("\r\nnRF24L01 detect fail.\r\n");
	}
	else
	{
		nRF24L01_RX_Mode();
		usart1_transmit_string("\r\nWait RF Data. Exit KEY5.\r\n");

		s_key = KEY_NONE;

		while(!(s_key & KEY_KEY5))
		{		
			if(nRF24L01_RxPacket(recv_buf) == 0)
			{
				recv_buf[32] = 0x00;
				usart1_transmit_string_format("\r\nReceived data = %s\r\n", recv_buf);
			}
			else
			{
				delay_us(100);
			}
		}
	}
}

void service_rf_start_tx(void)
{
#if 1
	u8 send_buf[33] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 0x00};
	
	//usart1_transmit_string("\r\nservice_rf_start_tx()\r\n");

	s_current_service = rfStartTxService;

	bsp_nrf24l01_init();	

	delay_ms(10);	

	if( nRF24L01_Check() == 1 )
	{
		usart1_transmit_string("\r\nnRF24L01 detect fail.\r\n");
	}
	else
	{
		nRF24L01_TX_Mode();
		usart1_transmit_string("\r\nSend RF Data.\r\n");

		if(nRF24L01_TxPacket(send_buf)==TX_OK)
		{
			usart1_transmit_string_format("\r\nSent data = %s\r\n", send_buf);
		}
		else
		{
			usart1_transmit_string("\r\nFail Send data\r\n");
		}
	}	
#endif	

}

#ifdef NRF24L01_BPS_CHECK
void service_rf_bps_start_rx(void)
{
	u8 recv_buf[33];
	
	//usart1_transmit_string("\r\nservice_rf_bps_start_rx()\r\n");

	s_current_service = rfStartRxBpsService;

	bsp_key_interrupt_init();

	register_exti_key_function(extiKey1ServiceFunction, key1_interrupt_event);	
#ifdef STM32_MIDDLE_HW_VER1	
	register_exti_key_function(extiKey2ServiceFunction, key2_interrupt_event);
#endif

	bsp_nrf24l01_init();

	delay_ms(10);

	if( nRF24L01_Check() == 1 )
	{
		usart1_transmit_string("\r\nnRF24L01 detect fail.\r\n");
	}
	else
	{
		nRF24L01_RX_Mode();
		// usart1_transmit_string("\r\nWait RF Data. Exit KEY1.\r\n");

		s_key = KEY_NONE;

		while(!(s_key & KEY_KEY1))
		{		
			if(nRF24L01_RxPacket(recv_buf) == 0)
			{
				recv_buf[0] = 0x00;
				// usart1_transmit_string_format("\r\nReceived data = %s\r\n", recv_buf);
			}
			else
			{
				delay_us(10);
			}

		}
	}
}

void service_rf_bps_start_tx(void)
{

	u8 i;
	u16 send_count = 0;
	u8 send_buf[33] = {0x00};
	
	//usart1_transmit_string("\r\nservice_rf_bps_start_tx()\r\n");

	s_current_service = rfStartTxBpsService;

	bsp_nrf24l01_init();	

	delay_ms(10);	

	if( nRF24L01_Check() == 1 )
	{
		usart1_transmit_string("\r\nnRF24L01 detect fail.\r\n");
	}
	else
	{
		for(i=0;i<32;i++)
			send_buf[i] = i;
		
		nRF24L01_TX_Mode();		

		s_rf_bps_check_cnt = 0;

		while(s_rf_bps_check_cnt < 1000 )
		{
			if(nRF24L01_TxPacket(send_buf) != TX_OK)
			{
				usart1_transmit_string("\r\nFail Send data\r\n");
			}
			else
			{
				send_count++;
			}
		}

		usart1_transmit_string_format("\r\nSend Speed = %dbps\r\n", 32*send_count*8);
	}	
	
}
#endif

void service_rf_stop(void)
{
	//usart1_transmit_string("\r\nservice_rf_stop()\r\n");

	s_current_service = rfStopService;
}


void service_mp3play_start(void)
{	

	u16 read_count;
  	unsigned int count=0;
	int j;
	UINT br;

	s_current_service = mp3StartPlayService;
	
	//usart1_transmit_string("\r\nservice_mp3play_start()\r\n");



	bsp_sd_gpio_init();

	if( disk_initialize(0) == 0 )
		usart1_transmit_string("\r\nSD initialize success.\r\n");
	else
		usart1_transmit_string("\r\nSD initialize failed.\r\n");  

	if(Vs1003_Init())
		usart1_transmit_string("\r\nvs1003b initialize failed..\r\n");
	else
		usart1_transmit_string("\r\nvs1003b initialize successed.\r\n");		


	// Already initialize SPI2 by disk_initialize()
	//  
#if 1
	VS1003B_Fill2048Zero();
#endif
	Vs1003SoftReset();	
	delay_ms(20);

	f_mount(0,&fs);

	if (f_opendir (&dirs, path) == FR_OK) 
  	{
  		//usart0_puts ("\r\n3");
      	while (f_readdir (&dirs, &finfo) == FR_OK)  
    	{
    			//usart0_puts ("\r\n4");
      		if (finfo.fattrib & AM_ARC) 
      		{      			
      			//usart0_puts ("\r\n5");
    			if (!finfo.fname[0])	
      				break;

				usart1_transmit_string_format ("\r\n file name is: \n %s \n", finfo.fname);

				res = f_open (&fsrc, finfo.fname, FA_OPEN_EXISTING | FA_READ); 

				usart1_transmit_string ("\r\n Playing¡¦");
				read_count = 0;


				for (; ;) 
				{
					// delay_ms(1);
					
					res = f_read (&fsrc, buffer, sizeof(buffer), &br);
					read_count++;
					// usart1_transmit_string_format ("\r\n read_count=%d\r\n", read_count);
					
					if(res==FR_OK) 
					{
						count=0;
						// delay_ms(1);
						
						while(count<BUFF_SIZE) 
						{					    		
					    	if(VS1003B_NeedData())
							{							
								//Vs1003_DATA_Write(buffer);	
								//count = count + 32;
								
								
								for (j=0; j<32; j++) // each time delivers 32 data
								{
									//if(VS1003B_NeedData())
									{
										MP3_DCS_SET(0); 										
										bsp_readwritebyte_spi2(buffer[count]);
										MP3_DCS_SET(1); 
										
										count++;
									}
								}
								
								
								// usart1_transmit_string_format ("\r\n count=%d\r\n", count);
								
							}
							
							
				    			if (res || br == 0) 
								break;   // error or eof	
						}
					
						// usart0_puts ((char*)buffer);

					}					

					if (res || br == 0) 
						break;   // error or eof		   
				}

				
				f_close(&fsrc);
      		}                      
      	}
	}

}



void service_exit(void)
{
	s_menu_level = MainService;	
}

void service_mp3play_stop(void)
{
	//usart1_transmit_string("\r\nservice_mp3play_stop()\r\n");

	s_current_service = mp3StopPlayService;
}

void service_mp3play_exit(void)
{
	s_menu_level = MainService;
	//usart1_transmit_string("\r\nservice_mp3play_exit()\r\n");
}

void service_uart2_start(void)
{
	//usart1_transmit_string("\r\nservice_uart2_start()\r\n");

	s_current_service = uart2StartService;

	bsp_init_irq_usart2();

	usart2_transmit_string("\r\nUart2 echo service started.\r\n");
}

void usart3_event(char data)
{	 
	char rs485_data[8];
	char	address;
	
	if( s_menu_level == Rs485Service && s_current_service == rs485MasterStartService )
	{
		usart1_transmit_string_format("\r\n 485 mastr data in : 0x%x", data);
	}		
	else if( s_menu_level == Rs485Service && s_current_service == rs485SlaveStartService )
	{
		usart1_transmit_string_format("\r\n 485 slave data in : 0x%x", data);		
		rs485_data_count++;

		if( rs485_data_count == 7 )
		{
			rs485_data_count = 0;
			
			address = 0x61;
			rs485_data[0] = address;
			rs485_data[1] = 0x68;
			rs485_data[2] = 0x69;
			rs485_data[3] = 0x70;
			rs485_data[4] = 0x71;
			rs485_data[5] = 0x72;
			rs485_data[6] = 0x73;
			rs485_data[7] = 0x00;	

			usart1_transmit_string_format("\r\nsend data 0x%x, 0x%x, 0x%x, 0x%x, 0x%x, 0x%x, 0x%x \r\n", rs485_data[0], rs485_data[1], rs485_data[2], rs485_data[3], rs485_data[4], rs485_data[5], rs485_data[6] );
			
			rs485_puts(rs485_data);
		}
	}
}

void service_rs485_master_start(void)
{
	char rs485_data[8];
	char	address;

	//usart1_transmit_string("\r\nservice_rs485_master_start()\r\n");
	
	register_usart_function(usart3ServiceFunction, usart3_event);

	s_current_service = rs485MasterStartService;	

	bsp_rs485_gpio_init();
	bsp_rs485_init();

	address = 0x61;
	rs485_data[0] = address;
	rs485_data[1] = 0x62;
	rs485_data[2] = 0x63;
	rs485_data[3] = 0x64;
	rs485_data[4] = 0x65;
	rs485_data[5] = 0x66;
	rs485_data[6] = 0x67;
	rs485_data[7] = 0x00;	

	usart1_transmit_string_format("\r\nsend data 0x%x, 0x%x, 0x%x, 0x%x, 0x%x, 0x%x, 0x%x \r\n", rs485_data[0], rs485_data[1], rs485_data[2], rs485_data[3], rs485_data[4], rs485_data[5], rs485_data[6] );
	
	rs485_puts(rs485_data);


	delay_ms(2);
}

void service_rs485_slave_start(void)
{
	//usart1_transmit_string("\r\nservice_rs485_slave_start()\r\n");

	register_usart_function(usart3ServiceFunction, usart3_event);

	s_current_service = rs485SlaveStartService;

	bsp_rs485_gpio_init();
	bsp_rs485_init();

	rs485_data_count = 0;
}

void service_can_loopback_start(void)
{
	int TestRx;

	//usart1_transmit_string("\r\nservice_can_loopback_start()\r\n");

	s_current_service = canLoopbackStartService;
	
	bsp_can_gpio_init();
	bsp_can_interrupt_init();

    /* CAN transmit at 100Kb/s and receive by polling in loopback mode */
    TestRx = can_polling();

    if (TestRx == DISABLE)
    {
		usart1_transmit_string("\r\nCAN-Bus by polling in loopback mode is False \r\n");
    }
    else
    {
		usart1_transmit_string("\r\nCAN-Bus by polling in loopback mode is OK \r\n");
    }	

    /* CAN transmit at 500Kb/s and receive by interrupt in loopback mode */
    TestRx = can_interrupt();

    if (TestRx == DISABLE)
    {
		usart1_transmit_string("\r\nCAN-Bus by interrupt in loopback mode is False \r\n");
    }
    else
    {
		usart1_transmit_string("\r\nCAN-Bus by interrupt in loopback mode is OK \r\n");
    }
}

void service_can_normal_start(void)
{
	CanTxMsg TxMessage;

	//usart1_transmit_string("\r\nservice_can_normal_start()\r\n");
	usart1_transmit_string("\r\nIf you want this function then press KEY5.\r\n");

	s_current_service = canNormalStartService;

	s_key = KEY_NONE;	

	bsp_key_gpio_init();
	bsp_key_interrupt_init();
	register_exti_key_function(extiKey5ServiceFunction, key5_interrupt_event);

	

	bsp_can_init();

	CanFlag = DISABLE;
	
	CAN_DATA0=0x01;
	CAN_DATA1=0x02;
	CAN_DATA2=0x03;  
	CAN_DATA3=0x04;
	CAN_DATA4=0x05;  
	CAN_DATA5=0x06;
	CAN_DATA6=0x07;  
	CAN_DATA7=0x08;
	
	/* transmit */
	TxMessage.StdId = 0xA5A5;  /* Éèset the standard id standard of care id up to 7 can not be recessive (1). 11 */
	//TxMessage.ExtId = 0x00;     //Éèset extensions id extension id 18
	TxMessage.RTR = CAN_RTR_DATA; /* set to the data frame */
	TxMessage.IDE = CAN_ID_STD;   /* use standard id */
	TxMessage.DLC = 8;            /*  data length, can message provides maximum data length is 8 bytes */
	TxMessage.Data[0] = CAN_DATA0;    
	TxMessage.Data[1] = CAN_DATA1;    
	TxMessage.Data[2] = CAN_DATA2;    
	TxMessage.Data[3] = CAN_DATA3;    
	TxMessage.Data[4] = CAN_DATA4;    
	TxMessage.Data[5] = CAN_DATA5;     
	TxMessage.Data[6] = CAN_DATA6;    
	TxMessage.Data[7] = CAN_DATA7;      
	CAN_Transmit(CAN1,&TxMessage);  /* returns this information request sent Mailbox No. 0,1,2 or E-mail application to send no_box */	

	usart1_transmit_string_format("\r\nTransmit CAN Data.\r\n");
	usart1_transmit_string_format("CAN ID %x \r\n",CAN_ID);
	usart1_transmit_string_format("CAN_DATA0 %x \r\n",CAN_DATA0);
	usart1_transmit_string_format("CAN_DATA1 %x \r\n",CAN_DATA1);
	usart1_transmit_string_format("CAN_DATA2 %x \r\n",CAN_DATA2);
	usart1_transmit_string_format("CAN_DATA3 %x \r\n",CAN_DATA3);
	usart1_transmit_string_format("CAN_DATA4 %x \r\n",CAN_DATA4);
	usart1_transmit_string_format("CAN_DATA5 %x \r\n",CAN_DATA5);
	usart1_transmit_string_format("CAN_DATA6 %x \r\n",CAN_DATA6);
	usart1_transmit_string_format("CAN_DATA7 %x \r\n",CAN_DATA7);


	delay_ms(10);
	delay_ms(10);
	delay_ms(10);
	delay_ms(10);
	delay_ms(10);

	do 
	{
		if( CanFlag == ENABLE )
		{
			CanFlag = DISABLE;
			usart1_transmit_string_format("\r\n\r\nCAN Receive Data \r\n");
			usart1_transmit_string_format("CAN ID %x \r\n",CAN_ID);
			usart1_transmit_string_format("CAN_DATA0 %x \r\n",CAN_DATA0);
			usart1_transmit_string_format("CAN_DATA1 %x \r\n",CAN_DATA1);
			usart1_transmit_string_format("CAN_DATA2 %x \r\n",CAN_DATA2);
			usart1_transmit_string_format("CAN_DATA3 %x \r\n",CAN_DATA3);
			usart1_transmit_string_format("CAN_DATA4 %x \r\n",CAN_DATA4);
			usart1_transmit_string_format("CAN_DATA5 %x \r\n",CAN_DATA5);
			usart1_transmit_string_format("CAN_DATA6 %x \r\n",CAN_DATA6);
			usart1_transmit_string_format("CAN_DATA7 %x \r\n",CAN_DATA7);
		}

		delay_ms(10);

	} while(!(s_key & KEY_KEY5));
}

void service_adc12_15_start(void)
{
	u16 AD_value;

	//usart1_transmit_string("\r\nservice_adc12_15_start()\r\n");
	usart1_transmit_string("\r\nIf you want this function then press KEY5.\r\n");

	s_current_service = adc12_15Service;

	s_key = KEY_NONE;	

	bsp_key_gpio_init();
	bsp_key_interrupt_init();
	register_exti_key_function(extiKey5ServiceFunction, key5_interrupt_event);

	bsp_adc_init(ADC_Channel_15);

	delay_ms(10);

	do 
	{
		delay_ms(500);

		if(ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC)==SET)
		{
			AD_value = ADC_GetConversionValue(ADC1);

			/* Printf message with AD value to serial port every 1 second */
			usart1_transmit_string_format("The current AD Channel15 value = %d \r\n", AD_value);
		}      

		delay_ms(500);

	} while(!(s_key & KEY_KEY5));
}

void service_adc12_9_start(void)
{
	float AD_value;

	//usart1_transmit_string("\r\nservice_adc12_9_start()\r\n");
	usart1_transmit_string("\r\nIf you want this function then press KEY5.\r\n");

	s_current_service = adc12_9Service;

	s_key = KEY_NONE;	

	bsp_key_gpio_init();
	bsp_key_interrupt_init();
	register_exti_key_function(extiKey5ServiceFunction, key5_interrupt_event);

	bsp_adc_init(ADC_Channel_9);

	delay_ms(10);

	do 
	{
		delay_ms(500);

		if(ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC)==SET)
		{
			AD_value = ADC_GetConversionValue(ADC1);

			/* Printf message with AD value to serial port every 1 second */
			usart1_transmit_string_format("The current AD Channel9 value = %4.2fV \r\n", (AD_value/4096)*3.3);
		}      

		delay_ms(500);

	} while(!(s_key & KEY_KEY5));
}

void service_adc12_8_start(void)
{
	float AD_value;

	//usart1_transmit_string("\r\nservice_adc12_8_start()\r\n");
	usart1_transmit_string("\r\nIf you want this function then press KEY5.\r\n");

	s_current_service = adc12_8Service;

	s_key = KEY_NONE;	

	bsp_key_gpio_init();
	bsp_key_interrupt_init();
	register_exti_key_function(extiKey5ServiceFunction, key5_interrupt_event);

	bsp_adc_init(ADC_Channel_8);

	delay_ms(10);

	do 
	{
		delay_ms(500);

		if(ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC)==SET)
		{
			AD_value = ADC_GetConversionValue(ADC1);

			/* Printf message with AD value to serial port every 1 second */
			usart1_transmit_string_format("The current AD Channel8 value = %4.2fV \r\n", (AD_value/4096)*3.3);
		}      

		delay_ms(500);

	} while(!(s_key & KEY_KEY5));
}

void service_dac1_noise_start(void)
{
	//usart1_transmit_string("\r\nservice_dac1_noise_start()\r\n");
	usart1_transmit_string("\r\nNot implemented.\r\n");
	//usart1_transmit_string("\r\nIf you want this function then press KEY5.\r\n");

	s_current_service = dac1_noiseService;

#if 0	

	s_key = KEY_NONE;	

	bsp_key_gpio_init();
	bsp_key_interrupt_init();
	register_exti_key_function(extiKey5ServiceFunction, key5_interrupt_event);

	bsp_dac_init();

	delay_ms(10);

	do 
	{
	} while(!(s_key & KEY_KEY5));

	/* Enable DMA2 Channel3 */
	DMA_Cmd(DMA2_Channel3, DISABLE);
	
	/* Enable DAC Channel1 */
	DAC_Cmd(DAC_Channel_1, DISABLE);
	
	/* Enable DMA for DAC Channel1 */
	DAC_DMACmd(DAC_Channel_1, DISABLE);
	
	/* TIM6 enable counter */
	TIM_Cmd(TIM6, DISABLE);
#endif

}

void service_buzzer_start(void)
{
	//usart1_transmit_string("\r\nservice_buzzer_start()\r\n");	

	s_current_service = buzzerStartService;

	bsp_buzzer_init();
	buzzer_on();
}

void service_buzzer_stop(void)
{
	//usart1_transmit_string("\r\nservice_buzzer_stop()\r\n");	

	s_current_service = buzzerStopService;

	bsp_buzzer_init();
	buzzer_off();
}

void service_lcd1602_start(void)
{
	//usart1_transmit_string("\r\nservice_lcd1602_start()\r\n");	

	s_current_service = lcd1602StartService;

	bsp_lcd1602_init();

	usart1_transmit_string("\r\nlcd1602 initialized.\r\n");

	LCD1602_Char_Set(1, 1, "JK Electronics");
	LCD1602_Char_Set(2, 0, "www.jkelec.co.kr");

}

void service_lcd12864_start(void)
{
	//usart1_transmit_string("\r\nservice_lcd12864_start()\r\n");	

	s_current_service = lcd12864StartService;

	bsp_lcd12864_init();

	usart1_transmit_string("\r\nlcd12864 initialized.\r\n");

	LCD12864_Char_Set(0,0, "JK Electronics");
	LCD12864_Char_Set(1,0, "LCD12864 Test.");
	LCD12864_Char_Set(2,0, "www.jkelec.co.kr");
	LCD12864_Char_Set(3,0, "Date:2012.09.05");

}


/*******************************************************************************
* Function Name  : tim_display
* Description    : Displays the current time.
* Input          : - TimeVar: RTC counter value.
* Output         : None
* Return         : None
*******************************************************************************/
void tim_display(u32 TimeVar)
{ 
	u32 THH = 0, TMM = 0, TSS = 0;

	/* Compute  hours */
	THH = TimeVar/3600;
	/* Compute minutes */
	TMM = (TimeVar % 3600)/60;
	/* Compute seconds */
	TSS = (TimeVar % 3600)% 60;

	usart1_transmit_string_format("\r\nTime: %0.2d:%0.2d:%0.2d",THH, TMM, TSS);
}

/*******************************************************************************
* Function Name  : Time_Show
* Description    : Shows the current time (HH:MM:SS) on the Hyperterminal.
* Input          : None
* Output         : None
* Return         : None
******************************************************************************/
void time_show(void)
{
	// usart1_transmit_string("\n\r");
	tim_display(RTC_GetCounter());
}

void usb_mouse_control(s8 Keys)
{
	u8 mouse_buffer[4] = {0, 0, 0, 0};
	s8 x = 0, y = 0;

	switch (Keys)
	{
		case 'l' :
		case 'L':
			x += 10;
			break;

		case 'r':
		case 'R':
			x -= 10;
			break;
		case 'u':
		case 'U':
			y -= 10;
			break;
		case 'd':
		case 'D':
			y += 10;
			break;
		default:
			return;
	}

	/* prepare buffer to send */
	mouse_buffer[0] = 0x00;
	mouse_buffer[1] = x;
	mouse_buffer[2] = y;

#if 0
	/*copy mouse position info in ENDP1 Tx Packet Memory Area*/
	UserToPMABufferCopy(mouse_buffer, GetEPTxAddr(ENDP1), 4);
	if(mouse_buffer[0]!= 0)
	{
		mouse_buffer[0] = 0;
		UserToPMABufferCopy(mouse_buffer, GetEPTxAddr(ENDP1), 4);
	} 
	
	/* enable endpoint for transmission */
	SetEPTxValid(ENDP1);
#endif

	/* Copy mouse position info in ENDP1 Tx Packet Memory Area*/
	USB_SIL_Write(EP1_IN, mouse_buffer, 4);

#ifndef STM32F10X_CL
	/* Enable endpoint for transmission */
	SetEPTxValid(ENDP1);
#endif /* STM32F10X_CL */

}



		


#ifdef  USE_FULL_ASSERT
/*******************************************************************************
* Function Name  : assert_failed
* Description    : Reports the name of the source file and the source line number
*                  where the assert_param error has occurred.
* Input          : - file: pointer to the source file name
*                  - line: assert_param error line source number
* Output         : None
* Return         : None
*******************************************************************************/
void assert_failed(uint8_t* file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {}
}
#endif



/******************* (C) COPYRIGHT 2010 STMicroelectronics *****END OF FILE****/
