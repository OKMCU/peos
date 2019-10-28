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

#ifndef __ST_CONST_H__
#define __ST_CONST_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes -------------------------------------------------------------------*/
/* Exported define ------------------------------------------------------------*/
#define ST_VERSION                    "V1.0.0"

#ifndef TRUE
#define TRUE    1
#endif

#ifndef FALSE
#define FALSE   0
#endif

#ifndef UINT8_MAX
#define UINT8_MAX   0xFF
#endif

#ifndef UINT16_MAX
#define UINT16_MAX  0xFFFF
#endif

#ifndef UINT32_MAX
#define UINT32_MAX  0xFFFFFFFF
#endif

#ifndef INT8_MAX
#define INT8_MAX    0x7F
#endif

#ifndef INT16_MAX
#define INT16_MAX   0x7FFF
#endif

#ifndef INT32_MAX
#define INT32_MAX   0x7FFFFFF
#endif

#ifdef ST_MSG_EN
#define ST_TASK_EVENT_MSG             0
#endif

/* Exported typedef -----------------------------------------------------------*/
/* Exported macro -------------------------------------------------------------*/
/* Exported variables ---------------------------------------------------------*/
/* Exported function prototypes -----------------------------------------------*/

#ifdef __cplusplus
}
#endif

#endif //__ST_CONST_H__
/****** (C) COPYRIGHT 2019 Single-Thread Development Team. *****END OF FILE****/