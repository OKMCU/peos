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
 
#ifndef __ST_PORTABLE_H__
#define __ST_PORTABLE_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes -------------------------------------------------------------------*/
#include <intrinsics.h>
#include <stdint.h>
#include <string.h>
#include "st_config.h"

/* Exported define ------------------------------------------------------------*/  
#define __IRAM                            
#define __XRAM                            
#define __FLASH             const
#define __STATIC_INLINE     static
#define __PACKED

/* Exported typedef -----------------------------------------------------------*/
typedef uint8_t     st_uint8_t;
typedef uint16_t    st_uint16_t;
typedef uint32_t    st_uint32_t;
typedef int8_t      st_int8_t;
typedef int16_t     st_int16_t;
typedef int32_t     st_int32_t;
typedef float       st_fpt32_t;
typedef double      st_fpt64_t;
typedef size_t      st_size_t;

/* Exported macro -------------------------------------------------------------*/
#define ST_ENTER_CRITICAL()         __disable_interrupt()
#define ST_EXIT_CRITICAL()          __enable_interrupt()
#define st_memset(ptr, val, len)    memset(ptr, val, len)
#define st_strcmp(s1, s2)           strcmp(s1, s2)

/* Exported variables ---------------------------------------------------------*/

/* Exported function prototypes -----------------------------------------------*/
#ifdef __cplusplus
}
#endif

#endif //__ST_PORTABLE_H__
/****** (C) COPYRIGHT 2019 Single-Thread Development Team. *****END OF FILE****/

