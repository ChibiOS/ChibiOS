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
#include <pal.h>
#include <nvic.h>

#include "board.h"
#include "stm32_serial.h"

#define AIRCR_VECTKEY           0x05FA0000

/*
 * Digital I/O ports static configuration as defined in @p board.h.
 */
static const STM32GPIOConfig config =
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
  RCC->CR = RCC_CR_HSITRIM_RESET_BITS | RCC_CR_HSION;
  while (!(RCC->CR & RCC_CR_HSIRDY))
    ;                           // Waits until HSI stable, it should already be.
  // HSE setup.
  RCC->CR |= RCC_CR_HSEON;
  while (!(RCC->CR & RCC_CR_HSERDY))
    ;                           // Waits until HSE stable.
  // PLL setup.
  RCC->CFGR = RCC_CFGR_PLLSRC_HSE_BITS | PLLPREBITS | PLLMULBITS;
  RCC->CR |= RCC_CR_PLLON;
  while (!(RCC->CR & RCC_CR_PLLRDY))
    ;                           // Waits until PLL stable.
  // Clock sources.
  RCC->CFGR |= RCC_CFGR_HPRE_DIV1   | RCC_CFGR_PPRE1_DIV2  |
               RCC_CFGR_PPRE2_DIV2  | RCC_CFGR_ADCPRE_DIV8 |
               RCC_CFGR_MCO_NOCLOCK | USBPREBITS;

  /*
   * Flash setup and final clock selection.
   */
  FLASH->ACR = FLASHBITS;       // Flash wait states depending on clock.
  RCC->CFGR |= RCC_CFGR_SW_PLL; // Switches on the PLL clock.
  while ((RCC->CFGR & RCC_CFGR_SW) != RCC_CFGR_SW_PLL)
    ;

  /*
   * I/O ports initialization as specified in board.h.
   */
  palInit(&config);
}

/*
 * Late initialization code.
 * This initialization is performed after BSS and DATA segments initialization
 * and before invoking the main() function.
 */
void hwinit1(void) {

  /*
   * NVIC/SCB initialization.
   * Note: PRIGROUP 4:0 (4:4).
   */
  SCB->AIRCR = AIRCR_VECTKEY | SCB_AIRCR_PRIGROUP_0 | SCB_AIRCR_PRIGROUP_1;
  NVICSetSystemHandlerPriority(HANDLER_SVCALL, PRIORITY_SVCALL);
  NVICSetSystemHandlerPriority(HANDLER_SYSTICK, PRIORITY_SYSTICK);
  NVICSetSystemHandlerPriority(HANDLER_PENDSV, PRIORITY_PENDSV);

  /*
   * SysTick initialization.
   */
  SysTick->LOAD = SYSCLK / (8000000 / CH_FREQUENCY) - 1;
  SysTick->VAL = 0;
  SysTick->CTRL = SysTick_CTRL_ENABLE | SysTick_CTRL_TICKINT;

  /*
   * Other subsystems initialization.
   */
  serial_init(0xC0, 0xC0, 0xC0);

  /*
   * ChibiOS/RT initialization.
   */
  chSysInit();
}
