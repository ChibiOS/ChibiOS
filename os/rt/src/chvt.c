/*
    ChibiOS - Copyright (C) 2006..2015 Giovanni Di Sirio.

    This file is part of ChibiOS.

    ChibiOS is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 3 of the License, or
    (at your option) any later version.

    ChibiOS is distributed in the hope that it will be useful,
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

  ch.vtlist.vt_next = ch.vtlist.vt_prev = (void *)&ch.vtlist;
  ch.vtlist.vt_delta = (systime_t)-1;
#if CH_CFG_ST_TIMEDELTA == 0
  ch.vtlist.vt_systime = 0;
#else /* CH_CFG_ST_TIMEDELTA > 0 */
  ch.vtlist.vt_lasttime = 0;
#endif /* CH_CFG_ST_TIMEDELTA > 0 */
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
  chDbgCheck((vtp != NULL) && (vtfunc != NULL) && (delay != TIME_IMMEDIATE));

  vtp->vt_par = par;
  vtp->vt_func = vtfunc;
  p = ch.vtlist.vt_next;

#if CH_CFG_ST_TIMEDELTA > 0 || defined(__DOXYGEN__)
  {
    systime_t now = port_timer_get_time();

    /* If the requested delay is lower than the minimum safe delta then it
       is raised to the minimum safe value.*/
    if (delay < CH_CFG_ST_TIMEDELTA) {
      delay = CH_CFG_ST_TIMEDELTA;
    }

    if (&ch.vtlist == (virtual_timers_list_t *)p) {
      /* The delta list is empty, the current time becomes the new
         delta list base time.*/
      ch.vtlist.vt_lasttime = now;
      port_timer_start_alarm(ch.vtlist.vt_lasttime + delay);
    }
    else {
      /* Now the delay is calculated as delta from the last tick interrupt
         time.*/
      delay += now - ch.vtlist.vt_lasttime;

      /* If the specified delay is closer in time than the first element
         in the delta list then it becomes the next alarm event in time.*/
      if (delay < p->vt_delta) {
        port_timer_set_alarm(ch.vtlist.vt_lasttime + delay);
      }
    }
  }
#endif /* CH_CFG_ST_TIMEDELTA > 0 */

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
  ch.vtlist.vt_delta = (systime_t)-1;
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
  chDbgCheck(vtp != NULL);
  chDbgAssert(vtp->vt_func != NULL, "timer not set or already triggered");

  /* Removing the element from the delta list.*/
  vtp->vt_next->vt_delta += vtp->vt_delta;
  vtp->vt_prev->vt_next = vtp->vt_next;
  vtp->vt_next->vt_prev = vtp->vt_prev;
  vtp->vt_func = (vtfunc_t)NULL;

  /* The above code changes the value in the header when the removed element
     is the last of the list, restoring it.*/
  ch.vtlist.vt_delta = (systime_t)-1;

#if CH_CFG_ST_TIMEDELTA > 0 || defined(__DOXYGEN__)
  {
    if (&ch.vtlist == (virtual_timers_list_t *)ch.vtlist.vt_next) {
      /* Just removed the last element in the list, alarm timer stopped.*/
      port_timer_stop_alarm();
    }
    else {
      /* Updating the alarm to the next deadline, deadline that must not be
         closer in time than the minimum time delta.*/
      if (ch.vtlist.vt_next->vt_delta >= CH_CFG_ST_TIMEDELTA) {
        port_timer_set_alarm(ch.vtlist.vt_lasttime +
                             ch.vtlist.vt_next->vt_delta);
      }
      else {
        port_timer_set_alarm(ch.vtlist.vt_lasttime +
                             CH_CFG_ST_TIMEDELTA);
      }
    }
  }
#endif /* CH_CFG_ST_TIMEDELTA > 0 */
}

/** @} */
