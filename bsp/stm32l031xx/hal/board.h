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
 
#ifndef __BOARD_H__
#define __BOARD_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes -------------------------------------------------------------------*/
#include "os.h"

/* Exported define ------------------------------------------------------------*/  
/* Exported typedef -----------------------------------------------------------*/
/* Exported macro -------------------------------------------------------------*/
/* Exported variables ---------------------------------------------------------*/
/* Exported function prototypes -----------------------------------------------*/
void os_board_init( void );
void os_board_idle( void );
#ifdef OS_ASSERT_EN
void os_assert_failed(char *file, os_uint32_t line);
#endif

#ifdef __cplusplus
}
#endif

#endif //__OS_BOARD_H__
/****** (C) COPYRIGHT 2019 Single-Thread Development Team. *****END OF FILE****/

