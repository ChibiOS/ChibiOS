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
 * @file templates/phy_lld.h
 * @brief PHY Driver subsystem low level driver header template
 * @addtogroup PHY_LLD
 * @{
 */

#ifndef _PHY_LLD_H_
#define _PHY_LLD_H_

/*===========================================================================*/
/* Driver pre-compile time settings.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Driver data structures and types.                                         */
/*===========================================================================*/

/**
 * @brief Type of a PHY register value.
 */
typedef uint16_t phyreg_t;

/**
 * @brief Type of a PHY register address.
 */
typedef uint8_t phyaddr_t;

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#ifdef __cplusplus
extern "C" {
#endif
  void phy_lld_init(void);
  void phy_lld_reset(MACDriver *macp);
  phyreg_t phy_lld_get(MACDriver *macp, phyaddr_t addr);
  void phy_lld_put(MACDriver *macp, phyaddr_t addr, phyreg_t value);
#ifdef __cplusplus
}
#endif

#endif /* _PHY_LLD_H_ */

/** @} */
