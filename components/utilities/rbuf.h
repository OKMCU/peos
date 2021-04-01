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

#ifndef __RBUF_H__
#define __RBUF_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes -------------------------------------------------------------------*/
#include "os.h"

/* Exported define ------------------------------------------------------------*/
/* Exported typedef -----------------------------------------------------------*/
typedef struct ring_buf {
    os_uint8_t *buf;
    os_size_t size;
	os_size_t head;
	os_size_t tail;
} ring_buf_t;

/* Exported macro -------------------------------------------------------------*/
#define RING_BUF_INC_INDEX( index, size )           index = (index+1)>=size ? 0:(index+1)
#define RING_BUF_FLUSH( head, tail )                st( head = 0; tail = 0; )
#define RING_BUF_PUT( byte, head, p_buf, size )     st( p_buf[head] = byte; RING_BUF_INC_INDEX(head, size); )
#define RING_BUF_GET( p_byte, tail, p_buf, size )   st( *(p_byte) = p_buf[tail]; RING_BUF_INC_INDEX(tail, size); )
#define RING_BUF_EMPTY( head, tail )                (head==tail)
#define RING_BUF_FULL( head, tail, size )           (((head+1)>=size?0:(head+1))==tail)
#define RING_BUF_USED_SIZE( head, tail, size )      ((head>=tail)?(head-tail):(size-(tail-head)))
#define RING_BUF_FREE_SIZE( head, tail, size )      ((head>=tail)?(size-1-(head-tail)):(tail-head-1))

/* Exported variables ---------------------------------------------------------*/
/* Exported function prototypes -----------------------------------------------*/
void ring_buf_flush( ring_buf_t *rb );
void ring_buf_put  ( ring_buf_t *rb, os_uint8_t byte );
os_uint8_t ring_buf_get  ( ring_buf_t *rb );
os_uint8_t ring_buf_full ( const ring_buf_t *rb );
os_uint8_t ring_buf_empty( const ring_buf_t *rb );
os_size_t ring_buf_used_size( const ring_buf_t *rb );
os_size_t ring_buf_free_size( const ring_buf_t *rb );

#ifdef __cplusplus
}
#endif

#endif //__RBUF_H__
/****** (C) COPYRIGHT 2019 Single-Thread Development Team. *****END OF FILE****/

