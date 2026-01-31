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
 * @file    ARMvx-M-SB/chcore.h
 * @brief   ARMvx-M-SB port macros and structures.
 *
 * @addtogroup ARMVXM_SB_CORE
 * @{
 */

#ifndef CHCORE_H
#define CHCORE_H

/* The following code is not processed when the file is included from an
   asm module because those intrinsic macros are not necessarily defined
   by the assembler too.*/
#if !defined(_FROM_ASM_)

#include "sbuser.h"

/*===========================================================================*/
/* Module constants.                                                         */
/*===========================================================================*/

/**
 * @brief   Compiler name and version.
 */
#if defined(__GNUC__) || defined(__DOXYGEN__)
#define PORT_COMPILER_NAME              "GCC " __VERSION__

#elif defined(__ICCARM__)
#define PORT_COMPILER_NAME              "IAR"
#error "unsupported compiler"

#elif defined(__CC_ARM)
#define PORT_COMPILER_NAME              "RVCT"
#error "unsupported compiler"

#else
#error "unsupported compiler"
#endif

#endif /* !defined(_FROM_ASM_) */
/** @} */

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
 * @note    It is the alignment required for the initial stack pointer,
 *          must be a multiple of sizeof (port_stkline_t).
 */
#define PORT_STACK_ALIGN                8U

/**
 * @brief   Working Areas alignment constant.
 * @note    It is the alignment to be enforced for thread working areas,
 *          must be a multiple of sizeof (port_stkline_t).
 */
#define PORT_WORKING_AREA_ALIGN         8U
/** @} */

/**
 * @name    Architecture
 * @{
 */
/**
 * @brief   Macro defining a generic ARM architecture.
 */
#define PORT_ARCHITECTURE_ARM

/**
 * @brief   Macro defining the specific ARM architecture.
 */
#define PORT_ARCHITECTURE_ARM_VXM_SB

/**
 * @brief   Name of the implemented architecture.
 */
#define PORT_ARCHITECTURE_NAME          "ARMvx-M-SB"

/**
 * @brief   Name of the architecture variant.
 */
#define PORT_CORE_VARIANT_NAME          "Cortex-M generic"

/**
 * @brief   Port-specific information string.
 */
#define PORT_INFO                       "Sandboxed kernel mode"
/** @} */

/*===========================================================================*/
/* Module pre-compile time settings.                                         */
/*===========================================================================*/

/**
 * @brief   Stack size for the system idle thread.
 * @details This size depends on the idle thread implementation, usually
 *          the idle thread should take no more space than those reserved
 *          by @p PORT_INT_REQUIRED_STACK.
 * @note    In this port it is set to 64 because the idle thread does have
 *          a stack frame when compiling without optimizations. You may
 *          reduce this value to zero when compiling with optimizations.
 */
#if !defined(PORT_IDLE_THREAD_STACK_SIZE) || defined(__DOXYGEN__)
#define PORT_IDLE_THREAD_STACK_SIZE     64
#endif

/**
 * @brief   Per-thread stack overhead for interrupts servicing.
 * @details This constant is used in the calculation of the correct working
 *          area size.
 * @note    The exact value depends on stack usage of your VSRs and compiler
 *          options.
 */
#if !defined(PORT_INT_REQUIRED_STACK) || defined(__DOXYGEN__)
#define PORT_INT_REQUIRED_STACK         256
#endif

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

/* The following code is not processed when the file is included from an
   asm module.*/
#if !defined(_FROM_ASM_)

/*===========================================================================*/
/* Module data structures and types.                                         */
/*===========================================================================*/

/**
 * @brief   Interrupt saved context.
 * @details This structure represents the stack frame saved during a
 *          preemption-capable interrupt handler.
 * @note    It is implemented to match the Cortex-Mx exception context.
 */
struct port_extctx {
  uint32_t      r0;
  uint32_t      r1;
  uint32_t      r2;
  uint32_t      r3;
  uint32_t      r12;
  uint32_t      lr_thd;
  uint32_t      pc;
  uint32_t      xpsr;
#if CORTEX_USE_FPU
  uint32_t      s0;
  uint32_t      s1;
  uint32_t      s2;
  uint32_t      s3;
  uint32_t      s4;
  uint32_t      s5;
  uint32_t      s6;
  uint32_t      s7;
  uint32_t      s8;
  uint32_t      s9;
  uint32_t      s10;
  uint32_t      s11;
  uint32_t      s12;
  uint32_t      s13;
  uint32_t      s14;
  uint32_t      s15;
  uint32_t      fpscr;
  uint32_t      reserved;
#endif /* CORTEX_USE_FPU */
};

/**
 * @brief   System saved context.
 * @details This structure represents the inner stack frame during a context
 *          switch.
 */
struct port_intctx {
#if CORTEX_USE_FPU
  uint32_t      s16;
  uint32_t      s17;
  uint32_t      s18;
  uint32_t      s19;
  uint32_t      s20;
  uint32_t      s21;
  uint32_t      s22;
  uint32_t      s23;
  uint32_t      s24;
  uint32_t      s25;
  uint32_t      s26;
  uint32_t      s27;
  uint32_t      s28;
  uint32_t      s29;
  uint32_t      s30;
  uint32_t      s31;
#endif /* CORTEX_USE_FPU */
  uint32_t      r4;
  uint32_t      r5;
  uint32_t      r6;
  uint32_t      r7;
  uint32_t      r8;
  uint32_t      r9;
  uint32_t      r10;
  uint32_t      r11;
  uint32_t      lr;
};

/**
 * @brief   Platform dependent part of the @p thread_t structure.
 * @details In this port the structure just holds a pointer to the
 *          @p port_intctx structure representing the stack pointer
 *          at context switch time.
 */
struct port_context {
  struct port_intctx    *sp;
};

#endif /* !defined(_FROM_ASM_) */

/*===========================================================================*/
/* Module macros.                                                            */
/*===========================================================================*/

/**
 * @brief   Priority level verification macro.
 */
#define PORT_IRQ_IS_VALID_PRIORITY(n) false

/**
 * @brief   Priority level verification macro.
 */
#define PORT_IRQ_IS_VALID_KERNEL_PRIORITY(n) false

/**
 * @brief   Optimized thread function declaration macro.
 */
#define PORT_THD_FUNCTION(tname, arg) void tname(void *arg)

/**
 * @brief   Platform dependent part of the @p chThdCreateI() API.
 * @details This code usually setup the context switching frame represented
 *          by an @p port_intctx structure.
 */
#define PORT_SETUP_CONTEXT(tp, wbase, wtop, pf, arg) do {                   \
  (tp)->ctx.sp = (struct port_intctx *)(void *)								\
  	  	  	  	   ((uint8_t *)(wtop) - sizeof (struct port_intctx));       \
  (tp)->ctx.sp->r4 = (uint32_t)(pf);                                        \
  (tp)->ctx.sp->r5 = (uint32_t)(arg);                                       \
  (tp)->ctx.sp->lr = (uint32_t)__port_thread_start;                         \
} while (false)

/**
 * @brief   Context switch area size.
 */
#define PORT_WA_CTX_SIZE (sizeof (struct port_intctx) +                     \
                          sizeof (struct port_extctx))

/**
 * @brief   Computes the thread working area global size.
 * @note    There is no need to perform alignments in this macro.
 */
#define PORT_WA_SIZE(n) ((size_t)PORT_WA_CTX_SIZE +                         \
                         (size_t)(n) +                                      \
                         (size_t)PORT_INT_REQUIRED_STACK)

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

/**
 * @brief   Performs a context switch between two threads.
 * @details This is the most critical code in any port, this function
 *          is responsible for the context switch between 2 threads.
 * @note    The implementation of this code affects <b>directly</b> the context
 *          switch performance so optimize here as much as you can.
 *
 * @param[in] ntp       the thread to be switched in
 * @param[in] otp       the thread to be switched out
 */
#if (CH_DBG_ENABLE_STACK_CHECK == FALSE) || defined(__DOXYGEN__)
  #define port_switch(ntp, otp) __port_switch(ntp, otp)

#else
  #define port_switch(ntp, otp) do {                                        \
    struct port_intctx *r13 = (struct port_intctx *)__get_PSP();            \
    if ((stkline_t *)(void *)(r13 - 1) < (otp)->wabase) {                   \
      chSysHalt("stack overflow");                                          \
    }                                                                       \
    __port_switch(ntp, otp);                                                \
  } while (false)
#endif

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#if !defined(_FROM_ASM_)

extern bool __sb_in_vrq;

#ifdef __cplusplus
extern "C" {
#endif
  void port_init(os_instance_t *oip);
  void __port_switch(thread_t *ntp, thread_t *otp);
  void __port_thread_start(void);
#ifdef __cplusplus
}
#endif

/*===========================================================================*/
/* Module inline functions.                                                  */
/*===========================================================================*/

/**
 * @brief   Returns a word encoding the current interrupts status.
 *
 * @return              The interrupts status.
 */
static inline syssts_t port_get_irq_status(void) {

  return (syssts_t)__sb_vrq_getisr();
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

  return (bool)((sts & 1U) == (syssts_t)0);
}

/**
 * @brief   Determines the current execution context.
 *
 * @return              The execution context.
 * @retval false        not running in ISR mode.
 * @retval true         running in ISR mode.
 */
static inline bool port_is_isr_context(void) {

  return __sb_in_vrq;
}

/**
 * @brief   Kernel-lock action.
 * @details Global disable in this port.
 */
static inline void port_lock(void) {

  __sb_vrq_disable();
}

/**
 * @brief   Kernel-unlock action.
 * @details Global enable in this port.
 */
static inline void port_unlock(void) {

  __sb_vrq_enable();
}

/**
 * @brief   Kernel-lock action from an interrupt handler.
 * @note    Empty in this port.
 */
static inline void port_lock_from_isr(void) {

}

/**
 * @brief   Kernel-unlock action from an interrupt handler.
 * @note    Empty in this port.
 */
static inline void port_unlock_from_isr(void) {

}

/**
 * @brief   Disables all the interrupt sources.
 * @details Global disable in this port.
 */
static inline void port_disable(void) {

  __sb_vrq_disable();
}

/**
 * @brief   Disables the interrupt sources below kernel-level priority.
 * @note    Same as disabling globally in this port.
 */
static inline void port_suspend(void) {

  __sb_vrq_disable();
}

/**
 * @brief   Enables all the interrupt sources.
 * @details Global enable in this port.
 */
static inline void port_enable(void) {

  __sb_vrq_enable();
}

/**
 * @brief   Enters an architecture-dependent IRQ-waiting mode.
 * @details The function is meant to return when an interrupt becomes pending.
 *          The simplest implementation is an empty function or macro but this
 *          would not take advantage of architecture-specific power saving
 *          modes.
 * @note    Implemented as a VRQ-wait which releases the CPU to the sandbox
 *          host OS.
 */
static inline void port_wait_for_interrupt(void) {

  __sb_vrq_wait();
}

/**
 * @brief   Returns the current value of the realtime counter.
 *
 * @return              The realtime counter value.
 */
static inline rtcnt_t port_rt_get_counter_value(void) {

  return (rtcnt_t)sbGetSystemTime();
}

#endif /* !defined(_FROM_ASM_) */

/*===========================================================================*/
/* Module late inclusions.                                                   */
/*===========================================================================*/

#if !defined(_FROM_ASM_)

#if CH_CFG_ST_TIMEDELTA > 0
#error "tickless mode not implemented in this port"
#endif /* CH_CFG_ST_TIMEDELTA > 0 */

#endif /* !defined(_FROM_ASM_) */

#endif /* CHCORE_H */

/** @} */
