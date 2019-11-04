
#include "st.h"
#include "hal_drivers.h"
#include "components/cli/cli.h"
#include "components/fifo/fifo.h"

static void *tx_fifo;
static char *rx_buff;
static cli_cmd_t *cmd;
//#define IS_CHAR(x)  ( x > 0 && x <= 127 )
static void cli_task( st_int8_t event_id );

static void cli_uart_driver_callback( st_uint8_t event )
{
    st_uint8_t size;
    st_uint8_t i;
    char *p_msg;
    
    switch ( event )
    {
        case HAL_UART_EVENT_RXD:
            size = hal_uart_rx_buf_used(CLI_UART_PORT);
            if( size )
            {
                p_msg = (char *)st_msg_create(size, 0, NULL);

                if( p_msg )
                {
                    for( i = 0; i < size; i++ )
                    {
                        p_msg[i] = hal_uart_getc(CLI_UART_PORT);
                    }
                    st_msg_send( p_msg, TASK_ID_CLI );
                }
            }
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
                        break;
                    }
                }
            }
        break;

        case HAL_UART_EVENT_OVF:
            // ignored
        break;

        case HAL_UART_EVENT_PERR:
            // ignored
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

    st_task_create( TASK_ID_CLI, cli_task );
    hal_uart_open( CLI_UART_PORT, &cfg );
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

void cli_print_str(const char *s)
{
    while(*s)
    {
        cli_print_char(*s++);
    }
}

void cli_deinit( void )
{
    tx_fifo = NULL;
    rx_buff = NULL;
    hal_uart_close( CLI_UART_PORT );
    st_task_delete( TASK_ID_CLI );
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


static void cli_task( st_int8_t event_id )
{
    st_uint16_t len, i;
    char *p_msg;
    switch ( event_id )
    {
        case ST_TASK_EVT_MSG:
            p_msg = st_msg_recv( st_get_task_id_self() );
            if( p_msg != NULL )
            {
                len = st_msg_len( p_msg );
                i = 0;
                while( len-- )
                {
                    cli_print_char( p_msg[i++] );
                }
                st_msg_delete( p_msg );
            }
        break;
    }
}


