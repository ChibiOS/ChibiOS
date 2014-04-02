/*
    ChibiOS/RT - Copyright (C) 2006-2013 Giovanni Di Sirio

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

        http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.
*/

/**
 * @file    RX62N/mac_lld.h
 * @brief   RX62N low level MAC driver header.
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

/**
 * @brief   This implementation supports the zero-copy mode API.
 */
#define MAC_SUPPORTS_ZERO_COPY              FALSE

/**
 * @brief   Every receive buffer must by aligned on 32-bytes boundary
 *          if multiple buffers used.
 */
#define EDMAC_RECEIVE_BUFFERS_SIZE          (256)    /* Do not modify */

/**
 * @brief   Every transmit buffer must by aligned on 32-bytes boundary
 *          if multiple buffers used.
 */
#define EDMAC_TRANSMIT_BUFFERS_SIZE                                           \
    (((RX62N_MAC_BUFFERS_SIZE - 1) | (32 - 1)) + 1)

#define EDMAC_RECEIVE_DESCRIPTORS                                             \
    (((((RX62N_MAC_BUFFERS_SIZE - 1) | (EDMAC_RECEIVE_BUFFERS_SIZE - 1)) + 1) \
      / EDMAC_RECEIVE_BUFFERS_SIZE) * RX62N_MAC_RECEIVE_BUFFERS)

#define EDMAC_TRANSMIT_DESCRIPTORS          RX62N_MAC_TRANSMIT_BUFFERS

/**
 * @name    RD0 constants
 * @{
 */
#define RX62N_RD0_RFS_MASK                  ((1<<27)-1)
#define RX62N_RD0_RFE                       (1<<27)
#define RX62N_RD0_RFP_COF                   (0<<28)
#define RX62N_RD0_RFP_EOF                   (1<<28)
#define RX62N_RD0_RFP_SOF                   (2<<28)
#define RX62N_RD0_RFP_ONE                   (3<<28)
#define RX62N_RD0_RFP_MASK                  (3<<28)
#define RX62N_RD0_RDLE                      (1<<30)
#define RX62N_RD0_RACT                      (1<<31)
/** @} */

/**
 * @name    RD1 constants
 * @{
 */
#define RX62N_RD1_RBL(x)                    (x>>16)
#define RX62N_RD1_RFL(x)                    (x&0xFFFF)
/** @} */

/**
 * @name    TD0 constants
 * @{
 */
#define RX62N_TD0_TLOCKED                   (1<<9) /* Not an EDMAC flag.*/
#define RX62N_TD0_TFS_MASK                  ((1<<26)-1)
#define RX62N_TD0_TWBI                      (1<<26)
#define RX62N_TD0_TFE                       (1<<27)
#define RX62N_TD0_TFP_COF                   (0<<28)
#define RX62N_TD0_TFP_EOF                   (1<<28)
#define RX62N_TD0_TFP_SOF                   (2<<28)
#define RX62N_TD0_TFP_ONE                   (3<<28)
#define RX62N_TD0_TDLE                      (1<<30)
#define RX62N_TD0_TACT                      (1<<31)
/** @} */

/**
 * @name    TDES1 constants
 * @{
 */
/** @} */

#define ETHERC_ECMR_RE                      (1<<6)
#define ETHERC_ECMR_TE                      (1<<5)
#define ETHERC_ECMR_ILB                     (1<<3)
#define ETHERC_ECMR_RTM                     (1<<2)
#define ETHERC_ECMR_DM                      (1<<1)
#define ETHERC_ECMR_PRM                     (1<<0)

/*===========================================================================*/
/* Driver pre-compile time settings.                                         */
/*===========================================================================*/

/**
 * @name    Configuration options
 * @{
 */
/**
 * @brief   Number of available transmit buffers.
 */
#if !defined(RX62N_MAC_TRANSMIT_BUFFERS) || defined(__DOXYGEN__)
#define RX62N_MAC_TRANSMIT_BUFFERS          2
#endif

/**
 * @brief   Number of available receive buffers.
 */
#if !defined(RX62N_MAC_RECEIVE_BUFFERS) || defined(__DOXYGEN__)
#define RX62N_MAC_RECEIVE_BUFFERS           2
#endif

/**
 * @brief   Maximum supported frame size.
 */
#if !defined(RX62N_MAC_BUFFERS_SIZE) || defined(__DOXYGEN__)
#define RX62N_MAC_BUFFERS_SIZE              1536
#endif


/**
 * @brief   PHY detection timeout.
 * @details Timeout, in milliseconds, for PHY address detection, if a PHY
 *          is not detected within the timeout then the driver halts during
 *          initialization. This setting applies only if the PHY address is
 *          not explicitly set in the board header file using
 *          @p BOARD_PHY_ADDRESS. A zero value disables the timeout and a
 *          single search path is performed.
 */
#if !defined(RX62N_MAC_PHY_TIMEOUT) || defined(__DOXYGEN__)
#define RX62N_MAC_PHY_TIMEOUT               100
#endif

/**
 * @brief   Change the PHY power state inside the driver.
 */
#if !defined(RX62N_MAC_ETH1_CHANGE_PHY_STATE) || defined(__DOXYGEN__)
#define RX62N_MAC_ETH1_CHANGE_PHY_STATE     TRUE
#endif

/**
 * @brief   ETHD1 interrupt priority level setting.
 */
#if !defined(RX62N_MAC_ETH1_IRQ_PRIORITY) || defined(__DOXYGEN__)
#define RX62N_MAC_ETH1_IRQ_PRIORITY         5
#endif
/** @} */

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

#if (RX62N_MAC_PHY_TIMEOUT > 0) && !HAL_IMPLEMENTS_COUNTERS
#error "RX62N_MAC_PHY_TIMEOUT requires the realtime counter service"
#endif

/*===========================================================================*/
/* Driver data structures and types.                                         */
/*===========================================================================*/

/**
 * @brief   Type of an RX62N Ethernet receive descriptor.
 */
typedef struct {
  volatile uint32_t     rd0;
  volatile uint32_t     rd1;
  volatile uint32_t     rd2;
  volatile uint32_t     pad;
} rx62n_eth_rx_descriptor_t;

/**
 * @brief   Type of an RX62N Ethernet transmit descriptor.
 */
typedef struct {
  volatile uint32_t     td0;
  volatile uint32_t     td1;
  volatile uint32_t     td2;
  volatile uint32_t     pad;
} rx62n_eth_tx_descriptor_t;

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
  /**
   * @brief PHY address.
   */
  uint8_t phyaddr;
  /**
   * @brief Receive next frame pointer.
   */
  rx62n_eth_rx_descriptor_t *rxptr;
  /**
   * @brief Transmit next frame pointer.
   */
  rx62n_eth_tx_descriptor_t *txptr;
};

/**
 * @brief   Structure representing a transmit descriptor.
 */
typedef struct {
  /**
   * @brief Current write offset.
   */
  size_t                    offset;
  /**
   * @brief Available space size.
   */
  size_t                    size;
  /* End of the mandatory fields.*/
  /**
   * @brief Pointer to the physical descriptor.
   */
  rx62n_eth_tx_descriptor_t *physdesc;
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
   * @brief Pointer to the physical descriptor.
   */
  rx62n_eth_rx_descriptor_t *physdesc;
} MACReceiveDescriptor;

/*===========================================================================*/
/* Driver macros.                                                            */
/*===========================================================================*/

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#if !defined(__DOXYGEN__)
extern MACDriver ETHD1;
#endif

#ifdef __cplusplus
extern "C" {
#endif
  void mac_lld_init(void);
  void mac_lld_start(MACDriver *macp);
  void mac_lld_stop(MACDriver *macp);
  msg_t mac_lld_get_transmit_descriptor(MACDriver *macp,
                                        MACTransmitDescriptor *tdp);
  void mac_lld_release_transmit_descriptor(MACTransmitDescriptor *tdp);
  msg_t mac_lld_get_receive_descriptor(MACDriver *macp,
                                       MACReceiveDescriptor *rdp);
  void mac_lld_release_receive_descriptor(MACReceiveDescriptor *rdp);
  bool_t mac_lld_poll_link_status(MACDriver *macp);
  size_t mac_lld_write_transmit_descriptor(MACTransmitDescriptor *tdp,
                                           uint8_t *buf,
                                           size_t size);
  size_t mac_lld_read_receive_descriptor(MACReceiveDescriptor *rdp,
                                         uint8_t *buf,
                                         size_t size);
#if MAC_USE_ZERO_COPY
  uint8_t *mac_lld_get_next_transmit_buffer(MACTransmitDescriptor *tdp,
                                            size_t size,
                                            size_t *sizep);
  const uint8_t *mac_lld_get_next_receive_buffer(MACReceiveDescriptor *rdp,
                                                 size_t *sizep);
#endif /* MAC_USE_ZERO_COPY */
#ifdef __cplusplus
}
#endif

#endif /* HAL_USE_MAC */

#endif /* _MAC_LLD_H_ */

/** @} */
