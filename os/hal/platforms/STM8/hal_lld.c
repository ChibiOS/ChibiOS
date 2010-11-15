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
 * @file    STM8/hal_lld.c
 * @brief   STM8 HAL subsystem low level driver source.
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
 *
 * @notapi
 */
void hal_lld_init(void) {

#if STM8_CLOCK_SOURCE != CLK_SOURCE_DEFAULT
#if STM8_CLOCK_SOURCE == CLK_SOURCE_HSI
  CLK->ICKR    = 1;                 /* HSIEN */
  while ((CLK->ICKR & 2) == 0)      /* HSIRDY */
    ;
#elif STM8_CLOCK_SOURCE == CLK_SOURCE_LSI
  CLK->ICKR    = 8;                 /* LSIEN */
  while ((CLK->ICKR & 16) == 0)     /* LSIRDY */
    ;
#else /* STM8_CLOCK_SOURCE == CLK_SOURCE_HSE */
  CLK->ECKR    = 1;                 /* HSEEN */
  while ((CLK->ECKR & 2) == 0)      /* HSERDY */
    ;
#endif
#if STM8_CLOCK_SOURCE != CLK_SOURCE_HSI
  /* Switching clock (manual switch mode).*/
  CLK->SWCR    = 0;
  CLK->SWR     = STM8_CLOCK_SOURCE;
  while ((CLK->SWCR & 8) == 0)      /* SWIF */
    ;
  CLK->SWCR    = 2;                 /* SWEN */
#endif
  /* Setting up clock dividers.*/
  CLK->CKDIVR  = (STM8_HSI_DIVIDER << 3) | (STM8_CPU_DIVIDER << 0);

  /* Clocks initially all disabled.*/
  CLK->PCKENR1 = 0;
  CLK->PCKENR2 = 0;

  /* Other clock related initializations.*/
  CLK->CSSR    = 0;
  CLK->CCOR    = 0;
  CLK->CANCCR  = 0;
#endif /* STM8_CLOCK_SOURCE != CLK_SOURCE_DEFAULT */
}

/** @} */
