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

/* Exported variables --------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
extern __FLASH ST_TASK_t st_task_list[ ST_TASK_MAX ];
#if ( ST_EVENT_MAX == 32 )
extern st_uint32_t st_task_event_list[ST_TASK_MAX];
#elif ( ST_EVENT_MAX == 16 )
extern st_uint16_t st_task_event_list[ST_TASK_MAX];
#elif ( ST_EVENT_MAX == 8 )
extern st_uint8_t  st_task_event_list[ST_TASK_MAX];
#else
#error "ST_EVENT_MAX must be 8, 16 or 32"
#endif

/* Private function prototypes -----------------------------------------------*/
/* Exported function implementations -----------------------------------------*/
void st_task_set_event   ( st_uint8_t task_id, st_uint8_t event_id )
{
#if (ST_EVENT_MAX == 32)
    st_uint32_t event;
#elif (ST_EVENT_MAX == 16)
    st_uint16_t event;
#elif (ST_EVENT_MAX == 8)
    st_uint8_t  event;
#else
#error "ST_EVENT_MAX must be 8, 16 or 32"
#endif
    
    ST_ASSERT( task_id < ST_TASK_MAX );
    ST_ASSERT( event_id < ST_EVENT_MAX );
    
    event = ( 1 << event_id );
    
    ST_ENTER_CRITICAL();
    st_task_event_list[task_id] |= event;
    ST_EXIT_CRITICAL();
}

void st_task_clr_event   ( st_uint8_t task_id, st_uint8_t event_id )
{
#if (ST_EVENT_MAX == 32)
    st_uint32_t event;
#elif (ST_EVENT_MAX == 16)
    st_uint16_t event;
#elif (ST_EVENT_MAX == 8)
    st_uint8_t  event;
#else
#error "ST_EVENT_MAX must be 8, 16 or 32"
#endif
    
    ST_ASSERT( task_id < ST_TASK_MAX );
    ST_ASSERT( event_id < ST_EVENT_MAX );
    
    event = ( 1 << event_id );
    event = ~event;
    
    ST_ENTER_CRITICAL();
    st_task_event_list[task_id] &= event;
    ST_EXIT_CRITICAL();
}

/* Private function implementations ------------------------------------------*/

/****** (C) COPYRIGHT 2019 Single-Thread Development Team. *****END OF FILE****/
