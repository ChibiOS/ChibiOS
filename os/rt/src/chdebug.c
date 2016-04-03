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
 * @file    chdebug.c
 * @brief   ChibiOS/RT Debug code.
 *
 * @addtogroup debug
 * @details Debug APIs and services:
 *          - Runtime system state and call protocol check. The following
 *            panic messages can be generated:
 *            - SV#1, misplaced @p chSysDisable().
 *              - Called from an ISR.
 *              - Called from a critical zone.
 *              .
 *            - SV#2, misplaced @p chSysSuspend()
 *              - Called from an ISR.
 *              - Called from a critical zone.
 *              .
 *            - SV#3, misplaced @p chSysEnable().
 *              - Called from an ISR.
 *              - Called from a critical zone.
 *              .
 *            - SV#4, misplaced @p chSysLock().
 *              - Called from an ISR.
 *              - Called from a critical zone.
 *              .
 *            - SV#5, misplaced @p chSysUnlock().
 *              - Called from an ISR.
 *              - Not called from a critical zone.
 *              .
 *            - SV#6, misplaced @p chSysLockFromISR().
 *              - Not called from an ISR.
 *              - Called from a critical zone.
 *              .
 *            - SV#7, misplaced @p chSysUnlockFromISR().
 *              - Not called from an ISR.
 *              - Not called from a critical zone.
 *              .
 *            - SV#8, misplaced @p CH_IRQ_PROLOGUE().
 *              - Not called at ISR begin.
 *              - Called from a critical zone.
 *              .
 *            - SV#9, misplaced @p CH_IRQ_EPILOGUE().
 *              - @p CH_IRQ_PROLOGUE() missing.
 *              - Not called at ISR end.
 *              - Called from a critical zone.
 *              .
 *            - SV#10, misplaced I-class function.
 *              - I-class function not called from within a critical zone.
 *              .
 *            - SV#11, misplaced S-class function.
 *              - S-class function not called from within a critical zone.
 *              - Called from an ISR.
 *            .
 *          - Trace buffer.
 *          - Parameters check.
 *          - Kernel assertions.
 *          - Kernel panics.
 *          .
 * @note    Stack checks are not implemented in this module but in the port
 *          layer in an architecture-dependent way.
 * @{
 */

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

#if (CH_DBG_TRACE_MASK != CH_DBG_TRACE_MASK_NONE) || defined(__DOXYGEN__)
/**
 * @brief   Writes a time stamp and increases the trace buffer pointer.
 *
 * @notapi
 */
static NOINLINE void trace_next(void) {

  ch.dbg.trace_buffer.ptr->time    = chVTGetSystemTimeX();
#if PORT_SUPPORTS_RT == TRUE
  ch.dbg.trace_buffer.ptr->rtstamp = chSysGetRealtimeCounterX();
#else
  ch.dbg.trace_buffer.ptr->rtstamp = (rtcnt_t)0;
#endif

  /* Trace hook, useful in order to interface debug tools.*/
  CH_CFG_TRACE_HOOK(ch.dbg.trace_buffer.ptr);

  if (++ch.dbg.trace_buffer.ptr >=
      &ch.dbg.trace_buffer.buffer[CH_DBG_TRACE_BUFFER_SIZE]) {
    ch.dbg.trace_buffer.ptr = &ch.dbg.trace_buffer.buffer[0];
  }
}
#endif

/*===========================================================================*/
/* Module exported functions.                                                */
/*===========================================================================*/

#if (CH_DBG_SYSTEM_STATE_CHECK == TRUE) || defined(__DOXYGEN__)
/**
 * @brief   Guard code for @p chSysDisable().
 *
 * @notapi
 */
void _dbg_check_disable(void) {

  if ((ch.dbg.isr_cnt != (cnt_t)0) || (ch.dbg.lock_cnt != (cnt_t)0)) {
    chSysHalt("SV#1");
  }
}

/**
 * @brief   Guard code for @p chSysSuspend().
 *
 * @notapi
 */
void _dbg_check_suspend(void) {

  if ((ch.dbg.isr_cnt != (cnt_t)0) || (ch.dbg.lock_cnt != (cnt_t)0)) {
    chSysHalt("SV#2");
  }
}

/**
 * @brief   Guard code for @p chSysEnable().
 *
 * @notapi
 */
void _dbg_check_enable(void) {

  if ((ch.dbg.isr_cnt != (cnt_t)0) || (ch.dbg.lock_cnt != (cnt_t)0)) {
    chSysHalt("SV#3");
  }
}

/**
 * @brief   Guard code for @p chSysLock().
 *
 * @notapi
 */
void _dbg_check_lock(void) {

  if ((ch.dbg.isr_cnt != (cnt_t)0) || (ch.dbg.lock_cnt != (cnt_t)0)) {
    chSysHalt("SV#4");
  }
  _dbg_enter_lock();
}

/**
 * @brief   Guard code for @p chSysUnlock().
 *
 * @notapi
 */
void _dbg_check_unlock(void) {

  if ((ch.dbg.isr_cnt != (cnt_t)0) || (ch.dbg.lock_cnt <= (cnt_t)0)) {
    chSysHalt("SV#5");
  }
  _dbg_leave_lock();
}

/**
 * @brief   Guard code for @p chSysLockFromIsr().
 *
 * @notapi
 */
void _dbg_check_lock_from_isr(void) {

  if ((ch.dbg.isr_cnt <= (cnt_t)0) || (ch.dbg.lock_cnt != (cnt_t)0)) {
    chSysHalt("SV#6");
  }
  _dbg_enter_lock();
}

/**
 * @brief   Guard code for @p chSysUnlockFromIsr().
 *
 * @notapi
 */
void _dbg_check_unlock_from_isr(void) {

  if ((ch.dbg.isr_cnt <= (cnt_t)0) || (ch.dbg.lock_cnt <= (cnt_t)0)) {
    chSysHalt("SV#7");
  }
  _dbg_leave_lock();
}

/**
 * @brief   Guard code for @p CH_IRQ_PROLOGUE().
 *
 * @notapi
 */
void _dbg_check_enter_isr(void) {

  port_lock_from_isr();
  if ((ch.dbg.isr_cnt < (cnt_t)0) || (ch.dbg.lock_cnt != (cnt_t)0)) {
    chSysHalt("SV#8");
  }
  ch.dbg.isr_cnt++;
  port_unlock_from_isr();
}

/**
 * @brief   Guard code for @p CH_IRQ_EPILOGUE().
 *
 * @notapi
 */
void _dbg_check_leave_isr(void) {

  port_lock_from_isr();
  if ((ch.dbg.isr_cnt <= (cnt_t)0) || (ch.dbg.lock_cnt != (cnt_t)0)) {
    chSysHalt("SV#9");
  }
  ch.dbg.isr_cnt--;
  port_unlock_from_isr();
}

/**
 * @brief   I-class functions context check.
 * @details Verifies that the system is in an appropriate state for invoking
 *          an I-class API function. A panic is generated if the state is
 *          not compatible.
 *
 * @api
 */
void chDbgCheckClassI(void) {

  if ((ch.dbg.isr_cnt < (cnt_t)0) || (ch.dbg.lock_cnt <= (cnt_t)0)) {
    chSysHalt("SV#10");
  }
}

/**
 * @brief   S-class functions context check.
 * @details Verifies that the system is in an appropriate state for invoking
 *          an S-class API function. A panic is generated if the state is
 *          not compatible.
 *
 * @api
 */
void chDbgCheckClassS(void) {

  if ((ch.dbg.isr_cnt != (cnt_t)0) || (ch.dbg.lock_cnt <= (cnt_t)0)) {
    chSysHalt("SV#11");
  }
}

#endif /* CH_DBG_SYSTEM_STATE_CHECK == TRUE */

#if (CH_DBG_TRACE_MASK != CH_DBG_TRACE_MASK_NONE) || defined(__DOXYGEN__)
/**
 * @brief   Trace circular buffer subsystem initialization.
 * @note    Internal use only.
 */
void _dbg_trace_init(void) {
  unsigned i;

  ch.dbg.trace_buffer.suspended = 0U;
  ch.dbg.trace_buffer.size      = CH_DBG_TRACE_BUFFER_SIZE;
  ch.dbg.trace_buffer.ptr       = &ch.dbg.trace_buffer.buffer[0];
  for (i = 0U; i < (unsigned)CH_DBG_TRACE_BUFFER_SIZE; i++) {
    ch.dbg.trace_buffer.buffer[i].type = CH_TRACE_TYPE_UNUSED;
  }
}

#if ((CH_DBG_TRACE_MASK & CH_DBG_TRACE_MASK_SWITCH) != 0U) ||               \
    defined(__DOXYGEN__)
/**
 * @brief   Inserts in the circular debug trace buffer a context switch record.
 *
 * @param[in] otp       the thread being switched out
 *
 * @notapi
 */
void _dbg_trace_switch(thread_t *otp) {

  if ((ch.dbg.trace_buffer.suspended & CH_TRACE_SUSPEND_SWITCH) == 0U) {
    ch.dbg.trace_buffer.ptr->type        = CH_TRACE_TYPE_SWITCH;
    ch.dbg.trace_buffer.ptr->state       = (uint8_t)otp->state;
    ch.dbg.trace_buffer.ptr->u.sw.ntp    = currp;
    ch.dbg.trace_buffer.ptr->u.sw.wtobjp = otp->u.wtobjp;
    trace_next();
  }
}
#endif /* (CH_DBG_TRACE_MASK & CH_DBG_TRACE_MASK_SWITCH) != 0U */

#if ((CH_DBG_TRACE_MASK & CH_DBG_TRACE_MASK_ISR) != 0U) ||                  \
    defined(__DOXYGEN__)
/**
 * @brief   Inserts in the circular debug trace buffer an ISR-enter record.
 *
 * @param[in] isr       name of the isr
 *
 * @notapi
 */
void _dbg_trace_isr_enter(const char *isr) {

  if ((ch.dbg.trace_buffer.suspended & CH_TRACE_SUSPEND_ISR_ENTER) == 0U) {
    port_lock_from_isr();
    ch.dbg.trace_buffer.ptr->type        = CH_TRACE_TYPE_ISR_ENTER;
    ch.dbg.trace_buffer.ptr->state       = 0U;
    ch.dbg.trace_buffer.ptr->u.isr.name  = isr;
    trace_next();
    port_unlock_from_isr();
  }
}

/**
 * @brief   Inserts in the circular debug trace buffer an ISR-leave record.
 *
 * @param[in] isr       name of the isr
 *
 * @notapi
 */
void _dbg_trace_isr_leave(const char *isr) {

  if ((ch.dbg.trace_buffer.suspended & CH_TRACE_SUSPEND_ISR_LEAVE) == 0U) {
    port_lock_from_isr();
    ch.dbg.trace_buffer.ptr->type        = CH_TRACE_TYPE_ISR_LEAVE;
    ch.dbg.trace_buffer.ptr->state       = 0U;
    ch.dbg.trace_buffer.ptr->u.isr.name  = isr;
    trace_next();
    port_unlock_from_isr();
  }
}
#endif /* (CH_DBG_TRACE_MASK & CH_DBG_TRACE_MASK_ISR) != 0U */

#if ((CH_DBG_TRACE_MASK & CH_DBG_TRACE_MASK_HALT) != 0U) ||                 \
    defined(__DOXYGEN__)
/**
 * @brief   Inserts in the circular debug trace buffer an halt record.
 *
 * @param[in] reason    the halt error string
 *
 * @notapi
 */
void _dbg_trace_halt(const char *reason) {

  if ((ch.dbg.trace_buffer.suspended & CH_TRACE_SUSPEND_HALT) == 0U) {
    ch.dbg.trace_buffer.ptr->type          = CH_TRACE_TYPE_HALT;
    ch.dbg.trace_buffer.ptr->state         = 0;
    ch.dbg.trace_buffer.ptr->u.halt.reason = reason;
    trace_next();
  }
}
#endif /* (CH_DBG_TRACE_MASK & CH_DBG_TRACE_MASK_HALT) != 0U */

#if ((CH_DBG_TRACE_MASK & CH_DBG_TRACE_MASK_USER) != 0U) ||                 \
    defined(__DOXYGEN__)
/**
 * @brief   Adds an user trace record to the trace buffer.
 *
 * @param[in] up1       user parameter 1
 * @param[in] up2       user parameter 2
 *
 * @iclass
 */
void chDbgWriteTraceI(void *up1, void *up2) {

  chDbgCheckClassI();

  if ((ch.dbg.trace_buffer.suspended & CH_TRACE_SUSPEND_SWITCH) == 0U) {
    ch.dbg.trace_buffer.ptr->type       = CH_TRACE_TYPE_USER;
    ch.dbg.trace_buffer.ptr->state      = 0;
    ch.dbg.trace_buffer.ptr->u.user.up1 = up1;
    ch.dbg.trace_buffer.ptr->u.user.up2 = up2;
    trace_next();
  }
}

/**
 * @brief   Adds an user trace record to the trace buffer.
 *
 * @param[in] up1       user parameter 1
 * @param[in] up2       user parameter 2
 *
 * @api
 */
void chDbgWriteTrace(void *up1, void *up2) {

  chSysLock();
  chDbgWriteTraceI(up1, up2);
  chSysUnlock();
}
#endif /* (CH_DBG_TRACE_MASK & CH_DBG_TRACE_MASK_USER) != 0U */

/**
 * @brief   Suspends one or more trace events.
 *
 * @paramin mask        mask of the trace events to be suspended
 *
 * @iclass
 */
void chDbgSuspendTraceI(uint16_t mask) {

  chDbgCheckClassI();

  ch.dbg.trace_buffer.suspended |= mask;
}

/**
 * @brief   Suspends one or more trace events.
 *
 * @paramin mask        mask of the trace events to be suspended
 *
 * @api
 */
void chDbgSuspendTrace(uint16_t mask) {

  chSysLock();
  chDbgSuspendTraceI(mask);
  chSysUnlock();
}

/**
 * @brief   Resumes one or more trace events.
 *
 * @paramin mask        mask of the trace events to be resumed
 *
 * @iclass
 */
void chDbgResumeTraceI(uint16_t mask) {

  chDbgCheckClassI();

  ch.dbg.trace_buffer.suspended &= ~mask;
}

/**
 * @brief   Resumes one or more trace events.
 *
 * @paramin mask        mask of the trace events to be resumed
 *
 * @api
 */
void chDbgResumeTrace(uint16_t mask) {

  chSysLock();
  chDbgResumeTraceI(mask);
  chSysUnlock();
}
#endif /* CH_DBG_TRACE_MASK != CH_DBG_TRACE_MASK_NONE */

/** @} */
