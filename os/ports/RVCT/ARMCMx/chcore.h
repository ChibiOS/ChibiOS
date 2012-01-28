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
 * @file    RVCT/ARMCMx/chcore.h
 * @brief   ARM Cortex-Mx port macros and structures.
 *
 * @addtogroup RVCT_ARMCMx_CORE
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
#if (CORTEX_MODEL == CORTEX_M0) || (CORTEX_MODEL == CORTEX_M3) ||           \
    (CORTEX_MODEL == CORTEX_M4)
#elif (CORTEX_MODEL == CORTEX_M1)
#error "untested Cortex-M model"
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
#if !defined(PORT_IDLE_THREAD_STACK_SIZE)
#define PORT_IDLE_THREAD_STACK_SIZE     16
#endif

/**
 * @brief   Per-thread stack overhead for interrupts servicing.
 * @details This constant is used in the calculation of the correct working
 *          area size.
 *          This value can be zero on those architecture where there is a
 *          separate interrupt stack and the stack space between @p intctx and
 *          @p extctx is known to be zero.
 * @note    In this port it is conservatively set to 16 because the function
 *          @p chSchDoReschedule() can have a stack frame, expecially with
 *          compiler optimizations disabled.
 */
#if !defined(PORT_INT_REQUIRED_STACK)
#define PORT_INT_REQUIRED_STACK         16
#endif

/**
 * @brief   Enables the use of the WFI instruction in the idle thread loop.
 */
#if !defined(CORTEX_ENABLE_WFI_IDLE)
#define CORTEX_ENABLE_WFI_IDLE          FALSE
#endif

/**
 * @brief   SYSTICK handler priority.
 * @note    The default SYSTICK handler priority is calculated as the priority
 *          level in the middle of the numeric priorities range.
 */
#if !defined(CORTEX_PRIORITY_SYSTICK)
#define CORTEX_PRIORITY_SYSTICK         (CORTEX_PRIORITY_LEVELS >> 1)
#elif !CORTEX_IS_VALID_PRIORITY(CORTEX_PRIORITY_SYSTICK)
/* If it is externally redefined then better perform a validity check on it.*/
#error "invalid priority level specified for CORTEX_PRIORITY_SYSTICK"
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

/**
 * @brief   Name of the compiler supported by this port.
 */
#define CH_COMPILER_NAME                "RVCT"

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

/* The following declarations are there just for Doxygen documentation, the
   real declarations are inside the sub-headers.*/
#if defined(__DOXYGEN__)

/**
 * @brief   Stack and memory alignment enforcement.
 * @note    In this architecture the stack alignment is enforced to 64 bits,
 *          32 bits alignment is supported by hardware but deprecated by ARM,
 *          the implementation choice is to not offer the option.
 */
typedef uint64_t stkalign_t;

/**
 * @brief   Interrupt saved context.
 * @details This structure represents the stack frame saved during a
 *          preemption-capable interrupt handler.
 * @note    It is implemented to match the Cortex-Mx exception context.
 */
struct extctx {};

/**
 * @brief   System saved context.
 * @details This structure represents the inner stack frame during a context
 *          switching.
 */
struct intctx {};

#endif /* defined(__DOXYGEN__) */

/**
 * @brief   Excludes the default @p chSchIsPreemptionRequired()implementation.
 */
#define PORT_OPTIMIZED_ISPREEMPTIONREQUIRED

#if (CH_TIME_QUANTUM > 0) || defined(__DOXYGEN__)
/**
 * @brief   Inlineable version of this kernel function.
 */
#define chSchIsPreemptionRequired()                                         \
  (rlist.r_preempt ? firstprio(&rlist.r_queue) > currp->p_prio :            \
                     firstprio(&rlist.r_queue) >= currp->p_prio)
#else /* CH_TIME_QUANTUM == 0 */
#define chSchIsPreemptionRequired()                                         \
  (firstprio(&rlist.r_queue) > currp->p_prio)
#endif /* CH_TIME_QUANTUM == 0 */

#endif /* _FROM_ASM_ */

#endif /* _CHCORE_H_ */

/** @} */
