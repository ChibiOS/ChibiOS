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
 * If enabled allows the idle thread to enter a low power mode.
 */
#ifndef ENABLE_WFI_IDLE
#define ENABLE_WFI_IDLE 0
#endif

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
  tp->p_ctx.r13->lr = _sys_thread_start;                                \
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
 *       ARM or THUMB mode.
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
 *       ARM or THUMB mode.
 */
#ifdef THUMB
#define SYS_IRQ_EPILOGUE() {                                            \
  asm volatile ("ldr      r0, =_sys_irq_common            \n\t"         \
                "bx       r0");                                         \
}
#else /* THUMB */
#define SYS_IRQ_EPILOGUE() {                                            \
  asm volatile ("b        _sys_irq_common");                            \
}
#endif /* !THUMB */

/**
 * IRQ handler function modifier.
 */
#define SYS_IRQ_HANDLER __attribute__((naked))

/**
 * Performs a context switch between two threads.
 * @param otp the thread to be switched out
 * @param ntp the thread to be switched in
 * @note This macro has a different implementation depending if compiled in
 *       ARM or THUMB mode.
 * @note This macro assumes to be invoked in ARM system mode.
 */
#ifdef THUMB
#define sys_switch(otp, ntp) _sys_switch_thumb(otp, ntp)
#else /* THUMB */
#define sys_switch(otp, ntp) _sys_switch_arm(otp, ntp)
#endif /* !THUMB */

/**
 * In this port this macro disables the IRQ sources.
 * @note This macro has a different implementation depending if compiled in
 *       ARM or THUMB mode.
 * @note This macro assumes to be invoked in ARM system mode.
 */
#ifdef THUMB
#define sys_disable() _sys_disable_thumb()
#else /* THUMB */
#define sys_disable() asm volatile ("msr     CPSR_c, #0x9F")
#endif /* !THUMB */

/**
 * This port function is implemented as inlined code for performance reasons.
 * @note This macro has a different implementation depending if compiled in
 *       ARM or THUMB mode.
 * @note This macro assumes to be invoked in ARM system mode.
 */
#ifdef THUMB
#define sys_enable() _sys_enable_thumb()
#else /* THUMB */
#define sys_enable() asm volatile ("msr     CPSR_c, #0x1F")
#endif /* !THUMB */

/**
 * This function is empty in this port.
 */
#define sys_disable_from_isr()

/**
 * This function is empty in this port.
 */
#define sys_enable_from_isr()

/**
 * Disables all the interrupt sources, even those having a priority higher
 * to the kernel.
 * In the ARM7 port this code disables both IRQ and FIQ sources.
 */
#ifdef THUMB
#define sys_disable_all() _sys_disable_all_thumb()
#else /* THUMB */
#define sys_disable_all() {                                             \
  asm volatile ("mrs     r3, CPSR                         \n\t"         \
                "orr     r3, #0x80                        \n\t"         \
                "msr     CPSR_c, r3                       \n\t"         \
                "orr     r3, #0x40                        \n\t"         \
                "msr     CPSR_c, r3" : : : "r3");                       \
}
#endif /* !THUMB */

#ifdef __cplusplus
extern "C" {
#endif
  void sys_puts(char *msg);
  void sys_wait_for_interrupt(void);
  void sys_halt(void);
  void _sys_enable_thumb(void);
  void _sys_disable_thumb(void);
#ifdef THUMB
  void _sys_switch_thumb(Thread *otp, Thread *ntp);
#else /* THUMB */
  void _sys_switch_arm(Thread *otp, Thread *ntp);
#endif /* !THUMB */
  void _sys_thread_start(void);
#ifdef __cplusplus
}
#endif

#endif /* _CHCORE_H_ */

/** @} */
