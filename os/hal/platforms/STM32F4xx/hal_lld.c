/*
    ChibiOS/RT - Copyright (C) 2006,2007,2008,2009,2010,
                 2011 Giovanni Di Sirio.

    This file is part of ChibiOS/RT.

    ChibiOS/RT is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 3 of the License, or
    (at your option) any later version.

    ChibiOS/RT is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

/**
 * @file    STM32F4xx/hal_lld.c
 * @brief   STM32F4xx HAL subsystem low level driver source.
 *
 * @addtogroup HAL
 * @{
 */

#include "ch.h"
#include "hal.h"

#define AIRCR_VECTKEY           0x05FA0000

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local variables.                                                   */
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

  /* Reset of all peripherals. AHB3 is not reseted because it could have
     been initialized in the board initialization file (board.c).*/
  rccResetAHB1(!0);
  rccResetAHB2(!0);
  rccResetAHB3(!0);
  rccResetAPB1(!RCC_APB1RSTR_PWRRST);
  rccResetAPB2(!0);

  /* SysTick initialization using the system clock.*/
  SysTick->LOAD = STM32_HCLK / CH_FREQUENCY - 1;
  SysTick->VAL = 0;
  SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk |
                  SysTick_CTRL_ENABLE_Msk |
                  SysTick_CTRL_TICKINT_Msk;

#if STM32_PVD_ENABLE
  /* Power voltage detector initialization */
  PWR->CR |= PWR_CR_PVDE;
  PWR->CR |= STM32_PLS & STM32_PLS_MASK;
#endif /* STM32_PVD_ENABLE */

#if defined(STM32_DMA_REQUIRED)
  dmaInit();
#endif
}

/**
 * @brief   STM32F2xx clocks and PLL initialization.
 * @note    All the involved constants come from the file @p board.h.
 * @note    This function should be invoked just after the system reset.
 *
 * @special
 */
void stm32_clock_init(void) {

#if !STM32_NO_INIT
  /* PWR clock enable.*/
  RCC->APB1ENR = RCC_APB1ENR_PWREN;

  /* Initial clocks setup and wait for HSI stabilization, the MSI clock is
     always enabled because it is the fallback clock when PLL the fails.*/
  RCC->CR |= RCC_CR_HSION;
  while ((RCC->CR & RCC_CR_HSIRDY) == 0)
    ;                           /* Waits until HSI is stable.               */

#if STM32_HSE_ENABLED
  /* HSE activation.*/
  RCC->CR |= RCC_CR_HSEON;
  while ((RCC->CR & RCC_CR_HSERDY) == 0)
    ;                           /* Waits until HSE is stable.               */
#endif

#if STM32_LSI_ENABLED
  /* LSI activation.*/
  RCC->CSR |= RCC_CSR_LSION;
  while ((RCC->CSR & RCC_CSR_LSIRDY) == 0)
    ;                           /* Waits until LSI is stable.               */
#endif

#if STM32_LSE_ENABLED
  /* LSE activation, have to unlock the register.*/
  if ((RCC->BDCR & RCC_BDCR_LSEON) == 0) {
    PWR->CR |= PWR_CR_DBP;
    RCC->BDCR |= RCC_BDCR_LSEON;
    PWR->CR &= ~PWR_CR_DBP;
  }
  while ((RCC->BDCR & RCC_BDCR_LSERDY) == 0)
    ;                           /* Waits until LSE is stable.               */
#endif

#if STM32_ACTIVATE_PLL
  /* PLL activation.*/
  RCC->PLLCFGR = STM32_PLLQ | STM32_PLLSRC | STM32_PLLP | STM32_PLLN | STM32_PLLM;
  RCC->CR |= RCC_CR_PLLON;
  while (!(RCC->CR & RCC_CR_PLLRDY))
    ;                           /* Waits until PLL is stable.               */
#endif

#if STM32_ACTIVATE_PLLI2S
  /* PLLI2S activation.*/
  RCC->PLLI2SCFGR = STM32_PLLI2SR_VALUE | STM32_PLLI2SN_VALUE;
  RCC->CR |= RCC_CR_PLLI2SON;
  while (!(RCC->CR & RCC_CR_PLLI2SRDY))
    ;                           /* Waits until PLLI2S is stable.            */
#endif

  /* Other clock-related settings (dividers, MCO etc).*/
  RCC->CFGR |= STM32_MCO2PRE | STM32_MCO2SEL | STM32_MCO1PRE | STM32_MCO1SEL |
               STM32_RTCPRE | STM32_PPRE2 | STM32_PPRE1 | STM32_HPRE;

  /* Flash setup.*/
  FLASH->ACR = FLASH_ACR_PRFTEN | FLASH_ACR_ICEN | FLASH_ACR_DCEN |
               STM32_FLASHBITS;

  /* Switching to the configured clock source if it is different from MSI.*/
#if (STM32_SW != STM32_SW_HSI)
  RCC->CFGR |= STM32_SW;        /* Switches on the selected clock source.   */
  while ((RCC->CFGR & RCC_CFGR_SWS) != (STM32_SW << 2))
    ;
#endif
#endif /* STM32_NO_INIT */

  /* SYSCFG clock enabled here because it is a multi-functional unit shared
     among multiple drivers.*/
  rccEnableAPB2(RCC_APB2ENR_SYSCFGEN, TRUE);
}

/** @} */
