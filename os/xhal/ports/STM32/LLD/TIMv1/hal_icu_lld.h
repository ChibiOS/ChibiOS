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
 * @file    TIMv1/hal_icu_lld.h
 * @brief   STM32 ICU subsystem low level driver header.
 *
 * @addtogroup HAL_ICU
 * @{
 */

#ifndef HAL_ICU_LLD_H
#define HAL_ICU_LLD_H

#include "stm32_tim.h"

#if HAL_USE_ICU || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver constants.                                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Driver pre-compile time settings.                                         */
/*===========================================================================*/

/**
 * @name    Configuration options
 * @{
 */
#if !defined(STM32_ICU_USE_TIM1) || defined(__DOXYGEN__)
#define STM32_ICU_USE_TIM1                  FALSE
#endif

#if !defined(STM32_ICU_USE_TIM2) || defined(__DOXYGEN__)
#define STM32_ICU_USE_TIM2                  FALSE
#endif

#if !defined(STM32_ICU_USE_TIM3) || defined(__DOXYGEN__)
#define STM32_ICU_USE_TIM3                  FALSE
#endif

#if !defined(STM32_ICU_USE_TIM4) || defined(__DOXYGEN__)
#define STM32_ICU_USE_TIM4                  FALSE
#endif

#if !defined(STM32_ICU_USE_TIM5) || defined(__DOXYGEN__)
#define STM32_ICU_USE_TIM5                  FALSE
#endif

#if !defined(STM32_ICU_USE_TIM8) || defined(__DOXYGEN__)
#define STM32_ICU_USE_TIM8                  FALSE
#endif

#if !defined(STM32_ICU_USE_TIM9) || defined(__DOXYGEN__)
#define STM32_ICU_USE_TIM9                  FALSE
#endif

#if !defined(STM32_ICU_USE_TIM10) || defined(__DOXYGEN__)
#define STM32_ICU_USE_TIM10                 FALSE
#endif

#if !defined(STM32_ICU_USE_TIM11) || defined(__DOXYGEN__)
#define STM32_ICU_USE_TIM11                 FALSE
#endif

#if !defined(STM32_ICU_USE_TIM12) || defined(__DOXYGEN__)
#define STM32_ICU_USE_TIM12                 FALSE
#endif

#if !defined(STM32_ICU_USE_TIM13) || defined(__DOXYGEN__)
#define STM32_ICU_USE_TIM13                 FALSE
#endif

#if !defined(STM32_ICU_USE_TIM14) || defined(__DOXYGEN__)
#define STM32_ICU_USE_TIM14                 FALSE
#endif

#if !defined(STM32_ICU_USE_TIM15) || defined(__DOXYGEN__)
#define STM32_ICU_USE_TIM15                 FALSE
#endif

#if !defined(STM32_ICU_USE_TIM16) || defined(__DOXYGEN__)
#define STM32_ICU_USE_TIM16                 FALSE
#endif

#if !defined(STM32_ICU_USE_TIM17) || defined(__DOXYGEN__)
#define STM32_ICU_USE_TIM17                 FALSE
#endif

#if !defined(STM32_ICU_USE_TIM20) || defined(__DOXYGEN__)
#define STM32_ICU_USE_TIM20                 FALSE
#endif

#if !defined(STM32_ICU_USE_TIM21) || defined(__DOXYGEN__)
#define STM32_ICU_USE_TIM21                 FALSE
#endif

#if !defined(STM32_ICU_USE_TIM22) || defined(__DOXYGEN__)
#define STM32_ICU_USE_TIM22                 FALSE
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

#if !defined(STM32_HAS_TIM20)
#define STM32_HAS_TIM20                     FALSE
#endif

#if !defined(STM32_HAS_TIM21)
#define STM32_HAS_TIM21                     FALSE
#endif

#if !defined(STM32_HAS_TIM22)
#define STM32_HAS_TIM22                     FALSE
#endif

#if STM32_ICU_USE_TIM1 && !STM32_HAS_TIM1
#error "TIM1 not present in the selected device"
#endif

#if STM32_ICU_USE_TIM2 && !STM32_HAS_TIM2
#error "TIM2 not present in the selected device"
#endif

#if STM32_ICU_USE_TIM3 && !STM32_HAS_TIM3
#error "TIM3 not present in the selected device"
#endif

#if STM32_ICU_USE_TIM4 && !STM32_HAS_TIM4
#error "TIM4 not present in the selected device"
#endif

#if STM32_ICU_USE_TIM5 && !STM32_HAS_TIM5
#error "TIM5 not present in the selected device"
#endif

#if STM32_ICU_USE_TIM8 && !STM32_HAS_TIM8
#error "TIM8 not present in the selected device"
#endif

#if STM32_ICU_USE_TIM9 && !STM32_HAS_TIM9
#error "TIM9 not present in the selected device"
#endif

#if STM32_ICU_USE_TIM10 && !STM32_HAS_TIM10
#error "TIM10 not present in the selected device"
#endif

#if STM32_ICU_USE_TIM11 && !STM32_HAS_TIM11
#error "TIM11 not present in the selected device"
#endif

#if STM32_ICU_USE_TIM12 && !STM32_HAS_TIM12
#error "TIM12 not present in the selected device"
#endif

#if STM32_ICU_USE_TIM13 && !STM32_HAS_TIM13
#error "TIM13 not present in the selected device"
#endif

#if STM32_ICU_USE_TIM14 && !STM32_HAS_TIM14
#error "TIM14 not present in the selected device"
#endif

#if STM32_ICU_USE_TIM15 && !STM32_HAS_TIM15
#error "TIM15 not present in the selected device"
#endif

#if STM32_ICU_USE_TIM16
#error "TIM16 is not supported by ICU TIMv1"
#endif

#if STM32_ICU_USE_TIM17
#error "TIM17 is not supported by ICU TIMv1"
#endif

#if STM32_ICU_USE_TIM20 && !STM32_HAS_TIM20
#error "TIM20 not present in the selected device"
#endif

#if STM32_ICU_USE_TIM21 && !STM32_HAS_TIM21
#error "TIM21 not present in the selected device"
#endif

#if STM32_ICU_USE_TIM22 && !STM32_HAS_TIM22
#error "TIM22 not present in the selected device"
#endif

#if !STM32_ICU_USE_TIM1  && !STM32_ICU_USE_TIM2  &&                         \
    !STM32_ICU_USE_TIM3  && !STM32_ICU_USE_TIM4  &&                         \
    !STM32_ICU_USE_TIM5  && !STM32_ICU_USE_TIM8  &&                         \
    !STM32_ICU_USE_TIM9  && !STM32_ICU_USE_TIM10 &&                         \
    !STM32_ICU_USE_TIM11 && !STM32_ICU_USE_TIM12 &&                         \
    !STM32_ICU_USE_TIM13 && !STM32_ICU_USE_TIM14 &&                         \
    !STM32_ICU_USE_TIM15 && !STM32_ICU_USE_TIM20 &&                         \
    !STM32_ICU_USE_TIM21 && !STM32_ICU_USE_TIM22
#error "ICU driver activated but no TIM peripheral assigned"
#endif

/* Checks on allocation of TIMx units.*/
#if STM32_ICU_USE_TIM1
#if defined(STM32_TIM1_IS_USED)
#error "ICUD1 requires TIM1 but the timer is already used"
#else
#define STM32_TIM1_IS_USED
#endif
#endif

#if STM32_ICU_USE_TIM2
#if defined(STM32_TIM2_IS_USED)
#error "ICUD2 requires TIM2 but the timer is already used"
#else
#define STM32_TIM2_IS_USED
#endif
#endif

#if STM32_ICU_USE_TIM3
#if defined(STM32_TIM3_IS_USED)
#error "ICUD3 requires TIM3 but the timer is already used"
#else
#define STM32_TIM3_IS_USED
#endif
#endif

#if STM32_ICU_USE_TIM4
#if defined(STM32_TIM4_IS_USED)
#error "ICUD4 requires TIM4 but the timer is already used"
#else
#define STM32_TIM4_IS_USED
#endif
#endif

#if STM32_ICU_USE_TIM5
#if defined(STM32_TIM5_IS_USED)
#error "ICUD5 requires TIM5 but the timer is already used"
#else
#define STM32_TIM5_IS_USED
#endif
#endif

#if STM32_ICU_USE_TIM8
#if defined(STM32_TIM8_IS_USED)
#error "ICUD8 requires TIM8 but the timer is already used"
#else
#define STM32_TIM8_IS_USED
#endif
#endif

#if STM32_ICU_USE_TIM9
#if defined(STM32_TIM9_IS_USED)
#error "ICUD9 requires TIM9 but the timer is already used"
#else
#define STM32_TIM9_IS_USED
#endif
#endif

#if STM32_ICU_USE_TIM10
#if defined(STM32_TIM10_IS_USED)
#error "ICUD10 requires TIM10 but the timer is already used"
#else
#define STM32_TIM10_IS_USED
#endif
#endif

#if STM32_ICU_USE_TIM11
#if defined(STM32_TIM11_IS_USED)
#error "ICUD11 requires TIM11 but the timer is already used"
#else
#define STM32_TIM11_IS_USED
#endif
#endif

#if STM32_ICU_USE_TIM12
#if defined(STM32_TIM12_IS_USED)
#error "ICUD12 requires TIM12 but the timer is already used"
#else
#define STM32_TIM12_IS_USED
#endif
#endif

#if STM32_ICU_USE_TIM13
#if defined(STM32_TIM13_IS_USED)
#error "ICUD13 requires TIM13 but the timer is already used"
#else
#define STM32_TIM13_IS_USED
#endif
#endif

#if STM32_ICU_USE_TIM14
#if defined(STM32_TIM14_IS_USED)
#error "ICUD14 requires TIM14 but the timer is already used"
#else
#define STM32_TIM14_IS_USED
#endif
#endif

#if STM32_ICU_USE_TIM15
#if defined(STM32_TIM15_IS_USED)
#error "ICUD15 requires TIM15 but the timer is already used"
#else
#define STM32_TIM15_IS_USED
#endif
#endif

#if STM32_ICU_USE_TIM20
#if defined(STM32_TIM20_IS_USED)
#error "ICUD20 requires TIM20 but the timer is already used"
#else
#define STM32_TIM20_IS_USED
#endif
#endif

#if STM32_ICU_USE_TIM21
#if defined(STM32_TIM21_IS_USED)
#error "ICUD21 requires TIM21 but the timer is already used"
#else
#define STM32_TIM21_IS_USED
#endif
#endif

#if STM32_ICU_USE_TIM22
#if defined(STM32_TIM22_IS_USED)
#error "ICUD22 requires TIM22 but the timer is already used"
#else
#define STM32_TIM22_IS_USED
#endif
#endif

/*===========================================================================*/
/* Driver data structures and types.                                         */
/*===========================================================================*/

#define icu_lld_config_fields                                               \
  icuchannel_t              channel;                                        \
  uint32_t                  dier;                                           \
  uint32_t                  arr

#define icu_lld_driver_fields                                               \
  uint32_t                  clock;                                          \
  stm32_tim_t               *tim;                                           \
  volatile uint32_t         *wccrp;                                         \
  volatile uint32_t         *pccrp

/*===========================================================================*/
/* Driver macros.                                                            */
/*===========================================================================*/

/**
 * @brief   Returns the width of the latest pulse.
 *
 * @param[in] icup      pointer to the @p hal_icu_driver_c object
 * @return              The number of ticks.
 *
 * @notapi
 */
#define icu_lld_get_width(icup)                                             \
  ((icucnt_t)(*((icup)->wccrp) + 1U))

/**
 * @brief   Returns the width of the latest cycle.
 *
 * @param[in] icup      pointer to the @p hal_icu_driver_c object
 * @return              The number of ticks.
 *
 * @notapi
 */
#define icu_lld_get_period(icup)                                            \
  ((icucnt_t)(*((icup)->pccrp) + 1U))

/**
 * @brief   Check on notifications status.
 *
 * @param[in] icup      pointer to the @p hal_icu_driver_c object
 * @return              The notifications status.
 *
 * @notapi
 */
#define icu_lld_are_notifications_enabled(icup)                             \
  (bool)(((icup)->tim->DIER & STM32_TIM_DIER_IRQ_MASK) != 0U)

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#if STM32_ICU_USE_TIM1 && !defined(__DOXYGEN__)
extern hal_icu_driver_c ICUD1;
#endif

#if STM32_ICU_USE_TIM2 && !defined(__DOXYGEN__)
extern hal_icu_driver_c ICUD2;
#endif

#if STM32_ICU_USE_TIM3 && !defined(__DOXYGEN__)
extern hal_icu_driver_c ICUD3;
#endif

#if STM32_ICU_USE_TIM4 && !defined(__DOXYGEN__)
extern hal_icu_driver_c ICUD4;
#endif

#if STM32_ICU_USE_TIM5 && !defined(__DOXYGEN__)
extern hal_icu_driver_c ICUD5;
#endif

#if STM32_ICU_USE_TIM8 && !defined(__DOXYGEN__)
extern hal_icu_driver_c ICUD8;
#endif

#if STM32_ICU_USE_TIM9 && !defined(__DOXYGEN__)
extern hal_icu_driver_c ICUD9;
#endif

#if STM32_ICU_USE_TIM10 && !defined(__DOXYGEN__)
extern hal_icu_driver_c ICUD10;
#endif

#if STM32_ICU_USE_TIM11 && !defined(__DOXYGEN__)
extern hal_icu_driver_c ICUD11;
#endif

#if STM32_ICU_USE_TIM12 && !defined(__DOXYGEN__)
extern hal_icu_driver_c ICUD12;
#endif

#if STM32_ICU_USE_TIM13 && !defined(__DOXYGEN__)
extern hal_icu_driver_c ICUD13;
#endif

#if STM32_ICU_USE_TIM14 && !defined(__DOXYGEN__)
extern hal_icu_driver_c ICUD14;
#endif

#if STM32_ICU_USE_TIM15 && !defined(__DOXYGEN__)
extern hal_icu_driver_c ICUD15;
#endif

#if STM32_ICU_USE_TIM20 && !defined(__DOXYGEN__)
extern hal_icu_driver_c ICUD20;
#endif

#if STM32_ICU_USE_TIM21 && !defined(__DOXYGEN__)
extern hal_icu_driver_c ICUD21;
#endif

#if STM32_ICU_USE_TIM22 && !defined(__DOXYGEN__)
extern hal_icu_driver_c ICUD22;
#endif

#ifdef __cplusplus
extern "C" {
#endif
  void icu_lld_init(void);
  msg_t icu_lld_start(hal_icu_driver_c *icup);
  void icu_lld_stop(hal_icu_driver_c *icup);
  const hal_icu_config_t *icu_lld_setcfg(hal_icu_driver_c *icup,
                                         const hal_icu_config_t *config);
  const hal_icu_config_t *icu_lld_selcfg(hal_icu_driver_c *icup,
                                         unsigned cfgnum);
  void icu_lld_set_callback(hal_icu_driver_c *icup, drv_cb_t cb);
  void icu_lld_start_capture(hal_icu_driver_c *icup);
  bool icu_lld_wait_capture(hal_icu_driver_c *icup);
  void icu_lld_stop_capture(hal_icu_driver_c *icup);
  void icu_lld_enable_events(hal_icu_driver_c *icup, icu_events_t events);
  void icu_lld_disable_events(hal_icu_driver_c *icup, icu_events_t events);
  void icu_lld_serve_interrupt(hal_icu_driver_c *icup);
#ifdef __cplusplus
}
#endif

#endif /* HAL_USE_ICU */

#endif /* HAL_ICU_LLD_H */

/** @} */
