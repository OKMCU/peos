/*******************************************************************************
 * Copyright (c) 2021-2022, PEOS Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 * 
 * Change Logs:
 * Date         Author       Notes
 * 2021-10-28   Wentao SUN   first version
 *
 ******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "os.h"

#ifdef OS_TIMER_EN

/* Exported variables --------------------------------------------------------*/
extern const os_uint8_t os_task_max;
/* Private define ------------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
#ifdef OS_TIMER_USE_HEAP
typedef struct os_timer_t {
    struct os_timer_t *p_timer_prev;
    struct os_timer_t *p_timer_next;
    void ( *p_fxn )( void * );
    void *p_arg;
    os_uint32_t timeout;
} OS_TIMER_t;
#else
typedef struct os_timer_t {
    os_uint32_t timeout;
    os_uint8_t task_id;
    os_int8_t event_id;
} OS_TIMER_t;
#endif //OS_TIMER_USE_HEAP

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
#ifdef OS_TIMER_USE_HEAP
static OS_TIMER_t *p_timers_head;
static OS_TIMER_t *p_timers_tail;
#else
static OS_TIMER_t os_timer_list[OS_TIMER_MAX];
#endif //OS_TIMER_USE_HEAP
/* Private function declarations ------------------------------------------*/
void __os_timer_init( void );
void __os_timer_process( os_uint8_t delta_systick );

/* Private function implementations ------------------------------------------*/
#ifdef OS_TIMER_USE_HEAP

#if 0
static OS_TIMER_t *os_timer_list_find( OS_TIMER_t *p_timer )
{
    OS_TIMER_t *p_timer_match;
    
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

static void os_timer_list_del( OS_TIMER_t *p_timer )
{
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

static void os_timer_list_add( OS_TIMER_t *p_timer_new )
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
        p_timers_head = p_timer_new;
        p_timers_tail = p_timer_new;
    }
}

static void os_timer_event_kernel ( void *p_arg )
{
    os_uint16_t u16tmp;
    os_int8_t task_id;
    os_uint8_t event_id;

    u16tmp = (os_uint16_t)( (os_uint32_t)p_arg );
    task_id = (os_uint8_t)HI_UINT16( u16tmp );
    event_id = (os_int8_t)LO_UINT16( u16tmp );
    os_task_set_event( task_id, event_id );
}

static OS_TIMER_t *os_timer_event_find( os_uint8_t task_id, os_int8_t event_id )
{
    OS_TIMER_t *p_timer_match;
    void *p_arg;
    
    if( p_timers_head == NULL )
        return NULL;

    //find the timer at the tail
    p_timer_match = p_timers_head;
    p_arg = (void *)BUILD_UINT16( event_id, task_id );
    while( p_timer_match->p_fxn != os_timer_event_kernel || p_timer_match->p_arg != p_arg )
    {
        p_timer_match = p_timer_match->p_timer_next;
        if( p_timer_match == NULL )
            return NULL;
    }
    
    return p_timer_match;
}

static void    *os_timer_cback_create ( void ( *p_fxn )( void * ), void *p_arg, os_uint32_t tick )
{
    OS_TIMER_t *p_timer_new;

    p_timer_new = os_mem_alloc( sizeof(OS_TIMER_t) );
    if( p_timer_new != NULL )
    {
        p_timer_new->p_fxn = p_fxn;
        p_timer_new->p_arg = p_arg;
        p_timer_new->timeout = tick;
        os_timer_list_add( p_timer_new );
    }
    
    return p_timer_new;
}

//static void os_timer_cback_update ( void *timer_id, os_timer_timeout_t timeout_ms )
//{
//    OS_ASSERT( timer_id != NULL );
//    OS_ASSERT( timeout_ms != 0 );
//    OS_ASSERT( os_timer_list_find( (OS_TIMER_t *)timer_id ) != NULL );
//    
//    ((OS_TIMER_t *)timer_id)->timeout = timeout_ms;
//}

static void os_timer_cback_delete ( void *timer_id )
{
    //OS_ASSERT( timer_id != NULL );
    //OS_ASSERT( os_timer_list_find( (OS_TIMER_t *)timer_id ) != NULL );
    os_timer_list_del( (OS_TIMER_t *)timer_id );
    os_mem_free( timer_id );
}

//static uint32_t os_timer_cback_query  ( void *timer_id )
//{
//    OS_ASSERT( timer_id != NULL );
//    OS_ASSERT( os_timer_list_find( (OS_TIMER_t *)timer_id ) != NULL );
//    
//    return ( (OS_TIMER_t *)timer_id )->timeout;
//}
#else

#if (OS_TIMER_MAX >= UINT8_MAX)
static os_uint16_t
#else
static os_uint8_t  
#endif //(OS_TIMER_MAX >= UINT8_MAX)
os_timer_event_find( os_uint8_t task_id, os_int8_t event_id )
{
#if (OS_TIMER_MAX >= UINT8_MAX)
    os_uint16_t timer_id;
#else
    os_uint8_t  timer_id;
#endif //(OS_TIMER_MAX >= UINT8_MAX)
    for( timer_id = 0; timer_id < OS_TIMER_MAX; timer_id++ )
    {
        if( os_timer_list[timer_id].timeout > 0 )
        {
            if( os_timer_list[timer_id].task_id  == task_id &&
                os_timer_list[timer_id].event_id == event_id )
            {
                break;
            }
        }
    }
    
    return timer_id;
}
#endif //OS_TIMER_USE_HEAP

/* Exported function implementations -----------------------------------------*/
void __os_timer_init( void )
{
#ifdef OS_TIMER_USE_HEAP
    p_timers_head = NULL;
    p_timers_tail = NULL;
#else
    os_memset( os_timer_list, 0, sizeof(os_timer_list) );
#endif
}

void __os_timer_process( os_uint8_t delta_systick )
{
#ifdef OS_TIMER_USE_HEAP
    OS_TIMER_t *p_timer_curr;
    OS_TIMER_t *p_timer_next;
    OS_TIMER_t *p_timer_head;
    OS_TIMER_t *p_timer_tail;
    void (*p_fxn)( void * );
    void *p_arg;
#else
#if (OS_TIMER_MAX >= UINT8_MAX)
    os_uint16_t timer_id;
#else
    os_uint8_t  timer_id;
#endif//(OS_TIMER_MAX >= UINT8_MAX)
#endif//OS_TIMER_USE_HEAP

    if( delta_systick == 0 )    return;

#ifdef OS_TIMER_USE_HEAP
    if( p_timers_head )
    {
        //OS_ASSERT( p_timers_tail != NULL );
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
                os_timer_list_del( p_timer_curr );
                os_mem_free( p_timer_curr );
                p_fxn( p_arg );
            }
        }
    }
#else
    for( timer_id = 0; timer_id < OS_TIMER_MAX; timer_id++ )
    {
        if( os_timer_list[timer_id].timeout )
        {
            os_timer_list[timer_id].timeout = ( os_timer_list[timer_id].timeout >= delta_systick ) ? (os_timer_list[timer_id].timeout - delta_systick) : 0;
            if( os_timer_list[timer_id].timeout == 0 )
            {
                os_task_set_event( os_timer_list[timer_id].task_id, os_timer_list[timer_id].event_id );
            }
        }
    }
#endif
}

os_err_t os_timer_create ( os_uint8_t task_id, os_int8_t event_id, os_uint32_t tick )
{
#ifdef OS_TIMER_USE_HEAP
    OS_TIMER_t *p_timer_match;
#else
#if (OS_TIMER_MAX >= UINT8_MAX)
    os_uint16_t timer_id;
#else
    os_uint8_t  timer_id;
#endif//(OS_TIMER_MAX >= UINT8_MAX)
#endif

    OS_ASSERT( task_id < os_task_max &&
               event_id >= 0 &&
               event_id < OS_TASK_EVENT_MAX &&
               tick != 0 );
    
#ifdef OS_TIMER_USE_HEAP
    p_timer_match = os_timer_event_find( task_id, event_id );
    if( p_timer_match )
    {
        //if found, update it
        p_timer_match->timeout = tick;
    }
    else
    {
        //if not found, create it
        p_timer_match = (void *)os_timer_cback_create( os_timer_event_kernel, (void *)BUILD_UINT16( event_id, task_id ), tick);
        if( p_timer_match == NULL )
            return OS_ERR_NOMEM;
    }
#else
    timer_id = os_timer_event_find( task_id, event_id );
    if( timer_id != OS_TIMER_MAX )
    {
        //if found, update it
        os_timer_list[timer_id].timeout = tick;
    }
    else
    {
        //find a free timer_id
        for( timer_id = 0; timer_id < OS_TIMER_MAX; timer_id++ )
        {
            if( os_timer_list[timer_id].timeout == 0 )
            {
                os_timer_list[timer_id].timeout = tick;
                os_timer_list[timer_id].task_id = task_id;
                os_timer_list[timer_id].event_id = event_id;
                break;
            }
        }

        if( timer_id == OS_TIMER_MAX )
            return OS_ERR_FULL;
    }
#endif
    return OS_ERR_NONE;
}

os_err_t os_timer_update ( os_uint8_t task_id, os_int8_t event_id, os_uint32_t tick )
{
#ifdef OS_TIMER_USE_HEAP
    OS_TIMER_t *p_timer_match;
#else
#if (OS_TIMER_MAX >= UINT8_MAX)
    os_uint16_t timer_id;
#else
    os_uint8_t  timer_id;
#endif//(OS_TIMER_MAX >= UINT8_MAX)
#endif

    OS_ASSERT( task_id < os_task_max &&
               event_id >= 0 &&
               event_id < OS_TASK_EVENT_MAX &&
               tick != 0 );

#ifdef OS_TIMER_USE_HEAP
    p_timer_match = os_timer_event_find( task_id, event_id );
    if( p_timer_match )
    {
        p_timer_match->timeout = tick;
    }
    else
    {
        return OS_ERR_GENERIC;
    }
#else
    timer_id = os_timer_event_find( task_id, event_id );
    if( timer_id != OS_TIMER_MAX )
    {
        os_timer_list[timer_id].timeout = tick;
    }
    else
    {
        return OS_ERR_GENERIC;
    }
#endif
    return OS_ERR_NONE;
}

os_err_t os_timer_delete ( os_uint8_t task_id, os_int8_t event_id )
{
#ifdef OS_TIMER_USE_HEAP
    OS_TIMER_t *p_timer_match;
#else
#if (OS_TIMER_MAX >= UINT8_MAX)
    os_uint16_t timer_id;
#else
    os_uint8_t  timer_id;
#endif//(OS_TIMER_MAX >= UINT8_MAX)
#endif

    OS_ASSERT( task_id < os_task_max &&
               event_id >= 0 &&
               event_id < OS_TASK_EVENT_MAX );
    
#ifdef OS_TIMER_USE_HEAP
    p_timer_match = os_timer_event_find( task_id, event_id );

    if( p_timer_match )
    {
        os_timer_cback_delete( p_timer_match );
    }
    else
    {
        return OS_ERR_GENERIC;
    }
#else
    timer_id = os_timer_event_find( task_id, event_id );
    if( timer_id != OS_TIMER_MAX )
    {
        os_timer_list[timer_id].timeout = 0;
    }
    else
    {
        return OS_ERR_GENERIC;
    }
#endif

    return OS_ERR_NONE;
}

os_uint32_t os_timer_query  ( os_uint8_t task_id, os_int8_t event_id )
{
#ifdef OS_TIMER_USE_HEAP
    OS_TIMER_t *p_timer_match;
#else
#if (OS_TIMER_MAX >= UINT8_MAX)
    os_uint16_t timer_id;
#else
    os_uint8_t  timer_id;
#endif//(OS_TIMER_MAX >= UINT8_MAX)
#endif

    OS_ASSERT( task_id < os_task_max &&
               event_id >= 0 &&
               event_id < OS_TASK_EVENT_MAX );

#ifdef OS_TIMER_USE_HEAP
    p_timer_match = os_timer_event_find( task_id, event_id );
    if( p_timer_match )
    {
        return p_timer_match->timeout;
    }
#else
    timer_id = os_timer_event_find( task_id, event_id );
    if( timer_id != OS_TIMER_MAX )
    {
        return os_timer_list[timer_id].timeout;
    }
#endif
    
    return 0;
}

#endif // OS_TIMER_EN

/****** (C) COPYRIGHT 2021 PEOS Development Team. *****END OF FILE****/

