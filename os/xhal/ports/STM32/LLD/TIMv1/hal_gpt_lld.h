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
 * @file    TIMv1/hal_gpt_lld.h
 * @brief   STM32 GPT subsystem low level driver header.
 *
 * @addtogroup GPT
 * @{
 */

#ifndef HAL_GPT_LLD_H
#define HAL_GPT_LLD_H

#include "stm32_tim.h"

#if HAL_USE_GPT || defined(__DOXYGEN__)

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
/**
 * @brief   GPTD1 driver enable switch.
 * @details If set to @p TRUE the support for GPTD1 is included.
 * @note    The default is @p FALSE.
 */
#if !defined(STM32_GPT_USE_TIM1) || defined(__DOXYGEN__)
#define STM32_GPT_USE_TIM1                  FALSE
#endif

/**
 * @brief   GPTD2 driver enable switch.
 * @details If set to @p TRUE the support for GPTD2 is included.
 * @note    The default is @p FALSE.
 */
#if !defined(STM32_GPT_USE_TIM2) || defined(__DOXYGEN__)
#define STM32_GPT_USE_TIM2                  FALSE
#endif

/**
 * @brief   GPTD3 driver enable switch.
 * @details If set to @p TRUE the support for GPTD3 is included.
 * @note    The default is @p FALSE.
 */
#if !defined(STM32_GPT_USE_TIM3) || defined(__DOXYGEN__)
#define STM32_GPT_USE_TIM3                  FALSE
#endif

/**
 * @brief   GPTD4 driver enable switch.
 * @details If set to @p TRUE the support for GPTD4 is included.
 * @note    The default is @p FALSE.
 */
#if !defined(STM32_GPT_USE_TIM4) || defined(__DOXYGEN__)
#define STM32_GPT_USE_TIM4                  FALSE
#endif

/**
 * @brief   GPTD5 driver enable switch.
 * @details If set to @p TRUE the support for GPTD5 is included.
 * @note    The default is @p FALSE.
 */
#if !defined(STM32_GPT_USE_TIM5) || defined(__DOXYGEN__)
#define STM32_GPT_USE_TIM5                  FALSE
#endif

/**
 * @brief   GPTD6 driver enable switch.
 * @details If set to @p TRUE the support for GPTD6 is included.
 * @note    The default is @p FALSE.
 */
#if !defined(STM32_GPT_USE_TIM6) || defined(__DOXYGEN__)
#define STM32_GPT_USE_TIM6                  FALSE
#endif

/**
 * @brief   GPTD7 driver enable switch.
 * @details If set to @p TRUE the support for GPTD7 is included.
 * @note    The default is @p FALSE.
 */
#if !defined(STM32_GPT_USE_TIM7) || defined(__DOXYGEN__)
#define STM32_GPT_USE_TIM7                  FALSE
#endif

/**
 * @brief   GPTD8 driver enable switch.
 * @details If set to @p TRUE the support for GPTD8 is included.
 * @note    The default is @p FALSE.
 */
#if !defined(STM32_GPT_USE_TIM8) || defined(__DOXYGEN__)
#define STM32_GPT_USE_TIM8                  FALSE
#endif

/**
 * @brief   GPTD9 driver enable switch.
 * @details If set to @p TRUE the support for GPTD9 is included.
 * @note    The default is @p FALSE.
 */
#if !defined(STM32_GPT_USE_TIM9) || defined(__DOXYGEN__)
#define STM32_GPT_USE_TIM9                  FALSE
#endif

/**
 * @brief   GPTD10 driver enable switch.
 * @details If set to @p TRUE the support for GPTD10 is included.
 * @note    The default is @p FALSE.
 */
#if !defined(STM32_GPT_USE_TIM10) || defined(__DOXYGEN__)
#define STM32_GPT_USE_TIM10                 FALSE
#endif

/**
 * @brief   GPTD11 driver enable switch.
 * @details If set to @p TRUE the support for GPTD11 is included.
 * @note    The default is @p FALSE.
 */
#if !defined(STM32_GPT_USE_TIM11) || defined(__DOXYGEN__)
#define STM32_GPT_USE_TIM11                 FALSE
#endif

/**
 * @brief   GPTD12 driver enable switch.
 * @details If set to @p TRUE the support for GPTD12 is included.
 * @note    The default is @p FALSE.
 */
#if !defined(STM32_GPT_USE_TIM12) || defined(__DOXYGEN__)
#define STM32_GPT_USE_TIM12                 FALSE
#endif

/**
 * @brief   GPTD13 driver enable switch.
 * @details If set to @p TRUE the support for GPTD13 is included.
 * @note    The default is @p FALSE.
 */
#if !defined(STM32_GPT_USE_TIM13) || defined(__DOXYGEN__)
#define STM32_GPT_USE_TIM13                 FALSE
#endif

/**
 * @brief   GPTD14 driver enable switch.
 * @details If set to @p TRUE the support for GPTD14 is included.
 * @note    The default is @p FALSE.
 */
#if !defined(STM32_GPT_USE_TIM14) || defined(__DOXYGEN__)
#define STM32_GPT_USE_TIM14                 FALSE
#endif

/**
 * @brief   GPTD14 driver enable switch.
 * @details If set to @p TRUE the support for GPTD15 is included.
 * @note    The default is @p FALSE.
 */
#if !defined(STM32_GPT_USE_TIM15) || defined(__DOXYGEN__)
#define STM32_GPT_USE_TIM15                 FALSE
#endif

/**
 * @brief   GPTD14 driver enable switch.
 * @details If set to @p TRUE the support for GPTD16 is included.
 * @note    The default is @p FALSE.
 */
#if !defined(STM32_GPT_USE_TIM16) || defined(__DOXYGEN__)
#define STM32_GPT_USE_TIM16                 FALSE
#endif

/**
 * @brief   GPTD14 driver enable switch.
 * @details If set to @p TRUE the support for GPTD17 is included.
 * @note    The default is @p FALSE.
 */
#if !defined(STM32_GPT_USE_TIM17) || defined(__DOXYGEN__)
#define STM32_GPT_USE_TIM17                 FALSE
#endif


/**
 * @brief   GPTD20 driver enable switch.
 * @details If set to @p TRUE the support for GPTD20 is included.
 * @note    The default is @p FALSE.
 */
#if !defined(STM32_GPT_USE_TIM20) || defined(__DOXYGEN__)
#define STM32_GPT_USE_TIM20                 FALSE
#endif

/**
 * @brief   GPTD21 driver enable switch.
 * @details If set to @p TRUE the support for GPTD21 is included.
 * @note    The default is @p FALSE.
 */
#if !defined(STM32_GPT_USE_TIM21) || defined(__DOXYGEN__)
#define STM32_GPT_USE_TIM21                 FALSE
#endif

/**
 * @brief   GPTD22 driver enable switch.
 * @details If set to @p TRUE the support for GPTD22 is included.
 * @note    The default is @p FALSE.
 */
#if !defined(STM32_GPT_USE_TIM22) || defined(__DOXYGEN__)
#define STM32_GPT_USE_TIM22                 FALSE
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

#if !defined(STM32_HAS_TIM6)
#define STM32_HAS_TIM6                      FALSE
#endif

#if !defined(STM32_HAS_TIM7)
#define STM32_HAS_TIM7                      FALSE
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

#if STM32_GPT_USE_TIM1 && !STM32_HAS_TIM1
#error "TIM1 not present in the selected device"
#endif

#if STM32_GPT_USE_TIM2 && !STM32_HAS_TIM2
#error "TIM2 not present in the selected device"
#endif

#if STM32_GPT_USE_TIM3 && !STM32_HAS_TIM3
#error "TIM3 not present in the selected device"
#endif

#if STM32_GPT_USE_TIM4 && !STM32_HAS_TIM4
#error "TIM4 not present in the selected device"
#endif

#if STM32_GPT_USE_TIM5 && !STM32_HAS_TIM5
#error "TIM5 not present in the selected device"
#endif

#if STM32_GPT_USE_TIM6 && !STM32_HAS_TIM6
#error "TIM6 not present in the selected device"
#endif

#if STM32_GPT_USE_TIM7 && !STM32_HAS_TIM7
#error "TIM7 not present in the selected device"
#endif

#if STM32_GPT_USE_TIM8 && !STM32_HAS_TIM8
#error "TIM8 not present in the selected device"
#endif

#if STM32_GPT_USE_TIM9 && !STM32_HAS_TIM9
#error "TIM9 not present in the selected device"
#endif

#if STM32_GPT_USE_TIM10 && !STM32_HAS_TIM10
#error "TIM10 not present in the selected device"
#endif

#if STM32_GPT_USE_TIM11 && !STM32_HAS_TIM11
#error "TIM11 not present in the selected device"
#endif

#if STM32_GPT_USE_TIM12 && !STM32_HAS_TIM12
#error "TIM12 not present in the selected device"
#endif

#if STM32_GPT_USE_TIM13 && !STM32_HAS_TIM13
#error "TIM13 not present in the selected device"
#endif

#if STM32_GPT_USE_TIM14 && !STM32_HAS_TIM14
#error "TIM14 not present in the selected device"
#endif

#if STM32_GPT_USE_TIM15 && !STM32_HAS_TIM15
#error "TIM15 not present in the selected device"
#endif

#if STM32_GPT_USE_TIM16 && !STM32_HAS_TIM16
#error "TIM16 not present in the selected device"
#endif

#if STM32_GPT_USE_TIM17 && !STM32_HAS_TIM17
#error "TIM17 not present in the selected device"
#endif

#if STM32_GPT_USE_TIM20 && !STM32_HAS_TIM20
#error "TIM20 not present in the selected device"
#endif

#if STM32_GPT_USE_TIM21 && !STM32_HAS_TIM21
#error "TIM21 not present in the selected device"
#endif

#if STM32_GPT_USE_TIM22 && !STM32_HAS_TIM22
#error "TIM22 not present in the selected device"
#endif

#if !STM32_GPT_USE_TIM1  && !STM32_GPT_USE_TIM2 &&                          \
    !STM32_GPT_USE_TIM3  && !STM32_GPT_USE_TIM4 &&                          \
    !STM32_GPT_USE_TIM5  && !STM32_GPT_USE_TIM6 &&                          \
    !STM32_GPT_USE_TIM7  && !STM32_GPT_USE_TIM8 &&                          \
    !STM32_GPT_USE_TIM9  && !STM32_GPT_USE_TIM10 &&                         \
    !STM32_GPT_USE_TIM11 && !STM32_GPT_USE_TIM12 &&                         \
    !STM32_GPT_USE_TIM13 && !STM32_GPT_USE_TIM14 &&                         \
    !STM32_GPT_USE_TIM15 && !STM32_GPT_USE_TIM16 &&                         \
    !STM32_GPT_USE_TIM17 && !STM32_GPT_USE_TIM20 &&                         \
    !STM32_GPT_USE_TIM21 && !STM32_GPT_USE_TIM22
#error "GPT driver activated but no TIM peripheral assigned"
#endif

/* Checks on allocation of TIMx units.*/
#if STM32_GPT_USE_TIM1
#if defined(STM32_TIM1_IS_USED)
#error "GPTD1 requires TIM1 but the timer is already used"
#else
#define STM32_TIM1_IS_USED
#endif
#endif

#if STM32_GPT_USE_TIM2
#if defined(STM32_TIM2_IS_USED)
#error "GPTD2 requires TIM2 but the timer is already used"
#else
#define STM32_TIM2_IS_USED
#endif
#endif

#if STM32_GPT_USE_TIM3
#if defined(STM32_TIM3_IS_USED)
#error "GPTD3 requires TIM3 but the timer is already used"
#else
#define STM32_TIM3_IS_USED
#endif
#endif

#if STM32_GPT_USE_TIM4
#if defined(STM32_TIM4_IS_USED)
#error "GPTD4 requires TIM4 but the timer is already used"
#else
#define STM32_TIM4_IS_USED
#endif
#endif

#if STM32_GPT_USE_TIM5
#if defined(STM32_TIM5_IS_USED)
#error "GPTD5 requires TIM5 but the timer is already used"
#else
#define STM32_TIM5_IS_USED
#endif
#endif

#if STM32_GPT_USE_TIM6
#if defined(STM32_TIM6_IS_USED)
#error "GPTD6 requires TIM6 but the timer is already used"
#else
#define STM32_TIM6_IS_USED
#endif
#endif

#if STM32_GPT_USE_TIM7
#if defined(STM32_TIM7_IS_USED)
#error "GPTD7 requires TIM7 but the timer is already used"
#else
#define STM32_TIM7_IS_USED
#endif
#endif

#if STM32_GPT_USE_TIM8
#if defined(STM32_TIM8_IS_USED)
#error "GPTD8 requires TIM8 but the timer is already used"
#else
#define STM32_TIM8_IS_USED
#endif
#endif

#if STM32_GPT_USE_TIM9
#if defined(STM32_TIM9_IS_USED)
#error "GPTD9 requires TIM9 but the timer is already used"
#else
#define STM32_TIM9_IS_USED
#endif
#endif

#if STM32_GPT_USE_TIM10
#if defined(STM32_TIM10_IS_USED)
#error "GPTD10 requires TIM10 but the timer is already used"
#else
#define STM32_TIM10_IS_USED
#endif
#endif

#if STM32_GPT_USE_TIM11
#if defined(STM32_TIM11_IS_USED)
#error "GPTD11 requires TIM11 but the timer is already used"
#else
#define STM32_TIM11_IS_USED
#endif
#endif

#if STM32_GPT_USE_TIM12
#if defined(STM32_TIM12_IS_USED)
#error "GPTD12 requires TIM12 but the timer is already used"
#else
#define STM32_TIM12_IS_USED
#endif
#endif

#if STM32_GPT_USE_TIM13
#if defined(STM32_TIM13_IS_USED)
#error "GPTD13 requires TIM13 but the timer is already used"
#else
#define STM32_TIM13_IS_USED
#endif
#endif

#if STM32_GPT_USE_TIM14
#if defined(STM32_TIM14_IS_USED)
#error "GPTD14 requires TIM14 but the timer is already used"
#else
#define STM32_TIM14_IS_USED
#endif
#endif

#if STM32_GPT_USE_TIM15
#if defined(STM32_TIM15_IS_USED)
#error "GPTD14 requires TIM15 but the timer is already used"
#else
#define STM32_TIM15_IS_USED
#endif
#endif

#if STM32_GPT_USE_TIM16
#if defined(STM32_TIM16_IS_USED)
#error "GPTD14 requires TIM16 but the timer is already used"
#else
#define STM32_TIM16_IS_USED
#endif
#endif

#if STM32_GPT_USE_TIM17
#if defined(STM32_TIM17_IS_USED)
#error "GPTD14 requires TIM17 but the timer is already used"
#else
#define STM32_TIM17_IS_USED
#endif
#endif

#if STM32_GPT_USE_TIM20
#if defined(STM32_TIM20_IS_USED)
#error "GPTD20 requires TIM20 but the timer is already used"
#else
#define STM32_TIM20_IS_USED
#endif
#endif

#if STM32_GPT_USE_TIM21
#if defined(STM32_TIM21_IS_USED)
#error "GPTD21 requires TIM21 but the timer is already used"
#else
#define STM32_TIM21_IS_USED
#endif
#endif

#if STM32_GPT_USE_TIM22
#if defined(STM32_TIM22_IS_USED)
#error "GPTD22 requires TIM22 but the timer is already used"
#else
#define STM32_TIM22_IS_USED
#endif
#endif

/*===========================================================================*/
/* Driver data structures and types.                                         */
/*===========================================================================*/

#define gpt_lld_config_fields                                               \
  uint32_t                  cr2;                                            \
  uint32_t                  dier

#define gpt_lld_driver_fields                                               \
  uint32_t                  clock;                                          \
  stm32_tim_t               *tim

/*===========================================================================*/
/* Driver macros.                                                            */
/*===========================================================================*/

/**
 * @brief   Changes the interval of GPT peripheral.
 * @details This function changes the interval of a running GPT unit.
 * @pre     The GPT unit must be running in continuous mode.
 * @post    The GPT unit interval is changed to the new value.
 * @note    The function has effect at the next cycle start.
 *
 * @param[in] gptp      pointer to a @p hal_gpt_driver_c object
 * @param[in] interval  new cycle time in timer ticks
 *
 * @notapi
 */
#define gpt_lld_change_interval(gptp, interval)                             \
  ((gptp)->tim->ARR = (uint32_t)((interval) - 1U))

/**
 * @brief   Returns the interval of GPT peripheral.
 * @pre     The GPT unit must be running in continuous mode.
 *
 * @param[in] gptp      pointer to a @p hal_gpt_driver_c object
 * @return              The current interval.
 *
 * @notapi
 */
#define gpt_lld_get_interval(gptp) ((gptcnt_t)((gptp)->tim->ARR + 1U))

/**
 * @brief   Returns the counter value of GPT peripheral.
 * @pre     The GPT unit must be running in continuous mode.
 * @note    The nature of the counter is not defined, it may count upward
 *          or downward, it could be continuously running or not.
 *
 * @param[in] gptp      pointer to a @p hal_gpt_driver_c object
 * @return              The current counter value.
 *
 * @notapi
 */
#define gpt_lld_get_counter(gptp) ((gptcnt_t)(gptp)->tim->CNT)

/**
 * @brief   Returns the TIM associated with a GPT.
 *
 * @param[in] gptp      pointer to the @p hal_gpt_driver_c object
 * @return              The TIM reference.
 *
 * @notapi
 */
#define gpt_lld_get_timer(gptp) ((gptp)->tim)

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#if STM32_GPT_USE_TIM1 && !defined(__DOXYGEN__)
extern hal_gpt_driver_c GPTD1;
#endif

#if STM32_GPT_USE_TIM2 && !defined(__DOXYGEN__)
extern hal_gpt_driver_c GPTD2;
#endif

#if STM32_GPT_USE_TIM3 && !defined(__DOXYGEN__)
extern hal_gpt_driver_c GPTD3;
#endif

#if STM32_GPT_USE_TIM4 && !defined(__DOXYGEN__)
extern hal_gpt_driver_c GPTD4;
#endif

#if STM32_GPT_USE_TIM5 && !defined(__DOXYGEN__)
extern hal_gpt_driver_c GPTD5;
#endif

#if STM32_GPT_USE_TIM6 && !defined(__DOXYGEN__)
extern hal_gpt_driver_c GPTD6;
#endif

#if STM32_GPT_USE_TIM7 && !defined(__DOXYGEN__)
extern hal_gpt_driver_c GPTD7;
#endif

#if STM32_GPT_USE_TIM8 && !defined(__DOXYGEN__)
extern hal_gpt_driver_c GPTD8;
#endif

#if STM32_GPT_USE_TIM9 && !defined(__DOXYGEN__)
extern hal_gpt_driver_c GPTD9;
#endif

#if STM32_GPT_USE_TIM10 && !defined(__DOXYGEN__)
extern hal_gpt_driver_c GPTD10;
#endif

#if STM32_GPT_USE_TIM11 && !defined(__DOXYGEN__)
extern hal_gpt_driver_c GPTD11;
#endif

#if STM32_GPT_USE_TIM12 && !defined(__DOXYGEN__)
extern hal_gpt_driver_c GPTD12;
#endif

#if STM32_GPT_USE_TIM13 && !defined(__DOXYGEN__)
extern hal_gpt_driver_c GPTD13;
#endif

#if STM32_GPT_USE_TIM14 && !defined(__DOXYGEN__)
extern hal_gpt_driver_c GPTD14;
#endif

#if STM32_GPT_USE_TIM15 && !defined(__DOXYGEN__)
extern hal_gpt_driver_c GPTD15;
#endif

#if STM32_GPT_USE_TIM16 && !defined(__DOXYGEN__)
extern hal_gpt_driver_c GPTD16;
#endif

#if STM32_GPT_USE_TIM17 && !defined(__DOXYGEN__)
extern hal_gpt_driver_c GPTD17;
#endif

#if STM32_GPT_USE_TIM20 && !defined(__DOXYGEN__)
extern hal_gpt_driver_c GPTD20;
#endif

#if STM32_GPT_USE_TIM21 && !defined(__DOXYGEN__)
extern hal_gpt_driver_c GPTD21;
#endif

#if STM32_GPT_USE_TIM22 && !defined(__DOXYGEN__)
extern hal_gpt_driver_c GPTD22;
#endif

#ifdef __cplusplus
extern "C" {
#endif
  void gpt_lld_init(void);
  msg_t gpt_lld_start(hal_gpt_driver_c *gptp);
  void gpt_lld_stop(hal_gpt_driver_c *gptp);
  const hal_gpt_config_t *gpt_lld_setcfg(hal_gpt_driver_c *gptp,
                                         const hal_gpt_config_t *config);
  const hal_gpt_config_t *gpt_lld_selcfg(hal_gpt_driver_c *gptp,
                                         unsigned cfgnum);
  void gpt_lld_set_callback(hal_gpt_driver_c *gptp, drv_cb_t cb);
  void gpt_lld_start_timer(hal_gpt_driver_c *gptp, gptcnt_t period);
  void gpt_lld_stop_timer(hal_gpt_driver_c *gptp);
  void gpt_lld_polled_delay(hal_gpt_driver_c *gptp, gptcnt_t interval);
  void gpt_lld_serve_interrupt(hal_gpt_driver_c *gptp);
#ifdef __cplusplus
}
#endif

#endif /* HAL_USE_GPT */

#endif /* HAL_GPT_LLD_H */

/** @} */
