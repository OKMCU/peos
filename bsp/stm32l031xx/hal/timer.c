/******************************************************************************

 @file  spl_mcu.c

 @brief This file contains the interface to the Drivers Service.

 Group: 
 Target Device: 

 ******************************************************************************
 

 ******************************************************************************
 Release Name: 
 Release Date: 
 *****************************************************************************/

/**************************************************************************************************
 *                                            INCLUDES
 **************************************************************************************************/
#include "M051Series.h"
#include "spl_config.h"
#include "spl_timer.h"
#include "spl_mcu.h"


#if SPL_TIMER_EN > 0
/**************************************************************************************************
 *                                         CONSTANTS
 **************************************************************************************************/

/**************************************************************************************************
 *                                      GLOBAL VARIABLES
 **************************************************************************************************/

/**************************************************************************************************
 *                                      FUNCTIONS
 **************************************************************************************************/
extern void spl_timer_init( void )
{
#if SPL_TIMER_SYSTICK_EN > 0
#if SPL_MCU_XTAL_EN > 0
    CLK_EnableModuleClock(TMR0_MODULE);
    CLK_SetModuleClock(TMR0_MODULE, CLK_CLKSEL1_TMR0_S_HXT, 0);
#else
    CLK_EnableModuleClock(TMR0_MODULE);
    CLK_SetModuleClock(TMR0_MODULE, CLK_CLKSEL1_TMR0_S_HCLK, 0);
#endif
    
    /* Start systick timer */
    TIMER_Open(TIMER0, TIMER_PERIODIC_MODE, SPL_TIMER_SYSTICK_FREQ);
    TIMER_EnableInt(TIMER0);
    NVIC_EnableIRQ(TMR0_IRQn);
    TIMER_Start(TIMER0);
#endif
}

extern void spl_timer_deinit( void )
{
#if SPL_TIMER_SYSTICK_EN > 0
    TIMER_Stop(TIMER0);
    NVIC_DisableIRQ(TMR0_IRQn);
    TIMER_DisableInt(TIMER0);
    TIMER_Close(TIMER0);

    CLK_DisableModuleClock(TMR0_MODULE);
#endif
}

#endif //SPL_TIMER_EN > 0
/**************************************************************************************************
**************************************************************************************************/

