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
#include "stm32f3xx_ll_bus.h"
#include "stm32f3xx_ll_gpio.h"
#include "stm32f3xx_ll_i2c.h"
#include "hal_i2c_slave.h"
#include "components/utilities/rbuf.h"

#ifdef ST_USING_HAL_I2C_SLAVE
/* Exported variables --------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define I2CS0_RX_CACHE_SIZE         8
#define I2CS0_TX_CACHE_SIZE         8

#define TASK_EVT_I2CS0_ADDRW        0
#define TASK_EVT_I2CS0_RXNE         1
#define TASK_EVT_I2CS0_ADDRR        2
#define TASK_EVT_I2CS0_TXE          3
#define TASK_EVT_I2CS0_NACK         4
#define TASK_EVT_I2CS0_STOP         5

/* Private typedef -----------------------------------------------------------*/
typedef struct {
    st_uint8_t *rx_cache;
    st_uint8_t *tx_cache;
    st_uint8_t rx_cache_size;
    st_uint8_t tx_cache_size;
} i2cs_cache_t;

typedef struct {
    void (*callback)( st_uint8_t event );
    st_uint8_t rx_head;
    st_uint8_t rx_tail;
    st_uint8_t tx_head;
    st_uint8_t tx_tail;
} i2cs_ctrl_t;

typedef struct {
    st_uint8_t addrw;
    st_uint8_t rxne;
    st_uint8_t addrr;
    st_uint8_t txe;
    st_uint8_t nack;
    st_uint8_t stop;
} i2cs_event_t;

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static st_uint8_t i2cs0_rx_cache[I2CS0_RX_CACHE_SIZE];
static st_uint8_t i2cs0_tx_cache[I2CS0_TX_CACHE_SIZE];
static st_uint8_t i2cs1_rx_cache[I2CS1_RX_CACHE_SIZE];
static st_uint8_t i2cs1_tx_cache[I2CS1_TX_CACHE_SIZE];
static st_uint8_t task_id_i2cs;

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

static i2cs_event_t const i2cs_event[HAL_I2CS_PORT_MAX] = {
    {
        .rxd = TASK_EVT_I2CS0_RXD,
        .txd = TASK_EVT_I2CS0_TXD,
        .ovf = TASK_EVT_I2CS0_OVF,
        .perr = TASK_EVT_I2CS0_PERR,
        .idle = TASK_EVT_I2CS0_IDLE,
    },

    {
        .rxd = TASK_EVT_I2CS1_RXD,
        .txd = TASK_EVT_I2CS1_TXD,
        .ovf = TASK_EVT_I2CS1_OVF,
        .perr = TASK_EVT_I2CS1_PERR,
        .idle = TASK_EVT_I2CS1_IDLE,
    },
};

static uart_ctrl_t uart_ctrl[HAL_UART_PORT_MAX] = { 0 };

/* Private function prototypes -----------------------------------------------*/
static void hal_uart_isr( st_uint8_t port );

extern void USART1_IRQHandler( void );
extern void USART2_IRQHandler( void );

/* Exported function implementations -----------------------------------------*/
void hal_uart_rxd_init( st_uint8_t task_id )
{
    task_id_rxd = task_id;
}

void hal_uart_rxd_task( st_int8_t event_id )
{
    switch ( event_id )
    {
        case TASK_EVT_UART0_RXD:
            if( uart_ctrl[HAL_UART_PORT_0].callback )
                uart_ctrl[HAL_UART_PORT_0].callback( HAL_UART_EVENT_RXD );
        break;

        case TASK_EVT_UART1_RXD:
            if( uart_ctrl[HAL_UART_PORT_1].callback )
                uart_ctrl[HAL_UART_PORT_1].callback( HAL_UART_EVENT_RXD );
        break;

        case TASK_EVT_UART0_PERR:
            if( uart_ctrl[HAL_UART_PORT_0].callback )
                uart_ctrl[HAL_UART_PORT_0].callback( HAL_UART_EVENT_PERR );
        break;

        case TASK_EVT_UART1_PERR:
            if( uart_ctrl[HAL_UART_PORT_1].callback )
                uart_ctrl[HAL_UART_PORT_1].callback( HAL_UART_EVENT_PERR );
        break;

        case TASK_EVT_UART0_OVF:
            if( uart_ctrl[HAL_UART_PORT_0].callback )
                uart_ctrl[HAL_UART_PORT_0].callback( HAL_UART_EVENT_OVF );
        break;

        case TASK_EVT_UART1_OVF:
            if( uart_ctrl[HAL_UART_PORT_1].callback )
                uart_ctrl[HAL_UART_PORT_1].callback( HAL_UART_EVENT_OVF );
        break;

        case TASK_EVT_UART0_IDLE:
            if( uart_ctrl[HAL_UART_PORT_0].callback )
                uart_ctrl[HAL_UART_PORT_0].callback( HAL_UART_EVENT_IDLE );
        break;

        case TASK_EVT_UART1_IDLE:
            if( uart_ctrl[HAL_UART_PORT_1].callback )
                uart_ctrl[HAL_UART_PORT_1].callback( HAL_UART_EVENT_IDLE );
        break;
    }
}

void hal_uart_txd_init( st_uint8_t task_id )
{
    task_id_txd = task_id;
}

void hal_uart_txd_task( st_int8_t event_id )
{
    switch ( event_id )
    {
        case TASK_EVT_UART0_TXD:
            if( uart_ctrl[HAL_UART_PORT_0].callback )
                uart_ctrl[HAL_UART_PORT_0].callback( HAL_UART_EVENT_TXD );
        break;

        case TASK_EVT_UART1_TXD:
            if( uart_ctrl[HAL_UART_PORT_1].callback )
                uart_ctrl[HAL_UART_PORT_1].callback( HAL_UART_EVENT_TXD );
        break;
    }
}


/**
  * @brief  Function brief
  * @param  param1
  * @param  param2
  * @note   None
  * @retval None
  */
void hal_uart_config( st_uint8_t port, const hal_uart_config_t *cfg )
{
    ST_ASSERT( port < HAL_UART_PORT_MAX );
    ST_ASSERT( cfg != NULL );
    ST_ASSERT( !LL_USART_IsEnabled(USARTx[port]) );
    
    // reset peripherals firstly
    switch ( port )
    {
        case HAL_UART_PORT_0:
            LL_APB2_GRP1_EnableClock( LL_APB2_GRP1_PERIPH_USART1 );
            LL_APB2_GRP1_ForceReset( LL_APB2_GRP1_PERIPH_USART1 );
            LL_APB2_GRP1_ReleaseReset( LL_APB2_GRP1_PERIPH_USART1 );
            LL_GPIO_SetPinPull( GPIOE, LL_GPIO_PIN_0, LL_GPIO_PULL_UP );
            LL_GPIO_SetPinPull( GPIOE, LL_GPIO_PIN_1, LL_GPIO_PULL_UP );
            LL_GPIO_SetPinMode( GPIOE, LL_GPIO_PIN_0, LL_GPIO_MODE_ALTERNATE ); // PE0: USART1_TX
            LL_GPIO_SetPinMode( GPIOE, LL_GPIO_PIN_1, LL_GPIO_MODE_ALTERNATE ); // PE1: USART1_RX
            LL_GPIO_SetAFPin_0_7( GPIOE, LL_GPIO_PIN_0, LL_GPIO_AF_7 );         // PE0: USART1_TX
            LL_GPIO_SetAFPin_0_7( GPIOE, LL_GPIO_PIN_1, LL_GPIO_AF_7 );         // PE1: USART1_RX
            NVIC_EnableIRQ( USART1_IRQn );
        break;
        case HAL_UART_PORT_1:
            LL_APB1_GRP1_EnableClock( LL_APB1_GRP1_PERIPH_USART2 );
            LL_APB1_GRP1_ForceReset( LL_APB1_GRP1_PERIPH_USART2 );
            LL_APB1_GRP1_ReleaseReset( LL_APB1_GRP1_PERIPH_USART2 );
            LL_GPIO_SetPinPull( GPIOA, LL_GPIO_PIN_2, LL_GPIO_PULL_UP );
            LL_GPIO_SetPinPull( GPIOA, LL_GPIO_PIN_3, LL_GPIO_PULL_UP );
            LL_GPIO_SetPinMode( GPIOA, LL_GPIO_PIN_2, LL_GPIO_MODE_ALTERNATE ); // PA2: USART2_TX
            LL_GPIO_SetPinMode( GPIOA, LL_GPIO_PIN_3, LL_GPIO_MODE_ALTERNATE ); // PA3: USART2_RX
            LL_GPIO_SetAFPin_0_7( GPIOA, LL_GPIO_PIN_2, LL_GPIO_AF_7 );         // PA2: USART2_TX
            LL_GPIO_SetAFPin_0_7( GPIOA, LL_GPIO_PIN_3, LL_GPIO_AF_7 );         // PA3: USART2_RX
            NVIC_EnableIRQ( USART2_IRQn );
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
        //case HAL_UART_DATA_BITS_7:
        //    LL_USART_SetDataWidth( USARTx[port], LL_USART_DATAWIDTH_7B );
        //break;
        //case HAL_UART_DATA_BITS_8:
        //    LL_USART_SetDataWidth( USARTx[port], LL_USART_DATAWIDTH_8B );
        //break;
        //case HAL_UART_DATA_BITS_9:
        //    LL_USART_SetDataWidth( USARTx[port], LL_USART_DATAWIDTH_9B );
        //break;
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
            LL_USART_EnableIT_PE( USARTx[port] );
        break;
        case HAL_UART_PARITY_ODD:
            LL_USART_SetParity( USARTx[port], LL_USART_PARITY_ODD );
            LL_USART_EnableIT_PE( USARTx[port] );
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
    uart_ctrl[port].callback = cfg->callback;
}

void hal_uart_open( st_uint8_t port )
{
    ST_ASSERT( port < HAL_UART_PORT_MAX );
    ST_ASSERT( !LL_USART_IsEnabled(USARTx[port]) );
    
    LL_USART_EnableDirectionRx( USARTx[port] );
    LL_USART_EnableDirectionTx( USARTx[port] );
    LL_USART_EnableIT_RXNE( USARTx[port] );
    LL_USART_EnableIT_IDLE( USARTx[port] );
    LL_USART_Enable( USARTx[port] );
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
    ST_ASSERT( port < HAL_UART_PORT_MAX );
    ST_ASSERT( LL_USART_IsEnabled(USARTx[port]) );
    
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
st_uint8_t hal_uart_getc( st_uint8_t port )
{
    st_uint8_t byte;
    
    ST_ASSERT( port < HAL_UART_PORT_MAX );
    ST_ASSERT( LL_USART_IsEnabled(USARTx[port]) );

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

st_uint8_t hal_uart_tx_buf_free( st_uint8_t port )
{
    ST_ASSERT( port < HAL_UART_PORT_MAX );
    return RING_BUF_FREE_SIZE( uart_ctrl[port].tx_head, 
                               uart_ctrl[port].tx_tail, 
                               uart_cache[port].tx_cache_size );
}

st_uint8_t hal_uart_rx_buf_used( st_uint8_t port )
{
    ST_ASSERT( port < HAL_UART_PORT_MAX );
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
void hal_uart_close( st_uint8_t port )
{
    ST_ASSERT( port < HAL_UART_PORT_MAX );
    ST_ASSERT( LL_USART_IsEnabled(USARTx[port]) );

    LL_USART_DisableDirectionTx( USARTx[port] );
    LL_USART_DisableDirectionRx( USARTx[port] );
    LL_USART_DisableIT_RXNE( USARTx[port] );
    LL_USART_DisableIT_IDLE( USARTx[port] );
    LL_USART_Disable( USARTx[port] );

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
/**
  * @brief  Function brief
  * @param  param1
  * @param  param2
  * @note   None
  * @retval None
  */
static void hal_uart_isr( st_uint8_t port )
{
    st_uint8_t byte;

    ST_ASSERT( port < HAL_UART_PORT_MAX );
    
    if( LL_USART_IsActiveFlag_RXNE(USARTx[port]) )
    {
        byte = LL_USART_ReceiveData8( USARTx[port] );
        if( RING_BUF_FULL(uart_ctrl[port].rx_head, 
                          uart_ctrl[port].rx_tail, 
                          uart_cache[port].rx_cache_size) )
        {
            st_task_set_event( task_id_rxd, uart_event[port].ovf );
        }
        else
        {
            RING_BUF_PUT( byte, 
                          uart_ctrl[port].rx_head, 
                          uart_cache[port].rx_cache, 
                          uart_cache[port].rx_cache_size );
            st_task_set_event( task_id_rxd, uart_event[port].rxd );
        }
        return;
    }

    if( LL_USART_IsActiveFlag_PE(USARTx[port]) )
    {
        st_task_set_event( task_id_rxd, uart_event[port].perr );
        LL_USART_ClearFlag_PE( USARTx[port] );
        return;
    }

    if( LL_USART_IsActiveFlag_IDLE( USARTx[port]) )
    {
        st_task_set_event( task_id_rxd, uart_event[port].idle );
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
            st_task_set_event( task_id_txd, uart_event[port].txd );
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
#endif //ST_USING_HAL_UART
/****** (C) COPYRIGHT 2019 Single-Thread Development Team. *****END OF FILE****/
