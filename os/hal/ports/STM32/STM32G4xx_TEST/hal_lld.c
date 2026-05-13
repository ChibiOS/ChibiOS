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
 * @file    STM32G4xx_TEST/hal_lld.c
 * @brief   STM32G4xx generated clock tree prototype HAL source.
 *
 * @addtogroup HAL
 * @{
 */

#include "hal.h"

/*===========================================================================*/
/* Driver local definitions.                                                 */
/*===========================================================================*/

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

/**
 * @brief   CMSIS system core clock variable.
 */
uint32_t SystemCoreClock;

#if defined(HAL_LLD_USE_CLOCK_MANAGEMENT) || defined(__DOXYGEN__)
/**
 * @brief   Post-reset clock configuration.
 */
const halclkcfg_t hal_clkcfg_reset = {
  .pwr_cr1              = PWR_CR1_VOS_0,
  .pwr_cr2              = 0U,
  .pwr_cr5              = PWR_CR5_R1MODE,
  .rcc_cr               = RCC_CR_HSIKERON | RCC_CR_HSION,
  .rcc_cfgr             = 0U,
  .rcc_pllcfgr          = 0U,
  .flash_acr            = 0U,
  .rcc_crrcr            = 0U
};

/**
 * @brief   Default clock configuration.
 */
const halclkcfg_t hal_clkcfg_default = {
  .pwr_cr1              = STM32_VOS | PWR_CR1_DBP,
  .pwr_cr2              = STM32_PWR_CR2,
  .pwr_cr5              = 0U,
  .rcc_cr               = STM32_HSI16_BITS | STM32_HSE_BITS,
  .rcc_cfgr             = STM32_MCO_BITS | STM32_MCODIV_BITS |
                          STM32_PCLK2_BITS | STM32_PCLK1_BITS |
                          STM32_HCLK_BITS | STM32_SYSCLK_BITS,
  .rcc_pllcfgr          = STM32_PLLP_BITS | STM32_PLLQ_BITS |
                          STM32_PLLR_BITS | STM32_PLLVCO_BITS |
                          STM32_PLLREF_BITS | STM32_PLLIN_BITS,
  .flash_acr            = 0U,
  .rcc_crrcr            = STM32_HSI48_BITS
};
#endif

/*===========================================================================*/
/* Driver local variables and types.                                         */
/*===========================================================================*/

#if defined(HAL_LLD_USE_CLOCK_MANAGEMENT) || defined(__DOXYGEN__)
/**
 * @brief   Dynamic clock points for this device.
 */
static halfreq_t clock_points[CLK_ARRAY_SIZE] = {
  [CLK_HSI16]           = STM32_HSI16_FREQ,
  [CLK_HSI48]           = STM32_HSI48_FREQ,
  [CLK_HSE]             = STM32_HSE_FREQ,
  [CLK_PLLP]            = STM32_PLLP_FREQ,
  [CLK_PLLQ]            = STM32_PLLQ_FREQ,
  [CLK_PLLR]            = STM32_PLLR_FREQ,
  [CLK_SYSCLK]          = STM32_SYSCLK_FREQ,
  [CLK_HCLK]            = STM32_HCLK_FREQ,
  [CLK_PCLK1]           = STM32_PCLK1_FREQ,
  [CLK_PCLK2]           = STM32_PCLK2_FREQ,
  [CLK_PCLK1TIM]        = STM32_PCLK1TIM_FREQ,
  [CLK_PCLK2TIM]        = STM32_PCLK2TIM_FREQ,
  [CLK_MCO]             = STM32_MCO_FREQ
};
#endif

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

__STATIC_INLINE void hal_lld_set_coreclock(halfreq_t coreclock) {

  SystemCoreClock = (uint32_t)coreclock;
}

/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

/**
 * @brief   Low level HAL driver initialization.
 *
 * @notapi
 */
void hal_lld_init(void) {

  hal_lld_set_coreclock(STM32_HCLK);

  /* IRQ subsystem initialization.*/
  nvicInit();
  irqInit();
}

/**
 * @brief   STM32G4xx clocks and PLL initialization.
 *
 * @special
 */
void stm32_clock_init(void) {

  hal_lld_set_coreclock(STM32_HCLK);
}

#if defined(HAL_LLD_USE_CLOCK_MANAGEMENT) || defined(__DOXYGEN__)
/**
 * @brief   Switches to a different clock configuration.
 *
 * @param[in] ccp       pointer to clock a @p halclkcfg_t structure
 * @return              The clock switch result.
 *
 * @notapi
 */
bool hal_lld_clock_switch_mode(const halclkcfg_t *ccp) {

  (void)ccp;

  /* Prototype stub: dynamic switching is not implemented yet.*/
  return true;
}

/**
 * @brief   Returns the frequency of a clock point in Hz.
 *
 * @param[in] clkpt     clock point to be returned
 * @return              The clock point frequency in Hz or zero if the
 *                      frequency is unknown.
 *
 * @notapi
 */
halfreq_t hal_lld_get_clock_point(halclkpt_t clkpt) {

  osalDbgAssert(clkpt < CLK_ARRAY_SIZE, "invalid clock point");

  return clock_points[clkpt];
}
#endif

/** @} */
