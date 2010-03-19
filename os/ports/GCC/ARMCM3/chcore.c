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
 * @file    ARMCM3/chcore.c
 * @brief   ARM Cortex-M3 architecture port code.
 *
 * @addtogroup ARMCM3_CORE
 * @{
 */

#include "ch.h"
#include "nvic.h"

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
 * @brief   System Timer vector.
 * @details This interrupt is used as system tick.
 * @note    The timer must be initialized in the startup code.
 */
void SysTickVector(void) {

  chSysLockFromIsr();
  chSysTimerHandlerI();
  if (chSchIsRescRequiredExI())
    SCB_ICSR = ICSR_PENDSVSET;
  chSysUnlockFromIsr();
}

/**
 * @brief   SVC vector.
 * @details The SVC vector is used for commanded context switch. Structures
 *          @p intctx are saved and restored from the process stacks of the
 *          switched threads.
 *
 * @param[in] ntp       the thread to be switched it
 * @param[in] otp       the thread to be switched out
 */
#if !defined(__DOXYGEN__)
__attribute__((naked))
#endif
void SVCallVector(Thread *ntp, Thread *otp) {
  (void)otp;
  (void)ntp;
#ifdef CH_CURRP_REGISTER_CACHE
  asm volatile ("mrs     r3, BASEPRI                            \n\t" \
                "mrs     r12, PSP                               \n\t" \
                "stmdb   r12!, {r3-r6,r8-r11, lr}               \n\t" \
                "str     r12, [r1, #12]                         \n\t" \
                "ldr     r12, [r0, #12]                         \n\t" \
                "ldmia   r12!, {r3-r6,r8-r11, lr}               \n\t" \
                "msr     PSP, r12                               \n\t" \
                "msr     BASEPRI, r3                            \n\t" \
                "bx      lr                                     ");
#else
  asm volatile ("mrs     r3, BASEPRI                            \n\t" \
                "mrs     r12, PSP                               \n\t" \
                "stmdb   r12!, {r3-r11, lr}                     \n\t" \
                "str     r12, [r1, #12]                         \n\t" \
                "ldr     r12, [r0, #12]                         \n\t" \
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
 * @brief   Preemption code.
 */
#if !defined(__DOXYGEN__)
__attribute__((naked))
#endif
void PendSVVector(void) {
  Thread *otp, *ntp;
  register struct intctx *sp_thd asm("r12");

  chSysLockFromIsr();

  PUSH_CONTEXT(sp_thd);

  (otp = currp)->p_ctx.r13 = sp_thd;
  ntp = fifo_remove(&rlist.r_queue);
  setcurrp(ntp);
  ntp->p_state = THD_STATE_CURRENT;
  chSchReadyI(otp);
#if CH_TIME_QUANTUM > 0
  /* Set the round-robin time quantum.*/
  rlist.r_preempt = CH_TIME_QUANTUM;
#endif
  chDbgTrace(ntp, otp);
  sp_thd = ntp->p_ctx.r13;

  POP_CONTEXT(sp_thd);
}

/** @} */
