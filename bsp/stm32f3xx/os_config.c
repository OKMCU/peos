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
#include "hal_drivers.h"
#include "components/cli/cli.h"
#include "components/led/led.h"
#include "application/demo.h"

/* Tasks ---------------------------------------------------------------------*/
static const ST_TASK_t st_task_array[] = {
#ifdef ST_USING_HAL_UART
    { hal_uart_rxd_init, hal_uart_rxd_task },
#endif
#ifdef ST_USING_HAL_UART
    { hal_uart_txd_init, hal_uart_txd_task },
#endif
#ifdef ST_USING_CLI
    { cli_init, cli_task },
#endif
#ifdef ST_USING_LED
    { led_init, led_task },
#endif
    { demo_init, demo_task },
};

/* Do NOT modify -------------------------------------------------------------*/
#define ST_TASK_NUM     (sizeof(st_task_array)/sizeof(ST_TASK_t))
static ST_TCB_t st_tcb_array [ST_TASK_NUM] = {0};
const ST_TASK_t *st_task_list = st_task_array;
const st_uint8_t st_task_max = ST_TASK_NUM;
ST_TCB_t *st_task_tcb = st_tcb_array;

/****** (C) COPYRIGHT 2019 Single-Thread Development Team. *****END OF FILE****/

