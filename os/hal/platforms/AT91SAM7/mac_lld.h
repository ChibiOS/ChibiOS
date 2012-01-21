/*
    ChibiOS/RT - Copyright (C) 2006,2007,2008,2009,2010,
                 2011,2012 Giovanni Di Sirio.

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
 * @file    AT91SAM7/mac_lld.h
 * @brief   AT91SAM7 low level MAC driver header.
 *
 * @addtogroup MAC
 * @{
 */

#ifndef _MAC_LLD_H_
#define _MAC_LLD_H_

#if HAL_USE_MAC || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver constants.                                                         */
/*===========================================================================*/

#define EMAC_RECEIVE_BUFFERS_SIZE       128     /* Do not modify */
#define EMAC_TRANSMIT_BUFFERS_SIZE      MAC_BUFFERS_SIZE
#define EMAC_RECEIVE_DESCRIPTORS                                            \
    (((((MAC_BUFFERS_SIZE - 1) | (EMAC_RECEIVE_BUFFERS_SIZE - 1)) + 1)      \
      / EMAC_RECEIVE_BUFFERS_SIZE) * MAC_RECEIVE_BUFFERS)
#define EMAC_TRANSMIT_DESCRIPTORS       MAC_TRANSMIT_BUFFERS

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

#define W1_T_ADDRESS_MASK       0xFFFFFFFF

#define W2_T_LENGTH_MASK        0x000007FF
#define W2_T_LOCKED             0x00000800 /* Not an EMAC flag.             */
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
/* Driver pre-compile time settings.                                         */
/*===========================================================================*/

/**
 * @brief   Number of available transmit buffers.
 */
#if !defined(MAC_TRANSMIT_BUFFERS) || defined(__DOXYGEN__)
#define MAC_TRANSMIT_BUFFERS        2
#endif

/**
 * @brief   Number of available receive buffers.
 */
#if !defined(MAC_RECEIVE_BUFFERS) || defined(__DOXYGEN__)
#define MAC_RECEIVE_BUFFERS         2
#endif

/**
 * @brief   Maximum supported frame size.
 */
#if !defined(MAC_BUFFERS_SIZE) || defined(__DOXYGEN__)
#define MAC_BUFFERS_SIZE            1518
#endif

/**
 * @brief   Interrupt priority level for the EMAC device.
 */
#if !defined(EMAC_INTERRUPT_PRIORITY) || defined(__DOXYGEN__)
#define EMAC_INTERRUPT_PRIORITY     (AT91C_AIC_PRIOR_HIGHEST - 3)
#endif

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

/*===========================================================================*/
/* Driver data structures and types.                                         */
/*===========================================================================*/

/**
 * @brief   Structure representing a buffer physical descriptor.
 * @note    It represents both descriptor types.
 */
typedef struct {
  uint32_t              w1;
  uint32_t              w2;
} EMACDescriptor;

/**
 * @brief   Driver configuration structure.
 */
typedef struct {
  /**
   * @brief MAC address.
   */
  uint8_t               *mac_address;
  /* End of the mandatory fields.*/
} MACConfig;

/**
 * @brief   Structure representing a MAC driver.
 */
struct MACDriver {
  /**
   * @brief Driver state.
   */
  macstate_t            state;
  /**
   * @brief Current configuration data.
   */
  const MACConfig       *config;
  /**
   * @brief Transmit semaphore.
   */
  Semaphore             tdsem;
  /**
   * @brief Receive semaphore.
   */
  Semaphore             rdsem;
#if MAC_USE_EVENTS || defined(__DOXYGEN__)
  /**
   * @brief Receive event.
   */
  EventSource           rdevent;
#endif
  /* End of the mandatory fields.*/
  /**
   * @brief Link status flag.
   */
  bool_t                link_up;
};

/**
 * @brief   Structure representing a transmit descriptor.
 */
typedef struct {
  /**
   * @brief Current write offset.
   */
  size_t                offset;
  /**
   * @brief Available space size.
   */
  size_t                size;
  /* End of the mandatory fields.*/
  /**
   * @brief Pointer to the physical descriptor.
   */
  EMACDescriptor        *physdesc;
} MACTransmitDescriptor;

/**
 * @brief   Structure representing a receive descriptor.
 */
typedef struct {
  /**
   * @brief Current read offset.
   */
  size_t                offset;
  /**
   * @brief Available data size.
   */
  size_t                size;
  /* End of the mandatory fields.*/
  /**
   * @brief Pointer to the first descriptor of the buffers chain.
   */
  EMACDescriptor        *physdesc;
} MACReceiveDescriptor;

/*===========================================================================*/
/* Driver macros.                                                            */
/*===========================================================================*/

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#if !defined(__DOXYGEN__)
extern MACDriver ETH1;
#endif

#ifdef __cplusplus
extern "C" {
#endif
  void mac_lld_init(void);
  void mac_lld_start(MACDriver *macp);
  void mac_lld_stop(MACDriver *macp);
  msg_t max_lld_get_transmit_descriptor(MACDriver *macp,
                                        MACTransmitDescriptor *tdp);
  size_t mac_lld_write_transmit_descriptor(MACTransmitDescriptor *tdp,
                                           uint8_t *buf,
                                           size_t size);
  void mac_lld_release_transmit_descriptor(MACTransmitDescriptor *tdp);
  msg_t max_lld_get_receive_descriptor(MACDriver *macp,
                                       MACReceiveDescriptor *rdp);
  size_t mac_lld_read_receive_descriptor(MACReceiveDescriptor *rdp,
                                         uint8_t *buf,
                                         size_t size);
  void mac_lld_release_receive_descriptor(MACReceiveDescriptor *rdp);
  bool_t mac_lld_poll_link_status(MACDriver *macp);
#ifdef __cplusplus
}
#endif

#endif /* HAL_USE_MAC */

#endif /* _MAC_LLD_H_ */

/** @} */
