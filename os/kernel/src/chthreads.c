/*
    ChibiOS/RT - Copyright (C) 2010 Giovanni Di Sirio.

    This file is part of ChibiOS/RT.

    ChibiOS/RT is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 3 of the License, or
    (at your option) any later version.

    ChibiOS/RT is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program. If not, see <http://www.gnu.org/licenses/>.

                                      ---

    A special exception to the GPL can be applied should you wish to distribute
    a combined work that includes ChibiOS/RT, without being obliged to provide
    the source code for any proprietary components. See the file exception.txt
    for full details of how and when the exception can be applied.
*/

/**
 * @file chthreads.c
 * @brief Threads code.
 * @addtogroup threads
 * @{
 */

#include "ch.h"

/*
 * Initializes a thread structure.
 */
Thread *init_thread(Thread *tp, tprio_t prio) {

  tp->p_flags = P_MEM_MODE_STATIC;
  tp->p_prio = prio;
  tp->p_state = PRSUSPENDED;
#if CH_USE_NESTED_LOCKS
  tp->p_locks = 0;
#endif
#if CH_DBG_THREADS_PROFILING
  tp->p_time = 0;
#endif
#if CH_USE_MUTEXES
  /* realprio is the thread's own, non-inherited, priority */
  tp->p_realprio = prio;
  tp->p_mtxlist = NULL;
#endif
#if CH_USE_WAITEXIT
  tp->p_waiting = NULL;
#endif
#if CH_USE_MESSAGES
  queue_init(&tp->p_msgqueue);
#endif
#if CH_USE_EVENTS
  tp->p_epending = 0;
#endif
  THREAD_EXT_INIT(tp);
  return tp;
}

#if CH_DBG_FILL_THREADS
static void memfill(uint8_t *startp, uint8_t *endp, uint8_t v) {

  while (startp < endp)
    *startp++ = v;
}
#endif

/**
 * @brief Initializes a new thread.
 * @details The new thread is initialized but not inserted in the ready list,
 *          the initial state is @p PRSUSPENDED.
 *
 * @param[out] wsp pointer to a working area dedicated to the thread stack
 * @param[in] size size of the working area
 * @param[in] prio the priority level for the new thread
 * @param[in] pf the thread function
 * @param[in] arg an argument passed to the thread function. It can be @p NULL.
 * @return The pointer to the @p Thread structure allocated for the
 *         thread into the working space area.
 * @note A thread can terminate by calling @p chThdExit() or by simply
 *       returning from its main function.
 * @note This function can be invoked from within an interrupt handler even if
 *       it is not an I-Class API because it does not touch any critical kernel
 *       data structure.
 */
Thread *chThdInit(void *wsp, size_t size, tprio_t prio, tfunc_t pf, void *arg) {
  /* Thread structure is layed out in the lower part of the thread workspace */
  Thread *tp = wsp;

  chDbgCheck((wsp != NULL) && (size >= THD_WA_SIZE(0)) &&
             (prio <= HIGHPRIO) && (pf != NULL),
             "chThdInit");
#if CH_DBG_FILL_THREADS
  memfill((uint8_t *)wsp, (uint8_t *)wsp + sizeof(Thread), THREAD_FILL_VALUE);
  memfill((uint8_t *)wsp + sizeof(Thread),
          (uint8_t *)wsp + size, STACK_FILL_VALUE);
#endif
  SETUP_CONTEXT(wsp, size, pf, arg);
  return init_thread(tp, prio);
}

/**
 * @brief Creates a new thread into a static memory area.
 *
 * @param[out] wsp pointer to a working area dedicated to the thread
 *                       stack
 * @param[in] size size of the working area
 * @param[in] prio the priority level for the new thread
 * @param[in] pf the thread function
 * @param[in] arg an argument passed to the thread function. It can be @p NULL.
 * @return The pointer to the @p Thread structure allocated for the
 *         thread into the working space area.
 * @note A thread can terminate by calling @p chThdExit() or by simply
 *       returning from its main function.
 */
Thread *chThdCreateStatic(void *wsp, size_t size,
                          tprio_t prio, tfunc_t pf, void *arg) {

  return chThdResume(chThdInit(wsp, size, prio, pf, arg));
}

#if CH_USE_DYNAMIC && CH_USE_WAITEXIT && CH_USE_HEAP
/**
 * @brief Creates a new thread allocating the memory from the heap.
 *
 * @param[in] heapp heap from which allocate the memory or NULL for the
 *                  default heap
 * @param[in] size size of the working area to be allocated
 * @param[in] prio the priority level for the new thread
 * @param[in] pf the thread function
 * @param[in] arg an argument passed to the thread function. It can be @p NULL.
 * @return The pointer to the @p Thread structure allocated for the
 *         thread into the working space area.
 * @retval NULL if the memory cannot be allocated.
 * @note A thread can terminate by calling @p chThdExit() or by simply
 *       returning from its main function.
 * @note The memory allocated for the thread is not released when the thread
 *       terminates but when a @p chThdWait() is performed.
 * @note The function is available only if the @p CH_USE_DYNAMIC,
 *       @p CH_USE_HEAP and @p CH_USE_WAITEXIT options are enabled
 *       in @p chconf.h.
 */
Thread *chThdCreateFromHeap(MemoryHeap *heapp, size_t size,
                            tprio_t prio, tfunc_t pf, void *arg) {
  void *wsp;
  Thread *tp;

  wsp = chHeapAlloc(heapp, size);
  if (wsp == NULL)
    return NULL;
  tp = chThdInit(wsp, size, prio, pf, arg);
  tp->p_flags = P_MEM_MODE_HEAP;
  return chThdResume(tp);
}
#endif /* CH_USE_DYNAMIC && CH_USE_WAITEXIT && CH_USE_HEAP */

#if CH_USE_DYNAMIC && CH_USE_WAITEXIT && CH_USE_MEMPOOLS
/**
 * @brief Creates a new thread allocating the memory from the specified Memory
 *        Pool.
 *
 * @param[in] mp the memory pool
 * @param[in] prio the priority level for the new thread
 * @param[in] pf the thread function
 * @param[in] arg an argument passed to the thread function. It can be @p NULL.
 * @return The pointer to the @p Thread structure allocated for the
 *         thread into the working space area or @p NULL if the memory cannot
 *         be allocated.
 * @retval NULL if the memory pool is empty.
 * @note A thread can terminate by calling @p chThdExit() or by simply
 *       returning from its main function.
 * @note The memory allocated for the thread is not released when the thread
 *       terminates but when a @p chThdWait() is performed.
 * @note The function is available only if the @p CH_USE_DYNAMIC,
 *       @p CH_USE_MEMPOOLS and @p CH_USE_WAITEXIT options are enabled
 *       in @p chconf.h.
 */
Thread *chThdCreateFromMemoryPool(MemoryPool *mp, tprio_t prio,
                                  tfunc_t pf, void *arg) {
  void *wsp;
  Thread *tp;

  chDbgCheck(mp != NULL, "chThdCreateFromMemoryPool");

  wsp = chPoolAlloc(mp);
  if (wsp == NULL)
    return NULL;
  tp = chThdInit(wsp, mp->mp_object_size, prio, pf, arg);
  tp->p_flags = P_MEM_MODE_MEMPOOL;
  tp->p_mpool = mp;
  return chThdResume(tp);
}
#endif /* CH_USE_DYNAMIC && CH_USE_WAITEXIT && CH_USE_MEMPOOLS */

/**
 * @brief Changes the running thread priority level then reschedules if
 *        necessary.
 *
 * @param[in] newprio the new priority level of the running thread
 * @return The old priority level.
 * @note The function returns the real thread priority regardless of the
 *       current priority that could be higher than the real priority because
 *       the priority inheritance mechanism.
 */
tprio_t chThdSetPriority(tprio_t newprio) {
  tprio_t oldprio;

  chDbgCheck((newprio >= LOWPRIO) && (newprio <= HIGHPRIO),
              "chThdSetPriority");

  chSysLock();
#if CH_USE_MUTEXES
  oldprio = currp->p_realprio;
  if ((currp->p_prio == currp->p_realprio) || (newprio > currp->p_prio))
    currp->p_prio = newprio;
  currp->p_realprio = newprio;
#else
  oldprio = currp->p_prio;
  currp->p_prio = newprio;
#endif
  chSchRescheduleS();
  chSysUnlock();
  return oldprio;
}

/**
 * @brief Resumes a suspended thread.
 *
 * @param[in] tp the pointer to the thread
 * @return The pointer to the thread.
 * @note This call is supposed to resume threads created with @p chThdInit().
 *       It should not be used on threads suspended using @p chThdSuspend().
 */
Thread *chThdResume(Thread *tp) {

  chSysLock();
  chDbgAssert(tp->p_state == PRSUSPENDED,
              "chThdResume(), #1",
              "thread not in PRSUSPENDED state");
  chSchWakeupS(tp, RDY_OK);
  chSysUnlock();
  return tp;
}

/**
 * @brief Requests a thread termination.
 *
 * @param[in] tp the pointer to the thread
 * @note The thread is not termitated but a termination request is added to
 *       its @p p_flags field. The thread can read this status by
 *       invoking @p chThdShouldTerminate() and then terminate cleanly.
 */
void chThdTerminate(Thread *tp) {

  chSysLock();
  tp->p_flags |= P_TERMINATE;
  chSysUnlock();
}

/**
 * @brief Suspends the invoking thread for the specified time.
 *
 * @param[in] time the delay in system ticks, the special values are handled as
 *                 follow:
 *                 - @a TIME_INFINITE the thread enters an infinite sleep
 *                   state.
 *                 - @a TIME_IMMEDIATE this value is accepted but interpreted
 *                   as a normal time specification not as an immediate timeout
 *                   specification.
 *                 .
 */
void chThdSleep(systime_t time) {

  chDbgCheck(time != TIME_INFINITE, "chThdSleep");

  chSysLock();
  chThdSleepS(time);
  chSysUnlock();
}

/**
 * @brief Suspends the invoking thread until the system time arrives to the
 *        specified value.
 *
 * @param[in] time the absolute system time
 */
void chThdSleepUntil(systime_t time) {

  chSysLock();
  if ((time -= chTimeNow()) > 0)
    chThdSleepS(time);
  chSysUnlock();
}

/**
 * @brief Yields the time slot.
 * @details Yields the CPU control to the next thread in the ready list with
 *          equal priority, if any.
 */
void chThdYield(void) {

  chSysLock();
  chSchDoYieldS();
  chSysUnlock();
}

/**
 * @brief Terminates the current thread by specifying an exit status code.
 *
 * @param[in] msg the thread exit code. The code can be retrieved by using
 *                @p chThdWait().
 */
void chThdExit(msg_t msg) {
  Thread *tp = currp;

  chSysLock();
  tp->p_exitcode = msg;
  THREAD_EXT_EXIT(tp);
#if CH_USE_WAITEXIT
  if (tp->p_waiting != NULL)
    chSchReadyI(tp->p_waiting);
#endif
  chSchGoSleepS(PREXIT);
}

#if CH_USE_WAITEXIT
/**
 * @brief Blocks the execution of the invoking thread until the specified
 *        thread terminates then the exit code is returned.
 * @details The memory used by the exited thread is handled in different ways
 *          depending on the API that spawned the thread:
 *          - If the thread was spawned by @p chThdCreateStatic() or by
 *            @p chThdInit() then nothing happens and the thread working area
 *            is not released or modified in any way. This is the default,
 *            totally static, behavior.
 *          - If the thread was spawned by @p chThdCreateFromHeap() then
 *            the working area is returned to the system heap.
 *          - If the thread was spawned by @p chThdCreateFromMemoryPool()
 *            then the working area is returned to the owning memory pool.
 *          .
 * @param[in] tp the thread pointer
 * @return The exit code from the terminated thread
 * @note After invoking @p chThdWait() the thread pointer becomes invalid and
 *       must not be used as parameter for further system calls.
 * @note The function is available only if the @p CH_USE_WAITEXIT
 *       option is enabled in @p chconf.h.
 * @note Only one thread can be waiting for another thread at any time. You
 *       should imagine the threads as having a reference counter that is set
 *       to one when the thread is created, chThdWait() decreases the reference
 *       and the memory is freed when the counter reaches zero. In the current
 *       implementation there is no real reference counter in the thread
 *       structure but it is a planned extension.
 */
msg_t chThdWait(Thread *tp) {
  msg_t msg;

  chDbgCheck(tp != NULL, "chThdWait");

  chSysLock();

  chDbgAssert(tp != currp, "chThdWait(), #1", "waiting self");
  chDbgAssert(tp->p_waiting == NULL, "chThdWait(), #2", "some other thread waiting");

  if (tp->p_state != PREXIT) {
    tp->p_waiting = currp;
    chSchGoSleepS(PRWAIT);
  }
  msg = tp->p_exitcode;
#if !CH_USE_DYNAMIC
  chSysUnlock();
  return msg;
#else /* CH_USE_DYNAMIC */

  /* Returning memory.*/
  tmode_t mode = tp->p_flags & P_MEM_MODE_MASK;
  chSysUnlock();

  switch (mode) {
#if CH_USE_HEAP
  case P_MEM_MODE_HEAP:
    chHeapFree(tp);
    break;
#endif
#if CH_USE_MEMPOOLS
  case P_MEM_MODE_MEMPOOL:
    chPoolFree(tp->p_mpool, tp);
    break;
#endif
  }
  return msg;
#endif /* CH_USE_DYNAMIC */
}
#endif /* CH_USE_WAITEXIT */

/** @} */
