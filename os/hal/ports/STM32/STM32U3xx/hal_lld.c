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
 * @file    STM32U3xx/hal_lld.c
 * @brief   STM32U3xx HAL subsystem low level driver source.
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
#define STM32_WS_THRESHOLDS             5

/**
 * @name    Registers reset values
 * @{
 */
#define STM32_PWR_VOSR_RESET            0x00020002U
#define STM32_FLASH_ACR_RESET           0x00000001U
#define STM32_RCC_CR_RESET              0x0000001DU
#define STM32_RCC_ICSCR1_RESET          0U
#define STM32_RCC_CFGR1_RESET           0U
#define STM32_RCC_CFGR2_RESET           0U
#define STM32_RCC_CFGR3_RESET           0U
#define STM32_RCC_CFGR4_RESET           0U
/** @} */

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

/**
 * @brief   CMSIS system core clock variable.
 * @note    It is declared in system_stm32u3xx.h.
 */
uint32_t SystemCoreClock = STM32_HCLK;

#if defined(HAL_LLD_USE_CLOCK_MANAGEMENT) || defined(__DOXYGEN__)
/**
 * @brief   Post-reset clock configuration.
 */
const halclkcfg_t hal_clkcfg_reset = {
  .pwr_vosr             = STM32_PWR_VOSR_RESET,
  .rcc_cr               = STM32_RCC_CR_RESET,
  .rcc_icscr1           = STM32_RCC_ICSCR1_RESET,
  .rcc_cfgr1            = STM32_RCC_CFGR1_RESET,
  .rcc_cfgr2            = STM32_RCC_CFGR2_RESET,
  .rcc_cfgr3            = STM32_RCC_CFGR3_RESET,
  .rcc_cfgr4            = STM32_RCC_CFGR4_RESET,
  .flash_acr            = STM32_FLASH_ACR_RESET
};

/**
 * @brief   Default clock configuration.
 */
const halclkcfg_t hal_clkcfg_default = {
  .pwr_vosr             = STM32_PWR_VOSR,
  .rcc_cr               = 0U
#if STM32_HSE_ENABLED
                        | RCC_CR_HSEON
#endif
#if defined(STM32_HSE_BYPASS)
                        | RCC_CR_HSEBYP
#endif
#if STM32_HSI48_ENABLED
                        | RCC_CR_HSI48ON
#endif
#if STM32_HSI16_ENABLED
                        | RCC_CR_HSIKERON | RCC_CR_HSION
#endif
                          ,
  .rcc_icscr1           = STM32_MSISSEL     | STM32_MSISDIV     |
                          STM32_MSIKSEL     | STM32_MSIKDIV     |
                          STM32_MSIPLL1N    | STM32_MSIBIAS     |
                          STM32_MSIPLL0SEL  | STM32_MSIPLL1SEL  |
                          STM32_MSIHSINDIV  |
                          STM32_MSIRGSEL_ICSCR1, /* Note, enforced.*/
  .rcc_cfgr1            = STM32_MCO2PRE     | STM32_MCO2SEL     |
                          STM32_MCO1PRE     | STM32_MCO1SEL     |
                          STM32_STOPKERWUCK | STM32_STOPWUCK,
  .rcc_cfgr2            = STM32_PPRE2       | STM32_PPRE1       |
                          STM32_HPRE,
  .rcc_cfgr3            = STM32_PPRE3,
  .rcc_cfgr4            = STM32_BOOSTDIV    | STM32_BOOSTSEL,
  .flash_acr            = (STM32_FLASH_ACR & ~STM32_ACR_LATENCY_MASK) |
                          STM32_FLASHBITS,
};
#endif /* defined(HAL_LLD_USE_CLOCK_MANAGEMENT) */

/*===========================================================================*/
/* Driver local variables and types.                                         */
/*===========================================================================*/

#if defined(HAL_LLD_USE_CLOCK_MANAGEMENT) || defined(__DOXYGEN__)
/**
 * @brief   Dynamic clock points for this device.
 */
static halfreq_t clock_points[CLK_ARRAY_SIZE] = {
#if STM32_HSI16_ENABLED
  [CLK_HSI16]           = STM32_HSI16CLK,
#else
  [CLK_HSI16]           = 0U,
#endif
#if STM32_HSI48_ENABLED
  [CLK_HSI48]           = STM32_HSI48CLK,
#else
  [CLK_HSI48]           = 0U,
#endif
#if STM32_HSE_ENABLED
  [CLK_HSE]             = STM32_HSECLK,
#else
  [CLK_HSE]             = 0U,
#endif
#if STM32_ACTIVATE_MSIS
  [CLK_MSIS]            = STM32_MSISCLK,
#else
  [CLK_MSIS]            = 0U,
#endif
#if STM32_ACTIVATE_MSIK
  [CLK_MSIK]            = STM32_MSIKCLK,
#else
  [CLK_MSIK]            = 0U,
#endif
  [CLK_HCLK]            = STM32_HCLK,
  [CLK_PCLK1]           = STM32_PCLK1,
  [CLK_PCLK1TIM]        = STM32_TIMP1CLK,
  [CLK_PCLK2]           = STM32_PCLK2,
  [CLK_PCLK2TIM]        = STM32_TIMP2CLK,
  [CLK_PCLK3]           = STM32_PCLK3,
  [CLK_MCO1]            = STM32_MCO2CLK,
  [CLK_MCO2]            = STM32_MCO1CLK
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
                           STM32_RANGE1_2WS_THRESHOLD, STM32_RANGE1_3WS_THRESHOLD,
                           STM32_RANGE1_4WS_THRESHOLD}
};

/**
 * @brief   System limits for range 2.
 */
static const system_limits_t vos_range2 = {
  .sysclk_max           = STM32_RANGE2_SYSCLK_MAX,
  .flash_thresholds     = {STM32_RANGE2_0WS_THRESHOLD, STM32_RANGE2_1WS_THRESHOLD,
                           STM32_RANGE2_2WS_THRESHOLD, STM32_RANGE2_3WS_THRESHOLD,
                           STM32_RANGE2_4WS_THRESHOLD}
};
#endif /* defined(HAL_LLD_USE_CLOCK_MANAGEMENT) */

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

#include "stm32_bd.inc"

/**
 * @brief   Safe setting of flash ACR register.
 *
 * @param[in] acr       value for the ACR register
 */
__STATIC_INLINE void flash_set_acr(uint32_t acr) {

  FLASH->ACR = acr;
  while ((FLASH->ACR & FLASH_ACR_LATENCY_Msk) != (acr & FLASH_ACR_LATENCY_Msk)) {
    /* Waiting for flash wait states setup.*/
  }
}

/**
 * @brief   Configures the PWR unit.
 * @note    CR1, CR2, VOSR are not initialized inside this function.
 */
__STATIC_INLINE void hal_lld_set_static_pwr(void) {

  /* PWR clock enabled.*/
  rccEnablePWRInterface(false);

  /* Enable write access to Backup domain.*/
  PWR->DBPR     = PWR_DBPR_DBP;
  PWR->BDCR     = STM32_PWR_BDCR;

  /* Static settings for PWR registers.*/
  PWR->CR3      = STM32_PWR_CR3;
  PWR->SVMCR    = STM32_PWR_SVMCR;
  PWR->WUCR1    = STM32_PWR_WUCR1;
  PWR->WUCR2    = STM32_PWR_WUCR2;
  PWR->WUCR3    = STM32_PWR_WUCR3;
  PWR->PUCRA    = STM32_PWR_PUCRA;
  PWR->PDCRA    = STM32_PWR_PDCRA;
  PWR->PDCRB    = STM32_PWR_PDCRB;
  PWR->PUCRB    = STM32_PWR_PUCRB;
  PWR->PUCRC    = STM32_PWR_PUCRC;
  PWR->PUCRD    = STM32_PWR_PUCRD;
  PWR->PUCRE    = STM32_PWR_PUCRE;
  PWR->PDCRC    = STM32_PWR_PDCRC;
  PWR->PDCRD    = STM32_PWR_PDCRD;
  PWR->PDCRE    = STM32_PWR_PDCRE;
  PWR->PUCRG    = STM32_PWR_PUCRG;
  PWR->PDCRG    = STM32_PWR_PDCRG;
  PWR->PUCRH    = STM32_PWR_PUCRH;
  PWR->PDCRH    = STM32_PWR_PDCRH;
  PWR->APCR     = STM32_PWR_APCR;
  PWR->I3CPUCR1 = STM32_PWR_I3CPUCR1;
  PWR->I3CPUCR2 = STM32_PWR_I3CPUCR2;

  /* Security/privilege settings for PWR.*/
  PWR->SECCFGR  = STM32_PWR_SECCFGR;
  PWR->PRIVCFGR = STM32_PWR_PRIVCFGR;
}

/**
 * @brief   Initializes static muxes and dividers.
 */
__STATIC_INLINE void hal_lld_set_static_clocks(void) {

  /* Clock-related settings (dividers, MCO etc).*/
  RCC->CFGR1  = STM32_MCO2PRE     | STM32_MCO2SEL     |
                STM32_MCO1PRE     | STM32_MCO1SEL     |
                STM32_STOPKERWUCK | STM32_STOPWUCK;
  RCC->CFGR2  = STM32_PPRE2       | STM32_PPRE1       |
                STM32_HPRE;
  RCC->CFGR3  = STM32_PPRE3;
  RCC->CFGR4  = STM32_BOOSTDIV    | STM32_BOOSTSEL;

  /* CCIPR registers initialization, note.*/
  RCC->CCIPR1 = STM32_TIMICSEL    | STM32_USB1SEL     |
                STM32_ICLKSEL     |
#if STM32_FDCAN1SEL != STM32_FDCAN1SEL_IGNORE
                STM32_FDCAN1SEL   |
#endif
                STM32_SYSTICKSEL  | STM32_SPI1SEL     |
                STM32_LPTIM2SEL   | STM32_SPI2SEL     |
                STM32_I3C2SEL     | STM32_I2C2SEL     |
                STM32_I2C1SEL     | STM32_I3C1SEL     |
                STM32_UART5SEL    | STM32_UART4SEL    |
                STM32_USART3SEL   | STM32_USART1SEL;
  RCC->CCIPR2 = STM32_OCTOSPISEL  |
#if STM32_DAC1SHSEL != STM32_DAC1SHSEL_IGNORE
                STM32_DAC1SHSEL   |
#endif
                STM32_ADCDACSEL   | STM32_ADCDACPRE   |
#if STM32_RNGSEL != STM32_RNGSEL_IGNORE
                STM32_RNGSEL      |
#endif
                STM32_SAI1SEL     | STM32_SPI3SEL     |
                STM32_ADF1SEL;
  RCC->CCIPR3 = STM32_LPTIM1SEL   | STM32_LPTIM34SEL  |
                STM32_I2C3SEL     | STM32_LPUART1SEL;
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
  switch (ccp->pwr_vosr & STM32_VOS_MASK) {
  case STM32_VOS_RANGE1:
    slp = &vos_range1;
    break;
  case STM32_VOS_RANGE2:
    slp = &vos_range2;
    break;
  default:
    return true;
  }

  /* HSE clock.*/
  if ((ccp->rcc_cr & STM32_CR_HSEON) != 0U) {
    hseclk = STM32_HSECLK;
  }

  /* HSI16 clock.*/
  if ((ccp->rcc_cr & STM32_CR_HSION) != 0U) {
    hsi16clk = STM32_HSI16CLK;
  }

  /* HSI48 clock.*/
  if ((ccp->rcc_cr & STM32_CR_HSI48ON) != 0U) {
    hsi48clk = STM32_HSI48CLK;
  }

  /* MSIRC0 base clock depending on MSIPLL0 mode. */
  if ((ccp->rcc_cr & STM32_CR_MSIPLL0EN) == 0U) {
    msirc0clk = 96000000U;
  }
  else {
    uint32_t sel0 = (ccp->rcc_icscr1 & STM32_MSIPLL0SEL_MASK) >> STM32_MSIPLL0SEL_POS;

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
  if ((ccp->rcc_cr & STM32_CR_MSIPLL1EN) == 0U) {
    msirc1clk = 24000000U;
  }
  else {
    uint32_t sel1 = (ccp->rcc_icscr1 & STM32_MSIPLL1SEL_MASK) >> STM32_MSIPLL1SEL_POS;
    uint32_t n1   = (ccp->rcc_icscr1 & STM32_MSIPLL1N_MASK)   >> STM32_MSIPLL1N_POS;

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
  if ((ccp->rcc_cr & STM32_CR_MSISON) != 0U) {
    halfreq_t msis_src;
    uint32_t msis_div;

    if ((ccp->rcc_icscr1 & STM32_MSISSEL_MASK) == STM32_MSISSEL_MSIRC0) {
      msis_src = msirc0clk;
    }
    else {
      msis_src = msirc1clk;
    }

    msis_div = (ccp->rcc_icscr1 & STM32_MSISDIV_MASK) >> STM32_MSISDIV_POS;
    msisclk = msis_src >> msis_div;
  }

  /* MSIK clock.*/
  if ((ccp->rcc_cr & STM32_CR_MSIKON) != 0U) {
    halfreq_t msik_src;
    uint32_t msik_div;

    if ((ccp->rcc_icscr1 & STM32_MSIKSEL_MASK) == STM32_MSIKSEL_MSIRC0) {
      msik_src = msirc0clk;
    }
    else {
      msik_src = msirc1clk;
    }

    msik_div = (ccp->rcc_icscr1 & STM32_MSIKDIV_MASK) >> STM32_MSIKDIV_POS;
    msikclk = msik_src >> msik_div;
  }

  /* SYSCLK frequency.*/
  switch (ccp->rcc_cfgr1 & STM32_SW_MASK) {
  case STM32_SW_MSIS:
    sysclk = msisclk;
    break;
  case STM32_SW_HSI16:
    sysclk = hsi16clk;
    break;
  case STM32_SW_HSE:
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
  switch (ccp->rcc_cfgr1 & STM32_MCO1SEL_MASK) {
  case STM32_MCO1SEL_SYSCLK:
    mco1clk = sysclk;
    break;
  case STM32_MCO1SEL_MSIS:
    mco1clk = msisclk;
    break;
  case STM32_MCO1SEL_HSI16:
    mco1clk = hsi16clk;
    break;
  case STM32_MCO1SEL_HSE:
    mco1clk = hseclk;
    break;
  case STM32_MCO1SEL_LSI:
    mco1clk = STM32_LSICLK;
    break;
  case STM32_MCO1SEL_LSE:
    mco1clk = STM32_LSECLK;
    break;
  case STM32_MCO1SEL_HSI48:
    mco1clk = STM32_HSI48CLK;
    break;
  case STM32_MCO1SEL_MSIK:
    mco1clk = msikclk;
    break;
  default:
    mco1clk = 0U;
  }
  n = (ccp->rcc_cfgr1 & STM32_MCO1PRE_MASK) >> STM32_MCO1PRE_POS;
  mco1clk /= 1U << n;

  /* MCO2 clock.*/
  switch (ccp->rcc_cfgr1 & STM32_MCO2SEL_MASK) {
  case STM32_MCO2SEL_SYSCLK:
    mco2clk = sysclk;
    break;
  case STM32_MCO2SEL_MSIS:
    mco2clk = msisclk;
    break;
  case STM32_MCO2SEL_HSI16:
    mco2clk = hsi16clk;
    break;
  case STM32_MCO2SEL_HSE:
    mco2clk = hseclk;
    break;
  case STM32_MCO2SEL_LSI:
    mco2clk = STM32_LSICLK;
    break;
  case STM32_MCO2SEL_LSE:
    mco2clk = STM32_LSECLK;
    break;
  case STM32_MCO2SEL_HSI48:
    mco2clk = STM32_HSI48CLK;
    break;
  case STM32_MCO2SEL_MSIK:
    mco2clk = msikclk;
    break;
  default:
    mco2clk = 0U;
  }
  n = (ccp->rcc_cfgr1 & STM32_MCO2PRE_MASK) >> STM32_MCO2PRE_POS;
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
static bool hal_lld_clock_raw_switch(const halclkcfg_t *ccp) {
  uint32_t cr, wtmask;

  /* Setting flash ACR to the safest value while the clock tree is reconfigured.*/
  flash_set_acr(STM32_LATENCY_4WS);

  /* MSIS must be active while performing the reconfiguration.*/
  RCC->CR = STM32_RCC_CR_RESET;
  while ((RCC->CR & RCC_CR_MSISRDY) == 0U) {
    /* Waiting for MSIS activation.*/
  }

  /* Resetting prescalers/MCO settings (MSIS is the SYSCLK source).*/
  RCC->CFGR1 = STM32_RCC_CFGR1_RESET;
  RCC->CFGR2 = STM32_RCC_CFGR2_RESET;
  RCC->CFGR3 = STM32_RCC_CFGR3_RESET;
  RCC->CFGR4 = STM32_RCC_CFGR4_RESET;
  while ((RCC->CFGR1 & STM32_SWS_MASK) != STM32_SWS_MSIS) {
    /* Wait until MSIS is selected as SYSCLK source.*/
  }

  /* Programmable voltage scaling configuration. */
  PWR->VOSR = ccp->pwr_vosr;
  wtmask = ccp->pwr_vosr << 16;
  while ((PWR->VOSR & wtmask) != wtmask) {
    /* Waiting for regulators and booster to be ready.*/
  }

  /* MSI configuration (sources, dividers, bias). */
  RCC->ICSCR1 = (ccp->rcc_icscr1 | STM32_MSIRGSEL_ICSCR1);

  /* Enabling required oscillators and MSIPLLs, HSI16 kept active. */
  cr = ccp->rcc_cr | RCC_CR_HSION;
  RCC->CR = cr;

  wtmask = 0U;
  if ((ccp->rcc_cr & RCC_CR_HSEON) != 0U) {
    wtmask |= RCC_CR_HSERDY;
  }
  if ((ccp->rcc_cr & RCC_CR_HSI48ON) != 0U) {
    wtmask |= RCC_CR_HSI48RDY;
  }
  if ((ccp->rcc_cr & RCC_CR_MSISON) != 0U) {
    wtmask |= RCC_CR_MSISRDY;
  }
  if ((ccp->rcc_cr & RCC_CR_MSIKON) != 0U) {
    wtmask |= RCC_CR_MSIKRDY;
  }
  if ((ccp->rcc_cr & RCC_CR_MSIPLL0EN) != 0U) {
    wtmask |= RCC_CR_MSIPLL0RDY;
  }
  if ((ccp->rcc_cr & RCC_CR_MSIPLL1EN) != 0U) {
    wtmask |= RCC_CR_MSIPLL1RDY;
  }
  while ((RCC->CR & wtmask) != wtmask) {
    /* Waiting for stabilization.*/
  }

  /* Final flash ACR settings according to the target configuration.*/
  flash_set_acr(ccp->flash_acr);

  /* Final RCC CFGR settings (prescalers, MCO, STOP wake-up sources, booster).*/
  RCC->CFGR1 = ccp->rcc_cfgr1;
  RCC->CFGR2 = ccp->rcc_cfgr2;
  RCC->CFGR3 = ccp->rcc_cfgr3;
  RCC->CFGR4 = ccp->rcc_cfgr4;

  /* Waiting for the requested SYSCLK source to become active. */
  while ((RCC->CFGR1 & STM32_SWS_MASK) != ((ccp->rcc_cfgr1 & STM32_SW_MASK) << STM32_SWS_POS)) {
    /* Waiting for SYSCLK switch.*/
  }

  /* If MSIS is not required in the final configuration then it is shut down. */
  if ((ccp->rcc_cr & RCC_CR_MSISON) == 0U) {
    RCC->CR &= ~RCC_CR_MSISON;
    while ((RCC->CR & RCC_CR_MSISRDY) != 0U) {
      /* Waiting for MSIS to stop.*/
    }
  }

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
#if defined(STM32_GPDMA_REQUIRED)
  gpdmaInit();
#endif

  /* NVIC initialization.*/
  nvicInit();

  /* IRQ subsystem initialization.*/
  irqInit();
}

#if defined(HAL_LLD_USE_CLOCK_MANAGEMENT) || defined(__DOXYGEN__)
/**
 * @brief   STM32L4xx clocks and PLL initialization.
 * @note    All the involved constants come from the file @p board.h.
 * @note    This function should be invoked just after the system reset.
 *
 * @special
 */
void stm32_clock_init(void) {

#if !STM32_NO_INIT

  /* Reset of all peripherals.
     Note, GPIOs are not reset because initialized before this point in
     board files.*/
  rccResetAHB1R1(~0);
  rccResetAHB1R2(~0);
  rccResetAHB2R1(~STM32_GPIO_EN_MASK);
  rccResetAHB2R2(~0);
  rccResetAPB1R1(~0);
  rccResetAPB1R2(~0);
  rccResetAPB2(~0);
  rccResetAPB3(~0);

  /* RTC APB clock enable.*/
#if (HAL_USE_RTC == TRUE) && defined(RCC_APB3ENR_RTCAPBEN)
  rccEnableAPB3(RCC_APB3ENR_RTCAPBEN, true);
#endif

  /* Static PWR configurations.*/
  hal_lld_set_static_pwr();

  /* Backup domain reset.*/
  bd_reset();

  /* Static oscillators setup.*/
  lse_init();
  lsi_init();
  hsi16_init();
  hsi48_init();
  hse_init();

  /* Static clocks setup (dividers, CCIPR selections).*/
  hal_lld_set_static_clocks();

  /* Selecting the default clock configuration. */
  if (hal_lld_clock_switch_mode(&hal_clkcfg_default)) {
    osalSysHalt("clkswc");
  }

  /* Backup domain initializations.*/
  bd_init();

  /* Cache enable.*/
  icache_init();
#endif /* STM32_NO_INIT */
}

#else /* !defined(HAL_LLD_USE_CLOCK_MANAGEMENT) */
void stm32_clock_init(void) {

#if !STM32_NO_INIT

  /* Reset of all peripherals.
     Note, GPIOs are not reset because initialized before this point in
     board files.*/
  rccResetAHB1R1(~0);
  rccResetAHB1R2(~0);
  rccResetAHB2R1(~STM32_GPIO_EN_MASK);
  rccResetAHB2R2(~0);
  rccResetAPB1R1(~0);
  rccResetAPB1R2(~0);
  rccResetAPB2(~0);
  rccResetAPB3(~0);

  /* RTC APB clock enable.*/
  /* TODO: move in the RTC driver.*/
#if (HAL_USE_RTC == TRUE) && defined(RCC_APB3ENR_RTCAPBEN)
  rccEnableAPB3(RCC_APB3ENR_RTCAPBEN, true);
#endif

  /* Static PWR configurations.*/
  hal_lld_set_static_pwr();

  /* Backup domain reset.*/
  bd_reset();

  /* Clocks setup.*/
  lse_init();
  lsi_init();
  hsi16_init();
  hsi48_init();
  hse_init();

  /* PWR core voltage (range) and thresholds setup (EPOD booster).*/
  PWR->VOSR = STM32_PWR_VOSR;
#if (STM32_PWR_VOSR & PWR_VOSR_R1EN) != 0U
  while ((PWR->VOSR & PWR_VOSR_R1RDY) == 0U) {
    /* Wait until regulator is stable.*/
  }
#endif
#if (STM32_PWR_VOSR & PWR_VOSR_R2EN) != 0U
  while ((PWR->VOSR & PWR_VOSR_R2RDY) == 0U) {
    /* Wait until regulator is stable.*/
  }
#endif
  if (STM32_BOOSTER_ENABLED) {
    RCC->CFGR4 = STM32_BOOSTSEL | STM32_BOOSTDIV;
    PWR->VOSR |= PWR_VOSR_BOOSTEN;
    while ((PWR->VOSR & PWR_VOSR_BOOSTRDY) == 0U) {
      /* Booster stabilization time.*/
    }
  }

  /* Backup domain initializations.*/
  bd_init();

  /* Setup of flash WS's before changing clocks.*/
  flash_set_acr((STM32_FLASH_ACR & ~STM32_ACR_LATENCY_MASK) | STM32_FLASHBITS);

  /* Setup of clocks dividers before changing clocks.*/
  hal_lld_set_static_clocks();

  /* MSI activation, if required.*/
  {
    uint32_t cr, crrdy, icscr1;

    icscr1 = RCC->ICSCR1 & ~(RCC_ICSCR1_MSISSEL_Msk    | RCC_ICSCR1_MSISDIV_Msk    |
                             RCC_ICSCR1_MSIKSEL_Msk    | RCC_ICSCR1_MSIKDIV_Msk    |
                             RCC_ICSCR1_MSIPLL1N_Msk   | RCC_ICSCR1_MSIBIAS_Msk    |
                             RCC_ICSCR1_MSIPLL0SEL_Msk | RCC_ICSCR1_MSIPLL1SEL_Msk |
                             RCC_ICSCR1_MSIHSINDIV_Msk);
    icscr1 |= STM32_MSISSEL    | STM32_MSISDIV    |
              STM32_MSIKSEL    | STM32_MSIKDIV    |
              STM32_MSIPLL1N   | STM32_MSIBIAS    |
              STM32_MSIPLL0SEL | STM32_MSIPLL1SEL |
              STM32_MSIHSINDIV;
    RCC->ICSCR1 = icscr1 | RCC_ICSCR1_MSIRGSEL; /* Note, ICSCR1 enforced.*/

    cr = RCC->CR & ~(RCC_CR_MSIPLL0FAST_Msk | RCC_CR_MSIPLL1FAST_Msk |
                     RCC_CR_MSIPLL0EN_Msk   | RCC_CR_MSIPLL1EN_Msk   |
                     RCC_CR_MSIKON_Msk      | RCC_CR_MSIKERON_Msk    |
                     RCC_CR_MSISON_Msk);

    /* MSI clocks activation and waiting.*/
    crrdy = 0U;
#if STM32_ACTIVATE_MSIS == TRUE
    cr    |= RCC_CR_MSISON;
    crrdy |= RCC_CR_MSISRDY;
#endif
#if STM32_ACTIVATE_MSIK == TRUE
    cr    |= RCC_CR_MSIKON | RCC_CR_MSIKERON; /* Note, MSIKERON enforced.*/
    crrdy |= RCC_CR_MSIKRDY;
#endif
    RCC->CR = cr;
    while ((RCC->CR & crrdy) != crrdy) {
      /* Waiting.*/
    }

    cr |= STM32_MSIPLL0FAST | STM32_MSIPLL1FAST |
          STM32_MSIPLL0EN | STM32_MSIPLL1EN;

    /* PLLs activation and wait time.*/
    RCC->CR = cr;
    crrdy = ((STM32_MSIPLL0EN | STM32_MSIPLL1EN) >> RCC_CR_MSIPLL1EN_Pos) << RCC_CR_MSIPLL1RDY_Pos;
    while ((RCC->CR & crrdy) != crrdy) {
      /* Waiting.*/
    }
  }

  /* Switching to the configured SYSCLK source if it is different from MSIS.*/
#if STM32_SW != STM32_SW_MSIS
  RCC->CFGR1 |= STM32_SW;       /* Switches on the selected clock source.   */
//  while(1);
  while ((RCC->CFGR1 & STM32_SWS_MASK) != (STM32_SW << RCC_CFGR1_SWS_Pos)) {
    /* Wait until SYSCLK is stable.*/
  }
#endif

  /* Cache enable.*/
  icache_init();

#endif /* STM32_NO_INIT */
}
#endif /* !defined(HAL_LLD_USE_CLOCK_MANAGEMENT) */

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

  if (hal_lld_clock_raw_switch(ccp)) {
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
