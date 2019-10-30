#ifndef __FIFO_H__
#define __FIFO_H__

extern void     *fifo_create(void);
extern void      fifo_delete(void *fifo);
extern st_uint8_t  *fifo_put   (void *fifo, st_uint8_t byte);
extern st_uint32_t  fifo_len   (void *fifo);
extern st_uint8_t   fifo_get   (void *fifo);

#endif