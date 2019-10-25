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
#ifndef __SPL_GPIO_H__
#define __SPL_GPIO_H__
#include <stdint.h>
#include "M051Series.h"

#define SPL_GPIO_PORT_0         0
#define SPL_GPIO_PORT_1         1
#define SPL_GPIO_PORT_2         2
#define SPL_GPIO_PORT_3         3
#define SPL_GPIO_PORT_4         4

#define SPL_GPIO_PIN_0          0
#define SPL_GPIO_PIN_1          1
#define SPL_GPIO_PIN_2          2
#define SPL_GPIO_PIN_3          3
#define SPL_GPIO_PIN_4          4
#define SPL_GPIO_PIN_5          5
#define SPL_GPIO_PIN_6          6
#define SPL_GPIO_PIN_7          7

#define SPL_GPIO_MODE_INPUT     GPIO_PMD_INPUT
#define SPL_GPIO_MODE_OUTPUT    GPIO_PMD_OUTPUT
#define SPL_GPIO_MODE_OD        GPIO_PMD_OPEN_DRAIN
#define SPL_GPIO_MODE_QUASI     GPIO_PMD_QUASI

#define PORTx(port)             ((GPIO_T *)(GPIO_BASE+0x0040*port))

#define spl_gpio_set_mode( port, pin, mode )        PORTx(port)->PMD = (PORTx(port)->PMD & ~(0x3 << (pin << 1))) | (mode << (pin << 1))
#define spl_gpio_write_pin( port, pin, value )      GPIO_PIN_ADDR( port, pin ) = value
#define spl_gpio_read_pin( port, pin )              GPIO_PIN_ADDR( port, pin )
#define spl_gpio_write_port( port, value )          (PORTx(port)->PIN) = value
#define spl_gpio_read_port( port )                  (PORTx(port)->PIN)


#endif /* __SPL_GPIO_H__ */

