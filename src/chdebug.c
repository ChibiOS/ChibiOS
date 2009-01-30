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

#include <ch.h>

#ifdef CH_USE_DEBUG

char *panicmsg;

/**
 * Debug subsystem initialization.
 */
void chDbgInit(void) {

#ifdef CH_USE_TRACE
  dbgtb.tb_size = TRACE_BUFFER_SIZE;
  dbgtb.tb_ptr = &dbgtb.tb_buffer[0];
#endif
}

/**
 * Prints a panic message on the console/debugger and then halts the system.
 * @param msg the pointer to the message string
 */
void chDbgPanic(char *msg) {

  panicmsg = msg;
  chSysPuts("PANIC: ");
  chSysPuts(msg);
  chSysHalt();
}

#ifdef CH_USE_TRACE
/**
 * Public trace buffer.
 */
TraceBuffer dbgtb;

/**
 * Inserts in the circular debug trace buffer a context switch record.
 * @param otp the thread being switched out
 * @param ntp the thread to be resumed
 */
void chDbgTrace(Thread *otp, Thread *ntp) {

  dbgtb.tb_ptr->cse_wtobjp = otp->p_wtobjp;
  dbgtb.tb_ptr->cse_time = chSysGetTime();
  dbgtb.tb_ptr->cse_state = otp->p_state;
  dbgtb.tb_ptr->cse_tid = ntp->p_tid;
  if (++dbgtb.tb_ptr >= &dbgtb.tb_buffer[TRACE_BUFFER_SIZE])
    dbgtb.tb_ptr = &dbgtb.tb_buffer[0];
}
#endif /* CH_USE_TRACE */

#endif /* CH_USE_DEBUG */
