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
void os_task_set_event   ( os_uint8_t task_id, os_int8_t event_id )
{
    os_event_t event;
    OS_ASSERT( task_id < os_task_max && event_id >= 0 );
    
    event = BV(event_id);
    OS_ENTER_CRITICAL();
    os_task_tcb[task_id].event |= event;
    OS_EXIT_CRITICAL();
}

void os_task_clr_event   ( os_uint8_t task_id, os_int8_t event_id )
{
    os_event_t event;
    OS_ASSERT( task_id < os_task_max && event_id >= 0 );
    
    event = ~(BV(event_id));
    OS_ENTER_CRITICAL();
    os_task_tcb[task_id].event &= event;
    OS_EXIT_CRITICAL();
}

/* Private function implementations ------------------------------------------*/

/****** (C) COPYRIGHT 2021 PEOS Development Team. *****END OF FILE****/

