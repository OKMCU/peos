/*******************************************************************************
 * Copyright (c) 2021-2022, PEOS Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date         Author       Notes
 * 2021-10-28   Wentao SUN   first version
 * 
 ******************************************************************************/

#ifndef __STRINGX_H__
#define __STRINGX_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes -------------------------------------------------------------------*/
#include "os.h"

/* Exported define ------------------------------------------------------------*/
#define SINT_STR_LEN_MAX    11
#define UINT_STR_LEN_MAX    10
#define HEX8_STR_LEN_MAX    2
#define HEX16_STR_LEN_MAX   4
#define HEX32_STR_LEN_MAX   8

/* Exported typedef -----------------------------------------------------------*/
/* Exported macro -------------------------------------------------------------*/
/* Exported variables ---------------------------------------------------------*/
/* Exported function prototypes -----------------------------------------------*/
os_uint8_t hexstr2uint(const char *str, os_uint32_t *num);
os_uint8_t decstr2uint(const char *str, os_uint32_t *num);
os_uint8_t decstr2sint(const char *str, os_int32_t *num);
os_uint8_t tostr_sint(os_int32_t num, char *str);
os_uint8_t tostr_uint(os_uint32_t num, char *str);
os_uint8_t tostr_hex8(os_uint8_t num, char *str);
os_uint8_t tostr_hex16(os_uint16_t num, char *str);
os_uint8_t tostr_hex32(os_uint32_t num, char *str);
char *str_starts_with(char *str1, const char *str2);
char *str_ends_with(char *str1, const char *str2);

#ifdef __cplusplus
}
#endif

#endif
/****** (C) COPYRIGHT 2021 PEOS Development Team. *****END OF FILE****/
