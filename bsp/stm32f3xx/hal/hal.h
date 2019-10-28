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

void drivers_init( void );
void drivers_task( st_uint8_t event_id );

#include "hal_pin.h"

#endif

