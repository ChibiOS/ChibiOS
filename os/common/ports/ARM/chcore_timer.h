/*
    ChibiOS - Copyright (C) 2006,2007,2008,2009,2010,2011,2012,2013,2014,
              2015,2016,2017,2018,2019,2020,2021 Giovanni Di Sirio.

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
 * @file    chcore_timer.h
 * @brief   System timer header file.
 *
 * @addtogroup ARM_TIMER
 * @{
 */

#ifndef CHCORE_TIMER_H
#define CHCORE_TIMER_H

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

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

/*===========================================================================*/
/* Module inline functions.                                                  */
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
static inline void port_timer_start_alarm(systime_t time) {
  void stStartAlarm(systime_t time);

  stStartAlarm(time);
}

/**
 * @brief   Stops the alarm interrupt.
 *
 * @notapi
 */
static inline void port_timer_stop_alarm(void) {
  void stStopAlarm(void);

  stStopAlarm();
}

/**
 * @brief   Sets the alarm time.
 *
 * @param[in] time      the time to be set for the next alarm
 *
 * @notapi
 */
static inline void port_timer_set_alarm(systime_t time) {
  void stSetAlarm(systime_t time);

  stSetAlarm(time);
}

/**
 * @brief   Returns the system time.
 *
 * @return              The system time.
 *
 * @notapi
 */
static inline systime_t port_timer_get_time(void) {
  systime_t stGetCounter(void);

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
  systime_t stGetAlarm(void);

  return stGetAlarm();
}

#endif /* CHCORE_TIMER_H */

/** @} */
