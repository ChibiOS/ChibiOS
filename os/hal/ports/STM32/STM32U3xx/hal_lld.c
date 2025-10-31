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
#define STM32_WS_THRESHOLDS             6

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
#if STM32_HSE_BYPASS
                        | RCC_CR_HSEBYP
#endif
#if STM32_HSI48_ENABLED
                        | RCC_CR_HSI48ON
#endif
#if STM32_HSI_ENABLED
                        | RCC_CR_HSIKERON | RCC_CR_HSION
#endif
                          ,
  .rcc_cfgr1            = STM32_MCO2SEL     | STM32_MCO2PRE     |
                          STM32_MCO1SEL     | STM32_MCO1PRE     |
                          STM32_TIMPRE      | STM32_RTCPRE      |
                          STM32_STOPKERWUCK | STM32_STOPWUCK    |
                          STM32_SW,
  .rcc_cfgr2            = STM32_PPRE3       | STM32_PPRE2       |
                          STM32_PPRE1       | STM32_HPRE,
  .flash_acr            = FLASH_ACR_PRFTEN  | STM32_FLASHBITS,
  .plls = {
    [0] = {
      .cfgr             = STM32_PLL1REN     | STM32_PLL1QEN     |
                          STM32_PLL1PEN     | STM32_PLL1M       |
                          STM32_PLL1RGE     | STM32_PLL1VCOSEL  |
                          STM32_PLL1SRC,
      .divr             = STM32_PLL1R       | STM32_PLL1Q       |
                          STM32_PLL1P       | STM32_PLL1N,
      .frac             = STM32_RCC_PLL1FRACR_RESET
    },
    [1] = {
      .cfgr             = STM32_PLL2REN     | STM32_PLL2QEN     |
                          STM32_PLL2PEN     | STM32_PLL2M       |
                          STM32_PLL2RGE     | STM32_PLL2VCOSEL  |
                          STM32_PLL2SRC,
      .divr             = STM32_PLL2R       | STM32_PLL2Q       |
                          STM32_PLL2P       | STM32_PLL2N,
      .frac             = STM32_RCC_PLL2FRACR_RESET
    },
#if STM32_RCC_HAS_PLL3
    [2] = {
      .cfgr             = STM32_PLL3REN     | STM32_PLL3QEN     |
                          STM32_PLL3PEN     | STM32_PLL3M       |
                          STM32_PLL3RGE     | STM32_PLL3VCOSEL  |
                          STM32_PLL3SRC,
      .divr             = STM32_PLL3R       | STM32_PLL3Q       |
                          STM32_PLL3P       | STM32_PLL3N,
      .frac             = STM32_RCC_PLL3FRACR_RESET
    }
#endif
  }
};
#endif /* defined(HAL_LLD_USE_CLOCK_MANAGEMENT) */

/*===========================================================================*/
/* Driver local variables and types.                                         */
/*===========================================================================*/

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
  flash_set_acr((STM32_FLASH_ACR & ~STM32_LATENCY_MASK) | STM32_FLASHBITS);

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
    RCC->ICSCR1 = icscr1 | STM32_MSIRGSEL_ICSCR1; /* Note, ICSCR1 enforced.*/

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

/** @} */
