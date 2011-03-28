/*
    ChibiOS/RT - Copyright (C) 2006,2007,2008,2009,2010,
                 2011 Giovanni Di Sirio.

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
 * @file    STM32/icu_lld.h
 * @brief   STM32 ICU subsystem low level driver header.
 *
 * @addtogroup ICU
 * @{
 */

#ifndef _ICU_LLD_H_
#define _ICU_LLD_H_

#if HAL_USE_ICU || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver constants.                                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Driver pre-compile time settings.                                         */
/*===========================================================================*/

/**
 * @brief   ICUD1 driver enable switch.
 * @details If set to @p TRUE the support for ICUD1 is included.
 * @note    The default is @p TRUE.
 */
#if !defined(STM32_ICU_USE_TIM1) || defined(__DOXYGEN__)
#define STM32_ICU_USE_TIM1                  TRUE
#endif

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

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

#if !STM32_ICU_USE_TIM1 && !STM32_ICU_USE_TIM2 &&                           \
    !STM32_ICU_USE_TIM3 && !STM32_ICU_USE_TIM4 &&                           \
    !STM32_ICU_USE_TIM5
#error "ICU driver activated but no TIM peripheral assigned"
#endif

/*===========================================================================*/
/* Driver data structures and types.                                         */
/*===========================================================================*/

/**
 * @brief ICU driver mode.
 */
typedef enum {
  ICU_INPUT_ACTIVE_HIGH = 0,        /**< Trigger on rising edge.            */
  ICU_INPUT_ACTIVE_LOW = 1,         /**< Trigger on falling edge.           */
} icumode_t;

/**
 * @brief   ICU counter type.
 */
typedef uint16_t icucnt_t;

/**
 * @brief   Type of a structure representing an ICU driver.
 */
typedef struct ICUDriver ICUDriver;

/**
 * @brief   ICU notification callback type.
 *
 * @param[in] icup      pointer to a @p ICUDriver object
 */
typedef void (*icucallback_t)(ICUDriver *icup);

/**
 * @brief   Driver configuration structure.
 * @note    It could be empty on some architectures.
 */
typedef struct {
  /**
   * @brief   Driver mode.
   */
  icumode_t                 mode;
  /**
   * @brief   Callback for pulse width measurement.
   */
  icucallback_t             width_cb;
  /**
   * @brief   Callback for cycle period measurement.
   */
  icucallback_t             period_cb;
  /* End of the mandatory fields.*/
  /**
   * @brief TIM PSC (pre-scaler) register initialization data.
   */
  uint16_t                  psc;
} ICUConfig;

/**
 * @brief   Structure representing an ICU driver.
 */
struct ICUDriver {
  /**
   * @brief Driver state.
   */
  icustate_t                state;
  /**
   * @brief Current configuration data.
   */
  const ICUConfig           *config;
  /* End of the mandatory fields.*/
  /**
   * @brief Pointer to the TIMx registers block.
   */
  TIM_TypeDef               *tim;
};

/*===========================================================================*/
/* Driver macros.                                                            */
/*===========================================================================*/


/**
 * @brief   Returns the width of the latest pulse.
 * @details The pulse width is defined as number of ticks between the start
 *          edge and the stop edge.
 *
 * @param[in] icup      pointer to the @p ICUDriver object
 * @return              The number of ticks.
 *
 * @notapi
 */
#define icu_lld_get_width(icup) ((icup)->tim->CCR2)

/**
 * @brief   Returns the width of the latest cycle.
 * @details The cycle width is defined as number of ticks between a start
 *          edge and the next start edge.
 *
 * @param[in] icup      pointer to the @p ICUDriver object
 * @return              The number of ticks.
 *
 * @notapi
 */
#define icu_lld_get_period(icup) ((icup)->tim->CCR1)

/**
 * @brief   ICU clock prescaler initialization utility.
 * @note    The real clock value is rounded to the lower valid value, please
 *          make sure that the source clock frequency is a multiple of the
 *          requested ICU clock frequency.
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
 * @param[in] icuclk    ICU clock frequency in cycles
 * @return              The value to be stored in the @p psc field of the
 *                      @p ICUConfig structure.
 */
#define ICU_COMPUTE_PSC(clksrc, icuclk)                                     \
  ((uint16_t)(((clksrc) / (icuclk)) - 1))

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#if STM32_ICU_USE_TIM1 && !defined(__DOXYGEN__)
extern ICUDriver ICUD1;
#endif

#ifdef __cplusplus
extern "C" {
#endif
  void icu_lld_init(void);
  void icu_lld_start(ICUDriver *icup);
  void icu_lld_stop(ICUDriver *icup);
  void icu_lld_enable(ICUDriver *icup);
  void icu_lld_disable(ICUDriver *icup);
  icucnt_t icu_lld_get_width(ICUDriver *icup);
  icucnt_t icu_lld_get_period(ICUDriver *icup);
#ifdef __cplusplus
}
#endif

#endif /* HAL_USE_ICU */

#endif /* _ICU_LLD_H_ */

/** @} */
