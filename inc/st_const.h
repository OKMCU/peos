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
#ifndef __ST_CONST_H__
#define __ST_CONST_H__

#define ST_VERSION                    "V1.0.0"

#ifndef TRUE
#define TRUE    1
#endif

#ifndef FALSE
#define FALSE   0
#endif

#ifndef UINT8_MAX
#define UINT8_MAX   0xFF
#endif

#ifndef UINT16_MAX
#define UINT16_MAX  0xFFFF
#endif

#ifndef UINT32_MAX
#define UINT32_MAX  0xFFFFFFFF
#endif

#ifndef INT8_MAX
#define INT8_MAX    0x7F
#endif

#ifndef INT16_MAX
#define INT16_MAX   0x7FFF
#endif

#ifndef INT32_MAX
#define INT32_MAX   0x7FFFFFF
#endif

#if (ST_MSG_EN > 0)
#define ST_TASK_EVENT_MSG             0
#endif

#endif /* __ST_CONST_H__ */

