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

#ifndef _CHCORE_H_
#define _CHCORE_H_

/*
 * Port-related configuration parameters.
 */
#ifndef BASEPRI_USER
#define BASEPRI_USER    0       /* User level BASEPRI, 0 = disabled.    */
#endif

#ifndef BASEPRI_KERNEL
#define BASEPRI_KERNEL  0x10    /* BASEPRI level within kernel lock.    */
#endif

#ifndef ENABLE_WFI_IDLE
#define ENABLE_WFI_IDLE 0       /* Enables the use of the WFI ins.      */
#endif

/*
 * Macro defining the ARM Cortex-M3 architecture.
 */
#define CH_ARCHITECTURE_ARMCM3

/*
 * 32 bit stack alignment.
 */
typedef uint32_t stkalign_t;

/*
 * Generic ARM register.
 */
typedef void *regarm_t;

/*
 * Interrupt saved context, empty in this architecture.
 */
struct extctx {
};

/*
 * System saved context.
 */
struct intctx {
  regarm_t      basepri;
  regarm_t      r4;
  regarm_t      r5;
  regarm_t      r6;
#ifndef CH_CURRP_REGISTER_CACHE
  regarm_t      r7;
#endif
  regarm_t      r8;
  regarm_t      r9;
  regarm_t      r10;
  regarm_t      r11;
  regarm_t      lr_exc;
  regarm_t      r0;
  regarm_t      r1;
  regarm_t      r2;
  regarm_t      r3;
  regarm_t      r12;
  regarm_t      lr_thd;
  regarm_t      pc;
  regarm_t      xpsr;
};

/*
 * Port dependent part of the Thread structure, you may add fields in
 * this structure.
 */
typedef struct {
  struct intctx *r13;
} Context;

/*
 * Platform dependent part of the \p chThdCreate() API.
 *
 * The top of the workspace is used for the intctx datastructure.
 *
 */
#define SETUP_CONTEXT(workspace, wsize, pf, arg) {                      \
  tp->p_ctx.r13 = (struct intctx *)((uint8_t *)workspace +              \
                                     wsize -                            \
                                     sizeof(struct intctx));            \
  tp->p_ctx.r13->basepri = BASEPRI_USER;                                \
  tp->p_ctx.r13->lr_exc = (regarm_t)0xFFFFFFFD;                         \
  tp->p_ctx.r13->r0 = arg;                                              \
  tp->p_ctx.r13->r1 = pf;                                               \
  tp->p_ctx.r13->pc = threadstart;                                      \
  tp->p_ctx.r13->xpsr = (regarm_t)0x01000000;                           \
}

#define chSysLock() {                                                   \
  register uint32_t tmp asm ("r3") = BASEPRI_KERNEL;                    \
  asm volatile ("msr     BASEPRI, %0" : : "r" (tmp));                   \
}
#define chSysUnlock() {                                                 \
  register uint32_t tmp asm ("r3") = BASEPRI_USER;                      \
  asm volatile ("msr     BASEPRI, %0" : : "r" (tmp));                   \
}
#define chSysEnable() {                                                 \
  register uint32_t tmp asm ("r3") = BASEPRI_USER;                      \
  asm volatile ("msr     BASEPRI, %0" : : "r" (tmp));                   \
}
#define chSysSwitchI(otp, ntp) {                                        \
  register Thread *_otp asm ("r0") = (otp);                             \
  register Thread *_ntp asm ("r1") = (ntp);                             \
  asm volatile ("svc     #0" : : "r" (_otp), "r" (_ntp));               \
}

#ifndef INT_REQUIRED_STACK
#define INT_REQUIRED_STACK 0            /* NOTE: Always safe for this port. */
#endif

/*
 * Enforces a 32 bit alignment for a stack area size value.
 */
#define STACK_ALIGN(n) ((((n) - 1) | sizeof(stkalign_t)) + 1)

#define THD_WA_SIZE(n) STACK_ALIGN(sizeof(Thread) +                     \
                                   sizeof(struct intctx) +              \
                                   sizeof(struct extctx) +              \
                                   (n) +                                \
                                   INT_REQUIRED_STACK)

#define WORKING_AREA(s, n) stkalign_t s[THD_WA_SIZE(n) / sizeof(stkalign_t)];

/* called on each interrupt entry, currently nothing is done */
#define chSysIRQEnterI()

/* called on each interrupt exit, pends a supervisor handler for
 * execution after all higher priority interrupts; PendSVVector() */
#define chSysIRQExitI() {                                               \
  SCB_ICSR = ICSR_PENDSVSET;                                            \
}

#define IDLE_THREAD_STACK_SIZE 0

#ifdef __cplusplus
extern "C" {
#endif
  void _idle(void *p) __attribute__((weak, noreturn));
  void chSysHalt(void);
  void chSysPuts(char *msg);
  void threadstart(void);
#ifdef __cplusplus
}
#endif

#endif /* _CHCORE_H_ */
