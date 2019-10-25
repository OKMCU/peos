/******************************************************************************

 @file  

 @brief 

 Group: 
 Target Device: 

 ******************************************************************************
 

 ******************************************************************************
 Release Name: 
 Release Date: 2016-06-09 06:57:09
 *****************************************************************************/
#ifndef __ST_H__
#define __ST_H__

#include "st_config.h"
#include "st_portable.h"
#include "st_const.h"
#include "st_typedef.h"
#include "st_macro.h"
#include <stdint.h>
#include <string.h>

extern void st_task_event_set( uint8_t task_id, uint8_t event_id );
extern void st_task_event_clr( uint8_t task_id, uint8_t event_id );

#if ( ST_ASSERT_EN == 0 )
#define ST_ASSERT(expr)                        
#define ST_ASSERT_FORCED()
#define ST_ASSERT_STATEMENT(statement)
#define ST_ASSERT_DECLARATION(declaration)
#else
extern void st_assert_failed(char *file, uint32_t line);
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

#if (ST_MEM_EN > 0)
#include "umm_malloc/umm_malloc.h"
#define st_mem_init()                 umm_init()
#define st_mem_alloc(size)            umm_malloc(size)
#define st_mem_calloc(num, size)      umm_calloc(num, size)
#define st_mem_realloc(ptr, size)     umm_realloc(ptr, size)
#define st_mem_free(ptr)              umm_free(ptr)
#endif

#if (ST_MSG_EN > 0)
extern void     st_msg_init    ( void );
extern void    *st_msg_create  ( uint16_t len );
extern void     st_msg_delete  ( void *p_msg );
extern void     st_msg_send    ( void *p_msg, uint8_t task_id );
extern void     st_msg_fwrd    ( void *p_msg, uint8_t task_id );
extern void    *st_msg_recv    ( uint8_t task_id );
extern uint16_t st_msg_len     ( void *p_msg );
extern uint8_t  st_msg_get_type( void *p_msg );
extern void     st_msg_set_type( void *p_msg, uint8_t type );
#endif

#if (ST_TIMER_EN > 0)
extern void     st_timer_init         ( void );
extern void     st_timer_get_time     ( uint32_t *p_sec, uint16_t *p_ms );
extern void     st_timer_set_time     ( uint32_t sec, uint16_t ms );
extern void     st_timer_event_create ( uint8_t task_id, uint8_t event_id, st_timer_timeout_t timeout_ms );
extern void     st_timer_event_update ( uint8_t task_id, uint8_t event_id, st_timer_timeout_t timeout_ms );
extern void     st_timer_event_delete ( uint8_t task_id, uint8_t event_id );
extern st_timer_timeout_t st_timer_event_query  ( uint8_t task_id, uint8_t event_id );
extern void st_timer_systick_inc( void );
extern void st_timer_update( void );
#endif

#endif //__ST_H__

