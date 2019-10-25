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

#include <stdint.h>
/**************************** TYPEDEFs ****************************/
typedef void ( *ST_TASK_t )( uint8_t task_id, uint8_t event_id );

#if (ST_TIMER_EN > 0)
#if (ST_TIMER_TIMEOUT_MAX == UINT8_MAX)
typedef uint8_t  st_timer_timeout_t;
#elif (ST_TIMER_TIMEOUT_MAX == UINT16_MAX)
typedef uint16_t st_timer_timeout_t;
#elif (ST_TIMER_TIMEOUT_MAX == UINT32_MAX)
typedef uint32_t st_timer_timeout_t;
#else
#error "ST_TIMER_TIMEOUT_MAX must be UINT8_MAX or UINT16_MAX or UINT32_MAX"
#endif
#endif // (ST_TIMER_EN > 0)

#if (ST_MSG_EN > 0)
typedef struct st_msg_t {
    struct st_msg_t *p_node_prev;
    struct st_msg_t *p_node_next;
    uint16_t len;
    uint8_t  task_id;
    uint8_t  type;
} ST_MSG_t;
#endif

#endif //__ST_TYPEDEF_H__
