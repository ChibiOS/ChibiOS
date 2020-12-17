/*
    ChibiOS - Copyright (C) 2006..2018 Giovanni Di Sirio.

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
 * @file    rt/include/chschd.h
 * @brief   Scheduler macros and structures.
 *
 * @addtogroup scheduler
 * @{
 */

#ifndef CHSCHD_H
#define CHSCHD_H

/*===========================================================================*/
/* Module constants.                                                         */
/*===========================================================================*/

/**
 * @name    Wakeup status codes
 * @{
 */
#define MSG_OK              (msg_t)0    /**< @brief Normal wakeup message.  */
#define MSG_TIMEOUT         (msg_t)-1   /**< @brief Wakeup caused by a timeout
                                             condition.                     */
#define MSG_RESET           (msg_t)-2   /**< @brief Wakeup caused by a reset
                                             condition.                     */
/** @} */

/**
 * @name    Priority constants
 * @{
 */
#define NOPRIO              (tprio_t)0      /**< @brief Ready list header
                                                 priority.                  */
#define IDLEPRIO            (tprio_t)1      /**< @brief Idle priority.      */
#define LOWPRIO             (tprio_t)2      /**< @brief Lowest priority.    */
#define NORMALPRIO          (tprio_t)128    /**< @brief Normal priority.    */
#define HIGHPRIO            (tprio_t)255    /**< @brief Highest priority.   */
/** @} */

/**
 * @name    Thread states
 * @{
 */
#define CH_STATE_READY      (tstate_t)0      /**< @brief Waiting on the
                                                  ready list.               */
#define CH_STATE_CURRENT    (tstate_t)1      /**< @brief Currently running. */
#define CH_STATE_WTSTART    (tstate_t)2      /**< @brief Just created.      */
#define CH_STATE_SUSPENDED  (tstate_t)3      /**< @brief Suspended state.   */
#define CH_STATE_QUEUED     (tstate_t)4      /**< @brief On a queue.        */
#define CH_STATE_WTSEM      (tstate_t)5      /**< @brief On a semaphore.    */
#define CH_STATE_WTMTX      (tstate_t)6      /**< @brief On a mutex.        */
#define CH_STATE_WTCOND     (tstate_t)7      /**< @brief On a cond.variable.*/
#define CH_STATE_SLEEPING   (tstate_t)8      /**< @brief Sleeping.          */
#define CH_STATE_WTEXIT     (tstate_t)9      /**< @brief Waiting a thread.  */
#define CH_STATE_WTOREVT    (tstate_t)10     /**< @brief One event.         */
#define CH_STATE_WTANDEVT   (tstate_t)11     /**< @brief Several events.    */
#define CH_STATE_SNDMSGQ    (tstate_t)12     /**< @brief Sending a message,
                                                  in queue.                 */
#define CH_STATE_SNDMSG     (tstate_t)13     /**< @brief Sent a message,
                                                  waiting answer.           */
#define CH_STATE_WTMSG      (tstate_t)14     /**< @brief Waiting for a
                                                  message.                  */
#define CH_STATE_FINAL      (tstate_t)15     /**< @brief Thread terminated. */

/**
 * @brief   Thread states as array of strings.
 * @details Each element in an array initialized with this macro can be
 *          indexed using the numeric thread state values.
 */
#define CH_STATE_NAMES                                                     \
  "READY", "CURRENT", "WTSTART", "SUSPENDED", "QUEUED", "WTSEM", "WTMTX",  \
  "WTCOND", "SLEEPING", "WTEXIT", "WTOREVT", "WTANDEVT", "SNDMSGQ",        \
  "SNDMSG", "WTMSG", "FINAL"
/** @} */

/**
 * @name    Thread flags and attributes
 * @{
 */
#define CH_FLAG_MODE_MASK   (tmode_t)3U     /**< @brief Thread memory mode
                                                 mask.                      */
#define CH_FLAG_MODE_STATIC (tmode_t)0U     /**< @brief Static thread.      */
#define CH_FLAG_MODE_HEAP   (tmode_t)1U     /**< @brief Thread allocated
                                                 from a Memory Heap.        */
#define CH_FLAG_MODE_MPOOL  (tmode_t)2U     /**< @brief Thread allocated
                                                 from a Memory Pool.        */
#define CH_FLAG_TERMINATE   (tmode_t)4U     /**< @brief Termination requested
                                                 flag.                      */
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
#define firstprio(rlp)                  (((thread_t *)(rlp)->next)->prio)

/**
 * @brief   Current thread pointer get macro.
 * @note    This macro is not meant to be used in the application code but
 *          only from within the kernel, use @p chThdGetSelfX() instead.
 */
#define __sch_get_currthread(oip)       (oip)->rlist.current

/**
 * @brief   Current thread pointer set macro.
 */
#define __sch_set_currthread(oip, tp)   (oip)->rlist.current = (tp)

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

/*
 * Scheduler APIs.
 */
#ifdef __cplusplus
extern "C" {
#endif
  void chSchObjectInit(os_instance_t *oip,
                       const os_instance_config_t *oicp);
  thread_t *chSchReadyI(thread_t *tp);
  void chSchGoSleepS(tstate_t newstate);
  msg_t chSchGoSleepTimeoutS(tstate_t newstate, sysinterval_t timeout);
  void chSchWakeupS(thread_t *ntp, msg_t msg);
  void chSchRescheduleS(void);
  bool chSchIsPreemptionRequired(void);
  void chSchDoPreemption(void);
  void chSchPreemption(void);
  void chSchDoYieldS(void);
  thread_t *chSchSelectFirstI(void);
#if CH_CFG_OPTIMIZE_SPEED == FALSE
  void queue_prio_insert(thread_t *tp, threads_queue_t *tqp);
  void queue_insert(thread_t *tp, threads_queue_t *tqp);
  thread_t *queue_fifo_remove(threads_queue_t *tqp);
  thread_t *queue_lifo_remove(threads_queue_t *tqp);
  thread_t *queue_dequeue(thread_t *tp);
  void list_insert(thread_t *tp, threads_list_t *tlp);
  thread_t *list_remove(threads_list_t *tlp);
#endif /* CH_CFG_OPTIMIZE_SPEED == FALSE */
#ifdef __cplusplus
}
#endif

/*===========================================================================*/
/* Module inline functions.                                                  */
/*===========================================================================*/

/* If the performance code path has been chosen then all the following
   functions are inlined into the various kernel modules.*/
#if CH_CFG_OPTIMIZE_SPEED == TRUE
static inline void queue_prio_insert(thread_t *tp, ch_queue_t *tqp) {

  thread_t *cp = (thread_t *)tqp;
  do {
    cp = (thread_t *)cp->hdr.queue.next;
  } while ((cp != (thread_t *)tqp) && (cp->prio >= tp->prio));
  tp->hdr.queue.next                 = (ch_queue_t *)cp;
  tp->hdr.queue.prev                 = cp->hdr.queue.prev;
  tp->hdr.queue.prev->next           = (ch_queue_t *)tp;
  cp->hdr.queue.prev                 = (ch_queue_t *)tp;
}
#endif /* CH_CFG_OPTIMIZE_SPEED == TRUE */

#endif /* CHSCHD_H */

/** @} */
