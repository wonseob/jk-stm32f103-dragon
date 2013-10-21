
#ifndef  RTC_PRESENT
#define  RTC_PRESENT

typedef enum
{
	rtcServiceFunction,
	rtcServiceFunctionMAX
} rtc_register_function_type;

typedef            void     (*rtc_register_function)(void);

typedef struct _rtc_service_function_type
{
	rtc_register_function_type service_type;
	rtc_register_function run;
} rtc_service_function_type;

/* ------------------------------------------------------------------------------------------------- */
/* BSP RTC */
/* ------------------------------------------------------------------------------------------------- */
extern void register_rtc_function(rtc_register_function_type timer_fn_type, rtc_register_function fn);
extern void bsp_init_rtc(void);
extern void rtc_configuration(void);
extern void time_adjust(void);
extern u32 time_regulate(void);

#endif
