/*
    ChibiOS/RT - Copyright (C) 2006,2007,2008,2009,2010 Giovanni Di Sirio.

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
 * @file    templates/mac_lld.c
 * @brief   MAC Driver subsystem low level driver source template.
 *
 * @addtogroup MAC
 * @{
 */

#include "ch.h"
#include "hal.h"

#if HAL_USE_MAC || defined(__DOXYGEN__)

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
 * @brief   Low level MAC initialization.
 *
 * @notapi
 */
void mac_lld_init(void) {

}

/**
 * @brief   Low level MAC address setup.
 *
 * @param[in] macp      pointer to the @p MACDriver object
 * @param[in] p         pointer to a six bytes buffer containing the MAC
 *                      address. If this parameter is set to @p NULL then
 *                      a system default MAC is used.
 *
 * @notapi
 */
void mac_lld_set_address(MACDriver *macp, const uint8_t *p) {

}

/**
 * @brief   Returns a transmission descriptor.
 * @details One of the available transmission descriptors is locked and
 *          returned.
 *
 * @param[in] macp      pointer to the @p MACDriver object
 * @param[out] tdp      pointer to a @p MACTransmitDescriptor structure
 * @return              The operation status.
 * @retval RDY_OK       a descriptor was obtained.
 * @retval RDY_TIMEOUT  descriptor not available.
 *
 * @notapi
 */
msg_t max_lld_get_transmit_descriptor(MACDriver *macp,
                                      MACTransmitDescriptor *tdp) {

  return RDY_OK;
}

/**
 * @brief   Writes to a transmit descriptor's stream.
 *
 * @param[in] tdp       pointer to a @p MACTransmitDescriptor structure
 * @param[in] buf       pointer to the buffer containing the data to be
 *                      written
 * @param[in] size      number of bytes to be written
 * @return              The number of bytes written into the descriptor's
 *                      stream, this value can be less than the amount
 *                      specified in the parameter @p size if the maximum
 *                      frame size is reached.
 *
 * @notapi
 */
size_t mac_lld_write_transmit_descriptor(MACTransmitDescriptor *tdp,
                                         uint8_t *buf,
                                         size_t size) {

  return 0;
}

/**
 * @brief   Releases a transmit descriptor and starts the transmission of the
 *          enqueued data as a single frame.
 *
 * @param[in] tdp       pointer to a @p MACTransmitDescriptor structure
 *
 * @notapi
 */
void mac_lld_release_transmit_descriptor(MACTransmitDescriptor *tdp) {

}

/**
 * @brief   Returns a receive descriptor.
 *
 * @param[in] macp      pointer to a @p MACDriver object
 * @param[out] rdp      pointer to a @p MACReceiveDescriptor structure
 * @return              The operation status.
 * @retval RDY_OK       a descriptor was obtained.
 * @retval RDY_TIMEOUT  descriptor not available.
 *
 * @notapi
 */
msg_t max_lld_get_receive_descriptor(MACDriver *macp,
                                     MACReceiveDescriptor *rdp) {

  return RDY_OK;
}

/**
 * @brief   Reads from a receive descriptor's stream.
 *
 * @param[in] rdp   pointer to a @p MACReceiveDescriptor structure
 * @param[in] buf   pointer to a buffer that will receive the read data
 * @param[in] size  number of bytes to be read
 * @return          The number of bytes read from the descriptor's stream,
 *                  this value can be less than the amount specified in
 *                  the parameter @p size if there are no more bytes to read.
 *
 * @notapi
 */
size_t mac_lld_read_receive_descriptor(MACReceiveDescriptor *rdp,
                                         uint8_t *buf,
                                         size_t size) {

  return 0;
}

/**
 * @brief   Releases a receive descriptor.
 * @details The descriptor and its buffer are made available for more incoming
 *          frames.
 *
 * @param[in] rdp       pointer to a @p MACReceiveDescriptor structure
 *
 * @notapi
 */
void mac_lld_release_receive_descriptor(MACReceiveDescriptor *rdp) {

}

/**
 * @brief   Updates and returns the link status.
 *
 * @param[in] macp      pointer to a @p MACDriver object
 * @return              The link status.
 * @retval TRUE         if the link is active.
 * @retval FALSE        if the link is down.
 *
 * @notapi
 */
bool_t mac_lld_poll_link_status(MACDriver *macp) {

  return FALSE;
}

#endif /* HAL_USE_MAC */

/** @} */
