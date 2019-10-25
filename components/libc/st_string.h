/**
  ******************************************************************************
  * @file    st_string.h
  * @author  SUN Wentao
  * @version V0.0.1
  * @date    17-DEC-2018
  * @brief   
  ******************************************************************************
  */ 

#ifndef __ST_STRING_H__
#define __ST_STRING_H__

#ifndef ST_NULL
#define ST_NULL 0
#endif

#ifndef ST_SIZE_T
#define ST_SIZE_T
typedef uint32_t st_size_t;
#endif

extern void        *st_memset   ( void *s, char val, st_size_t n );
extern int          st_strcmp   ( const char *dest, const char *source );
extern st_size_t    st_strlen   ( const char *s );
extern char        *st_strchr   ( const char *p, int ch );
extern int          st_strspn   ( const char *p, const char *s );
extern char        *st_strpbrk  ( const char *s1, const char *s2 );
extern char        *st_strtok_r ( char *s, const char *delim, char **save_ptr );


#endif

