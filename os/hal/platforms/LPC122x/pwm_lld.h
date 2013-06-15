/*
    ChibiOS/RT - Copyright (C) 2006-2013 Giovanni Di Sirio
    LPC122x PWM driver - Copyright (C) 2013 Marcin Jokel

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
 * @file    LPC122x/pwm_lld.h
 * @brief   LPC122x PWM subsystem low level driver header.
 *
 * @addtogroup PWM
 * @{
 */

#ifndef _PWM_LLD_H_
#define _PWM_LLD_H_

#if HAL_USE_PWM || defined(__DOXYGEN__)

/*===========================================================================*/
/* Unsupported modes and specific modes                                      */
/*===========================================================================*/
#undef PWM_OUTPUT_ACTIVE_HIGH

/*===========================================================================*/
/* Driver constants.                                                         */
/*===========================================================================*/

#define IR_MR0INT   (1 << 0)
#define IR_MR1INT   (1 << 1)
#define IR_MR2INT   (1 << 2)
#define IR_MR3INT   (1 << 3)
#define IR_CR0INT   (1 << 4)
#define IR_CR1INT   (1 << 5)
#define IR_CR2INT   (1 << 6)
#define IR_CR3INT   (1 << 7)

#define MCR_MR3I    (1 << 9)
#define MCR_MR3R    (1 << 10)
#define MCR_MR3S    (1 << 11)

/**
 * @brief   Number of PWM channels per PWM driver.
 */
#define PWM_CHANNELS                      2

#define PWM_CT16B0_CH0_IS_PIO0_11         0
#define PWM_CT16B0_CH0_IS_PIO0_28         1
#define PWM_CT16B0_CH0_IS_PIO2_0          2

#define PWM_CT16B0_CH1_IS_PIO0_12         0
#define PWM_CT16B0_CH1_IS_PIO0_29         1
#define PWM_CT16B0_CH1_IS_PIO2_1          2

#define PWM_CT16B1_CH0_IS_PIO0_15         0
#define PWM_CT16B1_CH0_IS_PIO1_5          1
#define PWM_CT16B1_CH0_IS_PIO2_2          2

#define PWM_CT16B1_CH1_IS_PIO0_16         0
#define PWM_CT16B1_CH1_IS_PIO1_6          1
#define PWM_CT16B1_CH1_IS_PIO2_3          2


#define PWM_CT32B0_CH0_IS_PIO0_1          0
#define PWM_CT32B0_CH0_IS_PIO0_18         1
#define PWM_CT32B0_CH0_IS_PIO2_4          2

#define PWM_CT32B0_CH1_IS_PIO0_2          0
#define PWM_CT32B0_CH1_IS_PIO0_19         1
#define PWM_CT32B0_CH1_IS_PIO2_5          2


#define PWM_CT32B1_CH0_IS_PIO0_6          0
#define PWM_CT32B1_CH0_IS_PIO0_23         1
#define PWM_CT32B1_CH0_IS_PIO2_8          2

#define PWM_CT32B1_CH1_IS_PIO0_7          0
#define PWM_CT32B1_CH1_IS_PIO0_24         1
#define PWM_CT32B1_CH1_IS_PIO2_9          2

/*===========================================================================*/
/* Driver pre-compile time settings.                                         */
/*===========================================================================*/

/**
 * @name    Configuration options
 * @{
 */

/**
 * @brief   PWMD1 driver enable switch.
 * @details If set to @p TRUE the support for PWMD1 is included.
 * @note    The default is @p TRUE.
 */
#if !defined(LPC122x_PWM_USE_CT16B0) || defined(__DOXYGEN__)
#define LPC122x_PWM_USE_CT16B0                  FALSE
#endif

/**
 * @brief   PWMD2 driver enable switch.
 * @details If set to @p TRUE the support for PWMD2 is included.
 * @note    The default is @p TRUE.
 */
#if !defined(LPC122x_PWM_USE_CT16B1) || defined(__DOXYGEN__)
#define LPC122x_PWM_USE_CT16B1                  FALSE
#endif

/**
 * @brief   PWMD3 driver enable switch.
 * @details If set to @p TRUE the support for PWMD3 is included.
 * @note    The default is @p TRUE.
 */
#if !defined(LPC122x_PWM_USE_CT32B0) || defined(__DOXYGEN__)
#define LPC122x_PWM_USE_CT32B0                  FALSE
#endif

/**
 * @brief   PWMD4 driver enable switch.
 * @details If set to @p TRUE the support for PWMD4 is included.
 * @note    The default is @p TRUE.
 */
#if !defined(LPC122x_PWM_USE_CT32B1) || defined(__DOXYGEN__)
#define LPC122x_PWM_USE_CT32B1                  FALSE
#endif

/**
 * @brief   PWMD1 Channel 0 driver enable switch.
 * @details If set to @p TRUE PWMD1 Channel 0 is enabled.
 * @note    The default is @p FALSE.
 */
#if !defined(LPC122x_PWM_USE_CT16B0_CH0) || defined(__DOXYGEN__)
#define LPC122x_PWM_USE_CT16B0_CH0                   FALSE
#endif

/**
 * @brief   PWMD1 Channel 1 driver enable switch.
 * @details If set to @p TRUE PWMD1 Channel 1 is enabled.
 * @note    The default is @p FALSE.
 */
#if !defined(LPC122x_PWM_USE_CT16B0_CH1) || defined(__DOXYGEN__)
#define LPC122x_PWM_USE_CT16B0_CH1                  FALSE
#endif

/**
 * @brief   PWMD2 Channel 0 driver enable switch.
 * @details If set to @p TRUE PWMD2 Channel 0 is enabled.
 * @note    The default is @p FALSE.
 */
#if !defined(LPC122x_PWM_USE_CT16B1_CH0) || defined(__DOXYGEN__)
#define LPC122x_PWM_USE_CT16B1_CH0                   FALSE
#endif

/**
 * @brief   PWMD2 Channel 1 driver enable switch.
 * @details If set to @p TRUE PWMD2 Channel 1 is enabled.
 * @note    The default is @p FALSE.
 */
#if !defined(LPC122x_PWM_USE_CT16B1_CH1) || defined(__DOXYGEN__)
#define LPC122x_PWM_USE_CT16B1_CH1                  FALSE
#endif

/**
 * @brief   PWMD3 Channel 0 driver enable switch.
 * @details If set to @p TRUE PWMD3 Channel 0 is enabled.
 * @note    The default is @p FALSE.
 */
#if !defined(LPC122x_PWM_USE_CT32B0_CH0) || defined(__DOXYGEN__)
#define LPC122x_PWM_USE_CT32B0_CH0                   FALSE
#endif

/**
 * @brief   PWMD3 Channel 1 driver enable switch.
 * @details If set to @p TRUE PWMD3 Channel 1 is enabled.
 * @note    The default is @p FALSE.
 */
#if !defined(LPC122x_PWM_USE_CT32B0_CH1) || defined(__DOXYGEN__)
#define LPC122x_PWM_USE_CT32B0_CH1                  FALSE
#endif

/**
 * @brief   PWMD4 Channel 0 driver enable switch.
 * @details If set to @p TRUE PWMD4 Channel 0 is enabled.
 * @note    The default is @p FALSE.
 */
#if !defined(LPC122x_PWM_USE_CT32B1_CH0) || defined(__DOXYGEN__)
#define LPC122x_PWM_USE_CT32B1_CH0                   FALSE
#endif

/**
 * @brief   PWMD4 Channel 1 driver enable switch.
 * @details If set to @p TRUE PWMD4 Channel 1 is enabled.
 * @note    The default is @p FALSE.
 */
#if !defined(LPC122x_PWM_USE_CT32B1_CH1) || defined(__DOXYGEN__)
#define LPC122x_PWM_USE_CT32B1_CH1                  FALSE
#endif
/**
 * @brief   PWMD1 interrupt priority level setting.
 */
#if !defined(LPC122x_PWM_CT16B0_IRQ_PRIORITY) || defined(__DOXYGEN__)
#define LPC122x_PWM_CT16B0_IRQ_PRIORITY         3
#endif

/**
 * @brief   PWMD2 interrupt priority level setting.
 */
#if !defined(LPC122x_PWM_CT16B1_IRQ_PRIORITY) || defined(__DOXYGEN__)
#define LPC122x_PWM_CT16B1_IRQ_PRIORITY         3
#endif

/**
 * @brief   PWMD3 interrupt priority level setting.
 */
#if !defined(LPC122x_PWM_CT32B0_IRQ_PRIORITY) || defined(__DOXYGEN__)
#define LPC122x_PWM_CT32B0_IRQ_PRIORITY         3
#endif

/**
 * @brief   PWMD4 interrupt priority level setting.
 */
#if !defined(LPC122x_PWM_CT32B1_IRQ_PRIORITY) || defined(__DOXYGEN__)
#define LPC122x_PWM_CT32B1_IRQ_PRIORITY         3
#endif

/**
 * @brief   PWM_CT16B0_CH0 signal selector.
 */
#if !defined(LPC122x_PWM_CT16B0_CH0_SELECTOR) || defined(__DOXYGEN__)
#define LPC122x_PWM_CT16B0_CH0_SELECTOR           PWM_CT16B0_CH0_IS_PIO0_11
#endif

/**
 * @brief   PWM_CT16B0_CH1 signal selector.
 */
#if !defined(LPC122x_PWM_CT16B0_CH1_SELECTOR) || defined(__DOXYGEN__)
#define LPC122x_PWM_CT16B0_CH1_SELECTOR           PWM_CT16B0_CH1_IS_PIO0_12
#endif

/**
 * @brief   PWM_CT16B1_CH0 signal selector.
 */
#if !defined(LPC122x_PWM_CT16B1_CH0_SELECTOR) || defined(__DOXYGEN__)
#define LPC122x_PWM_CT16B1_CH0_SELECTOR           PWM_CT16B1_CH0_IS_PIO0_15
#endif

/**
 * @brief   PWM_CT16B1_CH1 signal selector.
 */
#if !defined(LPC122x_PWM_CT16B1_CH1_SELECTOR) || defined(__DOXYGEN__)
#define LPC122x_PWM_CT16B1_CH1_SELECTOR           PWM_CT16B1_CH1_IS_PIO0_16
#endif

/**
 * @brief   PWM_CT32B0_CH0 signal selector.
 */
#if !defined(LPC122x_PWM_CT32B0_CH0_SELECTOR) || defined(__DOXYGEN__)
#define LPC122x_PWM_CT32B0_CH0_SELECTOR           PWM_CT32B0_CH0_IS_PIO0_1
#endif

/**
 * @brief   PWM_CT32B0_CH1 signal selector.
 */
#if !defined(LPC122x_PWM_CT32B0_CH1_SELECTOR) || defined(__DOXYGEN__)
#define LPC122x_PWM_CT32B0_CH1_SELECTOR           PWM_CT32B0_CH1_IS_PIO0_2
#endif

/**
 * @brief   PWM_CT32B1_CH0 signal selector.
 */
#if !defined(LPC122x_PWM_CT32B1_CH0_SELECTOR) || defined(__DOXYGEN__)
#define LPC122x_PWM_CT32B1_CH0_SELECTOR           PWM_CT32B1_CH0_IS_PIO0_6
#endif

/**
 * @brief   PWM_CT32B1_CH1 signal selector.
 */
#if !defined(LPC122x_PWM_CT32B1_CH1_SELECTOR) || defined(__DOXYGEN__)
#define LPC122x_PWM_CT32B1_CH1_SELECTOR           PWM_CT32B1_CH1_IS_PIO0_7
#endif

/*===========================================================================*/
/* Configuration checks.                                                     */
/*===========================================================================*/



/*===========================================================================*/
/* Driver data structures and types.                                         */
/*===========================================================================*/

/**
 * @brief PWM mode type.
 */
typedef uint32_t pwmmode_t;

/**
 * @brief   PWM channel type.
 */
typedef uint8_t pwmchannel_t;

/**
 * @brief   PWM counter type.
 */
typedef uint16_t pwmcnt_t;

/**
 * @brief   PWM driver channel configuration structure.
 */
typedef struct {
  /**
   * @brief Channel active logic level.
   */
  pwmmode_t                 mode;
  /**
   * @brief Channel callback pointer.
   * @note  This callback is invoked on the channel compare event. If set to
   *        @p NULL then the callback is disabled.
   */
  pwmcallback_t             callback;
  /* End of the mandatory fields.*/
} PWMChannelConfig;

/**
 * @brief   PWM driver configuration structure.
 */
typedef struct {
  /**
   * @brief   Timer clock in Hz.
   * @note    The low level can use assertions in order to catch invalid
   *          frequency specifications.
   */
  uint32_t                  frequency;
  /**
   * @brief   PWM period in ticks.
   * @note    The low level can use assertions in order to catch invalid
   *          period specifications.
   */
  pwmcnt_t                  period;
  /**
   * @brief Periodic callback pointer.
   * @note  This callback is invoked on PWM counter reset. If set to
   *        @p NULL then the callback is disabled.
   */
  pwmcallback_t             callback;
  /**
   * @brief Channels configurations.
   */
  PWMChannelConfig          channels[PWM_CHANNELS];
  /* End of the mandatory fields.*/

} PWMConfig;

/**
 * @brief   Structure representing a PWM driver.
 */
struct PWMDriver {
  /**
   * @brief Driver state.
   */
  pwmstate_t                state;
  /**
   * @brief Current driver configuration data.
   */
  const PWMConfig           *config;
  /**
   * @brief   Current PWM period in ticks.
   */
  pwmcnt_t                  period;
#if defined(PWM_DRIVER_EXT_FIELDS)
  PWM_DRIVER_EXT_FIELDS
#endif
  /* End of the mandatory fields.*/
  /**
   * @brief Timer base clock.
   */
  uint32_t                  clock;
  /**
   * @brief Pointer to the TIMx registers block.
   */
  LPC_CTxxBx_Type           *tim;
};

/*===========================================================================*/
/* Driver macros.                                                            */
/*===========================================================================*/

/**
 * @brief   Changes the period the PWM peripheral.
 * @details This function changes the period of a PWM unit that has already
 *          been activated using @p pwmStart().
 * @pre     The PWM unit must have been activated using @p pwmStart().
 * @post    The PWM unit period is changed to the new value.
 * @note    The function has effect at the next cycle start.
 * @note    If a period is specified that is shorter than the pulse width
 *          programmed in one of the channels then the behavior is not
 *          guaranteed.
 *
 * @param[in] pwmp      pointer to a @p PWMDriver object
 * @param[in] period    new cycle time in ticks
 *
 * @notapi
 */
#define pwm_lld_change_period(pwmp, period)                                 \
  ((pwmp)->tim->MR3 = (period))

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#if LPC122x_PWM_USE_CT16B0 && !defined(__DOXYGEN__)
extern PWMDriver PWMD1;
#endif

#if LPC122x_PWM_USE_CT16B1 && !defined(__DOXYGEN__)
extern PWMDriver PWMD2;
#endif

#if LPC122x_PWM_USE_CT32B0 && !defined(__DOXYGEN__)
extern PWMDriver PWMD3;
#endif

#if LPC122x_PWM_USE_CT32B1 && !defined(__DOXYGEN__)
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

#endif /* HAL_USE_PWM */

#endif /* _PWM_LLD_H_ */

/** @} */
