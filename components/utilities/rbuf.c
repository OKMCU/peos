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

/* Includes ------------------------------------------------------------------*/
#include "components/utilities/rbuf.h"

/* Exported variables --------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/

/* Exported function implementations -----------------------------------------*/
void ring_buf_flush( ring_buf_t *rb )
{
	rb->head = 0;
	rb->tail = 0;
}

void ring_buf_put( ring_buf_t *rb, os_uint8_t byte )
{
	os_size_t head;
	
	head = rb->head;
	rb->buf[head++] = byte;
	if( head >= rb->size )
		head = 0;
	rb->head = head;
}

os_uint8_t ring_buf_get( ring_buf_t *rb )
{
	os_size_t tail;
	os_uint8_t byte;
	
	tail = rb->tail;
	byte = rb->buf[tail++];
	if( tail >= rb->size )
		tail = 0;
	rb->tail = tail;
	return byte;
}

os_uint8_t ring_buf_full( const ring_buf_t *rb )
{
	os_size_t head;
	head = rb->head;
	head++;
	if( head >= rb->size )
		head = 0;
	return ( head == rb->tail );
}

os_uint8_t ring_buf_empty( const ring_buf_t *rb )
{
	return ( rb->tail == rb->head );
}

os_size_t ring_buf_used_size( const ring_buf_t *rb )
{
    return ( rb->head >= rb->tail ) ? (rb->head - rb->tail) : (rb->size - (rb->tail - rb->head));
}

os_size_t ring_buf_free_size( const ring_buf_t *rb )
{
    return ( rb->head >= rb->tail ) ? (rb->size - 1 - (rb->head - rb->tail)) : (rb->tail - rb->head - 1);
}

/****** (C) COPYRIGHT 2019 Single-Thread Development Team. *****END OF FILE****/

