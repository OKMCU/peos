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
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
#ifdef ST_TIMER_USE_HEAP
static ST_TIMER_t *ptimers_head;
static ST_TIMER_t *ptimers_tail;
#else
static ST_TIMER_t st_timer_list[ST_TIMER_MAX];
#endif //ST_TIMER_USE_HEAP
/* Private function prototypes -----------------------------------------------*/
void __st_timer_init( void );
void __st_timer_process( st_uint8_t delta_systick );

/* Private function implementations ------------------------------------------*/
#ifdef ST_TIMER_USE_HEAP
static void __st_timer_del( ST_TIMER_t *ptimer )
{
    //ST_ASSERT( ptimer != NULL );
    //ST_ASSERT( ptimers_head != NULL );
    
    if( ptimer->prev == NULL &&
        ptimer->next == NULL )
    {
        ptimers_head = NULL;
        ptimers_tail = NULL;
    }
    else
    {
        if( ptimer->prev != NULL )
        {
            ptimer->prev->next = ptimer->next;
            if( ptimer->prev->next == NULL )
            {
                ptimers_tail = ptimer->prev;
            }
        }

        if( ptimer->next != NULL )
        {
            ptimer->next->prev = ptimer->prev;
            if( ptimer->next->prev == NULL )
            {
                ptimers_head = ptimer->next;
            }
        }
    }
}

static void __st_timer_add( ST_TIMER_t *ptimer_new )
{
    ptimer_new->next = NULL;
    if( ptimers_tail )
    {
        //append the new timer to the tail
        ptimer_new->prev = ptimers_tail;
        ptimers_tail->next = ptimer_new;
        ptimers_tail = ptimer_new;
    }
    else
    {
        ptimer_new->prev = NULL;
        //ST_ASSERT( ptimers_head == NULL );
        ptimers_head = ptimer_new;
        ptimers_tail = ptimer_new;
    }
}

static ST_TIMER_t *__st_timer_find( st_uint8_t task_id, st_uint8_t event )
{
    ST_TIMER_t *match;
    
    if( ptimers_head == NULL )
        return NULL;

    //find the timer at the tail
    match = ptimers_head;
    while( match->task_id != task_id || match->event != event )
    {
        match = match->next;
        if( match == NULL )
            return NULL;
    }
    
    return match;
}

#else

#if (ST_TIMER_MAX >= UINT8_MAX)
static st_uint16_t
#else
static st_uint8_t  
#endif //(ST_TIMER_MAX >= UINT8_MAX)
__st_timer_find( st_uint8_t task_id, st_uint32_t event )
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
                st_timer_list[timer_id].event == event && 
                st_timer_list[timer_id].timeout )
            {
                break;
            }
        }
    }
    
    return timer_id;
}
#endif //ST_TIMER_USE_HEAP

/* Exported function implementations -----------------------------------------*/
void __st_timer_init( void )
{
#ifdef ST_TIMER_USE_HEAP
    ptimers_head = NULL;
    ptimers_tail = NULL;
#else
    st_memset( st_timer_list, 0, sizeof(st_timer_list) );
#endif
}

void __st_timer_process( st_uint8_t delta_systick )
{
#ifdef ST_TIMER_USE_HEAP
    ST_TIMER_t *p_timer_curr;
    ST_TIMER_t *next;
    ST_TIMER_t *p_timer_head;
    ST_TIMER_t *p_timer_tail;
    st_uint32_t event;
    st_uint8_t task_id;
#else
#if (ST_TIMER_MAX >= UINT8_MAX)
    st_uint16_t timer_id;
#else
    st_uint8_t  timer_id;
#endif//(ST_TIMER_MAX >= UINT8_MAX)
#endif//ST_TIMER_USE_HEAP

    if( delta_systick == 0 )    return;

#ifdef ST_TIMER_USE_HEAP
    if( ptimers_head )
    {
        //ST_ASSERT( ptimers_tail != NULL );
        //save the head and tail to local
        p_timer_head = ptimers_head;
        p_timer_tail = ptimers_tail;
        
        p_timer_curr = NULL;
        next = p_timer_head;
        while( p_timer_curr != p_timer_tail )
        {
            p_timer_curr = next;
            next = next->next;
            p_timer_curr->timeout = ( p_timer_curr->timeout >= delta_systick ) ? ( p_timer_curr->timeout - delta_systick ) : 0;
            if( p_timer_curr->timeout == 0 )
            {
                task_id = p_timer_curr->task_id;
                event = p_timer_curr->event;
                __st_timer_del( p_timer_curr );
                st_mem_free( p_timer_curr );
                st_task_set_event( task_id, event );
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
                st_task_set_event( st_timer_list[timer_id].task_id, st_timer_list[timer_id].event );
            }
        }
    }
#endif
}

st_err_t st_timer_create ( st_uint8_t task_id, st_uint32_t event, st_uint32_t tick )
{
#ifdef ST_TIMER_USE_HEAP
    ST_TIMER_t *match;
#else
#if (ST_TIMER_MAX >= UINT8_MAX)
    st_uint16_t timer_id;
#else
    st_uint8_t  timer_id;
#endif//(ST_TIMER_MAX >= UINT8_MAX)
#endif

    if( task_id >= ST_TASK_MAX || tick == 0 )    
        return ST_ERR_INVAL;             
    
#ifdef ST_TIMER_USE_HEAP
    match = __st_timer_find( task_id, event );
    if( match )
    {
        //if found, update it
        match->timeout = tick;
    }
    else
    {
        //if not found, create it
        match = (ST_TIMER_t *)st_mem_alloc( sizeof(ST_TIMER_t) );
        if( match == NULL ) return ST_ERR_NOMEM;
        
        match->task_id = task_id;
        match->event = event;
        match->prev = NULL;
        match->next = NULL;
        match->timeout = tick;
        __st_timer_add( match );
    }
#else
    timer_id = __st_timer_find( task_id, event );
    if( timer_id != ST_TIMER_MAX )
    {
        //if found, update it
        st_timer_list[timer_id].timeout = tick;
    }
    else
    {
        //find a free timer_id
        for( timer_id = 0; timer_id < ST_TIMER_MAX; timer_id++ )
        {
            if( st_timer_list[timer_id].timeout == 0 )
            {
                st_timer_list[timer_id].timeout = tick;
                st_timer_list[timer_id].task_id = task_id;
                st_timer_list[timer_id].event = event;
                return ST_ERR_NONE;
            }
        }

        return ST_ERR_FULL;
    }
#endif
    return ST_ERR_NONE;
}

st_err_t st_timer_update ( st_uint8_t task_id, st_uint32_t event, st_uint32_t tick )
{
#ifdef ST_TIMER_USE_HEAP
    ST_TIMER_t *match;
#else
#if (ST_TIMER_MAX >= UINT8_MAX)
    st_uint16_t timer_id;
#else
    st_uint8_t  timer_id;
#endif//(ST_TIMER_MAX >= UINT8_MAX)
#endif

    if( task_id >= ST_TASK_MAX || tick == 0 )    
        return ST_ERR_INVAL;

#ifdef ST_TIMER_USE_HEAP
    match = __st_timer_find( task_id, event );
    if( match == NULL ) return ST_ERR_GENERIC;
    match->timeout = tick;
#else
    timer_id = __st_timer_find( task_id, event );
    if( timer_id == ST_TIMER_MAX ) return ST_ERR_GENERIC;
    st_timer_list[timer_id].timeout = tick;
#endif

    return ST_ERR_NONE;
}

st_err_t st_timer_delete ( st_uint8_t task_id, st_uint32_t event )
{
#ifdef ST_TIMER_USE_HEAP
    ST_TIMER_t *match;
#else
#if (ST_TIMER_MAX >= UINT8_MAX)
    st_uint16_t timer_id;
#else
    st_uint8_t  timer_id;
#endif//(ST_TIMER_MAX >= UINT8_MAX)
#endif

    if( task_id >= ST_TASK_MAX )    
        return ST_ERR_INVAL;
    
#ifdef ST_TIMER_USE_HEAP
    match = __st_timer_find( task_id, event );
    if( match == NULL ) return ST_ERR_GENERIC;
    __st_timer_del( match );
    st_mem_free( match );
#else
    timer_id = __st_timer_find( task_id, event );
    if( timer_id == ST_TIMER_MAX ) return ST_ERR_GENERIC;
    st_timer_list[timer_id].timeout = 0;
#endif

    return ST_ERR_NONE;
}

st_uint32_t st_timer_query  ( st_uint8_t task_id, st_uint32_t event )
{
#ifdef ST_TIMER_USE_HEAP
    ST_TIMER_t *match;
#else
#if (ST_TIMER_MAX >= UINT8_MAX)
    st_uint16_t timer_id;
#else
    st_uint8_t  timer_id;
#endif//(ST_TIMER_MAX >= UINT8_MAX)
#endif

    if( task_id >= ST_TASK_MAX ) return 0;

#ifdef ST_TIMER_USE_HEAP
    match = __st_timer_find( task_id, event );
    if( match == NULL ) return 0;
    return match->timeout;
#else
    timer_id = __st_timer_find( task_id, event );
    if( timer_id == ST_TIMER_MAX ) return 0;
     return st_timer_list[timer_id].timeout;
#endif
}

#endif // ST_TIMER_EN

/****** (C) COPYRIGHT 2019 Single-Thread Development Team. *****END OF FILE****/
