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

#ifdef ST_MSG_EN

/* Exported variables --------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static ST_MSG_t *p_msg_list_head;
static ST_MSG_t *p_msg_list_tail;

/* Private function prototypes -----------------------------------------------*/
static ST_MSG_t *st_msg_list_find( ST_MSG_t *p_node );
static void st_msg_list_del( ST_MSG_t *p_node );
static void st_msg_list_add( ST_MSG_t *p_node_new );

/* Exported function implementations -----------------------------------------*/
void     st_msg_init         ( void )
{
    p_msg_list_head = NULL;
    p_msg_list_tail = NULL;
}

void *st_msg_create ( st_uint16_t len )
{
    ST_MSG_t *p_node_new;
    void *p_msg;

    ST_ASSERT( len > 0 );
    
    p_node_new = (ST_MSG_t *)st_mem_alloc( sizeof(ST_MSG_t) + len );

    if( p_node_new == NULL )
        return NULL;
    
    p_msg = (void *)( (st_uint8_t *)p_node_new + sizeof( ST_MSG_t ) );
    
    p_node_new->len = len;
    p_node_new->type = 0;
    
    return p_msg;
}

void st_msg_delete ( void *p_msg )
{
    ST_MSG_t *p_node;

    ST_ASSERT( p_msg != NULL );

    p_node = (ST_MSG_t *)((st_uint8_t *)p_msg - sizeof(ST_MSG_t));
    
    if( st_msg_list_find( p_node ) != NULL )
    {
        //kick it out of the list
        st_msg_list_del( p_node );
    }
    
    //free the RAM of the msg node
    st_mem_free( p_node );
}

void st_msg_send ( void *p_msg, st_uint8_t task_id )
{
    ST_MSG_t *p_node;
    
    ST_ASSERT( p_msg != NULL );
    p_node = (ST_MSG_t *)((st_uint8_t *)p_msg - sizeof(ST_MSG_t));
    ST_ASSERT( st_msg_list_find( p_node ) == NULL );
    
    p_node->task_id = task_id;
    st_msg_list_add( p_node );
    st_task_set_event( task_id, ST_TASK_EVENT_MSG );
}

void st_msg_fwrd ( void *p_msg, st_uint8_t task_id )
{
    ST_MSG_t *p_node;
    
    ST_ASSERT( p_msg != NULL );
    p_node = (ST_MSG_t *)((st_uint8_t *)p_msg - sizeof(ST_MSG_t));
    ST_ASSERT( st_msg_list_find( p_node ) != NULL );
    p_node->task_id = task_id;
    st_task_set_event( task_id, ST_TASK_EVENT_MSG );
}

void *st_msg_recv ( st_uint8_t task_id )
{
    ST_MSG_t *p_node_match;
    void *p_msg;
    
    if( p_msg_list_head == NULL || p_msg_list_tail == NULL )
        return NULL;

    //find the timer at the tail
    p_node_match = p_msg_list_head;
    while( p_node_match->task_id != task_id )
    {
        p_node_match = p_node_match->p_node_next;
        if( p_node_match == NULL )
            return NULL;
    }

    p_msg = (void *)((st_uint8_t *)p_node_match + sizeof(ST_MSG_t));
    return p_msg;
}

st_uint16_t st_msg_len ( void *p_msg )
{
    ST_MSG_t *p_node;
    
    ST_ASSERT( p_msg != NULL );
    p_node = (ST_MSG_t *)((st_uint8_t *)p_msg - sizeof(ST_MSG_t));
    
    return p_node->len;
}

st_uint8_t st_msg_get_type ( void *p_msg )
{
    ST_MSG_t *p_node;
    
    ST_ASSERT( p_msg != NULL );
    p_node = (ST_MSG_t *)((st_uint8_t *)p_msg - sizeof(ST_MSG_t));
    
    return p_node->type;
}

void st_msg_set_type     ( void *p_msg, st_uint8_t type )
{
    ST_MSG_t *p_node;
    
    ST_ASSERT( p_msg != NULL );
    p_node = (ST_MSG_t *)((st_uint8_t *)p_msg - sizeof(ST_MSG_t));
    
    p_node->type = type;
}

/* Private function implementations ------------------------------------------*/
static ST_MSG_t *st_msg_list_find( ST_MSG_t *p_node )
{
    ST_MSG_t *p_node_match;
    
    if( p_msg_list_head == NULL || p_msg_list_tail == NULL )
    {
        return NULL;
    }
    
    //find the timer at the tail
    p_node_match = p_msg_list_head;
    while( p_node != p_node_match )
    {
        p_node_match = p_node_match->p_node_next;
        if( p_node_match == NULL )
            return NULL;
    }
    
    return p_node_match;
}

static void st_msg_list_del( ST_MSG_t *p_node )
{
    ST_ASSERT( p_msg_list_head != NULL && p_msg_list_tail != NULL );
    
    if( p_node->p_node_prev == NULL &&
        p_node->p_node_next == NULL )
    {
        p_msg_list_head = NULL;
        p_msg_list_tail = NULL;
    }
    else
    {
        if( p_node->p_node_prev != NULL )
        {
            p_node->p_node_prev->p_node_next = p_node->p_node_next;
            if( p_node->p_node_prev->p_node_next == NULL )
            {
                p_msg_list_tail = p_node->p_node_prev;
            }
        }

        if( p_node->p_node_next != NULL )
        {
            p_node->p_node_next->p_node_prev = p_node->p_node_prev;
            if( p_node->p_node_next->p_node_prev == NULL )
            {
                p_msg_list_head = p_node->p_node_next;
            }
        }
    }
}

static void st_msg_list_add( ST_MSG_t *p_node_new )
{
    p_node_new->p_node_next = NULL;
    if( p_msg_list_tail )
    {
        ST_ASSERT( p_msg_list_head != NULL );
        //append the new timer to the tail
        p_node_new->p_node_prev = p_msg_list_tail;
        p_msg_list_tail->p_node_next = p_node_new;
        p_msg_list_tail = p_node_new;
    }
    else
    {
        ST_ASSERT( p_msg_list_head == NULL );
        p_node_new->p_node_prev = NULL;
        p_msg_list_head = p_node_new;
        p_msg_list_tail = p_node_new;
    }
}

#endif //ST_MSG_EN
/****** (C) COPYRIGHT 2019 Single-Thread Development Team. *****END OF FILE****/