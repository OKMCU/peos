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
 
#include "osal.h"
#include "spl.h"

#include "hal_config.h"
#include "hal_assert.h"
#include "hal_cli.h"

#include "main.h"

#include "stdstr.h"
#include "stringx.h"
#include "bufmgr.h"

#if (HAL_CLI_EN > 0)

#define HAL_CLI_RX_DATA_BLK_SIZE    8

static void *tx_fifo;
static void *rx_fifo;
//#define IS_CHAR(x)  ( x > 0 && x <= 127 )


extern void hal_cli_init( void )
{   
    tx_fifo = NULL;
    rx_fifo = NULL;
}

extern uint8_t hal_cli_rx_len    ( void )
{
    uint32_t len;
    
    if( rx_fifo != NULL )
    {
        len = osal_fifo_len( rx_fifo );
        HAL_ASSERT( len <= HAL_CLI_RX_BUF_SIZE );
        return (uint8_t) len;
    }
    
    return 0;
}

extern void hal_cli_putchar(char c)
{
    uint8_t *pc;
    if(tx_fifo == NULL)
        tx_fifo = osal_fifo_create();
    HAL_ASSERT( tx_fifo != NULL );
    pc = osal_fifo_put( tx_fifo, (uint8_t)c );
    HAL_ASSERT( pc != NULL );
    osal_event_set( TASK_ID_HAL_DRIVER_BASIC, TASK_EVT_HAL_DRIVER_BASIC_CLI_TXE );
}

extern char hal_cli_getchar( void )
{
    char c;
    
    c = '\0';
    
    if( rx_fifo != NULL )
    {
        c = (char)osal_fifo_get( rx_fifo );
        if( osal_fifo_len( rx_fifo ) == 0 )
        {
            osal_fifo_delete( rx_fifo );
            rx_fifo = NULL;
        }
    }
    return c;
}

#if (HAL_CLI_PRINT_EN > 0)
extern void hal_cli_print_str(const char *s)
{
    while(*s)
    {
        hal_cli_putchar(*s++);
    }
}

extern void hal_cli_print_sint(int32_t num)
{
    char str[SINT_STR_LEN_MAX];
    uint8_t len;
    uint8_t i;
    
    len = tostr_sint(num, str);
    
    for(i = 0; i < len; i++)
    {
        hal_cli_putchar(str[i]);
    }
    
}

extern void hal_cli_print_uint(uint32_t num)
{
    char str[UINT_STR_LEN_MAX];
    uint8_t len;
    uint8_t i;
    
    len = tostr_uint(num, str);
    
    for(i = 0; i < len; i++)
    {
        hal_cli_putchar(str[i]);
    }
}

extern void hal_cli_print_hex8(uint8_t num)
{
    char str[HEX8_STR_LEN_MAX];
    uint8_t len;
    uint8_t i;
    
    len = tostr_hex8(num, str);
    
    for(i = 0; i < len; i++)
    {
        hal_cli_putchar(str[i]);
    }
}


extern void hal_cli_print_hex16(uint16_t num)
{
    char str[HEX16_STR_LEN_MAX];
    uint8_t len;
    uint8_t i;
    
    len = tostr_hex16(num, str);
    
    for(i = 0; i < len; i++)
    {
        hal_cli_putchar(str[i]);
    }
}

extern void hal_cli_print_hex32(uint32_t num)
{
    char str[HEX32_STR_LEN_MAX];
    uint8_t len;
    uint8_t i;
    
    len = tostr_hex32(num, str);
    
    for(i = 0; i < len; i++)
    {
        hal_cli_putchar(str[i]);
    }
}
#endif

extern void hal_cli_driver_handle_rxne( void )
{
    char c;
    uint8_t *pc;
    
    if( rx_fifo == NULL )
        rx_fifo = osal_fifo_create();
    
    if( rx_fifo != NULL )
    {
        while( !spl_uart_rxd_empty(SPL_UART_PORT_0) )
        {
            c = spl_uart_rxd( SPL_UART_PORT_0 );

            if( osal_fifo_len( rx_fifo ) < HAL_CLI_RX_BUF_SIZE )
            {
                pc = osal_fifo_put( rx_fifo, (uint8_t)c );
                if( pc != NULL )
                {
                    osal_event_set( TASK_ID_APP_CLI, TASK_EVT_APP_CLI_RX_CHAR );
                }
                else
                {
                    HAL_ASSERT_FORCED();
                }
            }
            else
            {
                osal_event_set( TASK_ID_APP_CLI, TASK_EVT_APP_CLI_RX_OVF );
            }
        }
    }
    else
    {
        HAL_ASSERT_FORCED();
    }
}

extern void hal_cli_driver_handle_txe( void )
{
    uint8_t b;
    
    if( tx_fifo != NULL )
    {
        while( !spl_uart_txd_full(SPL_UART_PORT_0) )
        {
            if( osal_fifo_len( tx_fifo ) )
            {
                b = osal_fifo_get(tx_fifo);
                spl_uart_txd( SPL_UART_PORT_0, b );
            }
            else
            {
                osal_fifo_delete( tx_fifo );
                tx_fifo = NULL;
                break;
            }
        }
    }
}

extern void spl_uart0_callback( uint8_t event )
{
    switch (event)
    {
        case SPL_UART_ISR_EVT_RXD:
            osal_event_set( TASK_ID_HAL_DRIVER_BASIC, TASK_EVT_HAL_DRIVER_BASIC_CLI_RXNE );
        break;

        case SPL_UART_ISR_EVT_TXD_EMPTY:
            osal_event_set( TASK_ID_HAL_DRIVER_BASIC, TASK_EVT_HAL_DRIVER_BASIC_CLI_TXE );
        break;
        
        case SPL_UART_ISR_EVT_RXD_FULL:
            osal_event_set( TASK_ID_APP_CLI, TASK_EVT_APP_CLI_RX_OVF );
        break;

        default:
        break;
    }
    
}

#endif /* (HAL_CLI_EN > 0) */

