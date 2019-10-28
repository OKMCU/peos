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

#ifndef __ST_H__
#define __ST_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes -------------------------------------------------------------------*/
#include "st_config.h"
#include "st_portable.h"
#include "st_const.h"
#include "st_typedef.h"
#include "st_macro.h"

/* Exported define ------------------------------------------------------------*/
/* Exported typedef -----------------------------------------------------------*/
/* Exported macro -------------------------------------------------------------*/
#ifdef ST_ASSERT_EN
#define ST_ASSERT(expr)                        
#define ST_ASSERT_FORCED()
#define ST_ASSERT_STATEMENT(statement)
#define ST_ASSERT_DECLARATION(declaration)
#else
#define ST_ASSERT(expr)                        st( if (!( expr )) st_assert_failed(__FILE__, __LINE__); )
#define ST_ASSERT_FORCED()                     st_assert_failed(__FILE__, __LINE__)
#define ST_ASSERT_STATEMENT(statement)         st( statement )
#define ST_ASSERT_DECLARATION(declaration)     declaration
#endif

/*
 *  This macro compares the size of the first parameter to the integer value
 *  of the second parameter.  If they do not match, a compile time error for
 *  negative array size occurs (even gnu chokes on negative array size).
 *
 *  This compare is done by creating a typedef for an array.  No variables are
 *  created and no memory is consumed with this check.  The created type is
 *  used for checking only and is not for use by any other code.  The value
 *  of 10 in this macro is arbitrary, it just needs to be a value larger
 *  than one to result in a positive number for the array size.
 */
#define ST_ASSERT_SIZE(x,y) typedef char x ## _assert_size_t[-1+10*(sizeof(x) == (y))]

#ifdef ST_MEM_EN
#include "umm_malloc/umm_malloc.h"
#define st_mem_init()                 umm_init()
#define st_mem_alloc(size)            umm_malloc(size)
#define st_mem_calloc(num, size)      umm_calloc(num, size)
#define st_mem_realloc(ptr, size)     umm_realloc(ptr, size)
#define st_mem_free(ptr)              umm_free(ptr)
#endif

/* Exported variables ---------------------------------------------------------*/
/* Exported function prototypes -----------------------------------------------*/
void st_task_set_event( st_uint8_t task_id, st_uint8_t event_id );
void st_task_clr_event( st_uint8_t task_id, st_uint8_t event_id );
st_uint8_t st_get_task_id_self( void );
st_uint8_t st_get_task_id_by_handler( void(* p_task_handler)(st_uint8_t event_id) );
#ifdef ST_TASK_NAME_EN
st_uint8_t st_get_task_id_by_name( const char * p_name );
#endif

#ifdef ST_MSG_EN
void st_msg_init( void );
void *st_msg_create( st_uint16_t len );
void st_msg_delete( void *p_msg );
void st_msg_send( void *p_msg, st_uint8_t task_id );
void st_msg_fwrd( void *p_msg, st_uint8_t task_id );
void *st_msg_recv( st_uint8_t task_id );
st_uint16_t st_msg_len( void *p_msg );
st_uint8_t st_msg_get_type( void *p_msg );
void st_msg_set_type( void *p_msg, st_uint8_t type );
#endif

#ifdef ST_CLOCK_EN
void st_clock_init( void );
st_uint8_t st_clock_update( void );
void st_clock_get( ST_CLOCK_t * clock );
void st_clock_set( const ST_CLOCK_t *clock );
#endif

#ifdef ST_TIMER_EN
void st_timer_init( void );
void st_timer_create( st_uint8_t task_id, st_uint8_t event_id, st_timer_timeout_t timeout_ms );
void st_timer_update( st_uint8_t task_id, st_uint8_t event_id, st_timer_timeout_t timeout_ms );
void st_timer_delete( st_uint8_t task_id, st_uint8_t event_id );
st_timer_timeout_t st_timer_query( st_uint8_t task_id, st_uint8_t event_id );
void st_timer_process( st_uint8_t delta_systick );
#endif

#ifdef __cplusplus
}
#endif

#endif //__ST_H__
/****** (C) COPYRIGHT 2019 Single-Thread Development Team. *****END OF FILE****/
