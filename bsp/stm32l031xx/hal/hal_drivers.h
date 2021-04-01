

#ifndef __HAL_DRIVERS_H__
#define __HAL_DRIVERS_H__

#include "os.h"

#ifdef OS_USING_HAL_PIN
#include "hal_pin.h"
#endif //OS_USE_HAL_PIN

#ifdef OS_USING_HAL_UART
#include "hal_uart.h"
#endif //OS_USING_HAL_UART

#endif // __HAL_DRIVERS_H__

