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
__attribute__((weak))
#endif
void osalSysHalt(const char *reason) {

  osal_halt_msg = reason;
  chSysHalt();
}

/**
 * @brief   Sends the current thread sleeping and sets a reference variable.
 * @note    This function must reschedule, it can only be called from thread
 *          context.
 *
 * @param[in] trp       a pointer to a thread reference object
 * @return              The wake up message.
 *
 * @sclass
 */
msg_t osalThreadSuspendS(thread_reference_t *trp) {

  chDbgAssert(*trp == NULL, "osalThreadSuspendS(), #1", "not NULL");

  *trp = (thread_reference_t)chThdSelf();
  chSchGoSleepS(CH_STATE_SUSPENDED);
  return chThdSelf()->p_msg;
}

/**
 * @brief   Wakes up a thread waiting on a thread reference object.
 * @note    This function must not reschedule because it can be called from
 *          ISR context.
 *
 * @param[in] trp       a pointer to a thread reference object
 * @param[in] msg       the message code
 *
 * @iclass
 */
void osalThreadResumeI(thread_reference_t *trp, msg_t msg) {

  if (*trp != NULL) {

    chDbgAssert((*trp)->p_state == CH_STATE_SUSPENDED,
                "osalThreadResumeI(), #1", "not THD_STATE_SUSPENDED");

    (*trp)->p_u.rdymsg = msg;
    chSchReadyI((thread_t *)*trp);
    *trp = NULL;
  }
}

/**
 * @brief   Wakes up a thread waiting on a thread reference object.
 * @note    This function must reschedule, it can only be called from thread
 *          context.
 *
 * @param[in] trp       a pointer to a thread reference object
 * @param[in] msg       the message code
 *
 * @iclass
 */
void osalThreadResumeS(thread_reference_t *trp, msg_t msg) {

  if (*trp != NULL) {
    thread_t *tp = (thread_t *)*trp;

    chDbgAssert(tp->p_state == CH_STATE_SUSPENDED,
                "osalThreadResumeS(), #1", "not THD_STATE_SUSPENDED");

    *trp = NULL;
    chSchWakeupS(tp, msg);
  }
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

  void wakeup(void *p) {
    thread_t *tp = (thread_t *)p;

    chSysUnlockFromISR();
    tp->p_u.rdymsg = RDY_TIMEOUT;
    chSchReadyI(queue_dequeue(tp));
    chSysUnlockFromISR();
  }

  if (TIME_IMMEDIATE == time)
    return MSG_TIMEOUT;

  queue_insert(currp, tqp);
  if (TIME_INFINITE == time)
    chSchGoSleepS(CH_STATE_SUSPENDED);
  else {
    virtual_timer_t vt;

    chVTDoSetI(&vt, time, wakeup, currp);
    chSchGoSleepS(CH_STATE_SUSPENDED);
    if (chVTIsArmedI(&vt))
      chVTDoResetI(&vt);
  }
  return currp->p_u.rdymsg;
}

/**
 * @brief   Dequeues and wakes up one thread from the queue, if any.
 *
 * @param[in] tqp       pointer to the threads queue object
 * @param[in] msg       the message code
 *
 * @iclass
 */
void osalQueueWakeupOneI(threads_queue_t *tqp, msg_t msg) {

  if (queue_notempty(tqp)) {
    thread_t *tp = queue_fifo_remove(tqp);
    tp->p_u.rdymsg = msg;
    chSchReadyI(tp);
  }
}

/**
 * @brief   Dequeues and wakes up all threads from the queue.
 *
 * @param[in] tqp       pointer to the threads queue object
 * @param[in] msg       the message code
 *
 * @iclass
 */
void osalQueueWakeupAllI(threads_queue_t *tqp, msg_t msg) {

  while (queue_notempty(tqp)) {
    thread_t *tp = queue_fifo_remove(tqp);
    tp->p_u.rdymsg = msg;
    chSchReadyI(tp);
  }
}

/** @} */
