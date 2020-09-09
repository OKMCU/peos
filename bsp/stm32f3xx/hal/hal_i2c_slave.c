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
    st_uint8_t *rx_cache;
    st_uint8_t *tx_cache;
    st_uint8_t rx_cache_size;
    st_uint8_t tx_cache_size;
} i2cs_cache_t;

typedef struct {
    void (*callback[HAL_I2CS_ADDR_MAX])( st_uint8_t event );
    st_uint8_t addr_id;
    st_uint8_t rx_head;
    st_uint8_t rx_tail;
    st_uint8_t tx_head;
    st_uint8_t tx_tail;
} i2cs_ctrl_t;

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static st_uint8_t i2cs0_rx_cache[I2CS0_RX_CACHE_SIZE];
static st_uint8_t i2cs0_tx_cache[I2CS0_TX_CACHE_SIZE];
static st_uint8_t i2cs1_rx_cache[I2CS1_RX_CACHE_SIZE];
static st_uint8_t i2cs1_tx_cache[I2CS1_TX_CACHE_SIZE];
static st_uint8_t task_id_i2cs[HAL_I2CS_PORT_MAX];

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

static i2cs_ctrl_t i2cs_ctrl[HAL_I2CS_PORT_MAX] = { 0 };

/* Private function prototypes -----------------------------------------------*/
static void hal_i2cs_portx_init( st_uint8_t port, st_uint8_t task_id );
static void hal_i2cs_portx_task( st_uint8_t port, st_int8_t event_id );
static void hal_i2cs_isr( st_uint8_t port );

extern void I2C1_EV_IRQHandler( void );
extern void I2C2_EV_IRQHandler( void );
extern void I2C1_ER_IRQHandler( void );
extern void I2C2_ER_IRQHandler( void );

/* Exported function implementations -----------------------------------------*/
void hal_i2cs_port0_init( st_uint8_t task_id )
{
    hal_i2cs_portx_init( HAL_I2CS_PORT_0 );
}

void hal_i2cs_port1_init( st_uint8_t task_id )
{
    hal_i2cs_portx_init( HAL_I2CS_PORT_1, task_id );
}

void hal_i2cs_port0_task( st_int8_t event_id )
{
    hal_i2cs_portx_task( HAL_I2CS_PORT_0, event_id );
}

void hal_i2cs_port1_task( st_int8_t event_id )
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
void hal_i2cs_open( st_uint8_t port, const hal_i2cs_config_t *cfg )
{
    ST_ASSERT( port < HAL_I2CS_PORT_MAX );
    ST_ASSERT( cfg != NULL );
    ST_ASSERT( !LL_I2C_IsEnabled(I2Cx[port]) );
    
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
    st_memset( &i2cs_ctrl[port], 0, sizeof(i2cs_ctrl_t) );
    i2cs_ctrl[port].callback[0] = cfg->callback[0];
    i2cs_ctrl[port].callback[1] = cfg->callback[1];
    i2cs_ctrl[port].addr_id = HAL_I2CS_ADDR_MAX;

    LL_I2C_Enable( I2Cx[port] );
}

/**
  * @brief  Function brief
  * @param  param1
  * @param  param2
  * @note   None
  * @retval None
  */
void hal_i2cs_putc( st_uint8_t port, st_uint8_t byte )
{
    ST_ASSERT( port < HAL_I2CS_PORT_MAX );
    ST_ASSERT( LL_I2C_IsEnabled(I2Cx[port]) );
    
    ST_ASSERT( !RING_BUF_FULL(i2cs_ctrl[port].tx_head, 
                              i2cs_ctrl[port].tx_tail, 
                              i2cs_cache[port].tx_cache_size) );
    RING_BUF_PUT( byte,
                  i2cs_ctrl[port].tx_head, 
                  i2cs_cache[port].tx_cache, 
                  i2cs_cache[port].tx_cache_size );
}

/**
  * @brief  Function brief
  * @param  param1
  * @param  param2
  * @note   None
  * @retval None
  */
st_uint8_t hal_i2cs_getc( st_uint8_t port )
{
    st_uint8_t byte;
    
    ST_ASSERT( port < HAL_I2CS_PORT_MAX );
    ST_ASSERT( LL_I2C_IsEnabled(I2Cx[port]) )
    ST_ASSERT( !RING_BUF_EMPTY(i2cs_ctrl[port].rx_head, 
                               i2cs_ctrl[port].rx_tail) );
    RING_BUF_GET( &byte, 
                  i2cs_ctrl[port].rx_tail, 
                  i2cs_cache[port].rx_cache, 
                  i2cs_cache[port].rx_cache_size );
    return byte;
}

st_uint8_t hal_i2cs_tx_buf_free_size( st_uint8_t port )
{
    ST_ASSERT( port < HAL_I2CS_PORT_MAX );
    return RING_BUF_FREE_SIZE( i2cs_ctrl[port].tx_head, 
                               i2cs_ctrl[port].tx_tail, 
                               i2cs_cache[port].tx_cache_size );
}

st_uint8_t hal_i2cs_rx_buf_used_size( st_uint8_t port )
{
    ST_ASSERT( port < HAL_I2CS_PORT_MAX );
    return RING_BUF_USED_SIZE( i2cs_ctrl[port].rx_head, 
                               i2cs_ctrl[port].rx_tail, 
                               i2cs_cache[port].rx_cache_size );
}

/**
  * @brief  Function brief
  * @param  param1
  * @param  param2
  * @note   None
  * @retval None
  */
void hal_i2cs_close( st_uint8_t port )
{
    ST_ASSERT( port < HAL_I2CS_PORT_MAX );
    ST_ASSERT( LL_I2C_IsEnabled(I2Cx[port]) );

    LL_I2C_Disable( I2Cx[port] );

    switch ( port )
    {
        case HAL_UART_PORT_0:
            NVIC_DisableIRQ( I2C1_EV_IRQn );
            NVIC_DisableIRQ( I2C1_ER_IRQn );
            LL_GPIO_SetPinMode( GPIOB, LL_GPIO_PIN_6, LL_GPIO_MODE_ANALOG ); // PB6: I2C1_SCL
            LL_GPIO_SetPinMode( GPIOB, LL_GPIO_PIN_7, LL_GPIO_MODE_ANALOG ); // PB7: I2C1_SDA
            LL_GPIO_SetPinPull( GPIOB, LL_GPIO_PIN_6, LL_GPIO_PULL_NO );
            LL_GPIO_SetPinPull( GPIOB, LL_GPIO_PIN_7, LL_GPIO_PULL_NO );
            LL_APB1_GRP1_DisableClock( LL_APB1_GRP1_PERIPH_I2C1 );
        break;

        case HAL_UART_PORT_1:
            NVIC_DisableIRQ( I2C2_EV_IRQn );
            NVIC_DisableIRQ( I2C2_ER_IRQn );
            LL_GPIO_SetPinMode( GPIOF, LL_GPIO_PIN_0, LL_GPIO_MODE_ANALOG ); // PF0: I2C2_SDA
            LL_GPIO_SetPinMode( GPIOF, LL_GPIO_PIN_1, LL_GPIO_MODE_ANALOG ); // PF1: I2C2_SCL
            LL_GPIO_SetPinPull( GPIOF, LL_GPIO_PIN_0, LL_GPIO_PULL_NO );
            LL_GPIO_SetPinPull( GPIOF, LL_GPIO_PIN_1, LL_GPIO_PULL_NO );
            LL_APB1_GRP1_DisableClock( LL_APB1_GRP1_PERIPH_I2C2 );
        break;
    }
}

/* Private function implementations ------------------------------------------*/
static void hal_i2cs_portx_init( st_uint8_t port, st_uint8_t task_id )
{
    task_id_i2cs[port] = task_id;
}

static void hal_i2cs_portx_task( st_uint8_t port, st_int8_t event_id )
{
    st_uint8_t const event_mapping[] = {
        HAL_I2CS_EVENT_ADDRW,
        HAL_I2CS_EVENT_RXNE,
        HAL_I2CS_EVENT_ADDRR,
        HAL_I2CS_EVENT_TXE,
        HAL_I2CS_EVENT_NACK,
        HAL_I2CS_EVENT_STOP,
    };
    
    ST_ASSERT( event_id < sizeof(event_mapping)/sizeof(event_mapping[0]) );
    if( i2cs_ctrl[port].addr_id < HAL_I2CS_ADDR_MAX )
    {
        if( i2cs_ctrl[port].callback[i2cs_ctrl[port].addr_id] )
            i2cs_ctrl[port].callback[i2cs_ctrl[port].addr_id]( event_mapping[event_id] );
    }
}

/**
  * @brief  Function brief
  * @param  param1
  * @param  param2
  * @note   None
  * @retval None
  */
static void hal_i2cs_isr( st_uint8_t port )
{
    st_uint32_t isr;
    st_uint8_t addcode;
        
    isr = I2Cx[port]->ISR;                                        //Read the ISR status
    addcode = ((isr & I2C_ISR_ADDCODE) >> I2C_ISR_ADDCODE_Pos) << 1;
    
    if( addcode == ((I2Cx[port]->OAR1)&0xFE) )
    {
        
    }
    else if( addcode == ((I2Cx[port]->OAR2)&0xFE) )
    {
        
    }
    else
    {
        
    }
    
    {
        if( isr & I2C_ISR_RXNE )                            //RX buffer not empty
        {
            TwiSlaveData = I2Cx[port]->RXDR;
            TWI_SlaveWriteHandler();                        //get the received byte as data
            goto EXIT_ISR;
        }
        
        if( isr & I2C_ISR_TXE )                             //TX buffer is empty
        {
            if( isr & I2C_ISR_DIR )
            {
                TWI_SlaveReadHandler();
                I2Cx[port]->TXDR = TwiSlaveData;                  //put the continous bytes into TX buffer         
            }
            else
            {
                I2Cx[port]->TXDR = 0x00;                          //put a dummy byte into TXD buffer to clear the interrupt
                                                            //remember to flush TXD buffer when entering into slave transmitter mode
            }
            goto EXIT_ISR;
        }

        if( isr & I2C_ISR_NACKF )                           //Slave transmitter mode, Received NACK
        {
            I2Cx[port]->ICR |= I2C_ICR_NACKCF;                    //Clear the interrupt flag
            goto EXIT_ISR;
        }

        if( isr & I2C_ISR_STOPF )                           //Stop detected
        {
            I2Cx[port]->ICR |= I2C_ICR_STOPCF;                    //Clear the interrupt flag
            //hal_twi_slave_callback_stop();                //invoke the callback
            TWI_SlaveStopHandler();
            SysCtrlFlag.Bit.TwiBusy = 0;                    //Clear I2C busy flag
            goto EXIT_ISR;
        }

        if( isr & I2C_ISR_ADDR )                            //check if slave device address is matched
        {
            if( isr & I2C_ISR_DIR )                         //check transmission direction
            {
                //hal_twi_slave_callback_addrr();           //slave transmitter mode
                TWI_TX_bufPtr = TWI_RX_buf[0];
                TWI_SlaveReadHandler();
                I2Cx[port]->ISR |= I2C_ISR_TXE;                   //flush TXD buffer
                I2Cx[port]->TXDR = TwiSlaveData;                  //put the 1st byte into TX buffer
            }
            else
            {
                //hal_twi_slave_callback_addrw();           //slave receiver mode
                TWI_RX_bufPtr = 0;
            }
            I2Cx[port]->ICR |= I2C_ICR_ADDRCF;                    //clear interrupt flag
            goto EXIT_ISR;
        }
    }
    else
    {
        if( isr & I2C_ISR_RXNE )                            //RX buffer not empty
        {
            i2c_tunnelling_callback_rxd(I2C1->RXDR);        //get the received byte as data
            goto EXIT_ISR;
        }
        
        if( isr & I2C_ISR_TXE )                             //TX buffer is empty
        {
            if( isr & I2C_ISR_DIR )
                I2C1->TXDR = i2c_tunnelling_callback_txd(); //put the continous bytes into TX buffer         
            else
                I2C1->TXDR = 0x00;                          //put a dummy byte into TXD buffer to clear the interrupt
                                                            //remember to flush TXD buffer when entering into slave transmitter mode
            goto EXIT_ISR;
        }

        if( isr & I2C_ISR_NACKF )                           //Slave transmitter mode, Received NACK
        {
            I2C1->ICR |= I2C_ICR_NACKCF;                    //Clear the interrupt flag
            goto EXIT_ISR;
        }

        if( isr & I2C_ISR_STOPF )                           //Stop detected
        {
            I2C1->ICR |= I2C_ICR_STOPCF;                    //Clear the interrupt flag
            i2c_tunnelling_callback_stop();                 //invoke the callback
            goto EXIT_ISR;
        }

        if( isr & I2C_ISR_ADDR )                            //check if slave device address is matched
        {
            if( isr & I2C_ISR_DIR )                         //check transmission direction
            {
                i2c_tunnelling_callback_addrr();
                I2C1->ISR |= I2C_ISR_TXE;                   //flush TXD buffer
                I2C1->TXDR = i2c_tunnelling_callback_txd(); //put the 1st byte into TX buffer
            }
            else
            {
                i2c_tunnelling_callback_addrw();            //slave receiver mode
            }
            I2C1->ICR |= I2C_ICR_ADDRCF;                    //clear interrupt flag
            goto EXIT_ISR;
        }
    }
    
    //Should not reach here, otherwise there must be something wrong 
    TWI_DeInit();                                           //reset hardware I2C
    TWI_Init();                                             //re-init hardware I2C
    SysCtrlFlag.Bit.TwiBusy = 0;
    
EXIT_ISR:
#if DEBUG_PIN_LED_G_EN > 0
    LL_GPIO_ResetOutputPin( GPIO_PORT_LED_G, GPIO_PIN_LED_G );    //PF9 set to LOW
#endif
    TWI_ActivityCount++;                                    //expected event, increase counter

}

void I2C1_EV_IRQHandler( void )
{
    hal_i2cs_isr( HAL_I2CS_PORT_0 );
}

void I2C2_EV_IRQHandler( void )
{
    hal_i2cs_isr( HAL_I2CS_PORT_1 );
}

void I2C1_ER_IRQHandler( void )
{
    LL_I2C_Disable( I2Cx[HAL_I2CS_PORT_0] );
    LL_I2C_Enable( I2Cx[HAL_I2CS_PORT_0] );
}

void I2C2_ER_IRQHandler( void )
{
    LL_I2C_Disable( I2Cx[HAL_I2CS_PORT_1] );
    LL_I2C_Enable( I2Cx[HAL_I2CS_PORT_1] );
}

#endif //ST_USING_HAL_UART
/****** (C) COPYRIGHT 2019 Single-Thread Development Team. *****END OF FILE****/
