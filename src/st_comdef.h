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
#ifndef __ST_COMDEF_H__
#define __ST_COMDEF_H__

#include "stdint.h"
#include "stdmacro.h"
#include "st_config.h"
#include "st_port.h"

/**************************** CONSTANTs ***************************/
#define     ST_VERSION                    "V1.0.0"
#if (ST_MSG_EN > 0)
#define     ST_TASK_EVENT_MSG             0
#endif
#define     ST_TASK_ID_NONE               0xFF

/**************************** TYPEDEFs ****************************/
typedef void ( *ST_TASK_t )( uint8_t task_id, uint8_t event_id );


#if ( ST_ASSERT_EN == 0 )
#define ST_ASSERT(expr)                        
#define ST_ASSERT_FORCED()
#define ST_ASSERT_STATEMENT(statement)
#define ST_ASSERT_DECLARATION(declaration)
#else
#define ST_ASSERT(expr)                        st( if (!( expr )) st_assert_handler(); )
#define ST_ASSERT_FORCED()                     st_assert_handler()
#define ST_ASSERT_STATEMENT(statement)         st( statement )
#define ST_ASSERT_DECLARATION(declaration)     declaration
#endif


#endif //__ST_COMDEF_H__
