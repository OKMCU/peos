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
extern const ST_TASK_t *st_task_list;
extern ST_TCB_t *st_task_tcb;
extern const st_uint8_t st_task_max;


/* Private function prototypes -----------------------------------------------*/
/* Exported function implementations -----------------------------------------*/
void *st_msg_create ( st_uint16_t len, st_int8_t type )
{
    ST_MSG_t *pnode_new;
    void *pmsg = NULL;

    ST_ASSERT( len > 0 );
    
    pnode_new = (ST_MSG_t *)st_mem_alloc( sizeof(ST_MSG_t) + len );
    if( pnode_new )
    {
        pmsg = (void *)( (st_uint8_t *)pnode_new + sizeof( ST_MSG_t ) );
        pnode_new->len = len;
        pnode_new->type = type;
    }

    return pmsg;
}

void st_msg_delete ( void *pmsg )
{
    st_mem_free( (st_uint8_t *)pmsg - sizeof(ST_MSG_t) );
}

st_err_t st_msg_send ( void *pmsg, st_uint8_t task_id )
{
    ST_MSG_t *pnode;

    if( task_id < st_task_max )
    {
        if( st_task_list[task_id].p_task_handler == NULL )
            return ST_ERR_GENERIC;
        
        pnode = (ST_MSG_t *)((st_uint8_t *)pmsg - sizeof(ST_MSG_t));
        pnode->from_task_id = st_get_task_id_self();

        if( st_task_tcb[task_id].ptail )
        {
            st_task_tcb[task_id].ptail->next = pnode;
        }
        else
        {
            st_task_tcb[task_id].phead = pnode;
        }
        st_task_tcb[task_id].ptail = pnode;
        return ST_ERR_NONE;
    }

    return ST_ERR_INVAL;
}

st_err_t st_msg_send_urgent ( void *pmsg, st_uint8_t task_id )
{
    ST_MSG_t *pnode;

    if( task_id < st_task_max )
    {
        if( st_task_list[task_id].p_task_handler == NULL )
            return ST_ERR_GENERIC;
        
        pnode = (ST_MSG_t *)((st_uint8_t *)pmsg - sizeof(ST_MSG_t));
        pnode->from_task_id = st_get_task_id_self();

        if( st_task_tcb[task_id].phead )
        {
            pnode->next = st_task_tcb[task_id].phead;
        }
        else
        {
            st_task_tcb[task_id].ptail = pnode;
        }
        st_task_tcb[task_id].phead = pnode;
        return ST_ERR_NONE;
    }
    return ST_ERR_INVAL;
}


void *st_msg_recv( st_uint8_t task_id )
{
    void *pmsg = NULL;

    if( task_id < st_task_max )
    {
        if( st_task_tcb[task_id].phead != NULL )
        {
            pmsg = (void *)((st_uint8_t *)st_task_tcb[task_id].phead + sizeof(ST_MSG_t));
            st_task_tcb[task_id].phead = st_task_tcb[task_id].phead->next;
            if( st_task_tcb[task_id].phead == NULL )
            {
                st_task_tcb[task_id].ptail = NULL;
            }
        }
    }
    return pmsg;
}

st_uint16_t st_msg_len ( void *pmsg )
{
    return ((ST_MSG_t *)((st_uint8_t *)pmsg - sizeof(ST_MSG_t)))->len;
}

st_int8_t st_msg_type ( void *pmsg )
{
    return ((ST_MSG_t *)((st_uint8_t *)pmsg - sizeof(ST_MSG_t)))->type;
}

st_uint8_t st_msg_from ( void *pmsg )
{
    return ((ST_MSG_t *)((st_uint8_t *)pmsg - sizeof(ST_MSG_t)))->from_task_id;
}


/* Private function implementations ------------------------------------------*/


#endif //ST_MSG_EN
/****** (C) COPYRIGHT 2019 Single-Thread Development Team. *****END OF FILE****/
