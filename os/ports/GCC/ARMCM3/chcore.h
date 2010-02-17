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
 * @file ARMCM3/chcore.h
 * @brief ARM Cortex-M3 architecture port macros and structures.
 * @addtogroup ARMCM3_CORE
 * @{
 */

#ifndef _CHCORE_H_
#define _CHCORE_H_

/*
 * Port-related configuration parameters.
 */

/**
 * Enables the use of the WFI ins.
 */
#ifndef ENABLE_WFI_IDLE
#define ENABLE_WFI_IDLE         0
#endif

/**
 * BASEPRI user level, 0 = disabled.
 */
#ifndef BASEPRI_USER
#define BASEPRI_USER            0
#endif

/**
 * BASEPRI level within kernel lock.
 * Priority levels higher than this one (lower values) are unaffected by
 * the OS activity and can be classified as fast interrupt sources, see
 * @ref interrupt_classes.
 */
#ifndef BASEPRI_KERNEL
#define BASEPRI_KERNEL          0x40
#endif

/**
 * SVCALL handler priority.
 * @note This priority must always be one level above the @p BASEPRI_KERNEL
 *       value.
 * @note It is recommended to leave this priority level for this handler alone.
 */
#ifndef PRIORITY_SVCALL
#define PRIORITY_SVCALL         (BASEPRI_KERNEL - 0x10)
#endif

/**
 * SYSTICK handler priority.
 */
#ifndef PRIORITY_SYSTICK
#define PRIORITY_SYSTICK        0x80
#endif

/**
 * PENDSV handler priority.
 * @note It is recommended to leave this priority level for this handler alone.
 * @note This is a reserved handler and its priority must always be the
 *       lowest priority in the system in order to be always executed last
 *       in the interrupt servicing chain.
 */
#ifndef PRIORITY_PENDSV
#define PRIORITY_PENDSV         0xF0
#endif

/**
 * Macro defining the ARM Cortex-M3 architecture.
 */
#define CH_ARCHITECTURE_ARMCM3

/**
 * Name of the implemented architecture.
 */
#define CH_ARCHITECTURE_NAME "ARM Cortex-M3"

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
 * Interrupt saved context, empty in this architecture.
 */
struct extctx {
};
/** @endcond */

/** @cond never */
/**
 * This structure represents the inner stack frame during a context switching.
 */
struct intctx {
  regarm_t      basepri;
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
  regarm_t      lr_exc;
  regarm_t      r0;
  regarm_t      r1;
  regarm_t      r2;
  regarm_t      r3;
  regarm_t      r12;
  regarm_t      lr_thd;
  regarm_t      pc;
  regarm_t      xpsr;
};
/** @endcond */

/** @cond never */
/**
 * Cortex-M3 context structure.
 */
struct context {
  struct intctx *r13;
};
/** @endcond */

/**
 * Platform dependent part of the @p chThdInit() API.
 * This code usually setup the context switching frame represented by a
 * @p intctx structure.
 */
#define SETUP_CONTEXT(workspace, wsize, pf, arg) {                      \
  tp->p_ctx.r13 = (struct intctx *)((uint8_t *)workspace +              \
                                     wsize -                            \
                                     sizeof(struct intctx));            \
  tp->p_ctx.r13->basepri = BASEPRI_USER;                                \
  tp->p_ctx.r13->lr_exc = (regarm_t)0xFFFFFFFD;                         \
  tp->p_ctx.r13->r0 = arg;                                              \
  tp->p_ctx.r13->lr_thd = chThdExit;                                    \
  tp->p_ctx.r13->pc = pf;                                               \
  tp->p_ctx.r13->xpsr = (regarm_t)0x01000000;                           \
}

/**
 * The default idle thread implementation requires no extra stack space in
 * this port.
 */
#ifndef IDLE_THREAD_STACK_SIZE
#define IDLE_THREAD_STACK_SIZE 4
#endif

/**
 * This port requires no extra stack space for interrupt handling.
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
#define PORT_IRQ_EPILOGUE() {                                           \
  chSysLockFromIsr();                                                   \
  if (chSchIsRescRequiredI())                                           \
    SCB_ICSR = ICSR_PENDSVSET;                                          \
  chSysUnlockFromIsr();                                                 \
}

/**
 * IRQ handler function declaration.
 */
#define PORT_IRQ_HANDLER(id) void id(void)

/**
 * This function is empty in this port.
 */
#define port_init()

/**
 * Raises the base priority to kernel level.
 */
#if CH_OPTIMIZE_SPEED
#define port_lock() {                                                   \
  register uint32_t tmp asm ("r3") = BASEPRI_KERNEL;                    \
  asm volatile ("msr     BASEPRI, %0" : : "r" (tmp));                   \
}
#else
#define port_lock() {                                                   \
  asm volatile ("bl      _port_lock" : : : "r3", "lr");                 \
}
#endif

/**
 * Lowers the base priority to user level.
 */
#if CH_OPTIMIZE_SPEED
#define port_unlock() {                                                 \
  register uint32_t tmp asm ("r3") = BASEPRI_USER;                      \
  asm volatile ("msr     BASEPRI, %0" : : "r" (tmp));                   \
}
#else
#define port_unlock() {                                                 \
  asm volatile ("bl      _port_unlock" : : : "r3", "lr");               \
}
#endif

/**
 * Same as @p port_lock() in this port.
 */
#define port_lock_from_isr() port_lock()

/**
 * Same as @p port_unlock() in this port.
 */
#define port_unlock_from_isr() port_unlock()

/**
 * Disables all the interrupt sources by raising the priority mask to level 0.
 */
#define port_disable() asm volatile ("cpsid   i")

/**
 * Raises/lowers the base priority to kernel level.
 */
#define port_suspend() {                                                \
  register uint32_t tmp asm ("r3") = BASEPRI_KERNEL;                    \
  asm volatile ("msr     BASEPRI, %0                    \n\t"           \
                "cpsie   i" : : "r" (tmp));                             \
}

/**
 * Lowers the base priority to user level.
 */
#define port_enable() {                                                 \
  register uint32_t tmp asm ("r3") = BASEPRI_USER;                      \
  asm volatile ("msr     BASEPRI, %0                    \n\t"           \
                "cpsie   i" : : "r" (tmp));                             \
}

/**
 * This port function is implemented as inlined code for performance reasons.
 */
#if ENABLE_WFI_IDLE || defined(__DOXYGEN__)
#define port_wait_for_interrupt() {                                     \
  asm volatile ("wfi");                                                 \
}
#else
#define port_wait_for_interrupt()
#endif

/**
 * This port function is implemented as inlined code for performance reasons.
 */
#if CH_DBG_ENABLE_STACK_CHECK
#define port_switch(otp, ntp) {                                         \
  register Thread *_otp asm ("r0") = (otp);                             \
  register Thread *_ntp asm ("r1") = (ntp);                             \
  register char *sp asm ("sp");                                         \
  if (sp - sizeof(struct intctx) - sizeof(Thread) < (char *)_otp)       \
    asm volatile ("movs    r0, #0                               \n\t"   \
                  "b       chDbgPanic");                                \
  asm volatile ("svc     #0" : : "r" (_otp), "r" (_ntp) : "memory");    \
}
#else /* !CH_DBG_ENABLE_STACK_CHECK */
#define port_switch(otp, ntp) {                                         \
  register Thread *_otp asm ("r0") = (otp);                             \
  register Thread *_ntp asm ("r1") = (ntp);                             \
  asm volatile ("svc     #0" : : "r" (_otp), "r" (_ntp) : "memory");    \
}
#endif /* !CH_DBG_ENABLE_STACK_CHECK */

#ifdef __cplusplus
extern "C" {
#endif
  void port_halt(void);
#if !CH_OPTIMIZE_SPEED
  void _port_lock(void);
  void _port_unlock(void);
#endif
#ifdef __cplusplus
}
#endif

#endif /* _CHCORE_H_ */

/** @} */
