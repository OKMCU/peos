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
 
/* Includes ------------------------------------------------------------------*/
#include "os.h"
#include "components/utilities/stringx.h"

/* Exported variables --------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
#define SWAP(a, b)  a ^= b; b ^= a; a ^= b

os_uint8_t hexstr2uint(const char *str, os_uint32_t *num)
{
    char ch;
    os_uint8_t cnt = 0;
    os_uint8_t lenPrefix = 0;
    os_uint32_t val = 0;
    os_int32_t test_09;
    os_int32_t test_AF;
    os_int32_t test_af;
    
    if(str[0] == '0')
    {
        if(str[1] == 'x' || str[1] == 'X')
        {
            cnt = 2;
            lenPrefix = 2;
        }
    }
    //////////////////////
    
    for(;;)
    {
        ch = str[cnt];
        
        if(ch == '\0')
            break;

        cnt++;
        
        if(cnt-lenPrefix > HEX32_STR_LEN_MAX)
        {
            return 0;
        }
        
        test_09 = (ch - '0') * (ch - '9');
        test_AF = (ch - 'A') * (ch - 'F');
        test_af = (ch - 'a') * (ch - 'f');
        if(test_09 * test_AF * test_af <= 0)
        {
            val <<= 4;
            if(test_09 <= 0)
            {
                val += ch - '0';
            }
            else if(test_AF <= 0)
            {
                val += ch - 'A' + 10;
            }
            else if(test_af <= 0)
            {
                val += ch - 'a' + 10;
            }
            else
            {
                return 0;
            }
        }
        else
        {
            return 0;
        } 
    }

    if(cnt > lenPrefix)
    {
        if(num)
        {
            *num = val;
        }
    }
    
    return cnt;
}

os_uint8_t decstr2uint(const char *str, os_uint32_t *num)
{
    char ch;
    os_uint8_t cnt = 0;
    os_uint32_t val = 0;

    

    for(;;)
    {
        ch = str[cnt];
        
        if(ch == '\0')
            break;
        
        cnt++;
        
        if(cnt > UINT_STR_LEN_MAX)
            return 0;

        
        if(ch >= '0' && ch <= '9')
        {
            val *= 10;
            val += ch - '0';
        }
        else
        {
            return 0;
        }
        
    }

    if(cnt > 0)
    {
        if(num)
        {
            *num = val;
        }
    }
    
    return cnt;
}

os_uint8_t decstr2sint(const char *str, os_int32_t *num)
{
    os_int32_t val = 0;
    os_uint32_t u32tmp;
    os_uint8_t cnt;
    
    if(str[0] == '-')
    {
        cnt = decstr2uint(str+1, &u32tmp);
    }
    else
    {
        cnt = decstr2uint(str, &u32tmp);
    }

    if(cnt > 0)
    {
        if(str[0] == '-')
        {
            if(u32tmp > (os_uint32_t)INT32_MAX + 1)
            {
                return 0;
            }
            val = (os_int32_t)u32tmp;
            val =-val;
            cnt++;
        }
        else
        {
            if(u32tmp > (os_uint32_t)INT32_MAX)
            {
                return 0;
            }
            val = (os_int32_t)u32tmp;
        }
        
        if(num)
        {
            *num = val;
        }
    }
    
    
    return  cnt;
}

os_uint8_t tostr_sint(os_int32_t num, char *str)
{
    os_uint8_t numLen = 0;
    os_uint8_t i;
    os_uint8_t j;
    os_int8_t flag;
    os_uint32_t absNum;
    
    if(num < 0)
    {
        flag = -1;
        absNum = (os_uint32_t)(-num);
    }
    else
    {
        flag = 1;
        absNum = (os_uint32_t)num;
    }
    
    do{
        str[numLen++] = '0' + (absNum%10);
        absNum /= 10;
    }
    while(absNum != 0);

    if(flag == -1)
    {
        str[numLen++] = '-';
    }

    for(i = 0; i < numLen; i++)
    {
        if(numLen >= i + 1)
        {
            j = numLen - 1 - i;
            if(i < j)
            {
                SWAP(str[i], str[j]);
            }
            else
            {
                break;
            }
        }
        else
        {
            break;
        }
    }
    
    return numLen;
}

os_uint8_t tostr_uint(os_uint32_t num, char *str)
{
    os_uint8_t numLen = 0;
    os_uint8_t i;
    os_uint8_t j;
    
    do{
        str[numLen++] = '0' + (num%10);
        num /= 10;
    }
    while(num != 0);

    for(i = 0; i < numLen; i++)
    {
        if(numLen >= i + 1)
        {
            j = numLen - 1 - i;
            if(i < j)
            {
                SWAP(str[i], str[j]);
            }
            else
            {
                break;
            }
        }
        else
        {
            break;
        }
    }

    return numLen;
}

os_uint8_t tostr_hex8(os_uint8_t num, char *str)
{
    str[0] = num >> 4;
    str[1] = num & 0x0F;
    
    str[0] = (str[0] <= 9) ? (str[0] + '0') : (str[0] + 'A' - 10);
    str[1] = (str[1] <= 9) ? (str[1] + '0') : (str[1] + 'A' - 10);
    
    return HEX8_STR_LEN_MAX;
}

os_uint8_t tostr_hex16(os_uint16_t num, char *str)
{
    os_int8_t i;
    
    for(i = HEX16_STR_LEN_MAX-1; i >=0; i--)
    {
        str[i] = num & 0xF;
        str[i] = (str[i] <= 9) ? (str[i] + '0') : (str[i] + 'A' - 10);
        num >>= 4;
    }

    return HEX16_STR_LEN_MAX;
}

os_uint8_t tostr_hex32(os_uint32_t num, char *str)
{
    os_int8_t i;
    
    for(i = HEX32_STR_LEN_MAX-1; i >=0; i--)
    {
        str[i] = num & 0xF;
        str[i] = (str[i] <= 9) ? (str[i] + '0') : (str[i] + 'A' - 10);
        num >>= 4;
    }
    
    return HEX32_STR_LEN_MAX;
}

char *str_starts_with(char *str1, const char *str2)
{
    if(str1 != NULL && str2 != NULL)
    {
        while(*str1)
        {
            if(*str1 == *str2)
            {
                str1++;
                str2++;
                if(*str2 == '\0')
                {
                    return str1;
                }
            }
            else
            {
                break;
            }
        }
    }
        
    return NULL;
}

char *str_ends_with(char *str1, const char *str2)
{
    os_uint16_t len1;
    os_uint16_t len2;

    if(str1 != NULL && str2 != NULL)
    {
        len1 = os_strlen(str1);
        len2 = os_strlen(str2);

        if(len1 >= len2)
        {
            if(os_strcmp(str1+len1-len2, str2) == 0)
            {
                return (str1+len1-len2);
            }
        }
    }
    return NULL;
    
}

