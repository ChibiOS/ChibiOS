/*
    ChibiOS/HAL - Copyright (C) 2014 Uladzimir Pylinsky aka barthess

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
 * @file    hal_fsmc_sram.c
 * @brief   SRAM Driver subsystem low level driver source.
 *
 * @addtogroup SRAM
 * @{
 */
#include "hal.h"

#if (HAL_USE_SRAM == TRUE) || defined(__DOXYGEN__)

#include "hal_sram_lld.h"

/*===========================================================================*/
/* Driver local definitions.                                                 */
/*===========================================================================*/

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/
/**
 * @brief   SRAM1 driver identifier.
 */
#if STM32_SRAM_USE_SRAM1 || defined(__DOXYGEN__)
SRAMDriver SRAMD1;
#endif

/**
 * @brief   SRAM2 driver identifier.
 */
#if STM32_SRAM_USE_SRAM2 || defined(__DOXYGEN__)
SRAMDriver SRAMD2;
#endif

/**
 * @brief   SRAM3 driver identifier.
 */
#if STM32_SRAM_USE_SRAM3 || defined(__DOXYGEN__)
SRAMDriver SRAMD3;
#endif

/**
 * @brief   SRAM4 driver identifier.
 */
#if STM32_SRAM_USE_SRAM4 || defined(__DOXYGEN__)
SRAMDriver SRAMD4;
#endif

/*===========================================================================*/
/* Driver local types.                                                       */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local variables and types.                                         */
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

/**
 * @brief   Configures and activates the SRAM peripheral.
 *
 * @param[in] sramp         pointer to the @p SRAMDriver object
 * @param[in] cfgp          pointer to the @p SRAMConfig object
 *
 * @notapi
 */
void sram_lld_start(SRAMDriver *sramp, const SRAMConfig *cfgp) {

    sramp->sram->BTR  = cfgp->btr;
    sramp->sram->BWTR = cfgp->bwtr;
    sramp->sram->BCR  = cfgp->bcr;
}

/**
 * @brief   Deactivates the SRAM peripheral.
 *
 * @param[in] sramp         pointer to the @p SRAMDriver object
 *
 * @notapi
 */
void sram_lld_stop(SRAMDriver *sramp) {

    uint32_t mask = FSMC_BCR_MBKEN;
#if (defined(STM32F427xx) || defined(STM32F437xx) || \
     defined(STM32F429xx) || defined(STM32F439xx) || \
     defined(STM32F745xx) || defined(STM32F746xx) || \
     defined(STM32F756xx) || defined(STM32F767xx) || \
     defined(STM32F769xx) || defined(STM32F777xx) || \
     defined(STM32F779xx))
    mask |= FSMC_BCR_CCLKEN;
#endif
    sramp->sram->BCR &= ~mask;
}

#endif /* STM32_SRAM_USE_SRAM */

/** @} */

