/*
    ChibiOS/RT - Copyright (C) 2009 Giovanni Di Sirio.

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

                                      ---

    A special exception to the GPL can be applied should you wish to distribute
    a combined work that includes ChibiOS/RT, without being obliged to provide
    the source code for any proprietary components. See the file exception.txt
    for full details of how and when the exception can be applied.
*/

#include <ch.h>
#include <nvic.h>

#include "board.h"
#include "stm32_serial.h"

/*
 * Early initialization code.
 * This initialization is performed just after reset before BSS and DATA
 * segments initialization.
 */
void hwinit0(void) {

  /*
   * Clocks and PLL initialization.
   */
  // HSI setup.
  RCC->CR = HSITRIM_RESET_BITS | CR_HSION_MASK;
  while (!(RCC->CR & CR_HSIRDY_MASK))
    ;                           // Waits until HSI stable, it should already be.
  // HSE setup.
  RCC->CR |= CR_HSEON_MASK;
  while (!(RCC->CR & CR_HSERDY_MASK))
    ;                           // Waits until HSE stable.
  // PLL setup.
  RCC->CFGR = PLLSRC_HSE_BITS | PLLPREBITS | PLLMULBITS;
  RCC->CR |= CR_PLLON_MASK;
  while (!(RCC->CR & CR_PLLRDY_MASK))
    ;                           // Waits until PLL stable.
  // Clock sources.
  RCC->CFGR |= HPRE_DIV1_BITS | PPRE1_DIV2_BITS | PPRE2_DIV2_BITS |
               ADCPRE_DIV8_BITS | USBPREBITS | MCO_DISABLED_BITS;

  /*
   * Flash setup and final clock selection.
   */
  FLASH->ACR = FLASHBITS;       // Flash wait states depending on clock.
  RCC->CFGR |= SW_PLL_BITS;     // Switches on the PLL clock.
  while ((RCC->CFGR & CFGR_SWS_MASK) != SWS_PLL_BITS)
    ;

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
}

/*
 * Late initialization code.
 * This initialization is performed after BSS and DATA segments initialization
 * and before invoking the main() function.
 */
void hwinit1(void) {

  /*
   * NVIC/SCB initialization.
   */
  SCB_AIRCR = AIRCR_VECTKEY | AIRCR_PRIGROUP(0x3); // PRIGROUP 4:0 (4:4).
  SCB_SHPR(2) = 0xF0 << 16;     // PendSV at lowest priority.

  /*
   * SysTick initialization.
   */
  SCB_SHPR(2) |= 0x40 << 24;    // SysTick at priority 4:0.
  ST_RVR = SYSCLK / (8000000 / CH_FREQUENCY) - 1;
  ST_CVR = 0;
  ST_CSR = ENABLE_ON_BITS | TICKINT_ENABLED_BITS | CLKSOURCE_EXT_BITS;

  /*
   * Other subsystems initialization.
   */
  InitSerial(0x80, 0x80, 0x80);

  /*
   * ChibiOS/RT initialization.
   */
  chSysInit();
}
