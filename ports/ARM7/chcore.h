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

/**
 * @addtogroup ARM7_CORE
 * @{
 */

#ifndef _CHCORE_H_
#define _CHCORE_H_

/**
 * Macro defining the ARM7 architecture.
 */
#define CH_ARCHITECTURE_ARM7

/**
 * 32 bit stack alignment.
 */
typedef uint32_t stkalign_t;

/**
 * Generic ARM register.
 */
typedef void *regarm_t;

/**
 * Interrupt saved context.
 * This structure represents the stack frame saved during a preemption-capable
 * interrupt handler.
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

/**
 * System saved context.
 * This structure represents the inner stack frame during a context switching.
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

/**
 * Platform dependent part of the @p Thread structure.
 * In the ARM7 port this structure contains just the copy of the user mode
 * stack pointer.
 */
typedef struct {
  struct intctx *r13;
} Context;

/**
 * Platform dependent part of the @p chThdInit() API.
 */
#define SETUP_CONTEXT(workspace, wsize, pf, arg) {                      \
  tp->p_ctx.r13 = (struct intctx *)((uint8_t *)workspace +              \
                                     wsize -                            \
                                     sizeof(struct intctx));            \
  tp->p_ctx.r13->r4 = pf;                                               \
  tp->p_ctx.r13->r5 = arg;                                              \
  tp->p_ctx.r13->lr = threadstart;                                      \
}

/**
 * Stack size for the system idle thread.
 */
#ifndef IDLE_THREAD_STACK_SIZE
#define IDLE_THREAD_STACK_SIZE 0
#endif

/**
 * Per-thread stack overhead for interrupts servicing, it is used in the
 * calculation of the correct working area size.
 * In this port 0x10 is a safe value, it can be reduced after careful generated
 * code analysis.
 */
#ifndef INT_REQUIRED_STACK
#define INT_REQUIRED_STACK 0x10
#endif

/**
 * Enforces a correct alignment for a stack area size value.
 */
#define STACK_ALIGN(n) ((((n) - 1) | sizeof(stkalign_t)) + 1)

 /**
  * Computes the thread working area global size.
  */
#define THD_WA_SIZE(n) STACK_ALIGN(sizeof(Thread) +                     \
                                   sizeof(struct intctx) +              \
                                   sizeof(struct extctx) +              \
                                  (n) + (INT_REQUIRED_STACK))

/**
 * Macro used to allocate a thread working area aligned as both position and
 * size.
 */
#define WORKING_AREA(s, n) stkalign_t s[THD_WA_SIZE(n) / sizeof(stkalign_t)];

/**
 * IRQ prologue code, inserted at the start of all IRQ handlers enabled to
 * invoke system APIs.
 * @note This macro has a different implementation depending if compiled in
 *       ARM or TUMB mode.
 */
#ifdef THUMB
#define SYS_IRQ_PROLOGUE() {                                            \
  asm volatile (".code 32                                 \n\t"         \
                "stmfd    sp!, {r0-r3, r12, lr}           \n\t"         \
                "add      r0, pc, #1                      \n\t"         \
                "bx       r0                              \n\t"         \
                ".code 16");                                            \
}
#else /* THUMB */
#define SYS_IRQ_PROLOGUE() {                                            \
  asm volatile ("stmfd    sp!, {r0-r3, r12, lr}");                      \
}
#endif /* !THUMB */

/**
 * IRQ epilogue code, inserted at the end of all IRQ handlers enabled to
 * invoke system APIs.
 * @note This macro has a different implementation depending if compiled in
 *       ARM or TUMB mode.
 */
#ifdef THUMB
#define SYS_IRQ_EPILOGUE() {                                            \
  asm volatile ("ldr      r0, =IrqCommon                  \n\t"         \
                "bx       r0");                                         \
}
#else /* THUMB */
#define SYS_IRQ_EPILOGUE() {                                            \
  asm volatile ("b        IrqCommon");                                  \
}
#endif /* !THUMB */

/**
 * IRQ handler function modifier.
 */
#define SYS_IRQ_HANDLER __attribute__((naked))

#ifdef __cplusplus
extern "C" {
#endif
  void sys_puts(char *msg);
  void sys_switch(Thread *otp, Thread *ntp);
  void sys_enable(void);
  void sys_disable(void);
  void sys_disable_from_isr(void);
  void sys_enable_from_isr(void);
  void sys_wait_for_interrupt(void);
  void sys_halt(void);
#ifdef __cplusplus
}
#endif

#endif /* _CHCORE_H_ */

/** @} */
