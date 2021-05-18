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
 * @file    RCCv1/stm32_apb1.inc
 * @brief   Shared APB1 clock handler.
 *
 * @addtogroup STM32_APB1_HANDLER
 * @{
 */

/*===========================================================================*/
/* Driver local definitions.                                                 */
/*===========================================================================*/

/**
 * @name    PPRE1 field bits definitions
 * @{
 */
#define STM32_PPRE1_MASK        (7U << 8U)   /**< PPRE1 field mask.         */
#define STM32_PPRE1_FIELD(n)    ((n) << 8U)  /**< PPRE1 field value.        */
#define STM32_PPRE1_DIV1        STM32_PPRE1_FIELD(0U)
#define STM32_PPRE1_DIV2        STM32_PPRE1_FIELD(4U)
#define STM32_PPRE1_DIV4        STM32_PPRE1_FIELD(5U)
#define STM32_PPRE1_DIV8        STM32_PPRE1_FIELD(6U)
#define STM32_PPRE1_DIV16       STM32_PPRE1_FIELD(7U)
/** @} */

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

/* Registry checks for robustness.*/

/* Checks on configurations.*/
#if !defined(STM32_PPRE1)
#error "STM32_PPRE1 not defined in mcuconf.h"
#endif

/* Input checks.*/
#if !defined(STM32_PCLK1_MAX)
#error "STM32_PCLK1_MAX not defined in hal_lld.h"
#endif

#if !defined(STM32_HCLK)
#error "STM32_HCLK not defined in hal_lld.h"
#endif

/**
 * @brief   APB1 frequency.
 */
#if (STM32_PPRE1 == STM32_PPRE1_DIV1) || defined(__DOXYGEN__)
#define STM32_PCLK1                 (STM32_HCLK / 1)

#elif STM32_PPRE1 == STM32_PPRE1_DIV2
#define STM32_PCLK1                 (STM32_HCLK / 2)

#elif STM32_PPRE1 == STM32_PPRE1_DIV4
#define STM32_PCLK1                 (STM32_HCLK / 4)

#elif STM32_PPRE1 == STM32_PPRE1_DIV8
#define STM32_PCLK1                 (STM32_HCLK / 8)

#elif STM32_PPRE1 == STM32_PPRE1_DIV16
#define STM32_PCLK1                 (STM32_HCLK / 16)

#else
#error "invalid STM32_PPRE1 value specified"
#endif

/*
 * APB1 frequency check.
 */
#if STM32_PCLK1 > STM32_PCLK1_MAX
#error "STM32_PCLK1 exceeding maximum frequency (STM32_PCLK1_MAX)"
#endif

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local variables.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Driver interrupt handlers.                                                */
/*===========================================================================*/

/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

/** @} */
