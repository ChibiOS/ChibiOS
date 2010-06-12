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
 * @file LPC214x/pal_lld.c
 * @brief LPC214x FIO low level driver code.
 * @addtogroup LPC214x_PAL
 * @{
 */

#include "ch.h"
#include "hal.h"

#if CH_HAL_USE_PAL || defined(__DOXYGEN__)

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
 * @brief LPC214x I/O ports configuration.
 * @details FIO units and PINSEL registers initialization.
 *
 * @param[in] config the LPC214x ports configuration
 */
void _pal_lld_init(const PALConfig *config) {

  /* Enables the access through the fast registers.*/
  SCS = 3;

  /* I/O pads initial assignment, device drivers may change this setup at a
   * later time.*/
  PINSEL0 = config->pinsel0;
  PINSEL1 = config->pinsel1;
  PINSEL2 = config->pinsel2;

  /* I/O pads direction initial setting.*/
  FIO0Base->FIO_MASK = 0;
  FIO0Base->FIO_PIN = config->P0Data.pin;
  FIO0Base->FIO_DIR = config->P0Data.dir;
  FIO1Base->FIO_MASK = 0;
  FIO1Base->FIO_PIN = config->P1Data.pin;
  FIO1Base->FIO_DIR = config->P1Data.dir;
}

/**
 * @brief Pads mode setup.
 * @details This function programs a pads group belonging to the same port
 *          with the specified mode.
 *
 * @param[in] port the port identifier
 * @param[in] mask the group mask
 * @param[in] mode the mode
 *
 * @note This function is not meant to be invoked directly by the application
 *       code.
 * @note @p PAL_MODE_UNCONNECTED is implemented as push pull output with high
 *       state.
 * @note This function does not alter the @p PINSELx registers. Alternate
 *       functions setup must be handled by device-specific code.
 */
void _pal_lld_setgroupmode(ioportid_t port,
                           ioportmask_t mask,
                           uint_fast8_t mode) {

  switch (mode) {
  case PAL_MODE_RESET:
  case PAL_MODE_INPUT:
    port->FIO_DIR &= ~mask;
    break;
  case PAL_MODE_UNCONNECTED:
    port->FIO_PIN |= mask;
  case PAL_MODE_OUTPUT_PUSHPULL:
    port->FIO_DIR |= mask;
    break;
  }
}

#endif /* CH_HAL_USE_PAL */

/** @} */
