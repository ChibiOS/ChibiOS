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
 * @file    hal_fsmc_sram.h
 * @brief   SRAM Driver subsystem low level driver header.
 *
 * @addtogroup SRAM
 * @{
 */

#ifndef HAL_SRAM_LLD_H_
#define HAL_SRAM_LLD_H_

#include "hal_fsmc.h"

#if (HAL_USE_SRAM == TRUE) || defined(__DOXYGEN__)

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

/** @} */

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

/*===========================================================================*/
/* Driver data structures and types.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Driver macros.                                                            */
/*===========================================================================*/

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#if STM32_SRAM_USE_SRAM1 && !defined(__DOXYGEN__)
extern SRAMDriver SRAMD1;
#endif

#if STM32_SRAM_USE_SRAM2 && !defined(__DOXYGEN__)
extern SRAMDriver SRAMD2;
#endif

#if STM32_SRAM_USE_SRAM3 && !defined(__DOXYGEN__)
extern SRAMDriver SRAMD3;
#endif

#if STM32_SRAM_USE_SRAM4 && !defined(__DOXYGEN__)
extern SRAMDriver SRAMD4;
#endif

#ifdef __cplusplus
extern "C" {
#endif
  void sram_lld_start(SRAMDriver *sramp, const SRAMConfig *cfgp);
  void sram_lld_stop(SRAMDriver *sramp);
#ifdef __cplusplus
}
#endif

#endif /* STM32_SRAM_USE_SRAM */

#endif /* HAL_SRAM_LLD_H_ */

/** @} */
