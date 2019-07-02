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
#ifndef __ST_CONFIG_H__
#define __ST_CONFIG_H__

#define     ST_ASSERT_EN          1
#define     ST_FIFO_EN            1
#define     ST_MSG_EN             1
#define     ST_TIMER_EN           1
#define     ST_TIMER_STATIC_EN    1
#define     ST_TIMER_MAX          8           //meaningless if ST_TIMER_STATIC_EN == 0
#define     ST_TIMER_TIMEOUT_MAX  UINT32_MAX  //should be UINT8_MAX or UINT16_MAX or UINT32_MAX
#define     ST_MEM_EN             1

#define     ST_TASK_MAX           4
#define     ST_EVENT_MAX          32      //must be 8, 16 or 32
#define     ST_FIFO_PAGE_SIZE     64





#endif //__ST_CONFIG_H__

