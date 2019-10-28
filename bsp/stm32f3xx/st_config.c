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
#include "hal.h"
#include "components/led/led.h"
#include "application/demo.h"

/* Exported variables --------------------------------------------------------*/
__FLASH ST_TASK_t st_task_list[ST_TASK_MAX] = {
    { drivers_init, drivers_task, "driver" },
    { led_init, led_task, "led" },
    { demo_init, demo_task, "demo" },
};

/* Private define ------------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Exported function implementations -----------------------------------------*/
/* Private function implementations ------------------------------------------*/



/****** (C) COPYRIGHT 2019 Single-Thread Development Team. *****END OF FILE****/
