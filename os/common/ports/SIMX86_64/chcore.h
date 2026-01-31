/*
    Copyright (C) 2006-2026 Giovanni Di Sirio.

    This file is part of ChibiOS.

    ChibiOS is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation version 3 of the License.

    ChibiOS is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

/**
 * @file    SIMX86_64/chcore.h
 * @brief   Simulator on x86-64 port macros and structures.
 *
 * @addtogroup SIMX86_64_GCC_CORE
 * @{
 */

#ifndef CHCORE_H
#define CHCORE_H

/*===========================================================================*/
/* Module constants.                                                         */
/*===========================================================================*/

/**
 * @name    Port Capabilities and Constants
 * @{
 */
/**
 * @brief   This port supports a realtime counter.
 */
#define PORT_SUPPORTS_RT                TRUE

/**
 * @brief   Natural alignment constant.
 * @note    It is the minimum alignment for pointer-size variables.
 */
#define PORT_NATURAL_ALIGN              sizeof (void *)

/**
 * @brief   Stack initial alignment constant.
 * @note    It is the alignment required for the stack pointer.
 */
#define PORT_STACK_ALIGN                16U

/**
 * @brief   Working Areas alignment constant.
 * @note    It is the alignment to be enforced for thread working areas.
 */
#define PORT_WORKING_AREA_ALIGN         16U
/** @} */

/**
 * @name    Architecture and Compiler
 * @{
 */
/**
 * Macro defining the a simulated architecture into x86-64.
 */
#define PORT_ARCHITECTURE_SIMX86_64

/**
 * Name of the implemented architecture.
 */
#define PORT_ARCHITECTURE_NAME          "Simulator"

/**
 * @brief   Name of the architecture variant (optional).
 */
#define PORT_CORE_VARIANT_NAME          "x86-64 (integer only)"

/**
 * @brief   Name of the compiler supported by this port.
 */
#define PORT_COMPILER_NAME              "GCC " __VERSION__

/**
 * @brief   Port-specific information string.
 */
#define PORT_INFO                       "No preemption"
/** @} */

/*===========================================================================*/
/* Module pre-compile time settings.                                         */
/*===========================================================================*/

/**
 * @brief   Stack size for the system idle thread.
 * @details This size depends on the idle thread implementation, usually
 *          the idle thread should take no more space than those reserved
 *          by @p PORT_INT_REQUIRED_STACK.
 */
#if !defined(PORT_IDLE_THREAD_STACK_SIZE) || defined(__DOXYGEN__)
#define PORT_IDLE_THREAD_STACK_SIZE     256
#endif

/**
 * @brief   Per-thread stack overhead for interrupts servicing.
 * @details This constant is used in the calculation of the correct working
 *          area size.
 */
#if !defined(PORT_INT_REQUIRED_STACK) || defined(__DOXYGEN__)
#define PORT_INT_REQUIRED_STACK         16384
#endif

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

#if CH_DBG_ENABLE_STACK_CHECK
#error "option CH_DBG_ENABLE_STACK_CHECK not supported by this port"
#endif

/*===========================================================================*/
/* Module data structures and types.                                         */
/*===========================================================================*/

/* The following code is not processed when the file is included from an
   asm module.*/
#if !defined(_FROM_ASM_)

/**
 * @brief   16 bytes stack and memory alignment enforcement.
 */
typedef struct {
  uint8_t a[16];
} stkline_t __attribute__((aligned(16)));

/**
 * @brief   Type of a generic x86-64 register.
 */
typedef void *regx86;

/**
 * @brief   Interrupt saved context.
 * @details This structure represents the stack frame saved during a
 *          preemption-capable interrupt handler.
 */
struct port_extctx {
};

/**
 * @brief   System saved context.
 * @details This structure represents the inner stack frame during a context
 *          switch.
 */
struct port_intctx {
  regx86  rbp;
  regx86  rbx;
  regx86  r12;
  regx86  r13;
  regx86  r14;
  regx86  r15;
  regx86  rdi;
  regx86  rsi;
  regx86  rip;
};

/**
 * @brief   Platform dependent part of the @p thread_t structure.
 * @details This structure usually contains just the saved stack pointer
 *          defined as a pointer to a @p port_intctx structure.
 */
struct port_context {
  struct port_intctx *sp;
};

#endif /* !defined(_FROM_ASM_) */

/*===========================================================================*/
/* Module macros.                                                            */
/*===========================================================================*/

#define APUSH(p, a) do {                                                    \
  (p) -= sizeof(void *);                                                    \
  *(void **)(void *)(p) = (void*)(a);                                       \
} while (false)

/* SysV ABI requires stack to be 16-byte aligned before a call. */
#define AALIGN(p, mask)                                                     \
  (p) = (void *)((uintptr_t)(p) & ~(uintptr_t)(mask))

/**
 * @brief   Platform dependent part of the @p chThdCreateI() API.
 * @details This code usually setup the context switching frame represented
 *          by an @p port_intctx structure.
 */
#define PORT_SETUP_CONTEXT(tp, wbase, wtop, pf, arg) {                      \
  /*lint -save -e611 -e9033 -e9074 -e9087 [10.8, 11.1, 11.3] Valid casts.*/ \
  uint8_t *rsp = (uint8_t *)wtop;                                           \
  __CH_USED(wbase);                                                         \
  AALIGN(rsp, 15);                                                          \
  APUSH(rsp, 0);                                                            \
  uint8_t *savebp = rsp;                                                    \
  rsp -= sizeof(struct port_intctx);                                        \
  ((struct port_intctx *)(void *)rsp)->rip = (void *)_port_thread_start;    \
  ((struct port_intctx *)(void *)rsp)->rbp = (void *)savebp;                \
  ((struct port_intctx *)(void *)rsp)->rbx = NULL;                          \
  ((struct port_intctx *)(void *)rsp)->r12 = NULL;                          \
  ((struct port_intctx *)(void *)rsp)->r13 = NULL;                          \
  ((struct port_intctx *)(void *)rsp)->r14 = NULL;                          \
  ((struct port_intctx *)(void *)rsp)->r15 = NULL;                          \
  ((struct port_intctx *)(void *)rsp)->rdi = (void *)pf;                    \
  ((struct port_intctx *)(void *)rsp)->rsi = (void *)arg;                   \
  (tp)->ctx.sp = (struct port_intctx *)(void *)rsp;                         \
  /*lint -restore*/                                                         \
}

/**
 * @brief   Computes the thread working area global size.
 * @note    There is no need to perform alignments in this macro.
 */
#define PORT_WA_SIZE(n) ((sizeof (void *) * 4U) +                           \
                         sizeof (struct port_intctx) +                      \
                         ((size_t)(n)) +                                    \
                         ((size_t)(PORT_INT_REQUIRED_STACK)))

/**
 * @brief   Priority level verification macro.
 */
#define PORT_IRQ_IS_VALID_PRIORITY(n) false

/**
 * @brief   Priority level verification macro.
 */
#define PORT_IRQ_IS_VALID_KERNEL_PRIORITY(n) false

/**
 * @brief   IRQ prologue code.
 * @details This macro must be inserted at the start of all IRQ handlers
 *          enabled to invoke system APIs.
 */
#define PORT_IRQ_PROLOGUE() {                                               \
  port_isr_context_flag = true;                                             \
}

/**
 * @brief   IRQ epilogue code.
 * @details This macro must be inserted at the end of all IRQ handlers
 *          enabled to invoke system APIs.
 */
#define PORT_IRQ_EPILOGUE() {                                               \
  port_isr_context_flag = false;                                            \
}

/**
 * @brief   IRQ handler function declaration.
 * @note    @p id can be a function name or a vector number depending on the
 *          port implementation.
 */
#ifdef __cplusplus
#define PORT_IRQ_HANDLER(id) extern "C" void id(void)
#else
#define PORT_IRQ_HANDLER(id) void id(void)
#endif

/**
 * @brief   Fast IRQ handler function declaration.
 * @note    @p id can be a function name or a vector number depending on the
 *          port implementation.
 */
#ifdef __cplusplus
#define PORT_FAST_IRQ_HANDLER(id) extern "C" void id(void)
#else
#define PORT_FAST_IRQ_HANDLER(id) void id(void)
#endif

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

/* The following code is not processed when the file is included from an
   asm module.*/
#if !defined(_FROM_ASM_)

extern bool port_isr_context_flag;
extern syssts_t port_irq_sts;

#ifdef __cplusplus
extern "C" {
#endif
  /*lint -save -e950 [Dir-2.1] Non-ANSI keywords are fine in the port layer.*/
  void port_switch(thread_t *ntp, thread_t *otp);
  __attribute__((noreturn)) void _port_thread_start(msg_t (*pf)(void *p),
                                                    void *p);
  /*lint -restore*/
  rtcnt_t port_rt_get_counter_value(void);
  void _sim_check_for_interrupts(void);
#ifdef __cplusplus
}
#endif

#endif /* !defined(_FROM_ASM_) */

/*===========================================================================*/
/* Module inline functions.                                                  */
/*===========================================================================*/

/* The following code is not processed when the file is included from an
   asm module.*/
#if !defined(_FROM_ASM_)

/**
 * @brief   Port-related initialization code.
 */
static inline void port_init(os_instance_t *oip) {

  (void)oip;

  port_irq_sts = (syssts_t)0;
  port_isr_context_flag = false;
}

/**
 * @brief   Returns a word encoding the current interrupts status.
 *
 * @return              The interrupts status.
 */
static inline syssts_t port_get_irq_status(void) {

  return port_irq_sts;
}

/**
 * @brief   Checks the interrupt status.
 *
 * @param[in] sts       the interrupt status word
 *
 * @return              The interrupt status.
 * @retval false        the word specified a disabled interrupts status.
 * @retval true         the word specified an enabled interrupts status.
 */
static inline bool port_irq_enabled(syssts_t sts) {

  return sts == (syssts_t)0;
}

/**
 * @brief   Determines the current execution context.
 *
 * @return              The execution context.
 * @retval false        not running in ISR mode.
 * @retval true         running in ISR mode.
 */
static inline bool port_is_isr_context(void) {

  return port_isr_context_flag;
}

/**
 * @brief   Kernel-lock action.
 * @details In this port this function disables interrupts globally.
 */
static inline void port_lock(void) {

  port_irq_sts = (syssts_t)1;
}

/**
 * @brief   Kernel-unlock action.
 * @details In this port this function enables interrupts globally.
 */
static inline void port_unlock(void) {

  port_irq_sts = (syssts_t)0;
}

/**
 * @brief   Kernel-lock action from an interrupt handler.
 * @details In this port this function disables interrupts globally.
 * @note    Same as @p port_lock() in this port.
 */
static inline void port_lock_from_isr(void) {

  port_irq_sts = (syssts_t)1;
}

/**
 * @brief   Kernel-unlock action from an interrupt handler.
 * @details In this port this function enables interrupts globally.
 * @note    Same as @p port_lock() in this port.
 */
static inline void port_unlock_from_isr(void) {

  port_irq_sts = (syssts_t)0;
}

/**
 * @brief   Disables all the interrupt sources.
 */
static inline void port_disable(void) {

  port_irq_sts = (syssts_t)1;
}

/**
 * @brief   Disables the interrupt sources below kernel-level priority.
 */
static inline void port_suspend(void) {

  port_irq_sts = (syssts_t)1;
}

/**
 * @brief   Enables all the interrupt sources.
 */
static inline void port_enable(void) {

  port_irq_sts = (syssts_t)0;
}

/**
 * @brief   Enters an architecture-dependent IRQ-waiting mode.
 * @details The function is meant to return when an interrupt becomes pending.
 *          The simplest implementation is an empty function or macro but this
 *          would not take advantage of architecture-specific power saving
 *          modes.
 * @note    Implemented as an inlined @p WFI instruction.
 */
static inline void port_wait_for_interrupt(void) {

  _sim_check_for_interrupts();
}

#endif /* !defined(_FROM_ASM_) */

/*===========================================================================*/
/* Module late inclusions.                                                   */
/*===========================================================================*/

#if !defined(_FROM_ASM_)

#if CH_CFG_ST_TIMEDELTA > 0
#include "chcore_timer.h"
#endif /* CH_CFG_ST_TIMEDELTA > 0 */

#endif /* !defined(_FROM_ASM_) */

#endif /* CHCORE_H */

/** @} */
