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
 * @file    chobjects.h
 * @brief   Operating System Objects macros and structures.
 *
 * @addtogroup os_structures
 * @{
 */

#ifndef CHOBJECTS_H
#define CHOBJECTS_H

/*===========================================================================*/
/* Module constants.                                                         */
/*===========================================================================*/

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
 * @brief   Type of a Virtual Timer callback function.
 */
typedef void (*vtfunc_t)(void *p);

/**
 * @brief   Type of a Virtual Timer structure.
 */
typedef struct ch_delta_list delta_list_t;

/**
 * @brief   Virtual Timer delta list element and header structure.
 */
struct ch_delta_list {
  delta_list_t          *next;      /**< @brief Next timer in the list.     */
  delta_list_t          *prev;      /**< @brief Previous timer in the list. */
  sysinterval_t         delta;      /**< @brief Time delta before timeout.  */
};

/**
 * @brief   Type of a Virtual Timer.
 */
typedef struct ch_virtual_timer {
  delta_list_t          dlist;      /**< @brief Delta list element.         */
  vtfunc_t              func;       /**< @brief Timer callback function
                                                pointer.                    */
  void                  *par;       /**< @brief Timer callback function
                                                parameter.                  */
} virtual_timer_t;

/**
 * @brief   Type of virtual timers list header.
 * @note    The timers list is implemented as a double link bidirectional list
 *          in order to make the unlink time constant, the reset of a virtual
 *          timer is often used in the code.
 */
typedef struct ch_virtual_timers_list {
  delta_list_t          dlist;      /**< @brief Delta list header.          */
#if (CH_CFG_ST_TIMEDELTA == 0) || defined(__DOXYGEN__)
  volatile systime_t    systime;    /**< @brief System Time counter.        */
#endif
#if (CH_CFG_ST_TIMEDELTA > 0) || defined(__DOXYGEN__)
  /**
   * @brief   System time of the last tick event.
   */
  systime_t             lasttime;   /**< @brief System time of the last
                                                tick event.                 */
#endif
#if (CH_CFG_USE_TIMESTAMP == TRUE) || defined(__DOXYGEN__)
  volatile uint64_t     laststamp;  /**< @brief Last generated time stamp.  */
#endif
} virtual_timers_list_t;

/**
 * @brief   Type of a thread reference.
 */
typedef thread_t * thread_reference_t;

/**
 * @brief   Type of a threads queue.
 */
typedef struct ch_threads_queue {
  ch_queue_t            queue;      /**< @brief Threads queue header.       */
} threads_queue_t;

/**
 * @brief   Structure representing a thread.
 * @note    Not all the listed fields are always needed, by switching off some
 *          not needed ChibiOS/RT subsystems it is possible to save RAM space
 *          by shrinking this structure.
 */
struct ch_thread {
  union {
    ch_list_t           list;       /**< @brief Threads lists element.      */
    ch_queue_t          queue;      /**< @brief Threads queues element.     */
    ch_priority_queue_t pqueue;     /**< @brief Threads ordered queues
                                         element.                           */
  } hdr;
#if (CH_CFG_USE_REGISTRY == TRUE) || defined(__DOXYGEN__)
  thread_t              *newer;     /**< @brief Newer registry element.     */
  thread_t              *older;     /**< @brief Older registry element.     */
#endif
  /* End of the fields shared with the ReadyList structure. */
  /**
   * @brief   Processor context.
   */
  struct port_context   ctx;
#if (CH_CFG_USE_REGISTRY == TRUE) || defined(__DOXYGEN__)
  /**
   * @brief   Thread name or @p NULL.
   */
  const char            *name;
#endif
#if (CH_DBG_ENABLE_STACK_CHECK == TRUE) || (CH_CFG_USE_DYNAMIC == TRUE) ||  \
    defined(__DOXYGEN__)
  /**
   * @brief   Working area base address.
   * @note    This pointer is used for stack overflow checks and for
   *          dynamic threading.
   */
  stkalign_t            *wabase;
#endif
  /**
   * @brief   Current thread state.
   */
  tstate_t              state;
  /**
   * @brief   Various thread flags.
   */
  tmode_t               flags;
#if (CH_CFG_USE_REGISTRY == TRUE) || defined(__DOXYGEN__)
  /**
   * @brief   References to this thread.
   */
  trefs_t               refs;
#endif
  /**
   * @brief   Number of ticks remaining to this thread.
   */
#if (CH_CFG_TIME_QUANTUM > 0) || defined(__DOXYGEN__)
  tslices_t             ticks;
#endif
#if (CH_DBG_THREADS_PROFILING == TRUE) || defined(__DOXYGEN__)
  /**
   * @brief   Thread consumed time in ticks.
   * @note    This field can overflow.
   */
  volatile systime_t    time;
#endif
  /**
   * @brief   State-specific fields.
   * @note    All the fields declared in this union are only valid in the
   *          specified state or condition and are thus volatile.
   */
  union {
    /**
     * @brief   Thread wakeup code.
     * @note    This field contains the low level message sent to the thread
     *          by the waking thread or interrupt handler. The value is valid
     *          after exiting the @p chSchWakeupS() function.
     */
    msg_t               rdymsg;
    /**
     * @brief   Thread exit code.
     * @note    The thread termination code is stored in this field in order
     *          to be retrieved by the thread performing a @p chThdWait() on
     *          this thread.
     */
    msg_t               exitcode;
    /**
     * @brief   Pointer to a generic "wait" object.
     * @note    This field is used to get a generic pointer to a synchronization
     *          object and is valid when the thread is in one of the wait
     *          states.
     */
    void                *wtobjp;
    /**
     * @brief   Pointer to a generic thread reference object.
     * @note    This field is used to get a pointer to a synchronization
     *          object and is valid when the thread is in @p CH_STATE_SUSPENDED
     *          state.
     */
    thread_reference_t  *wttrp;
#if (CH_CFG_USE_MESSAGES == TRUE) || defined(__DOXYGEN__)
    /**
     * @brief   Thread sent message.
     */
    msg_t               sentmsg;
#endif
#if (CH_CFG_USE_SEMAPHORES == TRUE) || defined(__DOXYGEN__)
    /**
     * @brief   Pointer to a generic semaphore object.
     * @note    This field is used to get a pointer to a synchronization
     *          object and is valid when the thread is in @p CH_STATE_WTSEM
     *          state.
     */
    struct ch_semaphore *wtsemp;
#endif
#if (CH_CFG_USE_MUTEXES == TRUE) || defined(__DOXYGEN__)
    /**
     * @brief   Pointer to a generic mutex object.
     * @note    This field is used to get a pointer to a synchronization
     *          object and is valid when the thread is in @p CH_STATE_WTMTX
     *          state.
     */
    struct ch_mutex     *wtmtxp;
#endif
#if (CH_CFG_USE_EVENTS == TRUE) || defined(__DOXYGEN__)
    /**
     * @brief   Enabled events mask.
     * @note    This field is only valid while the thread is in the
     *          @p CH_STATE_WTOREVT or @p CH_STATE_WTANDEVT states.
     */
    eventmask_t         ewmask;
#endif
  }                     u;
#if (CH_CFG_USE_WAITEXIT == TRUE) || defined(__DOXYGEN__)
  /**
   * @brief   Termination waiting list.
   */
  ch_list_t             waiting;
#endif
#if (CH_CFG_USE_MESSAGES == TRUE) || defined(__DOXYGEN__)
  /**
   * @brief   Messages queue.
   */
  ch_queue_t            msgqueue;
#endif
#if (CH_CFG_USE_EVENTS == TRUE) || defined(__DOXYGEN__)
  /**
   * @brief   Pending events mask.
   */
  eventmask_t           epending;
#endif
#if (CH_CFG_USE_MUTEXES == TRUE) || defined(__DOXYGEN__)
  /**
   * @brief   List of the mutexes owned by this thread.
   * @note    The list is terminated by a @p NULL in this field.
   */
  struct ch_mutex       *mtxlist;
  /**
   * @brief   Thread's own, non-inherited, priority.
   */
  tprio_t               realprio;
#endif
#if ((CH_CFG_USE_DYNAMIC == TRUE) && (CH_CFG_USE_MEMPOOLS == TRUE)) ||      \
    defined(__DOXYGEN__)
  /**
   * @brief   Memory Pool where the thread workspace is returned.
   */
  void                  *mpool;
#endif
#if (CH_DBG_STATISTICS == TRUE) || defined(__DOXYGEN__)
  /**
   * @brief   Thread statistics.
   */
  time_measurement_t    stats;
#endif
#if defined(CH_CFG_THREAD_EXTRA_FIELDS)
  /* Extra fields defined in chconf.h.*/
  CH_CFG_THREAD_EXTRA_FIELDS
#endif
};

/**
 * @extends threads_queue_t
 *
 * @brief   Type of a ready list header.
 */
typedef struct ch_ready_list {
  /**
   * @brief     Threads ordered queues header.
   * @note      The priority field must be initialized to zero.
   */
  ch_priority_queue_t   pqueue;
#if (CH_CFG_USE_REGISTRY == TRUE) || defined(__DOXYGEN__)
  /**
   * @brief     Newer registry element.
   */
  thread_t              *newer;
  /**
   *  @brief    Older registry element.
   */
  thread_t              *older;
#endif
  /* End of the fields shared with the thread_t structure.*/
  /**
   * @brief     The currently running thread.
   */
  thread_t              *current;
} ready_list_t;

/**
 * @brief   Type of an system instance configuration.
 */
typedef struct ch_os_instance_config {
  /**
   * @brief   Instance name.
   */
  const char            *name;
#if (CH_DBG_ENABLE_STACK_CHECK == TRUE) || (CH_CFG_USE_DYNAMIC == TRUE) ||  \
    defined(__DOXYGEN__)
  /**
   * @brief   Lower limit of the main function thread stack.
   */
  stkalign_t            *mainthread_base;
  /**
   * @brief   Upper limit of the main function thread stack.
   */
  stkalign_t            *mainthread_end;
#endif
#if (CH_CFG_NO_IDLE_THREAD == FALSE) || defined(__DOXYGEN__)
  /**
   * @brief   Lower limit of the dedicated idle thread stack.
   */
  stkalign_t            *idlethread_base;
  /**
   * @brief   Upper limit of the dedicated idle thread stack.
   */
  stkalign_t            *idlethread_end;
#endif
} os_instance_config_t;

/**
 * @brief   System instance data structure.
 */
struct ch_os_instance {
  /**
   * @brief   Ready list header.
   */
  ready_list_t          rlist;
  /**
   * @brief   Virtual timers delta list header.
   */
  virtual_timers_list_t vtlist;
  /**
   * @brief   Main thread descriptor.
   */
  thread_t              mainthread;
  /**
   * @brief   System debug.
   */
  system_debug_t        dbg;
#if (CH_DBG_TRACE_MASK != CH_DBG_TRACE_MASK_DISABLED) || defined(__DOXYGEN__)
  /**
   * @brief   Trace buffer.
   */
  trace_buffer_t        trace_buffer;
#endif
#if (CH_CFG_USE_TM == TRUE) || defined(__DOXYGEN__)
  /**
   * @brief   Time measurement calibration data.
   */
  tm_calibration_t      tmc;
#endif
#if (CH_DBG_STATISTICS == TRUE) || defined(__DOXYGEN__)
  /**
   * @brief   Global kernel statistics.
   */
  kernel_stats_t        kernel_stats;
#endif
#if defined(PORT_INSTANCE_EXTRA_FIELDS) || defined(__DOXYGEN__)
  /* Extra fields from port layer.*/
  PORT_INSTANCE_EXTRA_FIELDS
#endif
  /* Extra fields from configuration.*/
  CH_CFG_OS_INSTANCE_EXTRA_FIELDS
};

/*===========================================================================*/
/* Module macros.                                                            */
/*===========================================================================*/

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __cplusplus
}
#endif

/*===========================================================================*/
/* Module inline functions.                                                  */
/*===========================================================================*/

#endif /* CHOBJECTS_H */

/** @} */
