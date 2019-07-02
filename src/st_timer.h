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
#ifndef __ST_TIMER_H__
#define __ST_TIMER_H__

#if (ST_TIMER_TIMEOUT_MAX == UINT8_MAX)
typedef uint8_t  st_timer_timeout_t;
#elif (ST_TIMER_TIMEOUT_MAX == UINT16_MAX)
typedef uint16_t st_timer_timeout_t;
#elif (ST_TIMER_TIMEOUT_MAX == UINT32_MAX)
typedef uint32_t st_timer_timeout_t;
#else
#error "ST_TIMER_TIMEOUT_MAX must be UINT8_MAX or UINT16_MAX or UINT32_MAX"
#endif
extern void     st_timer_init         ( void );
extern void     st_timer_get_time     ( uint32_t *p_sec, uint16_t *p_ms );
extern void     st_timer_set_time     ( uint32_t sec, uint16_t ms );

extern void     st_timer_event_create ( uint8_t task_id, uint8_t event_id, st_timer_timeout_t timeout_ms );
extern void     st_timer_event_update ( uint8_t task_id, uint8_t event_id, st_timer_timeout_t timeout_ms );
extern void     st_timer_event_delete ( uint8_t task_id, uint8_t event_id );
extern st_timer_timeout_t st_timer_event_query  ( uint8_t task_id, uint8_t event_id );


#endif //__ST_TIMER_H__

