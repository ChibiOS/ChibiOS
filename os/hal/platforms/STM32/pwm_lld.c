/*
    ChibiOS/RT - Copyright (C) 2006-2007 Giovanni Di Sirio.

    This file is part of ChibiOS/RT.

    ChibiOS/RT is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 3 of the License, or
    (at your option) any later version.

    ChibiOS/RT is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

/**
 * @file STM32/pwm_lld.c
 * @brief STM32 PWM subsystem low level driver header.
 * @addtogroup STM32_PWM
 * @{
 */

#include "ch.h"
#include "hal.h"

#if CH_HAL_USE_PWM || defined(__DOXYGEN__)

/** @brief PWM1 driver identifier.*/
#if defined(USE_STM32_PWM1) || defined(__DOXYGEN__)
PWMDriver PWMD1;
#endif

/*===========================================================================*/
/* Low Level Driver exported variables.                                      */
/*===========================================================================*/

/*===========================================================================*/
/* Low Level Driver local variables.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Low Level Driver local functions.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Low Level Driver interrupt handlers.                                      */
/*===========================================================================*/

/*===========================================================================*/
/* Low Level Driver exported functions.                                      */
/*===========================================================================*/

/**
 * @brief Low level PWM driver initialization.
 */
void pwm_lld_init(void) {

#if USE_STM32_PWM1
  /* TIM1 reset, ensures reset state in order to avoid trouble with JTAGs.*/
  RCC->APB2RSTR = RCC_APB2RSTR_TIM1RST;
  RCC->APB2RSTR = 0;

  /* Driver initialization.*/
  pwmObjectInit(&PWMD1);
#endif

}

/**
 * @brief Configures and activates the PWM peripheral.
 *
 * @param[in] pwmp pointer to the @p PWMDriver object
 */
void pwm_lld_start(PWMDriver *pwmp) {

  if (pwmp->pd_state == PWM_STOP) {
    /* Clock activation.*/
#if USE_STM32_PWM1
    if (&PWMD1 == pwmp) {
      NVICEnableVector(TIM1_CC_IRQn, STM32_PWM1_IRQ_PRIORITY);
      RCC->APB2ENR |= RCC_APB2ENR_TIM1EN;
    }
#endif
  }
  /* Configuration.*/
}

/**
 * @brief Deactivates the PWM peripheral.
 *
 * @param[in] pwmp pointer to the @p PWMDriver object
 */
void pwm_lld_stop(PWMDriver *pwmp) {

  /* If in ready state then disables the PWM clock.*/
  if (pwmp->pd_state == PWM_READY) {
#if USE_STM32_PWM1
    if (&PWMD1 == pwmp) {
      NVICDisableVector(TIM1_CC_IRQn);
      RCC->APB2ENR &= ~RCC_APB2ENR_TIM1EN;
    }
#endif
  }
}

/**
 * @brief Determines whatever the PWM channel is already enabled.
 *
 * @param[in] pwmp      pointer to the @p PWMDriver object
 * @param[in] channel   PWM channel identifier
 *
 * @return The PWM channel status.
 * @retval FALSE the channel is not enabled.
 * @retval TRUE the channel is enabled.
 */
bool_t pwm_lld_is_enabled(PWMDriver *pwmp, pwmchannel_t channel) {

  return FALSE;
}

/**
 * @brief Enables a callback mode for the specified PWM channel.
 * @details The callback mode must be set before starting a PWM channel.
 *
 * @param[in] pwmp      pointer to the @p PWMDriver object
 * @param[in] channel   PWM channel identifier
 * @param[in] edge      output edge mode
 * @param[in] callback  the callback function
 */
void pwm_lld_set_callback(PWMDriver *pwmp, pwmchannel_t channel,
                          pwmedge_t edge, pwmcallback_t callback) {

  if (edge == PWM_NONE) {
    /* Callback disable.*/
    pwmp->pd_callbacks[channel] = NULL;
  }
  else {
    /* Callback enable.*/
    pwmp->pd_callbacks[channel] = callback;
  }
}

/**
 * @brief Enables a PWM channel.
 *
 * @param[in] pwmp      pointer to the @p PWMDriver object
 * @param[in] channel   PWM channel identifier
 * @param[in] width     PWM pulse width as clock pulses number
 */
void pwm_lld_enable_channel(PWMDriver *pwmp,
                            pwmchannel_t channel,
                            pwmcnt_t width) {

}

/**
 * @brief Disables a PWM channel.
 * @details The channel is disabled and its output line returned to the
 *          idle state.
 *
 * @param[in] pwmp      pointer to the @p PWMDriver object
 * @param[in] channel   PWM channel identifier
 */
void pwm_lld_disable_channel(PWMDriver *pwmp, pwmchannel_t channel) {

}

#endif /* CH_HAL_USE_PWM */

/** @} */
