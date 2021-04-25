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
 * @file    STM32G0xx+/hal_lld.c
 * @brief   STM32G0xx+ HAL subsystem low level driver source.
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
 * @note    It is declared in system_stm32g0xx.h.
 */
uint32_t SystemCoreClock = STM32_HCLK;

/*===========================================================================*/
/* Driver local variables and types.                                         */
/*===========================================================================*/

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

#if !STM32_NO_INIT
  /* Reset of all peripherals.*/
  rccResetAHB(~0);
  rccResetAPBR1(~0);
  rccResetAPBR2(~0);

  /* SYSCFG clock enabled here because it is a multi-functional unit shared
     among multiple drivers.*/
  rccEnableAPBR2(RCC_APBENR2_SYSCFGEN, false);

  /* PWR clock enable, backup domain made accessible.*/
  rccEnablePWRInterface(false);

  /* Core voltage setup.*/
  PWR->CR1 = STM32_VOS | PWR_CR1_DBP;
  while ((PWR->SR2 & PWR_SR2_VOSF) != 0)    /* Wait until regulator is      */
    ;                                       /* stable.                      */

  /* Additional PWR configurations.*/
  PWR->CR2 = STM32_PWR_CR2;

  /* Backup domain reset.*/
  bd_reset();

  /* Clocks setup.*/
  lse_init();
  lsi_init();
  hsi16_init();
  hse_init();

  /* Backup domain initializations.*/
  bd_init();

  /* PLLs activation, if required.*/
  pll_init();

  /* Other clock-related settings (dividers, MCO etc).*/
  RCC->CFGR = STM32_MCOPRE | STM32_MCOSEL | STM32_PPRE | STM32_HPRE;

  /* CCIPR register initialization, note, must take care of the _OFF
     pseudo settings.*/
  RCC->CCIPR = STM32_ADCSEL    | STM32_RNGDIV    | STM32_RNGSEL    |
               STM32_TIM15SEL  | STM32_TIM1SEL   | STM32_LPTIM2SEL |
               STM32_LPTIM1SEL | STM32_I2S1SEL   | STM32_I2C1SEL   |
               STM32_CECSEL    | STM32_USART2SEL | STM32_USART1SEL |
               STM32_LPUART1SEL;

  /* Set flash WS's for SYSCLK source.*/
  FLASH->ACR = FLASH_ACR_DBG_SWEN | FLASH_ACR_ICEN | FLASH_ACR_PRFTEN |
               STM32_FLASHBITS;
  while ((FLASH->ACR & FLASH_ACR_LATENCY_Msk) !=
         (STM32_FLASHBITS & FLASH_ACR_LATENCY_Msk)) {
  }

  /* Switching to the configured SYSCLK source if it is different from HSI16.*/
#if STM32_SW != STM32_SW_HSISYS
  RCC->CFGR |= STM32_SW;        /* Switches on the selected clock source.   */
  /* Wait until SYSCLK is stable.*/
  while ((RCC->CFGR & RCC_CFGR_SWS) != (STM32_SW << 3))
    ;
#endif

#endif /* STM32_NO_INIT */
}

/** @} */
