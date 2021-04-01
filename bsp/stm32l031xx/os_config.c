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
#include "hal_drivers.h"
#include "components/cli/cli.h"
#include "components/led/led.h"
#include "application/demo.h"

/* Tasks ---------------------------------------------------------------------*/
static const OS_TASK_t os_task_array[] = {
#ifdef OS_USING_HAL_UART
    { hal_uart_rxd_init, hal_uart_rxd_task },
#endif
#ifdef OS_USING_HAL_UART
    { hal_uart_txd_init, hal_uart_txd_task },
#endif
#ifdef OS_USING_CLI
    { cli_init, cli_task },
#endif
#ifdef OS_USING_LED
    { led_init, led_task },
#endif
    { demo_init, demo_task },
};

/* Do NOT modify -------------------------------------------------------------*/
#define OS_TASK_NUM     (sizeof(os_task_array)/sizeof(OS_TASK_t))
static OS_TCB_t os_tcb_array [OS_TASK_NUM] = {0};
const OS_TASK_t *os_task_list = os_task_array;
const os_uint8_t os_task_max = OS_TASK_NUM;
OS_TCB_t *os_task_tcb = os_tcb_array;

/****** (C) COPYRIGHT 2019 Single-Thread Development Team. *****END OF FILE****/

