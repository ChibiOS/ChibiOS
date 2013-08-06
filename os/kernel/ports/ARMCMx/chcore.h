/*
    ChibiOS/RT - Copyright (C) 2006,2007,2008,2009,2010,
                 2011,2012,2013 Giovanni Di Sirio.

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
 * @file    ARMCMx/chcore.h
 * @brief   ARM Cortex-Mx port macros and structures.
 *
 * @addtogroup ARMCMx_CORE
 * @{
 */

#ifndef _CHCORE_H_
#define _CHCORE_H_

/*===========================================================================*/
/* Module constants.                                                         */
/*===========================================================================*/

/**
 * @name    Architecture and Compiler
 * @{
 */
/**
 * @brief   Macro defining a generic ARM architecture.
 */
#define CH_ARCHITECTURE_ARM

/**
 * @brief   Name of the compiler supported by this port.
 */
#define CH_COMPILER_NAME                "GCC " __VERSION__
/** @} */

/**
 * @name    Cortex-M variants
 * @{
 */
#define CORTEX_M0                       0   /**< @brief Cortex-M0 variant.  */
#define CORTEX_M0PLUS                   1   /**< @brief Cortex-M0+ variant. */
#define CORTEX_M1                       2   /**< @brief Cortex-M1 variant.  */
#define CORTEX_M3                       3   /**< @brief Cortex-M3 variant.  */
#define CORTEX_M4                       4   /**< @brief Cortex-M4 variant.  */
/** @} */

/* Inclusion of the Cortex-Mx implementation specific parameters.*/
#include "cmparams.h"

/*===========================================================================*/
/* Module pre-compile time settings.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

#if !defined(_FROM_ASM_)

/*
 * Inclusion of the appropriate CMSIS header for the selected device.
 */
#if CORTEX_MODEL == CORTEX_M0
#include "core_cm0.h"
#elif CORTEX_MODEL == CORTEX_M0PLUS
#include "core_cm0plus.h"
#elif CORTEX_MODEL == CORTEX_M3
#include "core_cm3.h"
#elif CORTEX_MODEL == CORTEX_M4
#include "core_cm4.h"
#else
#error "unknown or unsupported Cortex-M model"
#endif

#endif /* !defined(_FROM_ASM_) */

/*===========================================================================*/
/* Module data structures and types.                                         */
/*===========================================================================*/

#if !defined(_FROM_ASM_)

/* The following declarations are there just for Doxygen documentation, the
   real declarations are inside the sub-headers being specific for the
   sub-architectures.*/
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

#endif /* !defined(_FROM_ASM_) */

/*===========================================================================*/
/* Module macros.                                                            */
/*===========================================================================*/

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

/**
 * @brief   Priority level verification macro.
 */
#define CORTEX_IS_VALID_PRIORITY(n)                                         \
  (((n) >= 0) && ((n) < CORTEX_PRIORITY_LEVELS))

/**
 * @brief   Priority level verification macro.
 */
#define CORTEX_IS_VALID_KERNEL_PRIORITY(n)                                  \
  (((n) >= CORTEX_MAX_KERNEL_PRIORITY) && ((n) < CORTEX_PRIORITY_LEVELS))

/**
 * @brief   Priority level to priority mask conversion macro.
 */
#define CORTEX_PRIORITY_MASK(n)                                             \
  ((n) << (8 - CORTEX_PRIORITY_BITS))

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

/*===========================================================================*/
/* Module inline functions.                                                  */
/*===========================================================================*/

/* Includes the sub-architecture-specific part.*/
#if (CORTEX_MODEL == CORTEX_M0) || (CORTEX_MODEL == CORTEX_M0PLUS) ||       \
    (CORTEX_MODEL == CORTEX_M1)
#include "chcore_v6m.h"
#elif (CORTEX_MODEL == CORTEX_M3) || (CORTEX_MODEL == CORTEX_M4)
#include "chcore_v7m.h"
#endif

#endif /* _CHCORE_H_ */

/** @} */
