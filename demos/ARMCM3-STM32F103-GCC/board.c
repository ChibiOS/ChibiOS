/*
    ChibiOS/RT - Copyright (C) 2006-2007 Giovanni Di Sirio.

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

#include <ch.h>

#include "board.h"

/*
 * Wait states setting is a function of the system clock. Those are the
 * recommended values, there should not be need to change them.
 */
#if SYSCLK <= 24000000
#define FLASHBITS 0x00000010
#else
#if SYSCLK <= 48000000
#define FLASHBITS 0x00000011
#else
#define FLASHBITS 0x00000012
#endif
#endif

/*
 * Hardware initialization goes here.
 * NOTE: Interrupts are still disabled.
 */
void hwinit(void) {

  /*
   * Clocks and PLL initialization.
   */
  // HSI setup.
  RCC->CR = 0x00000083;         // Enforces a known state (HSI ON).
  while (!(RCC->CR & (1 << 1)))
    ;                           // Waits until HSI stable, it should already be.
  // HSE setup.
  RCC->CR |= (1 << 16);         // HSE ON.
  while (!(RCC->CR & (1 << 17)))
    ;                           // Waits until HSE stable.
  // PLL setup.
  RCC->CFGR |= PLLPREBITS | PLLMULBITS | PLLSRCBITS;
  RCC->CR |= (1 << 24);         // PLL ON.
  while (!(RCC->CR & (1 << 25)))
    ;                           // Waits until PLL stable.
  // Clock sources.
  RCC->CFGR |= AHBBITS | PPRE1BITS | PPRE2BITS | ADCPREBITS |
               USBPREBITS | MCOSRCBITS;

  /*
   * Flash setup and final clock selection.
   */
  FLASH->ACR = FLASHBITS;       // Flash wait states depending on clock.
  RCC->CFGR |= SYSSRCBITS;      // Switches on the PLL clock.

  /*
   * I/O ports initialization as specified in board.h.
   */
  RCC->APB2ENR = 0x0000003D;    // Ports A-D enabled, AFIO enabled.
  GPIOA->CRL = VAL_GPIOACRL;
  GPIOA->CRH = VAL_GPIOACRH;
  GPIOA->ODR = VAL_GPIOAODR;

  GPIOB->CRL = VAL_GPIOBCRL;
  GPIOB->CRH = VAL_GPIOBCRH;
  GPIOB->ODR = VAL_GPIOBODR;

  GPIOC->CRL = VAL_GPIOCCRL;
  GPIOC->CRH = VAL_GPIOCCRH;
  GPIOC->ODR = VAL_GPIOCODR;

  GPIOD->CRL = VAL_GPIODCRL;
  GPIOD->CRH = VAL_GPIODCRH;
  GPIOD->ODR = VAL_GPIODODR;

  /*
   * NVIC/SCB setup.
   */
  SCB->AIRCR = (0x5FA << 16) | (0x5 << 8);      // PRIGROUP = 5 (2:6).
}
