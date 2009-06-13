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

/**
 * @file ports/MSP430/chcore.h
 * @brief MSP430 architecture port macros and structures.
 * @addtogroup MSP430_CORE
 * @{
 */

#ifndef _CHCORE_H_
#define _CHCORE_H_

#include <iomacros.h>
#include <msp430/common.h>

/**
 * If enabled allows the idle thread to enter a low power mode.
 */
#ifndef ENABLE_WFI_IDLE
#define ENABLE_WFI_IDLE 0
#endif

/**
 * Macro defining the MSP430 architecture.
 */
#define CH_ARCHITECTURE_MSP430

/**
 * 16 bit stack alignment.
 */
typedef uint16_t stkalign_t;

/**
 * Generic MSP430 register.
 */
typedef void *regmsp_t;

/** @cond never */
/**
 * Interrupt saved context.
 */
struct extctx {
  regmsp_t      r12;
  regmsp_t      r13;
  regmsp_t      r14;
  regmsp_t      r15;
  regmsp_t      sr;
  regmsp_t      pc;
};
/** @endcond */

/** @cond never */
/**
 * This structure represents the inner stack frame during a context switching.
 */
struct intctx {
  regmsp_t      r4;
  regmsp_t      r5;
  regmsp_t      r6;
  regmsp_t      r7;
  regmsp_t      r8;
  regmsp_t      r9;
  regmsp_t      r10;
  regmsp_t      r11;
  regmsp_t      pc;
};
/** @endcond */

/** @cond never */
/**
 * In the MSP430 port this structure just holds a pointer to the @p intctx
 * structure representing the stack pointer at the time of the context switch.
 */
struct context {
  struct intctx *sp;
};
/** @endcond */

/**
 * Platform dependent part of the @p chThdInit() API.
 */
#define SETUP_CONTEXT(workspace, wsize, pf, arg) {                      \
  tp->p_ctx.sp = (struct intctx *)((uint8_t *)workspace +               \
                                   wsize -                              \
                                   sizeof(struct intctx));              \
  tp->p_ctx.sp->r10 = pf;                                               \
  tp->p_ctx.sp->r11 = arg;                                              \
  tp->p_ctx.sp->pc = threadstart;                                       \
}

/**
 * The default idle thread implementation requires no extra stack space in
 * this port.
 */
#ifndef IDLE_THREAD_STACK_SIZE
#define IDLE_THREAD_STACK_SIZE 0
#endif

/**
 * Per-thread stack overhead for interrupts servicing, it is used in the
 * calculation of the correct working area size. In this port the default is
 * 32 bytes per thread.
 */
#ifndef INT_REQUIRED_STACK
#define INT_REQUIRED_STACK 32
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
#define PORT_IRQ_EPILOGUE() {                                           \
  if (chSchRescRequiredI())                                             \
    chSchDoRescheduleI();                                               \
}

/**
 * IRQ handler function modifier.
 */
#define PORT_IRQ_HANDLER(id) interrupt(id) _vect_##id(void)

/**
 * This function is empty in this port.
 */
#define port_init()

/**
 * Implemented as global interrupt disable.
 */
#define port_lock() asm volatile ("dint")

/**
 * Implemented as global interrupt enable.
 */
#define port_unlock() asm volatile ("eint")

/**
 * This function is empty in this port.
 */
#define port_lock_from_isr()

/**
 * This function is empty in this port.
 */
#define port_unlock_from_isr()

/**
 * Implemented as global interrupt disable.
 */
#define port_disable() asm volatile ("dint")

/**
 * Same as @p port_disable() in this port, there is no difference between the
 * two states.
 */
#define port_suspend() asm volatile ("dint")

/**
 * Implemented as global interrupt enable.
 */
#define port_enable() asm volatile ("eint")

/**
 * This port function is implemented as inlined code for performance reasons.
 * @note The port code does not define a low power mode, this macro has to be
 *       defined externally. The default implementation is a "nop", not a
 *       real low power mode.
 */
#if ENABLE_WFI_IDLE != 0
#ifndef port_wait_for_interrupt
#define port_wait_for_interrupt() {                                     \
  asm volatile ("nop");                                                 \
}
#endif
#else
#define port_wait_for_interrupt()
#endif

#ifdef __cplusplus
extern "C" {
#endif
  void port_switch(Thread *otp, Thread *ntp);
  void port_halt(void);
  void threadstart(void);
#ifdef __cplusplus
}
#endif

#endif /* _CHCORE_H_ */

/** @} */
