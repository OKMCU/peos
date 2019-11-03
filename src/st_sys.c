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
#include "st.h"

/* Exported variables --------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
static st_uint32_t curr_event;
static st_uint32_t proc_event;
static st_uint32_t diff_event;
static st_uint32_t set_event;
static st_uint32_t clr_event;
static void *pmsg;
static st_uint8_t task_id;
ST_TCB_t st_task_list[ST_TASK_MAX];

/* Private function prototypes -----------------------------------------------*/
#ifdef ST_MEM_EN
#define __st_mem_init()     umm_init()
#endif
#ifdef ST_CLOCK_EN
extern void __st_clock_init( void );
extern st_uint8_t __st_clock_update( void );
#endif
#ifdef ST_TIMER_EN
extern void __st_timer_init( void );
extern void __st_timer_process( st_uint8_t delta_systick );
#endif
#ifdef ST_MSG_EN
extern void *__st_msg_recv( st_uint8_t task_id );
extern void __st_msg_delete ( void *pmsg );
#endif

/* Exported function implementations -----------------------------------------*/
st_uint8_t st_get_task_id_self( void )
{
    return task_id;
}

int main( void )
{
    /* Disable Interrupts */
    ST_ENTER_CRITICAL();
    
    /* Initialize the OS's vars */
    st_memset( st_task_list, 0, sizeof(st_task_list) );
    
#ifdef ST_MEM_EN
    __st_mem_init();
#endif /* (ST_MEM_EN > 0) */

#ifdef ST_CLOCK_EN
    __st_clock_init();
#endif

#ifdef ST_TIMER_EN
    __st_timer_init();
#endif /* (ST_TIMER_EN > 0) */

    /* Enable Interrupts */
    ST_EXIT_CRITICAL();

    /* Power on reset hook */
    st_por_hook();
    
    /* Start Single-Thread task scheduler */
    for(;;)
    {
#ifdef ST_CLOCK_EN
#ifdef ST_TIMER_EN
        __st_timer_process( __st_clock_update() );
#else
        __st_clock_update();
#endif // (ST_TIMER_EN > 0)
#endif // (ST_CLOCK_EN > 0)
        
        for( task_id = 0; task_id < ST_TASK_MAX; task_id++ )
        {
#ifdef ST_MSG_EN
            pmsg = __st_msg_recv( task_id );
            if( pmsg )
            {
                if( st_task_list[task_id].p_task_handler )
                {
                    st_task_list[task_id].p_task_handler( pmsg, 0x00000000 );
                    break;
                }
                __st_msg_delete( pmsg );
            }
#endif
            
            ST_ENTER_CRITICAL();
            curr_event = st_task_list[task_id].event;
            ST_EXIT_CRITICAL();
            
            if( curr_event )
            {
                if( st_task_list[task_id].p_task_handler )
                {
                    proc_event = st_task_list[task_id].p_task_handler( NULL, curr_event );
                    ST_ENTER_CRITICAL();
                    diff_event  =  curr_event ^ st_task_list[task_id].event;
                    set_event   =  diff_event & st_task_list[task_id].event;
                    clr_event   =  diff_event & (~st_task_list[task_id].event);
                    curr_event &= ~proc_event;
                    curr_event &= ~clr_event;
                    curr_event |=  set_event;
                    st_task_list[task_id].event = curr_event;
                    ST_EXIT_CRITICAL();
                    break;
                }
                else
                {
                    ST_ENTER_CRITICAL();
                    st_task_list[task_id].event = 0;
                    ST_EXIT_CRITICAL();
                }
            }
        }

        if( task_id == ST_TASK_MAX )
        {
            st_idle_hook();
        }
    }
    //return 0;
}

/* Private function implementations ------------------------------------------*/

/****** (C) COPYRIGHT 2019 Single-Thread Development Team. *****END OF FILE****/
