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
 * @brief PWM2 driver enable switch.
 * @details If set to @p TRUE the support for PWM2 is included.
 * @note The default is @p TRUE.
 */
#if !defined(USE_STM32_PWM2) || defined(__DOXYGEN__)
#define USE_STM32_PWM2              TRUE
#endif

/**
 * @brief PWM3 driver enable switch.
 * @details If set to @p TRUE the support for PWM3 is included.
 * @note The default is @p TRUE.
 */
#if !defined(USE_STM32_PWM3) || defined(__DOXYGEN__)
#define USE_STM32_PWM3              TRUE
#endif

/**
 * @brief PWM4 driver enable switch.
 * @details If set to @p TRUE the support for PWM4 is included.
 * @note The default is @p TRUE.
 */
#if !defined(USE_STM32_PWM4) || defined(__DOXYGEN__)
#define USE_STM32_PWM4              TRUE
#endif

/**
 * @brief PWM1 interrupt priority level setting.
 */
#if !defined(STM32_PWM1_IRQ_PRIORITY) || defined(__DOXYGEN__)
#define STM32_PWM1_IRQ_PRIORITY     7
#endif

/**
 * @brief PWM2 interrupt priority level setting.
 */
#if !defined(STM32_PWM2_IRQ_PRIORITY) || defined(__DOXYGEN__)
#define STM32_PWM2_IRQ_PRIORITY     7
#endif

/**
 * @brief PWM3 interrupt priority level setting.
 */
#if !defined(STM32_PWM3_IRQ_PRIORITY) || defined(__DOXYGEN__)
#define STM32_PWM3_IRQ_PRIORITY     7
#endif

/**
 * @brief PWM4 interrupt priority level setting.
 */
#if !defined(STM32_PWM4_IRQ_PRIORITY) || defined(__DOXYGEN__)
#define STM32_PWM4_IRQ_PRIORITY     7
#endif

/*===========================================================================*/
/* Configuration checks.                                                     */
/*===========================================================================*/

#if USE_STM32_PWM4 && defined(STM32F10X_LD)
#error "TIM4 not present in low density STM32 devices"
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
 * @brief PWM driver channel configuration structure.
 * @note It could be empty on some architectures.
 */
typedef struct {
  /**
   * @brief Channel active logic level.
   */
  pwmmode_t                 pcc_mode;
  /**
   * @brief Channel callback pointer.
   * @details This callback is invoked on the channel compare event. If set to
   * @p NULL then the callback is disabled.
   */
  pwmcallback_t             pcc_callback;
  /* End of the mandatory fields.*/
} PWMChannelConfig;

/**
 * @brief PWM driver configuration structure.
 * @note It could be empty on some architectures.
 */
typedef struct {
  /**
   * @brief Periodic callback pointer.
   * @details This callback is invoked on PWM counter reset. If set to
   * @p NULL then the callback is disabled.
   */
  pwmcallback_t             pc_callback;
  /**
   * @brief Channels configurations.
   */
  PWMChannelConfig          pc_channels[PWM_CHANNELS];
  /* End of the mandatory fields.*/
  /**
   * @brief TIM PSC (pre-scaler) register initialization data.
   */
  uint16_t                  pc_psc;
  /**
   * @brief TIM ARR (auto-reload) register initialization data.
   */
  uint16_t                  pc_arr;
  /**
   * @brief TIM CR2 register initialization data.
   * @note The value of this field should normally be equal to zero.
   */
  uint16_t                  pc_cr2;
} PWMConfig;

/**
 * @brief Structure representing a PWM driver.
 */
typedef struct {
  /**
   * @brief Driver state.
   */
  pwmstate_t                pd_state;
  /**
   * @brief Current driver configuration data.
   */
  const PWMConfig           *pd_config;
  /* End of the mandatory fields.*/
  /**
   * @brief Bit mask of the enabled channels.
   */
  uint32_t                  pd_enabled_channels;
  /**
   * @brief Pointer to the TIMx registers block.
   */
  TIM_TypeDef               *pd_tim;
} PWMDriver;

/*===========================================================================*/
/* Driver macros.                                                            */
/*===========================================================================*/

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#if defined(USE_STM32_PWM1) && !defined(__DOXYGEN__)
extern PWMDriver PWMD1;
#endif

#if defined(USE_STM32_PWM2) && !defined(__DOXYGEN__)
extern PWMDriver PWMD2;
#endif

#if defined(USE_STM32_PWM3) && !defined(__DOXYGEN__)
extern PWMDriver PWMD3;
#endif

#if defined(USE_STM32_PWM4) && !defined(__DOXYGEN__)
extern PWMDriver PWMD4;
#endif

#ifdef __cplusplus
extern "C" {
#endif
  void pwm_lld_init(void);
  void pwm_lld_start(PWMDriver *pwmp);
  void pwm_lld_stop(PWMDriver *pwmp);
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
