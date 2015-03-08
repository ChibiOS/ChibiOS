/*
    ChibiOS - Copyright (C) 2006..2015 Giovanni Di Sirio.

    This file is part of ChibiOS.

    ChibiOS is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 3 of the License, or
    (at your option) any later version.

    ChibiOS is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

/**
 * @file    ext.c
 * @brief   EXT Driver code.
 *
 * @addtogroup EXT
 * @{
 */

#include "hal.h"

#if (HAL_USE_EXT == TRUE) || defined(__DOXYGEN__)

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
 * @brief   EXT Driver initialization.
 * @note    This function is implicitly invoked by @p halInit(), there is
 *          no need to explicitly initialize the driver.
 *
 * @init
 */
void extInit(void) {

  ext_lld_init();
}

/**
 * @brief   Initializes the standard part of a @p EXTDriver structure.
 *
 * @param[out] extp     pointer to the @p EXTDriver object
 *
 * @init
 */
void extObjectInit(EXTDriver *extp) {

  extp->state  = EXT_STOP;
  extp->config = NULL;
}

/**
 * @brief   Configures and activates the EXT peripheral.
 * @post    After activation all EXT channels are in the disabled state,
 *          use @p extChannelEnable() in order to activate them.
 *
 * @param[in] extp      pointer to the @p EXTDriver object
 * @param[in] config    pointer to the @p EXTConfig object
 *
 * @api
 */
void extStart(EXTDriver *extp, const EXTConfig *config) {

  osalDbgCheck((extp != NULL) && (config != NULL));

  osalSysLock();
  osalDbgAssert((extp->state == EXT_STOP) || (extp->state == EXT_ACTIVE),
                "invalid state");
  extp->config = config;
  ext_lld_start(extp);
  extp->state = EXT_ACTIVE;
  osalSysUnlock();
}

/**
 * @brief   Deactivates the EXT peripheral.
 *
 * @param[in] extp      pointer to the @p EXTDriver object
 *
 * @api
 */
void extStop(EXTDriver *extp) {

  osalDbgCheck(extp != NULL);

  osalSysLock();
  osalDbgAssert((extp->state == EXT_STOP) || (extp->state == EXT_ACTIVE),
                "invalid state");
  ext_lld_stop(extp);
  extp->state = EXT_STOP;
  osalSysUnlock();
}

/**
 * @brief   Enables an EXT channel.
 * @pre     The channel must not be in @p EXT_CH_MODE_DISABLED mode.
 *
 * @param[in] extp      pointer to the @p EXTDriver object
 * @param[in] channel   channel to be enabled
 *
 * @api
 */
void extChannelEnable(EXTDriver *extp, expchannel_t channel) {

  osalDbgCheck((extp != NULL) && (channel < (expchannel_t)EXT_MAX_CHANNELS));

  osalSysLock();
  osalDbgAssert((extp->state == EXT_ACTIVE) &&
                ((extp->config->channels[channel].mode &
                  EXT_CH_MODE_EDGES_MASK) != EXT_CH_MODE_DISABLED),
                "invalid state");
  extChannelEnableI(extp, channel);
  osalSysUnlock();
}

/**
 * @brief   Disables an EXT channel.
 * @pre     The channel must not be in @p EXT_CH_MODE_DISABLED mode.
 *
 * @param[in] extp      pointer to the @p EXTDriver object
 * @param[in] channel   channel to be disabled
 *
 * @api
 */
void extChannelDisable(EXTDriver *extp, expchannel_t channel) {

  osalDbgCheck((extp != NULL) && (channel < (expchannel_t)EXT_MAX_CHANNELS));

  osalSysLock();
  osalDbgAssert((extp->state == EXT_ACTIVE) &&
                ((extp->config->channels[channel].mode &
                  EXT_CH_MODE_EDGES_MASK) != EXT_CH_MODE_DISABLED),
                "invalid state");
  extChannelDisableI(extp, channel);
  osalSysUnlock();
}

/**
 * @brief   Changes the operation mode of a channel.
 * @note    This function attempts to write over the current configuration
 *          structure that must have been not declared constant. This
 *          violates the @p const qualifier in @p extStart() but it is
 *          intentional.
 * @note    This function cannot be used if the configuration structure is
 *          declared @p const.
 * @note    The effect of this function on constant configuration structures
 *          is not defined.
 *
 * @param[in] extp      pointer to the @p EXTDriver object
 * @param[in] channel   channel to be changed
 * @param[in] extcp     new configuration for the channel
 *
 * @iclass
 */
void extSetChannelModeI(EXTDriver *extp,
                        expchannel_t channel,
                        const EXTChannelConfig *extcp) {
  EXTChannelConfig *oldcp;

  osalDbgCheck((extp != NULL) &&
               (channel < (expchannel_t)EXT_MAX_CHANNELS) &&
               (extcp != NULL));

  osalDbgAssert(extp->state == EXT_ACTIVE, "invalid state");

  /* Note that here the access is enforced as non-const, known access
     violation.*/
  oldcp = (EXTChannelConfig *)&extp->config->channels[channel];

  /* Overwiting the old channels configuration then the channel is reconfigured
     by the low level driver.*/
  *oldcp = *extcp;
  ext_lld_channel_enable(extp, channel);
}

#endif /* HAL_USE_EXT == TRUE */

/** @} */
