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
 * @file    pc.c
 * @brief   Pin Change Driver code.
 *
 * @addtogroup PC
 * @{
 */

#include "hal.h"

#if (HAL_USE_PC == TRUE) || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver local definitions.                                                 */
/*===========================================================================*/

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local variables and types.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

/**
 * @brief   PC Driver initialization.
 * @note    This function is implicitly invoked by @p halInit(), there is
 *          no need to explicitly initialize the driver.
 *
 * @init
 */
void pcInit(void) {
  pc_lld_init();
}

/**
 * @brief   Initializes the standard part of a @p PCDriver structure.
 *
 * @param[out] pcp     pointer to the @p PCDriver object
 *
 * @init
 */
void pcObjectInit(PCDriver *pcp) {

  pcp->state  = PC_STOP;
  pcp->config = NULL;
}

/**
 * @brief   Configures and activates the PC peripheral.
 * @post    After activation all PC channels are in the selected state,
 *          use @p pcChannelEnable() and  @p pcChannelDisable()
 *          in order to activate them.
 *
 * @param[in] pcp       pointer to the @p PCDriver object
 * @param[in] config    pointer to the @p PCConfig object
 *
 * @api
 */
void pcStart(PCDriver *pcp, const PCConfig *config) {

  osalDbgCheck((pcp != NULL) && (config != NULL));

  osalSysLock();
  osalDbgAssert((pcp->state == PC_STOP) || (pcp->state == PC_ACTIVE),
                "invalid state");
  pcp->config = config;
  pc_lld_start(pcp);
  pcp->state = PC_ACTIVE;
  osalSysUnlock();
}

/**
 * @brief   Deactivates the PC peripheral.
 *
 * @param[in] pcp      pointer to the @p PCDriver object
 *
 * @api
 */
void pcStop(PCDriver *pcp) {

  osalDbgCheck(pcp != NULL);

  osalSysLock();
  osalDbgAssert((pcp->state == PC_STOP) || (pcp->state == PC_ACTIVE),
                "invalid state");
  pc_lld_stop(pcp);
  pcp->state = PC_STOP;
  osalSysUnlock();
}

/**
 * @brief   Enables an PC channel.
 * @pre     The channel must not be in @p PC_CH_MODE_DISABLED mode.
 *
 * @param[in] pcp      pointer to the @p PCDriver object
 * @param[in] channel   channel to be enabled
 *
 * @api
 */
void pcChannelEnable(PCDriver *pcp, pcchannel_t channel) {

  osalDbgCheck((pcp != NULL) && (channel < (pcchannel_t)PC_MAX_CHANNELS));

  osalSysLock();
  pcChannelEnableI(pcp, channel);
  osalSysUnlock();
}

/**
 * @brief   Disables an PC channel.
 * @pre     The channel must not be in @p PC_CH_MODE_DISABLED mode.
 *
 * @param[in] pcp      pointer to the @p PCDriver object
 * @param[in] channel  channel to be disabled
 *
 * @api
 */
void pcChannelDisable(PCDriver *pcp, pcchannel_t channel) {

  osalDbgCheck((pcp != NULL) && (channel < (pcchannel_t)PC_MAX_CHANNELS));

  osalSysLock();
  pcChannelDisableI(pcp, channel);
  osalSysUnlock();
}
#endif /* HAL_USE_PC == TRUE */

/** @} */
