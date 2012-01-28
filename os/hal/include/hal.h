/*
    ChibiOS/RT - Copyright (C) 2006,2007,2008,2009,2010,
                 2011,2012 Giovanni Di Sirio.

    This file is part of ChibiOS/RT.

    ChibiOS/RT is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 3 of the License, or
    (at your option) any later version.

    ChibiOS/RT is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

                                      ---

    A special exception to the GPL can be applied should you wish to distribute
    a combined work that includes ChibiOS/RT, without being obliged to provide
    the source code for any proprietary components. See the file exception.txt
    for full details of how and when the exception can be applied.
*/

/**
 * @file    hal.h
 * @brief   HAL subsystem header.
 *
 * @addtogroup HAL
 * @{
 */

#ifndef _HAL_H_
#define _HAL_H_

#include "board.h"
#include "halconf.h"

#include "hal_lld.h"

#include "tm.h"
#include "pal.h"
#include "adc.h"
#include "can.h"
#include "ext.h"
#include "gpt.h"
#include "i2c.h"
#include "icu.h"
#include "mac.h"
#include "pwm.h"
#include "rtc.h"
#include "serial.h"
#include "sdc.h"
#include "spi.h"
#include "uart.h"
#include "usb.h"
#include "mmc_spi.h"
#include "serial_usb.h"

/*===========================================================================*/
/* Driver constants.                                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Driver pre-compile time settings.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

/*===========================================================================*/
/* Driver data structures and types.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Driver macros.                                                            */
/*===========================================================================*/

/**
 * @name    Time conversion utilities for the realtime counter
 * @{
 */
/**
 * @brief   Seconds to realtime ticks.
 * @details Converts from seconds to realtime ticks number.
 * @note    The result is rounded upward to the next tick boundary.
 *
 * @param[in] sec       number of seconds
 * @return              The number of ticks.
 *
 * @api
 */
#define S2RTT(sec) (halGetCounterFrequency() * (sec))

/**
 * @brief   Milliseconds to realtime ticks.
 * @details Converts from milliseconds to realtime ticks number.
 * @note    The result is rounded upward to the next tick boundary.
 *
 * @param[in] msec      number of milliseconds
 * @return              The number of ticks.
 *
 * @api
 */
#define MS2RTT(msec) (((halGetCounterFrequency() + 999UL) / 1000UL) * (msec))

/**
 * @brief   Microseconds to realtime ticks.
 * @details Converts from microseconds to realtime ticks number.
 * @note    The result is rounded upward to the next tick boundary.
 *
 * @param[in] usec      number of microseconds
 * @return              The number of ticks.
 *
 * @api
 */
#define US2RTT(usec) (((halGetCounterFrequency() + 999999UL) / 1000000UL) * \
                      (usec))
/** @} */

/**
 * @name    Macro Functions
 * @{
 */
/**
 * @brief   Returns the current value of the system free running counter.
 * @note    This is an optional service that could not be implemented in
 *          all HAL implementations.
 *
 * @return              The value of the system free running counter of
 *                      type halrtcnt_t.
 *
 * @api
 */
#define halGetCounterValue() hal_lld_get_counter_value()

/**
 * @brief   Realtime counter frequency.
 * @note    This is an optional service that could not be implemented in
 *          all HAL implementations.
 *
 * @return              The realtime counter frequency of type halclock_t.
 *
 * @api
 */
#define halGetCounterFrequency() hal_lld_get_counter_frequency()

/**
 * @brief   Realtime window test.
 * @details This function verifies if the current realtime counter value
 *          lies within the specified range or not. The test takes care
 *          of the realtime counter wrapping to zero on overflow.
 * @note    When start==end then the function returns always true because the
 *          whole time range is specified.
 *
 * @par Example 1
 * Example of a guarded loop using the realtime counter. The loop implements
 * a timeout after one second.
 * @code
 *   halrtcnt_t start = halGetCounterValue();
 *   halrtcnt_t timeout  = start + S2RTT(1);
 *   while (my_condition) {
 *     if (!halIsCounterWithin(start, timeout)
 *       return TIMEOUT;
 *     // Do something.
 *   }
 *   // Continue.
 * @endcode
 *
 * @par Example 2
 * Example of a loop that lasts exactly 50 microseconds.
 * @code
 *   halrtcnt_t start = halGetCounterValue();
 *   halrtcnt_t timeout  = start + US2RTT(50);
 *   while (halIsCounterWithin(start, timeout)) {
 *     // Do something.
 *   }
 *   // Continue.
 * @endcode
 *
 * @param[in] start     the start of the time window (inclusive)
 * @param[in] end       the end of the time window (non inclusive)
 * @retval TRUE         current time within the specified time window.
 * @retval FALSE        current time not within the specified time window.
 *
 * @api
 */
#define halIsCounterWithin(start, end)                                      \
  (end > start ? (halGetCounterValue() >= start) &&                         \
                 (halGetCounterValue() < end) :                             \
                 (halGetCounterValue() >= start) ||                         \
                 (halGetCounterValue() < end))
/** @} */

/**
 * @brief   Polled delay.
 * @note    The real delays is always few cycles in excess of the specified
 *          value.
 *
 * @param[in] ticks     number of ticks
 *
 * @api
 */
#define halPolledDelay(ticks) {                                             \
  halrtcnt_t start = halGetCounterValue();                                  \
  halrtcnt_t timeout  = start + (ticks);                                    \
  while (halIsCounterWithin(start, timeout))                                \
    ;                                                                       \
}

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#ifdef __cplusplus
extern "C" {
#endif
  void halInit(void);
#ifdef __cplusplus
}
#endif

#endif /* _HAL_H_ */

/** @} */
