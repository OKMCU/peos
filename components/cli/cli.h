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
#include "os.h"

/* Exported define ------------------------------------------------------------*/
/* Exported typedef -----------------------------------------------------------*/
typedef struct {
    char *string;
    void (*handler)( os_uint8_t argc, char **argv );
} cli_cmd_mapping_t;

/* Exported macro -------------------------------------------------------------*/
/* Exported variables ---------------------------------------------------------*/
/* Exported function prototypes -----------------------------------------------*/
void cli_init( os_uint8_t task_id );
void cli_task( os_int8_t event_id );

void cli_register_cmds( const cli_cmd_mapping_t *cmd );
void cli_print_char( char ch );
void cli_print_str( const char *s );
void cli_print_sint( os_int32_t num );
void cli_print_uint( os_uint32_t num );
void cli_print_hex8( os_uint8_t num );
void cli_print_hex16( os_uint16_t num );
void cli_print_hex32( os_uint32_t num );

#ifdef __cplusplus
}
#endif

#endif /* __CLI_H__ */

