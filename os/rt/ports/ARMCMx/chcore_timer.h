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
 * @file    ARMCMx/chcore_timer.h
 * @brief   System timer header file.
 *
 * @addtogroup ARMCMx_TIMER
 * @{
 */

#ifndef _CHCORE_TIMER_H_
#define _CHCORE_TIMER_H_

#if defined(PORT_DO_NOT_USE_ST)
/* If, for some reason, the use of the HAL-provided ST timer port interface
   is not wanted, it is possible to provide the timer interface into a custom
   module.*/
#include "chcore_timer_ext.h"

#else /* !defined(PORT_DO_NOT_USE_ST) */

/* This is the only header in the HAL designed to be include-able alone.*/
#include "st.h"

/*===========================================================================*/
/* Module constants.                                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Module pre-compile time settings.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

/*===========================================================================*/
/* Module data structures and types.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Module macros.                                                            */
/*===========================================================================*/

/**
 * @brief   Starts the alarm.
 * @note    Makes sure that no spurious alarms are triggered after
 *          this call.
 *
 * @param[in] time      the time to be set for the first alarm
 *
 * @notapi
 */
#define port_timer_start_alarm(time) {                                      \
  chDbgAssert(stIsAlarmActive() == false, "already active");                \
  stStartAlarm(time);                                                       \
}

/**
 * @brief   Stops the alarm interrupt.
 *
 * @notapi
 */
#define port_timer_stop_alarm() {                                           \
  chDbgAssert(stIsAlarmActive() != false, "not active");                    \
  stStopAlarm();                                                            \
}

/**
 * @brief   Sets the alarm time.
 *
 * @param[in] time      the time to be set for the next alarm
 *
 * @notapi
 */
#define port_timer_set_alarm(time) {                                        \
  chDbgAssert(stIsAlarmActive() != false, "not active");                    \
  stSetAlarm(time);                                                         \
}

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

/*===========================================================================*/
/* Module inline functions.                                                  */
/*===========================================================================*/

/**
 * @brief   Returns the system time.
 *
 * @return              The system time.
 *
 * @notapi
 */
static inline systime_t port_timer_get_time(void) {

  return stGetCounter();
}

/**
 * @brief   Returns the current alarm time.
 *
 * @return              The currently set alarm time.
 *
 * @notapi
 */
static inline systime_t port_timer_get_alarm(void) {

  return stGetAlarm();
}

#endif /* !defined(PORT_DO_NOT_USE_ST) */

#endif /* _CHCORE_TIMER_H_ */

/** @} */
