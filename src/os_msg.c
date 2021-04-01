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
#include "os.h"

#ifdef OS_MSG_EN

/* Exported variables --------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
extern const OS_TASK_t *os_task_list;
extern OS_TCB_t *os_task_tcb;
extern const os_uint8_t os_task_max;


/* Private function prototypes -----------------------------------------------*/
/* Exported function implementations -----------------------------------------*/
void *os_msg_create ( os_uint16_t len, os_int8_t type )
{
    OS_MSG_t *pnode_new;
    void *pmsg = NULL;

    OS_ASSERT( len > 0 );
    
    pnode_new = (OS_MSG_t *)os_mem_alloc( sizeof(OS_MSG_t) + len );
    if( pnode_new )
    {
        pmsg = (void *)( (os_uint8_t *)pnode_new + sizeof( OS_MSG_t ) );
        pnode_new->len = len;
        pnode_new->type = type;
        pnode_new->next = NULL;
    }

    return pmsg;
}

void os_msg_delete ( void *pmsg )
{
    OS_ASSERT( pmsg != NULL ); // should be in the range of theHeap start address and end address
    os_mem_free( (os_uint8_t *)pmsg - sizeof(OS_MSG_t) );
}

void os_msg_send ( void *pmsg, os_uint8_t task_id )
{
    OS_MSG_t *pnode;

    OS_ASSERT( pmsg != NULL ); // should be in the range of theHeap start address and end address
    OS_ASSERT( task_id < os_task_max );
    OS_ASSERT( os_task_list[task_id].p_task_handler != NULL );
    
    pnode = (OS_MSG_t *)((os_uint8_t *)pmsg - sizeof(OS_MSG_t));
    pnode->from_task_id = os_get_task_id_self();

    if( os_task_tcb[task_id].ptail )
    {
        os_task_tcb[task_id].ptail->next = pnode;
    }
    else
    {
        os_task_tcb[task_id].phead = pnode;
    }
    os_task_tcb[task_id].ptail = pnode;
}

void os_msg_send_urgent ( void *pmsg, os_uint8_t task_id )
{
    OS_MSG_t *pnode;

    OS_ASSERT( pmsg != NULL ); // should be in the range of theHeap start address and end address
    OS_ASSERT( task_id < os_task_max );
    OS_ASSERT( os_task_list[task_id].p_task_handler != NULL );
    
    pnode = (OS_MSG_t *)((os_uint8_t *)pmsg - sizeof(OS_MSG_t));
    pnode->from_task_id = os_get_task_id_self();

    if( os_task_tcb[task_id].phead )
    {
        pnode->next = os_task_tcb[task_id].phead;
    }
    else
    {
        os_task_tcb[task_id].ptail = pnode;
    }
    os_task_tcb[task_id].phead = pnode;
}


void *os_msg_recv( os_uint8_t task_id )
{
    void *pmsg = NULL;

    OS_ASSERT( task_id < os_task_max );
    
    if( os_task_tcb[task_id].phead != NULL )
    {
        pmsg = (void *)((os_uint8_t *)os_task_tcb[task_id].phead + sizeof(OS_MSG_t));
        os_task_tcb[task_id].phead = os_task_tcb[task_id].phead->next;
        if( os_task_tcb[task_id].phead == NULL )
        {
            os_task_tcb[task_id].ptail = NULL;
        }
    }
    
    return pmsg;
}

os_uint16_t os_msg_len ( void *pmsg )
{
    OS_ASSERT( pmsg != NULL ); // should be in the range of theHeap start address and end address
    return ((OS_MSG_t *)((os_uint8_t *)pmsg - sizeof(OS_MSG_t)))->len;
}

os_int8_t os_msg_type ( void *pmsg )
{
    OS_ASSERT( pmsg != NULL ); // should be in the range of theHeap start address and end address
    return ((OS_MSG_t *)((os_uint8_t *)pmsg - sizeof(OS_MSG_t)))->type;
}

os_uint8_t os_msg_from ( void *pmsg )
{
    OS_ASSERT( pmsg != NULL ); // should be in the range of theHeap start address and end address
    return ((OS_MSG_t *)((os_uint8_t *)pmsg - sizeof(OS_MSG_t)))->from_task_id;
}


/* Private function implementations ------------------------------------------*/


#endif //OS_MSG_EN
/****** (C) COPYRIGHT 2019 Single-Thread Development Team. *****END OF FILE****/

