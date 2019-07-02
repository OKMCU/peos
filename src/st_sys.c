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

#if (ST_TIMER_EN > 0)
extern void st_timer_update( void );
#endif /* (ST_TIMER_EN > 0) */

extern __FLASH ST_TASK_t st_task_list[];

#if ( ST_EVENT_MAX == 32 )
extern uint32_t st_event_list[];
#elif ( ST_EVENT_MAX == 16 )
extern uint16_t st_event_list[];
#elif ( ST_EVENT_MAX == 8 )
extern uint8_t  st_event_list[];
#else
#error "ST_EVENT_MAX must be 8, 16 or 32"
#endif

extern void st_sys_init ( void )
{
#if (ST_MEM_EN > 0)
    st_mem_init();
#endif /* (ST_MEM_EN > 0) */

#if (ST_TIMER_EN > 0)
    st_timer_init();
#endif /* (ST_TIMER_EN > 0) */
    st_task_init();

#if ( ST_MSG_EN > 0 )
    st_msg_init();
#endif /* ( ST_MSG_EN > 0 ) */
}

extern void st_sys_start( void )
{

#if (ST_EVENT_MAX == 32)
    uint32_t event;
#elif (ST_EVENT_MAX == 16)
    uint16_t event;
#elif (ST_EVENT_MAX == 8)
    uint8_t  event;
#else
#error "ST_EVENT_MAX must be 8, 16 or 32"
#endif

    uint8_t event_id;
    uint8_t index;
    
    for(;;)
    {
#if (ST_TIMER_EN > 0)
        st_timer_update();
#endif /* (ST_TIMER_EN > 0) */
        
        for( index = 0; index < ST_TASK_MAX; index++ )
        {
            ST_ENTER_CRITICAL();
            event = st_event_list[index];
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
                st_event_list[index] &= event;
                ST_EXIT_CRITICAL();
                
                st_task_list[index]( index, event_id );
                break;
            }
        }
    }
}

__FLASH ST_TASK_t st_task_list[ST_TASK_MAX] = {
    hal_task_driver_basic,

#if APP_CLI_EN > 0
    app_task_cli,
#else
    NULL,
#endif

#if APP_KEY_EN > 0
    app_task_key,
#else
    NULL,
#endif
    app_task_main,
};

int main( void )
{
    /* Disable Interrupts */
    ST_ENTER_CRITICAL();
    
    /* Initialize the OS's vars */
    st_system_init();
    
    /* Initialize mcu and devices */
    hal_driver_init();
    
    /* Create HAL Tasks */
    //st_task_create( hal_task_driver_basic, TASK_ID_HAL_DRIVER_BASIC );
    
    /* Create APP Tasks */
    //st_task_create( app_task_cli, TASK_ID_APP_CLI );
#if APP_CLI_EN > 0
    app_task_cli_init();
#endif

#if APP_KEY_EN > 0
    app_task_key_init();
#endif

    //st_task_create( app_task_main, TASK_ID_APP_MAIN );
    app_task_main_init();

    /* Enable Interrupts */
    ST_EXIT_CRITICAL();
    /* Start OSAL */
    st_system_start(); // No Return from here

    return 0;

}




















