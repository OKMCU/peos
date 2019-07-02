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
#ifndef __ST_MSG_H__
#define __ST_MSG_H__

extern void        st_msg_init         ( void );
extern void       *st_msg_create       ( uint16_t len );
extern void        st_msg_delete       ( void *p_msg );
extern void        st_msg_send         ( void *p_msg, uint8_t task_id );
extern void        st_msg_fwrd         ( void *p_msg, uint8_t task_id );
extern void       *st_msg_recv         ( uint8_t task_id );
extern uint16_t    st_msg_len          ( void *p_msg );
extern uint8_t     st_msg_get_type     ( void *p_msg );
extern void        st_msg_set_type     ( void *p_msg, uint8_t type );

#endif //__ST_MSG_H__

