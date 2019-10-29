/*******************************************************************************
 * Copyright (c) 2019-2020, Single-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date         Author       Notes
 * 2019-10-29   Wentao SUN   first version
 * 
 ******************************************************************************/

#ifndef __HAL_UART_H__
#define __HAL_UART_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes -------------------------------------------------------------------*/
/* Exported define ------------------------------------------------------------*/
#define HAL_UART_BAUD_RATE_2400                  2400
#define HAL_UART_BAUD_RATE_4800                  4800
#define HAL_UART_BAUD_RATE_9600                  9600
#define HAL_UART_BAUD_RATE_19200                 19200
#define HAL_UART_BAUD_RATE_38400                 38400
#define HAL_UART_BAUD_RATE_57600                 57600
#define HAL_UART_BAUD_RATE_115200                115200
#define HAL_UART_BAUD_RATE_230400                230400
#define HAL_UART_BAUD_RATE_460800                460800
#define HAL_UART_BAUD_RATE_921600                921600
#define HAL_UART_BAUD_RATE_2000000               2000000
#define HAL_UART_BAUD_RATE_3000000               3000000

#define HAL_UART_DATA_BITS_5                     5
#define HAL_UART_DATA_BITS_6                     6
#define HAL_UART_DATA_BITS_7                     7
#define HAL_UART_DATA_BITS_8                     8
#define HAL_UART_DATA_BITS_9                     9

#define HAL_UART_STOP_BITS_1                     0
#define HAL_UART_STOP_BITS_2                     1
#define HAL_UART_STOP_BITS_3                     2
#define HAL_UART_STOP_BITS_4                     3

#define HAL_UART_PARITY_NONE                     0
#define HAL_UART_PARITY_ODD                      1
#define HAL_UART_PARITY_EVEN                     2

#define HAL_UART_BIT_ORDER_LSB                   0
#define HAL_UART_BIT_ORDER_MSB                   1

#define HAL_UART_NRZ_NORMAL                      0     /* normal mode */
#define HAL_UART_NRZ_INVERTED                    1     /* inverted mode */


/* Exported typedef -----------------------------------------------------------*/
typedef struct hal_uart_config {
    st_uint32_t baud_rate;
    st_uint32_t data_bits   :4;
    st_uint32_t stop_bits   :2;
    st_uint32_t parity      :2;
    st_uint32_t bit_order   :1;
    st_uint32_t invert      :1;
    st_uint32_t reserved    :6;
    st_uint32_t bufsz       :16;
} hal_uart_config_t;
/* Exported macro -------------------------------------------------------------*/
/* Exported variables ---------------------------------------------------------*/
/* Exported function prototypes -----------------------------------------------*/
void hal_uart_open( st_uint8_t port );
void hal_uart_config( st_uint8_t port, const hal_uart_config_t *cfg );
void hal_uart_txd( st_uint8_t port, st_uint8_t byte );
st_uint8_t hal_uart_rxd( st_uint8_t port );
//st_uint8_t hal_uart_chk_tx_buf( st_uint8_t port );
//st_uint8_t hal_uart_chk_rx_buf( st_uint8_t port );
void hal_uart_close( st_uint8_t port );

#ifdef __cplusplus
}
#endif

#endif //__HAL_UART_H__
/****** (C) COPYRIGHT 2019 Single-Thread Development Team. *****END OF FILE****/
