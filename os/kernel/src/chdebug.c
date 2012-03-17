/*
    ChibiOS/RT - Copyright (C) 2006,2007,2008,2009,2010,
                 2011,2012 Giovanni Di Sirio.

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
 * @file    chdebug.c
 * @brief   ChibiOS/RT Debug code.
 *
 * @addtogroup debug
 * @details Debug APIs and services:
 *          - Trace buffer.
 *          - Parameters check.
 *          - Kernel assertions.
 *          .
 * @pre     In order to use the debug APIs the @p CH_DBG_ENABLE_TRACE,
 *          @p CH_DBG_ENABLE_ASSERTS, @p CH_DBG_ENABLE_CHECKS options must
 *          be enabled in @p chconf.h.
 * @{
 */

#include "ch.h"

#if CH_DBG_ENABLE_TRACE || defined(__DOXYGEN__)
/**
 * @brief   Public trace buffer.
 */
ch_trace_buffer_t dbg_trace_buffer;

/**
 * @brief   Trace circular buffer subsystem initialization.
 * @note    Internal use only.
 */
void trace_init(void) {

  dbg_trace_buffer.tb_size = CH_TRACE_BUFFER_SIZE;
  dbg_trace_buffer.tb_ptr = &dbg_trace_buffer.tb_buffer[0];
}

/**
 * @brief   Inserts in the circular debug trace buffer a context switch record.
 *
 * @param[in] otp       the thread being switched out
 *
 * @notapi
 */
void dbg_trace(Thread *otp) {

  dbg_trace_buffer.tb_ptr->se_time   = chTimeNow();
  dbg_trace_buffer.tb_ptr->se_tp     = currp;
  dbg_trace_buffer.tb_ptr->se_wtobjp = otp->p_u.wtobjp;
  dbg_trace_buffer.tb_ptr->se_state  = (uint8_t)otp->p_state;
  if (++dbg_trace_buffer.tb_ptr >=
      &dbg_trace_buffer.tb_buffer[CH_TRACE_BUFFER_SIZE])
    dbg_trace_buffer.tb_ptr = &dbg_trace_buffer.tb_buffer[0];
}
#endif /* CH_DBG_ENABLE_TRACE */

#if CH_DBG_ENABLE_ASSERTS || CH_DBG_ENABLE_CHECKS ||                        \
    CH_DBG_ENABLE_STACK_CHECK || defined(__DOXYGEN__)
/**
 * @brief   Pointer to the panic message.
 * @details This pointer is meant to be accessed through the debugger, it is
 *          written once and then the system is halted.
 */
char *dbg_panic_msg;

/**
 * @brief   Prints a panic message on the console and then halts the system.
 *
 * @param[in] msg       the pointer to the panic message string
 */
void chDbgPanic(char *msg) {

  dbg_panic_msg = msg;
  chSysHalt();
}
#endif /* CH_DBG_ENABLE_ASSERTS || CH_DBG_ENABLE_CHECKS || CH_DBG_ENABLE_STACK_CHECK */

/** @} */
