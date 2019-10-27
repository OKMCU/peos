/******************************************************************************

 @file  

 @brief 

 Group: 
 Target Device: 

 ******************************************************************************
 

 ******************************************************************************
 Release Name: 
 Release Date: 2016-06-09 06:57:09
 *****************************************************************************/
#include "M051Series.h"
#include "spl_config.h"
#include "spl_uart.h"
#include "spl_mcu.h"

#include "bufmgr.h"

#if SPL_UART0_EN > 0
extern void SPL_UART0_CALLBACK( uint8_t event );
#endif

#if SPL_UART1_EN > 0
extern void SPL_UART1_CALLBACK( uint8_t event );
#endif

#if SPL_UART0_EN > 0
static FIFO_BUF_t uart0_rx_fifo;
static FIFO_BUF_t uart0_tx_fifo;
static uint8_t uart0_rx_cache[SPL_UART0_RX_CACHE_SIZE];
static uint8_t uart0_tx_cache[SPL_UART0_TX_CACHE_SIZE];
#endif

#if SPL_UART1_EN > 0
static FIFO_BUF_t uart1_rx_fifo;
static FIFO_BUF_t uart1_tx_fifo;
static uint8_t uart1_rx_cache[SPL_UART1_RX_CACHE_SIZE];
static uint8_t uart1_tx_cache[SPL_UART1_TX_CACHE_SIZE];
#endif

extern void     spl_uart_init      ( uint8_t port )
{
#if SPL_UART0_EN > 0
    if( port == SPL_UART_PORT_0 )
    {
        uart0_rx_fifo.buf = uart0_rx_cache;
        uart0_rx_fifo.head = 0;
        uart0_rx_fifo.tail = 0;
        uart0_rx_fifo.size = sizeof( uart0_rx_cache );
        uart0_tx_fifo.buf = uart0_tx_cache;
        uart0_tx_fifo.head = 0;
        uart0_tx_fifo.tail = 0;
        uart0_tx_fifo.size = sizeof( uart0_tx_cache );

        CLK_EnableModuleClock(UART0_MODULE);
        CLK_SetModuleClock(UART0_MODULE, CLK_CLKSEL1_UART_S_PLL, CLK_CLKDIV_UART(1));
	    SYS->P3_MFP &= ~(SYS_MFP_P30_Msk | SYS_MFP_P31_Msk);
        SYS->P3_MFP |= (SYS_MFP_P30_RXD0 | SYS_MFP_P31_TXD0);
        
        return;
    }
#endif

#if SPL_UART1_EN > 0
    if( port == SPL_UART_PORT_1 )
    {
        uart1_rx_fifo.buf = uart1_rx_cache;
        uart1_rx_fifo.head = 0;
        uart1_rx_fifo.tail = 0;
        uart1_rx_fifo.size = sizeof( uart1_rx_cache );
        uart1_tx_fifo.buf = uart1_tx_cache;
        uart1_tx_fifo.head = 0;
        uart1_tx_fifo.tail = 0;
        uart1_tx_fifo.size = sizeof( uart1_tx_cache );

        CLK_EnableModuleClock(UART1_MODULE);
        CLK_SetModuleClock(UART1_MODULE, CLK_CLKSEL1_UART_S_PLL, CLK_CLKDIV_UART(1));
	    SYS->P1_MFP &= ~(SYS_MFP_P12_Msk | SYS_MFP_P13_Msk);
        SYS->P1_MFP |= (SYS_MFP_P12_RXD1 | SYS_MFP_P13_TXD1);
        
        return;
    }
#endif
}

extern void spl_uart_open ( uint8_t port )
{   
#if SPL_UART0_EN > 0
    if( port == SPL_UART_PORT_0 )
    {
        /* Reset UART0 */
        SYS_ResetModule( UART0_RST );
        /* Init UART0 to 115200-8n1 for print message */
        UART_Open( UART0, SPL_UART0_BAUDRATE );
        /* Enable Interrupt and install the call back function */
        //UART_ENABLE_INT(UART0, (UART_IER_RDA_IEN_Msk | UART_IER_THRE_IEN_Msk | UART_IER_RTO_IEN_Msk));
        UART_ENABLE_INT( UART0, ( UART_IER_RDA_IEN_Msk | UART_IER_RTO_IEN_Msk ));
        NVIC_EnableIRQ( UART0_IRQn );
        return;
    }
#endif

#if SPL_UART1_EN > 0
    if( port == SPL_UART_PORT_1 )
    {
        /* Reset UART1 */
        SYS_ResetModule( UART1_RST );
        /* Init UART1 to 115200-8n1 for print message */
        UART_Open( UART1, SPL_UART1_BAUDRATE );
        /* Enable Interrupt and install the call back function */
        //UART_ENABLE_INT(UART1, (UART_IER_RDA_IEN_Msk | UART_IER_THRE_IEN_Msk | UART_IER_RTO_IEN_Msk));
        UART_ENABLE_INT( UART1, (UART_IER_RDA_IEN_Msk | UART_IER_RTO_IEN_Msk) );
        NVIC_EnableIRQ ( UART1_IRQn );
        return;
    }
#endif
}

extern void spl_uart_txd( uint8_t port, uint8_t byte )
{
#if SPL_UART0_EN > 0
    if( port == SPL_UART_PORT_0 )
    {
        spl_mcu_disable_irq();
        fifo_buf_put( &uart0_tx_fifo, byte );
        spl_mcu_enable_irq();
        UART_ENABLE_INT( UART0, UART_IER_THRE_IEN_Msk);
        return;
    }
#endif

#if SPL_UART1_EN > 0
    if( port == SPL_UART_PORT_1 )
    {
        spl_mcu_disable_irq();
        fifo_buf_put( &uart1_tx_fifo, byte );
        spl_mcu_enable_irq();
        UART_ENABLE_INT( UART1, UART_IER_THRE_IEN_Msk);
        return;
    }
#endif
}

extern uint8_t  spl_uart_rxd      ( uint8_t port )
{
    uint8_t u8tmp;
    (void)u8tmp;
    
#if SPL_UART0_EN > 0
    if( port == SPL_UART_PORT_0 )
    {
        spl_mcu_disable_irq();
        u8tmp = fifo_buf_get(&uart0_rx_fifo);
        spl_mcu_enable_irq();
        return u8tmp;
    }
#endif

#if SPL_UART1_EN > 0
    if( port == SPL_UART_PORT_1 )
    {
        spl_mcu_disable_irq();
        u8tmp = fifo_buf_get(&uart1_rx_fifo);
        spl_mcu_enable_irq();
        return u8tmp;
    }
#endif

    return 0;

}

extern uint8_t  spl_uart_txd_full  ( uint8_t port )
{
    uint8_t u8tmp;
    (void)u8tmp;
#if SPL_UART0_EN > 0
    if( port == SPL_UART_PORT_0 )
    {
        spl_mcu_disable_irq();
        u8tmp = fifo_buf_full(&uart0_tx_fifo);
        spl_mcu_enable_irq();
        return u8tmp;
    }
#endif

#if SPL_UART1_EN > 0
    if( port == SPL_UART_PORT_1 )
    {
        spl_mcu_disable_irq();
        u8tmp = fifo_buf_full(&uart1_tx_fifo);
        spl_mcu_enable_irq();
        return u8tmp;
    }
#endif

    return 0;
}

extern uint8_t  spl_uart_rxd_empty ( uint8_t port )
{
    uint8_t u8tmp;
    (void)u8tmp;
#if SPL_UART0_EN > 0
    if( port == SPL_UART_PORT_0 )
    {
        spl_mcu_disable_irq();
        u8tmp = fifo_buf_empty(&uart0_rx_fifo);
        spl_mcu_enable_irq();
        return u8tmp;
    }
#endif

#if SPL_UART1_EN > 0
    if( port == SPL_UART_PORT_1 )
    {
        spl_mcu_disable_irq();
        u8tmp = fifo_buf_empty(&uart1_rx_fifo);
        spl_mcu_enable_irq();
        return u8tmp;
    }
#endif

    return 0;
}

extern void spl_uart_close( uint8_t port )
{
#if SPL_UART0_EN > 0
    if( port == SPL_UART_PORT_0 )
    {
        UART_DISABLE_INT( UART0, (UART_IER_RDA_IEN_Msk | UART_IER_THRE_IEN_Msk | UART_IER_RTO_IEN_Msk) );
        NVIC_DisableIRQ( UART0_IRQn );
        UART_Close( UART0 );
        return;
    }
#endif

#if SPL_UART1_EN > 0
    if(port == SPL_UART_PORT_1)
    {
        UART_DISABLE_INT(UART1, (UART_IER_RDA_IEN_Msk | UART_IER_THRE_IEN_Msk | UART_IER_RTO_IEN_Msk));
        NVIC_DisableIRQ(UART1_IRQn);
        UART_Close(UART1);
        return;
    }
#endif
}

extern void spl_uart_deinit( uint8_t port )
{
#if SPL_UART0_EN > 0
    if( port == SPL_UART_PORT_0 )
    {
        CLK_DisableModuleClock(UART0_MODULE);
	    SYS->P3_MFP &= ~(SYS_MFP_P30_Msk | SYS_MFP_P31_Msk);
        SYS->P3_MFP |= (SYS_MFP_P30_RXD0 | SYS_MFP_P31_TXD0);
        
        uart0_rx_fifo.buf = NULL;
        uart0_rx_fifo.head = 0;
        uart0_rx_fifo.tail = 0;
        uart0_rx_fifo.size = 0;
        uart0_tx_fifo.buf = NULL;
        uart0_tx_fifo.head = 0;
        uart0_tx_fifo.tail = 0;
        uart0_tx_fifo.size = 0;
        return;
    }
#endif

#if SPL_UART1_EN > 0
    if( port == SPL_UART_PORT_1 )
    {
        CLK_DisableModuleClock(UART1_MODULE);
	    SYS->P1_MFP &= ~(SYS_MFP_P12_Msk | SYS_MFP_P13_Msk);
        SYS->P1_MFP |= (SYS_MFP_P12_RXD1 | SYS_MFP_P13_TXD1);
        
        uart1_rx_fifo.buf = NULL;
        uart1_rx_fifo.head = 0;
        uart1_rx_fifo.tail = 0;
        uart1_rx_fifo.size = 0;
        uart1_tx_fifo.buf = NULL;
        uart1_tx_fifo.head = 0;
        uart1_tx_fifo.tail = 0;
        uart1_tx_fifo.size = 0;
        return;
    }
#endif
}

#if SPL_UART0_EN > 0
extern void spl_uart0_isr( void )
{
    uint32_t u32IntSts;
	uint8_t u8tmp = 0xFF;
    int8_t rx_cnt = 0;
    
    u32IntSts = UART0->ISR;
    
    if( u32IntSts & UART_ISR_RDA_INT_Msk )
    {
        /* Get all the input characters */
        while( UART_IS_RX_READY( UART0 ) )
        {
            /* Get the character from UART Buffer */
            u8tmp = UART_READ( UART0 );
            if( rx_cnt >= 0 )
            {
                if( !fifo_buf_full(&uart0_rx_fifo) )
                {
                    fifo_buf_put( &uart0_rx_fifo, u8tmp );
                    rx_cnt++;
                }
                else
                {
                    fifo_buf_flush(&uart0_rx_fifo);
                    rx_cnt = -1;
                    SPL_UART0_CALLBACK( SPL_UART_ISR_EVT_RXD_FULL );
                }
            }
        }

        if( rx_cnt > 0 )
            SPL_UART0_CALLBACK( SPL_UART_ISR_EVT_RXD );
    }
    
    
    if(u32IntSts & UART_ISR_THRE_INT_Msk)
    {
        while( !UART_IS_TX_FULL( UART0 ) )
        {
            if( !fifo_buf_empty(&uart0_tx_fifo) )
            {
                u8tmp = fifo_buf_get( &uart0_tx_fifo );
                UART_WRITE( UART0, u8tmp );
                //SPL_UART0_CALLBACK( SPL_UART_ISR_EVT_TXD );
            }
            else
            {
                UART_DISABLE_INT( UART0, UART_IER_THRE_IEN_Msk );
                SPL_UART0_CALLBACK( SPL_UART_ISR_EVT_TXD_EMPTY );
                break;
            }
        }
    }
}
#endif

#if SPL_UART1_EN > 0
extern void spl_uart1_isr( void )
{
    uint32_t u32IntSts;
	uint8_t u8tmp = 0xFF;
    int8_t rx_cnt = 0;
    
    u32IntSts = UART1->ISR;
    
    if( u32IntSts & UART_ISR_RDA_INT_Msk )
    {
        /* Get all the input characters */
        while( UART_IS_RX_READY( UART1 ) )
        {
            /* Get the character from UART Buffer */
            u8tmp = UART_READ( UART1 );
            if( rx_cnt >= 0 )
            {
                if( !fifo_buf_full(&uart1_rx_fifo) )
                {
                    fifo_buf_put( &uart1_rx_fifo, u8tmp );
                    rx_cnt++;
                }
                else
                {
                    fifo_buf_flush(&uart1_rx_fifo);
                    rx_cnt = -1;
                    SPL_UART1_CALLBACK( SPL_UART_ISR_EVT_RXD_FULL );
                }
            }
        }

        if( rx_cnt > 0 )
            SPL_UART1_CALLBACK( SPL_UART_ISR_EVT_RXD );
    }
    
    
    if(u32IntSts & UART_ISR_THRE_INT_Msk)
    {
        while( !UART_IS_TX_FULL( UART1 ) )
        {
            if( !fifo_buf_empty(&uart1_tx_fifo) )
            {
                u8tmp = fifo_buf_get( &uart1_tx_fifo );
                UART_WRITE( UART1, u8tmp );
                //SPL_UART1_CALLBACK( SPL_UART_ISR_EVT_TXD );
            }
            else
            {
                UART_DISABLE_INT( UART1, UART_IER_THRE_IEN_Msk );
                SPL_UART1_CALLBACK( SPL_UART_ISR_EVT_TXD_EMPTY );
                break;
            }
        }
    }
}
#endif

