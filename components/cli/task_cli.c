/******************************************************************************

 @file  app_task_cli.c

 @brief This file contains the command line interface service.

 Group: 
 Target Device: 

 ******************************************************************************
 

 ******************************************************************************
 Release Name: 
 Release Date: 2016-06-09 06:57:09
 *****************************************************************************/

/**************************************************************************************************
 * INCLUDES
 **************************************************************************************************/
#include "osal.h"
#include "hal.h"
#include "app.h"

#include "main.h"

#include "stdstr.h"
#include "stringx.h"

#if APP_CLI_EN > 0
/**************************************************************************************************
 * TYPES
 **************************************************************************************************/
typedef struct cli_cmd_t {
    char *str;
    uint16_t lenStr;
    uint16_t lenBuf;
} CLI_CMD_t;

/**************************************************************************************************
 * CONSTANTS
 **************************************************************************************************/
#define CLI_CMD_LEN_MIN     32
#define CLI_CMD_LEN_MAX     (8*CLI_CMD_LEN_MIN)

#define ASCII_VALUE_BACKSPACE   127

/**************************************************************************************************
 * GLOBAL VARIABLES
 **************************************************************************************************/
static CLI_CMD_t *p_cli_cmd;

extern void app_task_cli_init ( void )
{
     p_cli_cmd = NULL;
}



static void app_task_cli_handle_rx_char( char c )
{
    if( c != '\n' )
        hal_cli_putchar( c );

    if( c != '\r' && c != '\n' && c != 0x7F )
    {
        if( p_cli_cmd == NULL )
        {
            p_cli_cmd = osal_mem_alloc( sizeof( CLI_CMD_t ) );
            APP_ASSERT( p_cli_cmd != NULL );
            p_cli_cmd->str = osal_mem_alloc( CLI_CMD_LEN_MIN );
            APP_ASSERT( p_cli_cmd->str != NULL);
            p_cli_cmd->lenStr = 0;
            p_cli_cmd->lenBuf = CLI_CMD_LEN_MIN;
        }

        //check if enlarger the buffer is needed
        if( p_cli_cmd->lenStr >= p_cli_cmd->lenBuf - 1 )
        {
            if( p_cli_cmd->lenStr < CLI_CMD_LEN_MAX )
            {
                p_cli_cmd->lenBuf += CLI_CMD_LEN_MIN;
                p_cli_cmd->str = osal_mem_realloc( p_cli_cmd->str, p_cli_cmd->lenBuf );
                APP_ASSERT( p_cli_cmd->str != NULL);
            }
        }

        if( p_cli_cmd->lenStr < p_cli_cmd->lenBuf - 1 )
        {
            p_cli_cmd->str[p_cli_cmd->lenStr++] = c;
        }
    }
    else if( c == '\r' )
    {
        hal_cli_putchar( '\n' );
        
        if( p_cli_cmd != NULL )
        {
            p_cli_cmd->str[p_cli_cmd->lenStr] = '\0';

            app_event_cli_process_cmd( p_cli_cmd->str );
            
            osal_mem_free( p_cli_cmd->str );
            osal_mem_free( p_cli_cmd );
            p_cli_cmd = NULL;
        }
    }
    else if( c == ASCII_VALUE_BACKSPACE )
    {
        if( p_cli_cmd != NULL )
        {
            if( p_cli_cmd->lenStr > 0 )
            {
                p_cli_cmd->lenStr--;
                if( p_cli_cmd->lenStr )
                {
                    if( p_cli_cmd->lenBuf - p_cli_cmd->lenStr > CLI_CMD_LEN_MIN )
                    {
                        p_cli_cmd->lenBuf -= CLI_CMD_LEN_MIN;
                        p_cli_cmd->str = osal_mem_realloc( p_cli_cmd->str, p_cli_cmd->lenBuf );
                        APP_ASSERT( p_cli_cmd->str != NULL);
                    }
                }
                else
                {
                    osal_mem_free( p_cli_cmd->str );
                    osal_mem_free( p_cli_cmd );
                    p_cli_cmd = NULL;
                }
            }
        }
    }
}

/**************************************************************************************************
 * @fn      app_task_cli
 *
 * @brief   app_task_cli
 *
 * @param   task_id - CLI Task Id
 *          events - events
 *
 * @return  None
 **************************************************************************************************/
void app_task_cli( uint8_t task_id, uint8_t event_id )
{
    uint8_t len;
	
	task_id = task_id;
	
    switch ( event_id )
    {
        case TASK_EVT_APP_CLI_RX_CHAR:
        {
            len = hal_cli_rx_len();
            while ( len-- )
            {
                app_task_cli_handle_rx_char( hal_cli_getchar() );
            }
        }
        break;

        case TASK_EVT_APP_CLI_RX_OVF:
        {

        }
        break;
        
        default:
            APP_ASSERT_FORCED();
        break;
    }
}

#endif
/**************************************************************************************************
**************************************************************************************************/

