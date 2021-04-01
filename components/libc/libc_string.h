/**
  ******************************************************************************
  * @file    os_string.h
  * @author  SUN Wentao
  * @version V0.0.1
  * @date    17-DEC-2018
  * @brief   
  ******************************************************************************
  */ 

#ifndef __LIBC_STRING_H__
#define __LIBC_STRING_H__

extern void        *libc_memset   ( void *s, char val, os_size_t n );
extern int          libc_strcmp   ( const char *dest, const char *source );
extern os_size_t    libc_strlen   ( const char *s );
extern char        *libc_strchr   ( const char *p, int ch );
extern int          libc_strspn   ( const char *p, const char *s );
extern char        *libc_strpbrk  ( const char *s1, const char *s2 );
extern char        *libc_strtok_r ( char *s, const char *delim, char **save_ptr );

#endif

