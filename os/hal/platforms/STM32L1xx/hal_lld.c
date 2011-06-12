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
 * @file    STM32L1xx/hal_lld.c
 * @brief   STM32L1xx HAL subsystem low level driver source.
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

  /* Reset of all peripherals.*/
  RCC->APB1RSTR = 0xFFFFFFFF;
  RCC->APB2RSTR = 0xFFFFFFFF;
  RCC->APB1RSTR = 0;
  RCC->APB2RSTR = 0;

  /* SysTick initialization using the system clock.*/
  SysTick->LOAD = STM32_HCLK / CH_FREQUENCY - 1;
  SysTick->VAL = 0;
  SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk |
                  SysTick_CTRL_ENABLE_Msk |
                  SysTick_CTRL_TICKINT_Msk;

#if defined(STM32_DMA_REQUIRED)
  dmaInit();
#endif
}

/**
 * @brief   STM32L1xx clocks and PLL initialization.
 * @note    All the involved constants come from the file @p board.h.
 * @note    This function must be invoked only after the system reset.
 *
 * @special
 */
#if defined(STM32L1XX_MD) || defined(__DOXYGEN__)
/*
 * Clocks initialization for the LD, MD and HD sub-families.
 */
void stm32_clock_init(void) {

}
#else
void stm32_clock_init(void) {}
#endif

/** @} */
