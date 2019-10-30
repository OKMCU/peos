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

#ifndef __HAL_PIN_H__
#define __HAL_PIN_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes -------------------------------------------------------------------*/
#include "st.h"

/* Exported define ------------------------------------------------------------*/
#define HAL_PIN_MODE_OUTPUT         0x00
#define HAL_PIN_MODE_INPUT          0x01
#define HAL_PIN_MODE_INPUT_PU       0x02
#define HAL_PIN_MODE_INPUT_PD       0x03
#define HAL_PIN_MODE_OUTPUT_OD      0x04
#define HAL_PIN_MODE_QUASI_PU       0x05
#define HAL_PIN_MODE_ANALOG         0x06

#define HAL_PIN_LOW                 0
#define HAL_PIN_HIGH                1

#define HAL_GPIO_PORT_A             0
#define HAL_GPIO_PORT_B             1
#define HAL_GPIO_PORT_C             2
#define HAL_GPIO_PORT_D             3
#define HAL_GPIO_PORT_E             4
#define HAL_GPIO_PORT_F             5

/* Exported typedef -----------------------------------------------------------*/
/* Exported macro -------------------------------------------------------------*/
#define HAL_PIN_GET(port, pin)      (st_uint8_t)(port*16+pin)

/* Exported variables ---------------------------------------------------------*/
/* Exported function prototypes -----------------------------------------------*/
void hal_pin_mode( st_uint8_t pin, st_uint8_t mode );
void hal_pin_write( st_uint8_t pin, st_uint8_t value );
void hal_pin_toggle( st_uint8_t pin );
st_uint8_t hal_pin_read( st_uint8_t pin );

#ifdef __cplusplus
}
#endif

#endif //__HAL_PIN_H__
/****** (C) COPYRIGHT 2019 Single-Thread Development Team. *****END OF FILE****/
