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
 * @brief   NMI vector.
 * @details The NMI vector is used for exception mode re-entering after a
 *          context switch.
 */
void NMIVector(void) {
  register struct extctx *ctxp;

  /* Discarding the current exception context and positioning the stack to
     point to the real one.*/
  asm volatile ("mrs     %0, PSP" : "=r" (ctxp) : : "memory");
  ctxp++;
  asm volatile ("msr     PSP, %0" : : "r" (ctxp) : "memory");
  port_unlock_from_isr();
}

/**
 * @brief   Post-IRQ switch code.
 * @details The switch is performed in thread context then an NMI exception
 *          is enforced in order to return to the exact point before the
 *          preemption.
 */
#if !defined(__DOXYGEN__)
__attribute__((naked))
#endif
void _port_switch_from_isr(void) {

  chSchDoRescheduleI();
  SCB_ICSR = ICSR_NMIPENDSET;
  /* The following loop should never be executed, the NMI will kick in
     immediately.*/
  while (TRUE)
    ;
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
void _port_switch(Thread *ntp, Thread *otp) {
  register struct intctx *r13 asm ("r13");

  PUSH_CONTEXT(r13);

  otp->p_ctx.r13 = r13;
  r13 = ntp->p_ctx.r13;

  POP_CONTEXT(r13);
}

/**
 * @brief   IRQ epilogue code.
 *
 * @param[in] lr        value of the @p LR register on ISR entry
 */
void _port_irq_epilogue(regarm_t lr) {

  if (lr != (regarm_t)0xFFFFFFF1) {
    port_lock_from_isr();
    if (chSchIsRescRequiredExI()) {
      register struct extctx *ctxp;

      /* Adding an artificial exception return context, there is no need to
         populate it fully.*/
      asm volatile ("mrs     %0, PSP" : "=r" (ctxp) : : "memory");
      ctxp--;
      asm volatile ("msr     PSP, %0" : : "r" (ctxp) : "memory");
      ctxp->pc = _port_switch_from_isr;
      ctxp->xpsr = (regarm_t)0x01000000;
      /* Note, returning without unlocking is intentional, this is done in
        order to keep the rest of the context switching atomic.*/
      return;
    }
    port_unlock_from_isr();
  }
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
