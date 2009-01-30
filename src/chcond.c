/*
    ChibiOS/RT - Copyright (C) 2009 Giovanni Di Sirio.

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

                                      ---

    A special exception to the GPL can be applied should you wish to distribute
    a combined work that includes ChibiOS/RT, without being obliged to provide
    the source code for any proprietary components. See the file exception.txt
    for full details of how and when the exception can be applied.
*/
/*
   Concepts and parts of this file are contributed by and Copyright (C) 2008
   of Leon Woestenberg.
 */

/**
 * @addtogroup CondVars
 * @{
 */

#include <ch.h>

#if defined(CH_USE_CONDVARS) && defined(CH_USE_MUTEXES)

/**
 * Initializes s \p CondVar structure.
 * @param cp pointer to a \p CondVar structure
 */
void chCondInit(CondVar *cp) {

  queue_init(&cp->c_queue);
}

/**
 * Signals one thread that is waiting on the condition variable.
 *
 * @param cp pointer to the \p CondVar structure
 */
void chCondSignal(CondVar *cp) {

  chSysLock();

  if (notempty(&cp->c_queue))                           /* any thread ? */
    chSchWakeupS(fifo_remove(&cp->c_queue), RDY_OK);

  chSysUnlock();
}

/**
 * Signals one thread that is waiting on the condition variable.
 *
 * @param cp pointer to the \p CondVar structure
 * @note This function must be called within a \p chSysLock() / \p chSysUnlock()
 *       block.
 */
void chCondSignalI(CondVar *cp) {

  if (notempty(&cp->c_queue))                           /* any thread ? */
    chSchReadyI(fifo_remove(&cp->c_queue))->p_rdymsg = RDY_OK;
}

/**
 * Signals all threads that are waiting on the condition variable.
 *
 * @param cp pointer to the \p CondVar structure
 */
void chCondBroadcast(CondVar *cp) {

  chSysLock();

  chCondBroadcastI(cp);
  chSchRescheduleS();

  chSysUnlock();
}

/**
 * Signals all threads that are waiting on the condition variable.
 *
 * @param cp pointer to the \p CondVar structure
 * @note This function must be called within a \p chSysLock() / \p chSysUnlock()
 */
void chCondBroadcastI(CondVar *cp) {

  /* empties the condition variable queue and inserts all the Threads into the
   * ready list in FIFO order. The wakeup message is set to \p RDY_RESET in
   * order to make a chCondBroadcast() detectable from a chCondSignal(). */
  while (cp->c_queue.p_next != (void *)&cp->c_queue)
    chSchReadyI(fifo_remove(&cp->c_queue))->p_rdymsg = RDY_RESET;
}

/**
 * Waits on the condition variable releasing the mutex lock.
 *
 * Releases the mutex, waits on the condition variable, and finally acquires
 * the mutex again. This is done atomically.
 *
 * The thread MUST already have locked the mutex when calling chCondWait().
 *
 * @param cp pointer to the \p CondVar structure
 * @return The wakep mode.
 * @retval RDY_OK if the condvar was signaled using chCondSignal().
 * @retval RDY_RESET if the condvar was signaled using chCondBroadcast().
 */
msg_t chCondWait(CondVar *cp) {
  msg_t msg;

  chSysLock();

  msg = chCondWaitS(cp);

  chSysUnlock();
  return msg;
}

/**
 * Waits on the condition variable releasing the mutex lock.
 *
 * Releases the mutex, waits on the condition variable, and finally acquires
 * the mutex again. This is done atomically.
 *
 * The thread MUST already have locked the mutex when calling chCondWait().
 *
 * @param cp pointer to the \p CondVar structure
 * @return The wakep mode.
 * @retval RDY_OK if the condvar was signaled using chCondSignal().
 * @retval RDY_RESET if the condvar was signaled using chCondBroadcast().
 * @note This function must be called within a \p chSysLock() / \p chSysUnlock()
 */
msg_t chCondWaitS(CondVar *cp) {
  Mutex *mp;
  msg_t msg;

  chDbgAssert(currp->p_mtxlist != NULL, "chcond.c, chCondWaitS()");

  mp = chMtxUnlockS();                  /* unlocks the condvar mutex */
  prio_insert(currp, &cp->c_queue);     /* enters the condvar queue */
  currp->p_wtcondp = cp;                /* needed by the tracer */
  chSchGoSleepS(PRWTCOND);              /* waits on the condvar */
  msg = currp->p_rdymsg;                /* fetches the wakeup message */
  chMtxLockS(mp);                       /* atomically relocks the mutex */
  return msg;                           /* returns the wakeup message */
}

#ifdef CH_USE_CONDVARS_TIMEOUT
/**
 * Waits on the condition variable releasing the mutex lock.
 *
 * Releases the mutex, waits on the condition variable, and finally acquires
 * the mutex again. This is done atomically.
 *
 * The thread MUST already have locked the mutex when calling chCondWait().
 *
 * @param cp pointer to the \p CondVar structure
 * @param time the number of ticks before the operation fails
 * @return The wakep mode.
 * @retval RDY_OK if the condvar was signaled using chCondSignal().
 * @retval RDY_RESET if the condvar was signaled using chCondBroadcast().
 * @retval RDY_TIMEOUT if the condvar was not signaled within the specified
 *         timeout.
 */
msg_t chCondWaitTimeout(CondVar *cp, systime_t time) {
  msg_t msg;

  chSysLock();

  msg = chCondWaitTimeoutS(cp, time);

  chSysUnlock();
  return msg;
}

/**
 * Waits on the condition variable releasing the mutex lock.
 *
 * Releases the mutex, waits on the condition variable, and finally acquires
 * the mutex again. This is done atomically.
 *
 * The thread MUST already have locked the mutex when calling chCondWait().
 *
 * @param cp pointer to the \p CondVar structure
 * @param time the number of ticks before the operation fails
 * @return The wakep mode.
 * @retval RDY_OK if the condvar was signaled using chCondSignal().
 * @retval RDY_RESET if the condvar was signaled using chCondBroadcast().
 * @retval RDY_TIMEOUT if the condvar was not signaled within the specified
 *         timeout.
 * @note This function must be called within a \p chSysLock() / \p chSysUnlock()
 */
msg_t chCondWaitTimeoutS(CondVar *cp, systime_t time) {
  Mutex *mp;
  msg_t msg;

  chDbgAssert(currp->p_mtxlist != NULL, "chcond.c, chCondWaitS()");

  mp = chMtxUnlockS();                  /* unlocks the condvar mutex */
  prio_insert(currp, &cp->c_queue);     /* enters the condvar queue */
  currp->p_wtcondp = cp;                /* needed by the tracer */
  chSchGoSleepTimeoutS(PRWTCOND, time); /* waits on the condvar */
  msg = currp->p_rdymsg;                /* fetches the wakeup message */
  chMtxLockS(mp);                       /* atomically relocks the mutex */
  return msg;                           /* returns the wakeup message */
}
#endif /* CH_USE_CONDVARS_TIMEOUT */

#endif /* defined(CH_USE_CONDVARS) && defined(CH_USE_MUTEXES) */

/** @} */
