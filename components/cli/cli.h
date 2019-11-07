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
 
#ifndef __CLI_H__
#define __CLI_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes -------------------------------------------------------------------*/
#include "st.h"

/* Exported define ------------------------------------------------------------*/
/* Exported typedef -----------------------------------------------------------*/
typedef struct {
    char *string;
    void (*handler)( st_uint8_t argc, char **argv );
} cli_cmd_t;

/* Exported macro -------------------------------------------------------------*/
/* Exported variables ---------------------------------------------------------*/
/* Exported function prototypes -----------------------------------------------*/
void cli_init( st_uint8_t task_id );
void cli_task( st_int8_t event_id );

void cli_enable( void );
void cli_disable( void );
void cli_register_cmds( const cli_cmd_t *cmd );
void cli_print_char( char ch );
void cli_print_str( const char *s );
void cli_print_sint( st_int32_t num );
void cli_print_uint( st_uint32_t num );
void cli_print_hex8( st_uint8_t num );
void cli_print_hex16( st_uint16_t num );
void cli_print_hex32( st_uint32_t num );

#ifdef __cplusplus
}
#endif

#endif /* __CLI_H__ */

