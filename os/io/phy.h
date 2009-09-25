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
 * @file phy.h
 * @brief PHY Driver macros and structures.
 * @addtogroup PHY
 * @{
 */

#ifndef _PHY_H_
#define _PHY_H_

#include "mac_lld.h"
#include "phy_lld.h"

/**
 * @brief PHY Driver initialization.
 */
#define phyInit() phy_lld_init()

/**
 * Resets a PHY device.
 *
 * @param[in] macp pointer to the @p MACDriver object
 */
#define phyReset(macp) phy_lld_reset(macp)

/**
 * @brief Reads a PHY register.
 *
 * @param[in] macp pointer to the @p MACDriver object
 * @param addr the register address
 * @return The register value.
 */
#define phyGet(macp, addr) phy_lld_get(macp, addr)

/**
 * @brief Writes a PHY register.
 *
 * @param[in] macp pointer to the @p MACDriver object
 * @param addr the register address
 * @param value the new register value
 */
#define phyPut(macp, addr, value) phy_lld_put(macp, addr, value)

#endif /* _PHY_H_ */

/** @} */
