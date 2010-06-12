/*
    ChibiOS/RT - Copyright (C) 2006,2007,2008,2009,2010 Giovanni Di Sirio.

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
 * @file    LPC11xx/hal_lld.c
 * @brief   LPC11xx HAL subsystem low level driver source.
 *
 * @addtogroup LPC11xx_HAL
 * @{
 */

#include "ch.h"
#include "hal.h"

/**
 * @brief   Register missing in NXP header file.
 */
#define FLASHCFG (*((volatile uint32_t *)0x4003C010))

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
const PALConfig pal_default_config = {
 {VAL_GPIO0DATA, VAL_GPIO0DIR},
 {VAL_GPIO1DATA, VAL_GPIO1DIR},
 {VAL_GPIO2DATA, VAL_GPIO2DIR},
 {VAL_GPIO3DATA, VAL_GPIO3DIR},
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
 * @brief   Low level HAL driver initialization.
 */
void hal_lld_init(void) {

  /* SysTick initialization using the system clock.*/
  NVICSetSystemHandlerPriority(HANDLER_SYSTICK, CORTEX_PRIORITY_SYSTICK);
  SysTick->LOAD = LPC11xx_SYSCLK / CH_FREQUENCY - 1;
  SysTick->VAL = 0;
  SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk |
                  SysTick_CTRL_ENABLE_Msk |
                  SysTick_CTRL_TICKINT_Msk;
}

/**
 * @brief   LPC11xx clocks and PLL initialization.
 * @note    All the involved constants come from the file @p board.h.
 */
void lpc111x_clock_init(void) {
  unsigned i;

  /* Flash wait states setting, the code takes care to not touch TBD bits.*/
  FLASHCFG = (FLASHCFG & ~3) | LPC11xx_FLASHCFG_FLASHTIM;

  /* System oscillator initialization if required.*/
#if LPC11xx_MAINCLK_SOURCE == SYSMAINCLKSEL_PLLOUT
#if LPC11xx_PLLCLK_SOURCE == SYSPLLCLKSEL_SYSOSC
  LPC_SYSCON->SYSOSCCTRL = LPC11xx_SYSOSCCTRL;
  LPC_SYSCON->PDRUNCFG &= ~(1 << 5);            /* System oscillator ON.    */
  for (i = 0; i < 200; i++)
    __NOP();                                    /* Stabilization delay.     */
#endif /* LPC11xx_PLLCLK_SOURCE == SYSPLLCLKSEL_SYSOSC */

  /* PLL initialization if required.*/
  LPC_SYSCON->SYSPLLCLKSEL = LPC11xx_PLLCLK_SOURCE;
  LPC_SYSCON->SYSPLLCLKUEN = 1;                 /* Really required?         */
  LPC_SYSCON->SYSPLLCLKUEN = 0;
  LPC_SYSCON->SYSPLLCLKUEN = 1;
  LPC_SYSCON->SYSPLLCTRL = LPC11xx_SYSPLLCTRL_MSEL | LPC11xx_SYSPLLCTRL_PSEL;
  LPC_SYSCON->PDRUNCFG &= ~(1 << 7);            /* System PLL ON.           */
  while ((LPC_SYSCON->SYSPLLSTAT & 1) == 0)     /* Wait PLL lock.           */
    ;
#endif /* LPC11xx_MAINCLK_SOURCE == SYSMAINCLKSEL_PLLOUT */

  /* Main clock source selection.*/
  LPC_SYSCON->MAINCLKSEL = LPC11xx_MAINCLK_SOURCE;
  LPC_SYSCON->MAINCLKUEN = 1;                   /* Really required?         */
  LPC_SYSCON->MAINCLKUEN = 0;
  LPC_SYSCON->MAINCLKUEN = 1;
  while ((LPC_SYSCON->MAINCLKUEN & 1) == 0)     /* Wait switch completion.  */
    ;

  /* ABH divider initialization, peripheral clocks are initially disabled,
     the various device drivers will handle their own setup except GPIO and
     IOCON that are left enabled.*/
  LPC_SYSCON->SYSAHBCLKDIV = LPC11xx_SYSABHCLK_DIV;
  LPC_SYSCON->SYSAHBCLKCTRL = 0x0001005F;

  /* Peripheral clock dividers initialization.*/
  LPC_SYSCON->UARTCLKDIV = LPC11xx_UART_PCLK_DIV;

  /* Memory remapping, vectors always in ROM.*/
  LPC_SYSCON->SYSMEMREMAP = 2;
}

/** @} */
