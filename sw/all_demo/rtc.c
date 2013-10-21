

#include "stm32f10x.h"
#include "hw_config.h"
#include "rtc.h"
#include "usart.h"
//#include <stm32f10x_rtc.h>
//#include <stm32f10x_bkp.h>

#define RTCClockSource_LSE   /* Use the external 32.768 KHz oscillator as RTC clock source */
//#define RTCClockOutput_Enable  /* RTC Clock/64 is output on tamper pin(PC.13) */


rtc_service_function_type gbl_ar_rtc_service[rtcServiceFunctionMAX] = 
{
	{rtcServiceFunction, NULL}
};
 
/* ------------------------------------------------------------------------------------------------- */
/* BSP rtc */
/* ------------------------------------------------------------------------------------------------- */

void register_rtc_function(rtc_register_function_type rtc_fn_type, rtc_register_function fn)
{
	gbl_ar_rtc_service[rtc_fn_type].run = fn;
}

void RTC_IRQHandler(void)
{
	// Also cleared the wrong interrupt flag in the ISR
	TIM_ClearFlag(TIM2, TIM_FLAG_Update);
	// TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET
	TIM_ClearITPendingBit(TIM2, TIM_IT_Update); // Clear the interrupt flag

	if( gbl_ar_rtc_service[rtcServiceFunction].run != NULL )
	{
		gbl_ar_rtc_service[rtcServiceFunction].run();
	}
}

void bsp_init_rtc(void)
{

	if(BKP_ReadBackupRegister(BKP_DR1) != 0xA5A5)
	{
		/* Backup data register value is not correct or not yet programmed (when
		   the first time the program is executed) */

		usart1_transmit_string("\r\n\n RTC not yet configured....");

		/* RTC Configuration */
		rtc_configuration();

		usart1_transmit_string("\r\n RTC configured....\r\n");

		/* Adjust time by values entred by the user on the hyperterminal */
		time_adjust();

		BKP_WriteBackupRegister(BKP_DR1, 0xA5A5);    
	}
	else
	{
		/* Check if the Power On Reset flag is set */
		if(RCC_GetFlagStatus(RCC_FLAG_PORRST) != RESET)
		{
			usart1_transmit_string("\r\n\n Power On Reset occurred....\r\n");
		}
		/* Check if the Pin Reset flag is set */
		else if(RCC_GetFlagStatus(RCC_FLAG_PINRST) != RESET)
		{
			usart1_transmit_string("\r\n\n External Reset occurred....");
		}

		usart1_transmit_string("\r\n No need to configure RTC....\r\n");    
		/* Wait for RTC registers synchronization */
		RTC_WaitForSynchro();

		/* Enable the RTC Second */  
		RTC_ITConfig(RTC_IT_SEC, ENABLE);
		/* Wait until last write operation on RTC registers has finished */
		RTC_WaitForLastTask();
	}

	/* Clear reset flags */
	RCC_ClearFlag();

	
}


void rtc_configuration(void)
{

	/* Enable PWR and BKP clocks */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);

	/* Allow access to BKP Domain */
	PWR_BackupAccessCmd(ENABLE);

	/* Reset Backup Domain */
	BKP_DeInit();

#ifdef RTCClockSource_LSI
	/* Enable LSI */ 
	RCC_LSICmd(ENABLE);
	/* Wait till LSI is ready */
	while(RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == RESET)
	{
	}

	/* Select LSI as RTC Clock Source */
	RCC_RTCCLKConfig(RCC_RTCCLKSource_LSI);  
#elif defined	RTCClockSource_LSE  
	/* Enable LSE */
	RCC_LSEConfig(RCC_LSE_ON);
	/* Wait till LSE is ready */
	while(RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET)
	{
	}

	/* Select LSE as RTC Clock Source */
	RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);  
#endif


	/* Enable RTC Clock */
	RCC_RTCCLKCmd(ENABLE);


#ifdef RTCClockOutput_Enable  
	/* Disable the Tamper Pin */
	BKP_TamperPinCmd(DISABLE); /* To output RTCCLK/64 on Tamper pin, the tamper
	                           functionality must be disabled */
	                           
	/* Enable RTC Clock Output on Tamper Pin */
	BKP_RTCCalibrationClockOutputCmd(ENABLE);
#endif 

	/* Wait for RTC registers synchronization */
	RTC_WaitForSynchro();

	/* Wait until last write operation on RTC registers has finished */
	RTC_WaitForLastTask();

	/* Enable the RTC Second */  
	RTC_ITConfig(RTC_IT_SEC, ENABLE);

	/* Wait until last write operation on RTC registers has finished */
	RTC_WaitForLastTask();

	/* Set RTC prescaler: set RTC period to 1sec */
#ifdef RTCClockSource_LSI
	RTC_SetPrescaler(31999); /* RTC period = RTCCLK/RTC_PR = (32.000 KHz)/(31999+1) */
#elif defined	RTCClockSource_LSE
	RTC_SetPrescaler(32767); /* RTC period = RTCCLK/RTC_PR = (32.768 KHz)/(32767+1) */
#endif

	/* Wait until last write operation on RTC registers has finished */
	RTC_WaitForLastTask();	

}

/*******************************************************************************
* Function Name  : time_adjust
* Description    : Adjusts time.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void time_adjust(void)
{
	/* Wait until last write operation on RTC registers has finished */
	RTC_WaitForLastTask(); 
	
	/* Change the current time */
	RTC_SetCounter(time_regulate());
	
	/* Wait until last write operation on RTC registers has finished */
	RTC_WaitForLastTask();   
}

/*******************************************************************************
* Function Name  : time_regulate
* Description    : Returns the time entered by user, using Hyperterminal.
* Input          : None
* Output         : None
* Return         : Current time RTC counter value
*******************************************************************************/
u32 time_regulate(void)
{
	u32 Tmp_HH = 0xa, Tmp_MM = 0xb, Tmp_SS = 0x00;

#if 0
	usart1_transmit_string("\r\n==============Time Settings=====================================");
	usart1_transmit_string("\r\n  Please Set Hours");

	while(Tmp_HH == 0xFF)
	{
		Tmp_HH = USART_Scanf(23);
	}
	
	usart1_transmit_string(":  %d", Tmp_HH); 
	usart1_transmit_string("\r\n  Please Set Minutes");
	
	while(Tmp_MM == 0xFF)
	{
		Tmp_MM = USART_Scanf(59);
	}
	
	usart1_transmit_string(":  %d", Tmp_MM); 
	usart1_transmit_string("\r\n  Please Set Seconds");
	
	while(Tmp_SS == 0xFF)
	{
		Tmp_SS = USART_Scanf(59);
	}
	
	usart1_transmit_string(":  %d", Tmp_SS); 
#endif	

	/* Return the value to store in RTC counter register */
	return((Tmp_HH*3600 + Tmp_MM*60 + Tmp_SS));
}

