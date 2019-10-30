

#ifndef __HAL_DRIVERS_H__
#define __HAL_DRIVERS_H__

#include "st.h"

#define TASK_EVT_DRIVERS_UART0_RXD           0
#define TASK_EVT_DRIVERS_UART1_RXD           1
#define TASK_EVT_DRIVERS_UART0_TXD           2
#define TASK_EVT_DRIVERS_UART1_TXD           3

#define CPU_SYSCLK                          (64000000uL)
#define CPU_HCLK                            (64000000uL)
#define CPU_AHBCLK                          (64000000uL)
#define CPU_APB1CLK                         (32000000uL)
#define CPU_APB2CLK                         (64000000uL)

void drivers_init( void );
void drivers_task( st_uint8_t event_id );

#ifdef ST_USING_HAL_PIN
#include "hal_pin.h"
#endif //ST_USE_HAL_PIN

#ifdef ST_USING_HAL_UART
#include "hal_uart.h"
#endif //ST_USING_HAL_UART

#endif // __HAL_DRIVERS_H__

