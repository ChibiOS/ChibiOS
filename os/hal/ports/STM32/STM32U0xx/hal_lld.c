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
 * @brief   Number of thresholds in the wait states array.
 */
#define STM32_WS_THRESHOLDS             3

/**
 * @name    Registers reset values
 * @{
 */
#define STM32_PWR_CR1_RESET             0x00000208U
#define STM32_FLASH_ACR_RESET           0x00040600U
#define STM32_RCC_CR_RESET              0x00000083U
#define STM32_RCC_CFGR_RESET            0x00000000U
#define STM32_RCC_PLLCFGR_RESET         0x00001000U
#define STM32_RCC_CRRCR_RESET           0x00008800U
/** @} */

/* Reserved bit to be kept at 1, ST mysteries.*/
#define FLASH_ACR_RESVD10               (1U << 10)

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

/**
 * @brief   CMSIS system core clock variable.
 * @note    It is declared in system_stm32u3xx.h.
 */
uint32_t SystemCoreClock = STM32_HCLK;

/**
 * @brief   Post-reset clock configuration.
 */
const halclkcfg_t hal_clkcfg_reset = {
  .pwr_cr1              = STM32_PWR_CR1_RESET | PWR_CR1_DBP,
  .rcc_cr               = STM32_RCC_CR_RESET,
  .rcc_cfgr             = STM32_RCC_CFGR_RESET,
  .rcc_pllcfgr          = STM32_RCC_PLLCFGR_RESET,
  .rcc_crrcr            = STM32_RCC_CRRCR_RESET,
  .flash_acr            = STM32_FLASH_ACR_RESET
};

/**
 * @brief   Default clock configuration.
 * @note    This is the configuration defined in mcuconf.h.
 */
const halclkcfg_t hal_clkcfg_default = {
  .pwr_cr1              = STM32_PWR_CR1 | PWR_CR1_DBP,
  .rcc_cr               = 0U
#if STM32_ACTIVATE_PLL == TRUE
                        | RCC_CR_PLLON
#endif
#if defined(STM32_HSE_BYPASS)
                        | RCC_CR_HSEBYP
#endif
#if STM32_HSE_ENABLED == TRUE
                        | RCC_CR_HSEON
#endif
#if STM32_HSI16_ENABLED == TRUE
                        | RCC_CR_HSION | RCC_CR_HSIKERON
#endif
                        | STM32_MSIRANGE
                        | RCC_CR_MSIRGSEL
#if STM32_MSIPLL_ENABLED == TRUE
                        | RCC_CR_MSIPLLEN
#endif
#if STM32_MSI_ENABLED == TRUE
                        | RCC_CR_MSION
#endif
                          ,
  .rcc_cfgr             = STM32_MCO1PRE | STM32_MCO1SEL |
                          STM32_MCO2PRE | STM32_MCO2SEL |
                          STM32_PPRE    | STM32_HPRE    |
                          STM32_SW,
  .rcc_pllcfgr          = STM32_PLLR    | STM32_PLLREN  |
                          STM32_PLLQ    | STM32_PLLQEN  |
                          STM32_PLLP    | STM32_PLLPEN  |
                          STM32_PLLN    | STM32_PLLM    |
                          STM32_PLLSRC,
  .rcc_crrcr            = 0U
#if STM32_HSI48_ENABLED == TRUE
                        | RCC_CRRCR_HSI48ON
#endif
                        ,
  .flash_acr            = (STM32_FLASH_ACR & ~FLASH_ACR_LATENCY_Msk) |
                          STM32_FLASHBITS | FLASH_ACR_RESVD10
};

/*===========================================================================*/
/* Driver local variables and types.                                         */
/*===========================================================================*/

#if defined(HAL_LLD_USE_CLOCK_MANAGEMENT) || defined(__DOXYGEN__)
/**
 * @brief   Dynamic clock points for this device.
 * @note    This array is pre-initialized with the defaults value because
 *          clock_init() (called in early_init()) cannot initialize this
 *          at runtime, successive DATA/BSS segment initialization would
 *          overwrite it.
 */
static halfreq_t clock_points[CLK_ARRAY_SIZE] = {
  [CLK_SYSCLK]          = STM32_SYSCLK,
  [CLK_HSI16]           = STM32_HSI16CLK,
  [CLK_HSI48]           = STM32_HSI48CLK,
  [CLK_HSE]             = STM32_HSECLK,
  [CLK_LSI]             = STM32_LSICLK,
  [CLK_LSE]             = STM32_LSECLK,
  [CLK_MSI]             = STM32_MSICLK,
  [CLK_PLLPCLK]         = STM32_PLLPCLK,
  [CLK_PLLQCLK]         = STM32_PLLQCLK,
  [CLK_PLLRCLK]         = STM32_PLLRCLK,
  [CLK_HCLK]            = STM32_HCLK,
  [CLK_PCLK]            = STM32_PCLK,
  [CLK_PCLKTIM]         = STM32_TIMPCLK,
  [CLK_MCO1]            = STM32_MCO1CLK,
  [CLK_MCO2]            = STM32_MCO2CLK
};

/**
 * @brief   Type of a structure representing system limits.
 */
typedef struct {
  halfreq_t             sysclk_max;
  halfreq_t             flash_thresholds[STM32_WS_THRESHOLDS];
} system_limits_t;

/**
 * @brief   System limits for range 1.
 */
static const system_limits_t vos_range1 = {
  .sysclk_max           = STM32_RANGE1_SYSCLK_MAX,
  .flash_thresholds     = {STM32_RANGE1_0WS_THRESHOLD, STM32_RANGE1_1WS_THRESHOLD,
                           STM32_RANGE1_2WS_THRESHOLD}
};

/**
 * @brief   System limits for range 2.
 */
static const system_limits_t vos_range2 = {
  .sysclk_max           = STM32_RANGE2_SYSCLK_MAX,
  .flash_thresholds     = {STM32_RANGE2_0WS_THRESHOLD, STM32_RANGE2_1WS_THRESHOLD,
                           STM32_RANGE2_2WS_THRESHOLD}
};

/**
 * @brief   System limits for LPR.
 */
static const system_limits_t vos_range2 = {
  .sysclk_max           = STM32_LPR_SYSCLK_MAX,
  .flash_thresholds     = {STM32_LPR_0WS_THRESHOLD, STM32_LPR_1WS_THRESHOLD,
                           STM32_LPR_2WS_THRESHOLD}
};
#endif /* defined(HAL_LLD_USE_CLOCK_MANAGEMENT) */

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

#include "stm32_bd.inc"

/**
 * @brief   Configures the PWR unit.
 * @note    CR is not initialized inside this function except for DBP.
 */
static void hal_lld_set_static_pwr(void) {

  /* PWR clock enabled.*/
  rccEnablePWRInterface(false);

  /* Enable write access to Backup domain.*/
  PWR->CR1      = STM32_PWR_CR1_RESET | PWR_CR1_DBP;

  /* Static settings for PWR registers.*/
  PWR->CR2      = STM32_PWR_CR2;
  PWR->CR3      = STM32_PWR_CR3;
  PWR->CR4      = STM32_PWR_CR4;
  PWR->PUCRA    = STM32_PWR_PUCRA;
  PWR->PDCRA    = STM32_PWR_PDCRA;
  PWR->PDCRB    = STM32_PWR_PDCRB;
  PWR->PUCRB    = STM32_PWR_PUCRB;
  PWR->PUCRC    = STM32_PWR_PUCRC;
  PWR->PDCRC    = STM32_PWR_PDCRC;
  PWR->PUCRD    = STM32_PWR_PUCRD;
  PWR->PDCRD    = STM32_PWR_PDCRD;
  PWR->PUCRE    = STM32_PWR_PUCRE;
  PWR->PDCRE    = STM32_PWR_PDCRE;
  PWR->PUCRF    = STM32_PWR_PUCRF;
  PWR->PDCRF    = STM32_PWR_PDCRF;
}

/**
 * @brief   Initializes static muxes and dividers.
 */
static void hal_lld_set_static_clocks(void) {

  /* CCIPR registers initialization, note.*/
  RCC->CCIPR = STM32_ADCSEL     | STM32_CLK48SEL    |
               STM32_TIM15SEL   | STM32_TIM1SEL     |
               STM32_LPTIM3SEL  | STM32_LPTIM2SEL   |
               STM32_LPTIM1SEL  | STM32_I2C3SEL     |
               STM32_I2C1SEL    | STM32_LPUART1SEL  |
               STM32_LPUART2SEL | STM32_LPUART3SEL  |
               STM32_USART2SEL  | STM32_USART1SEL;
}

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
  halRegWrite32X(&FLASH->ACR,
                 FLASH_ACR_DBG_SWEN | FLASH_ACR_RESVD10 | FLASH_ACR_ICEN | FLASH_ACR_LATENCY_2WS,
                 true);

  /* Disabling low power run mode if activated, not touching current
     VOS range yet.*/
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
                       true);
  if (halRegWaitAllSet32X(&RCC->CR, RCC_CR_MSIRDY,
                          STM32_HSI_HSE_MSI_STARTUP_TIME,
                          NULL)) {
    return true;
  }

  /* Switching to MSI.*/
  halRegWrite32X(&RCC->CFGR, STM32_RCC_CFGR_RESET, true);
  if (halRegWaitMatch32X(&RCC->CFGR,
                         RCC_CFGR_SWS_Msk, RCC_CFGR_SWS_MSI,
                         STM32_SYSCLK_SWITCH_TIME,
                         NULL)) {
    return true;
  }

  /* Resetting clocks-related settings, this includes MSI.*/
  halRegWrite32X(&RCC->CR, STM32_RCC_CR_RESET, true);
  halRegWrite32X(&RCC->PLLCFGR, STM32_RCC_PLLCFGR_RESET, true);

  /* Post-reset voltage scaling enforcing.*/
  halRegWrite32X(&PWR->CR1, STM32_PWR_CR1_RESET, true);
  if (halRegWaitAllClear32X(&PWR->SR2, PWR_SR2_VOSF,
                            STM32_REGULATORS_TRANSITION_TIME, NULL)) {
    return true;
  }

  /* Enabling all required oscillators at same time, MSI enforced active,
     PLL not enabled yet.*/
  halRegWrite32X(&RCC->CR,
                  (ccp->rcc_cr | RCC_CR_MSION) & ~RCC_CR_PLLON,
                  true);

  /* Starting also HSI48 if required, waiting for it to become stable first
     because it is the fastest one.*/
  halRegWrite32X(&RCC->CRRCR, ccp->rcc_crrcr, true);
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

  /* Final programmable voltage scaling configuration. */
  halRegWrite32X(&PWR->CR1, ccp->pwr_cr1 | PWR_CR1_DBP, true);
  if (halRegWaitAllClear32X(&PWR->SR2, PWR_SR2_VOSF,
                            STM32_REGULATORS_TRANSITION_TIME, NULL)) {
    return true;
  }

  /* Enabling also PLLs if required by the configuration else skipping.*/
  if ((ccp->rcc_cr & RCC_CR_PLLON) != 0U) {
    halRegWrite32X(&RCC->PLLCFGR, ccp->rcc_pllcfgr, true);
    halRegWrite32X(&RCC->CR, ccp->rcc_cr | RCC_CR_MSION, true);
    if (halRegWaitAllSet32X(&RCC->CR,
                            RCC_CR_PLLRDY,
                            STM32_PLL_STARTUP_TIME,
                            NULL)) {
      return true;
    }
  }

  /* Final RCC CFGR settings (prescalers, MCO, etc).*/
  halRegWrite32X(&RCC->CFGR, ccp->rcc_cfgr, true);

  /* Final flash ACR settings according to the target configuration.*/
  halRegWrite32X(&FLASH->ACR, ccp->flash_acr | FLASH_ACR_RESVD10, true);

  /* Waiting for the requested SYSCLK source to become active. */
  if (halRegWaitMatch32X(&RCC->CFGR,
                         RCC_CFGR_SWS_Msk, (ccp->rcc_cfgr & RCC_CFGR_SW_Msk) << RCC_CFGR_SWS_Pos,
                         STM32_SYSCLK_SWITCH_TIME,
                         NULL)) {
    return true;
  }

  /* Final RCC_CR value, MSIS could go off at this point if it is not part
     of the mask.*/
  halRegWrite32X(&RCC->CR, ccp->rcc_cr, true);

  return false;
}

#if defined(HAL_LLD_USE_CLOCK_MANAGEMENT) || defined(__DOXYGEN__)
/**
 * @brief   Recalculates the clock tree frequencies.
 *
 * @param[in] ccp       pointer to clock a @p halclkcfg_t structure
 * @return              The frequency calculation result.
 * @retval false        if the clock settings look valid
 * @retval true         if the clock settings look invalid
 *
 * @notapi
 */
static bool hal_lld_clock_check_tree(const halclkcfg_t *ccp) {
  static const uint32_t hprediv[16] = {1U, 1U, 1U, 1U, 1U, 1U, 1U, 1U,
                                       2U, 4U, 8U, 16U, 64U, 128U, 256U, 512U};
  static const uint32_t pprediv[16] = {1U, 1U, 1U, 1U, 2U, 4U, 8U, 16U};
  const system_limits_t *slp;
  uint32_t n, flashws;
  halfreq_t hsi16clk = 0U, hsi48clk = 0U, hseclk = 0U;
  halfreq_t msirc0clk = 0U, msirc1clk = 0U, msisclk = 0U, msikclk = 0U;
  halfreq_t sysclk, hclk, pclk1, pclk2, pclk3, pclk1tim, pclk2tim, mco1clk, mco2clk;

  /* System limits based on desired VOS settings.*/
  switch (ccp->pwr_vosr & PWR_VOSR_RANGE_Msk) {
  case PWR_VOSR_RANGE1:
    slp = &vos_range1;
    break;
  case PWR_VOSR_RANGE2:
    slp = &vos_range2;
    break;
  default:
    return true;
  }

  /* HSE clock.*/
  if ((ccp->rcc_cr & RCC_CR_HSEON) != 0U) {
    hseclk = STM32_HSECLK;
  }

  /* HSI16 clock.*/
  if ((ccp->rcc_cr & RCC_CR_HSION) != 0U) {
    hsi16clk = STM32_HSI16CLK;
  }

  /* HSI48 clock.*/
  if ((ccp->rcc_cr & RCC_CR_HSI48ON) != 0U) {
    hsi48clk = STM32_HSI48CLK;
  }

  /* MSIRC0 base clock depending on MSIPLL0 mode. */
  if ((ccp->rcc_cr & RCC_CR_MSIPLL0EN) == 0U) {
    msirc0clk = 96000000U;
  }
  else {
    uint32_t sel0 = (ccp->rcc_icscr1 & RCC_ICSCR1_MSIPLL0SEL_Msk) >> RCC_ICSCR1_MSIPLL0SEL_Pos;

    switch (sel0) {
    case 0U: /* LSE reference */
      msirc0clk = 96010000U;
      break;
    case 1U: /* HSE reference */
      msirc0clk = 96000000U;
      break;
    default:
      return true;
    }
  }

  /* MSIRC1 base clock depending on MSIPLL1 mode. */
  if ((ccp->rcc_cr & RCC_CR_MSIPLL1EN) == 0U) {
    msirc1clk = 24000000U;
  }
  else {
    uint32_t sel1 = (ccp->rcc_icscr1 & RCC_ICSCR1_MSIPLL1SEL_Msk) >> RCC_ICSCR1_MSIPLL1SEL_Pos;
    uint32_t n1   = (ccp->rcc_icscr1 & RCC_ICSCR1_MSIPLL1N_Msk)   >> RCC_ICSCR1_MSIPLL1N_Pos;

    switch (n1) {
    case 0U:
      if (sel1 == 0U) {
        msirc1clk = 23986000U;
      }
      else if (sel1 == 1U) {
        msirc1clk = 24016000U;
      }
      else {
        return true;
      }
      break;
    case 2U:
      if (sel1 == 0U) {
        msirc1clk = 22577000U;
      }
      else if (sel1 == 1U) {
        msirc1clk = 22581000U;
      }
      else {
        return true;
      }
      break;
    case 3U:
      if (sel1 == 0U) {
        msirc1clk = 24576000U;
      }
      else if (sel1 == 1U) {
        msirc1clk = 24577000U;
      }
      else {
        return true;
      }
      break;
    default:
      return true;
    }
  }

  /* MSIS clock.*/
  if ((ccp->rcc_cr & RCC_CR_MSISON) != 0U) {
    halfreq_t msis_src;
    uint32_t msis_div;

    if ((ccp->rcc_icscr1 & RCC_ICSCR1_MSISSEL_Msk) == RCC_ICSCR1_MSISSEL_MSIRC0) {
      msis_src = msirc0clk;
    }
    else {
      msis_src = msirc1clk;
    }

    msis_div = (ccp->rcc_icscr1 & RCC_ICSCR1_MSISDIV_Msk) >> RCC_ICSCR1_MSISDIV_Pos;
    msisclk = msis_src >> msis_div;
  }

  /* MSIK clock.*/
  if ((ccp->rcc_cr & RCC_CR_MSIKON) != 0U) {
    halfreq_t msik_src;
    uint32_t msik_div;

    if ((ccp->rcc_icscr1 & RCC_ICSCR1_MSIKSEL_Msk) == RCC_ICSCR1_MSIKSEL_MSIRC0) {
      msik_src = msirc0clk;
    }
    else {
      msik_src = msirc1clk;
    }

    msik_div = (ccp->rcc_icscr1 & RCC_ICSCR1_MSIKDIV_Msk) >> RCC_ICSCR1_MSIKDIV_Pos;
    msikclk = msik_src >> msik_div;
  }

  /* SYSCLK frequency.*/
  switch (ccp->rcc_cfgr1 & RCC_CFGR1_SW_Msk) {
  case RCC_CFGR1_SW_MSIS:
    sysclk = msisclk;
    break;
  case RCC_CFGR1_SW_HSI16:
    sysclk = hsi16clk;
    break;
  case RCC_CFGR1_SW_HSE:
    sysclk = hseclk;
    break;
  default:
    sysclk = 0U;
  }

  /* Checking SYSCLK limit.*/
  if (sysclk > slp->sysclk_max) {
    return true;
  }

  /* HCLK frequency.*/
  hclk = sysclk / hprediv[(ccp->rcc_cfgr2 & STM32_HPRE_MASK) >> STM32_HPRE_POS];

  /* PPRE1 frequency.*/
  n = pprediv[(ccp->rcc_cfgr2 & STM32_PPRE1_MASK) >> STM32_PPRE1_POS];
  pclk1 = hclk / n;
  if (n < 2) {
    pclk1tim = pclk1;
  }
  else {
    pclk1tim = pclk1 * 2U;
  }

  /* PPRE2 frequency.*/
  n = pprediv[(ccp->rcc_cfgr2 & STM32_PPRE2_MASK) >> STM32_PPRE2_POS];
  pclk2 = hclk / n;
  if (n < 2) {
    pclk2tim = pclk2;
  }
  else {
    pclk2tim = pclk2 * 2U;
  }

  /* PPRE3 frequency.*/
  n = pprediv[(ccp->rcc_cfgr2 & STM32_PPRE3_MASK) >> STM32_PPRE3_POS];
  pclk3 = hclk / n;

  /* MCO1 clock.*/
  switch (ccp->rcc_cfgr1 & RCC_CFGR1_MCOSEL_Msk) {
  case RCC_CFGR1_MCO1SEL_SYSCLK:
    mco1clk = sysclk;
    break;
  case RCC_CFGR1_MCO1SEL_MSIS:
    mco1clk = msisclk;
    break;
  case RCC_CFGR1_MCO1SEL_HSI16:
    mco1clk = hsi16clk;
    break;
  case RCC_CFGR1_MCO1SEL_HSE:
    mco1clk = hseclk;
    break;
  case RCC_CFGR1_MCO1SEL_LSI:
    mco1clk = STM32_LSICLK;
    break;
  case RCC_CFGR1_MCO1SEL_LSE:
    mco1clk = STM32_LSECLK;
    break;
  case RCC_CFGR1_MCO1SEL_HSI48:
    mco1clk = STM32_HSI48CLK;
    break;
  case RCC_CFGR1_MCO1SEL_MSIK:
    mco1clk = msikclk;
    break;
  default:
    mco1clk = 0U;
  }
  n = (ccp->rcc_cfgr1 & RCC_CFGR1_MCOPRE_Msk) >> RCC_CFGR1_MCOPRE_Pos;
  mco1clk /= 1U << n;

  /* MCO2 clock.*/
  switch (ccp->rcc_cfgr1 & RCC_CFGR1_MCO2SEL_Msk) {
  case RCC_CFGR1_MCO2SEL_SYSCLK:
    mco2clk = sysclk;
    break;
  case RCC_CFGR1_MCO2SEL_MSIS:
    mco2clk = msisclk;
    break;
  case RCC_CFGR1_MCO2SEL_HSI16:
    mco2clk = hsi16clk;
    break;
  case RCC_CFGR1_MCO2SEL_HSE:
    mco2clk = hseclk;
    break;
  case RCC_CFGR1_MCO2SEL_LSI:
    mco2clk = STM32_LSICLK;
    break;
  case RCC_CFGR1_MCO2SEL_LSE:
    mco2clk = STM32_LSECLK;
    break;
  case RCC_CFGR1_MCO2SEL_HSI48:
    mco2clk = STM32_HSI48CLK;
    break;
  case RCC_CFGR1_MCO2SEL_MSIK:
    mco2clk = msikclk;
    break;
  default:
    mco2clk = 0U;
  }
  n = (ccp->rcc_cfgr1 & RCC_CFGR1_MCO2PRE_Msk) >> RCC_CFGR1_MCO2PRE_Pos;
  mco2clk /= 1U << n;

  /* Flash settings.*/
  flashws = ((ccp->flash_acr & FLASH_ACR_LATENCY_Msk) >> FLASH_ACR_LATENCY_Pos);
  if (flashws >= STM32_WS_THRESHOLDS) {
    return true;
  }
  if (hclk > slp->flash_thresholds[flashws]) {
    return true;
  }

  /* Writing out results.*/
  clock_points[CLK_HSI16]    = hsi16clk;
  clock_points[CLK_HSI48]    = hsi48clk;
  clock_points[CLK_HSE]      = hseclk;
  clock_points[CLK_MSIS]     = msisclk;
  clock_points[CLK_MSIK]     = msikclk;
  clock_points[CLK_SYSCLK]   = sysclk;
  clock_points[CLK_HCLK]     = hclk;
  clock_points[CLK_PCLK1]    = pclk1;
  clock_points[CLK_PCLK1TIM] = pclk1tim;
  clock_points[CLK_PCLK2]    = pclk2;
  clock_points[CLK_PCLK2TIM] = pclk2tim;
  clock_points[CLK_PCLK3]    = pclk3;
  clock_points[CLK_MCO1]     = mco1clk;
  clock_points[CLK_MCO2]     = mco2clk;

  return false;
}
#endif /* defined(HAL_LLD_USE_CLOCK_MANAGEMENT) */

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

  /* DWT cycles counter enabled, used for timeouts.*/
//  DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;

#if !STM32_NO_INIT
  /* Reset of all peripherals.
     Note, GPIOs are not reset because initialized before this point in
     board files.*/
  rccResetAHB(~0);
  rccResetAPBR1(~0);
  rccResetAPBR2(~0);

  /* RTC APB clock enable.*/
#if (HAL_USE_RTC == TRUE) && defined(RCC_APB3ENR_RTCAPBEN)
  rccEnableAPB3(RCC_APB3ENR_RTCAPBEN, true);
#endif

  /* Static PWR configurations.*/
  hal_lld_set_static_pwr();

  /* Backup domain reset if required.*/
  bd_reset();

  /* Static oscillators setup.*/
  lse_init();
  lsi_init();

  /* Static clocks setup (dividers, CCIPR selections).*/
  hal_lld_set_static_clocks();

  /* Selecting the default clock configuration. */
  halSftFailOnError(hal_lld_clock_configure(&hal_clkcfg_default), "clkinit");

  /* Backup domain initializations.*/
  bd_init();
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
