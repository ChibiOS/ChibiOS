/*
    ChibiOS/RT - Copyright (C) 2010 Giovanni Di Sirio.

    This file is part of ChibiOS/RT.

    ChibiOS/RT is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 3 of the License, or
    (at your option) any later version.

    ChibiOS/RT is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program. If not, see <http://www.gnu.org/licenses/>.

                                      ---

    A special exception to the GPL can be applied should you wish to distribute
    a combined work that includes ChibiOS/RT, without being obliged to provide
    the source code for any proprietary components. See the file exception.txt
    for full details of how and when the exception can be applied.
*/

/**
 * @file chdebug.c
 * @brief ChibiOS/RT Debug code.
 * @addtogroup debug
 * @{
 */

#include "ch.h"

#if CH_DBG_ENABLE_TRACE
/**
 * @brief Public trace buffer.
 */
TraceBuffer trace_buffer;

/**
 * @brief Trace circular buffer subsystem initialization.
 */
void trace_init(void) {

  trace_buffer.tb_size = TRACE_BUFFER_SIZE;
  trace_buffer.tb_ptr = &trace_buffer.tb_buffer[0];
}

/**
 * @brief Inserts in the circular debug trace buffer a context switch record.
 *
 * @param[in] otp the thread being switched out
 * @param[in] ntp the thread to be switched in
 */
void chDbgTrace(Thread *otp, Thread *ntp) {

  trace_buffer.tb_ptr->cse_wtobjp = otp->p_wtobjp;
  trace_buffer.tb_ptr->cse_time = chTimeNow();
  trace_buffer.tb_ptr->cse_state = otp->p_state;
  trace_buffer.tb_ptr->cse_tid = (unsigned)ntp >> 4;
  if (++trace_buffer.tb_ptr >= &trace_buffer.tb_buffer[TRACE_BUFFER_SIZE])
    trace_buffer.tb_ptr = &trace_buffer.tb_buffer[0];
}
#endif /* CH_DBG_ENABLE_TRACE */

#if CH_DBG_ENABLE_ASSERTS || CH_DBG_ENABLE_CHECKS || CH_DBG_ENABLE_STACK_CHECK
/**
 * @brief Pointer to the panic message.
 * @details This pointer is meant to be accessed through the debugger, it is
 *          written once and then the system is halted. This variable can be
 *          set to @p NULL if the halt is caused by a stack overflow.
 */
char *panic_msg;

/**
 * @brief Prints a panic message on the console and then halts the system.
 *
 * @param[in] msg the pointer to the panic message string
 */
void chDbgPanic(char *msg) {

  panic_msg = msg;
  chSysHalt();
}
#endif /* CH_DBG_ENABLE_ASSERTS || CH_DBG_ENABLE_CHECKS || CH_DBG_ENABLE_STACK_CHECK */

/** @} */
