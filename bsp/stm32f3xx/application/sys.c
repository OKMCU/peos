/******************************************************************************

 @file  app_event_main.c

 @brief 

 Group: 
 Target Device: 

 ******************************************************************************
 

 ******************************************************************************
 Release Name: 
 Release Date: 
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
/**************************************************************************************************
 * TYPE DEFINES
 **************************************************************************************************/

 /**************************************************************************************************
 * LOCAL API DECLARATION
 **************************************************************************************************/

/**************************************************************************************************
 * CONSTANTS
 **************************************************************************************************/

/**************************************************************************************************
 * GLOBAL VARIABLES
 **************************************************************************************************/

extern void app_event_main_por( void )
{
#if APP_CLI_EN > 0
    hal_cli_print_str( "\r\nThis is OSAL demo project on Nuvoton M051 series MCU.\r\n" );
    hal_cli_print_str( "Power on reset.\r\n" );

    hal_cli_print_str( "Free-OSAL Version " );
    hal_cli_print_str( OSAL_VERSION );
    hal_cli_print_str( "\r\n" );
    
    hal_cli_print_str( "Firmware Version " );
    hal_cli_print_str( FIRMWARE_VER );
    hal_cli_print_str( "\r\n" );

    hal_cli_print_str( "Hardware Version " );
    hal_cli_print_str( HARDWARE_VER );
    hal_cli_print_str( "\r\n" );
#endif

#if APP_KEY_EN > 0
    app_info.key_state = 0x00;
#endif

    osal_event_set( TASK_ID_APP_MAIN, TASK_EVT_APP_MAIN_IDLE );
}

extern void app_event_main_osal_exception( void )
{
#if (APP_CLI_EN > 0)
    hal_cli_print_str( "ERROR! " );
    hal_cli_print_str( "OSAL_EXCEPTION!\r\n" );
#endif
}

extern void app_event_main_idle( void )
{
    uint8_t u8_tmp;

#if APP_KEY_EN > 0
    //polling the keys
    u8_tmp = hal_key_read();
    if( u8_tmp != app_info.key_state )
    {
        app_info.key_state = u8_tmp;
        osal_timer_event_create( TASK_ID_APP_KEY, TASK_EVT_APP_KEY_UPDATE, HAL_KEY_DEBOUNCE_TIME );
    }
#endif

    osal_event_set( TASK_ID_APP_MAIN, TASK_EVT_APP_MAIN_IDLE );
}

/**************************************************************************************************
**************************************************************************************************/

