#ifndef __ST_PORTING_H__
#define __ST_PORTING_H__

#define __REG                             
#define __IRAM                            
#define __XRAM                            
#define __FLASH             const
#define __STATIC_INLINE     static

__STATIC_INLINE void __st_exit_critical(void)
{
}

__STATIC_INLINE void __st_enter_critical(void)
{
}

#define     ST_ENTER_CRITICAL()         __st_enter_critical()
#define     ST_EXIT_CRITICAL()          __st_exit_critical()

extern void st_hardware_init( void );

#endif
