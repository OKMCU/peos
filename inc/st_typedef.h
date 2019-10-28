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
typedef struct {
    void (*p_task_init)( void );
    void (*p_task_handler)( st_uint8_t event_id );
    
#ifdef ST_TASK_NAME_EN
    char *p_task_name;
#endif

} ST_TASK_t;

#ifdef ST_CLOCK_EN
typedef struct {
    st_uint32_t tick[2];
} ST_CLOCK_t;
#endif

#ifdef ST_TIMER_EN
#if (ST_TIMER_TIMEOUT_MAX <= UINT8_MAX)
typedef st_uint8_t  st_timer_timeout_t;
#elif (ST_TIMER_TIMEOUT_MAX <= UINT16_MAX)
typedef st_uint16_t st_timer_timeout_t;
#else
typedef st_uint32_t st_timer_timeout_t;
#endif
#endif // (ST_TIMER_EN > 0)

#ifdef ST_MSG_EN
typedef struct st_msg_t {
    struct st_msg_t *p_node_prev;
    struct st_msg_t *p_node_next;
    st_uint16_t len;
    st_uint8_t  task_id;
    st_uint8_t  type;
} ST_MSG_t;
#endif

/* Exported macro -------------------------------------------------------------*/
/* Exported variables ---------------------------------------------------------*/
/* Exported function prototypes -----------------------------------------------*/

#ifdef __cplusplus
}
#endif

#endif //__ST_TYPEDEF_H__
/****** (C) COPYRIGHT 2019 Single-Thread Development Team. *****END OF FILE****/
