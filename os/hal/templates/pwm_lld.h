/*
    ChibiOS/RT - Copyright (C) 2006,2007,2008,2009,2010 Giovanni Di Sirio.

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
 * @file    templates/pwm_lld.h
 * @brief   PWM Driver subsystem low level driver header template.
 *
 * @addtogroup PWM_LLD
 * @{
 */

#ifndef _PWM_LLD_H_
#define _PWM_LLD_H_

#if CH_HAL_USE_PWM || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver constants.                                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Driver pre-compile time settings.                                         */
/*===========================================================================*/

/**
 * @brief   Number of PWM channels per PWM driver.
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
 * @brief   PWM channel type.
 */
typedef uint8_t pwmchannel_t;

/**
 * @brief   PWM counter type.
 */
typedef uint16_t pwmcnt_t;

/**
 * @brief   Type of a structure representing an PWM driver.
 */
typedef struct PWMDriver PWMDriver;

/**
 * @brief   PWM notification callback type.
 *
 * @param[in] pwmp      pointer to a @p PWMDriver object
 */
typedef void (*pwmcallback_t)(PWMDriver *pwmp);

/**
 * @brief   PWM driver channel configuration structure.
 * @note    Some architectures may not be able to support the channel mode
 *          or the callback, in this case the fields are ignored.
 */
typedef struct {
  /**
   * @brief Channel active logic level.
   */
  pwmmode_t                 pcc_mode;
  /**
   * @brief Channel callback pointer.
   * @note  This callback is invoked on the channel compare event. If set to
   *        @p NULL then the callback is disabled.
   */
  pwmcallback_t             pcc_callback;
  /* End of the mandatory fields.*/
} PWMChannelConfig;

/**
 * @brief   Driver configuration structure.
 * @note    Implementations may extend this structure to contain more,
 *          architecture dependent, fields.
 */
typedef struct {
  /**
   * @brief Periodic callback pointer.
   * @note  This callback is invoked on PWM counter reset. If set to
   *        @p NULL then the callback is disabled.
   */
  pwmcallback_t             pc_callback;
  /**
   * @brief Channels configurations.
   */
  PWMChannelConfig          pc_channels[PWM_CHANNELS];
  /* End of the mandatory fields.*/
} PWMConfig;

/**
 * @brief   Structure representing an PWM driver.
 * @note    Implementations may extend this structure to contain more,
 *          architecture dependent, fields.
 */
struct PWMDriver {
  /**
   * @brief Driver state.
   */
  pwmstate_t                pd_state;
  /**
   * @brief Current configuration data.
   */
  const PWMConfig           *pd_config;
#if defined(PWM_DRIVER_EXT_FIELDS)
  PWM_DRIVER_EXT_FIELDS
#endif
  /* End of the mandatory fields.*/
};

/*===========================================================================*/
/* Driver macros.                                                            */
/*===========================================================================*/

/**
 * @brief   Converts from fraction to pulse width.
 * @note    Be careful with rounding errors, this is integer math not magic.
 *          You can specify tenths of thousandth but make sure you have the
 *          proper hardware resolution by carefully choosing the clock source
 *          and prescaler settings, see @p PWM_COMPUTE_PSC.
 *
 * @param[in] pwmp      pointer to a @p PWMDriver object
 * @param[in] numerator numerator of the fraction
 * @param[in] denominator percentage as an integer between 0 and numerator
 * @return              The pulse width to be passed to @p pwmEnableChannel().
 */
#define PWM_FRACTION_TO_WIDTH(pwmp, numerator, denominator) 0

/**
 * @brief   Converts from degrees to pulse width.
 * @note    Be careful with rounding errors, this is integer math not magic.
 *          You can specify hundredths of degrees but make sure you have the
 *          proper hardware resolution by carefully choosing the clock source
 *          and prescaler settings, see @p PWM_COMPUTE_PSC.
 *
 * @param[in] pwmp      pointer to a @p PWMDriver object
 * @param[in] degrees   degrees as an integer between 0 and 36000
 * @return              The pulse width to be passed to @p pwmEnableChannel().
 */
#define PWM_DEGREES_TO_WIDTH(pwmp, degrees)                                 \
  PWM_FRACTION_TO_WIDTH(pwmp, 36000, degrees)

/**
 * @brief   Converts from percentage to pulse width.
 * @note    Be careful with rounding errors, this is integer math not magic.
 *          You can specify tenths of thousandth but make sure you have the
 *          proper hardware resolution by carefully choosing the clock source
 *          and prescaler settings, see @p PWM_COMPUTE_PSC.
 *
 * @param[in] pwmp      pointer to a @p PWMDriver object
 * @param[in] percentage percentage as an integer between 0 and 10000
 * @return              The pulse width to be passed to @p pwmEnableChannel().
 */
#define PWM_PERCENTAGE_TO_WIDTH(pwmp, percentage)                           \
  PWM_FRACTION_TO_WIDTH(pwmp, 10000, percentage)

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
  void pwm_lld_enable_channel(PWMDriver *pwmp,
                              pwmchannel_t channel,
                              pwmcnt_t width);
  void pwm_lld_disable_channel(PWMDriver *pwmp, pwmchannel_t channel);
#ifdef __cplusplus
}
#endif

#endif /* CH_HAL_USE_PWM */

#endif /* _PWM_LLD_H_ */

/** @} */
