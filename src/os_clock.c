/*******************************************************************************
 * Copyright (c) 2021-2022, PEOS Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 * 
 * Change Logs:
 * Date         Author       Notes
 * 2021-10-28   Wentao SUN   first version
 *
 ******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "os.h"

#ifdef OS_CLOCK_EN
/* Exported variables --------------------------------------------------------*/
os_uint8_t os_systick;
/* Private define ------------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static OS_CLOCK_t sysclock;
static os_uint8_t prev_systick;

/* Private function prototypes -----------------------------------------------*/
void __os_clock_init( void );
os_uint8_t __os_clock_update( void );

/* Exported function implementations -----------------------------------------*/
void os_clock_get     ( OS_CLOCK_t *clock )
{
    OS_ASSERT( clock != NULL );
    clock->tick[0] = sysclock.tick[0];
    clock->tick[1] = sysclock.tick[1];
}

void os_clock_set     ( const OS_CLOCK_t *clock )
{
    OS_ASSERT( clock != NULL );
    sysclock.tick[0] = clock->tick[0];
    sysclock.tick[1] = clock->tick[1];
}

/* Private function implementations ------------------------------------------*/
void __os_clock_init( void )
{
    sysclock.tick[0] = 0;
    sysclock.tick[1] = 0;
    prev_systick = 0;
    os_systick = 0;
}

os_uint8_t __os_clock_update( void )
{
    os_uint8_t curr_systick;
    os_uint8_t delta_systick = 0;
    
    OS_ENTER_CRITICAL();
    curr_systick = os_systick;
    OS_EXIT_CRITICAL();

    if( curr_systick != prev_systick )
    {
        delta_systick = ( curr_systick > prev_systick ) ? ( curr_systick - prev_systick ) : ( UINT8_MAX - prev_systick + curr_systick );
        prev_systick = curr_systick;

        if( (UINT32_MAX - sysclock.tick[0]) < (os_uint32_t)delta_systick )
            sysclock.tick[1]++;
        sysclock.tick[0] += delta_systick;
    }
    return delta_systick;
}

#endif /* (OS_CLOCK_EN > 0) */
/****** (C) COPYRIGHT 2021 PEOS Development Team. *****END OF FILE****/

