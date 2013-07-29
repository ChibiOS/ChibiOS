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
 * @brief   Measurement calibration value.
 */
static rtcnt_t measurement_offset;

/*===========================================================================*/
/* Module local functions.                                                   */
/*===========================================================================*/

static inline void tm_stop(time_measurement_t *tmp, rtcnt_t now) {

  tmp->last = now - tmp->last - measurement_offset;
  tmp->cumulative += tmp->last;
  if (tmp->last > tmp->worst)
    tmp->worst = tmp->last;
  else if (tmp->last < tmp->best)
    tmp->best = tmp->last;
}

/*===========================================================================*/
/* Module exported functions.                                                */
/*===========================================================================*/

/**
 * @brief   Initializes the time measurement unit.
 *
 * @init
 */
void _tm_init(void) {
  time_measurement_t tm;

  /* Time Measurement subsystem calibration, it does a null measurement
     and calculates the call overhead which is subtracted to real
     measurements.*/
  measurement_offset = 0;
  chTMObjectInit(&tm);
  chTMStartX(&tm);
  chTMStopX(&tm);
  measurement_offset = tm.last;
}

/**
 * @brief   Realtime window test.
 * @details This function verifies if the current realtime counter value
 *          lies within the specified range or not. The test takes care
 *          of the realtime counter wrapping to zero on overflow.
 * @note    When start==end then the function returns always true because the
 *          whole time range is specified.
 *
 * @param[in] cnt       the counter value to be tested
 * @param[in] start     the start of the time window (inclusive)
 * @param[in] end       the end of the time window (non inclusive)
 * @retval true         current time within the specified time window.
 * @retval false        current time not within the specified time window.
 *
 * @xclass
 */
bool chTMIsCounterWithinX(rtcnt_t cnt, rtcnt_t start, rtcnt_t end) {

  return end > start ? (cnt >= start) && (cnt < end) :
                       (cnt >= start) || (cnt < end);
}

/**
 * @brief   Polled delay.
 * @note    The real delay is always few cycles in excess of the specified
 *          value.
 *
 * @param[in] cycles    number of cycles
 *
 * @xclass
 */
void chTMPolledDelayX(rtcnt_t cycles) {
  rtcnt_t start = chSysGetRealtimeCounterX();
  rtcnt_t end  = start + cycles;
  while (chTMIsCounterWithinX(chSysGetRealtimeCounterX(), start, end))
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
 *
 * @param[in,out] tmp   pointer to a @p TimeMeasurement structure
 *
 * @xclass
 */
NOINLINE void chTMStartX(time_measurement_t *tmp) {

  tmp->last = chSysGetRealtimeCounterX();
}

/**
 * @brief   Stops a measurement.
 * @pre     The @p time_measurement_t structure must be initialized.
 *
 * @param[in,out] tmp   pointer to a @p time_measurement_t structure
 *
 * @xclass
 */
NOINLINE void chTMStopX(time_measurement_t *tmp) {

  tm_stop(tmp, chSysGetRealtimeCounterX());
}

#endif /* CH_CFG_USE_TM */

/**
 * @brief   Stops a measurement and chains to the next one using the same time
 *          stamp.
 *
 * @param[in,out] tmp1  pointer to the @p time_measurement_t structure to be
 *                      stopped
 * @param[in,out] tmp2  pointer to the @p time_measurement_t structure to be
 *                      started
 *
 *
 * @xclass
 */
NOINLINE void chTMChainToX(time_measurement_t *tmp1,
                           time_measurement_t *tmp2) {

  /* Starts new measurement.*/
  tmp2->last = chSysGetRealtimeCounterX();

  /* Stops previous measurement using the same time stamp.*/
  tm_stop(tmp1, tmp2->last);
}

/** @} */
