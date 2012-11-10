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
*/

/**
 * @file    MSP430/chcore.c
 * @brief   MSP430 architecture port code.
 *
 * @addtogroup MSP430_CORE
 * @{
 */

#include "ch.h"

/**
 * @brief   Performs a context switch between two threads.
 * @details This is the most critical code in any port, this function
 *          is responsible for the context switch between 2 threads.
 * @note    The implementation of this code affects <b>directly</b> the context
 *          switch performance so optimize here as much as you can.
 * @note    The function is declared as a weak symbol, it is possible to
 *          redefine it in your application code.
 *
 * @param[in] ntp       the thread to be switched in
 * @param[in] otp       the thread to be switched out
 */
#if defined(__IAR_SYSTEMS_ICC__)
#if !defined(__DOXYGEN__)
#pragma no_epilogue
#endif
void port_switch(Thread *ntp, Thread *otp) {
  
  asm          ("pushm.a  #8, r11");
  asm          ("mova     sp, 10(r13)");	/* Save the current stack pointer (otp-Thread). */
  asm          ("mova     10(r12), sp");	/* Restore old pointer (of ntp-Thread). */
  asm          ("popm.a   #8, r11");
}
#else
#if !defined(__DOXYGEN__)
__attribute__((naked, weak))
#endif
void port_switch(Thread *ntp, Thread *otp) {
  register struct intctx *sp asm("r1");

  asm volatile ("pushm.a  #8, r11" : : : "memory");
  otp->p_ctx.sp = sp;
  sp = ntp->p_ctx.sp;
  asm volatile ("popm.a   #8, r11	\n\t" \
                "reta" : : "r" (sp) : "memory");
}
#endif

/**
 * @brief   Halts the system.
 * @details This function is invoked by the operating system when an
 *          unrecoverable error is detected (for example because a programming
 *          error in the application code that triggers an assertion while in
 *          debug mode).
 * @note    The function is declared as a weak symbol, it is possible to
 *          redefine it in your application code.
 */
#if !defined(__DOXYGEN__)
#if !defined(__IAR_SYSTEMS_ICC__)
__attribute__((weak))
#endif
#endif
void port_halt(void) {

  port_disable();
  while (TRUE) {
  }
}

#if defined(__IAR_SYSTEMS_ICC__)
#pragma required=chThdExit  /* http://tech.groups.yahoo.com/group/msp430/message/32860 */
#endif

/**
 * @brief   Start a thread by invoking its work function.
 * @details If the work function returns @p chThdExit() is automatically
 *          invoked.
 */
void _port_thread_start(void) {   

  chSysUnlock();
  #if defined(__IAR_SYSTEMS_ICC__)
  asm("mova      r11, r12");    /* Pass arg to thread (with IAR-Compiler). */  
  #else
  asm("mova      r11, r15");    /* Pass arg to thread (with GCC-Compiler). */  
  #endif
  asm("calla     r10");         /* Start thread. */
  asm("calla     #chThdExit");  /* Perform clean up on thread return. */
}

/** @} */
