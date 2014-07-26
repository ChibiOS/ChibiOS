/*
    ChibiOS/RT - Copyright (C) 2006,2007,2008,2009,2010,
                 2011,2012,2013,2014 Giovanni Di Sirio.

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

/*===========================================================================*/
/* Module pre-compile time settings.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

/*===========================================================================*/
/* Module data structures and types.                                         */
/*===========================================================================*/

/**
 * @extends threads_queue_t
 *
 * @brief   Type of a thread structure.
 */
typedef struct thread thread_t;

/**
 * @extends threads_list_t
 *
 * @brief   Type of a generic threads bidirectional linked list header and element.
 */
typedef struct {
  thread_t              *p_next;    /**< @brief Next in the list/queue.     */
  thread_t              *p_prev;    /**< @brief Previous in the queue.      */
} threads_queue_t;

/**
 * @brief   Type of a generic threads single link list, it works like a stack.
 */
typedef struct {
  thread_t              *p_next;    /**< @brief Next in the list/queue.     */
} threads_list_t;

/**
 * @extends threads_queue_t
 *
 * @brief   Ready list header.
 */
typedef struct {
  threads_queue_t       r_queue;    /**< @brief Threads queue.              */
  tprio_t               r_prio;     /**< @brief This field must be
                                                initialized to zero.        */
  struct context        r_ctx;      /**< @brief Not used, present because
                                                offsets.                    */
#if CH_CFG_USE_REGISTRY || defined(__DOXYGEN__)
  thread_t              *r_newer;   /**< @brief Newer registry element.     */
  thread_t              *r_older;   /**< @brief Older registry element.     */
#endif
  /* End of the fields shared with the thread_t structure.*/
  thread_t              *r_current; /**< @brief The currently running
                                                thread.                     */
} ready_list_t;

/**
 * @brief   Structure representing a thread.
 * @note    Not all the listed fields are always needed, by switching off some
 *          not needed ChibiOS/RT subsystems it is possible to save RAM space
 *          by shrinking this structure.
 */
struct thread {
  thread_t              *p_next;    /**< @brief Next in the list/queue.     */
  /* End of the fields shared with the threads_list_t structure.*/
  thread_t              *p_prev;    /**< @brief Previous in the queue.      */
  /* End of the fields shared with the threads_queue_t structure.*/
  tprio_t               p_prio;     /**< @brief Thread priority.            */
  struct context        p_ctx;      /**< @brief Processor context.          */
#if CH_CFG_USE_REGISTRY || defined(__DOXYGEN__)
  thread_t              *p_newer;   /**< @brief Newer registry element.     */
  thread_t              *p_older;   /**< @brief Older registry element.     */
#endif
  /* End of the fields shared with the ReadyList structure. */
#if CH_CFG_USE_REGISTRY || defined(__DOXYGEN__)
  /**
   * @brief Thread name or @p NULL.
   */
  const char            *p_name;
#endif
#if CH_DBG_ENABLE_STACK_CHECK || defined(__DOXYGEN__)
  /**
   * @brief Thread stack boundary.
   */
  stkalign_t            *p_stklimit;
#endif
  /**
   * @brief Current thread state.
   */
  tstate_t              p_state;
  /**
   * @brief Various thread flags.
   */
  tmode_t               p_flags;
#if CH_CFG_USE_DYNAMIC || defined(__DOXYGEN__)
  /**
   * @brief References to this thread.
   */
  trefs_t               p_refs;
#endif
  /**
   * @brief Number of ticks remaining to this thread.
   */
#if (CH_CFG_TIME_QUANTUM > 0) || defined(__DOXYGEN__)
  tslices_t             p_preempt;
#endif
#if CH_DBG_THREADS_PROFILING || defined(__DOXYGEN__)
  /**
   * @brief Thread consumed time in ticks.
   * @note  This field can overflow.
   */
  volatile systime_t    p_time;
#endif
  /**
   * @brief State-specific fields.
   * @note  All the fields declared in this union are only valid in the
   *        specified state or condition and are thus volatile.
   */
  union {
    /**
     * @brief Thread wakeup code.
     * @note  This field contains the low level message sent to the thread
     *        by the waking thread or interrupt handler. The value is valid
     *        after exiting the @p chSchWakeupS() function.
     */
    msg_t               rdymsg;
    /**
     * @brief Thread exit code.
     * @note  The thread termination code is stored in this field in order
     *        to be retrieved by the thread performing a @p chThdWait() on
     *        this thread.
     */
    msg_t               exitcode;
    /**
     * @brief Pointer to a generic "wait" object.
     * @note  This field is used to get a generic pointer to a synchronization
     *        object and is valid when the thread is in one of the wait
     *        states.
     */
    void                *wtobjp;
#if CH_CFG_USE_EVENTS || defined(__DOXYGEN__)
    /**
     * @brief Enabled events mask.
     * @note  This field is only valid while the thread is in the
     *        @p CH_STATE_WTOREVT or @p CH_STATE_WTANDEVT states.
     */
    eventmask_t         ewmask;
#endif
  }                     p_u;
#if CH_CFG_USE_WAITEXIT || defined(__DOXYGEN__)
  /**
   * @brief Termination waiting list.
   */
  threads_list_t        p_waiting;
#endif
#if CH_CFG_USE_MESSAGES || defined(__DOXYGEN__)
  /**
   * @brief Messages queue.
   */
  threads_queue_t       p_msgqueue;
  /**
   * @brief Thread message.
   */
  msg_t                 p_msg;
#endif
#if CH_CFG_USE_EVENTS || defined(__DOXYGEN__)
  /**
   * @brief Pending events mask.
   */
  eventmask_t           p_epending;
#endif
#if CH_CFG_USE_MUTEXES || defined(__DOXYGEN__)
  /**
   * @brief List of the mutexes owned by this thread.
   * @note  The list is terminated by a @p NULL in this field.
   */
  struct mutex          *p_mtxlist;
  /**
   * @brief Thread's own, non-inherited, priority.
   */
  tprio_t               p_realprio;
#endif
#if (CH_CFG_USE_DYNAMIC && CH_CFG_USE_MEMPOOLS) || defined(__DOXYGEN__)
  /**
   * @brief Memory Pool where the thread workspace is returned.
   */
  void                  *p_mpool;
#endif
#if CH_DBG_STATISTICS || defined(__DOXYGEN__)
  time_measurement_t    p_stats;
#endif
#if defined(CH_CFG_THREAD_EXTRA_FIELDS)
  /* Extra fields defined in chconf.h.*/
  CH_CFG_THREAD_EXTRA_FIELDS
#endif
};

/**
 * @brief   Type of a Virtual Timer callback function.
 */
typedef void (*vtfunc_t)(void *);

/**
 * @brief   Type of a Virtual Timer structure.
 */
typedef struct virtual_timer virtual_timer_t;

/**
 * @extends virtual_timers_list_t
 *
 * @brief   Virtual Timer descriptor structure.
 */
struct virtual_timer {
  virtual_timer_t       *vt_next;   /**< @brief Next timer in the list.     */
  virtual_timer_t       *vt_prev;   /**< @brief Previous timer in the list. */
  systime_t             vt_delta;   /**< @brief Time delta before timeout.  */
  vtfunc_t              vt_func;    /**< @brief Timer callback function
                                                pointer.                    */
  void                  *vt_par;    /**< @brief Timer callback function
                                                parameter.                  */
};

/**
 * @brief   Virtual timers list header.
 * @note    The timers list is implemented as a double link bidirectional list
 *          in order to make the unlink time constant, the reset of a virtual
 *          timer is often used in the code.
 */
typedef struct {
  virtual_timer_t       *vt_next;   /**< @brief Next timer in the delta
                                                list.                       */
  virtual_timer_t       *vt_prev;   /**< @brief Last timer in the delta
                                                list.                       */
  systime_t             vt_delta;   /**< @brief Must be initialized to -1.  */
#if CH_CFG_ST_TIMEDELTA == 0 || defined(__DOXYGEN__)
  volatile systime_t    vt_systime; /**< @brief System Time counter.        */
#endif
#if CH_CFG_ST_TIMEDELTA > 0 || defined(__DOXYGEN__)
  /**
   * @brief   System time of the last tick event.
   */
  systime_t             vt_lasttime;/**< @brief System time of the last
                                                tick event.                 */
#endif
} virtual_timers_list_t;

/**
 * @brief   System data structure.
 * @note    This structure contain all the data areas used by the OS except
 *          stacks.
 */
typedef struct ch_system {
  /**
   * @brief   Ready list header.
   */
  ready_list_t          rlist;
  /**
   * @brief   Virtual timers delta list header.
   */
  virtual_timers_list_t vtlist;
#if CH_CFG_USE_TM || defined(__DOXYGEN__)
  /**
   * @brief   Measurement calibration value.
   */
  rtcnt_t               measurement_offset;
#endif
#if CH_DBG_STATISTICS || defined(__DOXYGEN__)
  /**
   * @brief   Global kernel statistics.
   */
  kernel_stats_t        kernel_stats;
#endif
#if CH_DBG_ENABLED || defined(__DOXYGEN__)
  /**
   * @brief   Pointer to the panic message.
   * @details This pointer is meant to be accessed through the debugger, it is
   *          written once and then the system is halted.
   * @note    Accesses to this pointer must never be optimized out so the
   *          field itself is declared volatile.
   */
  const char            * volatile dbg_panic_msg;
#endif
#if CH_DBG_SYSTEM_STATE_CHECK || defined(__DOXYGEN__)
  /**
   * @brief   ISR nesting level.
   */
  cnt_t                 dbg_isr_cnt;
  /**
   * @brief   Lock nesting level.
   */
  cnt_t                 dbg_lock_cnt;
#endif
#if CH_DBG_ENABLE_TRACE || defined(__DOXYGEN__)
  /**
   * @brief   Public trace buffer.
   */
  ch_trace_buffer_t     dbg_trace_buffer;
#endif
} ch_system_t;

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

#if !defined(__DOXYGEN__)
extern ch_system_t ch;
#endif

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
#if !CH_CFG_OPTIMIZE_SPEED
  void queue_prio_insert(thread_t *tp, threads_queue_t *tqp);
  void queue_insert(thread_t *tp, threads_queue_t *tqp);
  thread_t *queue_fifo_remove(threads_queue_t *tqp);
  thread_t *queue_lifo_remove(threads_queue_t *tqp);
  thread_t *queue_dequeue(thread_t *tp);
  void list_insert(thread_t *tp, threads_list_t *tlp);
  thread_t *list_remove(threads_list_t *tlp);
#endif /* CH_CFG_OPTIMIZE_SPEED */
#ifdef __cplusplus
}
#endif

/*===========================================================================*/
/* Module inline functions.                                                  */
/*===========================================================================*/

 /**
  * @brief   Threads list initialization.
  *
  * @notapi
  */
 static inline void list_init(threads_list_t *tlp) {

   tlp->p_next = (thread_t *)tlp;
 }

 /**
  * @brief   Evaluates to @p true if the specified threads list is empty.
  *
  * @notapi
  */
 static inline bool list_isempty(threads_list_t *tlp) {

   return (bool)(tlp->p_next == (thread_t *)tlp);
 }

 /**
  * @brief   Evaluates to @p true if the specified threads list is not empty.
  *
  * @notapi
  */
 static inline bool list_notempty(threads_list_t *tlp) {

   return (bool)(tlp->p_next != (thread_t *)tlp);
 }

 /**
  * @brief   Threads queue initialization.
  *
  * @notapi
  */
 static inline void queue_init(threads_queue_t *tqp) {

   tqp->p_next = tqp->p_prev = (thread_t *)tqp;
 }

 /**
  * @brief   Evaluates to @p true if the specified threads queue is empty.
  *
  * @notapi
  */
 static inline bool queue_isempty(threads_queue_t *tqp) {

   return (bool)(tqp->p_next == (thread_t *)tqp);
 }

 /**
  * @brief   Evaluates to @p true if the specified threads queue is not empty.
  *
  * @notapi
  */
 static inline bool queue_notempty(threads_queue_t *tqp) {

   return (bool)(tqp->p_next != (thread_t *)tqp);
 }

 /* If the performance code path has been chosen then all the following
    functions are inlined into the various kernel modules.*/
 #if CH_CFG_OPTIMIZE_SPEED
 static inline void list_insert(thread_t *tp, threads_list_t *tlp) {

   tp->p_next = tlp->p_next;
   tlp->p_next = tp;
 }

 static inline thread_t *list_remove(threads_list_t *tlp) {

   thread_t *tp = tlp->p_next;
   tlp->p_next = tp->p_next;
   return tp;
 }

 static inline void queue_prio_insert(thread_t *tp, threads_queue_t *tqp) {

   thread_t *cp = (thread_t *)tqp;
   do {
     cp = cp->p_next;
   } while ((cp != (thread_t *)tqp) && (cp->p_prio >= tp->p_prio));
   tp->p_next = cp;
   tp->p_prev = cp->p_prev;
   tp->p_prev->p_next = cp->p_prev = tp;
 }

 static inline void queue_insert(thread_t *tp, threads_queue_t *tqp) {

   tp->p_next = (thread_t *)tqp;
   tp->p_prev = tqp->p_prev;
   tp->p_prev->p_next = tqp->p_prev = tp;
 }

 static inline thread_t *queue_fifo_remove(threads_queue_t *tqp) {
   thread_t *tp = tqp->p_next;

   (tqp->p_next = tp->p_next)->p_prev = (thread_t *)tqp;
   return tp;
 }

 static inline thread_t *queue_lifo_remove(threads_queue_t *tqp) {
   thread_t *tp = tqp->p_prev;

   (tqp->p_prev = tp->p_prev)->p_next = (thread_t *)tqp;
   return tp;
 }

 static inline thread_t *queue_dequeue(thread_t *tp) {

   tp->p_prev->p_next = tp->p_next;
   tp->p_next->p_prev = tp->p_prev;
   return tp;
 }
#endif /* CH_CFG_OPTIMIZE_SPEED */

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
