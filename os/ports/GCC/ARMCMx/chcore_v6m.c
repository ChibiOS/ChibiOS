/*
    ChibiOS/RT - Copyright (C) 2006,2007,2008,2009,2010,2011 Giovanni Di Sirio.

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
 * @file    GCC/ARMCMx/chcore_v6m.c
 * @brief   ARMv6-M architecture port code.
 *
 * @addtogroup ARMCMx_V6M_CORE
 * @{
 */

#include "ch.h"

/**
 * @brief   PC register temporary storage.
 */
regarm_t _port_saved_pc;

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
void _port_switch_from_isr(void) {
  /* Note, saves r4 to make space for the PC.*/
  asm volatile ("push    {r0, r1, r2, r3, r4}                   \n\t"
                "mrs     r0, APSR                               \n\t"
                "mov     r1, r12                                \n\t"
                "push    {r0, r1, lr}                           \n\t"
                "ldr     r0, =_port_saved_pc                    \n\t"
                "ldr     r0, [r0]                               \n\t"
                "add     r0, r0, #1                             \n\t"
                "str     r0, [sp, #28]" : : : "memory");

  chSchDoRescheduleI();

  /* Note, the last register is restored alone after re-enabling the
     interrupts in order to minimize the (very remote and unlikely)
     possibility that the stack is filled by continuous and saturating
     interrupts that would not allow that last words to be pulled out of
     the stack.*/
  asm volatile ("pop     {r0, r1, r2}                           \n\t"
                "mov     r12, r1                                \n\t"
                "msr     APSR, r0                               \n\t"
                "mov     lr, r2                                 \n\t"
                "pop     {r0, r1, r2, r3}                       \n\t"
                "cpsie   i                                      \n\t"
                "pop     {pc}" : : : "memory");
}

#define PUSH_CONTEXT(sp) {                                                  \
  asm volatile ("push    {r4, r5, r6, r7, lr}                   \n\t"       \
                "mov     r4, r8                                 \n\t"       \
                "mov     r5, r9                                 \n\t"       \
                "mov     r6, r10                                \n\t"       \
                "mov     r7, r11                                \n\t"       \
                "push    {r4, r5, r6, r7}" : : : "memory");                 \
}

#define POP_CONTEXT(sp) {                                                   \
  asm volatile ("pop     {r4, r5, r6, r7}                       \n\t"       \
                "mov     r8, r4                                 \n\t"       \
                "mov     r9, r5                                 \n\t"       \
                "mov     r10, r6                                \n\t"       \
                "mov     r11, r7                                \n\t"       \
                "pop     {r4, r5, r6, r7, pc}" : : "r" (sp) : "memory");    \
}

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
  register struct intctx *r13 asm ("r13");

  /* Stack overflow check, if enabled.*/
#if CH_DBG_ENABLE_STACK_CHECK
  if ((void *)(r13 - 1) < (void *)(otp + 1))
    asm volatile ("movs    r0, #0                               \n\t"
                  "b       chDbgPanic");
#endif /* CH_DBG_ENABLE_STACK_CHECK */

  PUSH_CONTEXT(r13);

  otp->p_ctx.r13 = r13;
  r13 = ntp->p_ctx.r13;

  POP_CONTEXT(r13);
}

/**
 * @brief   Start a thread by invoking its work function.
 * @details If the work function returns @p chThdExit() is automatically
 *          invoked.
 */
void _port_thread_start(void) {

  port_unlock();
  asm volatile ("mov     r0, r5                                 \n\t"
                "blx     r4                                     \n\t"
                "bl      chThdExit");
}

/** @} */
