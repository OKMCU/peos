/*******************************************************************************
 * Copyright (c) 2019-2020, Single-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 * 
 * Change Logs:
 * Date         Author       Notes
 * 2019-10-28   Wentao SUN   first version
 *
 ******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "st.h"
#include "hal_drivers.h"
#include "components/led/led.h"
#include "demo.h"

/* Exported variables --------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Exported function implementations -----------------------------------------*/
void demo_init( void )
{
    st_task_set_event( TASK_ID_DEMO, DEMO_TASK_EVT_LED_BLINK_FAST );
    //st_timer_event_create( TASK_ID_DEMO, DEMO_TASK_EVT_LED_BLINK_FAST, 10000 );
    st_timer_create( TASK_ID_DEMO, DEMO_TASK_EVT_LED_BLINK_SLOW, 10000 );
}

void demo_task( uint8_t event_id )
{
    switch( event_id )
    {
        case DEMO_TASK_EVT_LED_BLINK_FAST:
            led_blink( LED_ALL, 0, 50, 300 );
        break;

        case DEMO_TASK_EVT_LED_BLINK_SLOW:
            led_blink( LED_ALL, 0, 50, 1000 );
        break;

        default:
            ST_ASSERT_FORCED();
        break;
    }
}

/* Private function implementations ------------------------------------------*/

/****** (C) COPYRIGHT 2019 Single-Thread Development Team. *****END OF FILE****/
