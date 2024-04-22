/*
    ChibiOS - Copyright (C) 2006,2007,2008,2009,2010,2011,2012,2013,2014,
              2015,2016,2017,2018,2019,2020,2021 Giovanni Di Sirio.

    This file is part of ChibiOS.

    ChibiOS is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation version 3 of the License.

    ChibiOS is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

/**
 * @file    rt/src/chthreads.c
 * @brief   Threads management code.
 *
 * @addtogroup threads
 * @details Threads related APIs and services.
 *          <h2>Operation mode</h2>
 *          A thread is an abstraction of an independent instructions flow.
 *          In ChibiOS/RT a thread is represented by a "C" function owning
 *          a processor context, state informations and a dedicated stack
 *          area. In this scenario static variables are shared among all
 *          threads while automatic variables are local to the thread.<br>
 *          Operations defined for threads:
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
 * @{
 */

#include <string.h>

#include "ch.h"

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
 * @brief   Thread object initialization.
 * @note    This function does not create a fully initialized thread, do
 *          not use directly.
 *
 * @param[out] tp       pointer to a @p thread_t object
 * @param[in] tdp       pointer to the thread descriptor
 * @return              The same thread pointer passed as parameter.
 *
 * @init
 */
thread_t *chThdObjectInit(thread_t *tp,
                          const thread_descriptor_new_t *tdp) {

  chDbgCheck(tp != NULL);
  chDbgCheck(tdp != NULL);

#if (CH_DBG_ENABLE_STACK_CHECK == TRUE) || (CH_CFG_USE_DYNAMIC == TRUE)
  /* Stack boundary.*/
  tp->wabase = (void *)tdp->wa.base;
  tp->waend  = (void *)(tdp->wa.base + tdp->wa.size);
#endif

  /* Setting up the port-dependent part of the working area.*/
  /* TODO: Remove redundant parameters.*/
  PORT_SETUP_CONTEXT(tp, tp->wabase, tp->waend, tdp->funcp, tdp->arg);

  /* Thread-related fields.*/
  tp->hdr.pqueue.prio   = tdp->prio;
  tp->state             = CH_STATE_WTSTART;
  tp->flags             = CH_FLAG_MODE_STATIC;
  if (tdp->owner != NULL) {
    tp->owner           = tdp->owner;
  }
  else {
    tp->owner           = currcore;
  }
#if CH_CFG_TIME_QUANTUM > 0
  tp->ticks             = (tslices_t)CH_CFG_TIME_QUANTUM;
#endif
#if CH_CFG_USE_WAITEXIT == TRUE
  ch_list_init(&tp->waiting);
#endif

  /* Mutex-related fields.*/
#if CH_CFG_USE_MUTEXES == TRUE
  tp->realprio          = tdp->prio;
  tp->mtxlist           = NULL;
#endif

  /* Events-related fields.*/
#if CH_CFG_USE_EVENTS == TRUE
  tp->epending          = (eventmask_t)0;
#endif

  /* Debug-related fields.*/
#if CH_DBG_THREADS_PROFILING == TRUE
  tp->time              = (systime_t)0;
#endif

  /* Registry-related fields.*/
#if CH_CFG_USE_REGISTRY == TRUE
  tp->refs              = (trefs_t)1;
  tp->name              = tdp->tname;
//  REG_INSERT(tp->owner, tp);
#else
  (void)name;
#endif

  /* Messages-related fields.*/
#if CH_CFG_USE_MESSAGES == TRUE
  ch_queue_init(&tp->msgqueue);
#endif

  /* Statistics-related fields.*/
#if CH_DBG_STATISTICS == TRUE
  chTMObjectInit(&tp->stats);
#endif

  /* Custom thread initialization code.*/
  CH_CFG_THREAD_INIT_HOOK(tp);

  return tp;
}

/**
 * @brief   Disposes a thread.
 * @note    Objects disposing does not involve freeing memory but just
 *          performing checks that make sure that the object is in a
 *          state compatible with operations stop.
 * @note    If the option @p CH_CFG_HARDENING_LEVEL is greater than zero then
 *          the object is also cleared, attempts to use the object would likely
 *          result in a clean memory access violation because dereferencing
 *          of @p NULL pointers rather than dereferencing previously valid
 *          pointers.
 *
 * @param[in] tp        pointer to a @p thread_t object
 *
 * @dispose
 */
void chThdObjectDispose(thread_t *tp) {

  chDbgCheck(tp != NULL);

#if CH_CFG_USE_WAITEXIT == TRUE
  chDbgAssert(ch_queue_isempty(&tp->msgqueue), "wait queue in use");
#endif
#if CH_CFG_USE_REGISTRY == TRUE
  chDbgAssert(tp->refs == (trefs_t)0, "still references");
#endif
#if CH_CFG_USE_MUTEXES == TRUE
  chDbgAssert(tp->mtxlist == NULL, "owning mutexes");
#endif

#if CH_CFG_HARDENING_LEVEL > 0
  memset((void *)tp, 0, sizeof (thread_t));
#endif
}

/**
 * @brief   Creates a non-running thread.
 * @details The created thread is in the @p CH_STATE_WTSTART state and can
 *          be subsequently started.
 * @note    Threads created using this function do not obey to the
 *          @p CH_DBG_FILL_THREADS debug option because it would stay
 *          in a critical section for too long while filling.
 *
 * @param[out] tp       pointer to a @p thread_t object
 * @param[in] tdp       pointer to a @p thread_descriptor_t object
 * @return              Reference to the @p thread_t object.
 *
 * @api
 */
thread_t *chThdCreateSuspended2I(thread_t *tp,
                                 const thread_descriptor_new_t *tdp) {

  chDbgCheck(tp != NULL);
  chDbgCheck(tdp != NULL);

  /* Thread object initialization.*/
  tp = chThdObjectInit(tp, tdp);

  /* Registry-related fields.*/
#if CH_CFG_USE_REGISTRY == TRUE
  REG_INSERT(tp->owner, tp);
#endif

  return tp;
}

/**
 * @brief   Creates a non-running thread.
 * @details The new thread is initialized but not inserted in the ready list,
 *          the initial state is @p CH_STATE_WTSTART.
 * @post    The created thread has a reference counter set to one, it is
 *          caller responsibility to call @p chThdRelease() or @p chthdWait()
 *          in order to release the reference. The thread persists in the
 *          registry until its reference counter reaches zero.
 * @post    The initialized thread can be subsequently started by invoking
 *          @p chThdStart(), @p chThdStartI() or @p chSchWakeupS()
 *          depending on the execution context.
 * @note    A thread can terminate by calling @p chThdExit() or by simply
 *          returning from its main function.
 *
 * @param[out] tp       pointer to a @p thread_t object
 * @param[in] tdp       pointer to a @p thread_descriptor_t object
 * @return              Reference to the @p thread_t object.
 *
 * @api
 */
thread_t *chThdCreateSuspended2(thread_t *tp,
                                const thread_descriptor_new_t *tdp) {

#if CH_CFG_USE_REGISTRY == TRUE
  chDbgAssert(chRegFindThreadByWorkingArea((void *)tdp->wa.base) == NULL,
              "working area in use");
#endif

#if CH_DBG_FILL_THREADS == TRUE
  memset((void *)tdp->wa.base, CH_DBG_STACK_FILL_VALUE, tdp->wa.size);
#endif

  chSysLock();
  tp = chThdCreateSuspended2I(tp, tdp);
  chSysUnlock();

  return tp;
}

/**
 * @brief   Starts a thread.
 * @details Starts a thread previously created with @p chThdCreateSuspended()
 *          or @p chThdCreateSuspendedI().
 *
 * @param[in] tp        pointer to a @p thread_t object
 *
 * @api
 */
void chThdStart2(thread_t *tp) {

  chSysLock();
  chDbgAssert(tp->state == CH_STATE_WTSTART, "wrong state");
  chSchWakeupS(tp, MSG_OK);
  chSysUnlock();
}

/**
 * @brief   Creates a new thread.
 * @details The new thread is initialized and make ready to execute.
 * @post    The created thread has a reference counter set to one, it is
 *          caller responsibility to call @p chThdRelease() or @p chthdWait()
 *          in order to release the reference. The thread persists in the
 *          registry until its reference counter reaches zero.
 * @note    A thread can terminate by calling @p chThdExit() or by simply
 *          returning from its main function.
 * @note    Threads created using this function do not obey to the
 *          @p CH_DBG_FILL_THREADS debug option because it would keep
 *          the kernel locked for too much time.
 *
 * @param[out] tp       pointer to a @p thread_t object
 * @param[in] tdp       pointer to a @p thread_descriptor_t object
 * @return              Reference to the @p thread_t object.
 *
 * @iclass
 */
thread_t *chThdCreate2I(thread_t *tp, const thread_descriptor_new_t *tdp) {

  return chSchReadyI(chThdCreateSuspended2I(tp, tdp));
}

/**
 * @brief   Creates a new thread.
 * @details The new thread is initialized and make ready to execute.
 * @post    The created thread has a reference counter set to one, it is
 *          caller responsibility to call @p chThdRelease() or @p chthdWait()
 *          in order to release the reference. The thread persists in the
 *          registry until its reference counter reaches zero.
 * @note    A thread can terminate by calling @p chThdExit() or by simply
 *          returning from its main function.
 *
 * @param[out] tp       pointer to a @p thread_t object
 * @param[in] tdp       pointer to a @p thread_descriptor_t object
 * @return              Reference to the @p thread_t object.
 *
 * @iclass
 */
thread_t *chThdCreate2(thread_t *tp, const thread_descriptor_new_t *tdp) {

#if (CH_CFG_USE_REGISTRY == TRUE) &&                                        \
    ((CH_DBG_ENABLE_STACK_CHECK == TRUE) || (CH_CFG_USE_DYNAMIC == TRUE))
  chDbgAssert(chRegFindThreadByWorkingArea((void *)tdp->wa.base) == NULL,
              "working area in use");
#endif

#if CH_DBG_FILL_THREADS == TRUE
  memset((void *)tdp->wa.base, CH_DBG_STACK_FILL_VALUE, tdp->wa.size);
#endif

  chSysLock();
  tp = chThdCreateSuspended2I(tp, tdp);
  chSchWakeupS(tp, MSG_OK);
  chSysUnlock();

  return tp;
}

/**
 * @brief   Creates a new thread.
 * @post    The created thread has a reference counter set to one, it is
 *          caller responsibility to call @p chThdRelease() or @p chthdWait()
 *          in order to release the reference. The thread persists in the
 *          registry until its reference counter reaches zero.
 * @note    A thread can terminate by calling @p chThdExit() or by simply
 *          returning from its main function.
 * @warning This function allocates @p sizeof(thread_t) space into the
 *          working area for the thread structure. This space is not accounted
 *          for by the @p THD_WORKING_AREA() macro so it must be explicitly
 *          added to the working area size. This behavior is different from
 *          previous functions where the extra size was implicitly accounted
 *          for. This incompatibility is the cause of making this function
 *          deprecated, it is recommended to use the new thread creation API.
 *
 * @param[in] base      working area base
 * @param[in] size      working area size
 * @param[in] prio      priority level
 * @param[in] fp        thread function pointer
 * @param[in] arg       an argument passed to the thread function. It can be
 *                      @p NULL.
 * @return              The pointer to the @p thread_t structure allocated for
 *                      the thread into the working space area.
 *
 * @api
 * @deprecated
 */
thread_t *chThdCreateStatic2(void *base, size_t size,
                             tprio_t prio, tfunc_t fp, void *arg) {
  thread_t *tp;
  size_t tsize = MEM_ALIGN_NEXT(sizeof (thread_t), PORT_STACK_ALIGN);

  chDbgCheck((base != NULL) &&
             MEM_IS_ALIGNED(base, PORT_WORKING_AREA_ALIGN) &&
             MEM_IS_ALIGNED(size, PORT_STACK_ALIGN) &&
             (size >= THD_WORKING_AREA_SIZE(0) + tsize) &&
             (prio <= HIGHPRIO) && (fp != NULL));

#if (CH_CFG_USE_REGISTRY == TRUE) &&                                        \
    ((CH_DBG_ENABLE_STACK_CHECK == TRUE) || (CH_CFG_USE_DYNAMIC == TRUE))
  chDbgAssert(chRegFindThreadByWorkingArea(base) == NULL,
              "working area in use");
#endif

#if CH_DBG_FILL_THREADS == TRUE
  memset((void *)base, CH_DBG_STACK_FILL_VALUE, size);
#endif

  /* Creating a descriptor from the parameters.*/
  THD_DESC_DECL(tmp_desc, "noname", base, size - tsize, prio, fp, arg, NULL);

  chSysLock();

  /* The thread structure is laid out in the upper part of the thread
     workspace. The thread position structure is aligned to the required
     stack alignment because it represents the stack top.*/
  tp = threadref(((uint8_t *)tmp_desc.wa.base + tmp_desc.wa.size - tsize));

  /* Setting up the port-dependent part of the working area.*/
  PORT_SETUP_CONTEXT(tp, tmp_desc.wa.base, tp, tmp_desc.funcp, tmp_desc.arg);

  tp = chThdObjectInit(tp, &tmp_desc);

  /* Starting the thread immediately.*/
  chSchWakeupS(tp, MSG_OK);

  chSysUnlock();

  return tp;
}

/** @} */
