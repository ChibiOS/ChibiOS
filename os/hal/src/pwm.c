/*
    ChibiOS/RT - Copyright (C) 2010 Giovanni Di Sirio.

    This file is part of ChibiOS/RT.

    ChibiOS/RT is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 3 of the License, or
    (at your option) any later version.

    ChibiOS/RT is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program. If not, see <http://www.gnu.org/licenses/>.

                                      ---

    A special exception to the GPL can be applied should you wish to distribute
    a combined work that includes ChibiOS/RT, without being obliged to provide
    the source code for any proprietary components. See the file exception.txt
    for full details of how and when the exception can be applied.
*/

/**
 * @file PWM.c
 * @brief PWM Driver code.
 * @addtogroup PWM
 * @{
 */

#include "ch.h"
#include "hal.h"

#if CH_HAL_USE_PWM || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local variables.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

/**
 * @brief PWM Driver initialization.
 */
void pwmInit(void) {

  pwm_lld_init();
}

/**
 * @brief Initializes the standard part of a @p PWMDriver structure.
 *
 * @param[in] pwmp      pointer to a @p PWMDriver object
 */
void pwmObjectInit(PWMDriver *pwmp) {

  pwmp->pd_state    = PWM_STOP;
  pwmp->pd_config   = NULL;
}

/**
 * @brief Configures and activates the PWM peripheral.
 *
 * @param[in] pwmp      pointer to a @p PWMDriver object
 * @param[in] config    pointer to a @p PWMConfig object
 */
void pwmStart(PWMDriver *pwmp, const PWMConfig *config) {

  chDbgCheck((pwmp != NULL) && (config != NULL), "pwmStart");

  chSysLock();
  chDbgAssert((pwmp->pd_state == PWM_STOP) || (pwmp->pd_state == PWM_READY),
              "pwmStart(), #1",
              "invalid state");
  pwmp->pd_config = config;
  pwm_lld_start(pwmp);
  pwmp->pd_state = PWM_READY;
  chSysUnlock();
}

/**
 * @brief Deactivates the PWM peripheral.
 *
 * @param[in] pwmp      pointer to a @p PWMDriver object
 */
void pwmStop(PWMDriver *pwmp) {

  chDbgCheck(pwmp != NULL, "pwmStop");

  chSysLock();
  chDbgAssert((pwmp->pd_state == PWM_STOP) || (pwmp->pd_state == PWM_READY),
              "pwmStop(), #1",
              "invalid state");
  pwm_lld_stop(pwmp);
  pwmp->pd_state = PWM_STOP;
  chSysUnlock();
}

/**
 * @brief Enables a PWM channel.
 *
 * @param[in] pwmp      pointer to a @p PWMDriver object
 * @param[in] channel   PWM channel identifier
 * @param[in] width     PWM pulse width as clock pulses number
 */
void pwmEnableChannel(PWMDriver *pwmp,
                      pwmchannel_t channel,
                      pwmcnt_t width) {

  chDbgCheck((pwmp != NULL) && (channel < PWM_CHANNELS),
             "pwmEnableChannel");

  chSysLock();
  chDbgAssert(pwmp->pd_state == PWM_READY,
              "pwmEnableChannel(), #1", "invalid state");
  pwm_lld_enable_channel(pwmp, channel, width);
  chSysUnlock();
}

/**
 * @brief Disables a PWM channel.
 * @details The channel is disabled and its output line returned to the
 *          idle state.
 *
 * @param[in] pwmp      pointer to a @p PWMDriver object
 * @param[in] channel   PWM channel identifier
 */
void pwmDisableChannel(PWMDriver *pwmp, pwmchannel_t channel) {

  chDbgCheck((pwmp != NULL) && (channel < PWM_CHANNELS),
             "pwmEnableChannel");

  chSysLock();
  chDbgAssert(pwmp->pd_state == PWM_READY,
              "pwmDisableChannel(), #1", "invalid state");
  pwm_lld_disable_channel(pwmp, channel);
  chSysUnlock();
}

#endif /* CH_HAL_USE_PWM */

/** @} */
