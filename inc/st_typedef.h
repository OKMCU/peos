/******************************************************************************

 @file  

 @brief 

 Group: 
 Target Device: 

 ******************************************************************************
 

 ******************************************************************************
 Release Name: 
 Release Date: 
 *****************************************************************************/
#ifndef __ST_TYPEDEF_H__
#define __ST_TYPEDEF_H__

/**************************** TYPEDEFs ****************************/

typedef struct {
    void (*p_task_init)( void );
    void (*p_task_handler)( st_uint8_t event_id );
} ST_TASK_t;

#if (ST_TIMER_EN > 0)
#if (ST_TIMER_TIMEOUT_MAX <= UINT8_MAX)
typedef st_uint8_t  st_timer_timeout_t;
#elif (ST_TIMER_TIMEOUT_MAX <= UINT16_MAX)
typedef st_uint16_t st_timer_timeout_t;
#else
typedef st_uint32_t st_timer_timeout_t;
#endif
#endif // (ST_TIMER_EN > 0)

#if (ST_MSG_EN > 0)
typedef struct st_msg_t {
    struct st_msg_t *p_node_prev;
    struct st_msg_t *p_node_next;
    st_uint16_t len;
    st_uint8_t  task_id;
    st_uint8_t  type;
} ST_MSG_t;
#endif

#endif //__ST_TYPEDEF_H__
