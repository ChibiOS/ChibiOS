/*
    ChibiOS/RT - Copyright (C) 2006,2007,2008,2009,2010 Giovanni Di Sirio.

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
 * @file    STM32/pwm_lld.h
 * @brief   STM32 PWM subsystem low level driver header.
 *
 * @addtogroup PWM
 * @{
 */

#ifndef _PWM_LLD_H_
#define _PWM_LLD_H_

#if CH_HAL_USE_PWM || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver constants.                                                         */
/*===========================================================================*/

/**
 * @brief   Number of PWM channels per PWM driver.
 */
#define PWM_CHANNELS                        4

/*===========================================================================*/
/* Driver pre-compile time settings.                                         */
/*===========================================================================*/

/**
 * @brief   PWM1 driver enable switch.
 * @details If set to @p TRUE the support for PWM1 is included.
 * @note    The default is @p TRUE.
 */
#if !defined(STM32_PWM_USE_TIM1) || defined(__DOXYGEN__)
#define STM32_PWM_USE_TIM1                  TRUE
#endif

/**
 * @brief   PWM2 driver enable switch.
 * @details If set to @p TRUE the support for PWM2 is included.
 * @note    The default is @p TRUE.
 */
#if !defined(STM32_PWM_USE_TIM2) || defined(__DOXYGEN__)
#define STM32_PWM_USE_TIM2                  TRUE
#endif

/**
 * @brief   PWM3 driver enable switch.
 * @details If set to @p TRUE the support for PWM3 is included.
 * @note    The default is @p TRUE.
 */
#if !defined(STM32_PWM_USE_TIM3) || defined(__DOXYGEN__)
#define STM32_PWM_USE_TIM3                  TRUE
#endif

/**
 * @brief   PWM4 driver enable switch.
 * @details If set to @p TRUE the support for PWM4 is included.
 * @note    The default is @p TRUE.
 */
#if !defined(STM32_PWM_USE_TIM4) || defined(__DOXYGEN__)
#define STM32_PWM_USE_TIM4                  TRUE
#endif

/**
 * @brief   PWM1 interrupt priority level setting.
 */
#if !defined(STM32_PWM_PWM1_IRQ_PRIORITY) || defined(__DOXYGEN__)
#define STM32_PWM_PWM1_IRQ_PRIORITY         7
#endif

/**
 * @brief   PWM2 interrupt priority level setting.
 */
#if !defined(STM32_PWM_PWM2_IRQ_PRIORITY) || defined(__DOXYGEN__)
#define STM32_PWM_PWM2_IRQ_PRIORITY         7
#endif

/**
 * @brief   PWM3 interrupt priority level setting.
 */
#if !defined(STM32_PWM_PWM3_IRQ_PRIORITY) || defined(__DOXYGEN__)
#define STM32_PWM_PWM3_IRQ_PRIORITY         7
#endif

/**
 * @brief   PWM4 interrupt priority level setting.
 */
#if !defined(STM32_PWM_PWM4_IRQ_PRIORITY) || defined(__DOXYGEN__)
#define STM32_PWM_PWM4_IRQ_PRIORITY         7
#endif

/*===========================================================================*/
/* Configuration checks.                                                     */
/*===========================================================================*/

#if STM32_PWM_USE_TIM4 && defined(STM32F10X_LD)
#error "TIM4 not present in low density STM32 devices"
#endif

#if !STM32_SERIAL_USE_USART1 && !STM32_SERIAL_USE_USART2 &&                 \
    !STM32_SERIAL_USE_USART3 && !STM32_SERIAL_USE_UART4  &&                 \
    !STM32_SERIAL_USE_UART5
#error "PWM driver activated but no TIM peripheral assigned"
#endif

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
 * @brief   PWM driver configuration structure.
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
   * @note  The value of this field should normally be equal to zero.
   */
  uint16_t                  pc_cr2;
} PWMConfig;

/**
 * @brief   Structure representing a PWM driver.
 */
struct PWMDriver {
  /**
   * @brief Driver state.
   */
  pwmstate_t                pd_state;
  /**
   * @brief Current driver configuration data.
   */
  const PWMConfig           *pd_config;
#if defined(PWM_DRIVER_EXT_FIELDS)
  PWM_DRIVER_EXT_FIELDS
#endif
  /* End of the mandatory fields.*/
  /**
   * @brief Bit mask of the enabled channels.
   */
  uint32_t                  pd_enabled_channels;
  /**
   * @brief Pointer to the TIMx registers block.
   */
  TIM_TypeDef               *pd_tim;
};

/*===========================================================================*/
/* Driver macros.                                                            */
/*===========================================================================*/

/**
 * @brief   PWM clock prescaler initialization utility.
 * @note    The real clock value is rounded to the lower valid value, please
 *          make sure that the source clock frequency is a multiple of the
 *          requested PWM clock frequency.
 * @note    The calculated value must fit into an unsigned 16 bits integer.
 *
 * @param[in] clksrc    clock source frequency, depending on the target timer
 *                      cell it can be one of:
 *                      - STM32_TIMCLK1
 *                      - STM32_TIMCLK2
 *                      .
 *                      Please refer to the STM32 HAL driver documentation
 *                      and/or the STM32 Reference Manual for the right clock
 *                      source.
 * @param[in] nsec      PWM clock cycle time in nanoseconds
 * @return              The value to be stored in the @p pc_psc field of the
 *                      @p PWMConfig structure.
 */
#define PWM_COMPUTE_PSC(clksrc, nsec)                                       \
  ((uint16_t)(((clksrc) / (1000000000 / (nsec))) - 1))

/**
 * @brief   PWM cycle period initialization utility.
 * @note    The calculated value must fit into an unsigned 16 bits integer.
 *
 * @param[in] clkperiod PWM clock period in nanoseconds
 * @param[in] pwmperiod PWM cycle period in nanoseconds
 */
#define PWM_COMPUTE_ARR(clkperiod, pwmperiod)                               \
  ((uint16_t)(((clkperiod) / (1000000000 / (pwmperiod))) - 1))

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
 *
 * @api
 */
#define PWM_DEGREES_TO_WIDTH(pwmp, degrees)                                 \
  ((uint16_t)(((((uint32_t)(pwmp)->pd_config->pc_arr + 1UL) *               \
                 (uint32_t)(degrees)) / 36000UL) - 1UL))

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
 *
 * @api
 */
#define PWM_PERCENTAGE_TO_WIDTH(pwmp, percentage)                           \
  ((uint16_t)(((((uint32_t)(pwmp)->pd_config->pc_arr + 1UL) *               \
                 (uint32_t)(percentage)) / 10000UL) - 1UL))

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#if defined(STM32_PWM_USE_TIM1) && !defined(__DOXYGEN__)
extern PWMDriver PWMD1;
#endif

#if defined(STM32_PWM_USE_TIM2) && !defined(__DOXYGEN__)
extern PWMDriver PWMD2;
#endif

#if defined(STM32_PWM_USE_TIM3) && !defined(__DOXYGEN__)
extern PWMDriver PWMD3;
#endif

#if defined(STM32_PWM_USE_TIM4) && !defined(__DOXYGEN__)
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
