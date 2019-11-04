

#ifndef __HAL_DRIVERS_H__
#define __HAL_DRIVERS_H__

#include "st.h"

#ifdef ST_USING_HAL_PIN
#include "hal_pin.h"
#endif //ST_USE_HAL_PIN

#ifdef ST_USING_HAL_UART
#include "hal_uart.h"
#endif //ST_USING_HAL_UART

#endif // __HAL_DRIVERS_H__

