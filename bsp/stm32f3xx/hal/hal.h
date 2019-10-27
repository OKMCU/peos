/******************************************************************************

 @file  hal.h

 @brief Describe the purpose and contents of the file.

 Group: 
 Target Device: 

 ******************************************************************************

 ******************************************************************************
 Release Name: 
 Release Date: 
 *****************************************************************************/

#ifndef __HAL_H__
#define __HAL_H__

extern void drivers_init( void );
extern void drivers_task( st_uint8_t event_id );

#include "hal_pin.h"

#endif

