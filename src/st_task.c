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
extern ST_TASK_t st_task_list[ST_TASK_MAX];
extern ST_TCB_t st_task_tcb[ST_TASK_MAX];
/* Private function prototypes -----------------------------------------------*/
/* Exported function implementations -----------------------------------------*/
st_err_t st_task_set_event   ( st_uint8_t task_id, st_int8_t event_id )
{
    st_uint32_t event;
    
    if( task_id >= ST_TASK_MAX || event_id < 0 )
        return ST_ERR_INVAL;
    
    event = BV(event_id);
    ST_ENTER_CRITICAL();
    st_task_tcb[task_id].event |= event;
    ST_EXIT_CRITICAL();
    return ST_ERR_NONE;
}

st_err_t st_task_clr_event   ( st_uint8_t task_id, st_int8_t event_id )
{
    st_uint32_t event;
    
    if( task_id >= ST_TASK_MAX || event_id < 0 )
        return ST_ERR_INVAL;
    
    event = ~(BV(event_id));
    ST_ENTER_CRITICAL();
    st_task_tcb[task_id].event &= event;
    ST_EXIT_CRITICAL();
    return ST_ERR_NONE;
}

/* Private function implementations ------------------------------------------*/

/****** (C) COPYRIGHT 2019 Single-Thread Development Team. *****END OF FILE****/

