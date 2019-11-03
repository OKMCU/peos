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

/* Exported variables --------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
extern ST_TCB_t st_task_list[ST_TASK_MAX];
/* Private function prototypes -----------------------------------------------*/
/* Exported function implementations -----------------------------------------*/
st_err_t st_task_set_event   ( st_uint8_t task_id, st_uint32_t event )
{
    if( task_id < ST_TASK_MAX )
    {
        ST_ENTER_CRITICAL();
        st_task_list[task_id].event |= event;
        ST_EXIT_CRITICAL();
        return ST_ERR_NONE;
    }
    return ST_ERR_INVAL;
}

st_err_t st_task_clr_event   ( st_uint8_t task_id, st_uint32_t event )
{
    if( task_id < ST_TASK_MAX )
    {
        event = ~event;
        ST_ENTER_CRITICAL();
        st_task_list[task_id].event &= event;
        ST_EXIT_CRITICAL();
        return ST_ERR_NONE;
    }
    return ST_ERR_INVAL;
}

st_err_t st_task_create( st_uint8_t task_id, st_uint32_t (*ptask)( void*, st_uint32_t ) )
{
    if( task_id >= ST_TASK_MAX || ptask == NULL )
        return ST_ERR_INVAL;
    
    ST_ENTER_CRITICAL();
    if( st_task_list[task_id].p_task_handler )
    {
        ST_EXIT_CRITICAL();
        return ST_ERR_GENERIC;
    }
    st_task_list[task_id].p_task_handler = ptask;
    ST_EXIT_CRITICAL();
    
    return ST_ERR_NONE;
}

st_err_t st_task_delete( st_uint8_t task_id )
{
    if( task_id >= ST_TASK_MAX )
        return ST_ERR_INVAL;

    ST_ENTER_CRITICAL();
    if( st_task_list[task_id].p_task_handler == NULL )
    {
        ST_EXIT_CRITICAL();
        return ST_ERR_GENERIC;
    }
    if( st_task_list[task_id].event
#ifdef ST_MSG_EN
     || st_task_list[task_id].phead
     || st_task_list[task_id].ptail
#endif 
      )
    {
        ST_EXIT_CRITICAL();
        return ST_ERR_BUSY;
    }

    st_task_list[task_id].p_task_handler = NULL;
    ST_EXIT_CRITICAL();
    return ST_ERR_NONE;
}

/* Private function implementations ------------------------------------------*/

/****** (C) COPYRIGHT 2019 Single-Thread Development Team. *****END OF FILE****/
