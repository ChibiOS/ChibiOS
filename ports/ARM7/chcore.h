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
 * Macro defining the ARM7 architecture.
 */
#define CH_ARCHITECTURE_ARM7

/*
 * 32 bit stack alignment.
 */
typedef uint32_t stkalign_t;

/*
 * Generic ARM register.
 */
typedef void *regarm_t;

/*
 * Interrupt saved context.
 */
struct extctx {
  regarm_t      spsr_irq;
  regarm_t      lr_irq;
  regarm_t      r0;
  regarm_t      r1;
  regarm_t      r2;
  regarm_t      r3;
  regarm_t      r12;
  regarm_t      lr_usr;
};

/*
 * System saved context.
 */
struct intctx {
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
  regarm_t      lr;
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

#ifndef INT_REQUIRED_STACK
#ifdef THUMB
#define INT_REQUIRED_STACK 0x10
#else /* !THUMB */
#define INT_REQUIRED_STACK 0x10
#endif /* !THUMB */
#endif

#define STACK_ALIGN(n) ((((n) - 1) | (sizeof(stkalign_t) - 1)) + 1)

#define THD_WA_SIZE(n) STACK_ALIGN(sizeof(Thread) +                     \
                                   sizeof(struct intctx) +              \
                                   sizeof(struct extctx) +              \
                                   (n) +                                \
                                   INT_REQUIRED_STACK)

#define WORKING_AREA(s, n) stkalign_t s[THD_WA_SIZE(n) / sizeof(stkalign_t)];

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
  void _idle(void *p) __attribute__((weak, noreturn));
  void chSysHalt(void);
  void chSysSwitchI(Thread *otp, Thread *ntp);
  void chSysPuts(char *msg);
  void threadstart(void);
#ifdef __cplusplus
}
#endif

#endif /* _CHCORE_H_ */
