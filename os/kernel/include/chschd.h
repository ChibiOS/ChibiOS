/*
    ChibiOS/RT - Copyright (C) 2006,2007,2008,2009,2010,2011 Giovanni Di Sirio.

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
 * @file    chschd.h
 * @brief   Scheduler macros and structures.
 *
 * @addtogroup scheduler
 * @{
 */

#ifndef _CHSCHD_H_
#define _CHSCHD_H_

/** @brief Default thread wakeup low level message.*/
#define RDY_OK          0
/** @brief Low level message sent to a thread awakened by a timeout.*/
#define RDY_TIMEOUT     -1
/** @brief Low level message sent to a thread awakened by a reset operation.*/
#define RDY_RESET       -2

#define NOPRIO          0           /**< @brief Ready list header priority. */
#define IDLEPRIO        1           /**< @brief Idle thread priority.       */
#define LOWPRIO         2           /**< @brief Lowest user priority.       */
#define NORMALPRIO      64          /**< @brief Normal user priority.       */
#define HIGHPRIO        127         /**< @brief Highest user priority.      */
#define ABSPRIO         255         /**< @brief Greatest possible priority. */

/**
 * @brief   Zero time specification for some syscalls with a timeout
 *          specification.
 * @note    Not all functions accept @p TIME_IMMEDIATE as timeout parameter,
 *          see the specific function documentation.
 */
#define TIME_IMMEDIATE  ((systime_t)-1)

/**
 * @brief   Infinite time specification for all the syscalls with a timeout
 *          specification.
 */
#define TIME_INFINITE   ((systime_t)0)

/**
 * @brief   Returns the priority of the first thread on the given ready list.
 *
 * @notapi
 */
#define firstprio(rlp)  ((rlp)->p_next->p_prio)

/**
 * @extends ThreadsQueue
 *
 * @brief   Ready list header.
 */
#if !defined(PORT_OPTIMIZED_READYLIST_STRUCT) || defined(__DOXYGEN__)
typedef struct {
  ThreadsQueue          r_queue;    /**< @brief Threads queue.              */
  tprio_t               r_prio;     /**< @brief This field must be
                                                initialized to zero.        */
  struct context        r_ctx;      /**< @brief Not used, present because
                                                offsets.                    */
#if CH_USE_REGISTRY
  Thread                *r_newer;   /**< @brief Newer registry element.     */
  Thread                *r_older;   /**< @brief Older registry element.     */
#endif
  /* End of the fields shared with the Thread structure.*/
#if CH_TIME_QUANTUM > 0
  cnt_t                 r_preempt;  /**< @brief Round robin counter.        */
#endif
#ifndef CH_CURRP_REGISTER_CACHE
  Thread                *r_current; /**< @brief The currently running
                                                thread.                     */
#endif
} ReadyList;
#endif /* !defined(PORT_OPTIMIZED_READYLIST_STRUCT) */

#if !defined(PORT_OPTIMIZED_RLIST_EXT) && !defined(__DOXYGEN__)
extern ReadyList rlist;
#endif /* !defined(PORT_OPTIMIZED_RLIST_EXT) */

/**
 * @brief   Current thread pointer access macro.
 * @note    This macro is not meant to be used in the application code but
 *          only from within the kernel, use the @p chThdSelf() API instead.
 * @note    It is forbidden to use this macro in order to change the pointer
 *          (currp = something), use @p setcurrp() instead.
 */
#if !defined(PORT_OPTIMIZED_CURRP) || defined(__DOXYGEN__)
#if !defined(CH_CURRP_REGISTER_CACHE) || defined(__DOXYGEN__)
#define currp rlist.r_current
#else /* defined(CH_CURRP_REGISTER_CACHE) */
register Thread *currp asm(CH_CURRP_REGISTER_CACHE);
#endif /* defined(CH_CURRP_REGISTER_CACHE) */
#endif /* !defined(PORT_OPTIMIZED_CURRP) */

/**
 * @brief   Current thread pointer change macro.
 * @note    This macro is not meant to be used in the application code but
 *          only from within the kernel.
 *
 * @notapi
 */
#if !defined(PORT_OPTIMIZED_SETCURRP) || defined(__DOXYGEN__)
#define setcurrp(tp) (currp = (tp))
#endif /* !defined(PORT_OPTIMIZED_SETCURRP) */

/*
 * Scheduler APIs.
 */
#ifdef __cplusplus
extern "C" {
#endif
  void scheduler_init(void);
#if !defined(PORT_OPTIMIZED_READYI)
  Thread *chSchReadyI(Thread *tp);
#endif
#if !defined(PORT_OPTIMIZED_GOSLEEPS)
  void chSchGoSleepS(tstate_t newstate);
#endif
#if !defined(PORT_OPTIMIZED_GOSLEEPTIMEOUTS)
  msg_t chSchGoSleepTimeoutS(tstate_t newstate, systime_t time);
#endif
#if !defined(PORT_OPTIMIZED_WAKEUPS)
  void chSchWakeupS(Thread *tp, msg_t msg);
#endif
#if !defined(PORT_OPTIMIZED_DORESCHEDULEI)
  void chSchDoRescheduleI(void);
#endif
#if !defined(PORT_OPTIMIZED_RESCHEDULES)
  void chSchRescheduleS(void);
#endif
#if !defined(PORT_OPTIMIZED_ISRESCHREQUIREDEXI)
  bool_t chSchIsRescRequiredExI(void);
#endif
#ifdef __cplusplus
}
#endif

/**
 * @brief   Determines if the current thread must reschedule.
 * @details This function returns @p TRUE if there is a ready thread with
 *          higher priority.
 *
 * @iclass
 */
#if !defined(PORT_OPTIMIZED_ISRESCHREQUIREDI) || defined(__DOXYGEN__)
#define chSchIsRescRequiredI() (firstprio(&rlist.r_queue) > currp->p_prio)
#endif /* !defined(PORT_OPTIMIZED_ISRESCHREQUIREDI) */

/**
 * @brief   Determines if yielding is possible.
 * @details This function returns @p TRUE if there is a ready thread with
 *          equal or higher priority.
 *
 * @sclass
 */
#if !defined(PORT_OPTIMIZED_CANYIELDS) || defined(__DOXYGEN__)
#define chSchCanYieldS() (firstprio(&rlist.r_queue) >= currp->p_prio)
#endif /* !defined(PORT_OPTIMIZED_CANYIELDS) */

/**
 * @brief   Yields the time slot.
 * @details Yields the CPU control to the next thread in the ready list with
 *          equal or higher priority, if any.
 *
 * @sclass
 */
#if !defined(PORT_OPTIMIZED_DOYIELDS) || defined(__DOXYGEN__)
#define chSchDoYieldS() {                                                   \
  if (chSchCanYieldS())                                                     \
    chSchDoRescheduleI();                                                   \
}
#endif /* !defined(PORT_OPTIMIZED_DOYIELDS) */

#endif /* _CHSCHD_H_ */

/** @} */
