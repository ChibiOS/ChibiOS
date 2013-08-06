/*
    ChibiOS/RT - Copyright (C) 2006,2007,2008,2009,2010,
                 2011,2012,2013 Giovanni Di Sirio.

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
 * @file    ARMCMx/GCC/chcoreasm_v7m.s
 * @brief   ARMv7-M architecture port low level code.
 *
 * @addtogroup ARMCMx_CORE
 * @{
 */

#include "chconf.h"
#include "chcore.h"

#if !defined(FALSE) || defined(__DOXYGEN__)
#define FALSE                   0
#endif

#if !defined(TRUE) || defined(__DOXYGEN__)
#define TRUE                    1
#endif

#if !defined(__DOXYGEN__)

                .syntax unified
                .cpu cortex-m4
                .fpu softvfp

                .thumb
                .text

/*
 * Thread trampoline code.
 *
 * Threads execution starts here, the code leaves the system critical zone
 * and then jumps into the thread function passed in register R4. The
 * register R5 contains the thread parameter. The function chThdExit() is
 * called on thread function return.
 */
                .thumb_func
                .globl _port_thread_start
_port_thread_start:
#if CH_DBG_SYSTEM_STATE_CHECK
                bl      dbg_check_unlock
#endif
#if CH_DBG_STATISTICS
                bl      _stats_stop_measure_crit_thd
#endif
                movs    r3, #0
                msr     BASEPRI, r3
                mov     r0, r5
                blx     r4
                bl      chThdExit

#endif /* !defined(__DOXYGEN__) */

/** @} */
