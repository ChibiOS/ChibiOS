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
 * @file    hal.c
 * @brief   HAL subsystem code.
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
/* Driver exported functions.                                                */
/*===========================================================================*/

/**
 * @brief   HAL initialization.
 *
 * @init
 */
void halInit(void) {

  hal_lld_init();

#if HAL_USE_PAL
  palInit(&pal_default_config);
#endif
#if HAL_USE_ADC
  adcInit();
#endif
#if HAL_USE_CAN
  canInit();
#endif
#if HAL_USE_I2C
  i2cInit();
#endif
#if HAL_USE_MAC
  macInit();
#endif
#if HAL_USE_PWM
  pwmInit();
#endif
#if HAL_USE_SERIAL
  sdInit();
#endif
#if HAL_USE_SPI
  spiInit();
#endif
#if HAL_USE_MMC_SPI
  mmcInit();
#endif
#if HAL_USE_UART
  uartInit();
#endif
}

/** @} */
