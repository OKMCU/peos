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
#include "hal_drivers.h"

/* Exported variables --------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
extern void hal_uart_driver_event_txd( st_uint8_t port );
extern void hal_uart_driver_event_rxd( st_uint8_t port );

/* Exported function implementations -----------------------------------------*/
void drivers_init( void )
{
    

}

void drivers_task ( st_uint8_t event_id )
{
    switch ( event_id )
    {
        case TASK_EVT_DRIVERS_UART0_RXD:
            hal_uart_driver_event_rxd( HAL_UART_PORT_0 );
        break;

        case TASK_EVT_DRIVERS_UART1_RXD:
            hal_uart_driver_event_rxd( HAL_UART_PORT_1 );
        break;

        case TASK_EVT_DRIVERS_UART0_TXD:
            hal_uart_driver_event_txd( HAL_UART_PORT_0 );
        break;

        case TASK_EVT_DRIVERS_UART1_TXD:
            hal_uart_driver_event_txd( HAL_UART_PORT_1 );
        break;

        default:
            ST_ASSERT_FORCED();
        break;
    }
}

/* Private function implementations ------------------------------------------*/

/****** (C) COPYRIGHT 2019 Single-Thread Development Team. *****END OF FILE****/

