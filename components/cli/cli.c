
#include "st.h"
#include "hal_drivers.h"
#include "components/cli/cli.h"
#include "components/fifo/fifo.h"

static void *tx_fifo;
static char *rx_buff;
static cli_cmd_t *cmd;
//#define IS_CHAR(x)  ( x > 0 && x <= 127 )

static void cli_uart_driver_callback( st_uint8_t event )
{
    st_uint8_t size;
    
    switch ( event )
    {
        case HAL_UART_EVENT_RXD:
            
        break;

        case HAL_UART_EVENT_TXD:
            if( tx_fifo )
            {
                size = hal_uart_tx_buf_free(CLI_UART_PORT);
                while( size-- )
                {
                    if( fifo_len(tx_fifo) )
                    {
                        hal_uart_putc(CLI_UART_PORT, fifo_get(tx_fifo));
                    }
                    else
                    {
                        fifo_delete( tx_fifo );
                        tx_fifo = NULL;
                    }
                }
            }
        break;

        case HAL_UART_EVENT_OVF:
        break;

        case HAL_UART_EVENT_PERR:
        break;
    }
}

void cli_init( void )
{
    hal_uart_config_t cfg;
    
    tx_fifo = NULL;
    rx_buff = NULL;
    cfg.baud_rate = HAL_UART_BAUD_RATE_115200;
    cfg.data_bits = HAL_UART_DATA_BITS_8;
    cfg.stop_bits = HAL_UART_STOP_BITS_1;
    cfg.parity    = HAL_UART_PARITY_NONE;
    cfg.bit_order = HAL_UART_BIT_ORDER_LSB;
    cfg.invert    = HAL_UART_NRZ_NORMAL;
    cfg.callback  = cli_uart_driver_callback;

    hal_uart_init( CLI_UART_PORT, &cfg );
    hal_uart_open( CLI_UART_PORT );
}

void cli_task( st_uint8_t event )
{

}

void cli_register_cmds( const cli_cmd_t *cmd )
{
    
}

void cli_print_char( char ch )
{
    st_uint8_t *pc;

    if(tx_fifo == NULL)
    {
        if( hal_uart_tx_buf_free(CLI_UART_PORT) )
        {
            hal_uart_putc( CLI_UART_PORT, (st_uint8_t)ch );
        }
        else
        {
            tx_fifo = fifo_create();
            ST_ASSERT( tx_fifo != NULL );
            pc = fifo_put( tx_fifo, (st_uint8_t)ch );
            ST_ASSERT( pc != NULL );
        }
    }
    else
    {
        pc = fifo_put( tx_fifo, (st_uint8_t)ch );
        ST_ASSERT( pc != NULL );
    }
}

extern void hal_cli_print_str(const char *s)
{
    while(*s)
    {
        cli_print_char(*s++);
    }
}

#if 0
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

