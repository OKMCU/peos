/******************************************************************************

 @file  

 @brief 

 Group: 
 Target Device: 

 ******************************************************************************
 

 ******************************************************************************
 Release Name: 
 Release Date: 2016-06-09 06:57:09
 *****************************************************************************/
#ifndef __ST_MEM_H__
#define __ST_MEM_H__

#include "portable/umm_malloc/umm_malloc.h"

/* ------------------------------------------------------------------------------------------------
 *                                           Macros
 * ------------------------------------------------------------------------------------------------
 */

#define     st_mem_init()                 umm_init()
#define     st_mem_alloc(size)            umm_malloc(size)
#define     st_mem_calloc(num, size)      umm_calloc(num, size)
#define     st_mem_realloc(ptr, size)     umm_realloc(ptr, size)
#define     st_mem_free(ptr)              umm_free(ptr)

#endif //__ST_MEM_H__
