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
static uint32_t event;
#elif (ST_EVENT_MAX == 16)
static uint16_t event;
#elif (ST_EVENT_MAX == 8)
static uint8_t  event;
#else
#error "ST_EVENT_MAX must be 8, 16 or 32"
#endif
static uint8_t event_id;
static uint8_t index;

extern __FLASH ST_TASK_t st_task_list[ST_TASK_MAX];

#if ( ST_EVENT_MAX == 32 )
uint32_t st_task_event_list[ ST_TASK_MAX ];
#elif ( ST_EVENT_MAX == 16 )
uint16_t st_task_event_list[ ST_TASK_MAX ];
#elif ( ST_EVENT_MAX == 8 )
uint8_t  st_task_event_list[ ST_TASK_MAX ];
#else
#error "ST_EVENT_MAX must be 8, 16 or 32"
#endif

static void st_sys_init ( void )
{
    memset( st_task_event_list, 0, sizeof(st_task_event_list) );
    
#if (ST_MEM_EN > 0)
    st_mem_init();
#endif /* (ST_MEM_EN > 0) */

#if (ST_TIMER_EN > 0)
    st_timer_init();
#endif /* (ST_TIMER_EN > 0) */

#if ( ST_MSG_EN > 0 )
    st_msg_init();
#endif /* ( ST_MSG_EN > 0 ) */
}

int main( void )
{
    /* Disable Interrupts */
    ST_ENTER_CRITICAL();
    
    /* Initialize the OS's vars */
    st_sys_init();
    
    /* Initialize mcu and peripherals */
    st_hardware_init();
    
    /* Enable Interrupts */
    ST_EXIT_CRITICAL();
    
    /* Start OSAL */
    for(;;)
    {
#if (ST_TIMER_EN > 0)
        st_timer_update();
#endif /* (ST_TIMER_EN > 0) */
        
        for( index = 0; index < ST_TASK_MAX; index++ )
        {
            ST_ENTER_CRITICAL();
            event = st_task_event_list[index];
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
                st_task_event_list[index] &= event;
                ST_EXIT_CRITICAL();
                
                st_task_list[index]( index, event_id );
                break;
            }
        }
    }

    //return 0;

}

/* integrity check of type sizes */
ST_ASSERT_SIZE(  int8_t, 1);
ST_ASSERT_SIZE( uint8_t, 1);
ST_ASSERT_SIZE( int16_t, 2);
ST_ASSERT_SIZE(uint16_t, 2);
ST_ASSERT_SIZE( int32_t, 4);
ST_ASSERT_SIZE(uint32_t, 4);


















