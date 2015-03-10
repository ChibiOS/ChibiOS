/*
    ChibiOS - Copyright (C) 2006..2015 Giovanni Di Sirio

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
 * @file    cmparams.h
 * @brief   ARM Cortex-M4 parameters.
 *
 * @defgroup ARMCMx_ARMCM4 ARM Cortex-M4 Specific Parameters
 * @ingroup ARMCMx_SPECIFIC
 * @details This file contains the Cortex-M4 specific parameters for a
 *          generic platform.
 * @{
 */

#ifndef _CMPARAMS_H_
#define _CMPARAMS_H_

/**
 * @brief   Cortex core model.
 */
#define CORTEX_MODEL            CORTEX_M4

/**
 * @brief   Floating Point unit presence.
 */
#define CORTEX_HAS_FPU          1

/**
 * @brief   Number of bits in priority masks.
 */
#define CORTEX_PRIORITY_BITS    4

/**
 * @brief   Number of interrupt vectors.
 * @note    This number does not include the 16 system vectors and must be
 *          rounded to a multiple of 8.
 */
#define CORTEX_NUM_VECTORS      96

#if !defined(_FROM_ASM_)

/*
 * Replace the following inclusion with your vendor-provided CMSIS
 * device file.
 */
#define STM32F407xx
#include "stm32f4xx.h"

#endif /* !defined(__FROM_ASM__) */

#endif /* _CMPARAMS_H_ */

/** @} */
