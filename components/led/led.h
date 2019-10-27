/******************************************************************************

 @file  hal_led.h

 @brief This file contains the interface to the LED Service.

 Group: 
 Target Device: 

 ******************************************************************************

 ******************************************************************************
 Release Name: 
 Release Date: 
 *****************************************************************************/

#ifndef __LED_H__
#define __LED_H__

/*********************************************************************
 * INCLUDES
 */
#include "st.h"

/*********************************************************************
 * MACROS
 */

/*********************************************************************
 * CONSTANTS
 */


/* LEDS - The LED number is the same as the bit position */
#define LED_0     BV(0)
#define LED_1     BV(1)
#define LED_2     BV(2)
#define LED_3     BV(3)
#define LED_4     BV(4)
#define LED_5     BV(5)
#define LED_6     BV(6)
#define LED_7     BV(7)

#define LED_ALL   ( LED_0 | LED_1 | LED_2 | LED_3 | LED_4 | LED_5 | LED_6 | LED_7 )

/* Modes */
#define LED_MODE_OFF     0x00
#define LED_MODE_ON      0x01
#define LED_MODE_BLINK   0x02
#define LED_MODE_FLASH   0x04
#define LED_MODE_TOGGLE  0x08

/* Defaults */

#define LED_DEFAULT_DUTY_CYCLE    5
#define LED_DEFAULT_FLASH_COUNT   50
#define LED_DEFAULT_FLASH_TIME    1000

/*********************************************************************
 * TYPEDEFS
 */


/*********************************************************************
 * GLOBAL VARIABLES
 */

/*
 * Initialize LED Service.
 */
extern void led_init( void );

/*
 * LED task.
 */
extern void led_task( st_uint8_t event_id );

/*
 * Set the LED ON/OFF/TOGGLE.
 */
extern st_uint8_t led_set( st_uint8_t led, st_uint8_t mode );

/*
 * Blink the LED.
 */
extern void led_blink( st_uint8_t leds, st_uint8_t cnt, st_uint8_t duty, st_uint16_t time );

/*
 * Put LEDs in sleep state - store current values
 */
extern void led_enter_sleep( void );

/*
 * Retore LEDs from sleep state
 */
extern void led_exit_sleep( void );

/*
 * Return LED state
 */
extern st_uint8_t led_get_state ( void );


/*********************************************************************
*********************************************************************/

#endif

