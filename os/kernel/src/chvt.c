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
 * @file    chvt.c
 * @brief   Time and Virtual Timers module code.
 *
 * @addtogroup time
 * @details Time and Virtual Timers related APIs and services.
 * @{
 */

#include "ch.h"

/*===========================================================================*/
/* Module local definitions.                                                 */
/*===========================================================================*/

/*===========================================================================*/
/* Module exported variables.                                                */
/*===========================================================================*/

/**
 * @brief   Virtual timers delta list header.
 */
virtual_timers_list_t vtlist;

/*===========================================================================*/
/* Module local types.                                                       */
/*===========================================================================*/

/*===========================================================================*/
/* Module local variables.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Module local functions.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Module exported functions.                                                */
/*===========================================================================*/

/**
 * @brief   Virtual Timers initialization.
 * @note    Internal use only.
 *
 * @notapi
 */
void _vt_init(void) {

  vtlist.vt_next = vtlist.vt_prev = (void *)&vtlist;
  vtlist.vt_delta = (systime_t)-1;
#if CH_CFG_TIMEDELTA == 0
  vtlist.vt_systime = 0;
#else /* CH_CFG_TIMEDELTA > 0 */
  vtlist.vt_lasttime = 0;
#endif /* CH_CFG_TIMEDELTA > 0 */
}

/**
 * @brief   Checks if the current system time is within the specified time
 *          window.
 * @note    When start==end then the function returns always true because the
 *          whole time range is specified.
 * @note    This function can be called from any context.
 *
 * @param[in] time      the time to be verified
 * @param[in] start     the start of the time window (inclusive)
 * @param[in] end       the end of the time window (non inclusive)
 * @retval true         current time within the specified time window.
 * @retval false        current time not within the specified time window.
 *
 * @special
 */
bool chVTIsTimeWithin(systime_t time, systime_t start, systime_t end) {

  return end > start ? (time >= start) && (time < end) :
                       (time >= start) || (time < end);
}

/**
 * @brief   Enables a virtual timer.
 * @details The timer is enabled and programmed to trigger after the delay
 *          specified as parameter.
 * @pre     The timer must not be already armed before calling this function.
 * @note    The callback function is invoked from interrupt context.
 *
 * @param[out] vtp      the @p virtual_timer_t structure pointer
 * @param[in] delay     the number of ticks before the operation timeouts, the
 *                      special values are handled as follow:
 *                      - @a TIME_INFINITE is allowed but interpreted as a
 *                        normal time specification.
 *                      - @a TIME_IMMEDIATE this value is not allowed.
 *                      .
 * @param[in] vtfunc    the timer callback function. After invoking the
 *                      callback the timer is disabled and the structure can
 *                      be disposed or reused.
 * @param[in] par       a parameter that will be passed to the callback
 *                      function
 *
 * @iclass
 */
void chVTDoSetI(virtual_timer_t *vtp, systime_t delay,
                vtfunc_t vtfunc, void *par) {
  virtual_timer_t *p;

  chDbgCheckClassI();
  chDbgCheck((vtp != NULL) && (vtfunc != NULL) && (delay != TIME_IMMEDIATE),
             "chVTDoSetI");

  vtp->vt_par = par;
  vtp->vt_func = vtfunc;
  p = vtlist.vt_next;

#if CH_CFG_TIMEDELTA > 0 || defined(__DOXYGEN__)
  {
    systime_t now = port_timer_get_time();

    /* If the requested delay is lower than the minimum safe delta then it
       is raised to the minimum safe value.*/
    if (delay < CH_CFG_TIMEDELTA)
      delay = CH_CFG_TIMEDELTA;

    if (&vtlist == (virtual_timers_list_t *)p) {
      /* The delta list is empty, the current time becomes the new
         delta list base time.*/
      vtlist.vt_lasttime = now;
      port_timer_start_alarm(vtlist.vt_lasttime + delay);
    }
    else {
      /* Now the delay is calculated as delta from the last tick interrupt
         time.*/
      delay += now - vtlist.vt_lasttime;

      /* If the specified delay is closer in time than the first element
         in the delta list then it becomes the next alarm event in time.*/
      if (delay < p->vt_delta)
        port_timer_set_alarm(vtlist.vt_lasttime + delay);
    }
  }
#endif /* CH_CFG_TIMEDELTA > 0 */

  /* The delta list is scanned in order to find the correct position for
     this timer. */
  while (p->vt_delta < delay) {
    delay -= p->vt_delta;
    p = p->vt_next;
  }
  /* The timer is inserted in the delta list.*/
  vtp->vt_prev = (vtp->vt_next = p)->vt_prev;
  vtp->vt_prev->vt_next = p->vt_prev = vtp;
  vtp->vt_delta = delay

  /* Special case when the timer is in last position in the list, the
     value in the header must be restored.*/;
  p->vt_delta -= delay;
  vtlist.vt_delta = (systime_t)-1;
}

/**
 * @brief   Disables a Virtual Timer.
 * @pre     The timer must be in armed state before calling this function.
 *
 * @param[in] vtp       the @p virtual_timer_t structure pointer
 *
 * @iclass
 */
void chVTDoResetI(virtual_timer_t *vtp) {

  chDbgCheckClassI();
  chDbgCheck(vtp != NULL, "chVTDoResetI");
  chDbgAssert(vtp->vt_func != NULL,
              "chVTDoResetI(), #1",
              "timer not set or already triggered");

  /* Removing the element from the delta list.*/
  vtp->vt_next->vt_delta += vtp->vt_delta;
  vtp->vt_prev->vt_next = vtp->vt_next;
  vtp->vt_next->vt_prev = vtp->vt_prev;
  vtp->vt_func = (vtfunc_t)NULL;

  /* The above code changes the value in the header when the removed element
     is the last of the list, restoring it.*/
  vtlist.vt_delta = (systime_t)-1;

#if CH_CFG_TIMEDELTA > 0 || defined(__DOXYGEN__)
  {
    if (&vtlist == (virtual_timers_list_t *)vtlist.vt_next) {
      /* Just removed the last element in the list, alarm timer stopped.*/
      port_timer_stop_alarm();
    }
    else {
      /* The alarm is set to the next element in the delta list.*/
      port_timer_set_alarm(vtlist.vt_lasttime + vtlist.vt_next->vt_delta);
    }
  }
#endif /* CH_CFG_TIMEDELTA > 0 */
}

/** @} */
