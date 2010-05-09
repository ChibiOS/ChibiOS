/*
    ChibiOS/RT - Copyright (C) 2006,2007,2008,2009,2010 Giovanni Di Sirio.

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
 * @file STM32/hal_lld.c
 * @brief STM32 HAL subsystem low level driver source.
 * @addtogroup STM32_HAL
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

/**
 * @brief PAL setup.
 * @details Digital I/O ports static configuration as defined in @p board.h.
 */
const STM32GPIOConfig pal_default_config =
{
  {VAL_GPIOAODR, VAL_GPIOACRL, VAL_GPIOACRH},
  {VAL_GPIOBODR, VAL_GPIOBCRL, VAL_GPIOBCRH},
  {VAL_GPIOCODR, VAL_GPIOCCRL, VAL_GPIOCCRH},
  {VAL_GPIODODR, VAL_GPIODCRL, VAL_GPIODCRH},
#if !defined(STM32F10X_LD)
  {VAL_GPIOEODR, VAL_GPIOECRL, VAL_GPIOECRH},
#endif
#if defined(STM32F10X_HD)
  {VAL_GPIOFODR, VAL_GPIOFCRL, VAL_GPIOFCRH},
  {VAL_GPIOGODR, VAL_GPIOGCRL, VAL_GPIOGCRH},
#endif
};

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
 * @brief Low level HAL driver initialization.
 */
void hal_lld_init(void) {

  /* SysTick initialization using the system clock.*/
  SysTick->LOAD = STM32_HCLK / CH_FREQUENCY - 1;
  SysTick->VAL = 0;
  SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk |
                  SysTick_CTRL_ENABLE_Msk |
                  SysTick_CTRL_TICKINT_Msk;

#if CH_HAL_USE_ADC || CH_HAL_USE_SPI
  dmaInit();
#endif
}

/**
 * @brief STM32 clocks and PLL initialization.
 * @note All the involved constants come from the file @p board.h.
 */
void stm32_clock_init(void) {

  /* HSI setup.*/
  RCC->CR = 0x00000083;         /* Reset value, HSI ON.                     */
  while (!(RCC->CR & RCC_CR_HSIRDY))
    ;                           /* Waits until HSI stable.                  */
  /* HSE setup.*/
  RCC->CR |= RCC_CR_HSEON;
  while (!(RCC->CR & RCC_CR_HSERDY))
    ;                           /* Waits until HSE stable.                  */
#if STM32_SW == STM32_SW_PLL
  /* PLL setup, only if the PLL is the selected source of the system clock
     else it is left disabled.*/
  RCC->CFGR = ((STM32_PLLMUL - 2) << 18) | STM32_PLLXTPRE | STM32_PLLSRC;
  RCC->CR |= RCC_CR_PLLON;
  while (!(RCC->CR & RCC_CR_PLLRDY))
    ;                           /* Waits until PLL stable.                  */
#endif
  /* Clock settings.*/
  RCC->CFGR = ((STM32_PLLMUL - 2) << 18) | STM32_PLLXTPRE | STM32_PLLSRC |
              STM32_ADCPRE | STM32_PPRE2 | STM32_PPRE1 | STM32_HPRE;

  /* Flash setup and final clock selection.   */
  FLASH->ACR = STM32_FLASHBITS; /* Flash wait states depending on clock.    */
  RCC->CFGR |= STM32_SW;        /* Switches on the clock sources.           */
  while ((RCC->CFGR & RCC_CFGR_SWS) != (STM32_SW << 2))
    ;
}

/** @} */
