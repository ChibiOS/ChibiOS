/*
    ChibiOS - Copyright (C) 2006..2016 Giovanni Di Sirio.

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
 * @file    chdebug.h
 * @brief   Debug macros and structures.
 *
 * @addtogroup debug
 * @{
 */

#ifndef _CHDEBUG_H_
#define _CHDEBUG_H_

/*===========================================================================*/
/* Module constants.                                                         */
/*===========================================================================*/

/**
 * @name    Trace record types
 * @{
 */
#define CH_TRACE_TYPE_UNUSED                0U
#define CH_TRACE_TYPE_SWITCH                1U
#define CH_TRACE_TYPE_ISR_ENTER             2U
#define CH_TRACE_TYPE_ISR_LEAVE             3U
#define CH_TRACE_TYPE_HALT                  4U
#define CH_TRACE_TYPE_USER                  5U
/** @} */

/**
 * @name    Trace suspend masks
 * @{
 */
#define CH_TRACE_SUSPEND_NONE               0U
#define CH_TRACE_SUSPEND_SWITCH             (1U << CH_TRACE_TYPE_SWITCH)
#define CH_TRACE_SUSPEND_ISR_ENTER          (1U << CH_TRACE_TYPE_ISR_ENTER)
#define CH_TRACE_SUSPEND_ISR_LEAVE          (1U << CH_TRACE_TYPE_ISR_LEAVE)
#define CH_TRACE_SUSPEND_HALT               (1U << CH_TRACE_TYPE_HALT)
#define CH_TRACE_SUSPEND_USER               (1U << CH_TRACE_TYPE_USER)
#define CH_TRACE_SUSPEND_ALL                (CH_TRACE_SUSPEND_SWITCH |      \
                                             CH_TRACE_SUSPEND_ISR_ENTER |   \
                                             CH_TRACE_SUSPEND_ISR_LEAVE |   \
                                             CH_TRACE_SUSPEND_HALT |        \
                                             CH_TRACE_SUSPEND_USER)

/** @} */

/**
 * @name    Events to trace
 * @{
 */
#define CH_DBG_TRACE_MASK_NONE              0U
#define CH_DBG_TRACE_MASK_SWITCH            1U
#define CH_DBG_TRACE_MASK_ISR               2U
#define CH_DBG_TRACE_MASK_HALT              4U
#define CH_DBG_TRACE_MASK_USER              8U
#define CH_DBG_TRACE_MASK_ALL               (CH_DBG_TRACE_MASK_SWITCH |     \
                                             CH_DBG_TRACE_MASK_ISR |        \
                                             CH_DBG_TRACE_MASK_HALT |       \
                                             CH_DBG_TRACE_MASK_USER)
/** @} */

/*===========================================================================*/
/* Module pre-compile time settings.                                         */
/*===========================================================================*/

/**
 * @name    Debug related settings
 * @{
 */
/**
 * @brief   Trace buffer entries.
 */
#if !defined(CH_DBG_TRACE_MASK) || defined(__DOXYGEN__)
#define CH_DBG_TRACE_MASK                   CH_DBG_TRACE_MASK_ALL
#endif

/**
 * @brief   Trace buffer entries.
 * @note    The trace buffer is only allocated if @p CH_DBG_TRACE_MASK is
 *          different from @p CH_DBG_TRACE_MASK_NONE.
 */
#if !defined(CH_DBG_TRACE_BUFFER_SIZE) || defined(__DOXYGEN__)
#define CH_DBG_TRACE_BUFFER_SIZE            128
#endif

/**
 * @brief   Fill value for thread stack area in debug mode.
 */
#if !defined(CH_DBG_STACK_FILL_VALUE) || defined(__DOXYGEN__)
#define CH_DBG_STACK_FILL_VALUE             0x55
#endif
/** @} */

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

#if !defined(CH_CFG_TRACE_HOOK)
#error "CH_CFG_TRACE_HOOK not defined in chconf.h"
#endif

/*===========================================================================*/
/* Module data structures and types.                                         */
/*===========================================================================*/

#if (CH_DBG_TRACE_MASK != CH_DBG_TRACE_MASK_NONE) || defined(__DOXYGEN__)
/**
 * @brief   Trace buffer record.
 */
typedef struct {
  /**
   * @brief   Record type.
   */
  uint32_t              type:3;
  /**
   * @brief   Switched out thread state.
   */
  uint32_t              state:5;
  /**
   * @brief   Accurate time stamp.
   * @note    This field only available if the post supports
   *          @p PORT_SUPPORTS_RT else it is set to zero.
   */
  uint32_t              rtstamp:24;
  /**
   * @brief   System time stamp of the switch event.
   */
  systime_t             time;
  union {
    /**
     * @brief   Structure representing a  context switch.
     */
    struct {
      /**
       * @brief   Switched in thread.
       */
      thread_t              *ntp;
      /**
       * @brief   Object where going to sleep.
       */
      void                  *wtobjp;
    } sw;
    /**
     * @brief   Structure representing an ISR enter.
     */
    struct {
      /**
       * @brief   ISR function name taken using @p __func__.
       */
      const char            *name;
    } isr;
    /**
     * @brief   Structure representing an halt.
     */
    struct {
      /**
       * @brief   Halt error string.
       */
      const char            *reason;
    } halt;
    /**
     * @brief   User trace structure.
     */
    struct {
      /**
       * @brief   Trace user parameter 1.
       */
      void                  *up1;
      /**
       * @brief   Trace user parameter 2.
       */
      void                  *up2;
    } user;
  } u;
} ch_trace_event_t;

/**
 * @brief   Trace buffer header.
 */
typedef struct {
  /**
   * @brief   Suspended trace sources mask.
   */
  uint16_t              suspended;
  /**
   * @brief   Trace buffer size (entries).
   */
  uint16_t              size;
  /**
   * @brief   Pointer to the buffer front.
   */
  ch_trace_event_t      *ptr;
  /**
   * @brief   Ring buffer.
   */
  ch_trace_event_t      buffer[CH_DBG_TRACE_BUFFER_SIZE];
} ch_trace_buffer_t;
#endif /* CH_DBG_TRACE_MASK != CH_DBG_TRACE_MASK_NONE */

/*===========================================================================*/
/* Module macros.                                                            */
/*===========================================================================*/

#if CH_DBG_SYSTEM_STATE_CHECK == TRUE
#define _dbg_enter_lock() (ch.dbg.lock_cnt = (cnt_t)1)
#define _dbg_leave_lock() (ch.dbg.lock_cnt = (cnt_t)0)
#endif

/* When the state checker feature is disabled then the following functions
   are replaced by an empty macro.*/
#if CH_DBG_SYSTEM_STATE_CHECK == FALSE
#define _dbg_enter_lock()
#define _dbg_leave_lock()
#define _dbg_check_disable()
#define _dbg_check_suspend()
#define _dbg_check_enable()
#define _dbg_check_lock()
#define _dbg_check_unlock()
#define _dbg_check_lock_from_isr()
#define _dbg_check_unlock_from_isr()
#define _dbg_check_enter_isr()
#define _dbg_check_leave_isr()
#define chDbgCheckClassI()
#define chDbgCheckClassS()
#endif

/* When a trace feature is disabled the associated functions are replaced by
   an empty macro.*/
#if (CH_DBG_TRACE_MASK & CH_DBG_TRACE_MASK_SWITCH) == 0
#define _dbg_trace_switch(otp)
#endif
#if (CH_DBG_TRACE_MASK & CH_DBG_TRACE_MASK_ISR) == 0
#define _dbg_trace_isr_enter(isr)
#define _dbg_trace_isr_leave(isr)
#endif
#if (CH_DBG_TRACE_MASK & CH_DBG_TRACE_MASK_HALT) == 0
#define _dbg_trace_halt(reason)
#endif
#if (CH_DBG_TRACE_MASK & CH_DBG_TRACE_MASK_USER) == 0
#define chDbgWriteTraceI(up1, up2)
#define chDbgWriteTrace(up1, up2)
#endif

/**
 * @name    Macro Functions
 * @{
 */
/**
 * @brief   Function parameters check.
 * @details If the condition check fails then the kernel panics and halts.
 * @note    The condition is tested only if the @p CH_DBG_ENABLE_CHECKS switch
 *          is specified in @p chconf.h else the macro does nothing.
 *
 * @param[in] c         the condition to be verified to be true
 *
 * @api
 */
#if !defined(chDbgCheck)
#define chDbgCheck(c) do {                                                  \
  /*lint -save -e506 -e774 [2.1, 14.3] Can be a constant by design.*/       \
  if (CH_DBG_ENABLE_CHECKS != FALSE) {                                      \
    if (!(c)) {                                                             \
  /*lint -restore*/                                                         \
      chSysHalt(__func__);                                                  \
    }                                                                       \
  }                                                                         \
} while (false)
#endif /* !defined(chDbgCheck) */

/**
 * @brief   Condition assertion.
 * @details If the condition check fails then the kernel panics with a
 *          message and halts.
 * @note    The condition is tested only if the @p CH_DBG_ENABLE_ASSERTS switch
 *          is specified in @p chconf.h else the macro does nothing.
 * @note    The remark string is not currently used except for putting a
 *          comment in the code about the assertion.
 *
 * @param[in] c         the condition to be verified to be true
 * @param[in] r         a remark string
 *
 * @api
 */
#if !defined(chDbgAssert)
#define chDbgAssert(c, r) do {                                              \
  /*lint -save -e506 -e774 [2.1, 14.3] Can be a constant by design.*/       \
  if (CH_DBG_ENABLE_ASSERTS != FALSE) {                                     \
    if (!(c)) {                                                             \
  /*lint -restore*/                                                         \
      chSysHalt(__func__);                                                  \
    }                                                                       \
  }                                                                         \
} while (false)
#endif /* !defined(chDbgAssert) */
/** @} */

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#ifdef __cplusplus
extern "C" {
#endif
#if CH_DBG_SYSTEM_STATE_CHECK == TRUE
  void _dbg_check_disable(void);
  void _dbg_check_suspend(void);
  void _dbg_check_enable(void);
  void _dbg_check_lock(void);
  void _dbg_check_unlock(void);
  void _dbg_check_lock_from_isr(void);
  void _dbg_check_unlock_from_isr(void);
  void _dbg_check_enter_isr(void);
  void _dbg_check_leave_isr(void);
  void chDbgCheckClassI(void);
  void chDbgCheckClassS(void);
#endif
#if (CH_DBG_TRACE_MASK != CH_DBG_TRACE_MASK_NONE) || defined(__DOXYGEN__)
  void _dbg_trace_init(void);
#if (CH_DBG_TRACE_MASK & CH_DBG_TRACE_MASK_SWITCH) != 0
  void _dbg_trace_switch(thread_t *otp);
#endif
#if (CH_DBG_TRACE_MASK & CH_DBG_TRACE_MASK_ISR) != 0
  void _dbg_trace_isr_enter(const char *isr);
  void _dbg_trace_isr_leave(const char *isr);
#endif
#if (CH_DBG_TRACE_MASK & CH_DBG_TRACE_MASK_HALT) != 0
  void _dbg_trace_halt(const char *reason);
#endif
#if (CH_DBG_TRACE_MASK & CH_DBG_TRACE_MASK_USER) != 0
  void chDbgWriteTraceI(void *up1, void *up2);
  void chDbgWriteTrace(void *up1, void *up2);
  void chDbgSuspendTraceI(uint16_t mask);
  void chDbgSuspendTrace(uint16_t mask);
  void chDbgResumeTraceI(uint16_t mask);
  void chDbgResumeTrace(uint16_t mask);
#endif
#endif /* CH_DBG_TRACE_MASK != CH_DBG_TRACE_MASK_NONE */
#ifdef __cplusplus
}
#endif

/*===========================================================================*/
/* Module inline functions.                                                  */
/*===========================================================================*/

#endif /* _CHDEBUG_H_ */

/** @} */
