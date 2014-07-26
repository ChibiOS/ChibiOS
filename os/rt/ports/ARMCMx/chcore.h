/*
    ChibiOS/RT - Copyright (C) 2006,2007,2008,2009,2010,
                 2011,2012,2013,2014 Giovanni Di Sirio.

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
#define PORT_ARCHITECTURE_ARM

/* The following code is not processed when the file is included from an
   asm module because those intrinsic macros are not necessarily defined
   by the assembler too.*/
#if !defined(_FROM_ASM_)

/**
 * @brief   Compiler name and version.
 */
#if defined(__GNUC__) || defined(__DOXYGEN__)
#define PORT_COMPILER_NAME              "GCC " __VERSION__

#elif defined(__ICCARM__)
#define PORT_COMPILER_NAME              "IAR"

#elif defined(__CC_ARM)
#define PORT_COMPILER_NAME              "RVCT"

#else
#error "unsupported compiler"
#endif

#endif /* !defined(_FROM_ASM_) */

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

/**
 * @brief   Enables an alternative timer implementation.
 * @details Usually the port uses a timer interface defined in the file
 *          @p chcore_timer.h, if this option is enabled then the file
 *          @p chcore_timer_alt.h is included instead.
 */
#if !defined(PORT_USE_ALT_TIMER)
#define PORT_USE_ALT_TIMER              FALSE
#endif

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

/* The following code is not processed when the file is included from an
   asm module.*/
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

/* The following code is not processed when the file is included from an
   asm module.*/
#if !defined(_FROM_ASM_)

/**
 * @brief   Type of system time.
 */
#if (CH_CFG_ST_RESOLUTION == 32) || defined(__DOXYGEN__)
typedef uint32_t systime_t;
#else
typedef uint16_t systime_t;
#endif

/**
 * @brief   Type of a generic ARM register.
 */
typedef void *regarm_t;

/**
 * @brief   Type of stack and memory alignment enforcement.
 * @note    In this architecture the stack alignment is enforced to 64 bits,
 *          32 bits alignment is supported by hardware but deprecated by ARM,
 *          the implementation choice is to not offer the option.
 */
typedef uint64_t stkalign_t;

/* The following declarations are there just for Doxygen documentation, the
   real declarations are inside the sub-headers being specific for the
   sub-architectures.*/
#if defined(__DOXYGEN__)
/**
 * @brief   Interrupt saved context.
 * @details This structure represents the stack frame saved during a
 *          preemption-capable interrupt handler.
 * @note    It is implemented to match the Cortex-Mx exception context.
 */
struct port_extctx {};

/**
 * @brief   System saved context.
 * @details This structure represents the inner stack frame during a context
 *          switch.
 */
struct port_intctx {};
#endif /* defined(__DOXYGEN__) */

/**
 * @brief   Platform dependent part of the @p thread_t structure.
 * @details In this port the structure just holds a pointer to the
 *          @p port_intctx structure representing the stack pointer
 *          at context switch time.
 */
struct context {
  struct port_intctx *r13;
};

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
#define CORTEX_PRIO_MASK(n)                                                 \
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

#if !defined(_FROM_ASM_)

#if CH_CFG_ST_TIMEDELTA > 0
#if !PORT_USE_ALT_TIMER
#include "chcore_timer.h"
#else /* PORT_USE_ALT_TIMER */
#include "chcore_timer_alt.h"
#endif /* PORT_USE_ALT_TIMER */
#endif /* CH_CFG_ST_TIMEDELTA > 0 */

#endif /* !defined(_FROM_ASM_) */

#endif /* _CHCORE_H_ */

/** @} */
