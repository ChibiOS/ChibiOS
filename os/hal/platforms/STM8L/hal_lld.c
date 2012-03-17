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
 * @file    STM8L/hal_lld.c
 * @brief   STM8L HAL subsystem low level driver source.
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
 * @note    If the @p STM8L_CLOCK_INIT option is set to @p FALSE then the
 *          initialization is not performed and is left to the application.
 *
 * @notapi
 */
void hal_lld_init(void) {

#if !STM8L_NO_CLOCK_INIT
  /* Makes sure that HSI is stable before proceeding.*/
  CLK->ICKCR |= CLK_ICKCR_HSION;
  while ((CLK->ICKCR & CLK_ICKCR_HSIRDY) == 0)
    ;

  /* LSI startup and stabilization if required.*/
#if STM8L_LSI_ENABLED
  CLK->ICKCR |= CLK_ICKCR_LSION;
  while ((CLK->ICKCR & CLK_ICKCR_LSIRDY) == 0)
    ;
#endif

  /* HSE startup and stabilization if required.*/
#if STM8L_HSE_ENABLED
#if HSEBYPASS
  CLK->ECKCR |= CLK_ECKCR_HSEON | CLK_ECKCR_HSEBYP;
#else
  CLK->ECKCR |= CLK_ECKCR_HSEON;
#endif
  while ((CLK->ECKCR & CLK_ECKCR_HSERDY) == 0)
    ;
#endif

  /* LSE startup and stabilization if required.*/
#if STM8L_LSE_ENABLED
#if LSEBYPASS
  CLK->ECKCR |= CLK_ECKCR_LSEON | CLK_ECKCR_LSEBYP;
#else
  CLK->ECKCR |= CLK_ECKCR_LSEON;
#endif
  while ((CLK->ECKCR & CLK_ECKCR_LSERDY) == 0)
    ;
#endif

  /* Setting up clock dividers.*/
  CLK->CKDIVR = STM8L_SYSCLK_DIVIDER << 0;

  /* SYSCLK switch to the selected source, not necessary if it is HSI.*/
#if STM8L_SYSCLK_SOURCE != CLK_SYSSEL_HSI
  /* Switching clock (manual switch mode).*/
  CLK->SWR  = STM8L_SYSCLK_SOURCE;
  while ((CLK->SWCR & CLK_SWCR_SWIF) == 0)
    ;
  CLK->SWCR = CLK_SWCR_SWEN;
#endif

  /* Clocks initially all disabled, note the boot ROM clock is disabled
     because the boot loader is no more required and it draws pretious uAs.*/
  CLK->PCKENR1 = 0;
  CLK->PCKENR2 = 0;
  CLK->PCKENR3 = 0;

  /* Other clock related initializations.*/
  CLK->CSSR = 0;
  CLK->CCOR = 0;

  /* HSI disabled if it is no more required.*/
#if !STM8L_HSI_ENABLED
  CLK->ICKCR &= ~CLK_ICKCR_HSION;
#endif
#endif /* !STM8L_NO_CLOCK_INIT */
}

/** @} */
