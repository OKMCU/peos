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

#ifdef ST_CLOCK_EN
/* Exported variables --------------------------------------------------------*/
st_uint8_t st_systick;
/* Private define ------------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static ST_CLOCK_t sysclock;
static st_uint8_t prev_systick;

/* Private function prototypes -----------------------------------------------*/
void __st_clock_init( void );
st_uint8_t __st_clock_update( void );

/* Exported function implementations -----------------------------------------*/
void st_clock_get     ( ST_CLOCK_t *clock )
{
    ST_ASSERT( clock != NULL );
    clock->tick[0] = sysclock.tick[0];
    clock->tick[1] = sysclock.tick[1];
}

void st_clock_set     ( const ST_CLOCK_t *clock )
{
    ST_ASSERT( clock != NULL );
    sysclock.tick[0] = clock->tick[0];
    sysclock.tick[1] = clock->tick[1];
}

/* Private function implementations ------------------------------------------*/
void __st_clock_init( void )
{
    sysclock.tick[0] = 0;
    sysclock.tick[1] = 0;
    prev_systick = 0;
    st_systick = 0;
}

st_uint8_t __st_clock_update( void )
{
    st_uint8_t curr_systick;
    st_uint8_t delta_systick = 0;
    
    ST_ENTER_CRITICAL();
    curr_systick = st_systick;
    ST_EXIT_CRITICAL();

    if( curr_systick != prev_systick )
    {
        delta_systick = ( curr_systick > prev_systick ) ? ( curr_systick - prev_systick ) : ( UINT8_MAX - prev_systick + curr_systick );
        prev_systick = curr_systick;

        if( (UINT32_MAX - sysclock.tick[0]) < (st_uint32_t)delta_systick )
            sysclock.tick[1]++;
        sysclock.tick[0] += delta_systick;
    }
    return delta_systick;
}

#endif /* (ST_CLOCK_EN > 0) */
/****** (C) COPYRIGHT 2019 Single-Thread Development Team. *****END OF FILE****/

