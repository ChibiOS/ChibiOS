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
*/

/**
 * @file    MSP430/hal_lld.c
 * @brief   MSP430 HAL subsystem low level driver source.
 *
 * @addtogroup HAL
 * @{
 */

#include "ch.h"
#include "hal.h"
#include "F5XX_F6XX_CORE_LIB/HAL_PMM.h"
#include "F5XX_F6XX_CORE_LIB/HAL_UCS.h"

#define HZ_TO_KHZ(f_Hz) (f_Hz/1000)

#define SELECT_PMMCOREV(f_Hz)  (f_Hz <=  8000000ul ? PMMCOREV_0 : \
                               (f_Hz <= 12000000ul ? PMMCOREV_1 : \
                               (f_Hz <= 20000000ul ? PMMCOREV_2 : PMMCOREV_3)))

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
  #if MSP430_USE_CLOCK == MSP430_CLOCK_SOURCE_XT2CLK
  #error "XT2CLK is not implemented"
  #elif MSP430_USE_CLOCK == MSP430_CLOCK_SOURCE_DCOCLK
  SetVCore(SELECT_PMMCOREV(DCOCLK));        /* Set core voltage */
  SELECT_FLLREF(SELREF__REFOCLK);           /* Set DCO FLL reference = REFO */
  SELECT_ACLK(SELA__REFOCLK);               /* Set ACLK = REFO */
  Init_FLL_Settle(HZ_TO_KHZ(DCOCLK),DCOCLK/LFXT1CLK);
  #else
  #error "unknown clock source specified"
  #endif
}

/** @} */
