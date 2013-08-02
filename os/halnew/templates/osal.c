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
 * @file    osal.c
 * @brief   OSAL module code.
 *
 * @addtogroup OSAL
 * @{
 */

#include "osal.h"

/*===========================================================================*/
/* Module local definitions.                                                 */
/*===========================================================================*/

/*===========================================================================*/
/* Module exported variables.                                                */
/*===========================================================================*/

/**
 * @brief   Pointer to a halt error message.
 * @note    The message is meant to be retrieved by the debugger after the
 *          system halt caused by an unexpected error.
 */
const char *osal_halt_msg;

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
 * @brief   OSAL module initialization.
 *
 * @api
 */
void osalInit(void) {

}

/**
 * @brief   System halt with error message.
 *
 * @param[in] reason    the halt message pointer
 *
 * @api
 */
#if !defined(__DOXYGEN__)
__attribute__((weak, noreturn))
#endif
void osalSysHalt(const char *reason) {

  osalIsrDisable();
  osal_halt_msg = reason;
  while (1)
    ;
}

/**
 * @brief   Enqueues the caller thread.
 * @details The caller thread is enqueued and put to sleep until it is
 *          dequeued or the specified timeouts expires.
 *
 * @param[in] tqp       pointer to the threads queue object
 * @param[in] time      the timeout in system ticks, the special values are
 *                      handled as follow:
 *                      - @a TIME_INFINITE the thread enters an infinite sleep
 *                        state.
 *                      - @a TIME_IMMEDIATE the thread is not enqueued and
 *                        the function returns @p MSG_TIMEOUT as if a timeout
 *                        occurred.
 *                      .
 * @return              The message from @p osalQueueWakeupOneI() or
 *                      @p osalQueueWakeupAllI() functions.
 * @retval RDY_TIMEOUT  if the thread has not been dequeued within the
 *                      specified timeout or if the function has been
 *                      invoked with @p TIME_IMMEDIATE as timeout
 *                      specification.
 *
 * @sclass
 */
msg_t osalQueueGoSleepTimeoutS(threads_queue_t *tqp, systime_t time) {
  msg_t msg;
  virtual_timer_t vt;

  void wakeup(void *p) {
    osalSysLockFromISR();
    osalThreadResumeI((thread_reference_t *)p, MSG_TIMEOUT);
    osalSysUnlockFromISR();
  }

  if (TIME_IMMEDIATE == time)
    return MSG_TIMEOUT;

  tqp->tr = NULL;

  if (TIME_INFINITE == time)
   return osalThreadSuspendS(&tqp->tr);

  osalVTSetI(&vt, time, wakeup, (void *)&tqp->tr);
  msg = osalThreadSuspendS(&tqp->tr);
  if (osalVTIsArmedI(&vt))
    osalVTResetI(&vt);
  return msg;
}

/** @} */
