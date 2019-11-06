#ifndef __ST_CONFIG_H__
#define __ST_CONFIG_H__
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
 
/*******************************************************************************
 * Single-Thread Kernel
 ******************************************************************************/
#define ST_ASSERT_EN
#define ST_ARGCHK_EN                        // Enable arguments check
#define ST_MSG_EN
#define ST_CLOCK_EN
#define ST_TIMER_EN
#define ST_TIMER_USE_HEAP
#define ST_TIMER_MAX          8             // meaningless if ST_TIMER_STATIC_EN == 0
#define ST_MEM_EN

#define ST_TASK_EVENT_MAX     32            // should be 8, 16 or 32
/*******************************************************************************
 * Single-Thread HAL Drivers
 ******************************************************************************/
#define ST_USING_HAL_PIN
#define ST_USING_HAL_UART

/*******************************************************************************
 * Single-Thread Components - Task IDs
 ******************************************************************************/
//#define TASK_ID_DRIVERS         0
//#define TASK_ID_LED             1
//#define TASK_ID_DEMO            2
//#define TASK_ID_CLI             3

/*******************************************************************************
 * Single-Thread Components - LED
 ******************************************************************************/
#define ST_USING_LED
#ifdef  ST_USING_LED
#define LED_0_PIN               HAL_PIN_GET( HAL_GPIO_PORT_F, 6 )
#define LED_1_PIN               HAL_PIN_GET( HAL_GPIO_PORT_F, 9 )
#define LED_2_PIN               HAL_PIN_GET( HAL_GPIO_PORT_F, 10 )
//#define LED_3_PIN             
//#define LED_4_PIN             
//#define LED_5_PIN             
//#define LED_6_PIN             
//#define LED_7_PIN             
#define LED_ON_POLARITY         HAL_PIN_HIGH
#define LED_BLINK_EN            1
#endif

/*******************************************************************************
 * Single-Thread Components - FIFO buffer
 ******************************************************************************/
#define ST_FIFO_EN
#ifdef  ST_FIFO_EN
#define FIFO_PAGE_SIZE          64
#endif

/*******************************************************************************
 * Single-Thread Components - CLI
 ******************************************************************************/
#define ST_USING_CLI
#ifdef  ST_USING_CLI
#define CLI_UART_PORT           HAL_UART_PORT_1
#define CLI_MAX_CMD_LENGTH      128
#endif


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
/****** (C) COPYRIGHT 2019 Single-Thread Development Team. *****END OF FILE****/
