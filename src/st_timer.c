/*******************************************************************************
 * Copyright (c) 2019-2020, Single-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 * 
 * Change Logs:
 * Date         Author       Notes
 * 2019-10-28   Wentao SUN   first version
 *
 ******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "st.h"

#ifdef ST_TIMER_EN

/* Exported variables --------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
#ifdef ST_TIMER_USE_MEM_HEAP
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
    st_uint8_t task_id;
    st_uint8_t event_id;
} ST_TIMER_t;
#endif //ST_TIMER_USE_MEM_HEAP

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
#ifdef ST_TIMER_USE_MEM_HEAP
static ST_TIMER_t *p_timers_head;
static ST_TIMER_t *p_timers_tail;
#else
static ST_TIMER_t st_timer_list[ST_TIMER_MAX];
#endif //ST_TIMER_USE_MEM_HEAP

/* Private function implementations ------------------------------------------*/
#ifdef ST_TIMER_USE_MEM_HEAP
#ifdef ST_ASSERT_EN
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
    st_uint16_t u16tmp;
    st_uint8_t task_id;
    st_uint8_t event_id;

    u16tmp = (st_uint16_t)((uint32_t)p_arg);
    task_id = HI_UINT16(u16tmp);
    event_id = LO_UINT16(u16tmp);
    st_task_set_event(task_id, event_id);
}

static ST_TIMER_t *st_timer_event_find( st_uint8_t task_id, st_uint8_t event_id )
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
static st_uint16_t
#else
static st_uint8_t  
#endif //(ST_TIMER_MAX >= UINT8_MAX)
st_timer_event_find( st_uint8_t task_id, st_uint8_t event_id )
{
#if (ST_TIMER_MAX >= UINT8_MAX)
    st_uint16_t timer_id;
#else
    st_uint8_t  timer_id;
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
#endif //ST_TIMER_USE_MEM_HEAP

/* Exported function implementations -----------------------------------------*/
void st_timer_init( void )
{
#ifdef ST_TIMER_USE_MEM_HEAP
    p_timers_head = NULL;
    p_timers_tail = NULL;
#else
    st_memset( st_timer_list, 0, sizeof(st_timer_list) );
#endif
}

void st_timer_process( st_uint8_t delta_systick )
{
#ifdef ST_TIMER_USE_MEM_HEAP
    ST_TIMER_t *p_timer_curr;
    ST_TIMER_t *p_timer_next;
    ST_TIMER_t *p_timer_head;
    ST_TIMER_t *p_timer_tail;
    void (*p_fxn)( void * );
    void *p_arg;
#else
#if (ST_TIMER_MAX >= UINT8_MAX)
    st_uint16_t timer_id;
#else
    st_uint8_t  timer_id;
#endif//(ST_TIMER_MAX >= UINT8_MAX)
#endif//ST_TIMER_USE_MEM_HEAP

    if( delta_systick == 0 )    return;

#ifdef ST_TIMER_USE_MEM_HEAP
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
                st_task_set_event( st_timer_list[timer_id].task_id, st_timer_list[timer_id].event_id );
            }
        }
    }
#endif
}

void st_timer_create ( st_uint8_t task_id, st_uint8_t event_id, st_timer_timeout_t timeout_ms )
{
#ifdef ST_TIMER_USE_MEM_HEAP
    ST_TIMER_t *p_timer_match;
#else
#if (ST_TIMER_MAX >= UINT8_MAX)
    st_uint16_t timer_id;
#else
    st_uint8_t  timer_id;
#endif//(ST_TIMER_MAX >= UINT8_MAX)
#endif

    ST_ASSERT( task_id < ST_TASK_MAX );
    ST_ASSERT( event_id < ST_EVENT_MAX );
    ST_ASSERT( timeout_ms != 0 );
    
#ifdef ST_TIMER_USE_MEM_HEAP
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

void st_timer_update ( st_uint8_t task_id, st_uint8_t event_id, st_timer_timeout_t timeout_ms )
{
#ifdef ST_TIMER_USE_MEM_HEAP
    ST_TIMER_t *p_timer_match;
#else
#if (ST_TIMER_MAX >= UINT8_MAX)
    st_uint16_t timer_id;
#else
    st_uint8_t  timer_id;
#endif//(ST_TIMER_MAX >= UINT8_MAX)
#endif

    ST_ASSERT( task_id < ST_TASK_MAX);
    ST_ASSERT( event_id < ST_EVENT_MAX );
    ST_ASSERT( timeout_ms != 0 );

#ifdef ST_TIMER_USE_MEM_HEAP
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

void st_timer_delete ( st_uint8_t task_id, st_uint8_t event_id )
{
#ifdef ST_TIMER_USE_MEM_HEAP
    ST_TIMER_t *p_timer_match;
#else
#if (ST_TIMER_MAX >= UINT8_MAX)
    st_uint16_t timer_id;
#else
    st_uint8_t  timer_id;
#endif//(ST_TIMER_MAX >= UINT8_MAX)
#endif

    ST_ASSERT( task_id < ST_TASK_MAX);
    ST_ASSERT( event_id < ST_EVENT_MAX );
    
#ifdef ST_TIMER_USE_MEM_HEAP
    p_timer_match = st_timer_event_find( task_id, event_id );

    if( p_timer_match )
    {
        st_timer_cback_delete( p_timer_match );
    }
    else
    {
        st_task_clr_event( task_id, event_id );
    }
#else
    timer_id = st_timer_event_find( task_id, event_id );
    if( timer_id != ST_TIMER_MAX )
    {
        st_timer_list[timer_id].timeout = 0;
    }
    else
    {
        st_task_clr_event( task_id, event_id );
    }
#endif
}

st_timer_timeout_t st_timer_query  ( st_uint8_t task_id, st_uint8_t event_id )
{
#ifdef ST_TIMER_USE_MEM_HEAP
    ST_TIMER_t *p_timer_match;
#else
#if (ST_TIMER_MAX >= UINT8_MAX)
    st_uint16_t timer_id;
#else
    st_uint8_t  timer_id;
#endif//(ST_TIMER_MAX >= UINT8_MAX)
#endif

    ST_ASSERT( task_id < ST_TASK_MAX);
    ST_ASSERT( event_id < ST_EVENT_MAX );

#ifdef ST_TIMER_USE_MEM_HEAP
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

#endif // ST_TIMER_EN

/****** (C) COPYRIGHT 2019 Single-Thread Development Team. *****END OF FILE****/
