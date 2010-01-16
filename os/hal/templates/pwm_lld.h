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
 * @file templates/pwm_lld.h
 * @brief PWM Driver subsystem low level driver header template.
 * @addtogroup PWM_LLD
 * @{
 */

#ifndef _PWM_LLD_H_
#define _PWM_LLD_H_

/*===========================================================================*/
/* Driver constants.                                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Driver pre-compile time settings.                                         */
/*===========================================================================*/

/**
 * @brief Number of PWM channels per PWM driver.
 */
#if !defined(PWM_CHANNELS) || defined(__DOXYGEN__)
#define PWM_CHANNELS        1
#endif

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

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
} PWMDriver;

/*===========================================================================*/
/* Driver macros.                                                            */
/*===========================================================================*/

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

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

#endif /* _PWM_LLD_H_ */

/** @} */
