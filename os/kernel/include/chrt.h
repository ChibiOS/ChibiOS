/*
    ChibiOS/RT - Copyright (C) 2006,2007,2008,2009,2010,
                 2011,2012,2013 Giovanni Di Sirio.

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
*/

/**
 * @file    chrt.h
 * @brief   Real Time Counter and Measurement module macros and structures.
 *
 * @addtogroup rt_measurement
 * @{
 */

#ifndef _CHRT_H_
#define _CHRT_H_

#if CH_CFG_USE_RT || defined(__DOXYGEN__)

#define port_rt_get_counter_value() 0

/*===========================================================================*/
/* Module constants.                                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Module pre-compile time settings.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

#if !CH_PORT_SUPPORTS_RT
#error "the port layer does not support the realtime counter functionality"
#endif

/*===========================================================================*/
/* Module data structures and types.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Module macros.                                                            */
/*===========================================================================*/
/**
 * @name    Time conversion utilities for the realtime counter
 * @{
 */
/**
 * @brief   Seconds to realtime counter.
 * @details Converts from seconds to realtime counter cycles.
 * @note    The result is rounded upward to the next tick boundary.
 *
 * @param[in] sec       number of seconds
 * @return              The number of cycles.
 *
 * @api
 */
#define S2RTV(freq, sec) ((freq) * (sec))

/**
 * @brief   Milliseconds to realtime counter.
 * @details Converts from milliseconds to realtime counter cycles.
 * @note    The result is rounded upward to the next tick boundary.
 *
 * @param[in] msec      number of milliseconds
 * @return              The number of cycles.
 *
 * @api
 */
#define MS2RTC(freq, msec) (rtcnt_t)((((freq) + 999UL) / 1000UL) * (msec))

/**
 * @brief   Microseconds to realtime counter.
 * @details Converts from microseconds to realtime counter cycles.
 * @note    The result is rounded upward to the next tick boundary.
 *
 * @param[in] usec      number of microseconds
 * @return              The number of cycles.
 *
 * @api
 */
#define US2RTC(freq, usec) (rtcnt_t)((((freq) + 999999UL) / 1000000UL) * (usec))

/**
 * @brief   Realtime counter cycles to seconds.
 * @details Converts from realtime counter cycles number to seconds.
 *
 * @param[in] n         number of cycles
 * @return              The number of seconds.
 *
 * @api
 */
#define RTC2S(freq, n) (rtcnt_t)((n) / (freq))

/**
 * @brief   Realtime counter cycles to milliseconds.
 * @details Converts from realtime counter cycles number to milliseconds.
 *
 * @param[in] n         number of cycles
 * @return              The number of milliseconds.
 *
 * @api
 */
#define RTC2MS(freq, n) ((n) / ((freq) / 1000UL))

/**
 * @brief   Realtime counter cycles to microseconds.
 * @details Converts from realtime counter cycles number to microseconds.
 *
 * @param[in] n         number of cycles
 * @return              The number of microseconds.
 *
 * @api
 */
#define RTC2US(freq, n) ((n) / ((freq) / 1000000UL))
/** @} */

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#ifdef __cplusplus
extern "C" {
#endif
#ifdef __cplusplus
}
#endif

/*===========================================================================*/
/* Module inline functions.                                                  */
/*===========================================================================*/

/**
 * @brief   Returns the current value of the system real time counter.
 * @note    This function can be called from any context.
 *
 * @return              The value of the system free running counter of
 *                      type rtcnt_t.
 *
 * @special
 */
static inline rtcnt_t chRTGetCounterValue(void) {

  return port_rt_get_counter_value();
}

/**
 * @brief   Realtime window test.
 * @details This function verifies if the current realtime counter value
 *          lies within the specified range or not. The test takes care
 *          of the realtime counter wrapping to zero on overflow.
 * @note    When start==end then the function returns always true because the
 *          whole time range is specified.
 * @note    This function can be called from any context.
 *
 * @par Example 1
 * Example of a guarded loop using the realtime counter. The loop implements
 * a timeout after one second.
 * @code
 *   rtcnt_t start = chRTGetCounterValue();
 *   rtcnt_t timeout  = start + S2RTC(1);
 *   while (my_condition) {
 *     if (!chRTIsCounterWithin(start, timeout)
 *       return TIMEOUT;
 *     // Do something.
 *   }
 *   // Continue.
 * @endcode
 *
 * @par Example 2
 * Example of a loop that lasts exactly 50 microseconds.
 * @code
 *   rtcnt_t start = chRTGetCounterValue();
 *   rtcnt_t timeout  = start + US2RTC(50);
 *   while (chRTIsCounterWithin(start, timeout)) {
 *     // Do something.
 *   }
 *   // Continue.
 * @endcode
 *
 * @param[in] start     the start of the time window (inclusive)
 * @param[in] end       the end of the time window (non inclusive)
 * @retval true         current time within the specified time window.
 * @retval false        current time not within the specified time window.
 *
 * @special
 */
static inline bool chRTIsCounterWithin(rtcnt_t start, rtcnt_t end) {
  rtcnt_t now = chRTGetCounterValue();

  return end > start ? (now >= start) && (now < end) :
                       (now >= start) || (now < end);
}

/**
 * @brief   Polled delay.
 * @note    The real delay is always few cycles in excess of the specified
 *          value.
 * @note    This function can be called from any context.
 *
 * @param[in] ticks     number of ticks
 *
 * @special
 */
static inline void chRTPolledDelay(rtcnt_t ticks) {
  rtcnt_t start = chRTGetCounterValue();
  rtcnt_t timeout  = start + (ticks);
  while (chRTIsCounterWithin(start, timeout))
    ;
}

#endif /* CH_CFG_USE_RT */

#endif /* _CHRT_H_ */

/** @} */
