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
#ifndef __ST_ASSERT_H__
#define __ST_ASSERT_H__

/* ------------------------------------------------------------------------------------------------
 *                                           Macros
 * ------------------------------------------------------------------------------------------------
 */

/*
 *  ASSERT( expression ) - The given expression must evaluate as "true" or else the assert
 *  handler is called.  From here, the call stack feature of the debugger can pinpoint where
 *  the problem occurred.
 *
 *  ASSERT_FORCED( ) - If asserts are in use, immediately calls the assert handler.
 *
 *  ASSERT_STATEMENT( statement ) - Inserts the given C statement but only when asserts
 *  are in use.  This macros allows debug code that is not part of an expression.
 *
 *  ASSERT_DECLARATION( declaration ) - Inserts the given C declaration but only when asserts
 *  are in use.  This macros allows debug code that is not part of an expression.
 *
 *  Asserts can be disabled for optimum performance and minimum code size (ideal for
 *  finalized, debugged production code).  To disable, undefine the preprocessor
 *  symbol ST_ASSERT_EN at the project level.
 */
#ifndef ST_ASSERT_EN
#define ASSERT( expr )
#define ASSERT_FORCED()
#define ASSERT_STATEMENT( statement )
#define ASSERT_DECLARATION( declaration )
#else
extern void    st_assert_handler( void );
#define ASSERT( expr )                      st( if (!( expr )) st_assert_handler(); )
#define ASSERT_FORCED()                     st_assert_handler()
#define ASSERT_STATEMENT( statement )       st( statement )
#define ASSERT_DECLARATION( declaration )   declaration
#endif

#endif //__ST_ASSERT_H__
