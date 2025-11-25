/*
    ChibiOS - Copyright (C) 2006..2025 Giovanni Di Sirio

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
 * @file    STM32U0xx/hal_lld.c
 * @brief   STM32U0xx HAL subsystem low level driver source.
 *
 * @addtogroup HAL
 * @{
 */

#include "hal.h"

/*===========================================================================*/
/* Driver local definitions.                                                 */
/*===========================================================================*/

/**
 * @name    Registers reset values
 * @{
 */
#define STM32_PWR_CR1_RESET             0x00000208U
#define STM32_FLASH_ACR_RESET           0x00040600U
#define STM32_RCC_CR_RESET              0x00000083U
#define STM32_RCC_CFGR_RESET            0x00000000U
#define STM32_RCC_PLLCFGR_RESET         0x00001000U
/** @} */

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local variables and types.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

#include "stm32_bd.inc"

/**
 * @brief   Switches to a different clock configuration.
 *
 * @param[in] ccp       pointer to clock a @p halclkcfg_t structure
 * @return              The clock switch result.
 * @retval false        if the clock switch succeeded
 * @retval true         if the clock switch failed
 *
 * @notapi
 */
static bool hal_lld_clock_configure(const halclkcfg_t *ccp) {
  uint32_t wtmask;

  /* Setting flash ACR to the safest value while the clock tree is
     reconfigured. we don't know the current clock settings.*/
  if (halRegWrite32X(&FLASH->ACR, FLASH_ACR_LATENCY_2WS, true)) {
    return true;
  }

  /* Disabling low power run mode if activated, not touching current
     VOS range.*/
  halRegClear32X(&PWR->CR1, PWR_CR1_LPR, false);
  if (halRegWaitAllClear32X(&PWR->SR2, PWR_SR2_REGLPF,
                            STM32_REGULATORS_TRANSITION_TIME, NULL)) {
    return true;
  }

  /* Resetting and restarting MSI without touching the other settings, it
     is required during the (re)configuration.*/
  halRegMaskedWrite32X(&RCC->CR,
                       RCC_CR_MSIRANGE_Msk,
                       RCC_CR_MSIRANGE_4M | RCC_CR_MSION,
                       false);
  if (halRegWaitAllSet32X(&RCC->CR, RCC_CR_MSIRDY,
                          STM32_HSI_HSE_MSI_STARTUP_TIME,
                          NULL)) {
    return true;
  }

  /* Switching to MSI.*/
  RCC->CFGR     = STM32_RCC_CFGR_RESET;
  if (halRegWaitMatch32X(&RCC->CFGR,
                         RCC_CFGR_SWS_Msk, RCC_CFGR_SWS_MSI,
                         STM32_SYSCLK_SWITCH_TIME,
                         NULL)) {
    return true;
  }

  /* Resetting clocks-related settings, this includes MSI.*/
  RCC->CR       = STM32_RCC_CR_RESET;
  RCC->PLLCFGR  = STM32_RCC_PLLCFGR_RESET;

  /* Final power configuration.*/
  PWR->CR1      = ccp->pwr_cr1;
  if (halRegWaitAllSet32X(&PWR->SR2, PWR_SR2_VOSF,
                          STM32_REGULATORS_TRANSITION_TIME, NULL)) {
    return true;
  }

  /* Enabling all required oscillators at same time, MSI enforced active,
     PLL/MSIPLL not enabled yet.*/
  RCC->CR    = (ccp->rcc_cr | RCC_CR_MSION) & ~(RCC_CR_PLLON | RCC_CR_MSIPLLEN);

  /* Starting also HSI48 if required, waiting for it to become stable first
     because it is the faster one.*/
  RCC->CRRCR = ccp->rcc_crrcr;
  if ((ccp->rcc_crrcr & RCC_CRRCR_HSI48ON) != 0U) {
    if (halRegWaitAllSet32X(&RCC->CRRCR,
                            RCC_CRRCR_HSI48RDY,
                            STM32_HSI48_STARTUP_TIME,
                            NULL)) {
      return true;
    }
  }

  /* Adding to the "wait mask" the status bits of other enabled oscillators.*/
  wtmask = RCC_CR_MSIRDY;                  /* Known to be ready already.*/
  if ((ccp->rcc_cr & RCC_CR_HSEON) != 0U) {
    wtmask |= RCC_CR_HSERDY;
  }
  if ((ccp->rcc_cr & RCC_CR_HSION) != 0U) {
    wtmask |= RCC_CR_HSIRDY;
  }
  if (halRegWaitAllSet32X(&RCC->CR,
                          wtmask,
                          STM32_HSI_HSE_MSI_STARTUP_TIME,
                          NULL)) {
    return true;
  }

  /* Programmable voltage scaling configuration, this is done at the end
     because the booster clock must be ready (see RCC_CFGR4) before enabling
     the booster. */
  RCC->CFGR4 = ccp->rcc_cfgr4;
  PWR->VOSR  = ccp->pwr_vosr;
  wtmask = ccp->pwr_vosr << 16;
  if (halRegWaitAllSet32X(&PWR->VOSR,
                          wtmask,
                          STM32_OSCILLATORS_STARTUP_TIME,
                          NULL)) {
    return true;
  }

  /* MSI configuration (sources, dividers, bias). */
  RCC->ICSCR1 = ccp->rcc_icscr1 | RCC_ICSCR1_MSIRGSEL_ICSCR1;

  /* Enabling also PLLs if required by the configuration.*/
  RCC->CR = ccp->rcc_cr | RCC_CR_MSISON;
  wtmask = 0U;
  if ((ccp->rcc_cr & RCC_CR_MSIPLL0EN) != 0U) {
    wtmask |= RCC_CR_MSIPLL0RDY;
  }
  if ((ccp->rcc_cr & RCC_CR_MSIPLL1EN) != 0U) {
    wtmask |= RCC_CR_MSIPLL1RDY;
  }
  if (halRegWaitAllSet32X(&RCC->CR,
                          wtmask,
                          STM32_MSIPLL_STARTUP_TIME,
                          NULL)) {
    return true;
  }

  /* Final RCC CFGR settings (prescalers, MCO, STOP wake-up sources, booster).*/
  RCC->CFGR1 = ccp->rcc_cfgr1;
  RCC->CFGR2 = ccp->rcc_cfgr2;
  RCC->CFGR3 = ccp->rcc_cfgr3;

  /* Final flash ACR settings according to the target configuration.*/
  if (halRegWrite32X(&FLASH->ACR, ccp->flash_acr, true)) {
    return true;
  }

  /* Waiting for the requested SYSCLK source to become active. */
  if (halRegWaitMatch32X(&RCC->CFGR1,
                         RCC_CFGR1_SWS_Msk, (ccp->rcc_cfgr1 & RCC_CFGR1_SW_Msk) << RCC_CFGR1_SWS_Pos,
                         STM32_SYSCLK_SWITCH_TIME,
                         NULL)) {
    return true;
  }

  /* Final RCC_CR value, MSIS could go off at this point if it is not part
     of the mask.*/
  RCC->CR = ccp->rcc_cr;

  return false;
}

/*===========================================================================*/
/* Driver interrupt handlers.                                                */
/*===========================================================================*/

/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

/**
 * @brief   Low level HAL driver initialization.
 *
 * @notapi
 */
void hal_lld_init(void) {

  /* DMA subsystems initialization.*/
#if defined(STM32_DMA_REQUIRED)
  dmaInit();
#endif

  /* NVIC initialization.*/
  nvicInit();

  /* IRQ subsystem initialization.*/
  irqInit();
}

/**
 * @brief   STM32U0xx clocks and PLL initialization.
 * @note    All the involved constants come from the file @p board.h.
 * @note    This function should be invoked just after the system reset.
 *
 * @special
 */
void stm32_clock_init(void) {

#if !STM32_NO_INIT
#endif /* STM32_NO_INIT */
}

#if defined(HAL_LLD_USE_CLOCK_MANAGEMENT) || defined(__DOXYGEN__)
/**
 * @brief   Switches to a different clock configuration
 *
 * @param[in] ccp       pointer to clock a @p halclkcfg_t structure
 * @return              The clock switch result.
 * @retval false        if the clock switch succeeded
 * @retval true         if the clock switch failed
 *
 * @notapi
 */
bool hal_lld_clock_switch_mode(const halclkcfg_t *ccp) {

  if (hal_lld_clock_check_tree(ccp)) {
    return true;
  }

  if (hal_lld_clock_raw_config(ccp)) {
    return true;
  }

  /* Updating the CMSIS variable.*/
  SystemCoreClock = hal_lld_get_clock_point(CLK_HCLK);

  return false;
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
#endif /* defined(HAL_LLD_USE_CLOCK_MANAGEMENT) */

/** @} */
