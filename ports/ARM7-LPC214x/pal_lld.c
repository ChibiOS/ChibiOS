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
 * @file ports/ARM7-LPC214x/pal_lld.c
 * @brief LPC214x FIO low level driver code
 * @addtogroup LPC214x_PAL
 * @{
 */

#include <ch.h>
#include <pal.h>

/**
 * @brief LPC214x I/O ports configuration.
 * @details Ports A-D clocks enabled, AFIO clock enabled.
 *
 * @param[in] config the LPC214x ports configuration
 */
void _pal_lld_init(const LPC214xFIOConfig *config) {
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
 * @note @p PAL_MODE_UNCONNECTED is implemented as push pull output.
 * @note Writing on pads programmed as pull-up or pull-down has the side
 *       effect to modify the resistor setting because the output latched data
 *       is used for the resistor selection.
 */
void _pal_lld_setgroupmode(ioportid_t port,
                           ioportmask_t mask,
                           uint_fast8_t mode) {
}

/** @} */
