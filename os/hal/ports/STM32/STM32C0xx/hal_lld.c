/*
    ChibiOS - Copyright (C) 2006..2022 Giovanni Di Sirio

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
 * @file    STM32C0xx+/hal_lld.c
 * @brief   STM32C0xx+ HAL subsystem low level driver source.
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
#define STM32_WS_THRESHOLDS                 2

/**
 * @name    Registers reset values
 * @{
 */
#define STM32_FLASH_ACR_RESET               0x00040600U
#define STM32_RCC_CR_RESET                  0x00001540U
#define STM32_RCC_CFGR_RESET                0x00000000U
/** @} */

/**
 * @name    Stabilization times
 * @{
 */
#define STM32_RELAXED_TIMEOUT_FACTOR        5U
#define STM32_HSI_STARTUP_TIME              (4U * STM32_RELAXED_TIMEOUT_FACTOR)
#define STM32_OSCILLATORS_STARTUP_TIME      (2000U * STM32_RELAXED_TIMEOUT_FACTOR)
#define STM32_SYSCLK_SWITCH_TIME            (50U * STM32_RELAXED_TIMEOUT_FACTOR)
/** @} */

/* Reserved bit to be kept at 1, ST mysteries.*/
#define FLASH_ACR_RESVD10                   (1U << 10)

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

/**
 * @brief   CMSIS system core clock variable.
 * @note    It is declared in system_stm32c0xx.h.
 */
uint32_t SystemCoreClock = STM32_HCLK;

/**
 * @brief   Post-reset clock configuration.
 */
const halclkcfg_t hal_clkcfg_reset = {
  .rcc_cr               = STM32_RCC_CR_RESET,
  .rcc_cfgr             = STM32_RCC_CFGR_RESET,
  .flash_acr            = STM32_FLASH_ACR_RESET
};

/**
 * @brief   Default clock configuration.
 * @note    This is the configuration defined in mcuconf.h.
 */
const halclkcfg_t hal_clkcfg_default = {
  .rcc_cr               = 0U
#if STM32_HSIUSB48_ENABLED == TRUE
                        | RCC_CR_HSIUSB48ON
#endif
#if defined(STM32_HSE_BYPASS)
                        | RCC_CR_HSEBYP
#endif
#if STM32_HSE_ENABLED == TRUE
                        | RCC_CR_HSEON
#endif
#if STM32_HSI48_ENABLED == TRUE
                        | RCC_CR_HSION  | RCC_CR_HSIKERON
#endif
                        | STM32_HSIDIV  | STM32_HSIKERDIV | STM32_SYSDIV,
  .rcc_cfgr             = STM32_MCOPRE  | STM32_MCOSEL  |
                          STM32_MCO2PRE | STM32_MCO2SEL |
                          STM32_PPRE    | STM32_HPRE    |
                          STM32_SW,
  .flash_acr            = (STM32_FLASH_ACR & ~FLASH_ACR_LATENCY_Msk) |
                          STM32_FLASHBITS | FLASH_ACR_RESVD10
};

/*===========================================================================*/
/* Driver local variables and types.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

#include "stm32_bd_v3.inc"

/**
 * @brief   Configures the SYSCFG unit.
 */
static void hal_lld_set_static_syscfg(void) {

  /* SYSCFG clock enabled.*/
  rccEnableAPBR2(RCC_APBENR2_SYSCFGEN, false);

  halRegWrite32X(&SYSCFG->CFGR1, STM32_SYSCFG_CFGR1, true);
  halRegWrite32X(&SYSCFG->CFGR2, STM32_SYSCFG_CFGR2, true);
  halRegWrite32X(&SYSCFG->CFGR3, STM32_SYSCFG_CFGR3, true);
}

/**
 * @brief   Configures the PWR unit.
 * @note    CR1 is not initialized inside this function.
 */
static void hal_lld_set_static_pwr(void) {

  /* PWR clock enabled.*/
  rccEnablePWRInterface(false);

  /* Static PWR configurations.*/
#if defined(PWR_CR2_PVM_VDDIO2_Pos)
  halRegWrite32X(&PWR->CR2,   STM32_PWR_CR2,   true);
#endif
  halRegWrite32X(&PWR->CR3,   STM32_PWR_CR3,   true);
  halRegWrite32X(&PWR->CR4,   STM32_PWR_CR4,   true);
  halRegWrite32X(&PWR->PUCRA, STM32_PWR_PUCRA, true);
  halRegWrite32X(&PWR->PDCRA, STM32_PWR_PDCRA, true);
  halRegWrite32X(&PWR->PDCRB, STM32_PWR_PDCRB, true);
  halRegWrite32X(&PWR->PUCRB, STM32_PWR_PUCRB, true);
  halRegWrite32X(&PWR->PUCRC, STM32_PWR_PUCRC, true);
  halRegWrite32X(&PWR->PDCRC, STM32_PWR_PDCRC, true);
#if STM32_HAS_GPIOD
  halRegWrite32X(&PWR->PUCRD, STM32_PWR_PUCRD, true);
  halRegWrite32X(&PWR->PDCRD, STM32_PWR_PDCRD, true);
#endif
#if STM32_HAS_GPIOE
  halRegWrite32X(&PWR->PUCRE, STM32_PWR_PUCRE, true);
  halRegWrite32X(&PWR->PDCRE, STM32_PWR_PDCRE, true);
#endif
#if STM32_HAS_GPIOF
  halRegWrite32X(&PWR->PUCRF, STM32_PWR_PUCRF, true);
  halRegWrite32X(&PWR->PDCRF, STM32_PWR_PDCRF, true);
#endif
#if STM32_HAS_GPIOG
  halRegWrite32X(&PWR->PUCRG, STM32_PWR_PUCRG, true);
  halRegWrite32X(&PWR->PDCRG, STM32_PWR_PDCRG, true);
#endif
#if STM32_HAS_GPIOH
  halRegWrite32X(&PWR->PUCRH, STM32_PWR_PUCRH, true);
  halRegWrite32X(&PWR->PDCRH, STM32_PWR_PDCRH, true);
#endif
#if STM32_HAS_GPIOI
  halRegWrite32X(&PWR->PUCRI, STM32_PWR_PUCRI, true);
  halRegWrite32X(&PWR->PDCRI, STM32_PWR_PDCRI, true);
#endif
}

/**
 * @brief   Initializes static muxes and dividers.
 */
static void hal_lld_set_static_clocks(void) {

  /* CCIPR register initialization.*/
  halRegWrite32X(&RCC->CCIPR,
                 STM32_ADCSEL | STM32_I2S1SEL | STM32_I2C1SEL | STM32_FDCAN1SEL | STM32_USART1SEL,
                 true);

#if STM32_HAS_USB
  /* CCIPR2 register initialization.*/
  halRegWrite32X(&RCC->CCIPR2, STM32_USBSEL, true);
#endif
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
                 FLASH_ACR_DBG_SWEN | FLASH_ACR_RESVD10 | FLASH_ACR_ICEN | FLASH_ACR_LATENCY_1WS,
                 true);

  /* Using default clock settings (HSION) during transition.*/
  halRegWrite32X(&RCC->CR, STM32_RCC_CR_RESET, true);
  if (halRegWaitAllSet32X(&RCC->CR, RCC_CR_HSIRDY,
                          STM32_HSI_STARTUP_TIME,
                          NULL)) {
    return true;
  }

  /* Making sure to run from HSI.*/
  halRegWrite32X(&RCC->CFGR, STM32_RCC_CFGR_RESET, true);
  if (halRegWaitMatch32X(&RCC->CFGR,
                         RCC_CFGR_SWS_Msk, RCC_CFGR_SWS_HSI,
                         STM32_SYSCLK_SWITCH_TIME,
                         NULL)) {
    return true;
  }

  /* Enabling all required oscillators at same time, HSI enforced active
     because running from it.*/
  halRegWrite32X(&RCC->CR, ccp->rcc_cr | RCC_CR_HSION, true);

  /* Adding to the "wait mask" the status bits of other enabled oscillators,
     waiting for all oscillators to become ready.*/
  wtmask = RCC_CR_HSIRDY;                  /* Known to be ready already.*/
  if ((ccp->rcc_cr & RCC_CR_HSEON) != 0U) {
    wtmask |= RCC_CR_HSERDY;
  }
  if ((ccp->rcc_cr & RCC_CR_HSIUSB48ON) != 0U) {
    wtmask |= RCC_CR_HSIUSB48RDY;
  }
  if (halRegWaitAllSet32X(&RCC->CR,
                          wtmask,
                          STM32_OSCILLATORS_STARTUP_TIME,
                          NULL)) {
    return true;
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

  /* Final RCC_CR value, HSI could go off at this point if it is not part
     of the mask.*/
  halRegWrite32X(&RCC->CR, ccp->rcc_cr, true);

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
 * @brief   STM32G0xx clocks and PLL initialization.
 * @note    All the involved constants come from the file @p board.h.
 * @note    This function should be invoked just after the system reset.
 *
 * @special
 */
void stm32_clock_init(void) {

  /* Enabling TIM17 for timeout handling.*/
  rccResetTIM17();
  rccEnableTIM17(false);
  halRegWrite32X(&TIM17->PSC, (STM32_TIMPCLK / 1000000U) - 1U, true);
//  halRegWrite32X(&TIM7->ARR, 0xFFFFU, true);
  halRegWrite32X(&TIM17->EGR, TIM_EGR_UG, false);
  halRegWrite32X(&TIM17->CR1, TIM_CR1_CEN, true);

#if !STM32_NO_INIT
  /* Reset of all peripherals.*/
  rccResetAHB(~0);
  rccResetAPBR1(~0);
  rccResetAPBR2(~0);

  /* RTC APB clock enable.*/
#if (HAL_USE_RTC == TRUE) && defined(RCC_APBENR1_RTCAPBEN)
  rccEnableAPBR1(RCC_APBENR1_RTCAPBEN, false);
#endif

  /* Static SYSCFG configurations.*/
  hal_lld_set_static_syscfg();

  /* Static PWR configurations.*/
  hal_lld_set_static_pwr();

  /* Backup domain reset.*/
  bd_reset();

  /* Clocks setup.*/
  lse_init();
  lsi_init();

  /* Backup domain initializations.*/
  bd_init();

  /* Static clocks setup.*/
  hal_lld_set_static_clocks();

  /* Selecting the default clock configuration. */
  halSftFailOnError(hal_lld_clock_configure(&hal_clkcfg_default), "clkinit");

#endif /* STM32_NO_INIT */
}

/** @} */
