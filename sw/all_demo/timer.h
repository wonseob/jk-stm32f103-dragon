
#ifndef  TIMER_PRESENT
#define  TIMER_PRESENT

typedef enum
{
	timer1ServiceFunction,
	timer2ServiceFunction,
	timerServiceFunctionMAX
} timer_register_function_type;

typedef            void     (*timer_register_function)(void);

typedef struct _timer_service_function_type
{
	timer_register_function_type service_type;
	timer_register_function run;
} timer_service_function_type;

/* ------------------------------------------------------------------------------------------------- */
/* BSP Timer */
/* ------------------------------------------------------------------------------------------------- */
extern void register_timer_function(timer_register_function_type timer_fn_type, timer_register_function fn);
extern void bsp_init_timer2(void/*isr_function timer4_isr*/);
extern u16 bsp_get_timer2_cnt(void);
extern void bsp_set_timer2_cnt( u16 cnt);

#endif
