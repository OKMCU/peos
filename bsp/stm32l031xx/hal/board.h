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
#include "st.h"

/* Exported define ------------------------------------------------------------*/  
/* Exported typedef -----------------------------------------------------------*/
/* Exported macro -------------------------------------------------------------*/
/* Exported variables ---------------------------------------------------------*/
/* Exported function prototypes -----------------------------------------------*/
void st_board_init( void );
void st_board_idle( void );
#ifdef ST_ASSERT_EN
void st_assert_failed(char *file, st_uint32_t line);
#endif

#ifdef __cplusplus
}
#endif

#endif //__ST_BOARD_H__
/****** (C) COPYRIGHT 2019 Single-Thread Development Team. *****END OF FILE****/

