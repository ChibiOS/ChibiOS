/*
    ChibiOS - Copyright (C) 2006-2026 Giovanni Di Sirio.

    This file is part of ChibiOS.

    ChibiOS is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation version 3 of the License.

    ChibiOS is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

/**
 * @file    rt/src/chtm.c
 * @brief   Time Measurement module code.
 *
 * @addtogroup time_measurement
 * @details Time Measurement APIs and services.
 * @{
 */

#include <string.h>

#include "ch.h"

#if (CH_CFG_USE_TM == TRUE) || defined(__DOXYGEN__)

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

/*===========================================================================*/
/* Module local functions.                                                   */
/*===========================================================================*/

static inline void tm_stop(time_measurement_t *tmp,
                           rtcnt_t now,
                           rtcnt_t offset) {

  tmp->n++;
  tmp->last = (now - tmp->last) - offset;
  tmp->cumulative += (rttime_t)tmp->last;
  if (tmp->last > tmp->worst) {
    tmp->worst = tmp->last;
  }
  if (tmp->last < tmp->best) {
    tmp->best = tmp->last;
  }
}

/*===========================================================================*/
/* Module exported functions.                                                */
/*===========================================================================*/

/**
 * @brief   Initializes a @p time_measurement_t object.
 *
 * @param[out] tmp      pointer to a @p time_measurement_t object
 *
 * @init
 */
void chTMObjectInit(time_measurement_t *tmp) {

  tmp->best       = (rtcnt_t)-1;
  tmp->worst      = (rtcnt_t)0;
  tmp->last       = (rtcnt_t)0;
  tmp->n          = (ucnt_t)0;
  tmp->cumulative = (rttime_t)0;
}

/**
 * @brief   Disposes a @p time_measurement_t object.
 * @note    Objects disposing does not involve freeing memory but just
 *          performing checks that make sure that the object is in a
 *          state compatible with operations stop.
 * @note    If the option @p CH_CFG_HARDENING_LEVEL is greater than zero then
 *          the object is also cleared, attempts to use the object would likely
 *          result in a clean memory access violation because dereferencing
 *          of @p NULL pointers rather than dereferencing previously valid
 *          pointers.
 *
 * @param[in] tmp       pointer to a @p time_measurement_t object
 *
 * @dispose
 */
void chTMObjectDispose(time_measurement_t *tmp) {

  chDbgCheck(tmp != NULL);

#if CH_CFG_HARDENING_LEVEL > 0
  memset((void *)tmp, 0, sizeof (time_measurement_t));
#endif
}

/**
 * @brief   Starts a measurement.
 * @pre     The @p time_measurement_t object must be initialized.
 *
 * @param[in,out] tmp   pointer to a @p time_measurement_t object
 *
 * @xclass
 */
NOINLINE void chTMStartMeasurementX(time_measurement_t *tmp) {

  tmp->last = chSysGetRealtimeCounterX();
}

/**
 * @brief   Stops a measurement.
 * @pre     The @p time_measurement_t object must be initialized.
 *
 * @param[in,out] tmp   pointer to a @p time_measurement_t object
 *
 * @xclass
 */
NOINLINE void chTMStopMeasurementX(time_measurement_t *tmp) {

  tm_stop(tmp, chSysGetRealtimeCounterX(), ch_system.tmc.offset);
}

/**
 * @brief   Stops a measurement and chains to the next one using the same time
 *          stamp.
 *
 * @param[in,out] tmp1  pointer to the @p time_measurement_t object to be
 *                      stopped
 * @param[in,out] tmp2  pointer to the @p time_measurement_t object to be
 *                      started
 *
 *
 * @xclass
 */
NOINLINE void chTMChainMeasurementToX(time_measurement_t *tmp1,
                                      time_measurement_t *tmp2) {

  /* Get current time for measurement.*/
  rtcnt_t now = chSysGetRealtimeCounterX();

  /* Stop previous measurement using current time. Update chained only after
     stop since the same measurement object may be used.*/
  tm_stop(tmp1, now, (rtcnt_t)0);
  tmp2->last = now;
}

#endif /* CH_CFG_USE_TM == TRUE */

/** @} */
