/*
    ChibiOS - Copyright (C) 2006-2026 Giovanni Di Sirio.

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
 * @file    TIMv1/hal_pwm_lld.h
 * @brief   STM32 PWM subsystem low level driver header.
 *
 * @addtogroup HAL_PWM
 * @{
 */

#ifndef HAL_PWM_LLD_H
#define HAL_PWM_LLD_H

#include "stm32_tim.h"

#if HAL_USE_PWM || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver constants.                                                         */
/*===========================================================================*/

/**
 * @brief   Number of PWM channels per PWM driver.
 */
#define PWM_CHANNELS                            STM32_TIM_MAX_CHANNELS

/**
 * @name    STM32-specific PWM complementary output mode macros
 * @{
 */
/**
 * @brief   Complementary output modes mask.
 * @note    This is an STM32-specific setting.
 */
#define PWM_COMPLEMENTARY_OUTPUT_MASK           0xF0U

/**
 * @brief   Complementary output not driven.
 * @note    This is an STM32-specific setting.
 */
#define PWM_COMPLEMENTARY_OUTPUT_DISABLED       0x00U

/**
 * @brief   Complementary output, active is logic level one.
 * @note    This is an STM32-specific setting.
 * @note    This setting is only available if the timer supports the
 *          BDTR register.
 */
#define PWM_COMPLEMENTARY_OUTPUT_ACTIVE_HIGH    0x10U

/**
 * @brief   Complementary output, active is logic level zero.
 * @note    This is an STM32-specific setting.
 * @note    This setting is only available if the timer supports the
 *          BDTR register.
 */
#define PWM_COMPLEMENTARY_OUTPUT_ACTIVE_LOW     0x20U
/** @} */

/*===========================================================================*/
/* Driver pre-compile time settings.                                         */
/*===========================================================================*/

/**
 * @name    Configuration options
 * @{
 */
#if !defined(STM32_PWM_USE_TIM1) || defined(__DOXYGEN__)
#define STM32_PWM_USE_TIM1                  FALSE
#endif

#if !defined(STM32_PWM_USE_TIM2) || defined(__DOXYGEN__)
#define STM32_PWM_USE_TIM2                  FALSE
#endif

#if !defined(STM32_PWM_USE_TIM3) || defined(__DOXYGEN__)
#define STM32_PWM_USE_TIM3                  FALSE
#endif

#if !defined(STM32_PWM_USE_TIM4) || defined(__DOXYGEN__)
#define STM32_PWM_USE_TIM4                  FALSE
#endif

#if !defined(STM32_PWM_USE_TIM5) || defined(__DOXYGEN__)
#define STM32_PWM_USE_TIM5                  FALSE
#endif

#if !defined(STM32_PWM_USE_TIM8) || defined(__DOXYGEN__)
#define STM32_PWM_USE_TIM8                  FALSE
#endif

#if !defined(STM32_PWM_USE_TIM9) || defined(__DOXYGEN__)
#define STM32_PWM_USE_TIM9                  FALSE
#endif

#if !defined(STM32_PWM_USE_TIM10) || defined(__DOXYGEN__)
#define STM32_PWM_USE_TIM10                 FALSE
#endif

#if !defined(STM32_PWM_USE_TIM11) || defined(__DOXYGEN__)
#define STM32_PWM_USE_TIM11                 FALSE
#endif

#if !defined(STM32_PWM_USE_TIM12) || defined(__DOXYGEN__)
#define STM32_PWM_USE_TIM12                 FALSE
#endif

#if !defined(STM32_PWM_USE_TIM13) || defined(__DOXYGEN__)
#define STM32_PWM_USE_TIM13                 FALSE
#endif

#if !defined(STM32_PWM_USE_TIM14) || defined(__DOXYGEN__)
#define STM32_PWM_USE_TIM14                 FALSE
#endif

#if !defined(STM32_PWM_USE_TIM15) || defined(__DOXYGEN__)
#define STM32_PWM_USE_TIM15                 FALSE
#endif

#if !defined(STM32_PWM_USE_TIM16) || defined(__DOXYGEN__)
#define STM32_PWM_USE_TIM16                 FALSE
#endif

#if !defined(STM32_PWM_USE_TIM17) || defined(__DOXYGEN__)
#define STM32_PWM_USE_TIM17                 FALSE
#endif

#if !defined(STM32_PWM_USE_TIM20) || defined(__DOXYGEN__)
#define STM32_PWM_USE_TIM20                 FALSE
#endif

#if !defined(STM32_PWM_USE_TIM21) || defined(__DOXYGEN__)
#define STM32_PWM_USE_TIM21                 FALSE
#endif

#if !defined(STM32_PWM_USE_TIM22) || defined(__DOXYGEN__)
#define STM32_PWM_USE_TIM22                 FALSE
#endif
/** @} */

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

#if !defined(STM32_HAS_TIM1)
#define STM32_HAS_TIM1                      FALSE
#endif

#if !defined(STM32_HAS_TIM2)
#define STM32_HAS_TIM2                      FALSE
#endif

#if !defined(STM32_HAS_TIM3)
#define STM32_HAS_TIM3                      FALSE
#endif

#if !defined(STM32_HAS_TIM4)
#define STM32_HAS_TIM4                      FALSE
#endif

#if !defined(STM32_HAS_TIM5)
#define STM32_HAS_TIM5                      FALSE
#endif

#if !defined(STM32_HAS_TIM8)
#define STM32_HAS_TIM8                      FALSE
#endif

#if !defined(STM32_HAS_TIM9)
#define STM32_HAS_TIM9                      FALSE
#endif

#if !defined(STM32_HAS_TIM10)
#define STM32_HAS_TIM10                     FALSE
#endif

#if !defined(STM32_HAS_TIM11)
#define STM32_HAS_TIM11                     FALSE
#endif

#if !defined(STM32_HAS_TIM12)
#define STM32_HAS_TIM12                     FALSE
#endif

#if !defined(STM32_HAS_TIM13)
#define STM32_HAS_TIM13                     FALSE
#endif

#if !defined(STM32_HAS_TIM14)
#define STM32_HAS_TIM14                     FALSE
#endif

#if !defined(STM32_HAS_TIM15)
#define STM32_HAS_TIM15                     FALSE
#endif

#if !defined(STM32_HAS_TIM16)
#define STM32_HAS_TIM16                     FALSE
#endif

#if !defined(STM32_HAS_TIM17)
#define STM32_HAS_TIM17                     FALSE
#endif

#if !defined(STM32_HAS_TIM20)
#define STM32_HAS_TIM20                     FALSE
#endif

#if !defined(STM32_HAS_TIM21)
#define STM32_HAS_TIM21                     FALSE
#endif

#if !defined(STM32_HAS_TIM22)
#define STM32_HAS_TIM22                     FALSE
#endif

#if STM32_PWM_USE_TIM1 && !STM32_HAS_TIM1
#error "TIM1 not present in the selected device"
#endif

#if STM32_PWM_USE_TIM2 && !STM32_HAS_TIM2
#error "TIM2 not present in the selected device"
#endif

#if STM32_PWM_USE_TIM3 && !STM32_HAS_TIM3
#error "TIM3 not present in the selected device"
#endif

#if STM32_PWM_USE_TIM4 && !STM32_HAS_TIM4
#error "TIM4 not present in the selected device"
#endif

#if STM32_PWM_USE_TIM5 && !STM32_HAS_TIM5
#error "TIM5 not present in the selected device"
#endif

#if STM32_PWM_USE_TIM8 && !STM32_HAS_TIM8
#error "TIM8 not present in the selected device"
#endif

#if STM32_PWM_USE_TIM9 && !STM32_HAS_TIM9
#error "TIM9 not present in the selected device"
#endif

#if STM32_PWM_USE_TIM10 && !STM32_HAS_TIM10
#error "TIM10 not present in the selected device"
#endif

#if STM32_PWM_USE_TIM11 && !STM32_HAS_TIM11
#error "TIM11 not present in the selected device"
#endif

#if STM32_PWM_USE_TIM12 && !STM32_HAS_TIM12
#error "TIM12 not present in the selected device"
#endif

#if STM32_PWM_USE_TIM13 && !STM32_HAS_TIM13
#error "TIM13 not present in the selected device"
#endif

#if STM32_PWM_USE_TIM14 && !STM32_HAS_TIM14
#error "TIM14 not present in the selected device"
#endif

#if STM32_PWM_USE_TIM15 && !STM32_HAS_TIM15
#error "TIM15 not present in the selected device"
#endif

#if STM32_PWM_USE_TIM16 && !STM32_HAS_TIM16
#error "TIM16 not present in the selected device"
#endif

#if STM32_PWM_USE_TIM17 && !STM32_HAS_TIM17
#error "TIM17 not present in the selected device"
#endif

#if STM32_PWM_USE_TIM20 && !STM32_HAS_TIM20
#error "TIM20 not present in the selected device"
#endif

#if STM32_PWM_USE_TIM21 && !STM32_HAS_TIM21
#error "TIM21 not present in the selected device"
#endif

#if STM32_PWM_USE_TIM22 && !STM32_HAS_TIM22
#error "TIM22 not present in the selected device"
#endif

#if !STM32_PWM_USE_TIM1  && !STM32_PWM_USE_TIM2  &&                         \
    !STM32_PWM_USE_TIM3  && !STM32_PWM_USE_TIM4  &&                         \
    !STM32_PWM_USE_TIM5  && !STM32_PWM_USE_TIM8  &&                         \
    !STM32_PWM_USE_TIM9  && !STM32_PWM_USE_TIM10 &&                         \
    !STM32_PWM_USE_TIM11 && !STM32_PWM_USE_TIM12 &&                         \
    !STM32_PWM_USE_TIM13 && !STM32_PWM_USE_TIM14 &&                         \
    !STM32_PWM_USE_TIM15 && !STM32_PWM_USE_TIM16 &&                         \
    !STM32_PWM_USE_TIM17 && !STM32_PWM_USE_TIM20 &&                         \
    !STM32_PWM_USE_TIM21 && !STM32_PWM_USE_TIM22
#error "PWM driver activated but no TIM peripheral assigned"
#endif

/* Checks on allocation of TIMx units.*/
#if STM32_PWM_USE_TIM1
#if defined(STM32_TIM1_IS_USED)
#error "PWMD1 requires TIM1 but the timer is already used"
#else
#define STM32_TIM1_IS_USED
#endif
#endif

#if STM32_PWM_USE_TIM2
#if defined(STM32_TIM2_IS_USED)
#error "PWMD2 requires TIM2 but the timer is already used"
#else
#define STM32_TIM2_IS_USED
#endif
#endif

#if STM32_PWM_USE_TIM3
#if defined(STM32_TIM3_IS_USED)
#error "PWMD3 requires TIM3 but the timer is already used"
#else
#define STM32_TIM3_IS_USED
#endif
#endif

#if STM32_PWM_USE_TIM4
#if defined(STM32_TIM4_IS_USED)
#error "PWMD4 requires TIM4 but the timer is already used"
#else
#define STM32_TIM4_IS_USED
#endif
#endif

#if STM32_PWM_USE_TIM5
#if defined(STM32_TIM5_IS_USED)
#error "PWMD5 requires TIM5 but the timer is already used"
#else
#define STM32_TIM5_IS_USED
#endif
#endif

#if STM32_PWM_USE_TIM8
#if defined(STM32_TIM8_IS_USED)
#error "PWMD8 requires TIM8 but the timer is already used"
#else
#define STM32_TIM8_IS_USED
#endif
#endif

#if STM32_PWM_USE_TIM9
#if defined(STM32_TIM9_IS_USED)
#error "PWMD9 requires TIM9 but the timer is already used"
#else
#define STM32_TIM9_IS_USED
#endif
#endif

#if STM32_PWM_USE_TIM10
#if defined(STM32_TIM10_IS_USED)
#error "PWMD10 requires TIM10 but the timer is already used"
#else
#define STM32_TIM10_IS_USED
#endif
#endif

#if STM32_PWM_USE_TIM11
#if defined(STM32_TIM11_IS_USED)
#error "PWMD11 requires TIM11 but the timer is already used"
#else
#define STM32_TIM11_IS_USED
#endif
#endif

#if STM32_PWM_USE_TIM12
#if defined(STM32_TIM12_IS_USED)
#error "PWMD12 requires TIM12 but the timer is already used"
#else
#define STM32_TIM12_IS_USED
#endif
#endif

#if STM32_PWM_USE_TIM13
#if defined(STM32_TIM13_IS_USED)
#error "PWMD13 requires TIM13 but the timer is already used"
#else
#define STM32_TIM13_IS_USED
#endif
#endif

#if STM32_PWM_USE_TIM14
#if defined(STM32_TIM14_IS_USED)
#error "PWMD14 requires TIM14 but the timer is already used"
#else
#define STM32_TIM14_IS_USED
#endif
#endif

#if STM32_PWM_USE_TIM15
#if defined(STM32_TIM15_IS_USED)
#error "PWMD15 requires TIM15 but the timer is already used"
#else
#define STM32_TIM15_IS_USED
#endif
#endif

#if STM32_PWM_USE_TIM16
#if defined(STM32_TIM16_IS_USED)
#error "PWMD16 requires TIM16 but the timer is already used"
#else
#define STM32_TIM16_IS_USED
#endif
#endif

#if STM32_PWM_USE_TIM17
#if defined(STM32_TIM17_IS_USED)
#error "PWMD17 requires TIM17 but the timer is already used"
#else
#define STM32_TIM17_IS_USED
#endif
#endif

#if STM32_PWM_USE_TIM20
#if defined(STM32_TIM20_IS_USED)
#error "PWMD20 requires TIM20 but the timer is already used"
#else
#define STM32_TIM20_IS_USED
#endif
#endif

#if STM32_PWM_USE_TIM21
#if defined(STM32_TIM21_IS_USED)
#error "PWMD21 requires TIM21 but the timer is already used"
#else
#define STM32_TIM21_IS_USED
#endif
#endif

#if STM32_PWM_USE_TIM22
#if defined(STM32_TIM22_IS_USED)
#error "PWMD22 requires TIM22 but the timer is already used"
#else
#define STM32_TIM22_IS_USED
#endif
#endif

/*===========================================================================*/
/* Driver data structures and types.                                         */
/*===========================================================================*/

#define pwm_lld_config_fields                                               \
  uint32_t                  cr2;                                            \
  uint32_t                  bdtr;                                           \
  uint32_t                  dier

#define pwm_lld_driver_fields                                               \
  uint32_t                  clock;                                          \
  bool                      has_bdtr;                                       \
  stm32_tim_t               *tim

/*===========================================================================*/
/* Driver macros.                                                            */
/*===========================================================================*/

/**
 * @brief   Changes the period the PWM peripheral.
 *
 * @param[in] pwmp      pointer to a @p hal_pwm_driver_c object
 * @param[in] period    new cycle time in ticks
 *
 * @notapi
 */
#define pwm_lld_change_period(pwmp, period)                                 \
  ((pwmp)->tim->ARR = (uint32_t)((period) - 1U))

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#if STM32_PWM_USE_TIM1 && !defined(__DOXYGEN__)
extern hal_pwm_driver_c PWMD1;
#endif

#if STM32_PWM_USE_TIM2 && !defined(__DOXYGEN__)
extern hal_pwm_driver_c PWMD2;
#endif

#if STM32_PWM_USE_TIM3 && !defined(__DOXYGEN__)
extern hal_pwm_driver_c PWMD3;
#endif

#if STM32_PWM_USE_TIM4 && !defined(__DOXYGEN__)
extern hal_pwm_driver_c PWMD4;
#endif

#if STM32_PWM_USE_TIM5 && !defined(__DOXYGEN__)
extern hal_pwm_driver_c PWMD5;
#endif

#if STM32_PWM_USE_TIM8 && !defined(__DOXYGEN__)
extern hal_pwm_driver_c PWMD8;
#endif

#if STM32_PWM_USE_TIM9 && !defined(__DOXYGEN__)
extern hal_pwm_driver_c PWMD9;
#endif

#if STM32_PWM_USE_TIM10 && !defined(__DOXYGEN__)
extern hal_pwm_driver_c PWMD10;
#endif

#if STM32_PWM_USE_TIM11 && !defined(__DOXYGEN__)
extern hal_pwm_driver_c PWMD11;
#endif

#if STM32_PWM_USE_TIM12 && !defined(__DOXYGEN__)
extern hal_pwm_driver_c PWMD12;
#endif

#if STM32_PWM_USE_TIM13 && !defined(__DOXYGEN__)
extern hal_pwm_driver_c PWMD13;
#endif

#if STM32_PWM_USE_TIM14 && !defined(__DOXYGEN__)
extern hal_pwm_driver_c PWMD14;
#endif

#if STM32_PWM_USE_TIM15 && !defined(__DOXYGEN__)
extern hal_pwm_driver_c PWMD15;
#endif

#if STM32_PWM_USE_TIM16 && !defined(__DOXYGEN__)
extern hal_pwm_driver_c PWMD16;
#endif

#if STM32_PWM_USE_TIM17 && !defined(__DOXYGEN__)
extern hal_pwm_driver_c PWMD17;
#endif

#if STM32_PWM_USE_TIM20 && !defined(__DOXYGEN__)
extern hal_pwm_driver_c PWMD20;
#endif

#if STM32_PWM_USE_TIM21 && !defined(__DOXYGEN__)
extern hal_pwm_driver_c PWMD21;
#endif

#if STM32_PWM_USE_TIM22 && !defined(__DOXYGEN__)
extern hal_pwm_driver_c PWMD22;
#endif

#ifdef __cplusplus
extern "C" {
#endif
  void pwm_lld_init(void);
  msg_t pwm_lld_start(hal_pwm_driver_c *pwmp);
  void pwm_lld_stop(hal_pwm_driver_c *pwmp);
  const hal_pwm_config_t *pwm_lld_setcfg(hal_pwm_driver_c *pwmp,
                                         const hal_pwm_config_t *config);
  const hal_pwm_config_t *pwm_lld_selcfg(hal_pwm_driver_c *pwmp,
                                         unsigned cfgnum);
  void pwm_lld_set_callback(hal_pwm_driver_c *pwmp, drv_cb_t cb);
  void pwm_lld_enable_channel(hal_pwm_driver_c *pwmp,
                              pwmchannel_t channel,
                              pwmcnt_t width);
  void pwm_lld_disable_channel(hal_pwm_driver_c *pwmp, pwmchannel_t channel);
  void pwm_lld_enable_events(hal_pwm_driver_c *pwmp, pwm_events_t events);
  void pwm_lld_disable_events(hal_pwm_driver_c *pwmp, pwm_events_t events);
  void pwm_lld_serve_interrupt(hal_pwm_driver_c *pwmp);
#ifdef __cplusplus
}
#endif

#endif /* HAL_USE_PWM */

#endif /* HAL_PWM_LLD_H */

/** @} */
