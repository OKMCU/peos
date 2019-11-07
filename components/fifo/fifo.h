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

#ifndef __FIFO_H__
#define __FIFO_H__

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
void *fifo_create(void);
void fifo_delete(void *fifo);
st_uint8_t *fifo_put(void *fifo, st_uint8_t byte);
st_uint32_t fifo_len(void *fifo);
st_uint8_t fifo_get(void *fifo);

#ifdef __cplusplus
}
#endif

#endif

