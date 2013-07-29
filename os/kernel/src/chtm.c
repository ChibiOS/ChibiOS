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
 * @file    chtm.c
 * @brief   Time Measurement module code.
 *
 * @addtogroup time_measurement
 * @details Time Measurement APIs and services.
 * @{
 */

#include "ch.h"

#if CH_CFG_USE_TM || defined(__DOXYGEN__)

/*===========================================================================*/
/* Module local definitions.                                                 */
/*===========================================================================*/

/*===========================================================================*/
/* Module exported variables.                                                */
/*===========================================================================*/

/*===========================================================================*/
/* Module local types.                                                       */
/*===========================================================================*/

/*===========================================================================*/
/* Module local variables.                                                   */
/*===========================================================================*/

/**
 * @brief   Subsystem calibration value.
 */
static rtcnt_t measurement_offset;

/*===========================================================================*/
/* Module local functions.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Module exported functions.                                                */
/*===========================================================================*/

/**
 * @brief   Initializes the realtime counter unit.
 *
 * @init
 */
void _rt_init(void) {
  time_measurement_t tm;

  /* Time Measurement subsystem calibration, it does a null measurement
     and calculates the call overhead which is subtracted to real
     measurements.*/
  measurement_offset = 0;
  chRTTimeMeasurementObjectInit(&tm);
  chRTTimeMeasurementStartX(&tm);
  chRTTimeMeasurementStopX(&tm);
  measurement_offset = tm.last;
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
 *   rtcnt_t start = chSysGetRealtimeCounterX();
 *   rtcnt_t timeout  = start + S2RTC(RTCCLK, 1);
 *   while (my_condition) {
 *     if (!chTMIsCounterWithin(start, timeout)
 *       return TIMEOUT;
 *     // Do something.
 *   }
 *   // Continue.
 * @endcode
 *
 * @par Example 2
 * Example of a loop that lasts exactly 50 microseconds.
 * @code
 *   rtcnt_t start = chSysGetRealtimeCounterX();
 *   rtcnt_t timeout  = start + US2RTC(RTCCLK, 50);
 *   while (chTMIsCounterWithin(start, timeout)) {
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
bool chTMIsCounterWithin(rtcnt_t start, rtcnt_t end) {
  rtcnt_t now = chSysGetRealtimeCounterX();

  return end > start ? (now >= start) && (now < end) :
                       (now >= start) || (now < end);
}

/**
 * @brief   Polled delay.
 * @note    The real delay is always few cycles in excess of the specified
 *          value.
 * @note    This function can be called from any context.
 *
 * @param[in] cycles    number of cycles
 *
 * @special
 */
void chTMPolledDelay(rtcnt_t cycles) {
  rtcnt_t start = chSysGetRealtimeCounterX();
  rtcnt_t end  = start + cycles;
  while (chRTIsCounterWithin(start, end))
    ;
}

/**
 * @brief   Initializes a @p TimeMeasurement object.
 *
 * @param[out] tmp      pointer to a @p TimeMeasurement structure
 *
 * @init
 */
void chTMObjectInit(time_measurement_t *tmp) {

  tmp->best       = (rtcnt_t)-1;
  tmp->worst      = (rtcnt_t)0;
  tmp->cumulative = (rtcnt_t)0;
  tmp->last       = (rtcnt_t)0;
}

/**
 * @brief   Starts a measurement.
 * @pre     The @p time_measurement_t structure must be initialized.
 * @note    This function can be invoked from any context.
 *
 * @param[in,out] tmp   pointer to a @p TimeMeasurement structure
 *
 * @special
 */
NOINLINE void chTMStartX(time_measurement_t *tmp) {

  tmp->last = chSysGetRealtimeCounterX();
}

/**
 * @brief   Stops a measurement.
 * @pre     The @p time_measurement_t structure must be initialized.
 * @note    This function can be invoked from any context.
 *
 * @param[in,out] tmp   pointer to a @p time_measurement_t structure
 *
 * @special
 */
NOINLINE void chTMStopX(time_measurement_t *tmp) {

  rtcnt_t now = chSysGetRealtimeCounterX();
  tmp->last = now - tmp->last - measurement_offset;
  tmp->cumulative += tmp->last;
  if (tmp->last > tmp->worst)
    tmp->worst = tmp->last;
  else if (tmp->last < tmp->best)
    tmp->best = tmp->last;
}

#endif /* CH_CFG_USE_TM */

/** @} */
