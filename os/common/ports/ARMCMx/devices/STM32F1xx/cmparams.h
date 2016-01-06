/*
    ChibiOS - Copyright (C) 2006..2015 Giovanni Di Sirio.

    This file is part of ChibiOS.

    ChibiOS is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 3 of the License, or
    (at your option) any later version.

    ChibiOS is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

/**
 * @file    STM32F1xx/cmparams.h
 * @brief   ARM Cortex-M3 parameters for the STM32F1xx.
 *
 * @defgroup ARMCMx_STM32F1xx STM32F1xx Specific Parameters
 * @ingroup ARMCMx_SPECIFIC
 * @details This file contains the Cortex-M4 specific parameters for the
 *          STM32F1xx platform.
 * @{
 */

#ifndef _CMPARAMS_H_
#define _CMPARAMS_H_

/**
 * @brief   Cortex core model.
 */
#define CORTEX_MODEL            3

/**
 * @brief   Floating Point unit presence.
 */
#define CORTEX_HAS_FPU          0

/**
 * @brief   Number of bits in priority masks.
 */
#define CORTEX_PRIORITY_BITS    4

/**
 * @brief   Number of interrupt vectors.
 * @note    This number does not include the 16 system vectors and must be
 *          rounded to a multiple of 8.
 */
#define CORTEX_NUM_VECTORS      72

/* The following code is not processed when the file is included from an
   asm module.*/
#if !defined(_FROM_ASM_)

/* If the device type is not externally defined, for example from the Makefile,
   then a file named board.h is included. This file must contain a device
   definition compatible with the vendor include file.*/
#if !defined(STM32F100xB) && !defined(STM32F100xE) &&                       \
    !defined(STM32F101x6) && !defined(STM32F101xB) &&                       \
    !defined(STM32F101xE) && !defined(STM32F101xG) &&                       \
    !defined(STM32F102x6) && !defined(STM32F102xB) &&                       \
    !defined(STM32F103x6) && !defined(STM32F103xB) &&                       \
    !defined(STM32F103xE) && !defined(STM32F103xG) &&                       \
    !defined(STM32F105xC) && !defined(STM32F107xC)
#include "board.h"
#endif

/* Including the device CMSIS header. Note, we are not using the definitions
   from this header because we need this file to be usable also from
   assembler source files. We verify that the info matches instead.*/
#include "stm32f1xx.h"

#if CORTEX_MODEL != __CORTEX_M
#error "CMSIS __CORTEX_M mismatch"
#endif

#if CORTEX_PRIORITY_BITS != __NVIC_PRIO_BITS
#error "CMSIS __NVIC_PRIO_BITS mismatch"
#endif

#endif /* !defined(_FROM_ASM_) */

#endif /* _CMPARAMS_H_ */

/** @} */
