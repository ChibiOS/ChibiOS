/*
    Nil RTOS - Copyright (C) 2012 Giovanni Di Sirio.

    This file is part of Nil RTOS.

    Nil RTOS is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 3 of the License, or
    (at your option) any later version.

    Nil RTOS is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

/**
 * @file    nil.h
 * @brief   Nil RTOS main header file.
 * @details This header includes all the required kernel headers so it is the
 *          only header you usually need to include in your application.
 *
 * @addtogroup nil
 * @{
 */

#ifndef _NIL_H_
#define _NIL_H_

#include "nilconf.h"
#include "niltypes.h"

/*===========================================================================*/
/* Module constants.                                                         */
/*===========================================================================*/

/**
 * @name    Nil RTOS identification
 * @{
 */
#define _NIL_                           /**< @brief Nil RTOS identification.*/

#define NIL_KERNEL_VERSION      "0.0.1" /**< @brief Kernel version string.  */

#define NIL_KERNEL_MAJOR        0       /**< @brief Version major number.   */
#define NIL_KERNEL_MINOR        0       /**< @brief Version minor number.   */
#define NIL_KERNEL_PATCH        1       /**< @brief Version patch number.   */
/** @} */

/**
 * @name    Common constants
 */
/**
 * @brief   Generic 'false' boolean constant.
 */
#if !defined(FALSE) || defined(__DOXYGEN__)
#define FALSE                   0
#endif

/**
 * @brief   Generic 'true' boolean constant.
 */
#if !defined(TRUE) || defined(__DOXYGEN__)
#define TRUE                    !FALSE
#endif
/** @} */

/**
 * @name    Wakeup status codes
 * @{
 */
#define NIL_MSG_OK              0   /**< @brief Normal wakeup message.      */
#define NIL_MSG_TMO             -1  /**< @brief Wake-up caused by a timeout
                                         condition.                         */
#define NIL_MSG_RST             -2  /**< @brief Wake-up caused by a reset
                                         condition.                         */
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
#define TIME_IMMEDIATE          ((systime_t)-1)

/**
 * @brief   Infinite time specification for all functions with a timeout
 *          specification.
 */
#define TIME_INFINITE           ((systime_t)0)
/** @} */

/**
 * @name    Thread state related macros
 * @{
 */
#define NIL_THD_READY           0   /**< @brief Thread ready or executing.  */
#define NIL_THD_SLEEPING        1   /**< @brief Thread sleeping.            */
#define NIL_THD_SUSP            2   /**< @brief Thread suspended.           */
#define NIL_THD_WTSEM           3   /**< @brief Thread waiting on semaphore.*/
#define NIL_THD_IS_READY(tr)    ((tr)->state == NIL_THD_READY)
#define NIL_THD_IS_SLEEPING(tr) ((tr)->state == NIL_THD_SLEEPING)
#define NIL_THD_IS_SUSP(tr)     ((tr)->state == NIL_THD_SUSP)
#define NIL_THD_IS_WTSEM(tr)    ((tr)->state == NIL_THD_WTSEM)
/** @} */

/*===========================================================================*/
/* Module pre-compile time settings.                                         */
/*===========================================================================*/

/**
 * @brief   Number of user threads in the application.
 * @note    This number is not inclusive of the idle thread which is
 *          implicitly handled.
 */
#if !defined(NIL_CFG_NUM_THREADS) || defined(__DOXYGEN__)
#define NIL_CFG_NUM_THREADS                 2
#endif

/**
 * @brief   System timer resolution in Hz.
 */
#if !defined(NIL_CFG_FREQUENCY) || defined(__DOXYGEN__)
#define NIL_CFG_FREQUENCY                   100
#endif

/**
 * @brief   Time delta constant for the tick-less mode.
 * @note    If this value is zero then the system uses the classic
 *          periodic tick. This value represents the minimum number
 *          of ticks that is safe to specify in a timeout directive.
 *          The value one is not valid, timeouts are rounded up to
 *          this value.
 */
#if !defined(NIL_CFG_TIMEDELTA) || defined(__DOXYGEN__)
#define NIL_CFG_TIMEDELTA                   0
#endif

/**
 * @brief   System assertions.
 */
#if !defined(NIL_CFG_ENABLE_ASSERTS) || defined(__DOXYGEN__)
#define NIL_CFG_ENABLE_ASSERTS              FALSE
#endif

/**
 * @brief   Threads descriptor structure extension.
 * @details User fields added to the end of the @p thread_t structure.
 */
#if !defined(NIL_CFG_THREAD_EXT_FIELDS) || defined(__DOXYGEN__)
#define NIL_CFG_THREAD_EXT_FIELDS
#endif

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

#if NIL_CFG_NUM_THREADS < 1
#error "at least one thread must be defined"
#endif

#if NIL_CFG_NUM_THREADS > 12
#error "Nil is not recommended for thread-intensive applications, consider" \
       "ChibiOS/RT instead"
#endif

#if NIL_CFG_FREQUENCY <= 0
#error "invalid NIL_CFG_FREQUENCY specified"
#endif

#if (NIL_CFG_TIMEDELTA < 0) || (NIL_CFG_TIMEDELTA == 1)
#error "invalid NIL_CFG_TIMEDELTA specified"
#endif

#if NIL_CFG_ENABLE_ASSERTS
#define NIL_DBG_ENABLED                 TRUE
#else
#define NIL_DBG_ENABLED                 FALSE
#endif

/*===========================================================================*/
/* Module data structures and types.                                         */
/*===========================================================================*/

/**
 * @brief   Type of internal context structure.
 */
typedef struct port_intctx intctx_t;

/**
 * @brief   Type of a structure representing a counting semaphore.
 */
typedef struct {
  volatile cnt_t    cnt;        /**< @brief Semaphore counter.              */
} semaphore_t;

/**
 * @brief Thread function.
 */
typedef void (*tfunc_t)(void *);

/**
 * @brief   Type of a structure representing a thread static configuration.
 */
typedef struct nil_thread_cfg thread_config_t;

/**
 * @brief   Type of a structure representing a thread.
 */
typedef struct nil_thread thread_t;

/**
 * @brief   Structure representing a thread static configuration.
 */
struct nil_thread_cfg {
  const char        *namep;     /**< @brief Thread name, for debugging.     */
  tfunc_t           funcp;      /**< @brief Thread function.                */
  void              *arg;       /**< @brief Thread function argument.       */
  void              *wap;       /**< @brief Thread working area base.       */
  size_t            size;       /**< @brief Thread working area size.       */
};

/**
 * @brief   Type of a thread reference.
 */
typedef thread_t * thread_ref_t;

/**
 * @brief   Structure representing a thread.
 */
struct nil_thread {
  intctx_t              *ctxp;  /**< @brief Pointer to internal context.    */
  tstate_t              state;  /**< @brief Thread state.                   */
  /* Note, the following union contains a pointer while the thread is in a
     sleeping state (!NIL_THD_IS_READY()) else contains the wake-up message.*/
  union {
    msg_t               msg;    /**< @brief Wake-up message.                */
    void                *p;     /**< @brief Generic pointer.                */
    thread_ref_t        *trp;   /**< @brief Pointer to thread reference.    */
    semaphore_t         *semp;  /**< @brief Pointer to semaphore.           */
  } u1;
  volatile systime_t    timeout;/**< @brief Timeout counter, zero
                                            if disabled.                    */
  /* Optional extra fields.*/
  NIL_CFG_THREAD_EXT_FIELDS
};

/**
 * @brief   System data structure.
 * @note    This structure contain all the data areas used by the OS except
 *          stacks.
 */
typedef struct {
  /**
   * @brief   Pointer to the running thread.
   */
  thread_ref_t      current;
  /**
   * @brief   Pointer to the next thread to be executed.
   * @note    This pointer must point at the same thread pointed by @p currp
   *          or to an higher priority thread if a switch is required.
   */
  thread_ref_t      next;
#if NIL_CFG_TIMEDELTA == 0 || defined(__DOXYGEN__)
  /**
   * @brief   System time.
   */
  systime_t         systime;
#endif
#if NIL_CFG_TIMEDELTA > 0 || defined(__DOXYGEN__)
  /**
   * @brief   System time of the last tick event.
   */
  systime_t         lasttime;
  /**
   * @brief   Time of the next scheduled tick event.
   */
  systime_t         nexttime;
#endif
  /**
   * @brief   Thread structures for all the defined threads.
   */
  thread_t          threads[NIL_CFG_NUM_THREADS + 1];
#if NIL_DBG_ENABLED || defined(__DOXYGEN__)
  /**
   * @brief   Panic message.
   * @note    This field is only present if some debug options have been
   *          activated.
   */
  const char        *dbg_msg;
#endif
} nil_system_t;

/*===========================================================================*/
/* Module macros.                                                            */
/*===========================================================================*/

/**
 * @name    Threads tables definition macros
 * @{
 */
/**
 * @brief   Start of user threads table.
 */
#define NIL_THREADS_TABLE_BEGIN()                                           \
  const thread_config_t nil_thd_configs[NIL_CFG_NUM_THREADS + 1] = {

/**
 * @brief   Entry of user threads table
 */
#define NIL_THREADS_TABLE_ENTRY(name, funcp, arg, wap, size)                \
  {name, funcp, arg, wap, size},

/**
 * @brief   End of user threads table.
 */
#define NIL_THREADS_TABLE_END()                                             \
  {"idle", 0, NULL, NULL, 0}                                                \
};

/** @} */

/**
 * @name    Macro Functions
 * @{
 */
/**
 * @brief   System halt state.
 */
#define nilSysHalt() port_halt()

/**
 * @brief   Enters the kernel lock mode.
 *
 * @special
 */
#define nilSysDisable() port_disable()

/**
 * @brief   Enters the kernel lock mode.
 *
 * @special
 */
#define nilSysEnable() port_enable()

/**
 * @brief   Enters the kernel lock mode.
 *
 * @special
 */
#define nilSysLock() port_lock()

/**
 * @brief   Leaves the kernel lock mode.
 *
 * @special
 */
#define nilSysUnlock() port_unlock()

/**
 * @brief   Enters the kernel lock mode from within an interrupt handler.
 * @note    This API may do nothing on some architectures, it is required
 *          because on ports that support preemptable interrupt handlers
 *          it is required to raise the interrupt mask to the same level of
 *          the system mutual exclusion zone.<br>
 *          It is good practice to invoke this API before invoking any I-class
 *          syscall from an interrupt handler.
 * @note    This API must be invoked exclusively from interrupt handlers.
 *
 * @special
 */
#define nilSysLockFromISR() port_lock_from_isr()

/**
 * @brief   Leaves the kernel lock mode from within an interrupt handler.
 *
 * @note    This API may do nothing on some architectures, it is required
 *          because on ports that support preemptable interrupt handlers
 *          it is required to raise the interrupt mask to the same level of
 *          the system mutual exclusion zone.<br>
 *          It is good practice to invoke this API after invoking any I-class
 *          syscall from an interrupt handler.
 * @note    This API must be invoked exclusively from interrupt handlers.
 *
 * @special
 */
#define nilSysUnlockFromISR() port_unlock_from_isr()

/**
 * @brief   Delays the invoking thread for the specified number of seconds.
 * @note    The specified time is rounded up to a value allowed by the real
 *          system clock.
 * @note    The maximum specified value is implementation dependent.
 *
 * @param[in] sec       time in seconds, must be different from zero
 *
 * @api
 */
#define nilThdSleepSeconds(sec) nilThdSleep(S2ST(sec))

/**
 * @brief   Delays the invoking thread for the specified number of
 *          milliseconds.
 * @note    The specified time is rounded up to a value allowed by the real
 *          system clock.
 * @note    The maximum specified value is implementation dependent.
 *
 * @param[in] msec      time in milliseconds, must be different from zero
 *
 * @api
 */
#define nilThdSleepMilliseconds(msec) nilThdSleep(MS2ST(msec))

/**
 * @brief   Delays the invoking thread for the specified number of
 *          microseconds.
 * @note    The specified time is rounded up to a value allowed by the real
 *          system clock.
 * @note    The maximum specified value is implementation dependent.
 *
 * @param[in] usec      time in microseconds, must be different from zero
 *
 * @api
 */
#define nilThdSleepMicroseconds(usec) nilThdSleep(US2ST(usec))

/**
 * @brief   Suspends the invoking thread for the specified time.
 *
 * @param[in] timeout   the delay in system ticks
 *
 * @sclass
 */
#define nilThdSleepS(timeout) nilSchGoSleepTimeoutS(NIL_THD_SLEEPING, timeout)

/**
 * @brief   Suspends the invoking thread until the system time arrives to the
 *          specified value.
 *
 * @param[in] time      absolute system time
 *
 * @sclass
 */
#define nilThdSleepUntilS(time)                                             \
  nilSchGoSleepTimeoutS(NIL_THD_SLEEPING, (time) - nilTimeNow())

/**
 * @brief   Initializes a semaphore with the specified counter value.
 *
 * @param[out] sp       pointer to a @p semaphore_t structure
 * @param[in] n         initial value of the semaphore counter. Must be
 *                      non-negative.
 *
 * @init
 */
#define nilSemInit(sp, n) ((sp)->cnt = n)

/**
 * @brief   Performs a wait operation on a semaphore.
 *
 * @param[in] sp        pointer to a @p semaphore_t structure
 * @return              A message specifying how the invoking thread has been
 *                      released from the semaphore.
 * @retval NIL_MSG_OK   if the thread has not stopped on the semaphore or the
 *                      semaphore has been signaled.
 * @retval NIL_MSG_RST  if the semaphore has been reset using @p nilSemReset().
 *
 * @api
 */
#define nilSemWait(sp) nilSemWaitTimeout(sp, TIME_INFINITE)

/**
 * @brief   Performs a wait operation on a semaphore.
 *
 * @param[in] sp        pointer to a @p semaphore_t structure
 * @return              A message specifying how the invoking thread has been
 *                      released from the semaphore.
 * @retval NIL_MSG_OK   if the thread has not stopped on the semaphore or the
 *                      semaphore has been signaled.
 * @retval NIL_MSG_RST  if the semaphore has been reset using @p nilSemReset().
 *
 * @sclass
 */
#define nilSemWaitS(sp) nilSemWaitTimeoutS(sp, TIME_INFINITE)

/**
 * @brief   Current system time.
 * @details Returns the number of system ticks since the @p nilSysInit()
 *          invocation.
 * @note    The counter can reach its maximum and then restart from zero.
 * @note    This function is designed to work with the @p nilThdSleepUntil().
 *
 * @return              The system time in ticks.
 *
 * @iclass
 */
#if NIL_CFG_TIMEDELTA == 0 || defined(__DOXYGEN__)
#define nilTimeNowI() (nil.systime)
#else
#define nilTimeNowI() port_timer_get_time()
#endif

/**
 * @brief   Checks if the specified time is within the specified time window.
 * @note    When start==end then the function returns always true because the
 *          whole time range is specified.
 *
 * @param[in] time      the time to be verified
 * @param[in] start     the start of the time window (inclusive)
 * @param[in] end       the end of the time window (non inclusive)
 *
 * @retval true         current time within the specified time window.
 * @retval false        current time not within the specified time window.
 *
 * @api
 */
#define nilTimeIsWithin(time, start, end)                                   \
  ((end) > (start) ? ((time) >= (start)) && ((time) < (end)) :              \
                     ((time) >= (start)) || ((time) < (end)))
/** @} */

/**
 * @name    Threads abstraction macros
 */
/**
 * @brief   Thread declaration macro.
 * @note    Thread declarations should be performed using this macro because
 *          the port layer could define optimizations for thread functions.
 */
#define NIL_THREAD(tname, arg) PORT_THREAD(tname, arg)

#if NIL_CFG_ENABLE_ASSERTS || defined(__DOXYGEN__)
/**
 * @brief   Condition assertion.
 * @details If the condition check fails then the kernel panics with the
 *          specified message and halts.
 * @note    The condition is tested only if the @p NIL_CFG_ENABLE_ASSERTS
 *          switch is specified in @p nilconf.h else the macro does nothing.
 * @note    The convention for the message is the following:<br>
 *          @<function_name@>(), #@<assert_number@>
 * @note    The remark string is not currently used except for putting a
 *          comment in the code about the assertion.
 *
 * @param[in] c         the condition to be verified to be true
 * @param[in] m         the text message
 * @param[in] r         a remark string
 *
 * @api
 */
#if !defined(nilDbgAssert)
#define nilDbgAssert(c, m, r) {                                             \
  if (!(c)) {                                                               \
    nil.dbg_msg = (m);                                                      \
    nilSysHalt();                                                           \
  }                                                                         \
}
#endif /* !defined(chDbgAssert) */
#else /* !NIL_CFG_ENABLE_ASSERTS */
#define nilDbgAssert(c, m, r) {(void)(c);}
#endif /* !NIL_CFG_ENABLE_ASSERTS */
/** @} */

/**
 * @name    ISRs abstraction macros
 */
/**
 * @brief   IRQ handler enter code.
 * @note    Usually IRQ handlers functions are also declared naked.
 * @note    On some architectures this macro can be empty.
 *
 * @special
 */
#define NIL_IRQ_PROLOGUE() PORT_IRQ_PROLOGUE()

/**
 * @brief   IRQ handler exit code.
 * @note    Usually IRQ handlers function are also declared naked.
 *
 * @special
 */
#define NIL_IRQ_EPILOGUE() PORT_IRQ_EPILOGUE()

/**
 * @brief   Standard normal IRQ handler declaration.
 * @note    @p id can be a function name or a vector number depending on the
 *          port implementation.
 *
 * @special
 */
#define NIL_IRQ_HANDLER(id) PORT_IRQ_HANDLER(id)
/** @} */

/**
 * @name    Fast ISRs abstraction macros
 */
/**
 * @brief   Standard fast IRQ handler declaration.
 * @note    @p id can be a function name or a vector number depending on the
 *          port implementation.
 * @note    Not all architectures support fast interrupts.
 *
 * @special
 */
#define NIL_FAST_IRQ_HANDLER(id) PORT_FAST_IRQ_HANDLER(id)
/** @} */

/**
 * @name    Time conversion utilities
 * @{
 */
/**
 * @brief   Seconds to system ticks.
 * @details Converts from seconds to system ticks number.
 * @note    The result is rounded upward to the next tick boundary.
 *
 * @param[in] sec       number of seconds
 * @return              The number of ticks.
 *
 * @api
 */
#define S2ST(sec)                                                           \
  ((systime_t)((sec) * NIL_CFG_FREQUENCY))

/**
 * @brief   Milliseconds to system ticks.
 * @details Converts from milliseconds to system ticks number.
 * @note    The result is rounded upward to the next tick boundary.
 *
 * @param[in] msec      number of milliseconds
 * @return              The number of ticks.
 *
 * @api
 */
#define MS2ST(msec)                                                         \
  ((systime_t)(((((uint32_t)(msec)) * ((uint32_t)NIL_CFG_FREQUENCY) - 1UL) /\
                1000UL) + 1UL))

/**
 * @brief   Microseconds to system ticks.
 * @details Converts from microseconds to system ticks number.
 * @note    The result is rounded upward to the next tick boundary.
 *
 * @param[in] usec      number of microseconds
 * @return              The number of ticks.
 *
 * @api
 */
#define US2ST(usec)                                                         \
  ((systime_t)(((((uint32_t)(usec)) * ((uint32_t)NIL_CFG_FREQUENCY) - 1UL) /\
                1000000UL) + 1UL))
/** @} */

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#if !defined(__DOXYGEN__)
extern nil_system_t nil;
extern const thread_config_t nil_thd_configs[NIL_CFG_NUM_THREADS + 1];
#endif

#ifdef __cplusplus
extern "C" {
#endif
  void nilSysInit(void);
  void nilSysTimerHandlerI(void);
  thread_ref_t nilSchReadyI(thread_ref_t trp, msg_t msg);
  msg_t nilSchGoSleepTimeoutS(tstate_t newstate, systime_t timeout);
  void nilSchRescheduleS(void);
  msg_t nilThdSuspendTimeoutS(thread_ref_t *trp, systime_t timeout);
  void nilThdResumeI(thread_ref_t *trp, msg_t msg);
  void nilThdSleep(systime_t time);
  void nilThdSleepUntil(systime_t time);
  systime_t nilTimeNow(void);
  bool nilTimeNowIsWithin(systime_t start, systime_t end);
  msg_t nilSemWaitTimeout(semaphore_t *sp, systime_t time);
  msg_t nilSemWaitTimeoutS(semaphore_t *sp, systime_t time);
  void nilSemSignal(semaphore_t *sp);
  void nilSemSignalI(semaphore_t *sp);
  void nilSemReset(semaphore_t *sp, cnt_t n);
  void nilSemResetI(semaphore_t *sp, cnt_t n);
#ifdef __cplusplus
}
#endif

/* Late inclusion, this is done in order to let the port layer access
   the OS services like assertions.*/
#include "nilcore.h"

#endif /* _NIL_H_ */

/** @} */
