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

#ifndef __DEMO_H__
#define __DEMO_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes -------------------------------------------------------------------*/
#include <stdint.h>
/* Exported define ------------------------------------------------------------*/
#define DEMO_TASK_EVT_LED_BLINK_FAST    1
#define DEMO_TASK_EVT_LED_BLINK_SLOW    2

/* Exported typedef -----------------------------------------------------------*/
/* Exported macro -------------------------------------------------------------*/
/* Exported variables ---------------------------------------------------------*/
/* Exported function prototypes -----------------------------------------------*/
extern void demo_init( void );
extern void demo_task( uint8_t event_id );

#ifdef __cplusplus
}
#endif

#endif //__DEMO_H__
/****** (C) COPYRIGHT 2019 Single-Thread Development Team. *****END OF FILE****/