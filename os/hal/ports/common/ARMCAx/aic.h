/*
    ChibiOS - Copyright (C) 2006..2016 Giovanni Di Sirio

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
 * @file    common/ARMCMAx/aic.h
 * @brief   Cortex-MAx AIC support macros and structures.
 *
 * @addtogroup COMMON_ARMCMAx_AIC
 * @{
 */

#ifndef AIC_H
#define AIC_H

/*===========================================================================*/
/* Driver constants.                                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Driver pre-compile time settings.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

/*===========================================================================*/
/* Driver data structures and types.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Driver macros.                                                            */
/*===========================================================================*/

/**
 * @brief   Priority level to priority mask conversion macro.
 */

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#ifdef __cplusplus
extern "C" {
#endif
  void aicInit(void);
  void aicConfigureIt(uint32_t source, uint8_t prior);
  void aicSetSourceVector(uint32_t source, bool (*handler)(void));
  void aicSetSpuriousVector(bool (*handler)(void));
  void aicEnableIt(uint32_t source);
  void aicDisableIt(uint32_t source);
  void aicClearIt(uint32_t source);
  void aicSetIt(uint32_t source);
#ifdef __cplusplus
}
#endif

#endif /* AIC_H */

/** @} */
