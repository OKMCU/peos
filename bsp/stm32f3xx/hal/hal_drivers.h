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

#ifndef __HAL_DRIVERS_H__
#define __HAL_DRIVERS_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes -------------------------------------------------------------------*/
#include "stdint.h"

/* Exported define ------------------------------------------------------------*/
/* Exported typedef -----------------------------------------------------------*/
/* Exported macro -------------------------------------------------------------*/
/* Exported variables ---------------------------------------------------------*/
/* Exported function prototypes -----------------------------------------------*/
void hal_driver_init        ( void );
void hal_task_driver_basic  ( uint8_t task_id, uint8_t event_id );

#ifdef __cplusplus
}
#endif

#endif
/****** (C) COPYRIGHT 2019 Single-Thread Development Team. *****END OF FILE****/
