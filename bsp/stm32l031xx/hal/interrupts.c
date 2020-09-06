/******************************************************************************

 @file  

 @brief 

 Group: 
 Target Device: 

 ******************************************************************************
 

 ******************************************************************************
 Release Name: 
 Release Date: 2016-06-09 06:57:09
 *****************************************************************************/
#include "M051Series.h"
#include "spl_config.h"
#include "osal.h"

#if (SPL_UART_EN > 0)
#if (SPL_UART0_EN > 0)
extern void spl_uart0_isr( void );
extern void UART0_IRQHandler(void)  {spl_uart0_isr();}
#endif
#if (SPL_UART1_EN > 0)
extern void spl_uart1_isr( void );
extern void UART1_IRQHandler(void)  {spl_uart1_isr();}
#endif
#endif

#if (SPL_I2C_EN > 0)
#if (SPL_I2C0_SLAVE_EN > 0)
extern void spl_i2c0_isr( void );
extern void I2C0_IRQHandler(void)   {spl_i2c0_isr();}
#endif
#if (SPL_I2C1_SLAVE_EN > 0)
extern void spl_i2c1_isr( void );
extern void I2C1_IRQHandler(void)   {spl_i2c1_isr();}
#endif
#endif

#if (SPL_EXTINT_EN > 0)
#if (SPL_EXTINT0_EN > 0)
extern void spl_extint_int0_isr( void );
void EINT0_IRQHandler(void)         {spl_extint_int0_isr();}
#endif
#if (SPL_EXTINT1_EN > 0)
extern void spl_extint_int1_isr( void );
void EINT1_IRQHandler(void)         {spl_extint_int1_isr();}
#endif
#endif

#if (SPL_TIMER_EN > 0)
#if (SPL_TIMER_SYSTICK_EN > 0)
extern void SPL_TIMER_SYSTICK_CALLBACK( void );
extern void TMR0_IRQHandler(void)
{
    if(TIMER_GetIntFlag(TIMER0) == 1)
    {
        /* Clear Timer0 time-out interrupt flag */
        TIMER_ClearIntFlag(TIMER0);
		SPL_TIMER_SYSTICK_CALLBACK();
    }
}
#endif//(SPL_TIMER_SYSTICK_EN > 0)
#endif//(SPL_TIMER_EN > 0)


