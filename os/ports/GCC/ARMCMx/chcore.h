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
 * @brief   ARM Cortex-Mx port macros and structures.
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

/* Cortex model check, only M0 and M3 supported right now.*/
#if (CORTEX_MODEL == CORTEX_M0) || (CORTEX_MODEL == CORTEX_M3)
#elif (CORTEX_MODEL == CORTEX_M1) || (CORTEX_MODEL == CORTEX_M4)
#warning "untested Cortex-M model"
#else
#error "unknown or unsupported Cortex-M model"
#endif

/*===========================================================================*/
/* Port statically derived parameters.                                       */
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
 * @note    The default priority is calculated as the priority level in
 *          the middle of the numeric priorities range.
 */
#ifndef CORTEX_PRIORITY_SYSTICK
#define CORTEX_PRIORITY_SYSTICK (CORTEX_PRIORITY_LEVELS >> 1)
#else
/* If it is externally redefined then better perform a validity check on it.*/
#if !CORTEX_IS_VALID_PRIORITY(CORTEX_PRIORITY_SYSTICK)
#error "invalid priority level specified for CORTEX_PRIORITY_SYSTICK"
#endif
#endif

/**
 * @brief   Priority masking support.
 * @details The ARMv7-M architecture is capable to mask only interrupt
 *          priorities below or equal to a certain specified priority
 *          mask. If this option is enabled all the priorities above
 *          @p CORTEX_BASEPRI_KERNEL (lower numeric values) are not
 *          affected by the kernel locks and can operate with minimum
 *          latency.<br>
 *          This option makes the kernel code a bit larger and slower, if
 *          your application does not need fast interrups it is recommended
 *          to keep this option disabled.
 */
#if CORTEX_SUPPORTS_BASEPRI || defined(__DOXYGEN__)
#if !defined(CORTEX_USE_BASEPRI) || defined(__DOXYGEN__)
#define CORTEX_USE_BASEPRI      FALSE
#endif /* !defined(CORTEX_USE_BASEPRI) */
#else /* !CORTEX_SUPPORTS_BASEPRI */
#if defined(CORTEX_USE_BASEPRI) && CORTEX_USE_BASEPRI
#error "BASEPRI priority masking register not supported in this architecture"
#endif
#define CORTEX_USE_BASEPRI      FALSE
#endif /* !CORTEX_SUPPORTS_BASEPRI */

#if CORTEX_USE_BASEPRI || defined(__DOXYGEN__)
/**
 * @brief   BASEPRI user level.
 * @note    This constant is defined only if the @p CORTEX_USE_BASEPRI port
 *          option is enabled.
 */
#ifndef CORTEX_BASEPRI_USER
#define CORTEX_BASEPRI_USER     CORTEX_PRIORITY_MASK(0)
#endif

/**
 * @brief   BASEPRI level within kernel lock.
 * @details Priority levels higher than this one (lower numeric values) are
 *          unaffected by kernel locks and can be classified as fast
 *          interrupt sources, see @ref interrupt_classes.
 * @note    This constant is defined only if the @p CORTEX_USE_BASEPRI port
 *          option is enabled.
 * @note    The default setting reserves just the highest priority level
 *          (@p CORTEX_MAXIMUM_PRIORITY) for fast interrupts, you may redefine
 *          this setting in order to reserve more levels.
 */
#ifndef CORTEX_BASEPRI_KERNEL
#define CORTEX_BASEPRI_KERNEL   CORTEX_PRIORITY_MASK(CORTEX_MAXIMUM_PRIORITY+1)
#endif
#endif /* CORTEX_USE_BASEPRI */

/*===========================================================================*/
/* Port exported info.                                                       */
/*===========================================================================*/

/**
 * @brief   Macro defining a generic ARM architecture.
 */
#define CH_ARCHITECTURE_ARM

#if defined(__DOXYGEN__)
/**
 * @brief   Macro defining the specific ARM architecture.
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
#define CH_ARCHITECTURE_NAME    "ARMv7-ME"
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
  regarm_t      xpsr;
  regarm_t      r12;
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
 * @note    This port requires some extra stack space for interrupt handling
 *          representing the frame of the function @p chSchDoRescheduleI().
 */
#ifndef INT_REQUIRED_STACK
#define INT_REQUIRED_STACK          8
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
 */
#if CORTEX_USE_BASEPRI
#define port_lock() {                                                       \
  register uint32_t tmp asm ("r3") = CORTEX_BASEPRI_KERNEL;                 \
  asm volatile ("msr     BASEPRI, %0" : : "r" (tmp));                       \
}
#else /* !CORTEX_USE_BASEPRI */
#define port_lock() asm volatile ("cpsid   i")
#endif /* !CORTEX_USE_BASEPRI */

/**
 * @brief   Kernel-unlock action.
 * @details Usually this function just disables interrupts but may perform
 *          more actions.
 */
#if CORTEX_USE_BASEPRI
#define port_unlock() {                                                     \
  register uint32_t tmp asm ("r3") = CORTEX_BASEPRI_USER;                   \
  asm volatile ("msr     BASEPRI, %0" : : "r" (tmp));                       \
}
#else /* !CORTEX_USE_BASEPRI */
#define port_unlock() asm volatile ("cpsie   i")
#endif /* !CORTEX_USE_BASEPRI */

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
 * @note    Same as @p port_lock() in this port.
 */
#define port_unlock_from_isr() port_unlock()

/**
 * @brief   Disables all the interrupt sources.
 */
#define port_disable() asm volatile ("cpsid   i")

/**
 * @brief   Disables the interrupt sources below kernel-level priority.
 */
#if CORTEX_USE_BASEPRI
#define port_suspend() {                                                    \
  register uint32_t tmp asm ("r3") = CORTEX_BASEPRI_KERNEL;                 \
  asm volatile ("msr     BASEPRI, %0                    \n\t"               \
                "cpsie   i" : : "r" (tmp));                                 \
}
#else /* !CORTEX_USE_BASEPRI */
#define port_suspend() asm volatile ("cpsid   i")
#endif /* !CORTEX_USE_BASEPRI */

/**
 * @brief   Enables all the interrupt sources.
 */
#if CORTEX_USE_BASEPRI
#define port_enable() {                                                     \
  register uint32_t tmp asm ("r3") = CORTEX_BASEPRI_USER;                   \
  asm volatile ("msr     BASEPRI, %0                    \n\t"               \
                "cpsie   i" : : "r" (tmp));                                 \
}
#else /* !CORTEX_USE_BASEPRI */
#define port_enable() asm volatile ("cpsie   i")
#endif /* !CORTEX_USE_BASEPRI */

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
