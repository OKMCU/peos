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

#if (ST_TIMER_STATIC_EN == 0)
typedef struct st_timer_t {
    struct st_timer_t *p_timer_prev;
    struct st_timer_t *p_timer_next;
    void ( *p_fxn )( void * );
    void *p_arg;
    st_timer_timeout_t timeout;
} ST_TIMER_t;
#else
typedef struct st_timer_t {
    st_timer_timeout_t timeout;
    uint8_t task_id;
    uint8_t event_id;
} ST_TIMER_t;
#endif //(ST_TIMER_STATIC_EN == 0)

#if (ST_TIMER_STATIC_EN == 0)
static ST_TIMER_t *p_timers_head;
static ST_TIMER_t *p_timers_tail;
#else
static ST_TIMER_t st_timer_list[ST_TIMER_MAX];
#endif //(ST_TIMER_STATIC_EN == 0)

static uint32_t time_sec;
static uint16_t time_ms;
static uint8_t prev_systick;
static uint8_t st_systick;

#if (ST_TIMER_STATIC_EN == 0)
#if (ST_ASSERT_EN > 0)
static ST_TIMER_t *st_timer_list_find( ST_TIMER_t *p_timer )
{
    ST_TIMER_t *p_timer_match;
    
    ST_ASSERT( p_timer != NULL );
    if( p_timers_head == NULL )
        return NULL;

    //find the timer at the tail
    p_timer_match = p_timers_head;
    while( p_timer != p_timer_match )
    {
        p_timer_match = p_timer_match->p_timer_next;
        if( p_timer_match == NULL )
            return NULL;
    }
    
    return p_timer_match;
}
#endif

static void st_timer_list_del( ST_TIMER_t *p_timer )
{
    ST_ASSERT( p_timer != NULL );
    ST_ASSERT( p_timers_head != NULL );
    
    if( p_timer->p_timer_prev == NULL &&
        p_timer->p_timer_next == NULL )
    {
        p_timers_head = NULL;
        p_timers_tail = NULL;
    }
    else
    {
        if( p_timer->p_timer_prev != NULL )
        {
            p_timer->p_timer_prev->p_timer_next = p_timer->p_timer_next;
            if( p_timer->p_timer_prev->p_timer_next == NULL )
            {
                p_timers_tail = p_timer->p_timer_prev;
            }
        }

        if( p_timer->p_timer_next != NULL )
        {
            p_timer->p_timer_next->p_timer_prev = p_timer->p_timer_prev;
            if( p_timer->p_timer_next->p_timer_prev == NULL )
            {
                p_timers_head = p_timer->p_timer_next;
            }
        }
    }
}

static void st_timer_list_add( ST_TIMER_t *p_timer_new )
{
    p_timer_new->p_timer_next = NULL;
    if( p_timers_tail )
    {
        //append the new timer to the tail
        p_timer_new->p_timer_prev = p_timers_tail;
        p_timers_tail->p_timer_next = p_timer_new;
        p_timers_tail = p_timer_new;
    }
    else
    {
        p_timer_new->p_timer_prev = NULL;
        ST_ASSERT( p_timers_head == NULL );
        p_timers_head = p_timer_new;
        p_timers_tail = p_timer_new;
    }
}

static void st_timer_event_kernel ( void *p_arg )
{
    uint16_t u16tmp;
    uint8_t task_id;
    uint8_t event_id;

    u16tmp = (uint16_t)((uint32_t)p_arg);
    task_id = HI_UINT16(u16tmp);
    event_id = LO_UINT16(u16tmp);
    st_event_set(task_id, event_id);
}

static ST_TIMER_t *st_timer_event_find( uint8_t task_id, uint8_t event_id )
{
    ST_TIMER_t *p_timer_match;
    void *p_arg;
    
    if( p_timers_head == NULL )
        return NULL;

    //find the timer at the tail
    p_timer_match = p_timers_head;
    p_arg = (void *)BUILD_UINT16( event_id, task_id );
    while( p_timer_match->p_fxn != st_timer_event_kernel || p_timer_match->p_arg != p_arg )
    {
        p_timer_match = p_timer_match->p_timer_next;
        if( p_timer_match == NULL )
            return NULL;
    }
    
    return p_timer_match;
}

static void    *st_timer_cback_create ( void ( *p_fxn )( void * ), void *p_arg, st_timer_timeout_t timeout_ms )
{
    ST_TIMER_t *p_timer_new;

    ST_ASSERT( p_fxn != NULL );
    ST_ASSERT( timeout_ms != 0 );

    p_timer_new = st_mem_alloc( sizeof(ST_TIMER_t) );
    if( p_timer_new != NULL )
    {
        p_timer_new->p_fxn = p_fxn;
        p_timer_new->p_arg = p_arg;
        p_timer_new->timeout = timeout_ms;
        st_timer_list_add( p_timer_new );
    }
    
    return p_timer_new;
}

//static void st_timer_cback_update ( void *timer_id, st_timer_timeout_t timeout_ms )
//{
//    ST_ASSERT( timer_id != NULL );
//    ST_ASSERT( timeout_ms != 0 );
//    ST_ASSERT( st_timer_list_find( (ST_TIMER_t *)timer_id ) != NULL );
//    
//    ((ST_TIMER_t *)timer_id)->timeout = timeout_ms;
//}

static void st_timer_cback_delete ( void *timer_id )
{
    ST_ASSERT( timer_id != NULL );
    ST_ASSERT( st_timer_list_find( (ST_TIMER_t *)timer_id ) != NULL );
    
    st_timer_list_del( (ST_TIMER_t *)timer_id );
    st_mem_free( timer_id );
}

//static uint32_t st_timer_cback_query  ( void *timer_id )
//{
//    ST_ASSERT( timer_id != NULL );
//    ST_ASSERT( st_timer_list_find( (ST_TIMER_t *)timer_id ) != NULL );
//    
//    return ( (ST_TIMER_t *)timer_id )->timeout;
//}
#else

#if (ST_TIMER_MAX >= UINT8_MAX)
static uint16_t
#else
static uint8_t  
#endif //(ST_TIMER_MAX >= UINT8_MAX)
st_timer_event_find( uint8_t task_id, uint8_t event_id )
{
#if (ST_TIMER_MAX >= UINT8_MAX)
    uint16_t timer_id;
#else
    uint8_t  timer_id;
#endif //(ST_TIMER_MAX >= UINT8_MAX)
    for( timer_id = 0; timer_id < ST_TIMER_MAX; timer_id++ )
    {
        if( st_timer_list[timer_id].timeout > 0 )
        {
            if( st_timer_list[timer_id].task_id  == task_id &&
                st_timer_list[timer_id].event_id == event_id )
            {
                break;
            }
        }
    }
    
    return timer_id;
}
#endif //(ST_TIMER_STATIC_EN == 0)

extern void     st_timer_init         ( void )
{
#if (ST_TIMER_STATIC_EN == 0)
    p_timers_head = NULL;
    p_timers_tail = NULL;
#else
    st_memset( st_timer_list, 0, sizeof(st_timer_list) );
#endif
    time_sec = 0;
    time_ms = 0;
    prev_systick = 0;
    st_systick = 0;
}

extern void     st_timer_update       ( void )
{
#if (ST_TIMER_STATIC_EN == 0)
    ST_TIMER_t *p_timer_curr;
    ST_TIMER_t *p_timer_next;
    ST_TIMER_t *p_timer_head;
    ST_TIMER_t *p_timer_tail;
    void (*p_fxn)( void * );
    void *p_arg;
#else
#if (ST_TIMER_MAX >= UINT8_MAX)
    uint16_t timer_id;
#else
    uint8_t  timer_id;
#endif//(ST_TIMER_MAX >= UINT8_MAX)
#endif//(ST_TIMER_STATIC_EN == 0)

    uint8_t curr_systick;
    uint8_t delta_systick;
    
    ST_ENTER_CRITICAL();
    curr_systick = st_systick;
    ST_EXIT_CRITICAL();

    if( curr_systick != prev_systick )
    {
        delta_systick = ( curr_systick > prev_systick ) ? ( curr_systick - prev_systick ) : ( UINT8_MAX - prev_systick + curr_systick );
        prev_systick = curr_systick;
        
        time_ms += delta_systick;
        if( time_ms >= 1000 )
        {
            time_ms -= 1000;
            time_sec++;
        }

#if (ST_TIMER_STATIC_EN == 0)
        if( p_timers_head )
        {
            ST_ASSERT( p_timers_tail != NULL );
            //save the head and tail to local
            p_timer_head = p_timers_head;
            p_timer_tail = p_timers_tail;
            
            p_timer_curr = NULL;
            p_timer_next = p_timer_head;
            while( p_timer_curr != p_timer_tail )
            {
                p_timer_curr = p_timer_next;
                p_timer_next = p_timer_next->p_timer_next;
                p_timer_curr->timeout = ( p_timer_curr->timeout >= delta_systick ) ? ( p_timer_curr->timeout - delta_systick ) : 0;
                if( p_timer_curr->timeout == 0 )
                {
                    p_fxn = p_timer_curr->p_fxn;
                    p_arg = p_timer_curr->p_arg;
                    st_timer_list_del( p_timer_curr );
                    st_mem_free( p_timer_curr );
                    p_fxn( p_arg );
                }
            }
        }
#else
        for( timer_id = 0; timer_id < ST_TIMER_MAX; timer_id++ )
        {
            if( st_timer_list[timer_id].timeout )
            {
                st_timer_list[timer_id].timeout = ( st_timer_list[timer_id].timeout >= delta_systick ) ? (st_timer_list[timer_id].timeout - delta_systick) : 0;
                if( st_timer_list[timer_id].timeout == 0 )
                {
                    st_event_set( st_timer_list[timer_id].task_id, st_timer_list[timer_id].event_id );
                }
            }
        }
#endif
    }
}

extern void     st_timer_get_time     ( uint32_t *p_sec, uint16_t *p_ms )
{
    if( p_sec )
        *p_sec = time_sec;
    if( p_ms )
        *p_ms = time_ms;
}

extern void     st_timer_set_time     ( uint32_t sec, uint16_t ms )
{
    time_sec = sec;
    time_ms = ms;
}

extern void st_timer_event_create ( uint8_t task_id, uint8_t event_id, st_timer_timeout_t timeout_ms )
{
#if (ST_TIMER_STATIC_EN == 0)
    ST_TIMER_t *p_timer_match;
#else
#if (ST_TIMER_MAX >= UINT8_MAX)
    uint16_t timer_id;
#else
    uint8_t  timer_id;
#endif//(ST_TIMER_MAX >= UINT8_MAX)
#endif

    ST_ASSERT( task_id < ST_TASK_MAX );
    ST_ASSERT( event_id < ST_EVENT_MAX );
    ST_ASSERT( timeout_ms != 0 );
    
#if (ST_TIMER_STATIC_EN == 0)
    p_timer_match = st_timer_event_find( task_id, event_id );
    if( p_timer_match )
    {
        //if found, update it
        p_timer_match->timeout = timeout_ms;
    }
    else
    {
        //if not found, create it
        p_timer_match = (void *)st_timer_cback_create( st_timer_event_kernel, (void *)BUILD_UINT16( event_id, task_id ), timeout_ms);
    }
    ST_ASSERT( p_timer_match != NULL );
#else
    timer_id = st_timer_event_find( task_id, event_id );
    if( timer_id != ST_TIMER_MAX )
    {
        //if found, update it
        st_timer_list[timer_id].timeout = timeout_ms;
    }
    else
    {
        //find a free timer_id
        for( timer_id = 0; timer_id < ST_TIMER_MAX; timer_id++ )
        {
            if( st_timer_list[timer_id].timeout == 0 )
            {
                st_timer_list[timer_id].timeout = timeout_ms;
                st_timer_list[timer_id].task_id = task_id;
                st_timer_list[timer_id].event_id = event_id;
                break;
            }
        }

        ST_ASSERT( timer_id != ST_TIMER_MAX );
    }
#endif

}

extern void st_timer_event_update ( uint8_t task_id, uint8_t event_id, st_timer_timeout_t timeout_ms )
{
#if (ST_TIMER_STATIC_EN == 0)
    ST_TIMER_t *p_timer_match;
#else
#if (ST_TIMER_MAX >= UINT8_MAX)
    uint16_t timer_id;
#else
    uint8_t  timer_id;
#endif//(ST_TIMER_MAX >= UINT8_MAX)
#endif

    ST_ASSERT( task_id < ST_TASK_MAX);
    ST_ASSERT( event_id < ST_EVENT_MAX );
    ST_ASSERT( timeout_ms != 0 );

#if (ST_TIMER_STATIC_EN == 0)
    p_timer_match = st_timer_event_find( task_id, event_id );
    if( p_timer_match )
    {
        p_timer_match->timeout = timeout_ms;
    }
#else
    timer_id = st_timer_event_find( task_id, event_id );
    if( timer_id != ST_TIMER_MAX )
    {
        st_timer_list[timer_id].timeout = timeout_ms;
    }
#endif

}

extern void st_timer_event_delete ( uint8_t task_id, uint8_t event_id )
{
#if (ST_TIMER_STATIC_EN == 0)
    ST_TIMER_t *p_timer_match;
#else
#if (ST_TIMER_MAX >= UINT8_MAX)
    uint16_t timer_id;
#else
    uint8_t  timer_id;
#endif//(ST_TIMER_MAX >= UINT8_MAX)
#endif

    ST_ASSERT( task_id < ST_TASK_MAX);
    ST_ASSERT( event_id < ST_EVENT_MAX );
    
#if (ST_TIMER_STATIC_EN == 0)
    p_timer_match = st_timer_event_find( task_id, event_id );

    if( p_timer_match )
    {
        st_timer_cback_delete( p_timer_match );
    }
    else
    {
        st_event_clr( task_id, event_id );
    }
#else
    timer_id = st_timer_event_find( task_id, event_id );
    if( timer_id != ST_TIMER_MAX )
    {
        st_timer_list[timer_id].timeout = 0;
    }
    else
    {
        st_event_clr( task_id, event_id );
    }
#endif
}

extern uint32_t st_timer_event_query  ( uint8_t task_id, uint8_t event_id )
{
#if (ST_TIMER_STATIC_EN == 0)
    ST_TIMER_t *p_timer_match;
#else
#if (ST_TIMER_MAX >= UINT8_MAX)
    uint16_t timer_id;
#else
    uint8_t  timer_id;
#endif//(ST_TIMER_MAX >= UINT8_MAX)
#endif

    ST_ASSERT( task_id < ST_TASK_MAX);
    ST_ASSERT( event_id < ST_EVENT_MAX );

#if (ST_TIMER_STATIC_EN == 0)
    p_timer_match = st_timer_event_find( task_id, event_id );
    if( p_timer_match )
    {
        return p_timer_match->timeout;
    }
#else
    timer_id = st_timer_event_find( task_id, event_id );
    if( timer_id != ST_TIMER_MAX )
    {
        return st_timer_list[timer_id].timeout;
    }
#endif
    
    return 0;
}

extern void st_timer_systick_inc( void )
{
    st_systick++;
}

#endif /* (ST_TIMER_EN > 0) */











