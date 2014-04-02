/*
    ChibiOS/RT - Copyright (C) 2006,2007,2008,2009,2010,
                 2011,2012,2013 Giovanni Di Sirio.

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

/**
 * @file    RX/chcore.h
 * @brief   Renesas RX port macros and structures.
 *
 * @addtogroup RX_CORE
 * @{
 */

#ifndef _CHCORE_H_
#define _CHCORE_H_

/*===========================================================================*/
/* Port constants.                                                           */
/*===========================================================================*/

/* Inclusion of the RX implementation specific parameters.*/
#include "rxparams.h"

/* RX model check, only RX62N is supported right now.*/
#if (RX_MODEL == RX62N)
#else
#error "unknown or unsupported RX model"
#endif

/*===========================================================================*/
/* Port statically derived parameters.                                       */
/*===========================================================================*/

/*===========================================================================*/
/* Port macros.                                                              */
/*===========================================================================*/

/*===========================================================================*/
/* Port configurable parameters.                                             */
/*===========================================================================*/

/**
 * If enabled allows the idle thread to enter a low power mode.
 */
#ifndef RX_ENABLE_WFI_IDLE
#define RX_ENABLE_WFI_IDLE              FALSE
#endif

/*===========================================================================*/
/* Port exported info.                                                       */
/*===========================================================================*/

/**
 * Macro defining a generic RX architecture.
 */
#define CH_ARCHITECTURE_RX

/**
 * @brief   Name of the implemented architecture.
 * @note    The value is for documentation only, the real value changes
 *          depending on the selected architecture, the possible values are:
 *          - "ARM7".
 *          - "ARM9".
 *          .
 */
#define CH_ARCHITECTURE_NAME            "RX"

/**
 * @brief   Name of the compiler supported by this port.
 */
#define CH_COMPILER_NAME                "GCC " __VERSION__

/*===========================================================================*/
/* Port implementation part (common).                                        */
/*===========================================================================*/

/**
 * @brief   32 bits stack and memory alignment enforcement.
 */
typedef uint32_t stkalign_t;

/**
 * @brief   Generic RX register.
 */
typedef void *regrx_t;

/**
 * @brief Interrupt saved context.
 * @details This structure represents the stack frame saved during a
 * preemption-capable interrupt handler. These registers are caller-saved in GCC.
 */
struct extctx {
#if defined(RX_USE_FPU)
    regrx_t     fpsw;
#endif
    regrx_t     acc_hi;
    regrx_t     acc_lo;
    regrx_t     r1;
    regrx_t     r2;
    regrx_t     r3;
    regrx_t     r4;
    regrx_t     r5;
    regrx_t     r14;
    regrx_t     r15;
    regrx_t     psw;
    regrx_t     pc;
};

/**
 * @brief System saved context.
 * @details This structure represents the inner stack frame during a context
 * switching. These registers are callee-saved in GCC.
 */
struct intctx {
    regrx_t     r6;
    regrx_t     r7;
    regrx_t     r8;
    regrx_t     r9;
    regrx_t     r10;
    regrx_t     r11;
    regrx_t     r12;
#ifndef CH_CURRP_REGISTER_CACHE
/* -mint-register=1 - reserves R13 */
    regrx_t     r13;
#else
#warning CH_CURRP_REGISTER_CACHE feature is not tested.
#endif
    regrx_t     pc;
};

/**
 * @brief   Platform dependent part of the @p Thread structure.
 * @details In this port the structure just holds a pointer to the @p intctx
 *          structure representing the stack pointer at context switch time.
 */
struct context {
  struct intctx *sp;
};

/**
 * @brief   Platform dependent part of the @p chThdCreateI() API.
 * @details This code usually setup the context switching frame represented
 *          by an @p intctx structure.
 */
#define SETUP_CONTEXT(workspace, wsize, pf, arg) {                            \
    tp->p_ctx.sp = (struct intctx *)((uint8_t *)workspace +                   \
                    wsize -    sizeof(struct intctx));                        \
    tp->p_ctx.sp->r11 = arg;                                                  \
    tp->p_ctx.sp->r12 = pf;                                                   \
    tp->p_ctx.sp->pc = _port_thread_start;                                    \
}

/**
 * @brief   Stack size for the system idle thread.
 * @details This size depends on the idle thread implementation, usually
 *          the idle thread should take no more space than those reserved
 *          by @p PORT_INT_REQUIRED_STACK.
 */
#ifndef PORT_IDLE_THREAD_STACK_SIZE
#define PORT_IDLE_THREAD_STACK_SIZE     20
#endif

/**
 * @brief   Per-thread stack overhead for interrupts servicing.
 * @details This constant is used in the calculation of the correct working
 *          area size.
 *          This value can be zero on those architecture where there is a
 *          separate interrupt stack and the stack space between @p intctx and
 *          @p extctx is known to be zero.
 */
#ifndef PORT_INT_REQUIRED_STACK
/*#define PORT_INT_REQUIRED_STACK 64*/
#define PORT_INT_REQUIRED_STACK 256
#endif

/**
 * @brief   Enforces a correct alignment for a stack area size value.
 */
#define STACK_ALIGN(n) ((((n) - 1) | (sizeof(stkalign_t) - 1)) + 1)

/**
 * @brief   Computes the thread working area global size.
 */
#define THD_WA_SIZE(n) STACK_ALIGN(sizeof(Thread) +                           \
                                   sizeof(struct intctx) +                    \
                                   sizeof(struct extctx) +                    \
                                  (n) + (PORT_INT_REQUIRED_STACK))

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
#if !defined(RX_USE_FPU)
#define PORT_IRQ_PROLOGUE() {                                                 \
    asm volatile ("pushm   r14-r15                                  \n\t"     \
                  "pushm   r1-r5                                    \n\t"     \
                  "mvfaclo r1                                       \n\t"     \
                  "push    r1                                       \n\t"     \
                  "mvfachi r1                                       \n\t"     \
                  "push    r1                                       \n\t");   \
}
#else
#define PORT_IRQ_PROLOGUE() {                                                 \
    asm volatile ("pushm   r14-r15                                  \n\t"     \
                  "pushm   r1-r5                                    \n\t"     \
                  "mvfaclo r1                                       \n\t"     \
                  "push    r1                                       \n\t"     \
                  "mvfachi r1                                       \n\t"     \
                  "push    r1                                       \n\t"     \
                  "pushc   FPSW                                     \n\t");   \
}
#endif
/* #define PORT_IRQ_PROLOGUE() */

/**
 * @brief   IRQ epilogue code.
 * @details This macro must be inserted at the end of all IRQ handlers
 *          enabled to invoke system APIs.
 */
#if !defined(RX_USE_FPU)
#define PORT_IRQ_EPILOGUE() {                                                 \
    dbg_check_lock();                                                         \
    if (chSchIsPreemptionRequired()) {                                        \
      asm volatile ("mvtipl  #0                                     \n\t");   \
      chSchDoReschedule();                                                    \
    }                                                                         \
    dbg_check_unlock();                                                       \
    asm volatile ("pop     r1                                       \n\t"     \
                  "mvtachi r1                                       \n\t"     \
                  "pop     r1                                       \n\t"     \
                  "mvtaclo r1                                       \n\t"     \
                  "popm    r1-r5                                    \n\t"     \
                  "popm    r14-r15                                  \n\t"     \
                  "rte                                              \n\t");   \
}
#else
#define PORT_IRQ_EPILOGUE() {                                                 \
    dbg_check_lock();                                                         \
    if (chSchIsPreemptionRequired()) {                                        \
      asm volatile ("mvtipl  #0                                     \n\t");   \
      chSchDoReschedule();                                                    \
    }                                                                         \
    asm volatile ("popc    FPSW                                     \n\t"     \
                  "pop     r1                                       \n\t"     \
                  "mvtachi r1                                       \n\t"     \
                  "pop     r1                                       \n\t"     \
                  "mvtaclo r1                                       \n\t"     \
                  "popm    r1-r5                                    \n\t"     \
                  "popm    r14-r15                                  \n\t"     \
                  "rte                                              \n\t");   \
}
#endif

/**
 * IRQ handler function declaration.
 * @note @p id can be a function name or a vector number depending on the
 *       port implementation.
 */
#define PORT_IRQ_HANDLER(id) void __attribute__((naked)) id(void)

/**
 * @brief Kernel-unlock action.
 * @details Usually this function just disables interrupts but may perform more
 * actions.
 */
#define port_lock() asm volatile ("clrpsw    I                      \n\t")

/**
 * @brief Kernel-unlock action.
 * @details Usually this function just disables interrupts but may perform more
 * actions.
 */
#define port_unlock() asm volatile ("setpsw  I                      \n\t")

/**
 * @brief Kernel-lock action from an interrupt handler.
 * @details This function is invoked before invoking I-class APIs from
 * interrupt handlers. The implementation is architecture dependent, in its
 * simplest form it is void.
 */
#define port_lock_from_isr()

/**
 * @brief Kernel-unlock action from an interrupt handler.
 * @details This function is invoked after invoking I-class APIs from interrupt
 * handlers. The implementation is architecture dependent, in its simplest form
 * it is void.
 */
#define port_unlock_from_isr()

/**
 * @brief   Enters an architecture-dependent IRQ-waiting mode.
 * @details The function is meant to return when an interrupt becomes pending.
 *          The simplest implementation is an empty function or macro but this
 *          would not take advantage of architecture-specific power saving
 *          modes.
 * @note    This port function is implemented as inlined code for performance
 *          reasons.
 */
#if RX_ENABLE_WFI_IDLE == TRUE
#define port_wait_for_interrupt() {                                           \
  asm volatile ("wait");                                                      \
}
#else
#define port_wait_for_interrupt()
#endif


#ifdef __cplusplus
extern "C" {
#endif
  void port_init(void);
  void port_disable(void);
  void port_suspend(void);
  void port_enable(void);
  bool_t port_enabled(void);
  void port_halt(void);
  void port_switch(Thread *ntp, Thread *otp);
  void _port_thread_start(void);
#ifdef __cplusplus
}
#endif

#endif /* _CHCORE_H_ */

/** @} */
