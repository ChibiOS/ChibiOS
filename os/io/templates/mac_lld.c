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
#include <mac_lld.h>

/**
 * @brief Low level MAC initialization.
 */
void mac_lld_init(void) {

}

/**
 * @brief Low level MAC address setup.
 *
 * @param[in] p pointer to a six bytes buffer containing the MAC address. If
 *            this parameter is set to @p NULL then a system default MAC is
 *            used.
 *
 * @note This function should be invoked after the @p macInit() and before
 *       @p macStart() else the result is unspecified (performed or ignored).
 */
void mac_lld_set_address(uint8_t *p) {

}

/**
 * @brief Starts the I/O activity and enters a low power mode.
 */
void mac_lld_start(void) {

}

/**
 * @brief Stops the I/O activity.
 */
void mac_lld_stop(void) {

}

/**
 * @brief Allocates a transmission descriptor.
 * @details One of the available transmission descriptors is locked and
 *          returned.
 *
 * @return A pointer to a @p MACTransmitDescriptor structure or @p NULL if
 *         a descriptor is not available or the driver went in stop mode.
 */
MACTransmitDescriptor *max_lld_get_transmit_descriptor(void) {

  return NULL;
}

/**
 * @brief Releases a transmit descriptor and starts the transmission of the
 *        enqueued data as a single frame.
 *
 * @param[in] tdp the pointer to the @p MACTransmitDescriptor structure
 */
void mac_lld_release_transmit_descriptor(MACTransmitDescriptor *tdp) {

}

/**
 * @brief Enqueues data to a @p MACTransmitDescriptor.
 *
 * @param[in] tdp the pointer to the @p MACTransmitDescriptor structure
 * @param buf pointer to the data buffer
 * @param size size of the data to be enqueued
 */
void mac_lld_add_transmit_data(MACTransmitDescriptor *tdp,
                               uint8_t *buf,
                               size_t size) {

}

/** @} */
