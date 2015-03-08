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
 * @file    pwm.c
 * @brief   PWM Driver code.
 *
 * @addtogroup PWM
 * @{
 */

#include "hal.h"

#if (HAL_USE_PWM == TRUE) || defined(__DOXYGEN__)

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
 * @brief   PWM Driver initialization.
 * @note    This function is implicitly invoked by @p halInit(), there is
 *          no need to explicitly initialize the driver.
 *
 * @init
 */
void pwmInit(void) {

  pwm_lld_init();
}

/**
 * @brief   Initializes the standard part of a @p PWMDriver structure.
 *
 * @param[out] pwmp     pointer to a @p PWMDriver object
 *
 * @init
 */
void pwmObjectInit(PWMDriver *pwmp) {

  pwmp->state    = PWM_STOP;
  pwmp->config   = NULL;
  pwmp->enabled  = 0;
  pwmp->channels = 0;
#if defined(PWM_DRIVER_EXT_INIT_HOOK)
  PWM_DRIVER_EXT_INIT_HOOK(pwmp);
#endif
}

/**
 * @brief   Configures and activates the PWM peripheral.
 * @note    Starting a driver that is already in the @p PWM_READY state
 *          disables all the active channels.
 *
 * @param[in] pwmp      pointer to a @p PWMDriver object
 * @param[in] config    pointer to a @p PWMConfig object
 *
 * @api
 */
void pwmStart(PWMDriver *pwmp, const PWMConfig *config) {

  osalDbgCheck((pwmp != NULL) && (config != NULL));

  osalSysLock();
  osalDbgAssert((pwmp->state == PWM_STOP) || (pwmp->state == PWM_READY),
                "invalid state");
  pwmp->config = config;
  pwmp->period = config->period;
  pwm_lld_start(pwmp);
  pwmp->enabled = 0;
  pwmp->state = PWM_READY;
  osalSysUnlock();
}

/**
 * @brief   Deactivates the PWM peripheral.
 *
 * @param[in] pwmp      pointer to a @p PWMDriver object
 *
 * @api
 */
void pwmStop(PWMDriver *pwmp) {

  osalDbgCheck(pwmp != NULL);

  osalSysLock();
  osalDbgAssert((pwmp->state == PWM_STOP) || (pwmp->state == PWM_READY),
                "invalid state");
  pwm_lld_stop(pwmp);
  pwmp->enabled = 0;
  pwmp->state   = PWM_STOP;
  osalSysUnlock();
}

/**
 * @brief   Changes the period the PWM peripheral.
 * @details This function changes the period of a PWM unit that has already
 *          been activated using @p pwmStart().
 * @pre     The PWM unit must have been activated using @p pwmStart().
 * @post    The PWM unit period is changed to the new value.
 * @note    If a period is specified that is shorter than the pulse width
 *          programmed in one of the channels then the behavior is not
 *          guaranteed.
 *
 * @param[in] pwmp      pointer to a @p PWMDriver object
 * @param[in] period    new cycle time in ticks
 *
 * @api
 */
void pwmChangePeriod(PWMDriver *pwmp, pwmcnt_t period) {

  osalDbgCheck(pwmp != NULL);

  osalSysLock();
  osalDbgAssert(pwmp->state == PWM_READY, "invalid state");
  pwmChangePeriodI(pwmp, period);
  osalSysUnlock();
}

/**
 * @brief   Enables a PWM channel.
 * @pre     The PWM unit must have been activated using @p pwmStart().
 * @post    The channel is active using the specified configuration.
 * @note    Depending on the hardware implementation this function has
 *          effect starting on the next cycle (recommended implementation)
 *          or immediately (fallback implementation).
 *
 * @param[in] pwmp      pointer to a @p PWMDriver object
 * @param[in] channel   PWM channel identifier (0...channels-1)
 * @param[in] width     PWM pulse width as clock pulses number
 *
 * @api
 */
void pwmEnableChannel(PWMDriver *pwmp,
                      pwmchannel_t channel,
                      pwmcnt_t width) {

  osalDbgCheck((pwmp != NULL) && (channel < pwmp->channels));

  osalSysLock();

  osalDbgAssert(pwmp->state == PWM_READY, "not ready");

  pwmEnableChannelI(pwmp, channel, width);

  osalSysUnlock();
}

/**
 * @brief   Disables a PWM channel and its notification.
 * @pre     The PWM unit must have been activated using @p pwmStart().
 * @post    The channel is disabled and its output line returned to the
 *          idle state.
 * @note    Depending on the hardware implementation this function has
 *          effect starting on the next cycle (recommended implementation)
 *          or immediately (fallback implementation).
 *
 * @param[in] pwmp      pointer to a @p PWMDriver object
 * @param[in] channel   PWM channel identifier (0...channels-1)
 *
 * @api
 */
void pwmDisableChannel(PWMDriver *pwmp, pwmchannel_t channel) {

  osalDbgCheck((pwmp != NULL) && (channel < pwmp->channels));

  osalSysLock();

  osalDbgAssert(pwmp->state == PWM_READY, "not ready");

  pwmDisableChannelI(pwmp, channel);

  osalSysUnlock();
}

/**
 * @brief   Enables the periodic activation edge notification.
 * @pre     The PWM unit must have been activated using @p pwmStart().
 * @note    If the notification is already enabled then the call has no effect.
 *
 * @param[in] pwmp      pointer to a @p PWMDriver object
 *
 * @api
 */
void pwmEnablePeriodicNotification(PWMDriver *pwmp) {

  osalDbgCheck(pwmp != NULL);

  osalSysLock();

  osalDbgAssert(pwmp->state == PWM_READY, "not ready");
  osalDbgAssert(pwmp->config->callback != NULL, "undefined periodic callback");

  pwmEnablePeriodicNotificationI(pwmp);

  osalSysUnlock();
}

/**
 * @brief   Disables the periodic activation edge notification.
 * @pre     The PWM unit must have been activated using @p pwmStart().
 * @note    If the notification is already disabled then the call has no effect.
 *
 * @param[in] pwmp      pointer to a @p PWMDriver object
 *
 * @api
 */
void pwmDisablePeriodicNotification(PWMDriver *pwmp) {

  osalDbgCheck(pwmp != NULL);

  osalSysLock();

  osalDbgAssert(pwmp->state == PWM_READY, "not ready");
  osalDbgAssert(pwmp->config->callback != NULL, "undefined periodic callback");

  pwmDisablePeriodicNotificationI(pwmp);

  osalSysUnlock();
}

/**
 * @brief   Enables a channel de-activation edge notification.
 * @pre     The PWM unit must have been activated using @p pwmStart().
 * @pre     The channel must have been activated using @p pwmEnableChannel().
 * @note    If the notification is already enabled then the call has no effect.
 *
 * @param[in] pwmp      pointer to a @p PWMDriver object
 * @param[in] channel   PWM channel identifier (0...channels-1)
 *
 * @api
 */
void pwmEnableChannelNotification(PWMDriver *pwmp, pwmchannel_t channel) {

  osalDbgCheck((pwmp != NULL) && (channel < pwmp->channels));

  osalSysLock();

  osalDbgAssert(pwmp->state == PWM_READY, "not ready");
  osalDbgAssert((pwmp->enabled & ((pwmchnmsk_t)1U << (pwmchnmsk_t)channel)) != 0U,
                "channel not enabled");
  osalDbgAssert(pwmp->config->channels[channel].callback != NULL,
                "undefined channel callback");

  pwmEnableChannelNotificationI(pwmp, channel);

  osalSysUnlock();
}

/**
 * @brief   Disables a channel de-activation edge notification.
 * @pre     The PWM unit must have been activated using @p pwmStart().
 * @pre     The channel must have been activated using @p pwmEnableChannel().
 * @note    If the notification is already disabled then the call has no effect.
 *
 * @param[in] pwmp      pointer to a @p PWMDriver object
 * @param[in] channel   PWM channel identifier (0...channels-1)
 *
 * @api
 */
void pwmDisableChannelNotification(PWMDriver *pwmp, pwmchannel_t channel) {

  osalDbgCheck((pwmp != NULL) && (channel < pwmp->channels));

  osalSysLock();

  osalDbgAssert(pwmp->state == PWM_READY, "not ready");
  osalDbgAssert((pwmp->enabled & ((pwmchnmsk_t)1U << (pwmchnmsk_t)channel)) != 0U,
                "channel not enabled");
  osalDbgAssert(pwmp->config->channels[channel].callback != NULL,
                "undefined channel callback");

  pwmDisableChannelNotificationI(pwmp, channel);

  osalSysUnlock();
}

#endif /* HAL_USE_PWM == TRUE */

/** @} */
