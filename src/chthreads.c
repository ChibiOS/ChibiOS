/*
    ChibiOS/RT - Copyright (C) 2006-2007 Giovanni Di Sirio.

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
 * @addtogroup Threads
 * @{
 */

#include <ch.h>

/*
 * Initializes a thread structure.
 */
void init_thread(tprio_t prio, tmode_t mode, Thread *tp) {
  static tid_t nextid = 0;

  tp->p_tid = nextid++;
  tp->p_flags = mode;
  tp->p_prio = prio;
#ifdef CH_USE_MUTEXES
  /* realprio is the thread's own, non-inherited, priority */
  tp->p_realprio = prio;
  tp->p_mtxlist = NULL;
#endif
#ifdef CH_USE_WAITEXIT
  list_init(&tp->p_waiting);
#endif
#ifdef CH_USE_MESSAGES
  fifo_init(&tp->p_msgqueue);
#endif
#ifdef CH_USE_EVENTS
  tp->p_epending = 0;
#endif
#ifdef CH_USE_EXIT_EVENT
  chEvtInit(&tp->p_exitesource);
#endif
#ifdef CH_USE_THREAD_EXT
  THREAD_EXT_INIT(tp);
#endif
}

#ifdef CH_USE_DEBUG
static void memfill(uint8_t *p, uint32_t n, uint8_t v) {

  while (n)
    *p++ = v, n--;
}
#endif

/**
 * Creates a new thread.
 * @param prio the priority level for the new thread. Usually the threads are
 *             created with priority \p NORMALPRIO, priorities
 *             can range from \p LOWPRIO to \p HIGHPRIO.
 * @param mode the creation option flags for the thread. The following options
 *             can be OR'ed in this parameter:<br>
 *             <ul>
 *             <li>\p P_SUSPENDED, the thread is created in the
 *                 \p PRSUSPENDED state and a subsequent call to
 *                 \p chThdResume() will make it ready for
 *                 execution.</li>
 *             <li>\p P_TERMINATED, this flag is usually set
 *                 by the \p chThdTerminate() function and it is not
 *                 normally used as parameter for this function. The
 *                 result would be to create a thread with a termination
 *                 request already pending.</li>
 *             </ul>
 * @param workspace pointer to a working area dedicated to the thread stack
 * @param wsize size of the working area.
 * @param pf the thread function. Returning from this function automatically
 *           terminates the thread.
 * @param arg an argument passed to the thread function. It can be \p NULL.
 * @return the pointer to the \p Thread structure allocated for the
 *         thread into the working space area.
 * @note A thread can terminate by calling \p chThdExit() or by simply
 *       returning from its main function.
 */
Thread *chThdCreate(tprio_t prio, tmode_t mode, void *workspace,
                    size_t wsize, tfunc_t pf, void *arg) {
  /* thread structure is layed out in the lower part of the thread workspace */
  Thread *tp = workspace;

  chDbgAssert((wsize >= UserStackSize(0)) && (prio <= HIGHPRIO) &&
              (workspace != NULL) && (pf != NULL),
              "chthreads.c, chThdCreate()");
#ifdef CH_USE_DEBUG
  memfill(workspace, wsize, MEM_FILL_PATTERN);
#endif
  SETUP_CONTEXT(workspace, wsize, pf, arg);
  init_thread(prio, mode, tp);
#ifdef CH_USE_RESUME
  if (tp->p_flags & P_SUSPENDED)
    tp->p_state = PRSUSPENDED;
  else {
#endif
    chSysLock();
    chSchWakeupS(tp, RDY_OK);
    chSysUnlock();
#ifdef CH_USE_RESUME
  }
#endif
  return tp;
}

/**
 * Creates a new thread.
 * @param prio the priority level for the new thread. Usually the threads are
 *             created with priority \p NORMALPRIO, priorities
 *             can range from \p LOWPRIO to \p HIGHPRIO.
 * @param workspace pointer to a working area dedicated to the thread stack
 * @param wsize size of the working area.
 * @param pf the thread function. Returning from this function automatically
 *           terminates the thread.
 * @return the pointer to the \p Thread structure allocated for the
 *         thread into the working space area.
 * @note A thread can terminate by calling \p chThdExit() or by simply
 *       returning from its main function.
 */
Thread *chThdCreateFast(tprio_t prio, void *workspace,
                        size_t wsize, tfunc_t pf) {
  Thread *tp = workspace;

  chDbgAssert((wsize >= UserStackSize(0)) && (prio <= HIGHPRIO) &&
              (workspace != NULL) && (pf != NULL),
              "chthreads.c, chThdCreateFast()");
#ifdef CH_USE_DEBUG
  memfill(workspace, wsize, MEM_FILL_PATTERN);
#endif
  SETUP_CONTEXT(workspace, wsize, pf, NULL);
  init_thread(prio, 0, tp);
  chSysLock();
  chSchWakeupS(tp, RDY_OK);
  chSysUnlock();
  return tp;
}

/**
 * Changes the running thread priority, reschedules if necessary.
 *
 * @param newprio the new priority of the running thread
 */
void chThdSetPriority(tprio_t newprio) {

  chDbgAssert(newprio <= HIGHPRIO, "chthreads.c, chThdSetPriority()");
  chSysLock();

#ifdef CH_USE_MUTEXES
  if (currp->p_prio != currp->p_realprio) {
    if (newprio > currp->p_prio)
      currp->p_prio = newprio;
  }
  else
    currp->p_prio = newprio;
  currp->p_realprio = newprio;
#else
  currp->p_prio = newprio;
#endif
  chSchRescheduleS();

  chSysUnlock();
}

/**
 * Suspends the invoking thread.
 *
 * @param tpp pointer to a \p Thread pointer, the \p Thread pointer is set
 *            to point to the suspended process before it enters the
 *            \p PRSUSPENDED state, it is set to \p NULL after it is resumed.
 *            This allows to implement a "test and resume" on the variable
 *            into interrupt handlers.
 */
void chThdSuspend(Thread **tpp) {

  chSysLock();
  chDbgAssert(*tpp == NULL, "chthreads.c, chThdSuspend()");
  *tpp = currp;
  chSchGoSleepS(PRSUSPENDED);
  *tpp = NULL;
  chSysUnlock();
}

/**
 * Resumes a suspended thread.
 * @param tp the pointer to the thread
 */
void chThdResume(Thread *tp) {

  chSysLock();
  chDbgAssert(tp->p_state == PRSUSPENDED, "chthreads.c, chThdResume()");
  chSchWakeupS(tp, RDY_OK);
  chSysUnlock();
}

/**
 * Requests a thread termination.
 * @param tp the pointer to the thread
 * @note The thread is not termitated but a termination request is added to
 *       its \p p_flags field. The thread can read this status by
 *       invoking \p chThdShouldTerminate() and then terminate cleanly.
 */
void chThdTerminate(Thread *tp) {

  chSysLock();
  tp->p_flags |= P_TERMINATE;
  chSysUnlock();
}

/**
 * Terminates the current thread by specifying an exit status code.
 *
 * @param msg the thread exit code. The code can be retrieved by using
 *            \p chThdWait().
 */
void chThdExit(msg_t msg) {

  chSysLock();

  currp->p_exitcode = msg;
#ifdef CH_USE_WAITEXIT
  while (notempty(&currp->p_waiting))
    chSchReadyI(list_remove(&currp->p_waiting));
#endif
#ifdef CH_USE_EXIT_EVENT
  chEvtBroadcastI(&currp->p_exitesource);
#endif
  chSchGoSleepS(PREXIT);
}

#ifdef CH_USE_WAITEXIT
/**
 * Blocks the execution of the invoking thread until the specified thread
 * terminates then the exit code is returned.
 *
 * @param tp the pointer to the thread
 * @return the exit code
 * @note The function is available only if the \p CH_USE_WAITEXIT
 *       option is enabled in \p chconf.h.
 */
msg_t chThdWait(Thread *tp) {
  msg_t msg;

  chSysLock();

  if (tp->p_state != PREXIT) {
    list_insert(currp, &tp->p_waiting);
    chSchGoSleepS(PRWAIT);
  }
  msg = tp->p_exitcode;

  chSysUnlock();
  return msg;
}
#endif /* CH_USE_WAITEXIT */

/** @} */
