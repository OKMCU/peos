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
#include "stm32l0xx_ll_bus.h"
#include "stm32l0xx_ll_rcc.h"
#include "stm32l0xx_ll_system.h"
#include "stm32l0xx_ll_utils.h"
#include "stm32l0xx_ll_pwr.h"
#include "stm32l0xx_ll_exti.h"
#include "stm32l0xx_ll_gpio.h"
/* LL drivers specific to LL examples IPs */
#include "stm32l0xx_ll_adc.h"
#include "stm32l0xx_ll_comp.h"
#include "stm32l0xx_ll_cortex.h"
#include "stm32l0xx_ll_crc.h"
#include "stm32l0xx_ll_dac.h"
#include "stm32l0xx_ll_dma.h"
#include "stm32l0xx_ll_lptim.h"
#include "stm32l0xx_ll_i2c.h"
#include "stm32l0xx_ll_iwdg.h"
#include "stm32l0xx_ll_rtc.h"
#include "stm32l0xx_ll_spi.h"
#include "stm32l0xx_ll_tim.h"
#include "stm32l0xx_ll_usart.h"
#include "stm32l0xx_ll_wwdg.h"
#include "stm32l0xx_ll_crs.h"
#include "stm32l0xx_ll_lpuart.h"
#include "stm32l0xx_ll_rng.h"

#include "st.h"

/* Exported variables --------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
static void SystemClock_Config( void );

/* Exported function implementations -----------------------------------------*/


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

void st_board_init( void )
{
    SystemClock_Config();
    
 #ifdef ST_CLOCK_EN
    SysTick_Config( 32000 );
 #endif
 
    LL_IOP_GRP1_EnableClock( LL_IOP_GRP1_PERIPH_GPIOA );
    LL_IOP_GRP1_EnableClock( LL_IOP_GRP1_PERIPH_GPIOB );
    LL_IOP_GRP1_EnableClock( LL_IOP_GRP1_PERIPH_GPIOC );

    NVIC_SetPriority(USART2_IRQn, 0);
    //NVIC_SetPriority( I2C1_IRQn,      0 ); // Priority 0 (Highest)
    //NVIC_SetPriority( ADC1_COMP_IRQn, 3 ); // Priority 3 (Lowest)
    //NVIC_SetPriority( EXTI4_15_IRQn,  2 ); // Priority 2
    //NVIC_SetPriority( EXTI0_1_IRQn,   2 ); // Priority 2
    //NVIC_SetPriority( EXTI2_3_IRQn,   2 ); // Priority 2

    //NVIC_EnableIRQ( I2C1_IRQn );
    //NVIC_EnableIRQ( ADC1_COMP_IRQn );
    //NVIC_EnableIRQ( EXTI4_15_IRQn );
    //NVIC_EnableIRQ( EXTI0_1_IRQn );
    //NVIC_EnableIRQ( EXTI2_3_IRQn );
}

void st_board_idle( void )
{

}

/* Private function implementations ------------------------------------------*/
/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follow : 
  *            System Clock source            = PLL (HSI)
  *            SYSCLK(Hz)                     = 32000000
  *            HCLK(Hz)                       = 32000000
  *            AHB Prescaler                  = 1
  *            APB1 Prescaler                 = 1
  *            APB2 Prescaler                 = 1
  *            PLLMUL                         = 4
  *            PLLDIV                         = 2
  *            Flash Latency(WS)              = 1
  * @param  None
  * @retval None
  */
static void SystemClock_Config( void )
{
    LL_FLASH_EnablePreRead();
    LL_FLASH_EnablePrefetch();
     
    /* Enable Power Control clock */
    LL_APB1_GRP1_EnableClock( LL_APB1_GRP1_PERIPH_PWR );
    
    /* The voltage scaling allows optimizing the power consumption when the device is 
       clocked below the maximum system frequency, to update the voltage scaling value 
       regarding system frequency refer to product datasheet.  */
    LL_PWR_SetRegulVoltageScaling( LL_PWR_REGU_VOLTAGE_SCALE1 );
    /* Disable Power Control clock */
    LL_APB1_GRP1_DisableClock( LL_APB1_GRP1_PERIPH_PWR );
    
    /* Enable HSI Oscillator */
    LL_RCC_HSI_Enable();
    while( !LL_RCC_HSI_IsReady() );
    
    LL_RCC_HSI_SetCalibTrimming( 16 );
    LL_RCC_PLL_ConfigDomain_SYS( LL_RCC_PLLSOURCE_HSI, LL_RCC_PLL_MUL_4, LL_RCC_PLL_DIV_2 );
    LL_RCC_PLL_Enable();
    while( !LL_RCC_PLL_IsReady() );

    LL_RCC_LSI_Enable();
    while( !LL_RCC_LSI_IsReady() );

    LL_FLASH_SetLatency( LL_FLASH_LATENCY_1 );              // Range 1 (1.65V~1.95V), 32MHz, Flash latency should be set to 1
                                                            // Refer to RM0377, section 3.7.1.
    LL_RCC_SetSysClkSource( LL_RCC_SYS_CLKSOURCE_PLL );     // Use PLL output as SYSCLK
    LL_RCC_SetAHBPrescaler( LL_RCC_SYSCLK_DIV_1 );          // HCLK 32MHz
    LL_RCC_SetAPB1Prescaler( LL_RCC_APB1_DIV_1 );           // APB1 CLK 32MHz
    LL_RCC_SetAPB2Prescaler( LL_RCC_APB2_DIV_1 );           // APB2 CLK 32MHz
}

/* integrity check of type sizes */
ST_ASSERT_SIZE(  st_int8_t, 1);
ST_ASSERT_SIZE(st_uint8_t, 1);
ST_ASSERT_SIZE(st_int16_t, 2);
ST_ASSERT_SIZE(st_uint16_t, 2);
ST_ASSERT_SIZE(st_int32_t, 4);
ST_ASSERT_SIZE(st_uint32_t, 4);

/****** (C) COPYRIGHT 2019 Single-Thread Development Team. *****END OF FILE****/

