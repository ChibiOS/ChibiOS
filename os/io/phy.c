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
 * @file phy.c
 * @brief PHY Driver code.
 * @addtogroup PHY
 * @{
 */

#include <ch.h>
#include <mac.h>
#include <phy.h>

/**
 * @brief Interface status.
 */
static enum {ifStopped = 0, ifStarted} state;

/**
 * @brief PHY Driver initialization.
 */
void phyInit(void) {

  state = ifStopped;
}

/**
 * Initializes a PHY device.
 *
 * @param[in] macp pointer to the @p MACDriver object
 */
void phyReset(MACDriver *macp) {

}

/**
 * @brief Puts the PHY device in active mode.
 *
 * @param[in] macp pointer to the @p MACDriver object
 */
void phyStart(MACDriver *macp) {

  chSysLock();
  if (state == ifStarted) {
    chSysUnlock();
    return;
  }
  state = ifStarted;
  chSysUnlock();
  phy_lld_start(macp);
}

/**
 * @brief Puts the PHY device in a low power mode.
 *
 * @param[in] macp pointer to the @p MACDriver object
 */
void phyStop(MACDriver *macp) {

  chSysLock();
  if (state == ifStopped) {
    chSysUnlock();
    return;
  }
  state = ifStopped;
  chSysUnlock();
  phy_lld_stop(macp);
}

/**
 * @brief Reads a PHY register.
 *
 * @param[in] macp pointer to the @p MACDriver object
 * @param addr the register address
 * @return The register value.
 */
phyreg_t phyGet(MACDriver *macp, phyaddr_t addr) {

}

/**
 * @brief Writes a PHY register.
 *
 * @param[in] macp pointer to the @p MACDriver object
 * @param addr the register address
 * @param value the new register value
 */
void phyPut(MACDriver *macp, phyaddr_t addr, phyreg_t value) {

}

/** @} */
