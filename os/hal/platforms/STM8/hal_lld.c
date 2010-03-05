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

/**
 * @file    STM8/hal_lld.c
 * @brief   STM8 HAL subsystem low level driver source.
 *
 * @addtogroup STM8_HAL
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

/**
 * @brief PAL setup.
 * @details Digital I/O ports static configuration as defined in @p board.h.
 */
const STM8GPIOConfig pal_default_config =
{
  {
    {VAL_GPIOAODR, 0, VAL_GPIOADDR, VAL_GPIOACR1, VAL_GPIOACR2},
    {VAL_GPIOBODR, 0, VAL_GPIOBDDR, VAL_GPIOBCR1, VAL_GPIOBCR2},
    {VAL_GPIOCODR, 0, VAL_GPIOCDDR, VAL_GPIOCCR1, VAL_GPIOCCR2},
    {VAL_GPIODODR, 0, VAL_GPIODDDR, VAL_GPIODCR1, VAL_GPIODCR2},
    {VAL_GPIOEODR, 0, VAL_GPIOEDDR, VAL_GPIOECR1, VAL_GPIOECR2},
    {VAL_GPIOFODR, 0, VAL_GPIOFDDR, VAL_GPIOFCR1, VAL_GPIOFCR2},
    {VAL_GPIOGODR, 0, VAL_GPIOGDDR, VAL_GPIOGCR1, VAL_GPIOGCR2},
    {VAL_GPIOHODR, 0, VAL_GPIOHDDR, VAL_GPIOHCR1, VAL_GPIOHCR2},
    {VAL_GPIOIODR, 0, VAL_GPIOIDDR, VAL_GPIOICR1, VAL_GPIOICR2}
  }
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

#if STM8_CLOCK_SOURCE != CLK_SOURCE_DEFAULT
#if STM8_CLOCK_SOURCE == CLK_SOURCE_HSI
  CLK_ICKR    = 1;                  /* HSIEN */
  while ((CLK_ICKR & 2) == 0)       /* HSIRDY */
    ;
#elif STM8_CLOCK_SOURCE == CLK_SOURCE_LSI
  CLK_ICKR    = 8;                  /* LSIEN */             
  while ((CLK_ICKR & 16) == 0)      /* LSIRDY */
    ;
#else /* STM8_CLOCK_SOURCE == CLK_SOURCE_HSE */
  CLK_ECKR    = 1;                  /* HSEEN */
  while ((CLK_ECKR & 2) == 0)       /* HSERDY */
    ;
#endif
#if STM8_CLOCK_SOURCE != CLK_SOURCE_HSI
  /* Switching clock (manual switch mode).*/
  CLK_SWCR    = 0;
  CLK_SWR     = STM8_CLOCK_SOURCE;
  while ((CLK_SWCR & 8) == 0)       /* SWIF */
    ;
  CLK_SWCR    = 2;                  /* SWEN */
#endif
  /* Setting up clock dividers.*/
  CLK_CKDIVR  = (STM8_HSI_DIVIDER << 3) | (STM8_CPU_DIVIDER << 0);

  /* Clocks initially all disabled.*/
  CLK_PCKENR1 = 0;
  CLK_PCKENR2 = 0;
  
  /* Other clock related initializations.*/
  CLK_CSSR    = 0;
  CLK_CCOR    = 0;
  CLK_CANCCR  = 0;
#endif /* STM8_CLOCK_SOURCE != CLK_SOURCE_DEFAULT */
}

/** @} */
