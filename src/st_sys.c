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
#include "st.h"

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

#if ( ST_EVENT_MAX == 32 )
st_uint32_t st_task_event_list[ ST_TASK_MAX ];
#elif ( ST_EVENT_MAX == 16 )
st_uint16_t st_task_event_list[ ST_TASK_MAX ];
#elif ( ST_EVENT_MAX == 8 )
st_uint8_t  st_task_event_list[ ST_TASK_MAX ];
#else
#error "ST_EVENT_MAX must be 8, 16 or 32"
#endif

static void st_sys_init ( void )
{
    st_uint8_t i;
    
    st_memset( st_task_event_list, 0, sizeof(st_task_event_list) );
    
#if (ST_MEM_EN > 0)
    st_mem_init();
#endif /* (ST_MEM_EN > 0) */

#if (ST_TIMER_EN > 0)
    st_timer_init();
#endif /* (ST_TIMER_EN > 0) */

#if ( ST_MSG_EN > 0 )
    st_msg_init();
#endif /* ( ST_MSG_EN > 0 ) */

    for( i = 0; i < ST_TASK_MAX; i++ )
    {
        if( st_task_list[i].p_task_init != NULL )
            st_task_list[i].p_task_init();
    }
}

st_uint8_t rt_task_id_get( void )
{
    return task_id;
}

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
#if (ST_TIMER_EN > 0)
        st_timer_update();
#endif /* (ST_TIMER_EN > 0) */
        
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
                event = ~event;
                
                ST_ENTER_CRITICAL();
                st_task_event_list[task_id] &= event;
                ST_EXIT_CRITICAL();
                
                ST_ASSERT( st_task_list[task_id].p_task_handler != NULL );
                st_task_list[task_id].p_task_handler( event_id );
                
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


















