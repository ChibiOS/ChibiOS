/*
    ChibiOS - Copyright (C) 2006..2018 Giovanni Di Sirio

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
 * @file    STM32G4xx/hal_lld.c
 * @brief   STM32G4xx HAL subsystem low level driver source.
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
 * @note    It is declared in system_stm32g4xx.h.
 */
uint32_t SystemCoreClock = STM32_HCLK;


#if defined(HAL_LLD_USE_CLOCK_MANAGEMENT) || defined(__DOXYGEN__)
/**
 * @brief   Post-reset clock configuration.
 */
const halclkcfg_t hal_clkcfg_reset = {
  .pwr_cr1              = PWR_CR1_VOS_0,
  .pwr_cr2              = 0U,
  .pwr_cr3              = PWR_CR3_EIWF,
  .pwr_cr4              = 0U,
  .pwr_cr5              = PWR_CR5_R1MODE,
  .rcc_cr               = RCC_CR_HSIKERON | RCC_CR_HSION,
  .rcc_cfgr             = RCC_CFGR_SW_HSI,
  .rcc_pllcfgr          = 0U,
  .flash_acr            = FLASH_ACR_DBG_SWEN | FLASH_ACR_DCEN   |
                          FLASH_ACR_ICEN     | FLASH_ACR_LATENCY_1WS
};

/**
 * @brief   Default clock configuration.
 */
const halclkcfg_t hal_clkcfg_default = {
  .pwr_cr1              = STM32_VOS_RANGE1,
  .pwr_cr2              = STM32_PWR_CR2,
  .pwr_cr3              = STM32_PWR_CR3,
  .pwr_cr4              = STM32_PWR_CR4,
  .pwr_cr5              = STM32_CR5BITS,
  .rcc_cr               = 0U
#if STM32_HSI16_ENABLED
                        | RCC_CR_HSIKERON | RCC_CR_HSION
#endif
#if STM32_HSE_ENABLED
                        | RCC_CR_HSEON
#endif
#if STM32_ACTIVATE_PLL
                        | RCC_CR_PLLON
#endif
                        ,
  .rcc_cfgr             = STM32_MCOPRE  | STM32_MCOSEL |
                          STM32_PPRE2   | STM32_PPRE1  |
                          STM32_HPRE    | STM32_SW,
  .rcc_pllcfgr          = STM32_PLLPDIV | STM32_PLLR   |
                          STM32_PLLREN  | STM32_PLLQ   |
                          STM32_PLLQEN  | STM32_PLLP   |
                          STM32_PLLPEN  | STM32_PLLN   |
                          STM32_PLLM    | STM32_PLLSRC,
  .flash_acr            = FLASH_ACR_DBG_SWEN | FLASH_ACR_DCEN   |
                          FLASH_ACR_ICEN     | FLASH_ACR_PRFTEN |
                          STM32_FLASHBITS
};
#endif /* defined(HAL_LLD_USE_CLOCK_MANAGEMENT) */

/*===========================================================================*/
/* Driver local variables and types.                                         */
/*===========================================================================*/

#if defined(HAL_LLD_USE_CLOCK_MANAGEMENT) || defined(__DOXYGEN__)
/**
 * @brief   Dynamic clock points for this device.
 */
static halfreq_t clock_points[CLK_ARRAY_SIZE];

/**
 * @brief   Type of a structure representing system limits.
 */
typedef struct {
  halfreq_t     sysclk_max_boost;
  halfreq_t     sysclk_max_noboost;
  halfreq_t     pllin_max;
  halfreq_t     pllin_min;
  halfreq_t     pllvco_max;
  halfreq_t     pllvco_min;
  halfreq_t     pllp_max;
  halfreq_t     pllp_min;
  halfreq_t     pllq_max;
  halfreq_t     pllq_min;
  halfreq_t     pllr_max;
  halfreq_t     pllr_min;
  halfreq_t     flash_thresholds[9];
} system_limits_t;

/**
 * @brief   System limits for VOS RANGE1.
 */
static const system_limits_t vos_range1 = {
  .sysclk_max_boost     = 170000000U,
  .sysclk_max_noboost   = 150000000U,
  .pllin_max            =  16000000U,
  .pllin_min            =   2660000U,
  .pllvco_max           = 344000000U,
  .pllvco_min           =  96000000U,
  .pllp_max             = 170000000U,
  .pllp_min             =   2064500U,
  .pllq_max             = 170000000U,
  .pllq_min             =   8000000U,
  .pllr_max             = 170000000U,
  .pllr_min             =   8000000U,
  .flash_thresholds     = { 20000000U,  40000000U,  60000000U,  80000000U,
                           100000000U, 120000000U, 140000000U, 160000000U,
                           170000000U}
};

/**
 * @brief   System limits for VOS RANGE2.
 */
static const system_limits_t vos_range2 = {
  .sysclk_max_boost     =         0U,
  .sysclk_max_noboost   =  26000000U,
  .pllin_max            =  16000000U,
  .pllin_min            =   2660000U,
  .pllvco_max           = 128000000U,
  .pllvco_min           =  96000000U,
  .pllp_max             =  26000000U,
  .pllp_min             =   2064500U,
  .pllq_max             =  26000000U,
  .pllq_min             =   8000000U,
  .pllr_max             =  26000000U,
  .pllr_min             =   8000000U,
  .flash_thresholds     = {  8000000U,  16000000U,  26000000U,         0U,
                                   0U,         0U,         0U,         0U,
                                   0U}
};
#endif /* defined(HAL_LLD_USE_CLOCK_MANAGEMENT) */

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

/**
 * @brief   Resets the backup domain.
 */
__STATIC_INLINE void bd_reset(void) {

  /* Reset BKP domain if different clock source selected.*/
  if ((RCC->BDCR & STM32_RTCSEL_MASK) != STM32_RTCSEL) {
    /* Backup domain reset.*/
    RCC->BDCR = RCC_BDCR_BDRST;
    RCC->BDCR = 0U;
  }
}

/**
 * @brief   Initializes the backup domain.
 * @note    WARNING! Changing RTC clock source impossible without reset
 *          of the whole BKP domain.
 */
__STATIC_INLINE void bd_init(void) {
  uint32_t bdcr;

  /* Current settings.*/
  bdcr = RCC->BDCR;

#if HAL_USE_RTC
  /* RTC clock enabled.*/
  if ((bdcr & RCC_BDCR_RTCEN) == 0) {
    bdcr |= RCC_BDCR_RTCEN;
  }
#endif /* HAL_USE_RTC */

  /* Selectors.*/
  bdcr &= ~(STM32_RTCSEL_MASK | STM32_LSCOSEL_MASK);
  bdcr |= STM32_RTCSEL | STM32_LSCOSEL;

  /* Final settings.*/
  RCC->BDCR = bdcr;
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
bool hal_lld_clock_check_tree(const halclkcfg_t *ccp) {
  static const uint32_t hprediv[16] = {1U, 1U, 1U, 1U, 1U, 1U, 1U, 1U,
                                       2U, 4U, 8U, 16U, 64U, 128U, 256U, 512U};
  static const uint32_t pprediv[16] = {1U, 1U, 1U, 1U, 2U, 4U, 8U, 16U};
  const system_limits_t *slp;
  halfreq_t hsi16clk = 0U, hseclk = 0U, pllselclk;
  halfreq_t pllpclk = 0U, pllqclk = 0U, pllrclk = 0U;
  halfreq_t sysclk, hclk, pclk1, pclk2, pclk1tim, pclk2tim, mcoclk;
  uint32_t mcodiv;

  /* System limits based on desired VOS settings.*/
  if ((ccp->pwr_cr1 & PWR_CR1_VOS_Msk) == PWR_CR1_VOS_1) {
    slp = &vos_range2;
  }
  else if ((ccp->pwr_cr1 & PWR_CR1_VOS_Msk) == PWR_CR1_VOS_0) {
    slp = &vos_range1;
  }
  else {
    return true;
  }

  /* HSI16 clock.*/
  if ((ccp->rcc_cr & RCC_CR_HSION) != 0U) {
    hsi16clk = STM32_HSI16CLK;
  }

  /* HSE clock.*/
  if ((ccp->rcc_cr & RCC_CR_HSEON) != 0U) {
    hseclk = STM32_HSECLK;
  }

  /* PLL MUX clock.*/
  switch (ccp->rcc_pllcfgr & RCC_PLLCFGR_PLLSRC_Msk) {
  case RCC_PLLCFGR_PLLSRC_HSI:
    pllselclk = hsi16clk;
    break;
  case RCC_PLLCFGR_PLLSRC_HSE:
    pllselclk = hseclk;
    break;
  default:
    pllselclk = 0U;
  }

  /* PLL outputs.*/
  if ((ccp->rcc_cr & RCC_CR_PLLON) != 0U) {
    uint32_t pllmdiv, pllndiv, pllpdiv, pllqdiv, pllrdiv;
    halfreq_t pllvcoclk;

    /* PLL M divider.*/
    pllmdiv = ((ccp->rcc_pllcfgr & RCC_PLLCFGR_PLLM_Msk) >> RCC_PLLCFGR_PLLM_Pos) + 1U;

    /* PLL N divider.*/
    pllndiv = (ccp->rcc_pllcfgr & RCC_PLLCFGR_PLLN_Msk) >> RCC_PLLCFGR_PLLN_Pos;
    if (pllndiv < 8) {
      return true;
    }

    /* PLL VCO frequency.*/
    pllvcoclk = (pllselclk / (halfreq_t)pllmdiv) * (halfreq_t)pllndiv;

    if((pllvcoclk < slp->pllvco_min) || (pllvcoclk > slp->pllvco_max)) {
      return true;
    }

    /* PLL P output frequency.*/
    pllpdiv = (ccp->rcc_pllcfgr & RCC_PLLCFGR_PLLPDIV_Msk) >> RCC_PLLCFGR_PLLPDIV_Pos;
    if (pllpdiv == 1U) {
      return true;
    }
    if (pllpdiv == 0U) {
      if ((ccp->rcc_pllcfgr & RCC_PLLCFGR_PLLP) == 0U) {
        pllpdiv = 7U;
      }
      else {
        pllpdiv = 17U;
      }
    }
    if ((ccp->rcc_pllcfgr & RCC_PLLCFGR_PLLPEN) != 0U) {
      pllpclk = pllvcoclk / pllpdiv;

      if((pllpclk < slp->pllp_min) || (pllpclk > slp->pllp_max)) {
        return true;
      }
    }

    /* PLL Q output frequency.*/
    pllqdiv = 2U + (2U * (ccp->rcc_pllcfgr & RCC_PLLCFGR_PLLQ_Msk) >> RCC_PLLCFGR_PLLQ_Pos);
    if ((ccp->rcc_pllcfgr & RCC_PLLCFGR_PLLQEN) != 0U) {
      pllqclk = pllvcoclk / pllqdiv;

      if((pllqclk < slp->pllq_min) || (pllqclk > slp->pllq_max)) {
        return true;
      }
    }

    /* PLL R output frequency.*/
    pllrdiv = 2U + (2U * (ccp->rcc_pllcfgr & RCC_PLLCFGR_PLLR_Msk) >> RCC_PLLCFGR_PLLR_Pos);
    if ((ccp->rcc_pllcfgr & RCC_PLLCFGR_PLLREN) != 0U) {
      pllrclk = pllvcoclk / pllrdiv;

      if((pllrclk < slp->pllr_min) || (pllrclk > slp->pllr_max)) {
        return true;
      }
    }
  }

  /* SYSCLK frequency.*/
  switch(ccp->rcc_cfgr & RCC_CFGR_SW_Msk) {
  case RCC_CFGR_SW_HSI:
    sysclk = hsi16clk;
    break;
  case RCC_CFGR_SW_HSE:
    sysclk = hseclk;
    break;
  case RCC_CFGR_SW_PLL:
    sysclk = pllrclk;
    break;
  default:
    sysclk = 0U;
  }

  if ((ccp->pwr_cr5 & PWR_CR5_R1MODE) == 0U) {
    if (sysclk < slp->sysclk_max_boost) {
      return true;
    }
  }
  else {
    if (sysclk < slp->sysclk_max_noboost) {
      return true;
    }
  }

  /* HCLK frequency.*/
  hclk = sysclk / hprediv[(ccp->rcc_cfgr & RCC_CFGR_HPRE_Msk) >> RCC_CFGR_HPRE_Pos];

  /* PPRE1 frequency.*/
  pclk1 = hclk / pprediv[(ccp->rcc_cfgr & RCC_CFGR_PPRE1_Msk) >> RCC_CFGR_PPRE1_Pos];
  if ((ccp->rcc_cfgr & RCC_CFGR_PPRE1_Msk) < RCC_CFGR_PPRE1_DIV2) {
    pclk1tim = pclk1;
  }
  else {
    pclk1tim = pclk1 * 2U;
  }

  /* PPRE2 frequency.*/
  pclk2 = hclk / pprediv[(ccp->rcc_cfgr & RCC_CFGR_PPRE2_Msk) >> RCC_CFGR_PPRE2_Pos];
  if ((ccp->rcc_cfgr & RCC_CFGR_PPRE1_Msk) < RCC_CFGR_PPRE2_DIV2) {
    pclk2tim = pclk2;
  }
  else {
    pclk2tim = pclk2 * 2U;
  }

  /* MCO clock.*/
  switch(ccp->rcc_cfgr & RCC_CFGR_MCOSEL_Msk) {
  case STM32_MCOSEL_NOCLOCK:
    mcoclk = 0U;
    break;
  case STM32_MCOSEL_SYSCLK:
    mcoclk = sysclk;
    break;
  case STM32_MCOSEL_HSI16:
    mcoclk = hsi16clk;
    break;
  case STM32_MCOSEL_HSE:
    mcoclk = hseclk;
    break;
  case STM32_MCOSEL_PLLRCLK:
    mcoclk = pllrclk;
    break;
  case STM32_MCOSEL_LSI:
    mcoclk = STM32_LSICLK;
    break;
  case STM32_MCOSEL_LSE:
    mcoclk = STM32_LSECLK;
    break;
  case STM32_MCOSEL_HSI48:
    mcoclk = STM32_HSI48CLK;
    break;
  default:
    mcoclk = 0U;
  }
  mcodiv = 1U << ((ccp->rcc_cfgr & RCC_CFGR_MCOPRE_Msk) >> RCC_CFGR_MCOPRE_Pos);
  if (mcodiv > 16U) {
    return true;
  }
  mcoclk /= mcodiv;

  /* Writing out results.*/
  clock_points[CLK_SYSCLK]   = sysclk;
  clock_points[CLK_PLLPCLK]  = pllpclk;
  clock_points[CLK_PLLQCLK]  = pllqclk;
  clock_points[CLK_PLLRCLK]  = pllrclk;
  clock_points[CLK_HCLK]     = hclk;
  clock_points[CLK_PCLK1]    = pclk1;
  clock_points[CLK_PCLK1TIM] = pclk1tim;
  clock_points[CLK_PCLK2]    = pclk2;
  clock_points[CLK_PCLK2TIM] = pclk2tim;
  clock_points[CLK_MCO]      = mcoclk;

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
bool hal_lld_clock_raw_switch(const halclkcfg_t *ccp) {

  /* Restoring default PWR settings related clocks and sleep modes.*/
  PWR->CR1 = PWR_CR1_VOS_0;
  PWR->CR2 = 0U;
  PWR->CR3 = PWR_CR3_EIWF;
  PWR->CR4 = 0U;

  /* Waiting for all regulator status bits to be cleared, this means that
     power levels are stable.*/
  while ((PWR->SR2 & (PWR_SR2_VOSF | PWR_SR2_REGLPF)) != 0U) {
    /* Waiting for the regulator to be ready.*/
  }

  /* Making sure HSI16 is activated.*/
  RCC->CR |= RCC_CR_HSION;
  while ((RCC->CR & RCC_CR_HSIRDY) == 0U) {
    /* Waiting for HSI16 activation.*/
  }

  /* Disabling boost mode.*/
  PWR->CR5 = PWR_CR5_R1MODE;

  /* Switching to the HSI oscillator.*/
  RCC->CFGR = (RCC->CFGR & ~RCC_CFGR_SW_Msk) | RCC_CFGR_SW_HSI;
  while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_HSI) {
    /* Waiting for clock switch.*/
  }

  /* Resetting flash ACR settings to the default value.*/
  FLASH->ACR = 0x00040601U;

  /* HSE setup.*/
  if ((ccp->rcc_cr & RCC_CR_HSEON) == 0U) {
    RCC->CR &= ~RCC_CR_HSEON;
  }
  else {
    RCC->CR |= RCC_CR_HSEON;
    while ((RCC->CR & RCC_CR_HSERDY) == 0U) {
      /* Waiting for HSE activation.*/
    }
  }

  /* PLL setup.*/
  RCC->PLLCFGR = ccp->rcc_pllcfgr;

  /* HSI, HSE, PLL enabled if specified.*/
  RCC->CR =  ccp->rcc_cr;

  /* PLL activation polling if required.*/
  if ((ccp->rcc_cr & RCC_CR_PLLON) != 0U) {
    while ((RCC->CR & RCC_CR_PLLRDY) == 0U) {
      /* Waiting for PLL lock.*/
    }
  }

  /* MCO and bus dividers first.*/
  RCC->CFGR = (RCC->CFGR & RCC_CFGR_SW_Msk) | (ccp->rcc_cfgr & ~RCC_CFGR_SW_Msk);

  /* Final flash ACR settings.*/
  FLASH->ACR = ccp->flash_acr;

  /* Final PWR modes.*/
  PWR->CR1 = ccp->pwr_cr1;
  PWR->CR2 = ccp->pwr_cr2;
  PWR->CR3 = ccp->pwr_cr3;
  PWR->CR4 = ccp->pwr_cr4;
  PWR->CR5 = ccp->pwr_cr5;

  /* Wait on LPR bit clear.*/
  if ((ccp->pwr_cr1 & PWR_CR1_LPR) == 0U) {
    while ((PWR->SR2 & PWR_SR2_REGLPF) != 0U) {
      /* Waiting for the regulator to be ready.*/
    }
  }

  /* Switching to the final clock source.*/
  RCC->CFGR = (RCC->CFGR & ~RCC_CFGR_SW_Msk) | (ccp->rcc_cfgr & RCC_CFGR_SW_Msk);
  while ((RCC->CFGR & RCC_CFGR_SWS) != ((ccp->rcc_cfgr & RCC_CFGR_SW_Msk) << RCC_CFGR_SWS_Pos)) {
    /* Waiting for clock switch.*/
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

#if defined(HAL_LLD_USE_CLOCK_MANAGEMENT)
  if (hal_lld_clock_check_tree(&hal_clkcfg_default)) {
    osalSysHalt("clkcfg");
  }
#endif

  /* DMA subsystems initialization.*/
#if defined(STM32_DMA_REQUIRED)
  dmaInit();
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
  rccResetAHB1(~0);
  rccResetAHB2(~STM32_GPIO_EN_MASK);
  rccResetAHB3(~0);
  rccResetAPB1R1(~0);
  rccResetAPB1R2(~0);
  rccResetAPB2(~0);

  /* SYSCFG clock enabled here because it is a multi-functional unit shared
     among multiple drivers.*/
  rccEnableAPB2(RCC_APB2ENR_SYSCFGEN, false);

  /* PWR clock enable.*/
#if (HAL_USE_RTC == TRUE) && defined(RCC_APBENR1_RTCAPBEN)
  rccEnableAPB1R1(RCC_APB1ENR1_PWREN | RCC_APB1ENR1_RTCAPBEN, false)
#else
  rccEnableAPB1R1(RCC_APB1ENR1_PWREN, false)
#endif

  /* Backup domain made accessible.*/
  PWR->CR1 |= PWR_CR1_DBP;

  /* Backup domain reset.*/
  bd_reset();

  /* Static clocks setup.*/
  lse_init();
  lsi_init();

  /* Selecting the default clock/power/flash configuration.*/
  if (hal_lld_clock_raw_switch(&hal_clkcfg_default)) {
    osalSysHalt("clkswc");
  }

  /* Backup domain initializations.*/
  bd_init();
#endif /* STM32_NO_INIT */
}

#else /* !defined(HAL_LLD_USE_CLOCK_MANAGEMENT) */
void stm32_clock_init(void) {

#if !STM32_NO_INIT

  /* Reset of all peripherals.
     Note, GPIOs are not reset because initialized before this point in
     board files.*/
  rccResetAHB1(~0);
  rccResetAHB2(~STM32_GPIO_EN_MASK);
  rccResetAHB3(~0);
  rccResetAPB1R1(~0);
  rccResetAPB1R2(~0);
  rccResetAPB2(~0);

  /* SYSCFG clock enabled here because it is a multi-functional unit shared
     among multiple drivers.*/
  rccEnableAPB2(RCC_APB2ENR_SYSCFGEN, false);

  /* PWR clock enable.*/
#if (HAL_USE_RTC == TRUE) && defined(RCC_APBENR1_RTCAPBEN)
  rccEnableAPB1R1(RCC_APB1ENR1_PWREN | RCC_APB1ENR1_RTCAPBEN, false)
#else
  rccEnableAPB1R1(RCC_APB1ENR1_PWREN, false)
#endif

  /* Core voltage setup, backup domain made accessible.*/
  PWR->CR1 = STM32_VOS | PWR_CR1_DBP;
  while ((PWR->SR2 & PWR_SR2_VOSF) != 0)    /* Wait until regulator is      */
    ;                                       /* stable.                      */

  /* Additional PWR configurations.*/
  PWR->CR2 = STM32_PWR_CR2;
  PWR->CR3 = STM32_PWR_CR3;
  PWR->CR4 = STM32_PWR_CR4;
  PWR->CR5 = STM32_CR5BITS;

  /* Backup domain reset.*/
  bd_reset();

  /* Clocks setup.*/
  lse_init();
  lsi_init();
  hsi16_init();
  hsi48_init();
  hse_init();

  /* Backup domain initializations.*/
  bd_init();

  /* PLLs activation, if required.*/
  pll_init();

  /* Other clock-related settings (dividers, MCO etc).*/
  RCC->CFGR   = STM32_MCOPRE | STM32_MCOSEL | STM32_PPRE2 | STM32_PPRE1 |
                STM32_HPRE;

  /* CCIPR registers initialization, note.*/
  RCC->CCIPR  = STM32_ADC345SEL  | STM32_ADC12SEL   | STM32_CLK48SEL   |
                STM32_FDCANSEL   | STM32_I2S23SEL   | STM32_SAI1SEL    |
                STM32_LPTIM1SEL  | STM32_I2C3SEL    | STM32_I2C2SEL    |
                STM32_I2C1SEL    | STM32_LPUART1SEL | STM32_UART5SEL   |
                STM32_UART4SEL   | STM32_USART3SEL  | STM32_USART2SEL  |
                STM32_USART1SEL;
  RCC->CCIPR2 = STM32_QSPISEL    | STM32_I2C4SEL;

  /* Set flash WS's for SYSCLK source.*/
  FLASH->ACR = FLASH_ACR_DBG_SWEN | FLASH_ACR_DCEN | FLASH_ACR_ICEN   |
               FLASH_ACR_PRFTEN   | STM32_FLASHBITS;
  while ((FLASH->ACR & FLASH_ACR_LATENCY_Msk) !=
         (STM32_FLASHBITS & FLASH_ACR_LATENCY_Msk)) {
  }

  /* Switching to the configured SYSCLK source if it is different from HSI16.*/
#if STM32_SW != STM32_SW_HSI16
  RCC->CFGR |= STM32_SW;        /* Switches on the selected clock source.   */
  /* Wait until SYSCLK is stable.*/
  while ((RCC->CFGR & RCC_CFGR_SWS) != (STM32_SW << 2))
    ;
#endif

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
