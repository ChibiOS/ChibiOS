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
 * @brief   Dequeues and wakes up one thread from the queue, if any.
 *
 * @param[in] tqp       pointer to the threads queue object
 * @param[in] msg       the message code
 *
 * @iclass
 */
void osalThreadDequeueNextI(threads_queue_t *tqp, msg_t msg) {
  semaphore_t *sp = &tqp->sem;

  if (chSemGetCounterI(&tqp->sem) < (cnt_t)0) {
    thread_reference_t tr = nil.threads;
    while (true) {
      /* Is this thread waiting on this semaphore?*/
      if (tr->u1.semp == sp) {
        sp->cnt++;

        chDbgAssert(NIL_THD_IS_WTSEM(tr), "not waiting");

        (void) chSchReadyI(tr, msg);
        return;
      }
      tr++;

      chDbgAssert(tr < &nil.threads[NIL_CFG_NUM_THREADS],
                  "pointer out of range");
    }
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
void osalThreadDequeueAllI(threads_queue_t *tqp, msg_t msg) {
  semaphore_t *sp = &tqp->sem;
  thread_reference_t tr;
  cnt_t cnt;

  cnt = sp->cnt;
  sp->cnt = (cnt_t)0;
  tr = nil.threads;
  while (cnt < (cnt_t)0) {
    /* Is this thread waiting on this semaphore?*/
    if (tr->u1.semp == sp) {

      chDbgAssert(NIL_THD_IS_WTSEM(tr), "not waiting");

      cnt++;
      (void) chSchReadyI(tr, msg);
    }
    tr++;

    chDbgAssert(tr < &nil.threads[NIL_CFG_NUM_THREADS],
                "pointer out of range");
  }
}

/** @} */
