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

/**
 * @file ports/ARMCM3/chcore.c
 * @brief ARM Cortex-M3 architecture port code.
 * @addtogroup ARMCM3_CORE
 * @{
 */

#include <ch.h>
#include <nvic.h>

/**
 * Halts the system.
 * @note The function is declared as a weak symbol, it is possible to redefine
 *       it in your application code.
 */
/** @cond never */
__attribute__((weak))
/** @endcond */
void port_halt(void) {

  port_disable();
  while (TRUE) {
  }
}

#if !CH_OPTIMIZE_SPEED
void _port_lock(void) {
  register uint32_t tmp asm ("r3") = BASEPRI_KERNEL;
  asm volatile ("msr     BASEPRI, %0" : : "r" (tmp));
}

void _port_unlock(void) {
  register uint32_t tmp asm ("r3") = BASEPRI_USER;
  asm volatile ("msr     BASEPRI, %0" : : "r" (tmp));
}
#endif

/**
 * System Timer vector.
 * This interrupt is used as system tick.
 * @note The timer is initialized in the board setup code.
 */
CH_IRQ_HANDLER(SysTickVector) {

  CH_IRQ_PROLOGUE();

  chSysLockFromIsr();
  chSysTimerHandlerI();
  chSysUnlockFromIsr();

  CH_IRQ_EPILOGUE();
}

/**
 * The SVC vector is used for commanded context switch.
 * @param otp the thread to be switched out
 * @param ntp the thread to be switched it
 */
/** @cond never */
__attribute__((naked))
/** @endcond */
void SVCallVector(Thread *otp, Thread *ntp) {
  /* { r0 = otp, r1 = ntp } */
  /* get the BASEPRI in r3 */
  /* get the PSP in r12 */
  /* push the registers on the PSP stack */
  /* stores the modified PSP into the thread context */
  /* fetches the PSP position from the new thread context */
  /* pop the registers from the PSP stack */
  /* set the PSP from r12 */
  /* set the BASEPRI from R3 */
#ifdef CH_CURRP_REGISTER_CACHE
  asm volatile ("mrs     r3, BASEPRI                            \n\t" \
                "mrs     r12, PSP                               \n\t" \
                "stmdb   r12!, {r3-r6,r8-r11, lr}               \n\t" \
                "str     r12, [r0, #16]                         \n\t" \
                "ldr     r12, [r1, #16]                         \n\t" \
                "ldmia   r12!, {r3-r6,r8-r11, lr}               \n\t" \
                "msr     PSP, r12                               \n\t" \
                "msr     BASEPRI, r3                            \n\t" \
                "bx      lr                                     ");
#else
  asm volatile ("mrs     r3, BASEPRI                            \n\t" \
                "mrs     r12, PSP                               \n\t" \
                "stmdb   r12!, {r3-r11, lr}                     \n\t" \
                "str     r12, [r0, #16]                         \n\t" \
                "ldr     r12, [r1, #16]                         \n\t" \
                "ldmia   r12!, {r3-r11, lr}                     \n\t" \
                "msr     PSP, r12                               \n\t" \
                "msr     BASEPRI, r3                            \n\t" \
                "bx      lr                                     ");
#endif
}

#ifdef CH_CURRP_REGISTER_CACHE
#define PUSH_CONTEXT(sp) {                                              \
  register uint32_t tmp asm ("r3") = BASEPRI_USER;                      \
  asm volatile ("mrs     %0, PSP                                \n\t"   \
                "stmdb   %0!, {r3-r6,r8-r11, lr}" :                     \
                "=r" (sp) : "r" (sp), "r" (tmp));                       \
}

#define POP_CONTEXT(sp) {                                               \
  asm volatile ("ldmia   %0!, {r3-r6,r8-r11, lr}                \n\t"   \
                "msr     PSP, %0                                \n\t"   \
                "msr     BASEPRI, r3                            \n\t"   \
                "bx      lr" : "=r" (sp) : "r" (sp));                   \
}
#else
#define PUSH_CONTEXT(sp) {                                              \
  register uint32_t tmp asm ("r3") = BASEPRI_USER;                      \
  asm volatile ("mrs     %0, PSP                                \n\t"   \
                "stmdb   %0!, {r3-r11,lr}" :                            \
                "=r" (sp) : "r" (sp), "r" (tmp));                       \
}

#define POP_CONTEXT(sp) {                                               \
  asm volatile ("ldmia   %0!, {r3-r11, lr}                      \n\t"   \
                "msr     PSP, %0                                \n\t"   \
                "msr     BASEPRI, r3                            \n\t"   \
                "bx      lr" : "=r" (sp) : "r" (sp));                   \
}
#endif

/**
 * Preemption invoked context switch.
 */
/** @cond never */
__attribute__((naked))
/** @endcond */
void PendSVVector(void) {
  Thread *otp;
  register struct intctx *sp_thd asm("r12");

  asm volatile ("push    {lr}");
  chSysLockFromIsr();
  if (!chSchRescRequiredI()) {
    chSysUnlockFromIsr();
    asm volatile ("pop     {pc}");
  }
  asm volatile ("pop     {lr}");

  PUSH_CONTEXT(sp_thd);

  (otp = currp)->p_ctx.r13 = sp_thd;
  (currp = fifo_remove((void *)&rlist))->p_state = PRCURR;
  chSchReadyI(otp);
#if CH_USE_ROUNDROBIN
  /* set the round-robin time quantum */
  rlist.r_preempt = CH_TIME_QUANTUM;
#endif
  chDbgTrace(otp, currp);
  sp_thd = currp->p_ctx.r13;

  POP_CONTEXT(sp_thd);
}

/** @} */
