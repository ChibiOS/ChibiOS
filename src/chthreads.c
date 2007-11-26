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
void _InitThread(t_prio prio, t_tmode mode, Thread *tp) {
  static t_tid nextid = 0;

  tp->p_tid = nextid++;
  tp->p_flags = mode;
  tp->p_prio = prio;
  tp->p_rdymsg = RDY_OK;
#ifdef CH_USE_RT_SEMAPHORES
  tp->p_rtcnt = 0;
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
}

/**
 * Creates a new thread.
 * @param prio the priority level for the new thread. Usually the threads are
 *             created with priority \p NORMALPRIO (128), priorities
 *             can range from \p LOWPRIO (1) to \p HIGHPRIO
 *             (255).
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
Thread *chThdCreate(t_prio prio, t_tmode mode, void *workspace,
                    t_size wsize, t_tfunc pf, void *arg) {
  Thread *tp = workspace;

  _InitThread(prio, mode, tp);
  SETUP_CONTEXT(workspace, wsize, pf, arg);
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
 * Changes the thread priority, reschedules if necessary.
 * @param newprio the new priority of the invoking thread
 */
void chThdSetPriority(t_prio newprio) {

  chSysLock();

#ifdef CH_USE_RT_SEMAPHORES
  if (currp->p_rtcnt)
    currp->p_prio = newprio + MEPRIO;
  else
    currp->p_prio = newprio;
#else
  currp->p_prio = newprio;
#endif
  chSchRescheduleS();

  chSysUnlock();
}

#ifdef CH_USE_SUSPEND
/**
 * Suspends the invoking thread.
 *
 * @param tpp pointer to a \p Thread pointer, the \p Thread pointer is set
 *            to point to the suspended process before it enters the
 *            \p PRSUSPENDED state, it is set to \p NULL after it is resumed.
 *            This allows to implement a "test and resume" on the variable
 *            into interrupt handlers.
 * @note The function is available only if the \p CH_USE_SUSPEND
 *       option is enabled in \p chconf.h.
 */
void chThdSuspend(Thread **tpp) {

  chSysLock();

#ifdef CH_USE_DEBUG
  if (*tpp)
    chDbgPanic("chthreads.c, chThdSuspend()");
#endif
  *tpp = currp;
  chSchGoSleepS(PRSUSPENDED);
  *tpp = NULL;

  chSysUnlock();
}
#endif /* CH_USE_SUSPEND */

#ifdef CH_USE_RESUME
/**
 * Resumes a thread created with the \p P_SUSPENDED option or suspended with
 * \p chThdSuspend().
 * @param tp the pointer to the thread
 * @note The function has no effect on threads in any other state than
 *       \p PRSUSPENDED.
 * @note The function is available only if the \p CH_USE_RESUME
 *       option is enabled in \p chconf.h.
 */
void chThdResume(Thread *tp) {

  chSysLock();

  if ((tp)->p_state == PRSUSPENDED)
    chSchWakeupS((tp), RDY_OK);

  chSysUnlock();
}
#endif

#ifdef CH_USE_TERMINATE
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
#endif

/**
 * Terminates the current thread by specifying an exit status code.
 * @param msg the thread exit code. The code can be retrieved by using
 *            \p chThdWait().
 */
void chThdExit(t_msg msg) {

  chSysLock();

  currp->p_exitcode = msg;
#ifdef CH_USE_WAITEXIT
  while (notempty(&currp->p_waiting))
    chSchReadyI(list_remove(&currp->p_waiting), RDY_OK);
#endif
#ifdef CH_USE_EXIT_EVENT
  chEvtSendI(&currp->p_exitesource);
#endif
  chSchGoSleepS(PREXIT);
}

#ifdef CH_USE_WAITEXIT
/**
 * Blocks the execution of the invoking thread until the specified thread
 * terminates then the exit code is returned.
 * @param tp the pointer to the thread
 * @return the exit code
 * @note The function is available only if the \p CH_USE_WAITEXIT
 *       option is enabled in \p chconf.h.
 */
t_msg chThdWait(Thread *tp) {
  t_msg msg;

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
