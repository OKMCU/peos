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
#include "st.h"
#include "hal_drivers.h"
#include "components/cli/cli.h"

#if CLI_TX_BUF_SIZE > 0
#include "components/fifo/fifo.h"
#endif

#include "components/utilities/stringx.h"

/* Exported variables --------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define CLI_MAX_KEY_LEN             7

#define ASCII_LF                    0x0A
#define ASCII_CR                    0x0D
#define ASCII_BACKSPACE             0x7F

/* Private typedef -----------------------------------------------------------*/
typedef struct cli_key {
    st_uint8_t len;
    char val[CLI_MAX_KEY_LEN];
} cli_key_t;

typedef struct cli_cmd {
    st_uint8_t len;
    char str[CLI_MAX_CMD_LENGTH];
} cli_cmd_t;

/* Private macro -------------------------------------------------------------*/
#define IS_CHAR_CONTROL(c)       ( c <= 31  )
#define IS_CHAR_PRINTABLE(c)     ( c >= 32 && c <= 127 )

/* Private variables ---------------------------------------------------------*/
static cli_key_t *p_cli_key;
static cli_cmd_t *p_cli_cmd;

#if CLI_TX_BUF_SIZE > 0
static void *p_cli_tx_fifo;
#endif

static st_uint8_t cli_task_id;

/* Private function prototypes -----------------------------------------------*/
static void cli_uart_driver_callback( st_uint8_t event );
static void cli_rx_key( const cli_key_t *p_key );
static void cli_process_cmd( char *p_cmd );

/* Exported function implementations -----------------------------------------*/
void cli_init( st_uint8_t task_id )
{
    hal_uart_config_t cfg;
    
    cli_task_id = task_id;

    p_cli_key = NULL;
    p_cli_cmd = NULL;

#if CLI_TX_BUF_SIZE > 0
    p_cli_tx_fifo = NULL;
#endif
    
    cfg.baud_rate = CLI_UART_BAUDRATE;
    cfg.data_bits = HAL_UART_DATA_BITS_8;
    cfg.stop_bits = HAL_UART_STOP_BITS_1;
    cfg.parity    = HAL_UART_PARITY_NONE;
    cfg.bit_order = HAL_UART_BIT_ORDER_LSB;
    cfg.invert    = HAL_UART_NRZ_NORMAL;
    cfg.callback  = cli_uart_driver_callback;

    hal_uart_open( CLI_UART_PORT, &cfg );
}

void cli_task( st_int8_t event_id )
{
    cli_key_t *p_key;

    ST_ASSERT( event_id ==  ST_TASK_EVT_MSG );

    p_key = (cli_key_t *)st_msg_recv( st_get_task_id_self() );

    ST_ASSERT( p_key != NULL );
    cli_rx_key( p_key );
    st_msg_delete( p_key );
}

/*
void cli_enable( void )
{
    hal_uart_open( CLI_UART_PORT );
}

void cli_disable( void )
{
    void *p_msg;
    
    hal_uart_close( CLI_UART_PORT );
#if CLI_TX_BUF_SIZE > 0
    if( p_cli_tx_fifo )
    {
        fifo_delete( p_cli_tx_fifo );
        p_cli_tx_fifo = NULL;
    }
#endif

    if( p_cli_cmd )
    {
        st_mem_free( p_cli_cmd );
        p_cli_cmd = NULL;
    }

    if( p_cli_key )
    {
        st_msg_delete( p_cli_key );
        p_cli_key = NULL;
    }

    while( (p_msg = st_msg_recv(cli_task_id)) != NULL )
    {
        st_msg_delete( p_msg );
    }
}
*/

void cli_register_cmds( const cli_cmd_mapping_t *cmd )
{
    
}

void cli_print_char( char ch )
{
#if CLI_TX_BUF_SIZE > 0
    st_uint8_t *pc;

    if(p_cli_tx_fifo == NULL)
    {
        if( hal_uart_tx_buf_free(CLI_UART_PORT) )
        {
            hal_uart_putc( CLI_UART_PORT, (st_uint8_t)ch );
        }
        else
        {
            p_cli_tx_fifo = fifo_create();
            ST_ASSERT( p_cli_tx_fifo != NULL );
            pc = fifo_put( p_cli_tx_fifo, (st_uint8_t)ch );
            ST_ASSERT( pc != NULL );
        }
    }
    else
    {
        if( fifo_len(p_cli_tx_fifo) < CLI_TX_BUF_SIZE )
        {
            pc = fifo_put( p_cli_tx_fifo, (st_uint8_t)ch );
        }
        else
        {
            while( hal_uart_tx_buf_free(CLI_UART_PORT) == 0 );
            hal_uart_putc( CLI_UART_PORT, fifo_get(p_cli_tx_fifo) );
            pc = fifo_put( p_cli_tx_fifo, (st_uint8_t)ch );
        }
        ST_ASSERT( pc != NULL );
    }
#else
    while( hal_uart_tx_buf_free(CLI_UART_PORT) == 0 );
    hal_uart_putc( CLI_UART_PORT, ch );
#endif
}

void cli_print_str(const char *s)
{
    while(*s)
    {
        cli_print_char(*s++);
    }
}


void cli_print_sint(st_int32_t num)
{
    char str[SINT_STR_LEN_MAX];
    st_uint8_t len;
    st_uint8_t i;
    
    len = tostr_sint(num, str);
    
    for(i = 0; i < len; i++)
    {
        cli_print_char(str[i]);
    }
    
}

void cli_print_uint(st_uint32_t num)
{
    char str[UINT_STR_LEN_MAX];
    st_uint8_t len;
    st_uint8_t i;
    
    len = tostr_uint(num, str);
    
    for(i = 0; i < len; i++)
    {
        cli_print_char( str[i] );
    }
}

void cli_print_hex8(st_uint8_t num)
{
    char str[HEX8_STR_LEN_MAX];
    st_uint8_t len;
    st_uint8_t i;
    
    len = tostr_hex8(num, str);
    
    for(i = 0; i < len; i++)
    {
        cli_print_char(str[i]);
    }
}


void cli_print_hex16(st_uint16_t num)
{
    char str[HEX16_STR_LEN_MAX];
    st_uint8_t len;
    st_uint8_t i;
    
    len = tostr_hex16(num, str);
    
    for(i = 0; i < len; i++)
    {
        cli_print_char( str[i] );
    }
}

void cli_print_hex32(st_uint32_t num)
{
    char str[HEX32_STR_LEN_MAX];
    st_uint8_t len;
    st_uint8_t i;
    
    len = tostr_hex32(num, str);
    
    for(i = 0; i < len; i++)
    {
        cli_print_char(str[i]);
    }
}

/* Private function implementations ------------------------------------------*/
static void cli_uart_driver_callback( st_uint8_t event )
{
    st_uint8_t size;
    st_uint8_t i;
    st_uint8_t byte;
    
    switch ( event )
    {
        case HAL_UART_EVENT_RXD:
            size = hal_uart_rx_buf_used(CLI_UART_PORT);
            for( i = 0; i < size; i++ )
            {
                if( p_cli_key == NULL )
                {
                    p_cli_key = (cli_key_t *)st_msg_create( sizeof(cli_key_t), 0 );
                    if( p_cli_key != NULL )
                    {
                        p_cli_key->len = 0;
                    }
                }
                
                if( p_cli_key )
                {
                    byte = hal_uart_getc(CLI_UART_PORT);
                    if( p_cli_key->len < CLI_MAX_KEY_LEN )
                    {
                        p_cli_key->val[p_cli_key->len++] = byte;
                        if(p_cli_key->len == CLI_MAX_KEY_LEN)
                        {
                            st_msg_send( p_cli_key, cli_task_id );
                            p_cli_key = NULL;
                        }
                    }
                }
            }
        break;

        case HAL_UART_EVENT_TXD:

#if CLI_TX_BUF_SIZE > 0
            if( p_cli_tx_fifo )
            {
                size = hal_uart_tx_buf_free(CLI_UART_PORT);
                while( size-- )
                {
                    if( fifo_len(p_cli_tx_fifo) )
                    {
                        hal_uart_putc(CLI_UART_PORT, fifo_get(p_cli_tx_fifo));
                    }
                    else
                    {
                        fifo_delete( p_cli_tx_fifo );
                        p_cli_tx_fifo = NULL;
                        break;
                    }
                }
            }
#endif
        break;

        case HAL_UART_EVENT_OVF:
            // ignored
        break;

        case HAL_UART_EVENT_PERR:
            // ignored
        break;

        case HAL_UART_EVENT_IDLE:
            if( p_cli_key )
            {
                st_msg_send( p_cli_key, cli_task_id );
                p_cli_key = NULL;
            }
        break;
    }
}

static void cli_rx_key( const cli_key_t *p_key )
{
    char c;
#if 0
    st_uint8_t i;
    

    for( i = 0; i < p_key->len; i++ )
    {
        c = p_key->val[i];
        cli_print_hex8( c );
        cli_print_char( ' ' );
    }
    cli_print_str( "\r\n" );
#endif

    //cli_print_char(c);
    if( p_key->len == 1 )
    {
        c = p_key->val[0];
        if( IS_CHAR_PRINTABLE(c) )
        {
            if( c != ASCII_BACKSPACE )
            {
                if( p_cli_cmd == NULL )
                {
                    p_cli_cmd = (cli_cmd_t *)st_mem_alloc( sizeof(cli_cmd_t) );
                    if( p_cli_cmd )
                    {
                        p_cli_cmd->len = 0;
                    }
                }

                if( p_cli_cmd )
                {
                    if( p_cli_cmd->len < CLI_MAX_CMD_LENGTH - 1 )
                    {
                        p_cli_cmd->str[p_cli_cmd->len++] = c;
                        cli_print_char( c );
                    }
                }
            }
            else
            {
                if( p_cli_cmd )
                {
                    if( p_cli_cmd->len )
                    {
                        cli_print_char( c );
                        p_cli_cmd->len--;
                        
                        if( p_cli_cmd->len == 0 )
                        {
                            st_mem_free( p_cli_cmd );
                            p_cli_cmd = NULL;
                        }
                    }
                }
            }
        }
        else if( IS_CHAR_CONTROL(c) )
        {
            cli_print_char( c );
            if( c == ASCII_CR )
            {
                cli_print_char( '\n' );
                if( p_cli_cmd )
                {
                    p_cli_cmd->str[p_cli_cmd->len] = '\0';

                    cli_process_cmd( p_cli_cmd->str );
                    st_mem_free( p_cli_cmd );
                    p_cli_cmd = NULL;
                }
            }
        }
    }
}

static void cli_process_cmd( char *p_cmd )
{
    cli_print_str( "CMD:" );
    cli_print_str( p_cmd );
    cli_print_str( "\r\n" );
}

/****** (C) COPYRIGHT 2019 Single-Thread Development Team. *****END OF FILE****/
