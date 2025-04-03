/*
    ChibiOS - Copyright (C) 2006..2018 Giovanni Di Sirio

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

        http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.
*/

/**
 * @file    STM32L4xx/cmparams.h
 * @brief   ARM Cortex-M4 parameters for the STM32L4xx.
 *
 * @defgroup ARMCMx_STM32L4xx STM32L4xx Specific Parameters
 * @ingroup ARMCMx_SPECIFIC
 * @details This file contains the Cortex-M4 specific parameters for the
 *          STM32L4xx platform.
 * @{
 */

#ifndef CMPARAMS_H
#define CMPARAMS_H

/**
 * @brief   Cortex core model.
 */
#define CORTEX_MODEL            4

/**
 * @brief   Floating Point unit presence.
 */
#define CORTEX_HAS_FPU          1

/**
 * @brief   Number of bits in priority masks.
 */
#define CORTEX_PRIORITY_BITS    4

/* If the device type is not externally defined, for example from the Makefile,
   then a file named board.h is included. This file must contain a device
   definition compatible with the vendor include file.*/
#if !defined(STM32L431xx) && !defined(STM32L432xx) &&                       \
    !defined(STM32L433xx) && !defined(STM32L442xx) &&                       \
    !defined(STM32L443xx) && !defined(STM32L451xx) &&                       \
    !defined(STM32L452xx) && !defined(STM32L462xx) &&                       \
    !defined(STM32L471xx) && !defined(STM32L475xx) &&                       \
    !defined(STM32L476xx) && !defined(STM32L485xx) &&                       \
    !defined(STM32L486xx) && !defined(STM32L496xx) &&                       \
    !defined(STM32L4A6xx) &&                                                \
    !defined(STM32L4R5xx) && !defined(STM32L4R7xx) &&                       \
    !defined(STM32L4R9xx) && !defined(STM32L4S5xx) &&                       \
    !defined(STM32L4S7xx) && !defined(STM32L4S9xx)
#include "board.h"
#endif

/**
 * @brief   Number of interrupt vectors.
 * @note    This number does not include the 16 system vectors and must be
 *          rounded to a multiple of 8.
 */
#if defined(STM32L496xx) || defined(STM32L4A6xx) || defined(STM32L4R5xx) || \
    defined(STM32L4R7xx) || defined(STM32L4R9xx) || defined(STM32L4S5xx) || \
    defined(STM32L4S7xx) || defined(STM32L4S9xx)
#define CORTEX_NUM_VECTORS      96
#else
#define CORTEX_NUM_VECTORS      88
#endif

/**
 * @brief   Number of MPU regions.
 */
#define CORTEX_MPU_REGIONS      8

/* The following code is not processed when the file is included from an
   asm module.*/
#if !defined(_FROM_ASM_)

/* Including the device CMSIS header. Note, we are not using the definitions
   from this header because we need this file to be usable also from
   assembler source files. We verify that the info matches instead.*/
#include "stm32l4xx.h"

/*lint -save -e9029 [10.4] Signedness comes from external files, it is
  unpredictable but gives no problems.*/
#if CORTEX_MODEL != __CORTEX_M
#error "CMSIS __CORTEX_M mismatch"
#endif

#if CORTEX_HAS_FPU != __FPU_PRESENT
#error "CMSIS __FPU_PRESENT mismatch"
#endif

#if CORTEX_PRIORITY_BITS != __NVIC_PRIO_BITS
#error "CMSIS __NVIC_PRIO_BITS mismatch"
#endif
/*lint -restore*/

#endif /* !defined(_FROM_ASM_) */

#endif /* CMPARAMS_H */

/** @} */
