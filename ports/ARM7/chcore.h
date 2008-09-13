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

#ifndef _CHCORE_H_
#define _CHCORE_H_

#define CH_ARCHITECTURE_ARM7

typedef void *regarm;

/*
 * Interrupt saved context.
 */
struct extctx {
  regarm  spsr_irq;
  regarm  lr_irq;
  regarm  r0;
  regarm  r1;
  regarm  r2;
  regarm  r3;
  regarm  r12;
  regarm  lr_usr;
};

/*
 * System saved context.
 */
struct intctx {
  regarm  r4;
  regarm  r5;
  regarm  r6;
#ifndef CH_CURRP_REGISTER_CACHE
  regarm  r7;
#endif
  regarm  r8;
  regarm  r9;
  regarm  r10;
  regarm  r11;
  regarm  lr;
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
 */
#define SETUP_CONTEXT(workspace, wsize, pf, arg) {                      \
  tp->p_ctx.r13 = (struct intctx *)((uint8_t *)workspace +              \
                                     wsize -                            \
                                     sizeof(struct intctx));            \
  tp->p_ctx.r13->r4 = pf;                                               \
  tp->p_ctx.r13->r5 = arg;                                              \
  tp->p_ctx.r13->lr = threadstart;                                      \
}

#ifdef THUMB
#ifdef __cplusplus
extern "C" {
#endif
  uint32_t _lock(void);
  void _unlock(uint32_t);
  void _enable(void);
#ifdef __cplusplus
}
#endif
#ifdef REENTRANT_LOCKS
#define chSysLock() uint32_t ps = _lock()
#define chSysUnlock() _unlock(ps)
#else
#define chSysLock() _lock()
#define chSysUnlock() _enable()
#endif /* !REENTRANT_LOCKS */
#define chSysEnable() _enable()
#else /* !THUMB */
#ifdef REENTRANT_LOCKS
#define chSysLock()                                                     \
  uint32_t ps;                                                          \
  asm volatile ("mrs     %0, CPSR" : "=r" (ps) : );                     \
  asm volatile ("msr     CPSR_c, #0x9F");
#define chSysUnlock() asm volatile ("msr     CPSR_c, %0" : : "r" (ps))
#define chSysEnable() asm volatile ("msr     CPSR_c, #0x1F")
#else
#define chSysLock() asm volatile ("msr     CPSR_c, #0x9F")
#define chSysUnlock() asm volatile ("msr     CPSR_c, #0x1F")
#define chSysEnable() asm volatile ("msr     CPSR_c, #0x1F")
#endif /* !REENTRANT_LOCKS */
#endif /* THUMB */

#ifdef THUMB
#define INT_REQUIRED_STACK 0x10
#else /* !THUMB */
#define INT_REQUIRED_STACK 0
#endif /* !THUMB */
#define StackAlign(n) ((((n) - 1) | 3) + 1)
#define UserStackSize(n) StackAlign(sizeof(Thread) +                    \
                                    sizeof(struct intctx) +             \
                                    sizeof(struct extctx) +             \
                                    (n) +                               \
                                    INT_REQUIRED_STACK)
#define WorkingArea(s, n) uint32_t s[UserStackSize(n) >> 2];

#ifdef THUMB
#define chSysSwitchI chSysSwitchI_thumb

#define chSysIRQEnterI() {                                              \
  asm(".code 32                                 \n\t"                   \
      "stmfd    sp!, {r0-r3, r12, lr}           \n\t"                   \
      "add      r0, pc, #1                      \n\t"                   \
      "bx       r0                              \n\t"                   \
      ".code 16                                 \n\t");                 \
}

#define chSysIRQExitI() {                                               \
  asm("ldr      r0, =IrqCommon                  \n\t"                   \
      "bx       r0                              \n\t");                 \
}
#else /* !THUMB */
#define chSysSwitchI chSysSwitchI_arm

#define chSysIRQEnterI() {                                              \
  asm("stmfd    sp!, {r0-r3, r12, lr}           \n\t");                 \
}

#define chSysIRQExitI() {                                               \
  asm("b        IrqCommon                       \n\t");                 \
}
#endif /* !THUMB */

/* It requires zero bytes, but better be safe.*/
#define IDLE_THREAD_STACK_SIZE 8

#ifdef __cplusplus
extern "C" {
#endif
  void _IdleThread(void *p) __attribute__((noreturn));
  void chSysHalt(void);
  void chSysSwitchI(Thread *otp, Thread *ntp);
  void chSysPuts(char *msg);
  void threadstart(void);
#ifdef __cplusplus
}
#endif

#endif /* _CHCORE_H_ */
