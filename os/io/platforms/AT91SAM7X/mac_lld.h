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
 * @file templates/mac_lld.h
 * @brief MAC Driver subsystem low level driver header template
 * @addtogroup MAC_LLD
 * @{
 */

#ifndef _MAC_LLD_H_
#define _MAC_LLD_H_

/*===========================================================================*/
/* Driver pre-compile time settings.                                         */
/*===========================================================================*/

/**
 * @brief Number of available descriptors/buffers.
 */
#if !defined(MAC_TRANSMIT_DESCRIPTORS) || defined(__DOXYGEN__)
#define MAC_TRANSMIT_DESCRIPTORS        2
#endif

/*===========================================================================*/
/* EMAC specific constants and settings.                                     */
/*===========================================================================*/

#define EMAC_RECEIVE_BUFFERS            24
#define EMAC_RECEIVE_BUFFERS_SIZE       128     /* Do not modify */
#define EMAC_TRANSMIT_BUFFERS           MAC_TRANSMIT_DESCRIPTORS
#define EMAC_TRANSMIT_BUFFERS_SIZE      1518

#define W1_R_OWNERSHIP          0x00000001
#define W1_R_WRAP               0x00000002
#define W1_R_ADDRESS_MASK       0xFFFFFFFC

#define W2_R_LENGTH_MASK        0x00000FFF
#define W2_R_FRAME_START        0x00004000
#define W2_R_FRAME_END          0x00008000
#define W2_R_CFI                0x00010000
#define W2_R_VLAN_PRIO_MASK     0x000E0000
#define W2_R_PRIO_TAG_DETECTED  0x00100000
#define W2_R_VLAN_TAG_DETECTED  0x00200000
#define W2_R_TYPE_ID_MATCH      0x00400000
#define W2_R_ADDR4_MATCH        0x00800000
#define W2_R_ADDR3_MATCH        0x01000000
#define W2_R_ADDR2_MATCH        0x02000000
#define W2_R_ADDR1_MATCH        0x04000000
#define W2_R_RFU1               0x08000000
#define W2_R_ADDR_EXT_MATCH     0x10000000
#define W2_R_UNICAST_MATCH      0x20000000
#define W2_R_MULTICAST_MATCH    0x40000000
#define W2_R_BROADCAST_DETECTED 0x80000000

#define W2_T_LENGTH_MASK        0x000007FF
#define W2_T_LOCKED             0x00000800 /* Not an EMAC flag, used by the driver */
#define W2_T_RFU1               0x00003000
#define W2_T_LAST_BUFFER        0x00008000
#define W2_T_NO_CRC             0x00010000
#define W2_T_RFU2               0x07FE0000
#define W2_T_BUFFERS_EXHAUSTED  0x08000000
#define W2_T_TRANSMIT_UNDERRUN  0x10000000
#define W2_T_RETRY_LIMIT_EXC    0x20000000
#define W2_T_WRAP               0x40000000
#define W2_T_USED               0x80000000

/*===========================================================================*/
/* Driver data structures and types.                                         */
/*===========================================================================*/

/**
 * @brief Structure representing a MAC driver.
 */
typedef struct {
  enum {ifStopped = 0,
        ifStarted}      md_state;       /**< @brief Interface status.*/
  Semaphore             md_tdsem;       /**< Transmit semaphore.*/
  Semaphore             md_rdsem;       /**< Receive semaphore.*/
} MACDriver;

/**
 * @brief Structure representing a transmission descriptor.
 */
typedef struct {

  uint32_t      w1;
  uint32_t      w2;
} MACTransmitDescriptor;

/**
 * @brief Structure representing a receive descriptor.
 */
typedef struct {

  uint32_t      w1;
  uint32_t      w2;
} MACReceiveDescriptor;

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#ifdef __cplusplus
extern "C" {
#endif
  void mac_lld_init(void);
  void mac_lld_set_address(MACDriver *macp, uint8_t *p);
  void mac_lld_start(MACDriver *macp);
  void mac_lld_stop(MACDriver *macp);
  MACTransmitDescriptor *max_lld_get_transmit_descriptor(MACDriver *macp);
  void mac_lld_release_transmit_descriptor(MACDriver *macp,
                                           MACTransmitDescriptor *tdp);
  uint8_t *mac_lld_get_transmit_buffer(MACTransmitDescriptor *tdp);
  MACReceiveDescriptor *max_lld_get_receive_descriptor(MACDriver *macp);
  void mac_lld_release_receive_descriptor(MACDriver *macp,
                                          MACReceiveDescriptor *rdp);
  uint8_t *mac_lld_get_receive_buffer(MACReceiveDescriptor *rdp);
#ifdef __cplusplus
}
#endif

#endif /* _MAC_LLD_H_ */

/** @} */
