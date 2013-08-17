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
 * @file    chschd.h
 * @brief   Scheduler macros and structures.
 *
 * @addtogroup scheduler
 * @{
 */

#ifndef _CHSCHD_H_
#define _CHSCHD_H_

/*===========================================================================*/
/* Module constants.                                                         */
/*===========================================================================*/

/**
 * @name    Wakeup status codes
 * @{
 */
#define MSG_OK          0           /**< @brief Normal wakeup message.      */
#define MSG_TIMEOUT     -1          /**< @brief Wakeup caused by a timeout
                                         condition.                         */
#define MSG_RESET       -2          /**< @brief Wakeup caused by a reset
                                         condition.                         */
/** @} */

/**
 * @name    Priority constants
 * @{
 */
#define NOPRIO          0           /**< @brief Ready list header priority. */
#define IDLEPRIO        1           /**< @brief Idle thread priority.       */
#define LOWPRIO         2           /**< @brief Lowest user priority.       */
#define NORMALPRIO      64          /**< @brief Normal user priority.       */
#define HIGHPRIO        127         /**< @brief Highest user priority.      */
#define ABSPRIO         255         /**< @brief Greatest possible priority. */
/** @} */

/**
 * @name    Special time constants
 * @{
 */
/**
 * @brief   Zero time specification for some functions with a timeout
 *          specification.
 * @note    Not all functions accept @p TIME_IMMEDIATE as timeout parameter,
 *          see the specific function documentation.
 */
#define TIME_IMMEDIATE  ((systime_t)0)

/**
 * @brief   Infinite time specification for all functions with a timeout
 *          specification.
 */
#define TIME_INFINITE   ((systime_t)-1)
/** @} */

/*===========================================================================*/
/* Module pre-compile time settings.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

/*===========================================================================*/
/* Module data structures and types.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Module macros.                                                            */
/*===========================================================================*/

/**
 * @brief   Returns the priority of the first thread on the given ready list.
 *
 * @notapi
 */
#define firstprio(rlp)  ((rlp)->p_next->p_prio)

/**
 * @brief   Current thread pointer access macro.
 * @note    This macro is not meant to be used in the application code but
 *          only from within the kernel, use the @p chThdSelf() API instead.
 * @note    It is forbidden to use this macro in order to change the pointer
 *          (currp = something), use @p setcurrp() instead.
 */
#define currp ch.rlist.r_current

/**
 * @brief   Current thread pointer change macro.
 * @note    This macro is not meant to be used in the application code but
 *          only from within the kernel.
 *
 * @notapi
 */
#define setcurrp(tp) (currp = (tp))

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

/*
 * Scheduler APIs.
 */
#ifdef __cplusplus
extern "C" {
#endif
  void _scheduler_init(void);
  thread_t *chSchReadyI(thread_t *tp);
  void chSchGoSleepS(tstate_t newstate);
  msg_t chSchGoSleepTimeoutS(tstate_t newstate, systime_t time);
  void chSchWakeupS(thread_t *tp, msg_t msg);
  void chSchRescheduleS(void);
  bool chSchIsPreemptionRequired(void);
  void chSchDoRescheduleBehind(void);
  void chSchDoRescheduleAhead(void);
  void chSchDoReschedule(void);
#ifdef __cplusplus
}
#endif

/*===========================================================================*/
/* Module inline functions.                                                  */
/*===========================================================================*/

/**
 * @brief   Determines if the current thread must reschedule.
 * @details This function returns @p true if there is a ready thread with
 *          higher priority.
 *
 * @iclass
 */
static inline bool chSchIsRescRequiredI(void) {

  chDbgCheckClassI();

  return firstprio(&ch.rlist.r_queue) > currp->p_prio;
}

/**
 * @brief   Determines if yielding is possible.
 * @details This function returns @p true if there is a ready thread with
 *          equal or higher priority.
 *
 * @sclass
 */
static inline bool chSchCanYieldS(void) {

  chDbgCheckClassI();

  return firstprio(&ch.rlist.r_queue) >= currp->p_prio;
}

/**
 * @brief   Yields the time slot.
 * @details Yields the CPU control to the next thread in the ready list with
 *          equal or higher priority, if any.
 *
 * @sclass
 */
static inline void chSchDoYieldS(void) {

  chDbgCheckClassS();

  if (chSchCanYieldS())
    chSchDoRescheduleBehind();
}

/**
 * @brief   Inline-able preemption code.
 * @details This is the common preemption code, this function must be invoked
 *          exclusively from the port layer.
 *
 * @special
 */
static inline void chSchPreemption(void) {
  tprio_t p1 = firstprio(&ch.rlist.r_queue);
  tprio_t p2 = currp->p_prio;

#if CH_CFG_TIME_QUANTUM > 0
  if (currp->p_preempt) {
    if (p1 > p2)
      chSchDoRescheduleAhead();
  }
  else {
    if (p1 >= p2)
      chSchDoRescheduleBehind();
  }
#else /* CH_CFG_TIME_QUANTUM == 0 */
  if (p1 >= p2)
    chSchDoRescheduleAhead();
#endif /* CH_CFG_TIME_QUANTUM == 0 */
}

#endif /* _CHSCHD_H_ */

/** @} */
