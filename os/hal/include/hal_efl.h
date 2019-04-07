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
 * @file    hal_efl.h
 * @brief   Embedded Flash Driver macros and structures.
 *
 * @addtogroup HAL_EFL
 * @{
 */

#ifndef HAL_EFL_H
#define HAL_EFL_H

#if (HAL_USE_EFL == TRUE) || defined(__DOXYGEN__)

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

/**
 * @brief Driver state machine possible states.
 */
typedef enum {
  EFL_UNINIT = 0,                   /**< Not initialized.                   */
  EFL_STOP = 1,                     /**< Stopped.                           */
  EFL_READY = 2                     /**< Ready.                             */
} sdstate_t;

#include "hal_efl_lld.h"

/**
 * @brief   @p EFlashDriver specific methods.
 */
#define _efl_flash_methods_alone

/**
 * @brief   @p EFlashDriver specific methods with inherited ones.
 */
#define _efl_flash_methods                                                  \
  _base_flash_methods                                                       \
  _efl_flash_methods_alone

/**
 * @extends BaseFlashVMT
 *
 * @brief   @p EFlash virtual methods table.
 */
struct EFlashDriverVMT {
  _efl_flash_methods
};

/**
 * @brief   Type of a structure representing a flash driver configuration.
 */
typedef struct hal_efl_config {

} EFlashConfig;

/**
 * @extends BaseFlash
 *
 * @brief   Type of external flash driver class.
 */
typedef struct hal_efl_driver {
  /**
   * @brief   SNORDriver Virtual Methods Table.
   */
  const struct EFlashDriverVMT  *vmt;
  _base_flash_data
  /**
   * @brief   Current configuration data.
   */
  const EFlashConfig            *config;
} EFlashDriver;

/*===========================================================================*/
/* Driver macros.                                                            */
/*===========================================================================*/

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#ifdef __cplusplus
extern "C" {
#endif
  void eflInit(void);
  void eflObjectInit(SerialDriver *sdp);
#ifdef __cplusplus
}
#endif

#endif /* HAL_USE_EFL == TRUE */

#endif /* HAL_EFL_H */

/** @} */
