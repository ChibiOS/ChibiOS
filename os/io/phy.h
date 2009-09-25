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
 * @brief Type of a PHY register value.
 */
typedef uint16_t phyreg_t;

/**
 * @brief Type of a PHY register address.
 */
typedef uint8_t phyaddr_t;

#ifdef __cplusplus
extern "C" {
#endif
  void phyInit(void);
  void phyReset(MACDriver *macp);
  void phyStart(MACDriver *macp);
  void phyStop(MACDriver *macp);
  phyreg_t phyGet(MACDriver *macp, phyaddr_t addr);
  void phyPut(MACDriver *macp, phyaddr_t addr, phyreg_t value);
#ifdef __cplusplus
}
#endif

#endif /* _PHY_H_ */

/** @} */
