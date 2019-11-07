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

#ifndef __STRINGX_H__
#define __STRINGX_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes -------------------------------------------------------------------*/
#include "st.h"

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
st_uint8_t hexstr2uint(const char *str, st_uint32_t *num);
st_uint8_t decstr2uint(const char *str, st_uint32_t *num);
st_uint8_t decstr2sint(const char *str, st_int32_t *num);
st_uint8_t tostr_sint(st_int32_t num, char *str);
st_uint8_t tostr_uint(st_uint32_t num, char *str);
st_uint8_t tostr_hex8(st_uint8_t num, char *str);
st_uint8_t tostr_hex16(st_uint16_t num, char *str);
st_uint8_t tostr_hex32(st_uint32_t num, char *str);
char *str_starts_with(char *str1, const char *str2);
char *str_ends_with(char *str1, const char *str2);

#ifdef __cplusplus
}
#endif

#endif
/****** (C) COPYRIGHT 2019 Single-Thread Development Team. *****END OF FILE****/
