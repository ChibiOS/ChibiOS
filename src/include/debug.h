/*
    ChibiOS/RT - Copyright (C) 2009 Giovanni Di Sirio.

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

                                      ---

    A special exception to the GPL can be applied should you wish to distribute
    a combined work that includes ChibiOS/RT, without being obliged to provide
    the source code for any proprietary components. See the file exception.txt
    for full details of how and when the exception can be applied.
*/

/**
 * @addtogroup Debug
 * @{
 */

#ifndef _DEBUG_H_
#define _DEBUG_H_

#ifdef CH_USE_DEBUG

#ifndef TRACE_BUFFER_SIZE
#define TRACE_BUFFER_SIZE 64
#endif

/**
 * Fill value for threads working area in debug mode.
 */
#define MEM_FILL_PATTERN 0x55

typedef struct {
  void                  *cse_wtobjp;
  systime_t             cse_time;
  uint16_t              cse_state: 4;
  uint16_t              cse_tid: 12;
} CtxSwcEvent;

typedef struct {
  size_t                tb_size;
  CtxSwcEvent           *tb_ptr;
  CtxSwcEvent           tb_buffer[TRACE_BUFFER_SIZE];
} TraceBuffer;

extern CtxSwcEvent *dbgnext;
extern TraceBuffer dbgtb;
extern char *dbglastmsg;

#ifdef __cplusplus
extern "C" {
#endif
  void chDbgInit(void);
  void chDbgPanic(char *msg);
#ifdef __cplusplus
}
#endif

/**
 * Condition assertion, if the condition check fails then the kernel panics
 * with the specified message.
 * @param c the condition to be verified to be true
 * @param m the text message
 * @note The condition is tested only if the \p CH_USE_DEBUG switch is
 *       specified in \p chconf.h else the macro does nothing.
 */
#define chDbgAssert(c, m) {                                             \
  if (!(c))                                                             \
    chDbgPanic(m);                                                      \
}

#else /* !CH_USE_DEBUG */

#define chDbgInit()
#define chDbgPanic(msg) {}
#define chDbgAssert(c, m) {(void)(c);}

#endif /* CH_USE_DEBUG */

#ifdef CH_USE_TRACE
#ifdef __cplusplus
extern "C" {
#endif
  void chDbgTrace(Thread *otp, Thread *ntp);
#ifdef __cplusplus
}
#endif
#endif /* CH_USE_TRACE */

#endif /* _DEBUG_H_ */

/** @} */
