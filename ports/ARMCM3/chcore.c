/*
    ChibiOS/RT - Copyright (C) 2006-2007 Giovanni Di Sirio.

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

#include <ch.h>
#include <nvic.h>

/*
 * System idle thread loop.
 */
__attribute__((weak))
void _idle(void *p) {

  while (TRUE) {
#if ENABLE_WFI_IDLE != 0
    asm volatile ("wfi");
#endif
  }
}

/*
 * System console message (not implemented).
 */
__attribute__((weak))
void chSysPuts(char *msg) {
}

/*
 * System halt.
 */
__attribute__((naked, weak))
void chSysHalt(void) {

  asm volatile ("cpsid   i");
  while (TRUE) {
  }
}

/*
 * Start a thread by invoking its work function.
 *
 * Start a thread by calling its work function. If the work function returns,
 * call chThdExit and chSysHalt.
 */
__attribute__((naked, weak))
void threadstart(void) {

  asm volatile ("blx     r1                                     \n\t" \
                "bl      chThdExit                              \n\t" \
                "bl      chSysHalt                              ");
}

/*
 * System Timer vector.
 */
void SysTickVector(void) {

  chSysIRQEnterI();
  chSysLock();

  chSysTimerHandlerI();

  chSysUnlock();
  chSysIRQExitI();
}

/*
 * System invoked context switch.
 */
__attribute__((naked))
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

/*
 * Preemption invoked context switch.
 */
__attribute__((naked))
void PendSVVector(void) {
  Thread *otp;
  register struct intctx *sp_thd asm("r12");

  chSysLock();
  asm volatile ("push    {lr}");
  if (!chSchRescRequiredI()) {
    chSysUnlock();
    asm volatile ("pop     {pc}");
  }
  asm volatile ("pop     {lr}");

  PUSH_CONTEXT(sp_thd);

  (otp = currp)->p_ctx.r13 = sp_thd;
  (currp = fifo_remove((void *)&rlist))->p_state = PRCURR;
  chSchReadyI(otp);
#ifdef CH_USE_ROUNDROBIN
  /* set the round-robin time quantum */
  rlist.r_preempt = CH_TIME_QUANTUM;
#endif
#ifdef CH_USE_TRACE
  chDbgTrace(otp, currp);
#endif
  sp_thd = currp->p_ctx.r13;

  POP_CONTEXT(sp_thd);
}
