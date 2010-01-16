/*
    ChibiOS/RT - Copyright (C) 2010 Giovanni Di Sirio.

    This file is part of ChibiOS/RT.

    ChibiOS/RT is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 3 of the License, or
    (at your option) any later version.

    ChibiOS/RT is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program. If not, see <http://www.gnu.org/licenses/>.

                                      ---

    A special exception to the GPL can be applied should you wish to distribute
    a combined work that includes ChibiOS/RT, without being obliged to provide
    the source code for any proprietary components. See the file exception.txt
    for full details of how and when the exception can be applied.
*/

/**
 * @file mac.c
 * @brief MAC Driver code.
 * @addtogroup MAC
 * @{
 */

#include "ch.h"
#include "hal.h"

#if CH_HAL_USE_MAC || defined(__DOXYGEN__)

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
 * @brief MAC Driver initialization.
 */
void macInit(void) {

  mac_lld_init();
}

/**
 * @brief Initialize the standard part of a @p MACDriver structure.
 *
 * @param[in] macp pointer to the @p MACDriver object
 */
void macObjectInit(MACDriver *macp) {

  chSemInit(&macp->md_tdsem, 0);
  chSemInit(&macp->md_rdsem, 0);
#if CH_USE_EVENTS
  chEvtInit(&macp->md_rdevent);
#endif
}

/**
 * @brief MAC address setup.
 *
 * @param[in] macp pointer to the @p MACDriver object
 * @param[in] p pointer to a six bytes buffer containing the MAC address. If
 *            this parameter is set to @p NULL then a system default MAC is
 *            used.
 *
 * @note This function must be invoked only with the driver in the stopped
 *       state. If invoked on an active interface then it is ignored.
 */
void macSetAddress(MACDriver *macp, const uint8_t *p) {

  mac_lld_set_address(macp, p);
}

/**
 * @brief Allocates a transmission descriptor.
 * @details One of the available transmission descriptors is locked and
 *          returned. If a descriptor is not currently available then the
 *          invoking thread is queued until one is freed.
 *
 * @param[in] macp pointer to the @p MACDriver object
 * @param[out] tdp pointer to a @p MACTransmitDescriptor structure
 * @param[in] time the number of ticks before the operation timeouts,
 *            the following special values are allowed:
 *            - @a TIME_IMMEDIATE immediate timeout.
 *            - @a TIME_INFINITE no timeout.
 *            .
 * @return The operation status.
 * @retval RDY_OK the descriptor was obtained.
 * @retval RDY_TIMEOUT the operation timed out, descriptor not initialized.
 */
msg_t macWaitTransmitDescriptor(MACDriver *macp,
                                MACTransmitDescriptor *tdp,
                                systime_t time) {
  msg_t msg;

  while (((msg = max_lld_get_transmit_descriptor(macp, tdp)) != RDY_OK) &&
         (time > 0)) {
    chSysLock();
    systime_t now = chTimeNow();
    if ((msg = chSemWaitTimeoutS(&macp->md_tdsem, time)) == RDY_TIMEOUT)
      break;
    if (time != TIME_INFINITE)
      time -= (chTimeNow() - now);
    chSysUnlock();
  }
  return msg;
}

/**
 * @brief Releases a transmit descriptor and starts the transmission of the
 *        enqueued data as a single frame.
 *
 * @param[in] tdp the pointer to the @p MACTransmitDescriptor structure
 */
void macReleaseTransmitDescriptor(MACTransmitDescriptor *tdp) {

  mac_lld_release_transmit_descriptor(tdp);
}

/**
 * @brief Waits for a received frame.
 * @details Stops until a frame is received and buffered. If a frame is
 *          not immediately available then the invoking thread is queued
 *          until one is received.
 *
 * @param[in] macp pointer to the @p MACDriver object
 * @param[out] rdp pointer to a @p MACReceiveDescriptor structure
 * @param[in] time the number of ticks before the operation timeouts,
 *            the following special values are allowed:
 *            - @a TIME_IMMEDIATE immediate timeout.
 *            - @a TIME_INFINITE no timeout.
 *            .
 * @return The operation status.
 * @retval RDY_OK the descriptor was obtained.
 * @retval RDY_TIMEOUT the operation timed out, descriptor not initialized.
 */
msg_t macWaitReceiveDescriptor(MACDriver *macp,
                               MACReceiveDescriptor *rdp,
                               systime_t time) {
  msg_t msg;

  while (((msg = max_lld_get_receive_descriptor(macp, rdp)) != RDY_OK) &&
         (time > 0)) {
    chSysLock();
    systime_t now = chTimeNow();
    if ((msg = chSemWaitTimeoutS(&macp->md_rdsem, time)) == RDY_TIMEOUT)
      break;
    if (time != TIME_INFINITE)
      time -= (chTimeNow() - now);
    chSysUnlock();
  }
  return msg;
}

/**
 * @brief Releases a receive descriptor.
 * @details The descriptor and its buffer are made available for more incoming
 *          frames.
 *
 * @param[in] rdp the pointer to the @p MACReceiveDescriptor structure
 */
void macReleaseReceiveDescriptor(MACReceiveDescriptor *rdp) {

  mac_lld_release_receive_descriptor(rdp);
}

/**
 * @brief Updates and returns the link status.
 *
 * @param[in] macp pointer to the @p MACDriver object
 * @return The link status.
 * @retval TRUE if the link is active.
 * @retval FALSE if the link is down.
 */
bool_t macPollLinkStatus(MACDriver *macp) {

  return mac_lld_poll_link_status(macp);
}

#endif /* CH_HAL_USE_MAC */

/** @} */
