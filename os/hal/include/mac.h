/*
    ChibiOS/RT - Copyright (C) 2006,2007,2008,2009,2010,
                 2011 Giovanni Di Sirio.

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
 * @file    mac.h
 * @brief   MAC Driver macros and structures.
 * @addtogroup MAC
 * @{
 */

#ifndef _MAC_H_
#define _MAC_H_

#if HAL_USE_MAC || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver constants.                                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Driver pre-compile time settings.                                         */
/*===========================================================================*/

/**
 * @name    MAC configuration options
 * @{
 */
/**
 * @brief   Enables an event sources for incoming packets.
 */
#if !defined(MAC_USE_EVENTS) || defined(__DOXYGEN__)
#define MAC_USE_EVENTS          TRUE
#endif
/** @} */

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

#if !CH_USE_SEMAPHORES || !CH_USE_EVENTS
#error "the MAC driver requires CH_USE_SEMAPHORES"
#endif

#if MAC_USE_EVENTS && !CH_USE_EVENTS
#error "the MAC driver requires CH_USE_EVENTS"
#endif

/*===========================================================================*/
/* Driver data structures and types.                                         */
/*===========================================================================*/

/**
 * @brief   Driver state machine possible states.
 */
typedef enum {
  MAC_UNINIT = 0,                   /**< Not initialized.                   */
  MAC_STOP = 1,                     /**< Stopped.                           */
  MAC_ACTIVE = 2,                   /**< Active.                            */
} macstate_t;

/**
 * @brief   Type of a structure representing a MAC driver.
 */
typedef struct MACDriver MACDriver;

#include "mac_lld.h"

/*===========================================================================*/
/* Driver macros.                                                            */
/*===========================================================================*/

/**
 * @name    Macro Functions
 * @{
 */
/**
 * @brief   Returns the received frames event source.
 *
 * @param[in] macp      pointer to the @p MACDriver object
 * @return              The pointer to the @p EventSource structure.
 *
 * @api
 */
#if MAC_USE_EVENTS || defined(__DOXYGEN__)
#define macGetReceiveEventSource(macp)  (&(macp)->rdevent)
#endif

/**
 * @brief   Writes to a transmit descriptor's stream.
 *
 * @param[in] tdp       pointer to a @p MACTransmitDescriptor structure
 * @param[in] buf       pointer to the buffer containing the data to be written
 * @param[in] size      number of bytes to be written
 * @return              The number of bytes written into the descriptor's
 *                      stream, this value can be less than the amount
 *                      specified in the parameter @p size if the maximum frame
 *                      size is reached.
 *
 * @api
 */
#define macWriteTransmitDescriptor(tdp, buf, size)                          \
    mac_lld_write_transmit_descriptor(tdp, buf, size)

/**
 * @brief   Reads from a receive descriptor's stream.
 *
 * @param[in] rdp   pointer to a @p MACReceiveDescriptor structure
 * @param[in] buf   pointer to the buffer that will receive the read data
 * @param[in] size  number of bytes to be read
 * @return          The number of bytes read from the descriptor's stream, this
 *                  value can be less than the amount specified in the
 *                  parameter @p size if there are no more bytes to read.
 *
 * @api
 */
#define macReadReceiveDescriptor(rdp, buf, size)                            \
    mac_lld_read_receive_descriptor(rdp, buf, size)
/** @} */

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#ifdef __cplusplus
extern "C" {
#endif
  void macInit(void);
  void macObjectInit(MACDriver *macp);
  void macStart(MACDriver *macp, const MACConfig *config);
  void macStop(MACDriver *macp);
  void macSetAddress(MACDriver *macp, const uint8_t *p);
  msg_t macWaitTransmitDescriptor(MACDriver *macp,
                                  MACTransmitDescriptor *tdp,
                                  systime_t time);
  void macReleaseTransmitDescriptor(MACTransmitDescriptor *tdp);
  msg_t macWaitReceiveDescriptor(MACDriver *macp,
                                 MACReceiveDescriptor *rdp,
                                 systime_t time);
  void macReleaseReceiveDescriptor(MACReceiveDescriptor *rdp);
  bool_t macPollLinkStatus(MACDriver *macp);
#ifdef __cplusplus
}
#endif

#endif /* HAL_USE_MAC */

#endif /* _MAC_H_ */

/** @} */
