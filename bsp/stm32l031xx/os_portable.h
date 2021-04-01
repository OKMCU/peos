/*******************************************************************************
 * Copyright (c) 2019-2020, PEOS Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 * 
 * Change Logs:
 * Date         Author       Notes
 * 2019-10-28   Wentao SUN   first version
 *
 ******************************************************************************/
 
#ifndef __OS_PORTABLE_H__
#define __OS_PORTABLE_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes -------------------------------------------------------------------*/
#include <intrinsics.h>
#include <stdint.h>
#include <string.h>
#include "os_config.h"

/* Exported define ------------------------------------------------------------*/  
#define __IRAM                            
#define __XRAM                            
#define __FLASH             
#define __STATIC_INLINE     static inline
#define __PACKED

/* Exported typedef -----------------------------------------------------------*/
typedef uint8_t     os_uint8_t;
typedef uint16_t    os_uint16_t;
typedef uint32_t    os_uint32_t;
typedef int8_t      os_int8_t;
typedef int16_t     os_int16_t;
typedef int32_t     os_int32_t;
typedef float       os_fpt32_t;
typedef double      os_fpt64_t;
typedef size_t      os_size_t;

/* Exported macro -------------------------------------------------------------*/
#define OS_ENTER_CRITICAL()         __disable_interrupt()
#define OS_EXIT_CRITICAL()          __enable_interrupt()
#define os_memset(ptr, val, len)    memset(ptr, val, len)
#define os_strcmp(s1, s2)           strcmp(s1, s2)
#define os_strlen(s)                strlen(s)

/* Exported variables ---------------------------------------------------------*/

/* Exported function prototypes -----------------------------------------------*/
#ifdef __cplusplus
}
#endif

#endif //__OS_PORTABLE_H__
/****** (C) COPYRIGHT 2019 PEOS Development Team. *****END OF FILE****/

