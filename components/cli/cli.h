#ifndef __CLI_H__
#define __CLI_H__

#include "st.h"

typedef struct {
    char *string;
    void (*handler)( st_uint8_t argc, char **argv );
} cli_cmd_t;

void cli_init( void );
void cli_register_cmds( const cli_cmd_t *cmd );
void cli_print_char( char ch );
void cli_print_str( const char *s );
void cli_print_sint( st_int32_t num );
void cli_print_uint( st_uint32_t num );
void cli_print_hex8( st_uint8_t num );
void cli_print_hex16( st_uint16_t num );
void cli_print_hex32( st_uint32_t num );
void cli_deinit( void );


#endif /* __CLI_H__ */
