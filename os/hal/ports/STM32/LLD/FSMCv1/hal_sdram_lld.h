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
/*
   SDRAM routines added by Nick Klimov aka progfin.
 */

/**
 * @file    hal_sdram.h
 * @brief   SDRAM Driver subsystem low level driver header.
 *
 * @addtogroup SDRAM
 * @{
 */

#ifndef HAL_SDRAM_LLD_H_
#define HAL_SDRAM_LLD_H_

#include "hal_fsmc.h"

#if (HAL_USE_SDRAM == TRUE) || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver constants.                                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Driver pre-compile time settings.                                         */
/*===========================================================================*/
/**
 * @name    Configuration options
 * @{
 */

/**
 * @brief   SDRAM driver enable switch.
 * @details If set to @p TRUE the support for SDRAM1 is included.
 */
#if !defined(STM32_SDRAM_USE_SDRAM1) || defined(__DOXYGEN__)
#define STM32_SDRAM_USE_SDRAM1                  FALSE
#else
#define STM32_SDRAM1_MAP_BASE                        FSMC_Bank5_MAP_BASE
#endif

/**
 * @brief   SDRAM driver enable switch.
 * @details If set to @p TRUE the support for SDRAM2 is included.
 */
#if !defined(STM32_SDRAM_USE_SDRAM2) || defined(__DOXYGEN__)
#define STM32_SDRAM_USE_SDRAM2                  FALSE
#else
#define STM32_SDRAM2_MAP_BASE                        FSMC_Bank6_MAP_BASE
#endif

/** @} */

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

#if !STM32_SDRAM_USE_SDRAM1 && !STM32_SDRAM_USE_SDRAM2
#error "SDRAM driver activated but no SDRAM peripheral assigned"
#endif

#if (STM32_SDRAM_USE_SDRAM1 || STM32_SDRAM_USE_SDRAM2) && !STM32_HAS_FSMC
#error "FMC not present in the selected device"
#endif

#if (defined(STM32F427xx) || defined(STM32F437xx) || \
     defined(STM32F429xx) || defined(STM32F439xx) || \
     defined(STM32F745xx) || defined(STM32F746xx) || \
     defined(STM32F756xx) || defined(STM32F767xx) || \
     defined(STM32F769xx) || defined(STM32F777xx) || \
     defined(STM32F779xx))
#else
#error "Device is not compatible with SDRAM"
#endif

/*===========================================================================*/
/* Driver data structures and types.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Driver macros.                                                            */
/*===========================================================================*/

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

extern SDRAMDriver SDRAMD1;

#ifdef __cplusplus
extern "C" {
#endif
  void sdram_lld_start(SDRAMDriver *sdramp, const SDRAMConfig *cfgp);
  void sdram_lld_stop(SDRAMDriver *sdramp);
#ifdef __cplusplus
}
#endif

#endif /* STM32_SDRAM_USE_SDRAM */

#endif /* HAL_SDRAM_LLD_H_ */

/** @} */
