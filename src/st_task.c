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

extern __FLASH ST_TASK_t st_task_list[];

#if ( ST_EVENT_MAX == 32 )
uint32_t st_event_list[ ST_TASK_MAX ];
#elif ( ST_EVENT_MAX == 16 )
uint16_t st_event_list[ ST_TASK_MAX ];
#elif ( ST_EVENT_MAX == 8 )
uint8_t  st_event_list[ ST_TASK_MAX ];
#else
#error "ST_EVENT_MAX must be 8, 16 or 32"
#endif


extern void    st_task_init   ( void )
{
    st_memset( st_event_list, 0, sizeof(st_event_list) );
}

extern void st_task_set_event   ( uint8_t task_id, uint8_t event_id )
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
    
    ST_ASSERT( task_id < ST_TASK_MAX );
    ST_ASSERT( event_id < ST_EVENT_MAX );
    ST_ASSERT( st_task_list[task_id] != NULL );
    
    event = ( 1 << event_id );
    
    ST_ENTER_CRITICAL();
    st_event_list[task_id] |= event;
    ST_EXIT_CRITICAL();
}

extern void st_task_clr_event   ( uint8_t task_id, uint8_t event_id )
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
    
    ST_ASSERT( task_id < ST_TASK_MAX);
    ST_ASSERT( event_id < ST_EVENT_MAX );
    ST_ASSERT( st_task_list[task_id] != NULL );
    
    event = ( 1 << event_id );
    event = ~event;
    
    ST_ENTER_CRITICAL();
    st_event_list[task_id] &= event;
    ST_EXIT_CRITICAL();
    
}




















