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
 * @file    ARMCMx/chcore.h
 * @brief   ARM Cortex-Mx architecture port macros and structures.
 *
 * @addtogroup ARMCMx_CORE
 * @{
 */

#ifndef _CHCORE_H_
#define _CHCORE_H_

#include "nvic.h"

/*===========================================================================*/
/* Port constants.                                                           */
/*===========================================================================*/

/**
 * @brief   Port implementing a process mode context switching.
 * @details This macro can be used to differentiate this port from the other
 *          Cortex-Mx port which defines @p CORTEX_PORT_MODE_ENDOSWITCH.
 */
#define CORTEX_PORT_MODE_EXOSWITCH

#define CORTEX_M0               0       /**< @brief Cortex-M0 variant.      */
#define CORTEX_M1               1       /**< @brief Cortex-M1 variant.      */
#define CORTEX_M3               3       /**< @brief Cortex-M3 variant.      */
#define CORTEX_M4               4       /**< @brief Cortex-M4 variant.      */

/* Inclusion of the Cortex-Mx implementation specific parameters.*/
#include "cmparams.h"

/* Cortex model check, only M0 and M3 right now.*/
#if (CORTEX_MODEL == CORTEX_M0) || (CORTEX_MODEL == CORTEX_M3)
#else
#error "unknown or unsupported Cortex-M model"
#endif

/*===========================================================================*/
/* Port derived parameters.                                                  */
/*===========================================================================*/

/**
 * @brief   Priority masking support.
 */
#if defined(CH_ARCHITECTURE_ARM_v7M) || defined(__DOXYGEN__)
#define CORTEX_SUPPORTS_BASEPRI TRUE
#else
#define CORTEX_SUPPORTS_BASEPRI FALSE
#endif

/**
 * @brief   Total priority levels.
 */
#define CORTEX_PRIORITY_LEVELS  (1 << CORTEX_PRIORITY_BITS)

/**
 * @brief   Minimum priority level.
 * @details This minimum priority level is calculated from the number of
 *          priority bits supported by the specific Cortex-Mx implementation.
 */
#define CORTEX_MINIMUM_PRIORITY (CORTEX_PRIORITY_LEVELS - 1)

/**
 * @brief   Maximum priority level.
 * @details The maximum allowed priority level is always zero.
 */
#define CORTEX_MAXIMUM_PRIORITY 0

/*===========================================================================*/
/* Port macros.                                                              */
/*===========================================================================*/

/**
 * @brief   Priority level verification macro.
 */
#define CORTEX_IS_VALID_PRIORITY(n)                                         \
  (((n) >= 0) && ((n) < CORTEX_PRIORITY_LEVELS))

/**
 * @brief   Priority level to priority mask conversion macro.
 */
#define CORTEX_PRIORITY_MASK(n) ((n) << (8 - CORTEX_PRIORITY_BITS))

/*===========================================================================*/
/* Port configurable parameters.                                             */
/*===========================================================================*/

/**
 * @brief   Enables the use of the WFI instruction in the idle thread loop.
 */
#ifndef CORTEX_ENABLE_WFI_IDLE
#define CORTEX_ENABLE_WFI_IDLE  FALSE
#endif

/**
 * @brief   SYSTICK handler priority.
 * @note    The default is calculated as the priority level in the middle
 *          of the priority range.
 */
#ifndef CORTEX_PRIORITY_SYSTICK
#define CORTEX_PRIORITY_SYSTICK (CORTEX_PRIORITY_LEVELS >> 1)
#else
/* If it is externally redefined then better perform a validity check on it.*/
#if !CORTEX_IS_VALID_PRIORITY(CORTEX_PRIORITY_SYSTICK)
#error "invalid priority level specified for CORTEX_PRIORITY_SYSTICK"
#endif
#endif

/*===========================================================================*/
/* Port exported info.                                                       */
/*===========================================================================*/

#if defined(__DOXYGEN__)
/**
 * @brief   Macro defining the ARM architecture.
 */
#define CH_ARCHITECTURE_ARM_vxm

/**
 * @brief   Name of the implemented architecture.
 */
#define CH_ARCHITECTURE_NAME    "ARMvx-M"

/**
 * @brief   Name of the architecture variant (optional).
 */
#define CH_CORE_VARIANT_NAME    "Cortex-Mx"
#elif CORTEX_MODEL == CORTEX_M4
#define CH_ARCHITECTURE_ARM_v7M
#define CH_ARCHITECTURE_NAME    "ARMv7-M"
#define CH_CORE_VARIANT_NAME    "Cortex-M4"
#elif CORTEX_MODEL == CORTEX_M3
#define CH_ARCHITECTURE_ARM_v7M
#define CH_ARCHITECTURE_NAME    "ARMv7-M"
#define CH_CORE_VARIANT_NAME    "Cortex-M3"
#elif CORTEX_MODEL == CORTEX_M1
#define CH_ARCHITECTURE_ARM_v6M
#define CH_ARCHITECTURE_NAME    "ARMv6-M"
#define CH_CORE_VARIANT_NAME    "Cortex-M1"
#elif CORTEX_MODEL == CORTEX_M0
#define CH_ARCHITECTURE_ARM_v6M
#define CH_ARCHITECTURE_NAME    "ARMv6-M"
#define CH_CORE_VARIANT_NAME    "Cortex-M0"
#endif

/*===========================================================================*/
/* Port implementation part.                                                 */
/*===========================================================================*/

/**
 * @brief   32 bits stack and memory alignment enforcement.
 */
typedef uint32_t stkalign_t;

/**
 * @brief   Generic ARM register.
 */
typedef void *regarm_t;

/**
 * @brief   Cortex-Mx exception context.
 */
struct cmxctx {
  regarm_t      r0;
  regarm_t      r1;
  regarm_t      r2;
  regarm_t      r3;
  regarm_t      r12;
  regarm_t      lr_thd;
  regarm_t      pc;
  regarm_t      xpsr;
};

#if !defined(__DOXYGEN__)
/**
 * @brief   Interrupt saved context.
 * @details This structure represents the stack frame saved during a
 *          preemption-capable interrupt handler.
 */
struct extctx {
  regarm_t      r12;
  regarm_t      xpsr;
  regarm_t      lr;
  regarm_t      r0;
  regarm_t      r1;
  regarm_t      r2;
  regarm_t      r3;
  regarm_t      pc;
};
#endif

#if !defined(__DOXYGEN__)
/**
 * @brief   System saved context.
 * @details This structure represents the inner stack frame during a context
 *          switching.
 */
#if defined(CH_ARCHITECTURE_ARM_v6M)
struct intctx {
  regarm_t      r8;
  regarm_t      r9;
  regarm_t      r10;
  regarm_t      r11;
  regarm_t      r4;
  regarm_t      r5;
  regarm_t      r6;
  regarm_t      r7;
  regarm_t      lr;
};
#elif defined(CH_ARCHITECTURE_ARM_v7M)
struct intctx {
  regarm_t      r4;
  regarm_t      r5;
  regarm_t      r6;
  regarm_t      r7;
  regarm_t      r8;
  regarm_t      r9;
  regarm_t      r10;
  regarm_t      r11;
  regarm_t      lr;
};
#endif
#endif

#if !defined(__DOXYGEN__)
/**
 * @brief   Platform dependent part of the @p Thread structure.
 * @details In the Cortex-Mx port this structure just holds a pointer to the
 *          @p intctx structure representing the stack pointer at the time
 *          of the context switch.
 */
struct context {
  struct intctx *r13;
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
 * @note    In this port it is set to 4 because the idle thread does have
 *          a stack frame when compiling without optimizations.
 */
#ifndef IDLE_THREAD_STACK_SIZE
#define IDLE_THREAD_STACK_SIZE      4
#endif

/**
 * @brief   Per-thread stack overhead for interrupts servicing.
 * @details This constant is used in the calculation of the correct working
 *          area size.
 *          This value can be zero on those architecture where there is a
 *          separate interrupt stack and the stack space between @p intctx and
 *          @p extctx is known to be zero.
 * @note    This port requires no extra stack space for interrupt handling.
 */
#ifndef INT_REQUIRED_STACK
#define INT_REQUIRED_STACK          0
#endif

/**
 * @brief   Enforces a correct alignment for a stack area size value.
 */
#define STACK_ALIGN(n) ((((n) - 1) | (sizeof(stkalign_t) - 1)) + 1)

/**
 * @brief   Computes the thread working area global size.
 */
#define THD_WA_SIZE(n) STACK_ALIGN(sizeof(Thread) +                         \
                                   sizeof(struct intctx) +                  \
                                   sizeof(struct extctx) +                  \
                                   (n) + (INT_REQUIRED_STACK))

/**
 * @brief   Static working area allocation.
 * @details This macro is used to allocate a static thread working area
 *          aligned as both position and size.
 */
#define WORKING_AREA(s, n) stkalign_t s[THD_WA_SIZE(n) / sizeof(stkalign_t)];

/**
 * @brief   IRQ prologue code.
 * @details This macro must be inserted at the start of all IRQ handlers
 *          enabled to invoke system APIs.
 */
#define PORT_IRQ_PROLOGUE() {                                               \
  chSysLockFromIsr();                                                       \
  _port_irq_nesting++;                                                      \
  chSysUnlockFromIsr();                                                     \
}

/**
 * @brief   IRQ epilogue code.
 * @details This macro must be inserted at the end of all IRQ handlers
 *          enabled to invoke system APIs.
 */
#define PORT_IRQ_EPILOGUE() {                                               \
  chSysLockFromIsr();                                                       \
  if ((--_port_irq_nesting == 0) && chSchIsRescRequiredExI()) {             \
    register struct cmxctx *ctxp asm ("r3");                                \
                                                                            \
    asm volatile ("mrs     %0, PSP" : "=r" (ctxp) : "r" (ctxp));            \
    _port_saved_pc = ctxp->pc;                                              \
    ctxp->pc = _port_switch_from_irq;                                       \
    return;                                                                 \
  }                                                                         \
  chSysUnlockFromIsr();                                                     \
}

/**
 * @brief   IRQ handler function declaration.
 * @note    @p id can be a function name or a vector number depending on the
 *          port implementation.
 */
#define PORT_IRQ_HANDLER(id) void id(void)

/**
 * @brief   Port-related initialization code.
 */
#define port_init() {                                                       \
  _port_irq_nesting = 0;                                                    \
  SCB_AIRCR = AIRCR_VECTKEY | AIRCR_PRIGROUP(0);                            \
  NVICSetSystemHandlerPriority(HANDLER_SYSTICK,                             \
    CORTEX_PRIORITY_MASK(CORTEX_PRIORITY_SYSTICK));                         \
}

/**
 * @brief   Kernel-lock action.
 * @details Usually this function just disables interrupts but may perform
 *          more actions.
 * @note    In this port it disables all the interrupt sources.
 */
#define port_lock() asm volatile ("cpsid   i")

/**
 * @brief   Kernel-unlock action.
 * @details Usually this function just disables interrupts but may perform
 *          more actions.
 * @note    In this port it enables all the interrupt sources.
 */
#define port_unlock() asm volatile ("cpsie   i")

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
 * @note    In this port it disables all the interrupt sources.
 */
#define port_disable() asm volatile ("cpsid   i")

/**
 * @brief   Disables the interrupt sources below kernel-level priority.
 * @note    Interrupt sources above kernel level remains enabled.
 * @note    In this port it disables all the interrupt sources.
 */
#define port_suspend() asm volatile ("cpsid   i")

/**
 * @brief   Enables all the interrupt sources.
 * @note    In this port it enables all the interrupt sources.
 */
#define port_enable() asm volatile ("cpsie   i")

/**
 * @brief   Enters an architecture-dependent IRQ-waiting mode.
 * @details The function is meant to return when an interrupt becomes pending.
 *          The simplest implementation is an empty function or macro but this
 *          would not take advantage of architecture-specific power saving
 *          modes.
 * @note    Implemented as an inlined @p WFI instruction.
 */
#if CORTEX_ENABLE_WFI_IDLE || defined(__DOXYGEN__)
#define port_wait_for_interrupt() asm volatile ("wfi")
#else
#define port_wait_for_interrupt()
#endif

#if !defined(__DOXYGEN__)
extern regarm_t _port_saved_pc;
extern unsigned _port_irq_nesting;
#endif

#ifdef __cplusplus
extern "C" {
#endif
  void port_halt(void);
  void port_switch(Thread *ntp, Thread *otp);
  void _port_switch_from_irq(void);
  void _port_thread_start(void);
#ifdef __cplusplus
}
#endif

#endif /* _CHCORE_H_ */

/** @} */
