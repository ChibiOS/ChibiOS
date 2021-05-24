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

/*===========================================================================*/
/* Driver local variables and types.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

/**
 * @brief   Initializes the backup domain.
 * @note    WARNING! Changing RTC clock source impossible without resetting
 *          of the whole BKP domain.
 */
static void hal_lld_backup_domain_init(void) {

  /* Reset BKP domain if different clock source selected.*/
  if ((RCC->BDCR & STM32_RTCSEL_MASK) != STM32_RTCSEL) {
    /* Backup domain reset.*/
    RCC->BDCR = RCC_BDCR_BDRST;
    RCC->BDCR = 0;
  }

#if STM32_LSE_ENABLED
  /* LSE activation.*/
#if defined(STM32_LSE_BYPASS)
  /* LSE Bypass.*/
  RCC->BDCR |= STM32_LSEDRV | RCC_BDCR_LSEON | RCC_BDCR_LSEBYP;
#else
  /* No LSE Bypass.*/
  RCC->BDCR |= STM32_LSEDRV | RCC_BDCR_LSEON;
#endif
  while ((RCC->BDCR & RCC_BDCR_LSERDY) == 0)
    ;                                       /* Wait until LSE is stable.    */
#endif

#if HAL_USE_RTC
  /* If the backup domain hasn't been initialized yet then proceed with
     initialization.*/
  if ((RCC->BDCR & RCC_BDCR_RTCEN) == 0) {
    /* Selects clock source.*/
    RCC->BDCR |= STM32_RTCSEL;

    /* RTC clock enabled.*/
    RCC->BDCR |= RCC_BDCR_RTCEN;
  }
#endif /* HAL_USE_RTC */

  /* Low speed output mode.*/
  RCC->BDCR |= STM32_LSCOSEL;
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

  /* Initializes the backup domain.*/
  hal_lld_backup_domain_init();

  /* DMA subsystems initialization.*/
#if defined(STM32_DMA_REQUIRED)
  dmaInit();
#endif

  /* IRQ subsystem initialization.*/
  irqInit();

  /* Programmable voltage detector settings.*/
  PWR->CR2 = STM32_PWR_CR2;
}

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

  /* PWR clock enable.*/
#if (HAL_USE_RTC == TRUE) && defined(RCC_APBENR1_RTCAPBEN)
  rccEnableAPB1R1(RCC_APB1ENR1_PWREN | RCC_APB1ENR1_RTCAPBEN, false)
#else
  rccEnableAPB1R1(RCC_APB1ENR1_PWREN, false)
#endif

  /* Core voltage setup.*/
  PWR->CR1 = STM32_VOS;
  while ((PWR->SR2 & PWR_SR2_VOSF) != 0)    /* Wait until regulator is      */
    ;                                       /* stable.                      */

  /* Additional PWR configurations.*/
  PWR->CR2  = STM32_PWR_CR2;
  PWR->CR3  = STM32_PWR_CR3;
  PWR->CR4  = STM32_PWR_CR4;
  PWR->CR5  = STM32_CR5BITS;
  PWR->PUCRA = STM32_PWR_PUCRA;
  PWR->PDCRA = STM32_PWR_PDCRA;
  PWR->PUCRB = STM32_PWR_PUCRB;
  PWR->PDCRB = STM32_PWR_PDCRB;
  PWR->PUCRC = STM32_PWR_PUCRC;
  PWR->PDCRC = STM32_PWR_PDCRC;
  PWR->PUCRD = STM32_PWR_PUCRD;
  PWR->PDCRD = STM32_PWR_PDCRD;
  PWR->PUCRE = STM32_PWR_PUCRE;
  PWR->PDCRE = STM32_PWR_PDCRE;
  PWR->PUCRF = STM32_PWR_PUCRF;
  PWR->PDCRF = STM32_PWR_PDCRF;
  PWR->PUCRG = STM32_PWR_PUCRG;
  PWR->PDCRG = STM32_PWR_PDCRG;

#if STM32_HSI16_ENABLED
  /* HSI activation.*/
  RCC->CR |= RCC_CR_HSION;
  while ((RCC->CR & RCC_CR_HSIRDY) == 0)
    ;                                       /* Wait until HSI16 is stable.  */
#endif

#if STM32_HSI48_ENABLED
  /* HSI activation.*/
  RCC->CRRCR |= RCC_CRRCR_HSI48ON;
  while ((RCC->CRRCR & RCC_CRRCR_HSI48RDY) == 0)
    ;                                       /* Wait until HSI48 is stable.  */
#endif

#if STM32_HSE_ENABLED
#if defined(STM32_HSE_BYPASS)
  /* HSE Bypass.*/
  RCC->CR |= RCC_CR_HSEON | RCC_CR_HSEBYP;
#endif
  /* HSE activation.*/
  RCC->CR |= RCC_CR_HSEON;
  while ((RCC->CR & RCC_CR_HSERDY) == 0)
    ;                                       /* Wait until HSE is stable.    */
#endif

#if STM32_LSI_ENABLED
  /* LSI activation.*/
  RCC->CSR |= RCC_CSR_LSION;
  while ((RCC->CSR & RCC_CSR_LSIRDY) == 0)
    ;                                       /* Wait until LSI is stable.    */
#endif

  /* Backup domain access enabled and left open.*/
  PWR->CR1 |= PWR_CR1_DBP;

#if STM32_LSE_ENABLED
  /* LSE activation.*/
#if defined(STM32_LSE_BYPASS)
  /* LSE Bypass.*/
  RCC->BDCR |= STM32_LSEDRV | RCC_BDCR_LSEON | RCC_BDCR_LSEBYP;
#else
  /* No LSE Bypass.*/
  RCC->BDCR |= STM32_LSEDRV | RCC_BDCR_LSEON;
#endif
  while ((RCC->BDCR & RCC_BDCR_LSERDY) == 0)
    ;                                       /* Wait until LSE is stable.    */
#endif

#if STM32_ACTIVATE_PLL
  /* PLLM and PLLSRC are common to all PLLs.*/
  RCC->PLLCFGR = STM32_PLLPDIV |
                 STM32_PLLR    | STM32_PLLREN  |
                 STM32_PLLQ    | STM32_PLLQEN  |
                 STM32_PLLP    | STM32_PLLPEN  |
                 STM32_PLLN    | STM32_PLLM    |
                 STM32_PLLSRC;
#endif

#if STM32_ACTIVATE_PLL
  /* PLL activation.*/
  RCC->CR |= RCC_CR_PLLON;

  /* Waiting for PLL lock.*/
  while ((RCC->CR & RCC_CR_PLLRDY) == 0)
    ;
#endif

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

  /* Set flash WS's for SYSCLK source */
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

  /* SYSCFG clock enabled here because it is a multi-functional unit shared
     among multiple drivers.*/
  rccEnableAPB2(RCC_APB2ENR_SYSCFGEN, true);
}

/** @} */
