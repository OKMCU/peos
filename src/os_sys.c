/*******************************************************************************
 * Copyright (c) 2019-2020, Single-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 * 
 * Change Logs:
 * Date         Author       Notes
 * 2019-10-28   Wentao SUN   first version
 * 2019-10-29   Wentao SUN   double check event flag before entering task
 *
 ******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "os.h"

/* Exported variables --------------------------------------------------------*/
extern const OS_TASK_t *os_task_list;
extern OS_TCB_t *os_task_tcb;
extern const os_uint8_t os_task_max;

/* Private define ------------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
static os_event_t os_task_event;
static os_uint8_t os_task_id;
static os_int8_t os_event_id;

/* Private function prototypes -----------------------------------------------*/
#ifdef OS_MEM_EN
#define __os_mem_init()     umm_init()
#endif
#ifdef OS_CLOCK_EN
extern void __os_clock_init( void );
extern os_uint8_t __os_clock_update( void );
#endif
#ifdef OS_TIMER_EN
extern void __os_timer_init( void );
extern void __os_timer_process( os_uint8_t delta_systick );
#endif

/* Exported function implementations -----------------------------------------*/
os_uint8_t os_get_task_id_self( void )
{
    return os_task_id;
}

int main( void )
{
    /* Disable Interrupts */
    OS_ENTER_CRITICAL();

    /* Power on reset hook */
    os_board_init();
    
#ifdef OS_MEM_EN
    __os_mem_init();
#endif /* (OS_MEM_EN > 0) */

#ifdef OS_CLOCK_EN
    __os_clock_init();
#endif

#ifdef OS_TIMER_EN
    __os_timer_init();
#endif /* (OS_TIMER_EN > 0) */

    /* Enable Interrupts */
    OS_EXIT_CRITICAL();

    for( os_task_id = 0; os_task_id < os_task_max; os_task_id++ )
    {
        if(os_task_list[os_task_id].p_task_init)
            os_task_list[os_task_id].p_task_init( os_task_id );
    }
    
    /* Start Single-Thread task scheduler */
    for(;;)
    {
#ifdef OS_CLOCK_EN
#ifdef OS_TIMER_EN
        __os_timer_process( __os_clock_update() );
#else
        __os_clock_update();
#endif // (OS_TIMER_EN > 0)
#endif // (OS_CLOCK_EN > 0)
        
        for( os_task_id = 0; os_task_id < os_task_max; os_task_id++ )
        {
#ifdef OS_MSG_EN
            if( os_task_tcb[os_task_id].phead )
            {
                os_task_list[os_task_id].p_task_handler( OS_TASK_EVT_MSG );
                break;
            }
#endif
            
            OS_ENTER_CRITICAL();
            os_task_event = os_task_tcb[os_task_id].event;
            OS_EXIT_CRITICAL();
            
            if( os_task_event )
            {
                os_event_id = 0;
                while( (os_task_event & BV(os_event_id)) == 0 )
                {
                    os_event_id++;
                }
                os_task_event = BV( os_event_id );
                
                OS_ENTER_CRITICAL();
                if( os_task_tcb[os_task_id].event & os_task_event )
                {
                    os_task_tcb[os_task_id].event &= ~os_task_event;
                    OS_EXIT_CRITICAL();
                    OS_ASSERT( os_task_list[os_task_id].p_task_handler != NULL );
                    os_task_list[os_task_id].p_task_handler( os_event_id );
                }
                else
                {
                    OS_EXIT_CRITICAL();
                }
                break;
            }
        }

        if( os_task_id == os_task_max )
        {
            os_board_idle();
        }
    }
    //return 0;
}

/* Private function implementations ------------------------------------------*/

/****** (C) COPYRIGHT 2019 Single-Thread Development Team. *****END OF FILE****/

