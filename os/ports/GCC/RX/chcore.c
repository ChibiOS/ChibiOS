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

                                      ---

    A special exception to the GPL can be applied should you wish to distribute
    a combined work that includes ChibiOS/RT, without being obliged to provide
    the source code for any proprietary components. See the file exception.txt
    for full details of how and when the exception can be applied.
*/

/**
 * @file    GCC/RX/chcore.c
 * @brief   Renesas RX architecture port code.
 *
 * @addtogroup RX_CORE
 * @{
 */

#include "ch.h"

/**
 * @brief RX initialization code.
 *
 * @note This function is usually empty.
 */
void port_init(void){
}

/**
 * @brief Disables all the interrupt sources.
 *
 * @note Of course non maskable interrupt sources are not included.
 */
void port_disable() {

  asm volatile ("clrpsw  I                                          \n\t");
}

/**
 * @brief Disables the interrupt sources that are not supposed to preempt the kernel.
 */
void port_suspend(void) {

  asm volatile ("clrpsw  I                                          \n\t");
}

/**
 * @brief Enables all the interrupt sources.
 */
void port_enable(void) {

  asm volatile ("setpsw  I                                          \n\t");
}

/**
 * @brief Check if interrupts are enabled.
 */
bool_t port_enabled(void) {
  uint32_t psw;

  asm volatile("mvfc     PSW,%0                                     \n\t"
    : "=r" (psw) : : );
  return psw & (1<<16) ? true : false;
}

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
__attribute__((weak))
#endif
void port_halt(void) {

  port_disable();
  while (TRUE) {
  }
}

/**
 * @brief Performs a context switch between two threads.
 * @details This is the most critical code in any port, this function
 * is responsible for the context switch between 2 threads.
 *
 * @param ntp the thread to be switched in
 * @param otp the thread to be switched out
 * @note The implementation of this code affects <b>directly</b> the context
 *       switch performance so optimize here as much as you can.
 */
#if !defined(__DOXYGEN__)
__attribute__((naked, weak))
#endif
void port_switch(Thread *ntp, Thread *otp) {
/*
  register struct intctx *sp asm ("sp");
  asm volatile ("pushm   r6-r13                                     \n\t");
  otp->p_ctx.sp = sp;
  sp = ntp->p_ctx.sp;
  asm volatile ("popm    r6-r13                                     \n\t");
*/
#if !defined(CH_CURRP_REGISTER_CACHE)
  asm volatile ("pushm   r6-r13                                     \n\t"
                "mov.l   r0,12[r2]                                  \n\t"
                "mov.l   12[r1],r0                                  \n\t"
                "popm    r6-r13                                     \n\t");
#else
  asm volatile ("pushm   r6-r12                                     \n\t"
                "mov     r0,12[r2]                                  \n\t"
                "mov     12[r1],r0                                  \n\t"
                "popm    r6-r12                                     \n\t");
#endif
  asm volatile ("rts                                                \n\t");
}

/**
 * @brief   Start a thread by invoking its work function.
 * @details If the work function returns @p chThdExit() is automatically
 *          invoked.
 */
void _port_thread_start(void) {

  chSysUnlock();
  asm volatile ("mov.l   r11,r1                                     \n\t"
                "jsr     r12                                        \n\t"
                "bsr     _chThdExit                                 \n\t");
}

/** @} */
