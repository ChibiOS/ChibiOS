/*
    AVR Pin Change driver for ChibiOS
    Copyright (C) 2015 Igor Stoppa <igor.stoppa@gmail.com>

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
 * @file    pc.h
 * @brief   Pin Change Driver macros and structures.
 *
 * @addtogroup PC
 * @{
 */

#ifndef _PC_H_
#define _PC_H_

#if (HAL_USE_PC == TRUE) || defined(__DOXYGEN__)

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
 * @brief   Driver state machine possible states.
 */
typedef enum {
  PC_UNINIT = 0,                   /**< Not initialized.                   */
  PC_STOP = 1,                     /**< Stopped.                           */
  PC_ACTIVE = 2                    /**< Active.                            */
} pcstate_t;

/**
 * @brief   Type of a structure representing a PC driver.
 */
typedef struct PCDriver PCDriver;

#include "pc_lld.h"

/*===========================================================================*/
/* Driver macros.                                                            */
/*===========================================================================*/

/**
 * @name    Macro Functions
 * @{
 */
/**
 * @brief   Enables a PC channel.
 *
 * @param[in] pcp      pointer to the @p PCDriver object
 * @param[in] channel   channel to be enabled
 *
 * @iclass
 */
#define pcChannelEnableI(pcp, channel) pc_lld_channel_enable(pcp, channel)

/**
 * @brief   Disables a PC channel.
 *
 * @param[in] pcp      pointer to the @p PCDriver object
 * @param[in] channel   channel to be disabled
 *
 * @iclass
 */
#define pcChannelDisableI(pcp, channel) pc_lld_channel_disable(pcp, channel)

/** @} */

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#ifdef __cplusplus
extern "C" {
#endif
  void pcInit(void);
  void pcObjectInit(PCDriver *pcp);
  void pcStart(PCDriver *pcp, const PCConfig *config);
  void pcStop(PCDriver *pcp);
  void pcChannelEnable(PCDriver *pcp, pcchannel_t channel);
  void pcChannelDisable(PCDriver *pcp, pcchannel_t channel);
#ifdef __cplusplus
}
#endif

#endif /* HAL_USE_PC == TRUE */

#endif /* _PC_H_ */

/** @} */
