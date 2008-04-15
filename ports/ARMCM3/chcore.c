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
void _IdleThread(void *p) {

  while (TRUE) {
//    asm volatile ("wfi");
  }
}

/*
 * System console message (not implemented).
 */
__attribute__((weak))
void chSysPuts(char *msg) {
}

/*
 * Context switch.
 */
void chSysSwitchI(Thread *otp, Thread *ntp) {

  asm volatile ("cpsie   i                                      \n\t" \
                "svc     #0                                     \n\t" \
                "cpsid   i                                      ");
}

/*
 * System halt.
 */
__attribute__((naked, weak))
void chSysHalt(void) {

  chSysLock();
  while (TRUE) {
  }
}

__attribute__((naked, weak))
void threadstart(void) {

  asm volatile ("cpsie   i                                      \n\t" \
                "blx     r1                                     \n\t" \
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

void *retaddr;

/*
 * To be invoked at the end of any interrupt handler that can trigger a
 * reschedule.
 */
void chSysIRQExitI(void) {

  chSysLock();

  if ((SCB_ICSR & ICSR_RETTOBASE) && chSchRescRequiredI()) {
    SCB_ICSR = ICSR_PENDSVSET;
  }

  chSysUnlock();
}

/*
 * System invoked context switch.
 */
__attribute__((naked))
void SVCallVector(Thread *otp, Thread *ntp) {

#ifdef CH_CURRP_REGISTER_CACHE
  asm volatile ("mrs     r12, PSP                               \n\t" \
                "stmdb   r12!, {r4-6,r8-r11, lr}                \n\t" \
                "str     r12, [r0, #16]                         \n\t" \
                "ldr     r12, [r1, #16]                         \n\t" \
                "ldmia   r12!, {r4-6,r8-r11, lr}                \n\t" \
                "msr     PSP, r12                               \n\t" \
                "bx      lr                                     ");
#else
  asm volatile ("mrs     r12, PSP                               \n\t" \
                "stmdb   r12!, {r4-r11, lr}                     \n\t" \
                "str     r12, [r0, #16]                         \n\t" \
                "ldr     r12, [r1, #16]                         \n\t" \
                "ldmia   r12!, {r4-r11, lr}                     \n\t" \
                "msr     PSP, r12                               \n\t" \
                "bx      lr                                     ");
#endif
}

/*
 * Preemption invoked context switch.
 */
__attribute__((naked))
void PendSVVector(void) {
  Thread *otp;
  register struct intctx *sp_thd asm("r12");

  asm volatile ("cpsid   i                                      \n\t" \
                "mrs     %0, PSP" : "=r" (sp_thd) : );
#ifdef CH_CURRP_REGISTER_CACHE
  asm volatile ("stmdb   %0!, {r4-r6,r8-r11, lr}" :
                "=r" (sp_thd) :
                "r" (sp_thd));
#else
  asm volatile ("stmdb   %0!, {r4-r11, lr}" :
                "=r" (sp_thd) :
                "r" (sp_thd));
#endif

  (otp  = currp)->p_ctx.r13 = sp_thd;
  chSchReadyI(otp);
  (currp = fifo_remove(&rlist.r_queue))->p_state = PRCURR;
  rlist.r_preempt = CH_TIME_QUANTUM;
#ifdef CH_USE_TRACE
  chDbgTrace(otp, currp);
#endif

  sp_thd = currp->p_ctx.r13;

#ifdef CH_CURRP_REGISTER_CACHE
  asm volatile ("ldmia   %0!, {r4-r6,r8-r11, lr}" : : "r" (sp_thd));
#else
  asm volatile ("ldmia   %0!, {r4-r11, lr}" : : "r" (sp_thd));
#endif
  asm volatile ("msr     PSP, %0                                \n\t" \
                "cpsie   i                                      \n\t" \
                "bx      lr" : : "r" (sp_thd));
}
