/*
    ChibiOS/RT - Copyright (C) 2006,2007,2008,2009,2010 Giovanni Di Sirio.

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
 * @file    IAR/ARMCMx/chcore.h
 * @brief   ARM Cortex-Mx port macros and structures.
 *
 * @addtogroup IAR_ARMCMx_CORE
 * @{
 */

#ifndef _CHCORE_H_
#define _CHCORE_H_

#include <intrinsics.h>

#include "nvic.h"

/*===========================================================================*/
/* Port constants.                                                           */
/*===========================================================================*/

#define CORTEX_M0                   0   /**< @brief Cortex-M0 variant.      */
#define CORTEX_M1                   1   /**< @brief Cortex-M1 variant.      */
#define CORTEX_M3                   3   /**< @brief Cortex-M3 variant.      */
#define CORTEX_M4                   4   /**< @brief Cortex-M4 variant.      */

/* Inclusion of the Cortex-Mx implementation specific parameters.*/
#include "cmparams.h"

/* Cortex model check, only M0 and M3 supported right now.*/
#if (CORTEX_MODEL == CORTEX_M0) || (CORTEX_MODEL == CORTEX_M3)
#elif (CORTEX_MODEL == CORTEX_M1) || (CORTEX_MODEL == CORTEX_M4)
#warning "untested Cortex-M model"
#else
#error "unknown or unsupported Cortex-M model"
#endif

/*===========================================================================*/
/* Port statically derived parameters.                                       */
/*===========================================================================*/

/**
 * @brief   Total priority levels.
 */
#define CORTEX_PRIORITY_LEVELS      (1 << CORTEX_PRIORITY_BITS)

/**
 * @brief   Minimum priority level.
 * @details This minimum priority level is calculated from the number of
 *          priority bits supported by the specific Cortex-Mx implementation.
 */
#define CORTEX_MINIMUM_PRIORITY     (CORTEX_PRIORITY_LEVELS - 1)

/**
 * @brief   Maximum priority level.
 * @details The maximum allowed priority level is always zero.
 */
#define CORTEX_MAXIMUM_PRIORITY     0

/**
 * @brief   Disabled value for BASEPRI register.
 * @note    ARMv7-M architecture only.
 */
#define CORTEX_BASEPRI_DISABLED     0

/*===========================================================================*/
/* Port macros.                                                              */
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
/* Port configurable parameters.                                             */
/*===========================================================================*/

/**
 * @brief   Enables the use of the WFI instruction in the idle thread loop.
 */
#ifndef CORTEX_ENABLE_WFI_IDLE
#define CORTEX_ENABLE_WFI_IDLE      FALSE
#endif

/**
 * @brief   SYSTICK handler priority.
 * @note    The default SYSTICK handler priority is calculated as the priority
 *          level in the middle of the numeric priorities range.
 */
#ifndef CORTEX_PRIORITY_SYSTICK
#define CORTEX_PRIORITY_SYSTICK     (CORTEX_PRIORITY_LEVELS >> 1)
#else
/* If it is externally redefined then better perform a validity check on it.*/
#if !CORTEX_IS_VALID_PRIORITY(CORTEX_PRIORITY_SYSTICK)
#error "invalid priority level specified for CORTEX_PRIORITY_SYSTICK"
#endif
#endif

/**
 * @brief   SVCALL handler priority.
 * @note    The default SVCALL handler priority is calculated as
 *          @p CORTEX_MAXIMUM_PRIORITY+1, in the ARMv7-M port this reserves
 *          the @p CORTEX_MAXIMUM_PRIORITY priority level as fast interrupts
 *          priority level.
 * @note    The SVCALL vector is only used in the ARMv7-M port, it is available
 *          to user in the ARMv6-M port.
 */
#ifndef CORTEX_PRIORITY_SVCALL
#define CORTEX_PRIORITY_SVCALL      (CORTEX_MAXIMUM_PRIORITY + 1)
#else
/* If it is externally redefined then better perform a validity check on it.*/
#if !CORTEX_IS_VALID_PRIORITY(CORTEX_PRIORITY_SVCALL)
#error "invalid priority level specified for CORTEX_PRIORITY_SVCALL"
#endif
#endif

/**
 * @brief   PENDSV handler priority.
 * @note    The default PENDSV handler priority is set at the
 *          @p CORTEX_MINIMUM_PRIORITY priority level.
 * @note    The PENDSV vector is only used in the ARMv7-M legacy port, it is
 *          available to user in the ARMv6-M and ARMv7-M ports.
 * @note    In the ARMv7-M legacy port this value should be not changed from
 *          the minimum priority level.
 */
#ifndef CORTEX_PRIORITY_PENDSV
#define CORTEX_PRIORITY_PENDSV      CORTEX_MINIMUM_PRIORITY
#else
/* If it is externally redefined then better perform a validity check on it.*/
#if !CORTEX_IS_VALID_PRIORITY(CORTEX_PRIORITY_PENDSV)
#error "invalid priority level specified for CORTEX_PRIORITY_PENDSV"
#endif
#endif

/**
 * @brief   BASEPRI level within kernel lock.
 * @note    This value must not mask the SVCALL priority level or the
 *          kernel would hard fault.
 * @note    ARMv7-M architecture only.
 */
#ifndef CORTEX_BASEPRI_KERNEL
#define CORTEX_BASEPRI_KERNEL                                               \
  CORTEX_PRIORITY_MASK(CORTEX_PRIORITY_SVCALL+1)
#endif

/**
 * @brief   Stack alignment enforcement.
 * @note    The default value is 64 in order to comply with EABI, reducing
 *          the value to 32 can save some RAM space if you don't care about
 *          binary compatibility with EABI compiled libraries.
 * @note    Allowed values are 32 or 64.
 */
#ifndef CORTEX_STACK_ALIGNMENT
#define CORTEX_STACK_ALIGNMENT      64
#endif

/*===========================================================================*/
/* Port exported info.                                                       */
/*===========================================================================*/

/**
 * @brief   Macro defining a generic ARM architecture.
 */
#define CH_ARCHITECTURE_ARM

#if defined(__DOXYGEN__)
/**
 * @brief   Macro defining the specific ARM architecture.
 * @note    This macro is for documentation only, the real name changes
 *          depending on the selected architecture, the possible names are:
 *          - CH_ARCHITECTURE_ARM_v6M.
 *          - CH_ARCHITECTURE_ARM_v7M.
 *          .
 */
#define CH_ARCHITECTURE_ARM_vxm

/**
 * @brief   Name of the implemented architecture.
 * @note    The value is for documentation only, the real value changes
 *          depending on the selected architecture, the possible values are:
 *          - "ARMv6-M".
 *          - "ARMv7-M".
 *          - "ARMv7-ME".
 *          .
 */
#define CH_ARCHITECTURE_NAME        "ARMvx-M"

/**
 * @brief   Name of the architecture variant (optional).
 * @note    The value is for documentation only, the real value changes
 *          depending on the selected architecture, the possible values are:
 *          - "Cortex-M0"
 *          - "Cortex-M1"
 *          - "Cortex-M3"
 *          - "Cortex-M4"
 *          .
 */
#define CH_CORE_VARIANT_NAME        "Cortex-Mx"

#elif CORTEX_MODEL == CORTEX_M4
#define CH_ARCHITECTURE_ARM_v7M
#define CH_ARCHITECTURE_NAME        "ARMv7-ME"
#define CH_CORE_VARIANT_NAME        "Cortex-M4"
#elif CORTEX_MODEL == CORTEX_M3
#define CH_ARCHITECTURE_ARM_v7M
#define CH_ARCHITECTURE_NAME        "ARMv7-M"
#define CH_CORE_VARIANT_NAME        "Cortex-M3"
#elif CORTEX_MODEL == CORTEX_M1
#define CH_ARCHITECTURE_ARM_v6M
#define CH_ARCHITECTURE_NAME        "ARMv6-M"
#define CH_CORE_VARIANT_NAME        "Cortex-M1"
#elif CORTEX_MODEL == CORTEX_M0
#define CH_ARCHITECTURE_ARM_v6M
#define CH_ARCHITECTURE_NAME        "ARMv6-M"
#define CH_CORE_VARIANT_NAME        "Cortex-M0"
#endif

/*===========================================================================*/
/* Port implementation part (common).                                        */
/*===========================================================================*/

/**
 * @brief   Stack and memory alignment enforcement.
 */
#if (CORTEX_STACK_ALIGNMENT == 64) || defined(__DOXYGEN__)
typedef uint64_t stkalign_t;
#elif CORTEX_STACK_ALIGNMENT == 32
typedef uint32_t stkalign_t;
#else
#error "invalid stack alignment selected"
#endif

/**
 * @brief   Generic ARM register.
 */
typedef void *regarm_t;

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
#define WORKING_AREA(s, n) stkalign_t s[THD_WA_SIZE(n) / sizeof(stkalign_t)]

/* Includes the architecture-specific implementation part.*/
#if defined(CH_ARCHITECTURE_ARM_v6M)
#include "chcore_v6m.h"
#elif defined(CH_ARCHITECTURE_ARM_v7M)
#include "chcore_v7m.h"
#endif

#endif /* _CHCORE_H_ */

/** @} */
