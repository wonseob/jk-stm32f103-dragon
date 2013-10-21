/******************** (C) COPYRIGHT 2009 STMicroelectronics ********************
* File Name          : main.h
* Author             : MCD Application Team
* Version            : V2.0.0
* Date               : 04/27/2009
* Description        : Header for main.c module
********************************************************************************
* THE PRESENT SOFTWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH SOFTWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_it.h"
#include "hw_config.h"


/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void bsp_init_rcc(void);
void bsp_init_gpio(void);
void bsp_init_interrupt(void);
ErrorStatus Get_HSEStartUpStatus(void);


typedef enum
{
	MainService,
	Ledservice,
	TimerService,
	RtcService,
	SdService,	
	UsbService,
	LcdService,
	EepromService,
	KeyService,	
	RfService,
	Mp3Service,
	Uart2Service,
	Rs485Service,
	CanService,
	AdcService,	
	DacService,	
	BuzzerService,	
	Lcd1602Service,	
	Lcd12864Service,	
	serviceMAX
} service_define_type;


typedef enum
{
	ledUserOnService,
	ledUserOffService,
	ledExitService,
	ledServiceMAX
} service_led_type;

typedef enum
{
	timerTIM2Service,
	timerTIM2Stop,		
	timerExitService,
	timerServiceMAX
} service_timer_type;

typedef enum
{
	rtcStartService,
	rtcStopService,
	rtcExitService,	
	rtcServiceMAX
} service_rtc_type;

typedef enum
{
	sdFileListService,
	sdExitService,	
	sdServiceMAX
} service_sd_type;

typedef enum
{
	usbStartService,
	usbStopService,
	usbExitService,	
	usbServiceMAX
} service_usb_type;

typedef enum
{
	lcd28Service,
	lcd28TouchService,
	lcd32Service,
	lcd32TouchService,
	lcd43Service,
	lcd43TouchService,
	lcd70Service,
	lcd70TouchService,
	lcdExitService,
	lcdServiceMAX
} service_lcd_type;

typedef enum
{
	eepromWriteService,
	eepromReadService,
	eepromExitService,	
	eepromServiceMAX
} service_Eeprom_type;

typedef enum
{
	keyStartPollingService,
	keyStopPollingService,
	keyExitService,	
	keyServiceMAX
} service_key_type;

typedef enum
{
	rfStartRxService,
	rfStartTxService,
#ifdef NRF24L01_BPS_CHECK	
	rfStartRxBpsService,
	rfStartTxBpsService,
#endif	
	rfStopService,
	rfExitService,	
	rfServiceMAX
} service_rf_type;

typedef enum
{
	mp3StartPlayService,
	mp3StopPlayService,
	mp3ExitService,
	mp3ServiceMAX
} service_camera_type;

typedef enum
{
	uart2StartService,
	uart2ExitService,
	uart2ServiceMAX
} service_uart2_type;

typedef enum
{
	rs485MasterStartService,
	rs485SlaveStartService,
	rs485ExitService,
	rs485ServiceMAX
} service_rs485_type;

typedef enum
{
	canLoopbackStartService,
	canNormalStartService,
	canExitService,
	canServiceMAX
} service_can_type;

typedef enum
{
	adc12_15Service,
	adc12_9Service,
	adc12_8Service,
	adcExitService,
	adcServiceMAX
} service_adc_type;

typedef enum
{
	dac1_noiseService,
	dacExitService,
	dacServiceMAX
} service_dac_type;

typedef enum
{
	buzzerStartService,
	buzzerStopService,
	buzzerExitService,
	buzzerServiceMAX
} service_buzzer_type;

typedef enum
{
	lcd1602StartService,
	lcd1602ExitService,
	lcd1602ServiceMAX
} service_lcd1602_type;

typedef enum
{
	lcd12864StartService,
	lcd12864ExitService,
	lcd12864ServiceMAX
} service_lcd12864_type;

typedef            void     (*service_function)(void);

/*
typedef struct _service_type
{
	int service;
	char* service_string;
	service_function run;
	s8 cmd;	
} service_type;
*/

typedef struct _main_service_type
{
	 int service;
	 char* service_string;
	service_function run;
	s8 cmd;	
} main_service_type;

typedef struct _sub_service_type
{
	 int service;
	char* service_string;
	service_function run;
	s8 cmd;	
} sub_service_type;

void service_led(void);
void service_timer(void);
void service_rtc(void);
void service_sd(void);
void service_usb(void);
void service_lcd(void);
void service_eeprom(void);
void service_key(void);
void service_rf(void);
void service_mp3(void);
void service_uart2(void);
void service_rs485(void);
void service_can(void);
void service_adc(void);
void service_dac(void);
void service_buzzer(void);
void service_lcd1602(void);
void service_lcd12864(void);

void service_user_led_on(void);
void service_user_led_off(void);

void service_tim2_ticktime(void);
void service_tim2_stop(void);

void service_rtc_start(void);
void service_rtc_stop(void);

void service_sd_file_list(void);

void service_usb_start(void);
void service_usb_stop(void);

void service_lcd28_start(void);
void service_lcd28_touch_start(void);
void service_lcd32_start(void);
void service_lcd32_touch_start(void);
void service_lcd43_start(void);
void service_lcd43_touch_start(void);
void service_lcd70_start(void);
void service_lcd70_touch_start(void);

void service_eeprom_write(void);
void service_eeprom_read(void);

void service_key_polling_start(void);
void service_key_polling_stop(void);
void gpio_polling_key(void);
void key5_interrupt_event(void);

void service_rf_start_rx(void);
void service_rf_start_tx(void);
void service_rf_bps_start_rx(void);
void service_rf_bps_start_tx(void);
void service_rf_stop(void);

void service_mp3play_start(void);
void service_mp3play_stop(void);

void service_uart2_start(void);

void usart3_event(char data);
void service_rs485_master_start(void);
void service_rs485_slave_start(void);


void service_can_loopback_start(void);
void service_can_normal_start(void);


void service_adc12_15_start(void);
void service_adc12_9_start(void);
void service_adc12_8_start(void);

void service_dac1_noise_start(void);

void service_buzzer_start(void);
void service_buzzer_stop(void);

void service_lcd1602_start(void);

void service_lcd12864_start(void);

void service_exit(void);


void tim_display(u32 TimeVar);
void time_show(void);

void usb_mouse_control(s8 Keys);

#endif /* __MAIN_H */

/******************* (C) COPYRIGHT 2009 STMicroelectronics *****END OF FILE****/
