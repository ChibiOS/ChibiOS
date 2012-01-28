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
 * @file    STM8L/pal_lld.c
 * @brief   STM8L GPIO low level driver code.
 *
 * @addtogroup PAL
 * @{
 */

#include "ch.h"
#include "hal.h"

#if HAL_USE_PAL || defined(__DOXYGEN__)

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
 * @brief   Pads mode setup.
 * @details This function programs a pads group belonging to the same port
 *          with the specified mode.
 * @note    @p PAL_MODE_UNCONNECTED is implemented as push pull output at 2MHz.
 *
 * @param[in] port      the port identifier
 * @param[in] mask      the group mask
 * @param[in] mode      the mode
 *
 * @notapi
 */
void _pal_lld_setgroupmode(ioportid_t port,
                           ioportmask_t mask,
                           iomode_t mode) {

  switch (mode) {
  case PAL_MODE_RESET:
  case PAL_MODE_INPUT_PULLUP:
    port->DDR &= ~mask;
    port->CR1 |=  mask;
    port->CR2 &= ~mask;
    break;
  case PAL_MODE_INPUT:
  case PAL_MODE_INPUT_ANALOG:
    port->DDR &= ~mask;
    port->CR1 &= ~mask;
    port->CR2 &= ~mask;
    break;
  case PAL_MODE_UNCONNECTED:
  case PAL_MODE_OUTPUT_PUSHPULL_SLOW:
    port->DDR |=  mask;
    port->CR1 |=  mask;
    port->CR2 &= ~mask;
    break;
  case PAL_MODE_OUTPUT_PUSHPULL:
    port->DDR |=  mask;
    port->CR1 |=  mask;
    port->CR2 |=  mask;
    break;
  case PAL_MODE_OUTPUT_OPENDRAIN_SLOW:
    port->DDR |=  mask;
    port->CR1 &= ~mask;
    port->CR2 &= ~mask;
    break;
  case PAL_MODE_OUTPUT_OPENDRAIN:
    port->DDR |=  mask;
    port->CR1 &= ~mask;
    port->CR2 |=  mask;
    break;
  }
}

#endif /* HAL_USE_PAL */

/** @} */
