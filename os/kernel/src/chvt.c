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
VTList vtlist;

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
  vtlist.vt_time = 0;
}

/**
 * @brief   Checks if the current system time is within the specified time
 *          window.
 * @note    When start==end then the function returns always true because the
 *          whole time range is specified.
 *
 * @param[in] start     the start of the time window (inclusive)
 * @param[in] end       the end of the time window (non inclusive)
 * @retval TRUE         current time within the specified time window.
 * @retval FALSE        current time not within the specified time window.
 *
 * @api
 */
bool_t chVTIsSystemTimeWithin(systime_t start, systime_t end) {

  systime_t time = chVTGetSystemTime();
  return end > start ? (time >= start) && (time < end) :
                       (time >= start) || (time < end);
}

/**
 * @brief   Enables a virtual timer.
 * @details The timer is enabled and programmed to trigger at the absolute
 *          system time specified as parameter.
 * @note    The associated function is invoked from interrupt context.
 *
 * @param[out] vtp      the @p VirtualTimer structure pointer
 * @param[in] time      absolute system time
 * @param[in] vtfunc    the timer callback function. After invoking the
 *                      callback the timer is disabled and the structure can
 *                      be disposed or reused.
 * @param[in] par       a parameter that will be passed to the callback
 *                      function
 *
 * @iclass
 */
void chVTSetAbsoluteI(VirtualTimer *vtp, systime_t time,
                      vtfunc_t vtfunc, void *par) {
  VirtualTimer *p;
  systime_t systime = vtlist.vt_time;

  chDbgCheckClassI();
  chDbgCheck((vtp != NULL) && (vtfunc != NULL), "chVTSetI");

  vtp->vt_par = par;
  vtp->vt_func = vtfunc;
  vtp->vt_time = time;
  if (time <= systime) {
    p = vtlist.vt_prev;
    while ((p->vt_time <= systime) && (p->vt_time > time))
      p = p->vt_prev;
    vtp->vt_next = (vtp->vt_prev = p)->vt_next;
    vtp->vt_next->vt_prev = p->vt_next = vtp;
  }
  else {
    p = vtlist.vt_next;
    while ((p->vt_time > systime) && (p->vt_time < time))
      p = p->vt_next;
    vtp->vt_prev = (vtp->vt_next = p)->vt_prev;
    vtp->vt_prev->vt_next = p->vt_prev = vtp;
  }
}

/**
 * @brief   Disables a Virtual Timer.
 * @note    The timer is first checked and disabled only if armed.
 *
 * @param[in] vtp       the @p VirtualTimer structure pointer
 *
 * @iclass
 */
void chVTResetI(VirtualTimer *vtp) {

  chDbgCheckClassI();
  chDbgCheck(vtp != NULL, "chVTResetI");

  if (chVTIsArmedI(vtp)) {
    vtp->vt_prev->vt_next = vtp->vt_next;
    vtp->vt_next->vt_prev = vtp->vt_prev;
    vtp->vt_func = (vtfunc_t)NULL;
  }
}

/** @} */
