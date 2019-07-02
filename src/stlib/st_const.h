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
#ifndef __ST_CONST_H__
#define __ST_CONST_H__

#ifndef TRUE
#define TRUE    1
#endif

#ifndef FALSE
#define FALSE   0
#endif

#ifndef NULL
#define NULL    0
#endif

    /* minimum values of exact-width signed integer types */
#ifndef INT8_MIN
#define INT8_MIN                   -128
#endif

#ifndef INT16_MIN
#define INT16_MIN                -32768
#endif

#ifndef INT32_MIN
#define INT32_MIN          (~0x7fffffff)   /* -2147483648 is unsigned */
#endif

    /* maximum values of exact-width signed integer types */
#ifndef INT8_MAX
#define INT8_MAX                    127
#endif

#ifndef INT16_MAX
#define INT16_MAX                 32767
#endif

#ifndef INT32_MAX
#define INT32_MAX            2147483647
#endif

    /* maximum values of exact-width unsigned integer types */
#ifndef UINT8_MAX
#define UINT8_MAX                   255
#endif

#ifndef UINT16_MAX
#define UINT16_MAX                65535
#endif

#ifndef UINT32_MAX
#define UINT32_MAX           4294967295u
#endif

    /* limit of size_t */
#ifndef SIZE_MAX
#define SIZE_MAX 4294967295u
#endif


#endif /* __ST_CONST_H__ */

/* end of st_const.h */



