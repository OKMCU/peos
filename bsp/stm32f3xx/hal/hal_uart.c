/*******************************************************************************
 * Copyright (c) 2019-2020, Single-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 * 
 * Change Logs:
 * Date         Author       Notes
 * 2019-10-30   Wentao SUN   first version
 *
 ******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "stm32f3xx_ll_gpio.h"
#include "stm32f3xx_ll_usart.h"
#include "hal_drivers.h"
#include "hal_uart.h"
#include "components\fifo\fifo.h"

/* Exported variables --------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define UART_PORT_MAX               2
#define UART0_RX_CACHE_SIZE         8
#define UART0_TX_CACHE_SIZE         8
#define UART1_RX_CACHE_SIZE         8
#define UART1_TX_CACHE_SIZE         8
#define UART0_RX_FIFO_SIZE           128
#define UART0_TX_FIFO_SIZE           128
#define UART1_RX_FIFO_SIZE           128
#define UART1_TX_FIFO_SIZE           128

/* Private typedef -----------------------------------------------------------*/
typedef struct {
    st_uint8_t *rx_cache;
    st_uint8_t *tx_cache;
    st_uint8_t rx_cache_size;
    st_uint8_t tx_cache_size;
    st_uint16_t rx_fifo_size;
    st_uint16_t tx_fifo_size;
} uart_mem_t;

typedef struct {
    void (*rx_indicate)( st_uint8_t port, st_uint16_t size );
    void (*tx_complete)( st_uint8_t port );
    st_uint8_t rx_head;
    st_uint8_t rx_tail;
    st_uint8_t tx_head;
    st_uint8_t tx_tail;
    void *rx_fifo;
    void *tx_fifo;
} uart_ctrl_t;

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static st_uint8_t uart0_rx_cache[UART0_RX_CACHE_SIZE];
static st_uint8_t uart0_tx_cache[UART0_TX_CACHE_SIZE];
static st_uint8_t uart1_rx_cache[UART1_RX_CACHE_SIZE];
static st_uint8_t uart1_tx_cache[UART1_TX_CACHE_SIZE];

static const uart_mem_t uart_mem[UART_PORT_MAX] = {
    { 
        .rx_cache = uart0_rx_cache, 
        .tx_cache = uart0_tx_cache, 
        .rx_cache_size = sizeof(uart0_rx_cache), 
        .tx_cache_size = sizeof(uart0_tx_cache),
        .rx_fifo_size = UART0_RX_FIFO_SIZE,
        .tx_fifo_size = UART0_TX_FIFO_SIZE,
    },
    { 
        .rx_cache = uart1_rx_cache, 
        .tx_cache = uart1_tx_cache, 
        .rx_cache_size = sizeof(uart1_rx_cache), 
        .tx_cache_size = sizeof(uart1_tx_cache),
        .rx_fifo_size = UART1_RX_FIFO_SIZE,
        .tx_fifo_size = UART1_TX_FIFO_SIZE,
    },
};

static const USART_TypeDef *USARTx[UART_PORT_MAX] = {
    USART1,
    USART2
};

static const st_uint32_t PeriphClk[UART_PORT_MAX] = {
    CPU_APB2CLK,
    CPU_APB1CLK
};

static uart_ctrl_t uart_ctrl[UART_PORT_MAX] = { 0 };

/* Private function prototypes -----------------------------------------------*/
extern void hal_uart_driver_event_txd( st_uint8_t port );
extern void hal_uart_driver_event_rxd( st_uint8_t port );
extern void USART1_IRQHandler( void );
extern void USART2_IRQHandler( void );

/* Exported function implementations -----------------------------------------*/

/**
  * @brief  Function brief
  * @param  param1
  * @param  param2
  * @note   None
  * @retval None
  */
void hal_uart_init( st_uint8_t port, const hal_uart_config_t *cfg )
{
    ST_ASSERT( port < UART_PORT_MAX );
    ST_ASSERT( cfg != NULL );

    // reset peripherals firstly
    switch ( port )
    {
        case HAL_UART_PORT_0:
            LL_APB2_GRP1_ForceReset( LL_APB2_GRP1_PERIPH_USART1 );
            LL_APB2_GRP1_ReleaseReset( LL_APB2_GRP1_PERIPH_USART1 );
            
        break;

        case HAL_UART_PORT_1:
            LL_APB1_GRP1_ForceReset( LL_APB1_GRP1_PERIPH_USART2 );
            LL_APB1_GRP1_ReleaseReset( LL_APB1_GRP1_PERIPH_USART2 );
        break;
    }

    // set baud rate
    LL_USART_SetBaudRate( USARTx[port],
                          PeriphClk[port],
                          LL_USART_OVERSAMPLING_16, 
                          cfg->baud_rate );

    // set data bits
    switch ( cfg->data_bits )
    {
        case HAL_UART_DATA_BITS_7:
            LL_USART_SetDataWidth( USARTx[port], LL_USART_DATAWIDTH_7B );
        break;
        case HAL_UART_DATA_BITS_8:
            LL_USART_SetDataWidth( USARTx[port], LL_USART_DATAWIDTH_8B );
        break;
        case HAL_UART_DATA_BITS_9:
            LL_USART_SetDataWidth( USARTx[port], LL_USART_DATAWIDTH_9B );
        break;
        default:
            LL_USART_SetDataWidth( USARTx[port], LL_USART_DATAWIDTH_8B );
        break;
    }

    // set stop bits
    switch ( cfg->stop_bits )
    {
        case HAL_UART_STOP_BITS_1:
            LL_USART_SetStopBitsLength( USARTx[port], LL_USART_STOPBITS_1 );
        break;

        case HAL_UART_STOP_BITS_2:
            LL_USART_SetStopBitsLength( USARTx[port], LL_USART_STOPBITS_2 );
        break;

        default:
            LL_USART_SetStopBitsLength( USARTx[port], LL_USART_STOPBITS_1 );
        break;
    }

    // set parity
    switch ( cfg->parity )
    {
        case HAL_UART_PARITY_EVEN:
            LL_USART_SetParity( USARTx[port], LL_USART_PARITY_EVEN );
        break;
        case HAL_UART_PARITY_ODD:
            LL_USART_SetParity( USARTx[port], LL_USART_PARITY_ODD );
        break;
        default:
            LL_USART_SetParity( USARTx[port], LL_USART_PARITY_NONE );
        break;
    }

    // set bit order
    if( cfg->bit_order == HAL_UART_BIT_ORDER_LSB )
        LL_USART_SetTransferBitOrder( USARTx[port], LL_USART_BITORDER_LSBFIRST );
    else
        LL_USART_SetTransferBitOrder( USARTx[port], LL_USART_BITORDER_MSBFIRST );

    // set data invert
    if( cfg->invert == HAL_UART_NRZ_NORMAL )
        LL_USART_SetBinaryDataLogic( USARTx[port] , LL_USART_BINARY_LOGIC_POSITIVE );
    else
        LL_USART_SetBinaryDataLogic( USARTx[port] , LL_USART_BINARY_LOGIC_NEGATIVE );

    // init uart control body info
    st_memset( &uart_ctrl[port], 0, sizeof(uart_ctrl_t) );
    uart_ctrl[port].rx_indicate = cfg->rx_indicate;
    uart_ctrl[port].tx_complete = cfg->tx_complete;
    
}

/**
  * @brief  Function brief
  * @param  param1
  * @param  param2
  * @note   None
  * @retval None
  */
void hal_uart_open( st_uint8_t port )
{
    ST_ASSERT( port < UART_PORT_MAX );
}

/**
  * @brief  Function brief
  * @param  param1
  * @param  param2
  * @note   None
  * @retval None
  */
void hal_uart_putc( st_uint8_t port, st_uint8_t byte )
{
    ST_ASSERT( port < UART_PORT_MAX );
}

/**
  * @brief  Function brief
  * @param  param1
  * @param  param2
  * @note   None
  * @retval None
  */
st_uint8_t hal_uart_getc( st_uint8_t port )
{
    ST_ASSERT( port < UART_PORT_MAX );
}

/**
  * @brief  Function brief
  * @param  param1
  * @param  param2
  * @note   None
  * @retval None
  */
void hal_uart_close( st_uint8_t port )
{
    ST_ASSERT( port < UART_PORT_MAX );
}

/**
  * @brief  Function brief
  * @param  param1
  * @param  param2
  * @note   None
  * @retval None
  */
void hal_uart_deinit( st_uint8_t port )
{
    ST_ASSERT( port < UART_PORT_MAX );
    
    switch ( port )
    {
        case HAL_UART_PORT_0:
            LL_APB2_GRP1_ForceReset( LL_APB2_GRP1_PERIPH_USART1 );
            LL_APB2_GRP1_ReleaseReset( LL_APB2_GRP1_PERIPH_USART1 );
        break;

        case HAL_UART_PORT_1:
            LL_APB1_GRP1_ForceReset( LL_APB1_GRP1_PERIPH_USART2 );
            LL_APB1_GRP1_ReleaseReset( LL_APB1_GRP1_PERIPH_USART2 );
        break;
    }
}

/* Private function implementations ------------------------------------------*/
/**
  * @brief  Function brief
  * @param  param1
  * @param  param2
  * @note   None
  * @retval None
  */
void hal_uart_driver_event_txd( st_uint8_t port )
{

}

void hal_uart_driver_event_rxd( st_uint8_t port )
{

}

void hal_uart_isr( st_uint8_t port )
{
    
}

void USART1_IRQHandler( void )
{
    hal_uart_isr( HAL_UART_PORT_0 );
}

void USART2_IRQHandler( void )
{
    hal_uart_isr( HAL_UART_PORT_1 );
}

/****** (C) COPYRIGHT 2019 Single-Thread Development Team. *****END OF FILE****/

