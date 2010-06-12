/*
    ChibiOS/RT - Copyright (C) 2006,2007,2008,2009,2010 Giovanni Di Sirio.

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
 * @file    chthreads.c
 * @brief   Threads code.
 *
 * @addtogroup threads
 * @details Threads related APIs and services.
 *
 *          <h2>Operation mode</h2>
 *          A thread is an abstraction of an independent instructions flow.
 *          In ChibiOS/RT a thread is represented by a "C" function owning
 *          a processor context, state informations and a dedicated stack
 *          area. In this scenario static variables are shared among all
 *          threads while automatic variables are local to the thread.<br>
 *          Operations defined for threads:
 *          - <b>Init</b>, a thread is prepared and put in the suspended
 *            state.
 *          - <b>Create</b>, a thread is started on the specified thread
 *            function. This operation is available in multiple variants,
 *            both static and dynamic.
 *          - <b>Exit</b>, a thread terminates by returning from its top
 *            level function or invoking a specific API, the thread can
 *            return a value that can be retrieved by other threads.
 *          - <b>Wait</b>, a thread waits for the termination of another
 *            thread and retrieves its return value.
 *          - <b>Resume</b>, a thread created in suspended state is started.
 *          - <b>Sleep</b>, the execution of a thread is suspended for the
 *            specified amount of time or the specified future absolute time
 *            is reached.
 *          - <b>SetPriority</b>, a thread changes its own priority level.
 *          - <b>Yield</b>, a thread voluntarily renounces to its time slot.
 *          .
 *          The threads subsystem is implicitly included in kernel however
 *          some of its part may be excluded by disabling them in @p chconf.h,
 *          see the @p CH_USE_WAITEXIT and @p CH_USE_DYNAMIC configuration
 *          options.
 * @{
 */

#include "ch.h"

/**
 * @brief   Initializes a thread structure.
 *
 * @param[in] tp        pointer to the thread
 * @param[in] prio      the priority level for the new thread
 * @return              The same thread pointer passed as parameter.
 */
Thread *init_thread(Thread *tp, tprio_t prio) {

  tp->p_flags = THD_MEM_MODE_STATIC;
  tp->p_prio = prio;
  tp->p_state = THD_STATE_SUSPENDED;
#if CH_USE_REGISTRY
  REG_INSERT(tp);
#endif
#if CH_USE_DYNAMIC
  tp->p_refs = 1;
#endif
#if CH_USE_NESTED_LOCKS
  tp->p_locks = 0;
#endif
#if CH_DBG_THREADS_PROFILING
  tp->p_time = 0;
#endif
#if CH_USE_MUTEXES
  tp->p_realprio = prio;
  tp->p_mtxlist = NULL;
#endif
#if CH_USE_WAITEXIT
  list_init(&tp->p_waiting);
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
 * @brief   Initializes a new thread.
 * @details The new thread is initialized but not inserted in the ready list,
 *          the initial state is @p THD_STATE_SUSPENDED.
 * @note    A thread can terminate by calling @p chThdExit() or by simply
 *          returning from its main function.
 * @note    This function can be invoked from within an interrupt handler
 *          even if it is not an I-Class API because it does not touch
 *          any critical kernel data structure.
 *
 * @param[out] wsp      pointer to a working area dedicated to the thread stack
 * @param[in] size      size of the working area
 * @param[in] prio      the priority level for the new thread
 * @param[in] pf        the thread function
 * @param[in] arg       an argument passed to the thread function. It can be
 *                      @p NULL.
 * @return              The pointer to the @p Thread structure allocated for
 *                      the thread into the working space area.
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
 * @brief   Creates a new thread into a static memory area.
 * @note    A thread can terminate by calling @p chThdExit() or by simply
 *          returning from its main function.
 *
 * @param[out] wsp      pointer to a working area dedicated to the thread stack
 * @param[in] size      size of the working area
 * @param[in] prio      the priority level for the new thread
 * @param[in] pf        the thread function
 * @param[in] arg       an argument passed to the thread function. It can be
 *                      @p NULL.
 * @return              The pointer to the @p Thread structure allocated for
 *                      the thread into the working space area.
 */
Thread *chThdCreateStatic(void *wsp, size_t size,
                          tprio_t prio, tfunc_t pf, void *arg) {

  return chThdResume(chThdInit(wsp, size, prio, pf, arg));
}

#if CH_USE_DYNAMIC && CH_USE_HEAP
/**
 * @brief   Creates a new thread allocating the memory from the heap.
 * @note    A thread can terminate by calling @p chThdExit() or by simply
 *          returning from its main function.
 * @note    The memory allocated for the thread is not released when the thread
 *          terminates but when a @p chThdWait() is performed.
 * @note    The function is available only if the @p CH_USE_DYNAMIC,
 *          @p CH_USE_HEAP and @p CH_USE_WAITEXIT options are enabled
 *          in @p chconf.h.
 *
 * @param[in] heapp     heap from which allocate the memory or @p NULL for the
 *                      default heap
 * @param[in] size      size of the working area to be allocated
 * @param[in] prio      the priority level for the new thread
 * @param[in] pf        the thread function
 * @param[in] arg       an argument passed to the thread function. It can be
 *                      @p NULL.
 * @return              The pointer to the @p Thread structure allocated for
 *                      the thread into the working space area.
 * @retval NULL         if the memory cannot be allocated.
 */
Thread *chThdCreateFromHeap(MemoryHeap *heapp, size_t size,
                            tprio_t prio, tfunc_t pf, void *arg) {
  void *wsp;
  Thread *tp;

  wsp = chHeapAlloc(heapp, size);
  if (wsp == NULL)
    return NULL;
  tp = chThdInit(wsp, size, prio, pf, arg);
  tp->p_flags = THD_MEM_MODE_HEAP;
  return chThdResume(tp);
}
#endif /* CH_USE_DYNAMIC && CH_USE_HEAP */

#if CH_USE_DYNAMIC && CH_USE_MEMPOOLS
/**
 * @brief   Creates a new thread allocating the memory from the specified
 *          Memory Pool.
 * @note    A thread can terminate by calling @p chThdExit() or by simply
 *          returning from its main function.
 * @note    The memory allocated for the thread is not released when the thread
 *          terminates but when a @p chThdWait() is performed.
 * @note    The function is available only if the @p CH_USE_DYNAMIC,
 *          @p CH_USE_MEMPOOLS and @p CH_USE_WAITEXIT options are enabled
 *          in @p chconf.h.
 *
 * @param[in] mp        pointer to the memory pool object
 * @param[in] prio      the priority level for the new thread
 * @param[in] pf        the thread function
 * @param[in] arg       an argument passed to the thread function. It can be
 *                      @p NULL.
 * @return              The pointer to the @p Thread structure allocated for
 *                      the thread into the working space area.
 * @retval  NULL        if the memory pool is empty.
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
  tp->p_flags = THD_MEM_MODE_MEMPOOL;
  tp->p_mpool = mp;
  return chThdResume(tp);
}
#endif /* CH_USE_DYNAMIC && CH_USE_MEMPOOLS */

/**
 * @brief   Changes the running thread priority level then reschedules if
 *          necessary.
 * @note    The function returns the real thread priority regardless of the
 *          current priority that could be higher than the real priority
 *          because the priority inheritance mechanism.
 *
 * @param[in] newprio   the new priority level of the running thread
 * @return              The old priority level.
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
 * @brief   Resumes a suspended thread.
 * @note    Use this function to resume threads created with @p chThdInit().
 *
 * @param[in] tp        pointer to the thread
 * @return              The pointer to the thread.
 */
Thread *chThdResume(Thread *tp) {

  chSysLock();
  chDbgAssert(tp->p_state == THD_STATE_SUSPENDED,
              "chThdResume(), #1",
              "thread not in THD_STATE_SUSPENDED state");
  chSchWakeupS(tp, RDY_OK);
  chSysUnlock();
  return tp;
}

/**
 * @brief   Requests a thread termination.
 * @note    The thread is not terminated but a termination request is added to
 *          its @p p_flags field. The thread can read this status by
 *          invoking @p chThdShouldTerminate() and then terminate cleanly.
 *
 * @param[in] tp        pointer to the thread
 */
void chThdTerminate(Thread *tp) {

  chSysLock();
  tp->p_flags |= THD_TERMINATE;
  chSysUnlock();
}

/**
 * @brief   Suspends the invoking thread for the specified time.
 *
 * @param[in] time      the delay in system ticks, the special values are
 *                      handled as follow:
 *                      - @a TIME_INFINITE the thread enters an infinite sleep
 *                        state.
 *                      - @a TIME_IMMEDIATE this value is accepted but
 *                        interpreted as a normal time specification not as an
 *                        immediate timeout specification.
 *                      .
 */
void chThdSleep(systime_t time) {

  chDbgCheck(time != TIME_INFINITE, "chThdSleep");

  chSysLock();
  chThdSleepS(time);
  chSysUnlock();
}

/**
 * @brief   Suspends the invoking thread until the system time arrives to the
 *          specified value.
 *
 * @param[in] time      absolute system time
 */
void chThdSleepUntil(systime_t time) {

  chSysLock();
  if ((time -= chTimeNow()) > 0)
    chThdSleepS(time);
  chSysUnlock();
}

/**
 * @brief   Yields the time slot.
 * @details Yields the CPU control to the next thread in the ready list with
 *          equal priority, if any.
 */
void chThdYield(void) {

  chSysLock();
  chSchDoYieldS();
  chSysUnlock();
}

/**
 * @brief   Terminates the current thread by specifying an exit status code.
 *
 * @param[in] msg       thread exit code. The code can be retrieved by using
 *                      @p chThdWait().
 */
void chThdExit(msg_t msg) {
  Thread *tp = currp;

  chSysLock();
  tp->p_u.exitcode = msg;
  THREAD_EXT_EXIT(tp);
#if CH_USE_WAITEXIT
  while (notempty(&tp->p_waiting))
    chSchReadyI(list_remove(&tp->p_waiting));
#endif
#if CH_USE_REGISTRY
  REG_REMOVE(tp);
#endif
  chSchGoSleepS(THD_STATE_FINAL);
}

#if CH_USE_DYNAMIC || defined(__DOXYGEN__)
/**
 * @brief   Adds a reference to a thread object.
 *
 * @param[in] tp        pointer to the thread
 * @return              The same thread pointer passed as parameter
 *                      representing the new reference.
 */
Thread *chThdAddRef(Thread *tp) {

  chSysLock();
  chDbgAssert(tp->p_refs < 255, "chThdAddRef(), #1", "too many references");
  tp->p_refs++;
  chSysUnlock();
  return tp;
}

/**
 * @brief   Releases a reference to a thread object.
 * @details If the references counter reaches zero <b>and</b> the thread
 *          is in the @p THD_STATE_FINAL state then the thread's memory is
 *          returned to the proper allocator.
 * @note    Static threads are not affected.
 *
 * @param[in] tp        pointer to the thread
 */
void chThdRelease(Thread *tp) {
  trefs_t refs;

  chSysLock();
  chDbgAssert(tp->p_refs > 0, "chThdRelease(), #1", "not referenced");
  refs = --tp->p_refs;
  chSysUnlock();

  /* If the references counter reaches zero then the memory can be returned
     to the proper allocator. Of course static threads are not affected.*/
  if (refs == 0) {
    switch (tp->p_flags & THD_MEM_MODE_MASK) {
#if CH_USE_HEAP
    case THD_MEM_MODE_HEAP:
      chHeapFree(tp);
      break;
#endif
#if CH_USE_MEMPOOLS
    case THD_MEM_MODE_MEMPOOL:
      chPoolFree(tp->p_mpool, tp);
      break;
#endif
    }
  }
}
#endif /* CH_USE_DYNAMIC */

#if CH_USE_WAITEXIT || defined(__DOXYGEN__)
/**
 * @brief   Blocks the execution of the invoking thread until the specified
 *          thread terminates then the exit code is returned.
 * @details This function waits for the specified thread to terminate then
 *          decrements its reference counter, if the counter reaches zero then
 *          the thread working area is returned to the proper allocator.<br>
 *          The memory used by the exited thread is handled in different ways
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
 *          Please read the @ref article_lifecycle article for more details.
 * @note    After invoking @p chThdWait() the thread pointer becomes invalid
 *          and must not be used as parameter for further system calls.
 * @note    The function is available only if the @p CH_USE_WAITEXIT
 *          option is enabled in @p chconf.h.
 * @note    If @p CH_USE_DYNAMIC is not specified this function just waits for
 *          the thread termination, no memory allocators are involved.
 *
 * @param[in] tp        pointer to the thread
 * @return              The exit code from the terminated thread.
 */
msg_t chThdWait(Thread *tp) {
  msg_t msg;

  chDbgCheck(tp != NULL, "chThdWait");

  chSysLock();
  chDbgAssert(tp != currp, "chThdWait(), #1", "waiting self");
#if CH_USE_DYNAMIC
  chDbgAssert(tp->p_refs > 0, "chThdWait(), #2", "not referenced");
#endif
  if (tp->p_state != THD_STATE_FINAL) {
    list_insert(currp, &tp->p_waiting);
    chSchGoSleepS(THD_STATE_WTEXIT);
  }
  msg = tp->p_u.exitcode;
  chSysUnlock();
#if CH_USE_DYNAMIC
  chThdRelease(tp);
#endif
  return msg;
}
#endif /* CH_USE_WAITEXIT */

/** @} */
