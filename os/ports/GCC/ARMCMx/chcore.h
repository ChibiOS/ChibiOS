/*
    ChibiOS/RT - Copyright (C) 2006,2007,2008,2009,2010,
                 2011,2012 Giovanni Di Sirio.

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
 * @file    GCC/ARMCMx/chcore.h
 * @brief   ARM Cortex-Mx port macros and structures.
 *
 * @addtogroup ARMCMx_CORE
 * @{
 */

#ifndef _CHCORE_H_
#define _CHCORE_H_

/*===========================================================================*/
/* Port constants (common).                                                  */
/*===========================================================================*/

/* Added to make the header stand-alone when included from asm.*/
#ifndef FALSE
#define FALSE       0
#endif
#ifndef TRUE
#define TRUE        (!FALSE)
#endif

#define CORTEX_M0                       0   /**< @brief Cortex-M0 variant.  */
#define CORTEX_M1                       1   /**< @brief Cortex-M1 variant.  */
#define CORTEX_M3                       3   /**< @brief Cortex-M3 variant.  */
#define CORTEX_M4                       4   /**< @brief Cortex-M4 variant.  */

/* Inclusion of the Cortex-Mx implementation specific parameters.*/
#include "cmparams.h"

/* Cortex model check, only M0 and M3 supported right now.*/
#if (CORTEX_MODEL == CORTEX_M0) || (CORTEX_MODEL == CORTEX_M3)
#elif (CORTEX_MODEL == CORTEX_M1) || (CORTEX_MODEL == CORTEX_M4)
#warning "untested Cortex-M model"
#else
#error "unknown or unsupported Cortex-M model"
#endif

/**
 * @brief   Total priority levels.
 */
#define CORTEX_PRIORITY_LEVELS          (1 << CORTEX_PRIORITY_BITS)

/**
 * @brief   Minimum priority level.
 * @details This minimum priority level is calculated from the number of
 *          priority bits supported by the specific Cortex-Mx implementation.
 */
#define CORTEX_MINIMUM_PRIORITY         (CORTEX_PRIORITY_LEVELS - 1)

/**
 * @brief   Maximum priority level.
 * @details The maximum allowed priority level is always zero.
 */
#define CORTEX_MAXIMUM_PRIORITY         0

/*===========================================================================*/
/* Port macros (common).                                                     */
/*===========================================================================*/

/**
 * @brief   Priority level verification macro.
 */
#define CORTEX_IS_VALID_PRIORITY(n)                                         \
  (((n) >= 0) && ((n) < CORTEX_PRIORITY_LEVELS))

/**
 * @brief   Priority level to priority mask conversion macro.
 */
#define CORTEX_PRIORITY_MASK(n)                                             \
  ((n) << (8 - CORTEX_PRIORITY_BITS))

/*===========================================================================*/
/* Port configurable parameters (common).                                    */
/*===========================================================================*/

/**
 * @brief   Stack size for the system idle thread.
 * @details This size depends on the idle thread implementation, usually
 *          the idle thread should take no more space than those reserved
 *          by @p PORT_INT_REQUIRED_STACK.
 * @note    In this port it is set to 16 because the idle thread does have
 *          a stack frame when compiling without optimizations. You may
 *          reduce this value to zero when compiling with optimizations.
 */
#ifndef IDLE_THREAD_STACK_SIZE
#define IDLE_THREAD_STACK_SIZE          16
#endif

/**
 * @brief   Per-thread stack overhead for interrupts servicing.
 * @details This constant is used in the calculation of the correct working
 *          area size.
 *          This value can be zero on those architecture where there is a
 *          separate interrupt stack and the stack space between @p intctx and
 *          @p extctx is known to be zero.
 * @note    In this port it is conservatively set to 16 because the function
 *          @p chSchDoRescheduleI() can have a stack frame, expecially with
 *          compiler optimizations disabled.
 */
#ifndef INT_REQUIRED_STACK
#define INT_REQUIRED_STACK              16
#endif

/**
 * @brief   Enables the use of the WFI instruction in the idle thread loop.
 */
#ifndef CORTEX_ENABLE_WFI_IDLE
#define CORTEX_ENABLE_WFI_IDLE          FALSE
#endif

/**
 * @brief   SYSTICK handler priority.
 * @note    The default SYSTICK handler priority is calculated as the priority
 *          level in the middle of the numeric priorities range.
 */
#ifndef CORTEX_PRIORITY_SYSTICK
#define CORTEX_PRIORITY_SYSTICK         (CORTEX_PRIORITY_LEVELS >> 1)
#else
/* If it is externally redefined then better perform a validity check on it.*/
#if !CORTEX_IS_VALID_PRIORITY(CORTEX_PRIORITY_SYSTICK)
#error "invalid priority level specified for CORTEX_PRIORITY_SYSTICK"
#endif
#endif

/**
 * @brief   Stack alignment enforcement.
 * @note    The default value is 64 in order to comply with EABI, reducing
 *          the value to 32 can save some RAM space if you don't care about
 *          binary compatibility with EABI compiled libraries.
 * @note    Allowed values are 32 or 64.
 */
#ifndef CORTEX_STACK_ALIGNMENT
#define CORTEX_STACK_ALIGNMENT          64
#endif

/*===========================================================================*/
/* Port derived parameters (common).                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Port exported info (common).                                              */
/*===========================================================================*/

/**
 * @brief   Macro defining a generic ARM architecture.
 */
#define CH_ARCHITECTURE_ARM

/*===========================================================================*/
/* Port implementation part (common).                                        */
/*===========================================================================*/

/* Includes the sub-architecture-specific part.*/
#if (CORTEX_MODEL == CORTEX_M0) || (CORTEX_MODEL == CORTEX_M1)
#include "chcore_v6m.h"
#elif (CORTEX_MODEL == CORTEX_M3) || (CORTEX_MODEL == CORTEX_M4)
#include "chcore_v7m.h"
#endif

#if !defined(_FROM_ASM_)

#include "nvic.h"

/**
 * @brief   Stack and memory alignment enforcement.
 */
#if (CORTEX_STACK_ALIGNMENT == 64) || defined(__DOXYGEN__)
#if defined(__DOXYGEN__)
/* Dummy declaration, for Doxygen only.*/
typedef uint64_t stkalign_t;
#else
typedef uint64_t stkalign_t __attribute__ ((aligned (8)));
#endif
#elif CORTEX_STACK_ALIGNMENT == 32
typedef uint32_t stkalign_t __attribute__ ((aligned (4)));
#else
#error "invalid stack alignment selected"
#endif

#if defined(__DOXYGEN__)
/**
 * @brief   Interrupt saved context.
 * @details This structure represents the stack frame saved during a
 *          preemption-capable interrupt handler.
 * @note    It is implemented to match the Cortex-Mx exception context.
 */
struct extctx {
  /* Dummy definition, just for Doxygen.*/
};

/**
 * @brief   System saved context.
 * @details This structure represents the inner stack frame during a context
 *          switching.
 */
struct intctx {
  /* Dummy definition, just for Doxygen.*/
};
#endif

/**
 * @brief   Platform dependent part of the @p Thread structure.
 * @details In this port the structure just holds a pointer to the @p intctx
 *          structure representing the stack pointer at context switch time.
 */
struct context {
  struct intctx *r13;
};

/**
 * @brief   Platform dependent part of the @p chThdCreateI() API.
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
#define WORKING_AREA(s, n) stkalign_t s[THD_WA_SIZE(n) / sizeof(stkalign_t)]

#endif /* _FROM_ASM_ */

#endif /* _CHCORE_H_ */

/** @} */
