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
#include "stm32f3xx_ll_gpio.h"
#include "st.h"
#include "hal.h"

/* Exported variables --------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
typedef struct {
    GPIO_TypeDef * port;
    uint32_t pin;
} PIN_t;

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
__FLASH static PIN_t pin_list[] = {
    { GPIOA, LL_GPIO_PIN_0 },
    { GPIOA, LL_GPIO_PIN_1 },
    { GPIOA, LL_GPIO_PIN_2 },
    { GPIOA, LL_GPIO_PIN_3 },
    { GPIOA, LL_GPIO_PIN_4 },
    { GPIOA, LL_GPIO_PIN_5 },
    { GPIOA, LL_GPIO_PIN_6 },
    { GPIOA, LL_GPIO_PIN_7 },
    { GPIOA, LL_GPIO_PIN_8 },
    { GPIOA, LL_GPIO_PIN_9 },
    { GPIOA, LL_GPIO_PIN_10 },
    { GPIOA, LL_GPIO_PIN_11 },
    { GPIOA, LL_GPIO_PIN_12 },
    { GPIOA, LL_GPIO_PIN_13 },
    { GPIOA, LL_GPIO_PIN_14 },
    { GPIOA, LL_GPIO_PIN_15 },

    { GPIOB, LL_GPIO_PIN_0 },
    { GPIOB, LL_GPIO_PIN_1 },
    { GPIOB, LL_GPIO_PIN_2 },
    { GPIOB, LL_GPIO_PIN_3 },
    { GPIOB, LL_GPIO_PIN_4 },
    { GPIOB, LL_GPIO_PIN_5 },
    { GPIOB, LL_GPIO_PIN_6 },
    { GPIOB, LL_GPIO_PIN_7 },
    { GPIOB, LL_GPIO_PIN_8 },
    { GPIOB, LL_GPIO_PIN_9 },
    { GPIOB, LL_GPIO_PIN_10 },
    { GPIOB, LL_GPIO_PIN_11 },
    { GPIOB, LL_GPIO_PIN_12 },
    { GPIOB, LL_GPIO_PIN_13 },
    { GPIOB, LL_GPIO_PIN_14 },
    { GPIOB, LL_GPIO_PIN_15 },

    { GPIOC, LL_GPIO_PIN_0 },
    { GPIOC, LL_GPIO_PIN_1 },
    { GPIOC, LL_GPIO_PIN_2 },
    { GPIOC, LL_GPIO_PIN_3 },
    { GPIOC, LL_GPIO_PIN_4 },
    { GPIOC, LL_GPIO_PIN_5 },
    { GPIOC, LL_GPIO_PIN_6 },
    { GPIOC, LL_GPIO_PIN_7 },
    { GPIOC, LL_GPIO_PIN_8 },
    { GPIOC, LL_GPIO_PIN_9 },
    { GPIOC, LL_GPIO_PIN_10 },
    { GPIOC, LL_GPIO_PIN_11 },
    { GPIOC, LL_GPIO_PIN_12 },
    { GPIOC, LL_GPIO_PIN_13 },
    { GPIOC, LL_GPIO_PIN_14 },
    { GPIOC, LL_GPIO_PIN_15 },

    { GPIOD, LL_GPIO_PIN_0 },
    { GPIOD, LL_GPIO_PIN_1 },
    { GPIOD, LL_GPIO_PIN_2 },
    { GPIOD, LL_GPIO_PIN_3 },
    { GPIOD, LL_GPIO_PIN_4 },
    { GPIOD, LL_GPIO_PIN_5 },
    { GPIOD, LL_GPIO_PIN_6 },
    { GPIOD, LL_GPIO_PIN_7 },
    { GPIOD, LL_GPIO_PIN_8 },
    { GPIOD, LL_GPIO_PIN_9 },
    { GPIOD, LL_GPIO_PIN_10 },
    { GPIOD, LL_GPIO_PIN_11 },
    { GPIOD, LL_GPIO_PIN_12 },
    { GPIOD, LL_GPIO_PIN_13 },
    { GPIOD, LL_GPIO_PIN_14 },
    { GPIOD, LL_GPIO_PIN_15 },

    { GPIOE, LL_GPIO_PIN_0 },
    { GPIOE, LL_GPIO_PIN_1 },
    { GPIOE, LL_GPIO_PIN_2 },
    { GPIOE, LL_GPIO_PIN_3 },
    { GPIOE, LL_GPIO_PIN_4 },
    { GPIOE, LL_GPIO_PIN_5 },
    { GPIOE, LL_GPIO_PIN_6 },
    { GPIOE, LL_GPIO_PIN_7 },
    { GPIOE, LL_GPIO_PIN_8 },
    { GPIOE, LL_GPIO_PIN_9 },
    { GPIOE, LL_GPIO_PIN_10 },
    { GPIOE, LL_GPIO_PIN_11 },
    { GPIOE, LL_GPIO_PIN_12 },
    { GPIOE, LL_GPIO_PIN_13 },
    { GPIOE, LL_GPIO_PIN_14 },
    { GPIOE, LL_GPIO_PIN_15 },

    { GPIOF, LL_GPIO_PIN_0 },
    { GPIOF, LL_GPIO_PIN_1 },
    { GPIOF, LL_GPIO_PIN_2 },
    { GPIOF, LL_GPIO_PIN_3 },
    { GPIOF, LL_GPIO_PIN_4 },
    { GPIOF, LL_GPIO_PIN_5 },
    { GPIOF, LL_GPIO_PIN_6 },
    { GPIOF, LL_GPIO_PIN_7 },
    { GPIOF, LL_GPIO_PIN_8 },
    { GPIOF, LL_GPIO_PIN_9 },
    { GPIOF, LL_GPIO_PIN_10 },
    { GPIOF, LL_GPIO_PIN_11 },
    { GPIOF, LL_GPIO_PIN_12 },
    { GPIOF, LL_GPIO_PIN_13 },
    { GPIOF, LL_GPIO_PIN_14 },
    { GPIOF, LL_GPIO_PIN_15 },
};

/* Private function prototypes -----------------------------------------------*/
/* Exported function implementations -----------------------------------------*/
extern void hal_pin_mode( st_uint8_t pin, st_uint8_t mode )
{

    ST_ASSERT( pin < sizeof(pin_list)/sizeof(pin_list[0]) );

    switch( mode )
    {
        case HAL_PIN_MODE_OUTPUT:
            LL_GPIO_SetPinPull( pin_list[pin].port, pin_list[pin].pin, LL_GPIO_PULL_NO );
            LL_GPIO_SetPinMode( pin_list[pin].port, pin_list[pin].pin, LL_GPIO_MODE_OUTPUT );
            LL_GPIO_SetPinOutputType( pin_list[pin].port, pin_list[pin].pin, LL_GPIO_OUTPUT_PUSHPULL );
        break;

        
        case HAL_PIN_MODE_INPUT:
            LL_GPIO_SetPinPull( pin_list[pin].port, pin_list[pin].pin, LL_GPIO_PULL_NO );
            LL_GPIO_SetPinMode( pin_list[pin].port, pin_list[pin].pin, LL_GPIO_MODE_INPUT );
        break;
        
        case HAL_PIN_MODE_INPUT_PU:
            LL_GPIO_SetPinPull( pin_list[pin].port, pin_list[pin].pin, LL_GPIO_PULL_UP );
            LL_GPIO_SetPinMode( pin_list[pin].port, pin_list[pin].pin, LL_GPIO_MODE_INPUT );
        break;
        case HAL_PIN_MODE_INPUT_PD:
            LL_GPIO_SetPinPull( pin_list[pin].port, pin_list[pin].pin, LL_GPIO_PULL_DOWN );
            LL_GPIO_SetPinMode( pin_list[pin].port, pin_list[pin].pin, LL_GPIO_MODE_INPUT );
        break;
        
        case HAL_PIN_MODE_OUTPUT_OD:
            LL_GPIO_SetPinPull( pin_list[pin].port, pin_list[pin].pin, LL_GPIO_PULL_NO );
            LL_GPIO_SetPinMode( pin_list[pin].port, pin_list[pin].pin, LL_GPIO_MODE_OUTPUT );
            LL_GPIO_SetPinOutputType( pin_list[pin].port, pin_list[pin].pin, LL_GPIO_OUTPUT_OPENDRAIN );
        break;

        case HAL_PIN_MODE_QUASI_PU:
            LL_GPIO_SetPinPull( pin_list[pin].port, pin_list[pin].pin, LL_GPIO_PULL_UP );
            LL_GPIO_SetPinMode( pin_list[pin].port, pin_list[pin].pin, LL_GPIO_MODE_OUTPUT );
            LL_GPIO_SetPinOutputType( pin_list[pin].port, pin_list[pin].pin, LL_GPIO_OUTPUT_OPENDRAIN );
        break;
        
        case HAL_PIN_MODE_ANALOG:
            LL_GPIO_SetPinPull( pin_list[pin].port, pin_list[pin].pin, LL_GPIO_PULL_NO );
            LL_GPIO_SetPinMode( pin_list[pin].port, pin_list[pin].pin, LL_GPIO_MODE_ANALOG );
        break;
        
        default:
            ST_ASSERT_FORCED();
        break;
    }
}

extern void hal_pin_write( st_uint8_t pin, st_uint8_t value )
{
    ST_ASSERT( pin < sizeof(pin_list)/sizeof(pin_list[0]) );
    if( value )
        LL_GPIO_SetOutputPin( pin_list[pin].port, pin_list[pin].pin );
    else
        LL_GPIO_ResetOutputPin( pin_list[pin].port, pin_list[pin].pin );
}

extern void hal_pin_toggle( st_uint8_t pin )
{
    ST_ASSERT( pin < sizeof(pin_list)/sizeof(pin_list[0]) );
    LL_GPIO_TogglePin( pin_list[pin].port, pin_list[pin].pin );
}

extern st_uint8_t hal_pin_read( st_uint8_t pin )
{
    ST_ASSERT( pin < sizeof(pin_list)/sizeof(pin_list[0]) );
    return (st_uint8_t)LL_GPIO_IsInputPinSet( pin_list[pin].port, pin_list[pin].pin );
}

/* Private function implementations ------------------------------------------*/

/****** (C) COPYRIGHT 2019 Single-Thread Development Team. *****END OF FILE****/
