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
 * @addtogroup Debug
 * @{
 */

#ifndef _DEBUG_H_
#define _DEBUG_H_

#ifdef CH_USE_DEBUG

#define TRACE_BUFFER_SIZE 1024

typedef struct {
  void          *cse_slpdata;
  t_time        cse_time;
  UWORD16       cse_state: 4;
  UWORD16       cse_tid: 12;
} CtxSwcEvent;

typedef struct {
  t_size        tb_size;
  CtxSwcEvent   *tb_ptr;
  CtxSwcEvent   tb_buffer[TRACE_BUFFER_SIZE];
} TraceBuffer;

extern CtxSwcEvent *dbgnext;
extern TraceBuffer dbgtb;
extern char *dbglastmsg;

#ifdef __cplusplus
extern "C" {
#endif
  void chDbgInit(void);
  void chDbgTrace(Thread *otp, Thread *ntp);
  void chDbgPuts(char *msg);
  void chDbgPanic(char *msg);
#ifdef __cplusplus
}
#endif

#else /* CH_USE_DEBUG */

#define chDbgInit()
#define chDbgPuts(msg) {}
#define chDbgPanic(msg) {}

#endif /* CH_USE_DEBUG */

#endif /* _DEBUG_H_ */

/** @} */
