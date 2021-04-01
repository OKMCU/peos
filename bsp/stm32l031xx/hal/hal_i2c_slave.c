/*******************************************************************************
 * Copyright (c) 2021-2022, PEOS Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 * 
 * Change Logs:
 * Date         Author       Notes
 * 2021-10-30   Wentao SUN   first version
 *
 ******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "stm32f3xx_ll_bus.h"
#include "stm32f3xx_ll_gpio.h"
#include "stm32f3xx_ll_i2c.h"
#include "hal_i2c_slave.h"
#include "components/utilities/rbuf.h"

#ifdef OS_USING_HAL_I2C_SLAVE
/* Exported variables --------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define I2CS0_RX_CACHE_SIZE         8
#define I2CS0_TX_CACHE_SIZE         8
#define I2CS1_RX_CACHE_SIZE         8
#define I2CS1_TX_CACHE_SIZE         8

#define TASK_EVT_I2CS_ADDRW         0
#define TASK_EVT_I2CS_RXNE          1
#define TASK_EVT_I2CS_ADDRR         2
#define TASK_EVT_I2CS_TXE           3
#define TASK_EVT_I2CS_NACK          4
#define TASK_EVT_I2CS_STOP          5

/* Private typedef -----------------------------------------------------------*/
typedef struct {
    os_uint8_t *rx_cache;
    os_uint8_t *tx_cache;
    os_uint8_t rx_cache_size;
    os_uint8_t tx_cache_size;
} i2cs_cache_t;

typedef struct {
    void (*callback)( os_uint8_t event );
    os_uint8_t rx_head;
    os_uint8_t rx_tail;
    os_uint8_t tx_head;
    os_uint8_t tx_tail;
} i2cs_ctrl_t;

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static os_uint8_t i2cs0_rx_cache[I2CS0_RX_CACHE_SIZE];
static os_uint8_t i2cs0_tx_cache[I2CS0_TX_CACHE_SIZE];
static os_uint8_t i2cs1_rx_cache[I2CS1_RX_CACHE_SIZE];
static os_uint8_t i2cs1_tx_cache[I2CS1_TX_CACHE_SIZE];
static os_uint8_t task_id_i2cs[HAL_I2CS_ADDR_MAX];

static i2cs_cache_t const i2cs_cache[HAL_I2CS_PORT_MAX] = {
    { 
        .rx_cache = i2cs0_rx_cache, 
        .tx_cache = i2cs0_tx_cache, 
        .rx_cache_size = sizeof(i2cs0_rx_cache), 
        .tx_cache_size = sizeof(i2cs0_tx_cache),
    },
    { 
        .rx_cache = i2cs1_rx_cache, 
        .tx_cache = i2cs1_tx_cache, 
        .rx_cache_size = sizeof(i2cs1_rx_cache), 
        .tx_cache_size = sizeof(i2cs1_tx_cache),
    },
};

static I2C_TypeDef* const I2Cx[HAL_I2CS_PORT_MAX] = {
    I2C1,
    I2C2
};

static i2cs_ctrl_t i2cs_ctrl[HAL_UART_PORT_MAX] = { 0 };

/* Private function prototypes -----------------------------------------------*/
static void hal_i2cs_portx_init( os_uint8_t port, os_uint8_t task_id );
static void hal_i2cs_portx_task( os_uint8_t port, os_int8_t event_id );
static void hal_i2cs_isr( os_uint8_t port );

extern void I2C0_EV_IRQHandler( void );
extern void I2C1_EV_IRQHandler( void );
extern void I2C0_ER_IRQHandler( void );
extern void I2C1_ER_IRQHandler( void );

/* Exported function implementations -----------------------------------------*/
void hal_i2cs_port0_init( os_uint8_t task_id )
{
    hal_i2cs_portx_init( HAL_I2CS_PORT_0 );
}

void hal_i2cs_port1_init( os_uint8_t task_id )
{
    hal_i2cs_portx_init( HAL_I2CS_PORT_1, task_id );
}

void hal_i2cs_port0_task( os_int8_t event_id )
{
    hal_i2cs_portx_task( HAL_I2CS_PORT_0, event_id );
}

void hal_i2cs_port1_task( os_int8_t event_id )
{
    hal_i2cs_portx_task( HAL_I2CS_PORT_1, event_id );
}

/**
  * @brief  Function brief
  * @param  param1
  * @param  param2
  * @note   None
  * @retval None
  */
void hal_i2cs_config( os_uint8_t port, const hal_i2cs_config_t *cfg )
{
    OS_ASSERT( port < HAL_I2CS_PORT_MAX );
    OS_ASSERT( cfg != NULL );
    OS_ASSERT( !LL_I2C_IsEnabled(I2Cx[port]) );
    
    // reset peripherals firstly
    switch ( port )
    {
        case HAL_I2CS_PORT_0:
            LL_RCC_SetI2CClockSource( LL_RCC_I2C1_CLKSOURCE_SYSCLK );
            LL_APB1_GRP1_EnableClock( LL_APB1_GRP1_PERIPH_I2C1 );
            LL_APB1_GRP1_ForceReset( LL_APB1_GRP1_PERIPH_I2C1 );
            LL_APB1_GRP1_ReleaseReset( LL_APB1_GRP1_PERIPH_I2C1 );
            
            LL_GPIO_SetPinMode( GPIOB, LL_GPIO_PIN_6, LL_GPIO_MODE_ALTERNATE ); // PB6: I2C1_SCL
            LL_GPIO_SetPinMode( GPIOB, LL_GPIO_PIN_7, LL_GPIO_MODE_ALTERNATE ); // PB7: I2C1_SDA
            LL_GPIO_SetPinPull( GPIOB, LL_GPIO_PIN_6, LL_GPIO_PULL_NO );        // Disable pull up and pull down resistor
            LL_GPIO_SetPinPull( GPIOB, LL_GPIO_PIN_7, LL_GPIO_PULL_NO );        // Disable pull up and pull down resistor
            LL_GPIO_SetAFPin_0_7( GPIOB , LL_GPIO_PIN_6, LL_GPIO_AF_4 );           // Enable alternate function I2C1_SCL on this pin
            LL_GPIO_SetAFPin_0_7( GPIOB , LL_GPIO_PIN_7, LL_GPIO_AF_4 );           // Enable alternate function I2C1_SDA on this pin
            
            NVIC_EnableIRQ(I2C1_EV_IRQn);
            NVIC_EnableIRQ(I2C1_ER_IRQn);
        break;
        
        case HAL_I2CS_PORT_1:
            LL_RCC_SetI2CClockSource( LL_RCC_I2C2_CLKSOURCE_SYSCLK );
            LL_APB1_GRP1_EnableClock( LL_APB1_GRP1_PERIPH_I2C2 );
            LL_APB1_GRP1_ForceReset( LL_APB1_GRP1_PERIPH_I2C2 );
            LL_APB1_GRP1_ReleaseReset( LL_APB1_GRP1_PERIPH_I2C2 );
            
            LL_GPIO_SetPinMode( GPIOF, LL_GPIO_PIN_1, LL_GPIO_MODE_ALTERNATE ); // PF1: I2C1_SCL
            LL_GPIO_SetPinMode( GPIOF, LL_GPIO_PIN_0, LL_GPIO_MODE_ALTERNATE ); // PF0: I2C1_SDA
            LL_GPIO_SetPinPull( GPIOF, LL_GPIO_PIN_1, LL_GPIO_PULL_NO );        // Disable pull up and pull down resistor
            LL_GPIO_SetPinPull( GPIOF, LL_GPIO_PIN_0, LL_GPIO_PULL_NO );        // Disable pull up and pull down resistor
            LL_GPIO_SetAFPin_0_7( GPIOF , LL_GPIO_PIN_1, LL_GPIO_AF_4 );           // Enable alternate function I2C1_SCL on this pin
            LL_GPIO_SetAFPin_0_7( GPIOF , LL_GPIO_PIN_0, LL_GPIO_AF_4 );           // Enable alternate function I2C1_SDA on this pin
            
            NVIC_EnableIRQ(I2C2_EV_IRQn);
            NVIC_EnableIRQ(I2C2_ER_IRQn);
        break;
    }
    
    I2Cx[port]->CR1 = I2C_CR1_ERRIE  | 
                      I2C_CR1_TCIE   | 
                      I2C_CR1_STOPIE |
                      I2C_CR1_NACKIE |
                      I2C_CR1_ADDRIE |
                      I2C_CR1_TXIE   |
                      I2C_CR1_RXIE   |
                      I2C_CR1_DNF;
    if( cfg->addr[0] )
        I2Cx[port]->OAR1 = (cfg->addr[0]&0xFE) | I2C_OAR1_OA1EN;
    if( cfg->addr[1] )
        I2Cx[port]->OAR2 = (cfg->addr[1]&0xFE) | I2C_OAR2_OA2EN;
    
    //Config I2C_TIMING
    I2Cx[port]->TIMINGR = 0x00200000;     // tHD.DAT = 0, tSU.DAT = 0.1us
    //Config I2C_TIMEOUTR
    I2Cx[port]->TIMEOUTR = 0;
    
    // init uart control body info
    os_memset( &i2cs_ctrl[port], 0, sizeof(i2cs_ctrl_t) );
    i2cs_ctrl[port].callback = cfg->callback;
}

void hal_i2cs_open( os_uint8_t port )
{
    OS_ASSERT( port < HAL_I2CS_PORT_MAX );
    OS_ASSERT( !LL_I2C_IsEnabled(I2Cx[port]) );
    LL_I2C_Enable( I2Cx[port] );
}

/**
  * @brief  Function brief
  * @param  param1
  * @param  param2
  * @note   None
  * @retval None
  */
void hal_uart_putc( os_uint8_t port, os_uint8_t byte )
{
    OS_ASSERT( port < HAL_UART_PORT_MAX );
    OS_ASSERT( LL_USART_IsEnabled(USARTx[port]) );
    
    while( RING_BUF_FULL(uart_ctrl[port].tx_head, 
                         uart_ctrl[port].tx_tail, 
                         uart_cache[port].tx_cache_size) );

    if( RING_BUF_EMPTY(uart_ctrl[port].tx_head, uart_ctrl[port].tx_tail) &&
        LL_USART_IsActiveFlag_TXE(USARTx[port]) )
    {
        LL_USART_TransmitData8( USARTx[port], byte );
        LL_USART_EnableIT_TXE( USARTx[port] );
    }
    else
    {
        LL_USART_DisableIT_TXE( USARTx[port] );
        RING_BUF_PUT( byte,
                      uart_ctrl[port].tx_head, 
                      uart_cache[port].tx_cache, 
                      uart_cache[port].tx_cache_size );
        LL_USART_EnableIT_TXE( USARTx[port] );
    }
}

/**
  * @brief  Function brief
  * @param  param1
  * @param  param2
  * @note   None
  * @retval None
  */
os_uint8_t hal_uart_getc( os_uint8_t port )
{
    os_uint8_t byte;
    
    OS_ASSERT( port < HAL_UART_PORT_MAX );
    OS_ASSERT( LL_USART_IsEnabled(USARTx[port]) );

    while( RING_BUF_EMPTY(uart_ctrl[port].rx_head, 
                          uart_ctrl[port].rx_tail) );

    LL_USART_DisableIT_RXNE( USARTx[port] );
    RING_BUF_GET( &byte, 
                  uart_ctrl[port].rx_tail, 
                  uart_cache[port].rx_cache, 
                  uart_cache[port].rx_cache_size );
    LL_USART_EnableIT_RXNE( USARTx[port] );

    return byte;
}

os_uint8_t hal_uart_tx_buf_free( os_uint8_t port )
{
    OS_ASSERT( port < HAL_UART_PORT_MAX );
    return RING_BUF_FREE_SIZE( uart_ctrl[port].tx_head, 
                               uart_ctrl[port].tx_tail, 
                               uart_cache[port].tx_cache_size );
}

os_uint8_t hal_uart_rx_buf_used( os_uint8_t port )
{
    OS_ASSERT( port < HAL_UART_PORT_MAX );
    return RING_BUF_USED_SIZE( uart_ctrl[port].rx_head, 
                               uart_ctrl[port].rx_tail, 
                               uart_cache[port].rx_cache_size );
}

/**
  * @brief  Function brief
  * @param  param1
  * @param  param2
  * @note   None
  * @retval None
  */
void hal_i2cs_close( os_uint8_t port )
{
    OS_ASSERT( port < HAL_I2CS_PORT_MAX );
    OS_ASSERT( LL_I2C_IsEnabled(I2Cx[port]) );

    LL_I2C_Disable( I2Cx[port] );

    switch ( port )
    {
        case HAL_UART_PORT_0:
            NVIC_DisableIRQ( USART1_IRQn );
            LL_GPIO_SetPinMode( GPIOE, LL_GPIO_PIN_0, LL_GPIO_MODE_ANALOG ); // PE0: USART1_TX
            LL_GPIO_SetPinMode( GPIOE, LL_GPIO_PIN_1, LL_GPIO_MODE_ANALOG ); // PE1: USART1_RX
            LL_GPIO_SetPinPull( GPIOE, LL_GPIO_PIN_0, LL_GPIO_PULL_NO );
            LL_GPIO_SetPinPull( GPIOE, LL_GPIO_PIN_1, LL_GPIO_PULL_NO );
            LL_APB2_GRP1_DisableClock( LL_APB2_GRP1_PERIPH_USART1 );
        break;

        case HAL_UART_PORT_1:
            NVIC_DisableIRQ( USART2_IRQn );
            LL_GPIO_SetPinMode( GPIOA, LL_GPIO_PIN_2, LL_GPIO_MODE_ANALOG ); // PA2: USART2_TX
            LL_GPIO_SetPinMode( GPIOA, LL_GPIO_PIN_3, LL_GPIO_MODE_ANALOG ); // PA3: USART2_RX
            LL_GPIO_SetPinPull( GPIOA, LL_GPIO_PIN_2, LL_GPIO_PULL_NO );
            LL_GPIO_SetPinPull( GPIOA, LL_GPIO_PIN_3, LL_GPIO_PULL_NO );
            LL_APB1_GRP1_DisableClock( LL_APB1_GRP1_PERIPH_USART2 );
        break;
    }
}

/* Private function implementations ------------------------------------------*/
static void hal_i2cs_portx_init( os_uint8_t port, os_uint8_t task_id )
{
    task_id_i2cs[port] = task_id;
}

static void hal_i2cs_portx_task( os_uint8_t port, os_int8_t event_id )
{
    os_uint8_t const event_mapping[] = {
        HAL_I2CS_EVENT_ADDRW,
        HAL_I2CS_EVENT_RXNE,
        HAL_I2CS_EVENT_ADDRR,
        HAL_I2CS_EVENT_TXE,
        HAL_I2CS_EVENT_NACK,
        HAL_I2CS_EVENT_STOP,
    };
    
    OS_ASSERT( event_id < sizeof(event_mapping)/sizeof(event_mapping[0]) );
    if( i2cs_ctrl[port].callback )
        i2cs_ctrl[port].callback( event_mapping[event_id] );
}

/**
  * @brief  Function brief
  * @param  param1
  * @param  param2
  * @note   None
  * @retval None
  */
static void hal_uart_isr( os_uint8_t port )
{
    os_uint8_t byte;

    OS_ASSERT( port < HAL_UART_PORT_MAX );
    
    if( LL_USART_IsActiveFlag_RXNE(USARTx[port]) )
    {
        byte = LL_USART_ReceiveData8( USARTx[port] );
        if( RING_BUF_FULL(uart_ctrl[port].rx_head, 
                          uart_ctrl[port].rx_tail, 
                          uart_cache[port].rx_cache_size) )
        {
            os_task_set_event( task_id_rxd, uart_event[port].ovf );
        }
        else
        {
            RING_BUF_PUT( byte, 
                          uart_ctrl[port].rx_head, 
                          uart_cache[port].rx_cache, 
                          uart_cache[port].rx_cache_size );
            os_task_set_event( task_id_rxd, uart_event[port].rxd );
        }
        return;
    }

    if( LL_USART_IsActiveFlag_PE(USARTx[port]) )
    {
        os_task_set_event( task_id_rxd, uart_event[port].perr );
        LL_USART_ClearFlag_PE( USARTx[port] );
        return;
    }

    if( LL_USART_IsActiveFlag_IDLE( USARTx[port]) )
    {
        os_task_set_event( task_id_rxd, uart_event[port].idle );
        LL_USART_ClearFlag_IDLE( USARTx[port] );
        return;
    }
    
    if( LL_USART_IsActiveFlag_TXE(USARTx[port]) )
    {
        if( RING_BUF_EMPTY(uart_ctrl[port].tx_head, uart_ctrl[port].tx_tail) )
        {
            LL_USART_DisableIT_TXE( USARTx[port] );
        }
        else
        {
            RING_BUF_GET( &byte, 
                          uart_ctrl[port].tx_tail, 
                          uart_cache[port].tx_cache, 
                          uart_cache[port].tx_cache_size );
            LL_USART_TransmitData8( USARTx[port], byte );
            os_task_set_event( task_id_txd, uart_event[port].txd );
        }
        return;
    }

    
}

void USART1_IRQHandler( void )
{
    hal_uart_isr( HAL_UART_PORT_0 );
}

void USART2_IRQHandler( void )
{
    hal_uart_isr( HAL_UART_PORT_1 );
}
#endif //OS_USING_HAL_UART
/****** (C) COPYRIGHT 2021 PEOS Development Team. *****END OF FILE****/
