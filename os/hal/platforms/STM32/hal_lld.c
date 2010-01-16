/*
    ChibiOS/RT - Copyright (C) 2010 Giovanni Di Sirio.

    This file is part of ChibiOS/RT.

    ChibiOS/RT is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 3 of the License, or
    (at your option) any later version.

    ChibiOS/RT is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program. If not, see <http://www.gnu.org/licenses/>.

                                      ---

    A special exception to the GPL can be applied should you wish to distribute
    a combined work that includes ChibiOS/RT, without being obliged to provide
    the source code for any proprietary components. See the file exception.txt
    for full details of how and when the exception can be applied.
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

  /* Note: PRIGROUP 4:0 (4:4).*/
  SCB->AIRCR = AIRCR_VECTKEY | SCB_AIRCR_PRIGROUP_0 | SCB_AIRCR_PRIGROUP_1;
  NVICSetSystemHandlerPriority(HANDLER_SVCALL, PRIORITY_SVCALL);
  NVICSetSystemHandlerPriority(HANDLER_SYSTICK, PRIORITY_SYSTICK);
  NVICSetSystemHandlerPriority(HANDLER_PENDSV, PRIORITY_PENDSV);

  /* Systick initialization.*/
  SysTick->LOAD = SYSCLK / (8000000 / CH_FREQUENCY) - 1;
  SysTick->VAL = 0;
  SysTick->CTRL = SysTick_CTRL_ENABLE | SysTick_CTRL_TICKINT;

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
  /* PLL setup.*/
  RCC->CFGR = RCC_CFGR_PLLSRC | PLLPREBITS | PLLMULBITS;
  RCC->CR |= RCC_CR_PLLON;
  while (!(RCC->CR & RCC_CR_PLLRDY))
    ;                           /* Waits until PLL stable.                  */
  /* Clock sources.*/
  RCC->CFGR |= RCC_CFGR_HPRE_DIV1   | RCC_CFGR_PPRE1_DIV2  |
               RCC_CFGR_PPRE2_DIV2  | RCC_CFGR_ADCPRE_DIV8 |
               RCC_CFGR_MCO_NOCLOCK | USBPREBITS;

  /* Flash setup and final clock selection.   */
  FLASH->ACR = FLASHBITS;       /* Flash wait states depending on clock.    */
  RCC->CFGR |= RCC_CFGR_SW_PLL; /* Switches the PLL clock ON.               */
  while ((RCC->CFGR & RCC_CFGR_SW) != RCC_CFGR_SW_PLL)
    ;
}

/** @} */
