/*
    ChibiOS/RT - Copyright (C) 2010 Giovanni Di Sirio.

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
 * @file ARM7/chcore.h
 * @brief ARM7 architecture port macros and structures.
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
#include <wfi.h>

/**
 * Macro defining the ARM7 architecture.
 */
#define CH_ARCHITECTURE_ARM7

/**
 * Name of the implemented architecture.
 */
#define CH_ARCHITECTURE_NAME "ARM7TDMI"

/**
 * 32 bit stack alignment.
 */
typedef uint32_t stkalign_t;

/**
 * Generic ARM register.
 */
typedef void *regarm_t;

/** @cond never */
/**
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
/** @endcond */

/** @cond never */
/**
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
/** @endcond */

/** @cond never */
/**
 * In the ARM7 port this structure contains just the copy of the user mode
 * stack pointer.
 */
struct context {
  struct intctx *r13;
};
/** @endcond */

/**
 * Platform dependent part of the @p chThdInit() API.
 */
#define SETUP_CONTEXT(workspace, wsize, pf, arg) {                      \
  tp->p_ctx.r13 = (struct intctx *)((uint8_t *)workspace +              \
                                     wsize -                            \
                                     sizeof(struct intctx));            \
  tp->p_ctx.r13->r4 = pf;                                               \
  tp->p_ctx.r13->r5 = arg;                                              \
  tp->p_ctx.r13->lr = _port_thread_start;                               \
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
#define STACK_ALIGN(n) ((((n) - 1) | (sizeof(stkalign_t) - 1)) + 1)

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
 * @note The THUMB implementation starts with ARM code because interrupt
 *       vectors are always invoked in ARM mode regardless the bit 0
 *       value. The switch in THUMB mode is done in the function prologue so
 *       it is transparent to the user code.
 */
#ifdef THUMB
#define PORT_IRQ_PROLOGUE() {                                           \
  asm volatile (".code 32                               \n\t"           \
                "stmfd   sp!, {r0-r3, r12, lr}          \n\t"           \
                "add     r0, pc, #1                     \n\t"           \
                "bx      r0                             \n\t"           \
                ".code 16");                                            \
}
#else /* !THUMB */
#define PORT_IRQ_PROLOGUE() {                                           \
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
#define PORT_IRQ_EPILOGUE() {                                           \
  asm volatile ("ldr     r0, =_port_irq_common          \n\t"           \
                "bx      r0");                                          \
}
#else /* !THUMB */
#define PORT_IRQ_EPILOGUE() {                                           \
  asm volatile ("b       _port_irq_common");                            \
}
#endif /* !THUMB */

/**
 * IRQ handler function declaration.
 */
#define PORT_IRQ_HANDLER(id) __attribute__((naked)) void id(void)

/**
 * This function is empty in this port.
 */
#define port_init()

/**
 * Disables the IRQ sources and keeps the FIQ sources enabled.
 */
#ifdef THUMB
//#define port_lock() _port_lock_thumb()
#define port_lock() {                                                   \
  asm volatile ("bl     _port_lock_thumb" : : : "r3", "lr");            \
}
#else /* !THUMB */
#define port_lock() asm volatile ("msr     CPSR_c, #0x9F")
#endif /* !THUMB */

/**
 * Enables both the IRQ and FIQ sources.
 */
#ifdef THUMB
//#define port_unlock() _port_unlock_thumb()
#define port_unlock() {                                                 \
  asm volatile ("bl     _port_unlock_thumb" : : : "r3", "lr");          \
}
#else /* !THUMB */
#define port_unlock() asm volatile ("msr     CPSR_c, #0x1F")
#endif /* !THUMB */

/**
 * This function is empty in this port.
 */
#define port_lock_from_isr()

/**
 * This function is empty in this port.
 */
#define port_unlock_from_isr()

/**
 * Disables both the IRQ and FIQ sources.
 * @note Implements a workaround for spurious interrupts taken from the NXP
 *       LPC214x datasheet.
 */
#ifdef THUMB
//#define port_disable() _port_disable_thumb()
#define port_disable() {                                                \
  asm volatile ("bl     _port_disable_thumb" : : : "r3", "lr");         \
}
#else /* !THUMB */
#define port_disable() {                                                \
  asm volatile ("mrs     r3, CPSR                       \n\t"           \
                "orr     r3, #0x80                      \n\t"           \
                "msr     CPSR_c, r3                     \n\t"           \
                "orr     r3, #0x40                      \n\t"           \
                "msr     CPSR_c, r3" : : : "r3");                       \
}
#endif /* !THUMB */

/**
 * Disables the IRQ sources and enables the FIQ sources.
 */
#ifdef THUMB
#define port_suspend() {                                                \
  asm volatile ("bl     _port_suspend_thumb" : : : "r3", "lr");         \
}
#else /* !THUMB */
#define port_suspend() asm volatile ("msr     CPSR_c, #0x9F")
#endif /* !THUMB */

/**
 * Enables both the IRQ and FIQ sources.
 */
#ifdef THUMB
#define port_enable() {                                                 \
  asm volatile ("bl     _port_enable_thumb" : : : "r3", "lr");          \
}
#else /* !THUMB */
#define port_enable() asm volatile ("msr     CPSR_c, #0x1F")
#endif /* !THUMB */

/**
 * Performs a context switch between two threads.
 * @param otp the thread to be switched out
 * @param ntp the thread to be switched in
 */
#ifdef THUMB
#if CH_DBG_ENABLE_STACK_CHECK
#define port_switch(otp, ntp) {                                         \
  register Thread *_otp asm ("r0") = (otp);                             \
  register Thread *_ntp asm ("r1") = (ntp);                             \
  register char *sp asm ("sp");                                         \
  if (sp - sizeof(struct intctx) - sizeof(Thread) < (char *)_otp)       \
    asm volatile ("mov     r0, #0                               \n\t"   \
                  "ldr     r1, =chDbgPanic                      \n\t"   \
                  "bx      r1");                                        \
    _port_switch_thumb(_otp, _ntp);                                     \
}
#else /* !CH_DBG_ENABLE_STACK_CHECK */
#define port_switch(otp, ntp) _port_switch_thumb(otp, ntp)
#endif /* !CH_DBG_ENABLE_STACK_CHECK */
#else /* !THUMB */
#if CH_DBG_ENABLE_STACK_CHECK
#define port_switch(otp, ntp) {                                         \
  register Thread *_otp asm ("r0") = (otp);                             \
  register Thread *_ntp asm ("r1") = (ntp);                             \
  register char *sp asm ("sp");                                         \
  if (sp - sizeof(struct intctx) - sizeof(Thread) < (char *)_otp)       \
    asm volatile ("mov     r0, #0                               \n\t"   \
                  "b       chDbgPanic");                                \
  _port_switch_arm(_otp, _ntp);                                         \
}
#else /* !CH_DBG_ENABLE_STACK_CHECK */
#define port_switch(otp, ntp) _port_switch_arm(otp, ntp)
#endif /* !CH_DBG_ENABLE_STACK_CHECK */
#endif /* !THUMB */

#ifdef __cplusplus
extern "C" {
#endif
  void port_halt(void);
#ifdef THUMB
  void _port_switch_thumb(Thread *otp, Thread *ntp);
#else /* !THUMB */
  void _port_switch_arm(Thread *otp, Thread *ntp);
#endif /* !THUMB */
  void _port_thread_start(void);
#ifdef __cplusplus
}
#endif

#endif /* _CHCORE_H_ */

/** @} */
