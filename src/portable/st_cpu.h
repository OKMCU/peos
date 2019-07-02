#ifndef __ST_CPU_H__
#define __ST_CPU_H__


#define     __REG                             
#define     __IRAM                            
#define     __XRAM                            
#define     __FLASH                       const

    /* exact-width signed integer types */
typedef   signed          char int8_t;
typedef   signed short     int int16_t;
typedef   signed           int int32_t;

    /* exact-width unsigned integer types */
typedef unsigned          char uint8_t;
typedef unsigned short     int uint16_t;
typedef unsigned           int uint32_t;

__attribute__( ( always_inline ) ) static __inline void __st_exit_critical(void)
{
  __asm volatile ("cpsie i");
}

__attribute__( ( always_inline ) ) static __inline void __st_enter_critical(void)
{
  __asm volatile ("cpsid i");
}

#define     ST_ENTER_CRITICAL()         __st_enter_critical()
#define     ST_EXIT_CRITICAL()          __st_exit_critical()


#endif
