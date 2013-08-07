/*
    ChibiOS/RT - Copyright (C) 2006-2013 Giovanni Di Sirio

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
 * @file    common/ARMCMx/nvic.h
 * @brief   Cortex-Mx NVIC support macros and structures.
 *
 * @addtogroup COMMON_ARMCMx_NVIC
 * @{
 */

#ifndef _NVIC_H_
#define _NVIC_H_

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
#define NVIC_PRIORITY_MASK(prio) ((prio) << (8 - __NVIC_PRIO_BITS))

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#ifdef __cplusplus
extern "C" {
#endif
  void nvicEnableVector(uint32_t n, uint32_t prio);
  void nvicDisableVector(uint32_t n);
#ifdef __cplusplus
}
#endif

#endif /* _NVIC_H_ */

/** @} */
