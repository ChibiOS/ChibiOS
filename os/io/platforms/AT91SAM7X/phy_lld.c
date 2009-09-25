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
 * @file templates/mac_lld.c
 * @brief MAC Driver subsystem low level driver source template
 * @addtogroup MAC_LLD
 * @{
 */

#include <ch.h>
#include <mac.h>
#include <phy.h>

/**
 * @brief Low level PHY initialization.
 */
void phy_lld_init(void) {

}

/**
 * Resets a PHY device.
 *
 * @param[in] macp pointer to the @p MACDriver object
 */
void phy_lld_reset(MACDriver *macp) {

}

/**
 * @brief Reads a PHY register.
 *
 * @param[in] macp pointer to the @p MACDriver object
 * @param addr the register address
 * @return The register value.
 */
phyreg_t phy_lld_get(MACDriver *macp, phyaddr_t addr) {

  return 0;
}

/**
 * @brief Writes a PHY register.
 *
 * @param[in] macp pointer to the @p MACDriver object
 * @param addr the register address
 * @param value the new register value
 */
void phy_lld_put(MACDriver *macp, phyaddr_t addr, phyreg_t value) {

}

/** @} */
