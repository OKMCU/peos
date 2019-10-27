#ifndef __ST_PORTABLE_H__
#define __ST_PORTABLE_H__

#include <intrinsics.h>
#include <stdint.h>
#include <string.h>
#include "st_config.h"

#define __REG                             
#define __IRAM                            
#define __XRAM                            
#define __FLASH             const
#define __STATIC_INLINE     static

typedef uint8_t     st_uint8_t;
typedef uint16_t    st_uint16_t;
typedef uint32_t    st_uint32_t;
typedef int8_t      st_int8_t;
typedef int16_t     st_int16_t;
typedef int32_t     st_int32_t;
typedef float       st_fpt32_t;
typedef double      st_fpt64_t;
typedef size_t      st_size_t;

#define ST_ENTER_CRITICAL()         __disable_interrupt()
#define ST_EXIT_CRITICAL()          __enable_interrupt()
#define st_memset(ptr, val, len)    memset(ptr, val, len)

extern void st_hal_init( void );
extern void st_idle_hook( void );

#if ST_ASSERT_EN > 0
extern void st_assert_failed(char *file, st_uint32_t line);
#endif

#endif
