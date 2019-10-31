#include "rbuf.h"

void ring_buf_flush( ring_buf_t *rb )
{
	rb->head = 0;
	rb->tail = 0;
}

void ring_buf_put( ring_buf_t *rb, st_uint8_t byte )
{
	st_size_t head;
	
	head = rb->head;
	rb->buf[head++] = byte;
	if( head >= rb->size )
		head = 0;
	rb->head = head;
}

st_uint8_t ring_buf_get( ring_buf_t *rb )
{
	st_size_t tail;
	st_uint8_t byte;
	
	tail = rb->tail;
	byte = rb->buf[tail++];
	if( tail >= rb->size )
		tail = 0;
	rb->tail = tail;
	return byte;
}

st_uint8_t ring_buf_full( const ring_buf_t *rb )
{
	st_size_t head;
	head = rb->head;
	head++;
	if( head >= rb->size )
		head = 0;
	return ( head == rb->tail );
}

st_uint8_t ring_buf_empty( const ring_buf_t *rb )
{
	return ( rb->tail == rb->head );
}

st_size_t ring_buf_used_size( const ring_buf_t *rb )
{
    return ( rb->head >= rb->tail ) ? (rb->head - rb->tail) : (rb->size - (rb->tail - rb->head));
}

st_size_t ring_buf_free_size( const ring_buf_t *rb )
{
    return ( rb->head >= rb->tail ) ? (rb->size - 1 - (rb->head - rb->tail)) : (rb->tail - rb->head - 1);
}

