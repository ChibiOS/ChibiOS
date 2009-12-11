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
 * @file STM32/pwm_lld.h
 * @brief STM32 PWM subsystem low level driver header.
 * @addtogroup STM32_PWM
 * @{
 */

#ifndef _PWM_LLD_H_
#define _PWM_LLD_H_

#if CH_HAL_USE_PWM || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver constants.                                                         */
/*===========================================================================*/

/**
 * @brief Number of PWM channels per PWM driver.
 */
#define PWM_CHANNELS                4

/*===========================================================================*/
/* Driver pre-compile time settings.                                         */
/*===========================================================================*/

/**
 * @brief PWM1 driver enable switch.
 * @details If set to @p TRUE the support for PWM1 is included.
 * @note The default is @p TRUE.
 */
#if !defined(USE_STM32_PWM1) || defined(__DOXYGEN__)
#define USE_STM32_PWM1              TRUE
#endif

/**
 * @brief PWM1 interrupt priority level setting.
 * @note @p BASEPRI_KERNEL >= @p STM32_PWM1_IRQ_PRIORITY > @p PRIORITY_PENDSV.
 */
#if !defined(STM32_PWM1_IRQ_PRIORITY) || defined(__DOXYGEN__)
#define STM32_PWM1_IRQ_PRIORITY     0x80
#endif

/*===========================================================================*/
/* Driver data structures and types.                                         */
/*===========================================================================*/

/**
 * @brief PWM channel type.
 */
typedef uint8_t pwmchannel_t;

/**
 * @brief PWM counter type.
 */
typedef uint16_t pwmcnt_t;

/**
 * @brief Driver configuration structure.
 * @note It could be empty on some architectures.
 */
typedef struct {

} PWMConfig;

/**
 * @brief Structure representing an PWM driver.
 */
typedef struct {
  /**
   * @brief Driver state.
   */
  pwmstate_t                pd_state;
  /**
   * @brief Current configuration data.
   */
  const PWMConfig           *pd_config;
  /* End of the mandatory fields.*/
  /**
   * @brief Bit mask of the enabled channels.
   */
  uint32_t                  pd_enabled_channels;
  /**
   * @brief Callback pointers.
   */
  pwmcallback_t             pd_callbacks[PWM_CHANNELS];
} PWMDriver;

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

/** @cond never*/
#if defined(USE_STM32_PWM1)
extern PWMDriver PWMD1;
#endif

#ifdef __cplusplus
extern "C" {
#endif
  void pwm_lld_init(void);
  void pwm_lld_start(PWMDriver *pwmp);
  void pwm_lld_stop(PWMDriver *pwmp);
  bool_t pwm_lld_is_enabled(PWMDriver *pwmp, pwmchannel_t channel);
  void pwm_lld_set_callback(PWMDriver *pwmp, pwmchannel_t channel,
                            pwmedge_t edge, pwmcallback_t callback);
  void pwm_lld_enable_channel(PWMDriver *pwmp,
                              pwmchannel_t channel,
                              pwmcnt_t width);
  void pwm_lld_disable_channel(PWMDriver *pwmp, pwmchannel_t channel);
#ifdef __cplusplus
}
#endif
/** @endcond*/

#endif /* CH_HAL_USE_PWM */

#endif /* _PWM_LLD_H_ */

/** @} */
