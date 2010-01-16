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
 * @file templates/chcore.h
 * @brief Port related template macros and structures.
 * @addtogroup core
 * @{
 */

#ifndef _CHCORE_H_
#define _CHCORE_H_

/**
 * Unique macro for the implemented architecture.
 */
#define CH_ARCHITECTURE_XXX

/**
 * Name of the implemented architecture.
 */
#define CH_ARCHITECTURE_NAME ""

/**
 * Base type for stack alignment.
 * This type is used only for stack alignment reasons thus can be anything from
 * a char to a double.
 */
typedef uint8_t stkalign_t;

/**
 * @brief Interrupt saved context.
 * @details This structure represents the stack frame saved during a
 * preemption-capable interrupt handler.
 */
struct extctx {
};

/**
 * @brief System saved context.
 * @details This structure represents the inner stack frame during a context
 * switching.
 */
struct intctx {
};

/**
 * @brief Platform dependent part of the @p Thread structure.
 * @details This structure usually contains just the saved stack pointer
 * defined as a pointer to a @p intctx structure.
 */
struct context {
  struct intctx *sp;
};

/**
 * Platform dependent part of the @p chThdInit() API.
 * This code usually setup the context switching frame represented by a
 * @p intctx structure.
 */
#define SETUP_CONTEXT(workspace, wsize, pf, arg) {                      \
}

/**
 * Stack size for the system idle thread.
 * This size depends on the idle thread implementation, usually the idle
 * thread should take no more space than those reserved
 * by @p INT_REQUIRED_STACK.
 */
#ifndef IDLE_THREAD_STACK_SIZE
#define IDLE_THREAD_STACK_SIZE 0
#endif

/**
 * Per-thread stack overhead for interrupts servicing, it is used in the
 * calculation of the correct working area size.
 * This value can be zero on those architecture where there is a separate
 * interrupt stack and the stack space between @p intctx and @p extctx is
 * known to be zero.
 */
#ifndef INT_REQUIRED_STACK
#define INT_REQUIRED_STACK 0
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
 */
#define PORT_IRQ_PROLOGUE()

/**
 * IRQ epilogue code, inserted at the end of all IRQ handlers enabled to
 * invoke system APIs.
 */
#define PORT_IRQ_EPILOGUE()

/**
 * IRQ handler function declaration.
 * @note @p id can be a function name or a vector number depending on the
 *       port implementation.
 */
#define PORT_IRQ_HANDLER(id) void id(void)

#ifdef __cplusplus
extern "C" {
#endif
  void port_init(void);
  void port_lock(void);
  void port_unlock(void);
  void port_lock_from_isr(void);
  void port_unlock_from_isr(void);
  void port_disable(void);
  void port_suspend(void);
  void port_enable(void);
  void port_wait_for_interrupt(void);
  void port_halt(void);
  void port_switch(Thread *otp, Thread *ntp);
#ifdef __cplusplus
}
#endif

#endif /* _CHCORE_H_ */

/** @} */
