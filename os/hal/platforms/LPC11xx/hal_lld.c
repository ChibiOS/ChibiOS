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
 * @file    LPC11xx/hal_lld.c
 * @brief   LPC11xx HAL subsystem low level driver source.
 *
 * @addtogroup LPC11xx_HAL
 * @{
 */

#include "ch.h"
#include "hal.h"

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
 */
void hal_lld_init(void) {

  /* Note: PRIGROUP 2:0 (2:6).*/
//  SCB->AIRCR = AIRCR_VECTKEY | SCB_AIRCR_PRIGROUP_0 | SCB_AIRCR_PRIGROUP_1;
  NVICSetSystemHandlerPriority(HANDLER_SVCALL, CORTEX_PRIORITY_SVCALL);
  NVICSetSystemHandlerPriority(HANDLER_SYSTICK, CORTEX_PRIORITY_SYSTICK);
  NVICSetSystemHandlerPriority(HANDLER_PENDSV, CORTEX_PRIORITY_PENDSV);

  /* Systick initialization.*/
  SysTick->LOAD = LPC11xx_SYSCLK / (8000000 / CH_FREQUENCY) - 1;
  SysTick->VAL = 0;
  SysTick->CTRL = SysTick_CTRL_ENABLE_Msk | SysTick_CTRL_TICKINT_Msk;
}

/**
 * @brief   LPC111x clocks and PLL initialization.
 * @note    All the involved constants come from the file @p board.h.
 */
void lpc111x_clock_init(void) {
}

/** @} */
