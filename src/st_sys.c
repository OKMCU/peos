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
#if ( ST_EVENT_MAX == 32 )
st_uint32_t st_task_event_list[ ST_TASK_MAX ];
#elif ( ST_EVENT_MAX == 16 )
st_uint16_t st_task_event_list[ ST_TASK_MAX ];
#elif ( ST_EVENT_MAX == 8 )
st_uint8_t  st_task_event_list[ ST_TASK_MAX ];
#else
#error "ST_EVENT_MAX must be 8, 16 or 32"
#endif

/* Private define ------------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
#if (ST_EVENT_MAX == 32)
static st_uint32_t event;
#elif (ST_EVENT_MAX == 16)
static st_uint16_t event;
#elif (ST_EVENT_MAX == 8)
static st_uint8_t  event;
#else
#error "ST_EVENT_MAX must be 8, 16 or 32"
#endif
static st_uint8_t event_id;
static st_uint8_t task_id;
extern __FLASH ST_TASK_t st_task_list[ST_TASK_MAX];

/* Private function prototypes -----------------------------------------------*/
static void st_sys_init ( void );

/* Exported function implementations -----------------------------------------*/
st_uint8_t st_get_task_id_self( void )
{
    return task_id;
}

st_uint8_t st_get_task_id_by_handler( void (*p_task_handler)( st_uint8_t event_id ) )
{
    st_uint8_t i;

    ST_ASSERT( p_task_handler != NULL );
    
    for( i = 0; i < ST_TASK_MAX; i++ )
    {
        if( p_task_handler == st_task_list[i].p_task_handler )
            break;
    }

    return i;
}


#ifdef ST_TASK_NAME_EN
st_uint8_t st_get_task_id_by_name( const char *p_name )
{
    st_uint8_t i;

    ST_ASSERT( p_name != NULL );
    
    for( i = 0; i < ST_TASK_MAX; i++ )
    {
        if( st_strcmp(p_name, st_task_list[i].p_task_name) == 0 )
            break;
    }

    return i;
}
#endif

int main( void )
{
    /* Disable Interrupts */
    ST_ENTER_CRITICAL();
    
    /* Initialize mcu and peripherals */
    st_hal_init();
    
    /* Initialize the OS's vars */
    st_sys_init();

    /* Enable Interrupts */
    ST_EXIT_CRITICAL();
    
    /* Start OSAL */
    for(;;)
    {
#ifdef ST_CLOCK_EN
#ifdef ST_TIMER_EN
        st_timer_process( st_clock_update() );
#else
        st_clock_update();
#endif // (ST_TIMER_EN > 0)
#endif // (ST_CLOCK_EN > 0)
        
        for( task_id = 0; task_id < ST_TASK_MAX; task_id++ )
        {
            ST_ENTER_CRITICAL();
            event = st_task_event_list[task_id];
            ST_EXIT_CRITICAL();
            
            if( event )
            {
                for( event_id = 0; event_id < ST_EVENT_MAX; event_id++ )
                {
                    if( event & BV( event_id ) )
                    {
                        break;
                    }
                }

                event = BV( event_id );
                
                ST_ENTER_CRITICAL();
                if( st_task_event_list[task_id] & event )
                {
                    st_task_event_list[task_id] &= ~event;
                    ST_EXIT_CRITICAL();
                    ST_ASSERT( st_task_list[task_id].p_task_handler != NULL );
                    st_task_list[task_id].p_task_handler( event_id );
                }
                else
                {
                    ST_EXIT_CRITICAL();
                }
                break;
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
static void st_sys_init ( void )
{
    st_uint8_t i;
    
    st_memset( st_task_event_list, 0, sizeof(st_task_event_list) );
    
#ifdef ST_MEM_EN
    st_mem_init();
#endif /* (ST_MEM_EN > 0) */

#ifdef ST_CLOCK_EN
    st_clock_init();
#endif

#ifdef ST_TIMER_EN
    st_timer_init();
#endif /* (ST_TIMER_EN > 0) */

#ifdef ST_MSG_EN
    st_msg_init();
#endif /* ( ST_MSG_EN > 0 ) */

    for( i = 0; i < ST_TASK_MAX; i++ )
    {
        if( st_task_list[i].p_task_init != NULL )
            st_task_list[i].p_task_init();
    }
}

/****** (C) COPYRIGHT 2019 Single-Thread Development Team. *****END OF FILE****/
