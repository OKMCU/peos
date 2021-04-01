/**
  ******************************************************************************
  * @file    os_string.c
  * @author  SUN Wentao
  * @version V0.0.1
  * @date    17-DEC-2018
  * @brief   
  ******************************************************************************
  */ 


#include "components/libc/libc_string.h"

extern void *libc_memset  (void *s, char val, os_size_t n)
{
    char *p_mem;
    p_mem = (char *)s;
    while( n-- > 0 )
    {
        *p_mem++ = val;
    }

    return s;
}

extern os_size_t libc_strlen ( const char *s )
{
    os_size_t len = 0;
    while(s[len++]);
    return len;
}

extern int libc_strcmp(const char *dest, const char *source)
{
    while (*dest && *source && (*dest == *source))
    {
        dest++;
        source++;
    }
    return *dest - *source;
}

extern char *libc_strchr(const char *p, int ch)
{
	char c;

	c = ch;
	for (;; ++p) {
		if (*p == c)
			return ((char *)p);
		if (*p == '\0')
			return (NULL);
	}
	/* NOTREACHED */
}

extern int libc_strspn(const char *p, const char *s)
{
	int i, j;

	for (i = 0; p[i]; i++) {
		for (j = 0; s[j]; j++) {
			if (s[j] == p[i])
				break;
		}
		if (!s[j])
			break;
	}
	return (i);
}

extern char *libc_strpbrk(const char *s1, const char *s2)
{
	for (; *s1; s1++) {
		if (str_chr (s2, *s1) != 0)
			return ((char *)s1);
	}
	return (0);
}

extern char *libc_strtok_r(char *s, const char *delim, char **save_ptr) 
{     
    char *token;
     
    if (s == NULL) s = *save_ptr;     
     
    /* Scan leading delimiters.  */     
    s += str_spn(s, delim);     
    if (*s == '\0')      
        return NULL;     
     
    /* Find the end of the token.  */     
    token = s;     
    s = str_pbrk(token, delim);     
    if (s == NULL)     
        /* This token finishes the string.  */     
        *save_ptr = str_chr(token, '\0');     
    else {     
        /* Terminate the token and make *SAVE_PTR point past it.  */     
        *s = '\0';     
        *save_ptr = s + 1;     
    }     
     
    return token;     
}

