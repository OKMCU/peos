/**
  ******************************************************************************
  * @file    
  * @author  
  * @brief   
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2019 Cloud Light Technology Ltd.</center></h2>
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "st.h"
#include "hal.h"
#include "components/led/led.h"
#include "application/demo.h"

/* Exported variables --------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

__FLASH ST_TASK_t st_task_list[ST_TASK_MAX] = {
    { drivers_init, drivers_task },
    { led_init, led_task },
    { demo_init, demo_task },
};


/**
  * @}
  */

/**
  * @}
  */
/********** (C) COPYRIGHT 2019 Cloud Light Technology Ltd. *****END OF FILE****/

