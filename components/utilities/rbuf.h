#ifndef __RBUF_H__
#define __RBUF_H__

#include "st.h"

#define RING_BUF_INC_INDEX( index, size )           index = (index+1)>=size ? 0:(index+1)
#define RING_BUF_FLUSH( head, tail )                st( head = 0; tail = 0; )
#define RING_BUF_PUT( byte, head, p_buf, size )     st( p_buf[head] = byte; RING_BUF_INC_INDEX(head, size); )
#define RING_BUF_GET( p_byte, tail, p_buf, size )   st( *(p_byte) = p_buf[tail]; RING_BUF_INC_INDEX(tail, size); )
#define RING_BUF_EMPTY( head, tail )                (head==tail)
#define RING_BUF_FULL( head, tail, size )           (((head+1)>=size?0:(head+1))==tail)
#define RING_BUF_USED_SIZE( head, tail, size )      ((head>=tail)?(head-tail):(size-(tail-head)))
#define RING_BUF_FREE_SIZE( head, tail, size )      ((head>=tail)?(size-1-(head-tail)):(tail-head-1))

typedef struct ring_buf {
    st_uint8_t *buf;
    st_size_t size;
	st_size_t head;
	st_size_t tail;
} ring_buf_t;

void ring_buf_flush( ring_buf_t *rb );
void ring_buf_put  ( ring_buf_t *rb, st_uint8_t byte );
st_uint8_t ring_buf_get  ( ring_buf_t *rb );
st_uint8_t ring_buf_full ( const ring_buf_t *rb );
st_uint8_t ring_buf_empty( const ring_buf_t *rb );
st_size_t ring_buf_used_size( const ring_buf_t *rb );
st_size_t ring_buf_free_size( const ring_buf_t *rb );


#endif

