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
 * @file    ARMCMx/chcore.c
 * @brief   ARM Cortex-Mx architecture port code.
 *
 * @addtogroup ARMCMx_CORE
 * @{
 */

#include "ch.h"
#include "nvic.h"

/**
 * @brief   PC register temporary storage.
 */
regarm_t _port_saved_pc;

/**
 * @brief   Halts the system.
 * @note    The function is declared as a weak symbol, it is possible
 *          to redefine it in your application code.
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
 * @brief   System Timer vector.
 * @details This interrupt is used as system tick.
 * @note    The timer must be initialized in the startup code.
 */
CH_IRQ_HANDLER(SysTickVector) {

  CH_IRQ_PROLOGUE();

  chSysLockFromIsr();
  chSysTimerHandlerI();
  chSysUnlockFromIsr();

  CH_IRQ_EPILOGUE();
}

/**
 * @brief   Post-IRQ switch code.
 * @details On entry the stack and the registers are restored by the exception
 *          return, the PC value is stored in @p _port_saved_pc, the interrupts
 *          are disabled.
 */
#if !defined(__DOXYGEN__)
__attribute__((naked))
#endif
void _port_switch_from_irq(void) {
  /* Note, saves r4 to make space for the PC.*/
  asm volatile ("push    {r0, r1, r2, r3, r4}                   \n\t"   \
                "mov     r0, r12                                \n\t"   \
                "mrs     r1, XPSR                               \n\t"   \
                "push    {r0, r1, lr}                           \n\t"   \
                "ldr     r0, =_port_saved_pc                    \n\t"   \
                "str     r0, [sp, #28]");

  chSchDoRescheduleI();

  /* Note, the PC is restored alone after re-enabling the interrupts in
     order to minimize the (very remote and unlikely) possibility that
     the stack is filled by continous and saturating interrups that would
     not allow that last word to be pulled out of the stack.*/
  asm volatile ("pop     {r0, r1, r2}                           \n\t"   \
                "mov     r12, r0                                \n\t"   \
                "msr     XPSR, r1                               \n\t"   \
                "mov     lr, r2                                 \n\t"   \
                "pop     {r0, r1, r2, r3}                       \n\t"   \
                "cpsie   i                                      \n\t"   \
                "pop     {pc}");
}

#if CORTEX_MODEL == CORTEX_M0
#define PUSH_CONTEXT(sp) {                                              \
  asm volatile ("push    {r4, r5, r6, r7, lr}                   \n\t"   \
                "mov     r4, r8                                 \n\t"   \
                "mov     r5, r9                                 \n\t"   \
                "mov     r6, r10                                \n\t"   \
                "mov     r7, r11                                \n\t"   \
                "push    {r4, r5, r6, r7}");                            \
}

#define POP_CONTEXT(sp) {                                               \
  asm volatile ("pop     {r4, r5, r6, r7}                       \n\t"   \
                "mov     r8, r4                                 \n\t"   \
                "mov     r9, r5                                 \n\t"   \
                "mov     r10, r6                                \n\t"   \
                "mov     r11, r7                                \n\t"   \
                "pop     {r4, r5, r6, r7, pc}" :  : "r" (sp));          \
}
#else /* CORTEX_MODEL != CORTEX_M0 */
#endif /* CORTEX_MODEL != CORTEX_M0 */

/**
 * @brief   Performs a context switch between two threads.
 * @details This is the most critical code in any port, this function
 *          is responsible for the context switch between 2 threads.
 * @note    The implementation of this code affects <b>directly</b> the context
 *          switch performance so optimize here as much as you can.
 *
 * @param[in] ntp       the thread to be switched in
 * @param[in] otp       the thread to be switched out
 */
#if !defined(__DOXYGEN__)
__attribute__((naked))
#endif
void port_switch(Thread *ntp, Thread *otp) {
  register struct intctx *sp_thd asm ("sp");

  PUSH_CONTEXT(sp_thd);

  otp->p_ctx.r13 = sp_thd;
  sp_thd = ntp->p_ctx.r13;

  POP_CONTEXT(sp_thd);
}

/**
 * @brief   Start a thread by invoking its work function.
 * @details If the work function returns @p chThdExit() is automatically
 *          invoked.
 */
void _port_thread_start(void) {

  asm volatile ("cpsie   i                                      \n\t" \
                "mov     r0, r5                                 \n\t" \
                "blx     r4                                     \n\t" \
                "bl      chThdExit");
}

/** @} */
