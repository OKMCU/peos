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
#include "st.h"

#if (ST_ASSERT_EN > 0)
extern void    st_assert_handler( void )
{
    volatile uint8_t i = 0;
    while(1)
    {
        i++;
    }
}



/* ------------------------------------------------------------------------------------------------
 *                                    Compile Time Assertions
 * ------------------------------------------------------------------------------------------------
 */
 
/*
 *  This macro compares the size of the first parameter to the integer value
 *  of the second parameter.  If they do not match, a compile time error for
 *  negative array size occurs (even gnu chokes on negative array size).
 *
 *  This compare is done by creating a typedef for an array.  No variables are
 *  created and no memory is consumed with this check.  The created type is
 *  used for checking only and is not for use by any other code.  The value
 *  of 10 in this macro is arbitrary, it just needs to be a value larger
 *  than one to result in a positive number for the array size.
 */
#define ST_ASSERT_SIZE(x,y) typedef char x ## _assert_size_t[-1+10*(sizeof(x) == (y))]

/* integrity check of type sizes */
ST_ASSERT_SIZE(  int8_t, 1);
ST_ASSERT_SIZE( uint8_t, 1);
ST_ASSERT_SIZE( int16_t, 2);
ST_ASSERT_SIZE(uint16_t, 2);
ST_ASSERT_SIZE( int32_t, 4);
ST_ASSERT_SIZE(uint32_t, 4);

#endif /* (ST_ASSERT_EN > 0) */
