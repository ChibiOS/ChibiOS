/*
    ChibiOS/RT - Copyright (C) 2006-2007 Giovanni Di Sirio.

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
 * @file debug.h
 * @brief Debug macros and structures.
 * @addtogroup Debug
 * @{
 */

#ifndef _DEBUG_H_
#define _DEBUG_H_

/**
 * @brief Trace buffer entries.
 */
#ifndef TRACE_BUFFER_SIZE
#define TRACE_BUFFER_SIZE 64
#endif

/**
 * @brief Fill value for threads working area in debug mode.
 */
#ifndef MEM_FILL_PATTERN
#define MEM_FILL_PATTERN 0x55
#endif

/**
 * @brief Trace buffer record.
 */
typedef struct {
  void                  *cse_wtobjp;    /**< Object where going to sleep.*/
  systime_t             cse_time;       /**< Time of the switch event.*/
  uint16_t              cse_state: 4;   /**< Switched out thread state.*/
  uint16_t              cse_tid: 12;    /**< Switched in thdread id.*/
} CtxSwcEvent;

/**
 * @brief Trace buffer header.
 */
typedef struct {
  unsigned              tb_size;        /**< Trace buffer size (records).*/
  CtxSwcEvent           *tb_ptr;        /**< Pointer to the ring buffer front.*/
  CtxSwcEvent           tb_buffer[TRACE_BUFFER_SIZE];   /**< Ring buffer.*/
} TraceBuffer;

#if CH_DBG_ENABLE_ASSERTS
/**
 * Condition assertion, if the condition check fails then the kernel panics
 * with the specified message.
 * @param c the condition to be verified to be true
 * @param m the text message
 * @note The condition is tested only if the @p CH_DBG_ENABLE_ASSERTS switch is
 *       specified in @p chconf.h else the macro does nothing.
 */
#define chDbgAssert(c, m) {                                             \
  if (!(c))                                                             \
    chDbgPanic(m);                                                      \
}

#else /* !CH_DBG_ENABLE_ASSERTS */

#define chDbgPanic(msg) {}
#define chDbgAssert(c, m) {(void)(c);}

#endif /* !CH_DBG_ENABLE_ASSERTS */

#ifdef __cplusplus
extern "C" {
#endif
#if CH_DBG_ENABLE_TRACE
  extern TraceBuffer trace_buffer;
  void trace_init(void);
  void chDbgTrace(Thread *otp, Thread *ntp);
#endif
#if CH_DBG_ENABLE_ASSERTS
  extern char *panic_msg;
  void chDbgPanic(char *msg);
#endif
#ifdef __cplusplus
}
#endif

#endif /* _DEBUG_H_ */

/** @} */
