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
 * @file    STM32WLxx/hal_lld.c
 * @brief   STM32WLxx HAL subsystem low level driver source.
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
 * @note    It is declared in system_stm32wlxx.h.
 */
uint32_t SystemCoreClock = STM32_HCLK;

/*===========================================================================*/
/* Driver local variables and types.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

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

  /* IRQ subsystem initialization.*/
  irqInit();
}

/**
 * @brief   STM32WLxx clocks and PLL initialization.
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
  /* Reset all except FLASH.*/
  rccResetAHB3(RCC_AHB3RSTR_PKARST | RCC_AHB3RSTR_AESRST |
               RCC_AHB3RSTR_RNGRST | RCC_AHB3RSTR_HSEMRST);
  rccResetAPB1R1(~0);
  rccResetAPB1R2(~0);
  rccResetAPB2(~0);

  /* Flash setup for selected MSI speed setting.*/
  FLASH->ACR = FLASH_ACR_DCEN | FLASH_ACR_ICEN | FLASH_ACR_PRFTEN |
               STM32_MSI_FLASHBITS;

  /* RTC clock enable.*/
#if HAL_USE_RTC
  rccEnableAPB1R1(RCC_APB1ENR1_RTCAPBEN, false)
#endif

  /* Core voltage setup, backup domain access enabled and left open.*/
  PWR->CR1 = STM32_VOS | PWR_CR1_DBP;
  while ((PWR->SR2 & PWR_SR2_VOSF) != 0)    /* Wait until regulator is      */
    ;                                       /* stable.                      */

  /* Programmable voltage detector enable.*/
#if STM32_PVD_ENABLE
  PWR->CR2 = PWR_CR2_PVDE | (STM32_PLS & STM32_PLS_MASK);
#else
  PWR->CR2 = 0;
#endif /* STM32_PVD_ENABLE */

  /* MSI clock reset.*/
  msi_reset();

  /* Backup domain reset.*/
  bd_reset();

  /* Clocks setup.*/
  lse_init();
  lsi_init();
  msi_init();
  hsi16_init();
  hse32_init();

  /* Backup domain initializations.*/
  bd_init();

  /* PLLs activation, if required.*/
  pll_init();

  /* Other clock-related settings (dividers, MCO etc).*/
  RCC->CFGR = STM32_MCOPRE | STM32_MCOSEL | STM32_STOPWUCK |
              STM32_PPRE2  | STM32_PPRE1  | STM32_HPRE;
  RCC->EXTCFGR = STM32_SHDHPRE;

  /* CCIPR register initialization, note, must take care of the _OFF
     pseudo settings.*/
  {
    uint32_t ccipr = STM32_RNGSEL    | STM32_ADCSEL    | STM32_LPTIM3SEL  |
                     STM32_LPTIM2SEL | STM32_LPTIM1SEL | STM32_I2C3SEL    |
                     STM32_I2C2SEL   | STM32_I2C1SEL   | STM32_LPUART1SEL |
                     STM32_SPI2SEL   | STM32_USART2SEL | STM32_USART1SEL;

    RCC->CCIPR = ccipr;
  }

  /* Set flash WS's for SYSCLK source */
  if (STM32_FLASHBITS > STM32_MSI_FLASHBITS) {
    FLASH->ACR = (FLASH->ACR & ~FLASH_ACR_LATENCY_Msk) | STM32_FLASHBITS;
    while ((FLASH->ACR & FLASH_ACR_LATENCY_Msk) !=
           (STM32_FLASHBITS & FLASH_ACR_LATENCY_Msk)) {
    }
  }

  /* Switching to the configured SYSCLK source if it is different from MSI.*/
#if (STM32_SW != STM32_SW_MSI)
  RCC->CFGR |= STM32_SW;        /* Switches on the selected clock source.   */
  /* Wait until SYSCLK is stable.*/
  while ((RCC->CFGR & RCC_CFGR_SWS) != (STM32_SW << 2))
    ;
#endif

  /* Reduce the flash WS's for SYSCLK source if they are less than MSI WSs */
  if (STM32_FLASHBITS < STM32_MSI_FLASHBITS) {
    FLASH->ACR = (FLASH->ACR & ~FLASH_ACR_LATENCY_Msk) | STM32_FLASHBITS;
    while ((FLASH->ACR & FLASH_ACR_LATENCY_Msk) !=
           (STM32_FLASHBITS & FLASH_ACR_LATENCY_Msk)) {
    }
  }
#endif /* STM32_NO_INIT */
}

/** @} */
