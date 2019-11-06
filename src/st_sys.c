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
extern const ST_TASK_t *st_task_list;
extern ST_TCB_t *st_task_tcb;
extern const st_uint8_t st_task_max;

/* Private define ------------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
static st_uint32_t st_task_event;
static st_uint8_t st_task_id;
static st_int8_t st_event_id;

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

/* Exported function implementations -----------------------------------------*/
st_uint8_t st_get_task_id_self( void )
{
    return st_task_id;
}

int main( void )
{
    /* Disable Interrupts */
    ST_ENTER_CRITICAL();

    /* Power on reset hook */
    st_board_init();
    
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

    for( st_task_id = 0; st_task_id < st_task_max; st_task_id++ )
    {
        if(st_task_list[st_task_id].p_task_init)
            st_task_list[st_task_id].p_task_init( st_task_id );
    }
    
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
        
        for( st_task_id = 0; st_task_id < st_task_max; st_task_id++ )
        {
#ifdef ST_MSG_EN
            if( st_task_tcb[st_task_id].phead )
            {
                st_task_list[st_task_id].p_task_handler( ST_TASK_EVT_MSG );
                break;
            }
#endif
            
            ST_ENTER_CRITICAL();
            st_task_event = st_task_tcb[st_task_id].event;
            ST_EXIT_CRITICAL();
            
            if( st_task_event )
            {
                for( st_event_id = 0; st_event_id < 32; st_event_id++ )
                {
                    if( st_task_event & BV( st_event_id ) )
                    {
                        break;
                    }
                }

                st_task_event = BV( st_event_id );
                
                ST_ENTER_CRITICAL();
                if( st_task_tcb[st_task_id].event & st_task_event )
                {
                    st_task_tcb[st_task_id].event &= ~st_task_event;
                    ST_EXIT_CRITICAL();
                    if( st_task_list[st_task_id].p_task_handler )
                        st_task_list[st_task_id].p_task_handler( st_event_id );
                }
                else
                {
                    ST_EXIT_CRITICAL();
                }
                break;
            }
        }

        if( st_task_id == st_task_max )
        {
            st_board_idle();
        }
    }
    //return 0;
}

/* Private function implementations ------------------------------------------*/

/****** (C) COPYRIGHT 2019 Single-Thread Development Team. *****END OF FILE****/
