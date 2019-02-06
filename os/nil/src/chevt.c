/*
    ChibiOS - Copyright (C) 2006..2018 Giovanni Di Sirio.

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
 * @file    nil/src/chevt.c
 * @brief   Nil RTOS events source file.
 *
 * @addtogroup NIL_EVENTS
 * @{
 */

#include "ch.h"

#if (CH_CFG_USE_EVENTS == TRUE) || defined(__DOXYGEN__)

/*===========================================================================*/
/* Module local definitions.                                                 */
/*===========================================================================*/

/*===========================================================================*/
/* Module exported variables.                                                */
/*===========================================================================*/

/*===========================================================================*/
/* Module local variables.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Module local functions.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Module interrupt handlers.                                                */
/*===========================================================================*/

/*===========================================================================*/
/* Module exported functions.                                                */
/*===========================================================================*/

/**
 * @brief   Adds a set of event flags directly to the specified @p thread_t.
 *
 * @param[in] tp        the thread to be signaled
 * @param[in] mask      the event flags set to be ORed
 *
 * @api
 */
void chEvtSignal(thread_t *tp, eventmask_t mask) {

  chSysLock();
  chEvtSignalI(tp, mask);
  chSchRescheduleS();
  chSysUnlock();
}

/**
 * @brief   Adds a set of event flags directly to the specified @p thread_t.
 * @post    This function does not reschedule so a call to a rescheduling
 *          function must be performed before unlocking the kernel. Note that
 *          interrupt handlers always reschedule on exit so an explicit
 *          reschedule must not be performed in ISRs.
 *
 * @param[in] tp        the thread to be signaled
 * @param[in] mask      the event flags set to be ORed
 *
 * @iclass
 */
void chEvtSignalI(thread_t *tp, eventmask_t mask) {

  chDbgCheckClassI();
  chDbgCheck(tp != NULL);

  tp->epmask |= mask;
  if ((NIL_THD_IS_WTOREVT(tp) &&
       ((tp->epmask & tp->u1.ewmask) != (eventmask_t)0)) ||
      (NIL_THD_IS_WTANDEVT(tp) &&
       ((tp->epmask & tp->u1.ewmask) == tp->u1.ewmask))) {
    (void) chSchReadyI(tp, MSG_OK);
  }
}

/**
 * @brief   Waits for any of the specified events.
 * @details The function waits for any event among those specified in
 *          @p mask to become pending then the events are cleared and
 *          returned.
 *
 * @param[in] mask      mask of the event flags that the function should wait
 *                      for, @p ALL_EVENTS enables all the events
 * @param[in] timeout   the number of ticks before the operation timeouts,
 *                      the following special values are allowed:
 *                      - @a TIME_IMMEDIATE immediate timeout.
 *                      - @a TIME_INFINITE no timeout.
 *                      .
 * @return              The mask of the served and cleared events.
 * @retval 0            if the operation has timed out.
 *
 * @api
 */
eventmask_t chEvtWaitAnyTimeout(eventmask_t mask, sysinterval_t timeout) {
  thread_t *ctp = nil.current;
  eventmask_t m;

  chSysLock();
  if ((m = (ctp->epmask & mask)) == (eventmask_t)0) {
    if (TIME_IMMEDIATE == timeout) {
      chSysUnlock();

      return (eventmask_t)0;
    }
    ctp->u1.ewmask = mask;
    if (chSchGoSleepTimeoutS(NIL_STATE_WTOREVT, timeout) < MSG_OK) {
      chSysUnlock();

      return (eventmask_t)0;
    }
    m = ctp->epmask & mask;
  }
  ctp->epmask &= ~m;
  chSysUnlock();

  return m;
}

/**
 * @brief   Waits for all the specified events.
 * @details The function waits for all the events specified in @p mask to
 *          become pending then the events are cleared and returned.
 *
 * @param[in] mask      mask of the event flags that the function should wait
 *                      for, @p ALL_EVENTS enables all the events
 * @param[in] timeout   the number of ticks before the operation timeouts,
 *                      the following special values are allowed:
 *                      - @a TIME_IMMEDIATE immediate timeout.
 *                      - @a TIME_INFINITE no timeout.
 *                      .
 * @return              The mask of the served and cleared events.
 * @retval 0            if the operation has timed out.
 *
 * @api
 */
eventmask_t chEvtWaitAllTimeout(eventmask_t mask, sysinterval_t timeout) {
  thread_t *ctp = nil.current;

  chSysLock();
  if ((ctp->epmask & mask) != mask) {
    if (TIME_IMMEDIATE == timeout) {
      chSysUnlock();

      return (eventmask_t)0;
    }
    ctp->u1.ewmask = mask;
    if (chSchGoSleepTimeoutS(NIL_STATE_WTANDEVT, timeout) < MSG_OK) {
      chSysUnlock();

      return (eventmask_t)0;
    }
  }
  ctp->epmask &= ~mask;
  chSysUnlock();

  return mask;
}

#endif /* CH_CFG_USE_EVENTS == TRUE */

/** @} */
