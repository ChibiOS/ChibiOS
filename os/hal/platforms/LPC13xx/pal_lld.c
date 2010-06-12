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
 * @file    LPC13xx/pal_lld.c
 * @brief   LPC13xx GPIO low level driver code.
 *
 * @addtogroup LPC13xx_PAL
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
 * @brief LPC13xx I/O ports configuration.
 * @details GPIO unit registers initialization.
 *
 * @param[in] config the LPC13xx ports configuration
 */
void _pal_lld_init(const PALConfig *config) {

  LPC_GPIO0->DIR = config->P0.dir;
  LPC_GPIO1->DIR = config->P1.dir;
  LPC_GPIO2->DIR = config->P2.dir;
  LPC_GPIO3->DIR = config->P3.dir;
  LPC_GPIO0->DATA = config->P0.data;
  LPC_GPIO1->DATA = config->P1.data;
  LPC_GPIO2->DATA = config->P2.data;
  LPC_GPIO3->DATA = config->P3.data;
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

  switch (mode & PAL_MODE_MASK) {
  case PAL_MODE_RESET:
  case PAL_MODE_INPUT:
    port->DIR &= ~mask;
    break;
  case PAL_MODE_UNCONNECTED:
    palSetPort(port, PAL_WHOLE_PORT);
  case PAL_MODE_OUTPUT_PUSHPULL:
    port->DIR |=  mask;
    break;
  }
}

#endif /* CH_HAL_USE_PAL */

/** @} */
