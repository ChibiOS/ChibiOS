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
 * @file mac.h
 * @brief MAC Driver macros and structures.
 * @addtogroup MAC
 * @{
 */

#ifndef _MAC_H_
#define _MAC_H_

#include "mac_lld.h"

/**
 * @brief Returns the buffer associated to a @p MACTransmitDescriptor.
 *
 * @param[in] tdp the pointer to the @p MACTransmitDescriptor structure
 * @return The pointer to the transmit buffer.
 */
#define macGetTransmitBuffer(tdp) mac_lld_get_transmit_buffer(tdp)

/**
 * @brief Returns the buffer associated to a @p MACReceiveDescriptor.
 *
 * @param[in] rdp the pointer to the @p MACReceiveDescriptor structure
 * @return The pointer to the receive buffer.
 */
#define macGetReceiveBuffer(rdp) mac_lld_get_receive_buffer(rdp)

#ifdef __cplusplus
extern "C" {
#endif
  void macInit(void);
  void macSetAddress(uint8_t *p);
  void macStart(void);
  void macStop(void);
  MACTransmissionDescriptor *macWaitTransmitDescriptor(systime_t time);
  void macReleaseTransmitDescriptor(MACTransmitDescriptor *tdp);
  void macAddTransmitData(MACTransmitDescriptor *tdp,
                          uint8_t *buf,
                          size_t size);
#ifdef __cplusplus
}
#endif

#endif /* _MAC_H_ */

/** @} */
