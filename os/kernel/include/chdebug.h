/*
    ChibiOS/RT - Copyright (C) 2006,2007,2008,2009,2010 Giovanni Di Sirio.

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
 * @file    chdebug.h
 * @brief   Debug macros and structures.
 *
 * @addtogroup debug
 * @{
 */

#ifndef _CHDEBUG_H_
#define _CHDEBUG_H_

/**
 * @brief Trace buffer entries.
 */
#ifndef TRACE_BUFFER_SIZE
#define TRACE_BUFFER_SIZE       64
#endif

/**
 * @brief Fill value for thread stack area in debug mode.
 */
#ifndef STACK_FILL_VALUE
#define STACK_FILL_VALUE        0x55
#endif

/**
 * @brief   Fill value for thread area in debug mode.
 * @note    The chosen default value is 0xFF in order to make evident which
 *          thread fields were not initialized when inspecting the memory with
 *          a debugger. A uninitialized field is not an error in itself but it
 *          better to know it.
 */
#ifndef THREAD_FILL_VALUE
#define THREAD_FILL_VALUE       0xFF
#endif

#if CH_DBG_ENABLE_TRACE || defined(__DOXYGEN__)
/**
 * @brief Trace buffer record.
 */
typedef struct {
  void                  *cse_wtobjp;    /**< @brief Object where going to
                                                    sleep.                  */
  systime_t             cse_time;       /**< @brief Time of the switch
                                                    event.                  */
  uint16_t              cse_state: 4;   /**< @brief Switched out thread
                                                    state.                  */
  uint16_t              cse_tid: 12;    /**< @brief Switched in thread id.  */
} CtxSwcEvent;

/**
 * @brief Trace buffer header.
 */
typedef struct {
  unsigned              tb_size;        /**< @brief Trace buffer size
                                                    (entries).              */
  CtxSwcEvent           *tb_ptr;        /**< @brief Pointer to the ring buffer
                                                    front.                  */
  /** @brief Ring buffer.*/
  CtxSwcEvent           tb_buffer[TRACE_BUFFER_SIZE];
} TraceBuffer;
#endif /* CH_DBG_ENABLE_TRACE */

#define __QUOTE_THIS(p) #p

#if CH_DBG_ENABLE_CHECKS
/**
 * @brief   Function parameter check.
 * @details If the condition check fails then the kernel panics and halts.
 * @note    The condition is tested only if the @p CH_DBG_ENABLE_CHECKS switch
 *          is specified in @p chconf.h else the macro does nothing.
 *
 * @param[in] c         the condition to be verified to be true
 * @param[in] func      the undecorated function name
 */
#define chDbgCheck(c, func) {                                           \
  if (!(c))                                                             \
    chDbgPanic(__QUOTE_THIS(func)"(), line "__QUOTE_THIS(__LINE__));    \
}
#else /* !CH_DBG_ENABLE_CHECKS */
#define chDbgCheck(c, func) {                                           \
  (void)(c), (void)__QUOTE_THIS(func)"(), line "__QUOTE_THIS(__LINE__); \
}
#endif /* !CH_DBG_ENABLE_CHECKS */

#if CH_DBG_ENABLE_ASSERTS
/**
 * @brief   Condition assertion.
 * @details If the condition check fails then the kernel panics with the
 *          specified message and halts.
 * @note    The condition is tested only if the @p CH_DBG_ENABLE_ASSERTS switch
 *          is specified in @p chconf.h else the macro does nothing.
 * @note    The convention for the message is the following:<br>
 *          @<function_name@>(), #@<assert_number@>
 * @note    The remark string is not currently used except for putting a
 *          comment in the code about the assertion.
 *
 * @param[in] c         the condition to be verified to be true
 * @param[in] m         the text message
 * @param[in] r         a remark string
 */
#define chDbgAssert(c, m, r) {                                          \
  if (!(c))                                                             \
    chDbgPanic(m);                                                      \
}
#else /* !CH_DBG_ENABLE_ASSERTS */
#define chDbgAssert(c, m, r) {(void)(c);}
#endif /* !CH_DBG_ENABLE_ASSERTS */

#if !(CH_DBG_ENABLE_ASSERTS ||                                          \
      CH_DBG_ENABLE_CHECKS ||                                           \
      CH_DBG_ENABLE_STACK_CHECK)
/* When the debug features are disabled this function is replaced by an empty
   macro.*/
#define chDbgPanic(msg) {}
#endif

#if !CH_DBG_ENABLE_TRACE
/* When the trace feature is disabled this function is replaced by an empty
   macro.*/
#define chDbgTrace(otp, ntp) {}
#endif

#if !defined(__DOXYGEN__)
#ifdef __cplusplus
extern "C" {
#endif
#if CH_DBG_ENABLE_TRACE
  extern TraceBuffer trace_buffer;
  void trace_init(void);
  void chDbgTrace(Thread *ntp, Thread *otp);
#endif
#if CH_DBG_ENABLE_ASSERTS || CH_DBG_ENABLE_CHECKS || CH_DBG_ENABLE_STACK_CHECK
  extern char *panic_msg;
  void chDbgPanic(char *msg);
#endif
#ifdef __cplusplus
}
#endif
#endif /* !defined(__DOXYGEN__) */

#endif /* _CHDEBUG_H_ */

/** @} */
