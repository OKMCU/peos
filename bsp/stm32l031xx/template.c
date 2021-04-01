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
#include "os.h"

/* Exported variables --------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
static void private_function( void );
/* Exported function implementations -----------------------------------------*/

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @note   None
  * @retval None
  */
void exported_function(char *file, os_uint32_t line)
{
    /* Infinite loop */
    while (1)
    {
    }
}

/* Private function implementations ------------------------------------------*/
static void private_function( void )
{
    
}

/****** (C) COPYRIGHT 2019 Single-Thread Development Team. *****END OF FILE****/
