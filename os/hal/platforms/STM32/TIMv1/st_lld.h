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
 * @file    STM32/st_lld.h
 * @brief   ST Driver subsystem low level driver header.
 *
 * @addtogroup ST
 * @{
 */

#ifndef _ST_LLD_H_
#define _ST_LLD_H_

#if (OSAL_ST_MODE != OSAL_ST_MODE_NONE) || defined(__DOXYGEN__)

#include "stm32_tim.h"

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
 * @brief   SysTick timer priority.
 */
#if !defined(ST_TIMER_PRIORITY) || defined(__DOXYGEN__)
#define ST_TIMER_PRIORITY                   8
#endif
/** @} */

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

#if !(OSAL_ST_MODE == OSAL_ST_MODE_PERIODIC) &&                             \
    !(OSAL_ST_MODE == OSAL_ST_MODE_FREERUNNING)
#error "invalid OSAL_ST_MODE setting in osal.h"
#endif

#if (OSAL_ST_MODE == OSAL_ST_MODE_FREERUNNING) && !STM32_HAS_TIM2
#error "TIM2 not present in the selected device"
#endif

#if (OSAL_ST_MODE == OSAL_ST_MODE_FREERUNNING) && !STM32_TIM2_IS_32BITS
#error "TIM2 is not a 32 bits timer"
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

#ifdef __cplusplus
extern "C" {
#endif
  void st_lld_init(void);
#ifdef __cplusplus
}
#endif

/*===========================================================================*/
/* Driver inline functions.                                                  */
/*===========================================================================*/

#endif /* OSAL_ST_MODE != OSAL_ST_MODE_NONE */

#endif /* _ST_LLD_H_ */

/** @} */
