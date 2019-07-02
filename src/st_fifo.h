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
#ifndef __ST_FIFO_H__
#define __ST_FIFO_H__

extern void *st_fifo_create(void);
extern void st_fifo_delete(void *fifo);
extern uint8_t *st_fifo_put(void *fifo, uint8_t byte);
extern uint32_t st_fifo_len(void *fifo);
extern uint8_t st_fifo_get(void *fifo);

#endif /* __ST_FIFO_H__ */

