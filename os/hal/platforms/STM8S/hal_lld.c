/*
    ChibiOS/RT - Copyright (C) 2006,2007,2008,2009,2010,
                 2011,2012 Giovanni Di Sirio.

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

/**
 * @file    STM8S/hal_lld.c
 * @brief   STM8S HAL subsystem low level driver source.
 *
 * @addtogroup HAL
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
 * @details Clock sources initialization, HSI is assumed to be already
 *          started after reset.
 * @note    If the @p STM8S_CLOCK_INIT option is set to @p FALSE then the
 *          initialization is not performed and is left to the application.
 *
 * @notapi
 */
void hal_lld_init(void) {

#if !STM8S_NO_CLOCK_INIT
  /* Makes sure that HSI is stable before proceeding.*/
  CLK->ICKR |= CLK_ICKR_HSIRDY;
  while ((CLK->ICKR & CLK_ICKR_HSIRDY) == 0)
    ;

  /* LSI startup and stabilization if required.*/
#if STM8S_LSI_ENABLED
  CLK->ICKR |= CLK_ICKR_LSIEN;
  while ((CLK->ICKR & CLK_ICKR_LSIRDY) == 0)
    ;
#endif

  /* HSE startup and stabilization if required.*/
#if STM8S_HSE_ENABLED
  CLK->ECKR |= CLK_ECKR_HSEEN;
  while ((CLK->ECKR & CLK_ECKR_HSERDY) == 0)
    ;
#endif

  /* Setting up clock dividers.*/
  CLK->CKDIVR  = (STM8S_HSI_DIVIDER << 3) | (STM8S_CPU_DIVIDER << 0);

  /* SYSCLK switch to the selected source, not necessary if it is HSI.*/
#if STM8S_SYSCLK_SOURCE != CLK_SYSSEL_HSI
  /* Switching clock (manual switch mode).*/
  CLK->SWR  = STM8S_SYSCLK_SOURCE;
  while ((CLK->SWCR & CLK_SWCR_SWIF) == 0)
    ;
  CLK->SWCR = CLK_SWCR_SWEN;
#endif

  /* Clocks initially all disabled.*/
  CLK->PCKENR1 = 0;
  CLK->PCKENR2 = 0;

  /* Other clock related initializations.*/
  CLK->CSSR    = 0;
  CLK->CCOR    = 0;
  CLK->CANCCR  = STM8S_CAN_DIVIDER_VALUE;

  /* HSI disabled if it is no more required.*/
#if !STM8S_HSI_ENABLED
  CLK->ICKR &= ~CLK_ICKR_HSIEN;
#endif
#endif /* !STM8S_NO_CLOCK_INIT */
}

/** @} */
