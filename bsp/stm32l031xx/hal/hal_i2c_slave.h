/*******************************************************************************
 * Copyright (c) 2019-2020, Single-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date         Author       Notes
 * 2019-11-21   Wentao SUN   first version
 * 
 ******************************************************************************/

#ifndef __HAL_I2C_SLAVE_H__
#define __HAL_I2C_SLAVE_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes -------------------------------------------------------------------*/
#include "os.h"

/* Exported define ------------------------------------------------------------*/
#define HAL_I2CS_PORT_0                          0
#define HAL_I2CS_PORT_1                          1
#define HAL_I2CS_PORT_MAX                        2

#define HAL_I2CS_ADDR_MAX                        2

#define HAL_I2CS_EVENT_ADDRW                     0  // I2C slave event, ADDR+W matched
#define HAL_I2CS_EVENT_RXNE                      1  // I2C slave event, RX buffer is not empty
#define HAL_I2CS_EVENT_ADDRR                     2  // I2C slave event, ADDR+R matched
#define HAL_I2CS_EVENT_TXE                       3  // I2C slave event, TX buffer is empty
#define HAL_I2CS_EVENT_NACK                      4  // I2C slave event, Received NACK
#define HAL_I2CS_EVENT_STOP                      5  // I2C slave event, Received STOP

/* Exported typedef -----------------------------------------------------------*/
typedef struct {
    os_uint8_t addr[HAL_I2CS_ADDR_MAX];
    os_uint8_t general_call_en  : 1;
    os_uint8_t reserved         : 7;
    void (*callback)( os_uint8_t event );
} hal_i2cs_config_t;

/* Exported macro -------------------------------------------------------------*/
/* Exported variables ---------------------------------------------------------*/
/* Exported function prototypes -----------------------------------------------*/
void hal_i2cs_port0_init( os_uint8_t task_id );
void hal_i2cs_port0_task( os_int8_t event_id );
void hal_i2cs_port1_init( os_uint8_t task_id );
void hal_i2cs_port1_task( os_int8_t event_id );

void hal_i2cs_config( os_uint8_t port, const hal_i2cs_config_t *cfg );
void hal_i2cs_open( os_uint8_t port );
os_uint8_t hal_i2cs_set_addr( os_uint8_t port, os_uint8_t addr_id, os_uint8_t addr );
os_uint8_t hal_i2cs_get_addr( os_uint8_t port, os_uint8_t addr_id );
os_uint8_t hal_i2cs_matched_addr( os_uint8_t port );
void hal_i2cs_putc( os_uint8_t port, os_uint8_t byte );
os_uint8_t hal_i2cs_getc( os_uint8_t port );
os_uint8_t hal_i2cs_tx_buf_free_size( os_uint8_t port );
os_uint8_t hal_i2cs_rx_buf_used_size( os_uint8_t port );
void hal_i2cs_close( os_uint8_t port );

#ifdef __cplusplus
}
#endif

#endif //__HAL_I2C_SLAVE_H__
/****** (C) COPYRIGHT 2019 Single-Thread Development Team. *****END OF FILE****/

