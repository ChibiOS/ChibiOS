/*
    ChibiOS/RT - Copyright (C) 2006,2007,2008,2009,2010 Giovanni Di Sirio.

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
 * @file    ARMCMx/chcore_v7m.h
 * @brief   ARMv7-M architecture port macros and structures.
 *
 * @addtogroup ARMCMx_V7M_CORE
 * @{
 */

#ifndef _CHCORE_V7M_H_
#define _CHCORE_V7M_H_

/*===========================================================================*/
/* Port implementation part.                                                 */
/*===========================================================================*/

#if !defined(__DOXYGEN__)
/**
 * @brief   Interrupt saved context.
 * @details This structure represents the stack frame saved during a
 *          preemption-capable interrupt handler.
 * @note    It is implemented to match the Cortex-Mx exception context.
 */
struct extctx {
  regarm_t      r0;
  regarm_t      r1;
  regarm_t      r2;
  regarm_t      r3;
  regarm_t      r12;
  regarm_t      lr_thd;
  regarm_t      pc;
  regarm_t      xpsr;
};
#endif

#if !defined(__DOXYGEN__)
/**
 * @brief   System saved context.
 * @details This structure represents the inner stack frame during a context
 *          switching.
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
#endif

/**
 * @brief   Platform dependent part of the @p chThdInit() API.
 * @details This code usually setup the context switching frame represented
 *          by an @p intctx structure.
 */
#define SETUP_CONTEXT(workspace, wsize, pf, arg) {                          \
  tp->p_ctx.r13 = (struct intctx *)((uint8_t *)workspace +                  \
                                     wsize -                                \
                                     sizeof(struct intctx));                \
  tp->p_ctx.r13->r4 = pf;                                                   \
  tp->p_ctx.r13->r5 = arg;                                                  \
  tp->p_ctx.r13->lr = _port_thread_start;                                   \
}

/**
 * @brief   Stack size for the system idle thread.
 * @details This size depends on the idle thread implementation, usually
 *          the idle thread should take no more space than those reserved
 *          by @p INT_REQUIRED_STACK.
 * @note    In this port it is set to 8 because the idle thread does have
 *          a stack frame when compiling without optimizations. You may
 *          reduce this value to zero when compiling with optimizations.
 */
#ifndef IDLE_THREAD_STACK_SIZE
#define IDLE_THREAD_STACK_SIZE      8
#endif

/**
 * @brief   Per-thread stack overhead for interrupts servicing.
 * @details This constant is used in the calculation of the correct working
 *          area size.
 *          This value can be zero on those architecture where there is a
 *          separate interrupt stack and the stack space between @p intctx and
 *          @p extctx is known to be zero.
 * @note    In this port it is conservatively set to 16 because the function
 *          @p chSchDoRescheduleI() can have a stack frame, expecially with
 *          compiler optimizations disabled.
 */
#ifndef INT_REQUIRED_STACK
#define INT_REQUIRED_STACK          16
#endif

/**
 * @brief   IRQ prologue code.
 * @details This macro must be inserted at the start of all IRQ handlers
 *          enabled to invoke system APIs.
 */
#define PORT_IRQ_PROLOGUE()

/**
 * @brief   IRQ epilogue code.
 * @details This macro must be inserted at the end of all IRQ handlers
 *          enabled to invoke system APIs.
 */
#define PORT_IRQ_EPILOGUE() _port_irq_epilogue()

/**
 * @brief   IRQ handler function declaration.
 * @note    @p id can be a function name or a vector number depending on the
 *          port implementation.
 */
#define PORT_IRQ_HANDLER(id) void id(void)

/**
 * @brief   Fast IRQ handler function declaration.
 * @note    @p id can be a function name or a vector number depending on the
 *          port implementation.
 */
#define PORT_FAST_IRQ_HANDLER(id) void id(void)

/**
 * @brief   Port-related initialization code.
 */
#define port_init() {                                                       \
  SCB_AIRCR = AIRCR_VECTKEY | AIRCR_PRIGROUP(0);                            \
  NVICSetSystemHandlerPriority(HANDLER_SVCALL,                              \
    CORTEX_PRIORITY_MASK(CORTEX_PRIORITY_SVCALL));                          \
  NVICSetSystemHandlerPriority(HANDLER_SYSTICK,                             \
    CORTEX_PRIORITY_MASK(CORTEX_PRIORITY_SYSTICK));                         \
}

/**
 * @brief   Kernel-lock action.
 * @details Usually this function just disables interrupts but may perform
 *          more actions.
 * @note    In this port this it raises the base priority to kernel level.
 */
#if CH_OPTIMIZE_SPEED
#define port_lock() {                                                       \
  register uint32_t tmp asm ("r3") = CORTEX_BASEPRI_KERNEL;                 \
  asm volatile ("msr     BASEPRI, %0" : : "r" (tmp));                       \
}
#else
#define port_lock() {                                                       \
  asm volatile ("bl      _port_lock" : : : "r3", "lr");                     \
}
#endif

/**
 * @brief   Kernel-unlock action.
 * @details Usually this function just disables interrupts but may perform
 *          more actions.
 * @note    In this port this it lowers the base priority to user level.
 */
#if CH_OPTIMIZE_SPEED
#define port_unlock() {                                                     \
  register uint32_t tmp asm ("r3") = CORTEX_BASEPRI_DISABLED;               \
  asm volatile ("msr     BASEPRI, %0" : : "r" (tmp));                       \
}
#else
#define port_unlock() {                                                     \
  asm volatile ("bl      _port_unlock" : : : "r3", "lr");                   \
}
#endif

/**
 * @brief   Kernel-lock action from an interrupt handler.
 * @details This function is invoked before invoking I-class APIs from
 *          interrupt handlers. The implementation is architecture dependent,
 *          in its simplest form it is void.
 * @note    Same as @p port_lock() in this port.
 */
#define port_lock_from_isr() port_lock()

/**
 * @brief   Kernel-unlock action from an interrupt handler.
 * @details This function is invoked after invoking I-class APIs from interrupt
 *          handlers. The implementation is architecture dependent, in its
 *          simplest form it is void.
 * @note    Same as @p port_unlock() in this port.
 */
#define port_unlock_from_isr() port_unlock()

/**
 * @brief   Disables all the interrupt sources.
 * @note    Of course non maskable interrupt sources are not included.
 * @note    In this port it disables all the interrupt sources by raising
 *          the priority mask to level 0.
 */
#define port_disable() asm volatile ("cpsid   i")

/**
 * @brief   Disables the interrupt sources below kernel-level priority.
 * @note    Interrupt sources above kernel level remains enabled.
 * @note    In this port it raises/lowers the base priority to kernel level.
 */
#define port_suspend() {                                                    \
  register uint32_t tmp asm ("r3") = CORTEX_BASEPRI_KERNEL;                 \
  asm volatile ("msr     BASEPRI, %0                    \n\t"               \
                "cpsie   i" : : "r" (tmp));                                 \
}

/**
 * @brief   Enables all the interrupt sources.
 * @note    In this port it lowers the base priority to user level.
 */
#define port_enable() {                                                     \
  register uint32_t tmp asm ("r3") = CORTEX_BASEPRI_DISABLED;               \
  asm volatile ("msr     BASEPRI, %0                    \n\t"               \
                "cpsie   i" : : "r" (tmp));                                 \
}

/**
 * @brief   Enters an architecture-dependent IRQ-waiting mode.
 * @details The function is meant to return when an interrupt becomes pending.
 *          The simplest implementation is an empty function or macro but this
 *          would not take advantage of architecture-specific power saving
 *          modes.
 * @note    Implemented as an inlined @p WFI instruction.
 */
#if CORTEX_ENABLE_WFI_IDLE || defined(__DOXYGEN__)
#define port_wait_for_interrupt() {                                         \
  asm volatile ("wfi");                                                     \
}
#else
#define port_wait_for_interrupt()
#endif

#ifdef __cplusplus
extern "C" {
#endif
  void port_halt(void);
  void port_switch(Thread *ntp, Thread *otp);
  void _port_irq_epilogue(void);
  void _port_switch_from_isr(void);
  void _port_thread_start(void);
#if !CH_OPTIMIZE_SPEED
  void _port_lock(void);
  void _port_unlock(void);
#endif
#ifdef __cplusplus
}
#endif

#endif /* _CHCORE_V7M_H_ */

/** @} */
