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

/**
 * @brief Low level MAC initialization.
 */
void mac_lld_init(void) {

}

/**
 * @brief Low level MAC address setup.
 *
 * @param[in] macp pointer to the @p MACDriver object
 * @param[in] p pointer to a six bytes buffer containing the MAC address. If
 *            this parameter is set to @p NULL then a system default MAC is
 *            used.
 */
void mac_lld_set_address(MACDriver *macp, const uint8_t *p) {

}

/**
 * @brief Returns a transmission descriptor.
 * @details One of the available transmission descriptors is locked and
 *          returned.
 *
 * @param[in] macp pointer to the @p MACDriver object
 * @param[in] size size of the frame to be transmitted
 * @return A pointer to a @p MACTransmitDescriptor structure or @p NULL if
 *         a descriptor is not available.
 */
MACTransmitDescriptor *max_lld_get_transmit_descriptor(MACDriver *macp,
                                                       size_t size) {

  return NULL;
}

/**
 * @brief Releases a transmit descriptor and starts the transmission of the
 *        enqueued data as a single frame.
 *
 * @param[in] macp pointer to the @p MACDriver object
 * @param[in] tdp the pointer to the @p MACTransmitDescriptor structure
 */
void mac_lld_release_transmit_descriptor(MACDriver *macp,
                                         MACTransmitDescriptor *tdp) {

}

/**
 * @brief Returns the buffer associated to a @p MACTransmitDescriptor.
 *
 * @param[in] tdp the pointer to the @p MACTransmitDescriptor structure
 * @return The pointer to the transmit buffer.
 */
uint8_t *mac_lld_get_transmit_buffer(MACTransmitDescriptor *tdp) {

  return NULL;
}

/**
 * @brief Returns a received frame.
 *
 * @param[in] macp pointer to the @p MACDriver object
 * @param[out szp size of the received frame
 * @return A pointer to a @p MACReceiveDescriptor structure or @p NULL if
 *         the operation timed out or some transient error happened.
 */
MACReceiveDescriptor *max_lld_get_receive_descriptor(MACDriver *macp,
                                                     size_t *szp) {

  return NULL;
}

/**
 * @brief Releases a receive descriptor.
 * @details The descriptor and its buffer is made available for more incoming
 *          frames.
 *
 * @param[in] macp pointer to the @p MACDriver object
 * @param[in] rdp the pointer to the @p MACReceiveDescriptor structure
 */
void mac_lld_release_receive_descriptor(MACDriver *macp,
                                        MACReceiveDescriptor *rdp) {

}

/**
 * @brief Returns the buffer associated to a @p MACTransmitDescriptor.
 *
 * @param[in] tdp the pointer to the @p MACTransmitDescriptor structure
 * @return The pointer to the transmit buffer.
 */
uint8_t *mac_lld_get_receive_buffer(MACReceiveDescriptor *rdp) {

  return NULL;
}

/**
 * @brief Updates and returns the link status.
 *
 * @param[in] macp pointer to the @p MACDriver object
 * @return The link status.
 * @retval TRUE if the link is active.
 * @retval FALSE if the link is down.
 */
bool_t mac_lld_poll_link_status(MACDriver *macp) {

  return FALSE;
}

/** @} */
