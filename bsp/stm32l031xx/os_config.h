#ifndef __OS_CONFIG_H__
#define __OS_CONFIG_H__
/*******************************************************************************
 * Copyright (c) 2021-2020, PEOS Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date         Author       Notes
 * 2021-10-28   Wentao SUN   first version
 * 
 ******************************************************************************/
 
/*******************************************************************************
 * PEOS Kernel
 ******************************************************************************/
#define OS_ASSERT_EN
#define OS_MSG_EN
#define OS_CLOCK_EN
#define OS_TIMER_EN
#define OS_TIMER_USE_HEAP
#define OS_TIMER_MAX          8             // meaningless if defined OS_TIMER_USE_HEAP 
#define OS_MEM_EN

#define OS_TASK_EVENT_MAX     32            // should be 8, 16 or 32
/*******************************************************************************
 * PEOS HAL Drivers
 ******************************************************************************/
#define OS_USING_HAL_PIN
#define OS_USING_HAL_UART

/*******************************************************************************
 * PEOS Components - Task IDs
 ******************************************************************************/
//#define TASK_ID_DRIVERS         0
//#define TASK_ID_LED             1
//#define TASK_ID_DEMO            2
//#define TASK_ID_CLI             3

/*******************************************************************************
 * PEOS Components - LED
 ******************************************************************************/
#define OS_USING_LED
#ifdef  OS_USING_LED
#define LED_0_PIN               HAL_PIN_GET( HAL_GPIO_PORT_B, 3 )
//#define LED_1_PIN
//#define LED_2_PIN
//#define LED_3_PIN             
//#define LED_4_PIN             
//#define LED_5_PIN             
//#define LED_6_PIN             
//#define LED_7_PIN             
#define LED_ON_POLARITY         HAL_PIN_HIGH
#define LED_BLINK_EN            1
#endif

/*******************************************************************************
 * PEOS Components - FIFO buffer
 ******************************************************************************/
#define OS_USING_FIFO
#ifdef  OS_USING_FIFO
#define FIFO_PAGE_SIZE          64
#endif

/*******************************************************************************
 * PEOS Components - CLI
 ******************************************************************************/
#define OS_USING_CLI
#ifdef  OS_USING_CLI
#define CLI_UART_PORT           HAL_UART_PORT_0
#define CLI_UART_BAUDRATE       HAL_UART_BAUD_RATE_115200
#define CLI_MAX_CMD_LENGTH      128
#define CLI_TX_BUF_SIZE         128
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

/* PEOS online packages */

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
/****** (C) COPYRIGHT 2021 PEOS Development Team. *****END OF FILE****/

