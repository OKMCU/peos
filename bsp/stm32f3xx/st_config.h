#ifndef __ST_CONFIG_H__
#define __ST_CONFIG_H__

/* Automatically generated file; DO NOT EDIT. */
/* Single-Thread Configuration */
#define     ST_ASSERT_EN          1
#define     ST_MSG_EN             1
#define     ST_TIMER_EN           1
#define     ST_TIMER_STATIC_EN    1
#define     ST_TIMER_MAX          8             //meaningless if ST_TIMER_STATIC_EN == 0
#define     ST_TIMER_TIMEOUT_MAX  UINT32_MAX    //should be UINT8_MAX or UINT16_MAX or UINT32_MAX
#define     ST_MEM_EN             1

#define     ST_TASK_MAX           4
#define     ST_EVENT_MAX          32            //must be 8, 16 or 32

/* FIFO buffer */
#define     FIFO_EN               1
#define     FIFO_PAGE_SIZE        64

/* Single-Thread Kernel */

/* Inter-Thread communication */

/* Memory Management */

/* Kernel Device Object */

/* Single-Thread Components */

/* C++ features */

/* Command shell */

/* Device virtual file system */

/* Device Drivers */

/* Using WiFi */

/* Using USB */

/* POSIX layer and C standard library */

/* Network */

/* Socket abstraction layer */

/* protocol stack implement */

/* Network interface device */

/* light weight TCP/IP stack */

/* Modbus master and slave stack */

/* AT commands */

/* VBUS(Virtual Software BUS) */

/* Utilities */

/* Single-Thread online packages */

/* IoT - internet of things */

/* Wi-Fi */

/* Marvell WiFi */

/* Wiced WiFi */

/* IoT Cloud */

/* security packages */

/* language packages */

/* multimedia packages */

/* tools packages */

/* system packages */

/* peripheral libraries and drivers */

/* miscellaneous packages */

/* samples: kernel and components samples */

#endif
