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
/* LL drivers common to all LL examples */
#include "stm32f3xx_ll_bus.h"
#include "stm32f3xx_ll_rcc.h"
#include "stm32f3xx_ll_system.h"
#include "stm32f3xx_ll_utils.h"
#include "stm32f3xx_ll_pwr.h"
#include "stm32f3xx_ll_exti.h"
#include "stm32f3xx_ll_gpio.h"
/* LL drivers specific to LL examples IPs */
#include "stm32f3xx_ll_adc.h"
#include "stm32f3xx_ll_comp.h"
#include "stm32f3xx_ll_cortex.h"
#include "stm32f3xx_ll_crc.h"
#include "stm32f3xx_ll_dac.h"
#include "stm32f3xx_ll_dma.h"
#include "stm32f3xx_ll_hrtim.h"
#include "stm32f3xx_ll_i2c.h"
#include "stm32f3xx_ll_iwdg.h"
#include "stm32f3xx_ll_opamp.h"
#include "stm32f3xx_ll_rtc.h"
#include "stm32f3xx_ll_spi.h"
#include "stm32f3xx_ll_tim.h"
#include "stm32f3xx_ll_usart.h"
#include "stm32f3xx_ll_wwdg.h"

#include "st.h"

/* Exported variables --------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Exported function implementations -----------------------------------------*/
static void SystemClock_Config( void );

#ifdef ST_ASSERT_EN
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void st_assert_failed(char *file, st_uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif // (ST_ASSERT_EN > 0)

void st_idle_hook( void )
{
    // go into sleep or low power mode
}


#ifdef ST_CLOCK_EN
/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void);
void SysTick_Handler(void)
{
    extern st_uint8_t st_systick;
    st_systick++;
}
#endif // (ST_TIMER_EN > 0)

void st_hal_init( void )
{
    SystemClock_Config();
    
 #ifdef ST_CLOCK_EN
    SysTick_Config( 64000 );
 #endif

    LL_AHB1_GRP1_EnableClock( LL_AHB1_GRP1_PERIPH_GPIOA );
    LL_AHB1_GRP1_EnableClock( LL_AHB1_GRP1_PERIPH_GPIOB );
    LL_AHB1_GRP1_EnableClock( LL_AHB1_GRP1_PERIPH_GPIOC );
    LL_AHB1_GRP1_EnableClock( LL_AHB1_GRP1_PERIPH_GPIOD );
    LL_AHB1_GRP1_EnableClock( LL_AHB1_GRP1_PERIPH_GPIOE );
    LL_AHB1_GRP1_EnableClock( LL_AHB1_GRP1_PERIPH_GPIOF );
    LL_APB2_GRP1_EnableClock( LL_APB2_GRP1_PERIPH_SYSCFG );
}

/* Private function implementations ------------------------------------------*/
/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follow : 
  *            System Clock source            = PLL (HSI)
  *            SYSCLK(Hz)                     = 64000000
  *            HCLK(Hz)                       = 64000000
  *            AHB Prescaler                  = 1
  *            APB1 Prescaler                 = 2
  *            APB2 Prescaler                 = 1
  *            PLLMUL                         = 16
  *            Flash Latency(WS)              = 2
  * @param  None
  * @retval None
  */
static void SystemClock_Config( void )
{
    /* Set FLASH latency */ 
    LL_FLASH_SetLatency(LL_FLASH_LATENCY_2);

    /* Enable HSI if not already activated*/
    if (LL_RCC_HSI_IsReady() == 0)
    {
        /* Enable HSI and wait for activation*/
        LL_RCC_HSI_Enable(); 
        while(LL_RCC_HSI_IsReady() != 1)
        {
        };
    }

    /* Enable LSI */
    LL_RCC_LSI_Enable();
    while( !LL_RCC_LSI_IsReady() );

    /* Main PLL configuration and activation */
    LL_RCC_PLL_ConfigDomain_SYS(LL_RCC_PLLSOURCE_HSI_DIV_2, LL_RCC_PLL_MUL_16);

    LL_RCC_PLL_Enable();
    while(LL_RCC_PLL_IsReady() != 1) 
    {
    };

    /* Sysclk activation on the main PLL */
    LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);
    LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_PLL);
    while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_PLL)
    {
    };

    /* Set APB1 & APB2 prescaler*/
    LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_2);
    LL_RCC_SetAPB2Prescaler(LL_RCC_APB2_DIV_1);

    /* Set systick to 1ms in using frequency set to 64MHz */
    /* This frequency can be calculated through LL RCC macro */
    /* ex: __LL_RCC_CALC_PLLCLK_FREQ ((HSI_VALUE / 2), LL_RCC_PLL_MUL_16) */
    //LL_Init1msTick(64000000);
    //SysTick_Config( 64000 );

    /* Update CMSIS variable (which can be updated also through SystemCoreClockUpdate function) */
    //LL_SetSystemCoreClock(64000000);
}

/* integrity check of type sizes */
ST_ASSERT_SIZE(  st_int8_t, 1);
ST_ASSERT_SIZE(st_uint8_t, 1);
ST_ASSERT_SIZE(st_int16_t, 2);
ST_ASSERT_SIZE(st_uint16_t, 2);
ST_ASSERT_SIZE(st_int32_t, 4);
ST_ASSERT_SIZE(st_uint32_t, 4);

/****** (C) COPYRIGHT 2019 Single-Thread Development Team. *****END OF FILE****/
