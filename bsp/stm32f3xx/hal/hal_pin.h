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
#ifndef __HAL_PIN_H__
#define __HAL_PIN_H__

#include "hal.h"

#define HAL_PIN_MODE_OUTPUT         0x00
#define HAL_PIN_MODE_INPUT          0x01
#define HAL_PIN_MODE_INPUT_PU       0x02
#define HAL_PIN_MODE_INPUT_PD       0x03
#define HAL_PIN_MODE_OUTPUT_OD      0x04
#define HAL_PIN_MODE_QUASI_PU       0x05
#define HAL_PIN_MODE_ANALOG         0x06


#define HAL_PIN_LOW                 0
#define HAL_PIN_HIGH                1

#define HAL_GPIO_PORT_A             0
#define HAL_GPIO_PORT_B             1
#define HAL_GPIO_PORT_C             2
#define HAL_GPIO_PORT_D             3
#define HAL_GPIO_PORT_E             4
#define HAL_GPIO_PORT_F             5

#define HAL_PIN_GET(port, pin)      (st_int8_t)(port*16+pin)

extern void hal_pin_mode( st_int8_t pin, st_uint8_t mode );
extern void hal_pin_write( st_int8_t pin, st_uint8_t value );
extern void hal_pin_toggle( st_int8_t pin );
extern st_uint8_t hal_pin_read( st_int8_t pin );

#endif /* __HAL_GPIO_H__ */

