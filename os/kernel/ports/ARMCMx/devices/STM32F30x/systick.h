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
 * @file    STM32F3xx/systick.h
 * @brief   System timer header file.
 *
 * @addtogroup STM32F3_TIMER
 * @{
 */

#ifndef _SYSTICK_H_
#define _SYSTICK_H_

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
 * @brief   Returns the system time.
 *
 * @return              The system time.
 *
 * @notapi
 */
static inline systime_t port_timer_get_time(void) {

  return TIM2->CNT;
}

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

  chDbgAssert((TIM2->DIER & 2) == 0,
              "port_timer_start_alarm(), #1",
              "already started");

  TIM2->CCR1  = time;
  TIM2->SR      = 0;
  TIM2->DIER    = 2;            /* CC1IE */
}

/**
 * @brief   Stops the alarm interrupt.
 *
 * @notapi
 */
static inline void port_timer_stop_alarm(void) {

  chDbgAssert((TIM2->DIER & 2) != 0,
              "port_timer_stop_alarm(), #1",
              "not started");

  TIM2->DIER    = 0;
}

/**
 * @brief   Sets the alarm time.
 *
 * @param[in] time      the time to be set for the next alarm
 *
 * @notapi
 */
static inline void port_timer_set_alarm(systime_t time) {

  chDbgAssert((TIM2->DIER & 2) != 0,
              "port_timer_set_alarm(), #1",
              "not started");

  TIM2->CCR1  = time;
}

/**
 * @brief   Returns the current alarm time.
 *
 * @return              The currently set alarm time.
 *
 * @notapi
 */
static inline systime_t port_timer_get_alarm(void) {

  chDbgAssert((TIM2->DIER & 2) != 0,
              "port_timer_get_alarm(), #1",
              "not started");

  return TIM2->CCR1;
}

#endif /* _SYSTICK_H_ */

/** @} */
