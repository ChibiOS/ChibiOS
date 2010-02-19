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
 * @file    PPCE200Z/chcore.h
 * @brief   PowerPC E200Zx architecture port macros and structures.
 *
 * @addtogroup PPCE200Z_CORE
 * @{
 */

#ifndef _CHCORE_H_
#define _CHCORE_H_

/*
 * Port-related configuration parameters.
 */

/**
 * @brief   Enables the use of the WFI ins.
 */
#ifndef ENABLE_WFI_IDLE
#define ENABLE_WFI_IDLE         0
#endif

/**
 * @brief   Unique macro for the implemented architecture.
 */
#define CH_ARCHITECTURE_PPCE200Z

/**
 * @brief   Name of the implemented architecture.
 */
#define CH_ARCHITECTURE_NAME "PPC E200Zx"

/**
 * @brief   Base type for stack alignment.
 * @details This type is used only for stack alignment reasons thus can be
 *          anything from a char to a double.
 */
typedef uint64_t stkalign_t;

/**
 * @brief   Generic PPC register.
 */
typedef void *regppc_t;

/**
 * @brief   Mandatory part of a stack frame.
 */
struct eabi_frame {
  regppc_t      slink;          /**< Stack back link.                       */
  regppc_t      shole;          /**< Stack hole for LR storage.             */
};

/**
 * @brief   Interrupt saved context.
 * @details This structure represents the stack frame saved during a
 *          preemption-capable interrupt handler.
 * @note    R2 and R13 are not saved because those are assumed to be immutable
 *          during the system life cycle.
 */
struct extctx {
  struct eabi_frame frame;
  /* Start of the e_stmvsrrw frame (offset 8).*/
  regppc_t      pc;
  regppc_t      msr;
  /* Start of the e_stmvsprw frame (offset 16).*/
  regppc_t      cr;
  regppc_t      lr;
  regppc_t      ctr;
  regppc_t      xer;
  /* Start of the e_stmvgprw frame (offset 32).*/
  regppc_t      r0;
  regppc_t      r3;
  regppc_t      r4;
  regppc_t      r5;
  regppc_t      r6;
  regppc_t      r7;
  regppc_t      r8;
  regppc_t      r9;
  regppc_t      r10;
  regppc_t      r11;
  regppc_t      r12;
  regppc_t      padding;
 };

/**
 * @brief   System saved context.
 * @details This structure represents the inner stack frame during a context
 *          switching.
 * @note    R2 and R13 are not saved because those are assumed to be immutable
 *          during the system life cycle.
 * @note    LR is stored in the caller contex so it is not present in this
 *          structure.
 */
struct intctx {
  regppc_t      cr;                 /* Part of it is not volatile...        */
  regppc_t      r14;
  regppc_t      r15;
  regppc_t      r16;
  regppc_t      r17;
  regppc_t      r18;
  regppc_t      r19;
  regppc_t      r20;
  regppc_t      r21;
  regppc_t      r22;
  regppc_t      r23;
  regppc_t      r24;
  regppc_t      r25;
  regppc_t      r26;
  regppc_t      r27;
  regppc_t      r28;
  regppc_t      r29;
  regppc_t      r30;
  regppc_t      r31;
  regppc_t      padding;
};

/**
 * @brief   Platform dependent part of the @p Thread structure.
 * @details This structure usually contains just the saved stack pointer
 *          defined as a pointer to a @p intctx structure.
 */
struct context {
  struct intctx *sp;
};

/**
 * @brief   Platform dependent part of the @p chThdInit() API.
 * @details This code usually setup the context switching frame represented
 *          by an @p intctx structure.
 */
#define SETUP_CONTEXT(workspace, wsize, pf, arg) {                          \
  uint8_t *sp = (uint8_t *)workspace + wsize - sizeof(struct eabi_frame);   \
  ((struct eabi_frame *)sp)->slink = 0;                                     \
  ((struct eabi_frame *)sp)->shole = _port_thread_start;                    \
  tp->p_ctx.sp = (struct intctx *)(sp - sizeof(struct intctx));             \
  tp->p_ctx.sp->r31 = arg;                                                  \
  tp->p_ctx.sp->r30 = pf;                                                   \
}

/**
 * @brief   Stack size for the system idle thread.
 * @details This size depends on the idle thread implementation, usually
 *          the idle thread should take no more space than those reserved
 *          by @p INT_REQUIRED_STACK.
 */
#ifndef IDLE_THREAD_STACK_SIZE
#define IDLE_THREAD_STACK_SIZE          0
#endif

/**
 * @brief   Per-thread stack overhead for interrupts servicing.
 * @details This constant is used in the calculation of the correct working
 *          area size.
 *          This value can be zero on those architecture where there is a
 *          separate interrupt stack and the stack space between @p intctx and
 *          @p extctx is known to be zero.
 */
#ifndef INT_REQUIRED_STACK
#define INT_REQUIRED_STACK              128
#endif

/**
 * @brief   Enforces a correct alignment for a stack area size value.
 */
#define STACK_ALIGN(n) ((((n) - 1) | (sizeof(stkalign_t) - 1)) + 1)

/**
 * @brief   Computes the thread working area global size.
 */
#define THD_WA_SIZE(n) STACK_ALIGN(sizeof(Thread) +                     \
                                   sizeof(struct intctx) +              \
                                   sizeof(struct extctx) +              \
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
#define PORT_IRQ_PROLOGUE()

/**
 * @brief   IRQ epilogue code.
 * @details This macro must be inserted at the end of all IRQ handlers
 *          enabled to invoke system APIs.
 */
#define PORT_IRQ_EPILOGUE()

/**
 * @brief   IRQ handler function declaration.
 * @note    @p id can be a function name or a vector number depending on the
 *          port implementation.
 */
#define PORT_IRQ_HANDLER(id) void id(void)

/**
 * @brief   Kernel port layer initialization.
 * @details IVPR4 and IVPR10 initialization, INTC_IACKR_PRC0 initialization.
 */
#define port_init() {                                                       \
	asm volatile ("li          %r3, IVOR4@l         \t\n"                   \
  	              "mtIVOR4     %r3                  \t\n"                   \
                  "li          %r3, IVOR10@l        \t\n"                   \
  	              "mtIVOR10    %r3");                                       \
}

/**
 * @details Implemented as global interrupt disable.
 */
#define port_lock() asm ("wrteei  0")

/**
 * @details Implemented as global interrupt enable.
 */
#define port_unlock() asm ("wrteei  1")

/**
 * @details Implemented as global interrupt disable.
 */
#define port_lock_from_isr() /*asm ("wrteei  0")*/

/**
 * @details Implemented as global interrupt enable.
 */
#define port_unlock_from_isr() /*asm ("wrteei  1")*/

/**
 * @details Implemented as global interrupt disable.
 */
#define port_disable() asm ("wrteei  0")

/**
 * @details Same as @p port_disable() in this port, there is no difference
 *          between the two states.
 */
#define port_suspend() asm ("wrteei  0")

/**
 * @details Implemented as global interrupt enable.
 */
#define port_enable() asm ("wrteei  1")

/**
 * @details This port function is implemented as inlined code for performance
 *          reasons.
 */
#if ENABLE_WFI_IDLE != 0
#ifndef port_wait_for_interrupt
#define port_wait_for_interrupt() {                                     \
  asm ("wait");                                                         \
}
#endif
#else
#define port_wait_for_interrupt()
#endif

#ifdef __cplusplus
extern "C" {
#endif
  void port_halt(void);
  void port_switch(Thread *otp, Thread *ntp);
  void _port_thread_start(void);
#ifdef __cplusplus
}
#endif

#endif /* _CHCORE_H_ */

/** @} */
