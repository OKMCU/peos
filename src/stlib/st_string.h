/**
  ******************************************************************************
  * @file    stdstr.h
  * @author  SUN Wentao
  * @version V0.0.1
  * @date    17-DEC-2018
  * @brief   
  ******************************************************************************
  */ 

#ifndef __ST_STRING_H__
#define __ST_STRING_H__

#ifndef NULL
#define NULL 0
#endif

#ifndef _SIZE_T
#define _SIZE_T
typedef unsigned int size_t;
#endif

extern void  *st_memset   ( void *s, char val, size_t n );
extern int    st_strcmp   ( const char *dest, const char *source );
extern size_t st_strlen   ( const char *s );
extern char  *st_strchr   ( const char *p, int ch );
extern int    st_strspn   ( const char *p, const char *s );
extern char  *st_strpbrk  ( const char *s1, const char *s2 );
extern char  *st_strtok_r ( char *s, const char *delim, char **save_ptr );


#endif

