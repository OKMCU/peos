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

#ifndef __ST_TYPEDEF_H__
#define __ST_TYPEDEF_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes -------------------------------------------------------------------*/
/* Exported define ------------------------------------------------------------*/
/* Exported typedef -----------------------------------------------------------*/
typedef st_uint8_t st_err_t;

#ifdef ST_CLOCK_EN
typedef struct st_clock {
    st_uint32_t tick[2];
} ST_CLOCK_t;
#endif

#ifdef ST_MSG_EN
typedef struct st_msg {
    struct st_msg *p_msg_next;
    st_uint16_t len;
    st_uint8_t from_task_id;
    st_uint8_t type;
} ST_MSG_t;
#endif

#ifdef ST_TIMER_EN
#ifdef ST_TIMER_USE_HEAP
typedef struct st_timer_t {
    struct st_timer_t *prev;
    struct st_timer_t *next;
    st_uint32_t event;
    st_uint32_t timeout;
    st_uint8_t task_id;
} ST_TIMER_t;
#else
typedef struct st_timer_t {
    st_uint32_t timeout;
    st_uint8_t task_id;
    st_uint8_t event;
} ST_TIMER_t;
#endif //ST_TIMER_USE_HEAP
#endif //ST_TIMER_EN

typedef struct st_tcb {

    st_uint32_t (*p_task_handler)( void *p_msg, st_uint32_t event );
    st_uint32_t event;
    
#ifdef ST_MSG_EN
    ST_MSG_t *phead;
    ST_MSG_t *ptail;
#endif

} ST_TCB_t;

/* Exported macro -------------------------------------------------------------*/
/* Exported variables ---------------------------------------------------------*/
/* Exported function prototypes -----------------------------------------------*/

#ifdef __cplusplus
}
#endif

#endif //__ST_TYPEDEF_H__
/****** (C) COPYRIGHT 2019 Single-Thread Development Team. *****END OF FILE****/

