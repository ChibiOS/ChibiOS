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
 * @addtogroup AVR_CORE
 * @{
 */

#ifndef _CHCORE_H_
#define _CHCORE_H_

#include <avr/io.h>
#include <avr/interrupt.h>

/**
 * If enabled allows the idle thread to enter a low power mode.
 */
#ifndef ENABLE_WFI_IDLE
#define ENABLE_WFI_IDLE 0
#endif

/**
 * Macro defining the AVR architecture.
 */
#define CH_ARCHITECTURE_AVR

/**
 * 8 bit stack alignment.
 */
typedef uint8_t stkalign_t;

/**
 * Interrupt saved context.
 * @note The field @p _next is not part of the context, it represents the
 *       offset of the structure relative to the stack pointer.
 */
struct extctx {
  uint8_t       _next;
  uint8_t       r31;
  uint8_t       r30;
  uint8_t       r27;
  uint8_t       r26;
  uint8_t       r25;
  uint8_t       r24;
  uint8_t       r23;
  uint8_t       r22;
  uint8_t       r21;
  uint8_t       r20;
  uint8_t       r19;
  uint8_t       r18;
  uint8_t       sr;
  uint8_t       r1;
  uint8_t       r0;
  uint16_t      pc;
};

/**
 * System saved context.
 * @note The field @p _next is not part of the context, it represents the
 *       offset of the structure relative to the stack pointer.
 */
struct intctx {
  uint8_t       _next;
  uint8_t       r29;
  uint8_t       r28;
  uint8_t       r17;
  uint8_t       r16;
  uint8_t       r15;
  uint8_t       r14;
  uint8_t       r13;
  uint8_t       r12;
  uint8_t       r11;
  uint8_t       r10;
#ifndef CH_CURRP_REGISTER_CACHE
  uint8_t       r9;
  uint8_t       r8;
#endif
  uint8_t       r7;
  uint8_t       r6;
  uint8_t       r5;
  uint8_t       r4;
  uint8_t       r3;
  uint8_t       r2;
  uint8_t       pcl;
  uint8_t       pch;
};

/**
 * Platform dependent part of the @p Thread structure.
 * In the AVR port this structure just holds a pointer to the @p intctx
 * structure representing the stack pointer at the time of the context switch.
 */
typedef struct {
  struct intctx *sp;
} Context;

/**
 * Platform dependent part of the @p chThdInit() API.
 * This code usually setup the context switching frame represented by a
 * @p intctx structure.
 */
#define SETUP_CONTEXT(workspace, wsize, pf, arg) {                      \
  tp->p_ctx.sp = (struct intctx*)((uint8_t *)workspace + wsize  -       \
                                  sizeof(struct intctx));               \
  tp->p_ctx.sp->r2  = (int)pf;                                          \
  tp->p_ctx.sp->r3  = (int)pf >> 8;                                     \
  tp->p_ctx.sp->r4  = (int)arg;                                         \
  tp->p_ctx.sp->r5  = (int)arg >> 8;                                    \
  tp->p_ctx.sp->pcl = (int)threadstart >> 8;                            \
  tp->p_ctx.sp->pch = (int)threadstart;                                 \
}

/**
 * The default idle thread implementation requires no extra stack space in
 * this port.
 */
#ifndef IDLE_THREAD_STACK_SIZE
#define IDLE_THREAD_STACK_SIZE 8
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
#define STACK_ALIGN(n) ((((n) - 1) | sizeof(stkalign_t)) + 1)

/**
 * Computes the thread working area global size.
 */
#define THD_WA_SIZE(n) STACK_ALIGN(sizeof(Thread) +                     \
                                   (sizeof(struct intctx) - 1) +        \
                                   (sizeof(struct extctx) - 1) +        \
                                   (n) + (INT_REQUIRED_STACK))

/**
 * Macro used to allocate a thread working area aligned as both position and
 * size.
 */
#define WORKING_AREA(s, n) stkalign_t s[THD_WA_SIZE(n) / sizeof(stkalign_t)];

/**
 * IRQ prologue code, inserted at the start of all IRQ handlers enabled to
 * invoke system APIs.
 * This code tricks the compiler to save the specified registers by "touching"
 * them.
 */
#define SYS_IRQ_PROLOGUE() {                                            \
asm ("" : : : "r18", "r19", "r20", "r21", "r22", "r23", "r24",          \
              "r25", "r26", "r27", "r30", "r31");                       \
}

/**
 * IRQ epilogue code, inserted at the end of all IRQ handlers enabled to
 * invoke system APIs.
 */
#define SYS_IRQ_EPILOGUE() {                                            \
  if (chSchRescRequiredI())                                             \
    chSchDoRescheduleI();                                               \
}

/**
 * IRQ handler function modifier. Note, it just aliases the WinAVR "ISR"
 * macro.
 */
#define SYS_IRQ_HANDLER ISR

/**
 * This port function is implemented as inlined code for performance reasons.
 */
#define sys_disable() asm volatile ("cli")

/**
 * This port function is implemented as inlined code for performance reasons.
 */
#define sys_enable() asm volatile ("sei")

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
 * In this port it is no different than sys_disable() because the simple
 * interrupt handling
 */
#define sys_disable_all() sys_disable()

/**
 * This port function is implemented as inlined code for performance reasons.
 */
#if ENABLE_WFI_IDLE != 0
#define sys_wait_for_interrupt() {                                      \
  asm volatile ("sleep");                                               \
}
#else
#define sys_wait_for_interrupt()
#endif

#ifdef __cplusplus
extern "C" {
#endif
  void sys_puts(char *msg);
  void sys_switch(Thread *otp, Thread *ntp);
  void sys_halt(void);
  void threadstart(void);
#ifdef __cplusplus
}
#endif

#endif /* _CHCORE_H_ */

/** @} */
