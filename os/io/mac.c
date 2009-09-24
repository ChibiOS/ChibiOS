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
 * @file mac.c
 * @brief MAC Driver code.
 * @addtogroup MAC
 * @{
 */

#include <ch.h>
#include <mac.h>

/**
 * @brief MAC Driver initialization.
 */
void macInit(void) {

  mac_lld_init();
}

/**
 * @brief MAC address setup.
 *
 * @param[in] p pointer to a six bytes buffer containing the MAC address. If
 *            this parameter is set to @p NULL then a system default MAC is
 *            used.
 */
void macSetAddress(uint8_t *p) {

  mac_lld_set_address(p);
}

MACTransmissionDescriptor *macGetTransmissionDescriptor(void) {

}

void macReleaseTransmissionDescriptor(MACTransmissionDescriptor *dp) {

}

void macSetTransmitSequentialData(MACTransmissionDescriptor *dp,
                                  uint8_t *buf,
                                  size_t size) {

}

/** @} */
