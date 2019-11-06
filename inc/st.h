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
#include "board.h"

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

#define ST_ERR_NONE         0
#define ST_ERR_GENERIC      1
#define ST_ERR_INVAL        2
#define ST_ERR_NOMEM        3
#define ST_ERR_FULL         4
#define ST_ERR_EMPTY        5
#define ST_ERR_BUSY         6
#define ST_ERR_IO           7

#ifdef  ST_MSG_EN
#define ST_TASK_EVT_MSG     (-1)
#define ST_MSG_TYPE_CHAR    (-1)
#define ST_MSG_TYPE_UINT8   (-2)
#define ST_MSG_TYPE_UINT16  (-3)
#define ST_MSG_TYPE_UINT32  (-4)
#define ST_MSG_TYPE_INT8    (-5)
#define ST_MSG_TYPE_INT16   (-6)
#define ST_MSG_TYPE_INT32   (-7)
#define ST_MSG_TYPE_FPT32   (-8)
#define ST_MSG_TYPE_FPT64   (-9)
#endif//ST_MSG_EN

/* Exported typedef -----------------------------------------------------------*/
typedef st_uint8_t st_err_t;
#if ST_TASK_EVENT_MAX <= 8
typedef st_uint8_t st_event_t;
#elif ST_TASK_EVENT_MAX <= 16
typedef st_uint16_t st_event_t;
#elif ST_TASK_EVENT_MAX <= 32
typedef st_uint16_t st_event_t;
#else
#error "ST_TASK_EVENT_MAX should not be larger than 32."
#endif

#ifdef ST_CLOCK_EN
typedef struct st_clock {
    st_uint32_t tick[2];
} ST_CLOCK_t;
#endif

#ifdef ST_MSG_EN
typedef struct st_msg {
    struct st_msg *next;
    st_uint16_t len;
    st_uint8_t from_task_id;
    st_int8_t type;
} ST_MSG_t;
#endif

typedef struct st_tcb {

    st_event_t event;

#ifdef ST_MSG_EN
    ST_MSG_t *phead;
    ST_MSG_t *ptail;
#endif

} ST_TCB_t;

typedef struct st_task {
    void (*p_task_init)( st_uint8_t task_id );
    void (*p_task_handler)( st_int8_t event_id );
} ST_TASK_t;

/* Exported macro -------------------------------------------------------------*/
#ifndef BV
#define BV(n)      (1 << (n))
#endif

#ifndef BF
#define BF(x,b,s)  (((x) & (b)) >> (s))
#endif

#ifndef MIN
#define MIN(n,m)   (((n) < (m)) ? (n) : (m))
#endif

#ifndef MAX
#define MAX(n,m)   (((n) < (m)) ? (m) : (n))
#endif

#ifndef ABS
#define ABS(n)     (((n) < 0) ? -(n) : (n))
#endif

/* takes a byte out of a uint32_t : var - uint32_t,  ByteNum - byte to take out (0 - 3) */
#define BREAK_UINT32( var, ByteNum ) \
          (st_uint8_t)((st_uint32_t)(((var) >>((ByteNum) * 8)) & 0x00FF))

#define BUILD_UINT32(Byte0, Byte1, Byte2, Byte3) \
          ((st_uint32_t)((st_uint32_t)((Byte0) & 0x00FF) \
          + ((st_uint32_t)((Byte1) & 0x00FF) << 8) \
          + ((st_uint32_t)((Byte2) & 0x00FF) << 16) \
          + ((st_uint32_t)((Byte3) & 0x00FF) << 24)))

#define BUILD_UINT16(loByte, hiByte) \
          ((st_uint16_t)(((loByte) & 0x00FF) + (((hiByte) & 0x00FF) << 8)))

#define HI_UINT16(a) (((a) >> 8) & 0xFF)
#define LO_UINT16(a) ((a) & 0xFF)

#define BUILD_UINT8(hiByte, loByte) \
          ((st_uint8_t)(((loByte) & 0x0F) + (((hiByte) & 0x0F) << 4)))

#define HI_UINT8(a) (((a) >> 4) & 0x0F)
#define LO_UINT8(a) ((a) & 0x0F)

// Write the 32bit value of 'val' in little endian format to the buffer pointed 
// to by pBuf, and increment pBuf by 4
#define UINT32_TO_BUF_LITTLE_ENDIAN(p_buf,val) \
	do { \
		*(p_buf)++ = ((((st_uint32_t)(val)) >>  0) & 0xFF); \
		*(p_buf)++ = ((((st_uint32_t)(val)) >>  8) & 0xFF); \
		*(p_buf)++ = ((((st_uint32_t)(val)) >> 16) & 0xFF); \
		*(p_buf)++ = ((((st_uint32_t)(val)) >> 24) & 0xFF); \
	} while (0)

// Return the 32bit little-endian formatted value pointed to by pBuf, and increment pBuf by 4
#define BUF_TO_UINT32_LITTLE_ENDIAN(p_buf) (((p_buf) += 4), BUILD_UINT32((p_buf)[-4], (p_buf)[-3], (p_buf)[-2], (p_buf)[-1]))

//#ifndef GET_BIT
//#define GET_BIT(DISCS, IDX)  (((DISCS)[((IDX) / 8)] & BV((IDX) % 8)) ? TRUE : FALSE)
//#endif
//#ifndef SET_BIT
//#define SET_BIT(DISCS, IDX)  (((DISCS)[((IDX) / 8)] |= BV((IDX) % 8)))
//#endif
//#ifndef CLR_BIT
//#define CLR_BIT(DISCS, IDX)  (((DISCS)[((IDX) / 8)] &= (BV((IDX) % 8) ^ 0xFF)))
//#endif

/*
 *  This macro is for use by other macros to form a fully valid C statement.
 *  Without this, the if/else conditionals could show unexpected behavior.
 *
 *  For example, use...
 *    #define SET_REGS()  st( ioreg1 = 0; ioreg2 = 0; )
 *  instead of ...
 *    #define SET_REGS()  { ioreg1 = 0; ioreg2 = 0; }
 *  or
 *    #define  SET_REGS()    ioreg1 = 0; ioreg2 = 0;
 *  The last macro would not behave as expected in the if/else construct.
 *  The second to last macro will cause a compiler error in certain uses
 *  of if/else construct
 *
 *  It is not necessary, or recommended, to use this macro where there is
 *  already a valid C statement.  For example, the following is redundant...
 *    #define CALL_FUNC()   st(  func();  )
 *  This should simply be...
 *    #define CALL_FUNC()   func()
 *
 * (The while condition below evaluates false without generating a
 *  constant-controlling-loop type of warning on most compilers.)
 */
#define st(x)      do { x } while (__LINE__ == -1)


#ifndef ST_ASSERT_EN
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
#define st_mem_alloc(size)            umm_malloc(size)
#define st_mem_calloc(num, size)      umm_calloc(num, size)
#define st_mem_realloc(ptr, size)     umm_realloc(ptr, size)
#define st_mem_free(ptr)              umm_free(ptr)
#endif

/* Exported variables ---------------------------------------------------------*/
/* Exported function prototypes -----------------------------------------------*/
st_err_t st_task_set_event( st_uint8_t task_id, st_int8_t event_id );
st_err_t st_task_clr_event( st_uint8_t task_id, st_int8_t event_id );
st_uint8_t st_get_task_id_self( void );

#ifdef ST_MSG_EN
void *st_msg_create( st_uint16_t len, st_int8_t type, st_err_t *err );
void st_msg_delete ( void *pmsg );
st_err_t st_msg_send( void *pmsg, st_uint8_t task_id );
st_err_t st_msg_send_urgent ( void *pmsg, st_uint8_t task_id );
void *st_msg_recv( st_uint8_t task_id );
st_uint16_t st_msg_len( void *pmsg );
st_int8_t st_msg_type( void *pmsg );
st_uint8_t st_msg_from( void *pmsg );
#endif

#ifdef ST_CLOCK_EN
void st_clock_get( ST_CLOCK_t * clock );
void st_clock_set( const ST_CLOCK_t *clock );
#endif

#ifdef ST_TIMER_EN
st_err_t st_timer_create( st_uint8_t task_id, st_int8_t event_id, st_uint32_t tick );
st_err_t st_timer_update( st_uint8_t task_id, st_int8_t event_id, st_uint32_t tick );
st_err_t st_timer_delete( st_uint8_t task_id, st_int8_t event_id );
st_uint32_t st_timer_query( st_uint8_t task_id, st_int8_t event_id );
#endif

#ifdef __cplusplus
}
#endif

#endif //__ST_H__
/****** (C) COPYRIGHT 2019 Single-Thread Development Team. *****END OF FILE****/
