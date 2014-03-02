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

/*
  This file has been contributed by:
                Marcin Jokel.
*/

/**
 * @file    LPC17xx/mac_lld.h
 * @brief   LPC17xx low level MAC driver header.
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
#define MAC_SUPPORTS_ZERO_COPY              TRUE

/**
 * @name     MAC Configuration Register 1 bits
 * @{
 */
#define EMAC_MAC1_RECEIVE_EN                (1UL << 0)
#define EMAC_MAC1_PASS_ALL_REC              (1UL << 1)
#define EMAC_MAC1_RX_FLOW_CTRL              (1UL << 2)
#define EMAC_MAC1_TX_FLOW_CTRL              (1UL << 3)
#define EMAC_MAC1_LOOPBACK                  (1UL << 4)
#define EMAC_MAC1_RESET_TX                  (1UL << 8)
#define EMAC_MAC1_RESET_MCS_TX              (1UL << 9)
#define EMAC_MAC1_RESET_RX                  (1UL << 10)
#define EMAC_MAC1_RESET_MCS_RX              (1UL << 11)
#define EMAC_MAC1_SIM_RESET                 (1UL << 14)
#define EMAC_MAC1_SOFT_RESET                (1UL << 15)

/**
 * @name     MAC Configuration Register 2 bits
 * @{
 */
#define EMAC_MAC2_FULL_DUPLEX               (1UL << 0)
#define EMAC_MAC2_FRAME_LEN_CHECK           (1UL << 1)
#define EMAC_MAC2_HUGE_FRAME_EN             (1UL << 2)
#define EMAC_MAC2_DELAYED_CRC               (1UL << 3)
#define EMAC_MAC2_CRC_EN                    (1UL << 4)
#define EMAC_MAC2_PAD_CRC_EN                (1UL << 5)
#define EMAC_MAC2_VLAN_PAN_EN               (1UL << 6)
#define EMAC_MAC2_AUTO_DET_PAD_EN           (1UL << 7)
#define EMAC_MAC2_PURE_PREA_ENF             (1UL << 8)
#define EMAC_MAC2_LONG_PRE_ENF              (1UL << 9)
#define EMAC_MAC2_NO_BACKOFF                (1UL << 12)
#define EMAC_MAC2_BACK_PRESSURE             (1UL << 13)
#define EMAC_MAC2_EXCESS_DEFFER             (1UL << 14)
/** @} */

/**
 * @name     PHY Support Register bits
 * @{
 */
#define EMAC_SUPP_SPEED                     (1UL << 8)
/** @} */

/**
 * @name     Test Register bits
 * @{
 */
#define EMAC_TEST_SHORTCUT_PAUSE_QUANTA     (1UL << 0)
#define EMAC_TEST_PAUSE                     (1UL << 1)
#define EMAC_TEST_BACKPRESSURE              (1UL << 2)
/** @} */

/**
 * @name     Test Register bits
 * @{
 */
#define EMAC_TEST_SHORTCUT_PAUSE_QUANTA     (1UL << 0)
#define EMAC_TEST_PAUSE                     (1UL << 1)
#define EMAC_TEST_BACKPRESSURE              (1UL << 2)
/** @} */

/**
 * @name     MII Mgmt Configuration Register bits
 * @{
 */
#define EMAC_MCFG_SCAN_INCR                 (1UL << 0)
#define EMAC_MCFG_SUPPRESS_PREAMBLE         (1UL << 1)
#define EMAC_MCFG_CLK_DIV_4                 (1UL << 2)
#define EMAC_MCFG_CLK_DIV_6                 (2UL << 2)
#define EMAC_MCFG_CLK_DIV_8                 (3UL << 2)
#define EMAC_MCFG_CLK_DIV_10                (4UL << 2)
#define EMAC_MCFG_CLK_DIV_14                (5UL << 2)
#define EMAC_MCFG_CLK_DIV_20                (6UL << 2)
#define EMAC_MCFG_CLK_DIV_28                (7UL << 2)
#define EMAC_MCFG_CLK_DIV_36                (8UL << 2)
#define EMAC_MCFG_CLK_DIV_40                (9UL << 2)
#define EMAC_MCFG_CLK_DIV_44                (10UL << 2)
#define EMAC_MCFG_CLK_DIV_48                (11UL << 2)
#define EMAC_MCFG_CLK_DIV_52                (12UL << 2)
#define EMAC_MCFG_CLK_DIV_56                (13UL << 2)
#define EMAC_MCFG_CLK_DIV_60                (14UL << 2)
#define EMAC_MCFG_CLK_DIV_64                (15UL << 2)
#define EMAC_MCFG_RESET_MII_MGMT            (1UL << 15)
/** @} */


/**
 * @name     MII Mgmt Command Register bits
 * @{
 */
#define EMAC_MCMD_READ                      (1UL << 0)
#define EMAC_MCMD_SCAN                      (1UL << 1)
/** @} */

/**
 * @name    MIND MII Mgmt Indicators Register bits
 * @{
 */
#define EMAC_MIND_BUSY                      (1UL << 0)
#define EMAC_MIND_SCANNING                  (1UL << 1)
#define EMAC_MIND_NOT_VALID                 (1UL << 2)
#define EMAC_MIND_LINK_FAIL                 (1UL << 3)
/** @} */

/**
 * @name    Command Register bits
 * @{
 */
#define EMAC_COMMAND_RX_ENABLE              (1UL << 0)
#define EMAC_COMMAND_TX_ENABLE              (1UL << 1)
#define EMAC_COMMAND_REG_RESET              (1UL << 3)
#define EMAC_COMMAND_TX_RESET               (1UL << 4)
#define EMAC_COMMAND_RX_RESET               (1UL << 5)
#define EMAC_COMMAND_PASS_RUN_FRAME         (1UL << 6)
#define EMAC_COMMAND_PASS_RX_FILTER         (1UL << 7)
#define EMAC_COMMAND_PASS_TX_FLOW_CTRL      (1UL << 8)
#define EMAC_COMMAND_RMII                   (1UL << 9)
#define EMAC_COMMAND_FULL_DUPLEX            (1UL << 10)
/** @} */

/**
 * @name    Status Register bits
 * @{
 */
#define EMAC_STATUS_RX                      (1UL << 0)
#define EMAC_STATUS_TX                      (1UL << 1)
/** @} */

/**
 * @name    Transmit Status Vector 0 Register bits
 * @{
 */
#define EMAC_TSV0_CRC_ERROR                 (1UL << 0)
#define EMAC_TSV0_LEN_CHECK_ERROR           (1UL << 1)
#define EMAC_TSV0_LEN_OUT_OF_RANGE          (1UL << 2)
#define EMAC_TSV0_DONE                      (1UL << 3)
#define EMAC_TSV0_MULTICAST                 (1UL << 4)
#define EMAC_TSV0_BROADCAST                 (1UL << 5)
#define EMAC_TSV0_PACKET_DEFER              (1UL << 6)
#define EMAC_TSV0_EXCESSIVE_DEFER           (1UL << 7)
#define EMAC_TSV0_EXCESSIVE_COLLISION       (1UL << 8)
#define EMAC_TSV0_LATE_COLLISION            (1UL << 9)
#define EMAC_TSV0_GIANT                     (1UL << 10)
#define EMAC_TSV0_UNDERRUN                  (1UL << 11)
#define EMAC_TSV0_CONTROL_FRAME             (1UL << 28)
#define EMAC_TSV0_PAUSE                     (1UL << 29)
#define EMAC_TSV0_BACKPRESSURE              (1UL << 30)
#define EMAC_TSV0_VLAN                      (1UL << 31)
/** @} */

/**
 * @name    Receive Status Vector Register bits
 * @{
 */
#define EMAC_RSV_PACKET_PREV_IGN            (1UL << 16)
#define EMAC_RSV_RXDV_EVENT                 (1UL << 17)
#define EMAC_RSV_CARRIER_EVENT              (1UL << 18)
#define EMAC_RSV_RECEIVE_CODE               (1UL << 19)
#define EMAC_RSV_CRC_ERROR                  (1UL << 20)
#define EMAC_RSV_LEN_CHECK_ERROR            (1UL << 21)
#define EMAC_RSV_LEN_OUT_OF_RANGE           (1UL << 22)
#define EMAC_RSV_RECEIVE_OK                 (1UL << 23)
#define EMAC_RSV_MULTICAST                  (1UL << 24)
#define EMAC_RSV_BROADCAST                  (1UL << 25)
#define EMAC_RSV_DRIBBLE_NIBBLE             (1UL << 26)
#define EMAC_RSV_CTRL_FRAME                 (1UL << 27)
#define EMAC_RSV_PAUSE                      (1UL << 28)
#define EMAC_RSV_UNSUPPORTED_OPCODE         (1UL << 29)
#define EMAC_RSV_VLAN                       (1UL << 30)
/** @} */

/**
 * @name    Receive Filter Control Register bits
 * @{
 */
#define EMAC_RXFILCTRL_UNICAST_EN           (1UL << 0)
#define EMAC_RXFILCTRL_BROADCAST_EN         (1UL << 1)
#define EMAC_RXFILCTRL_MULTICAST_EN         (1UL << 2)
#define EMAC_RXFILCTRL_UNICAST_HASH_EN      (1UL << 3)
#define EMAC_RXFILCTRL_MULTICAST_HASH_EN    (1UL << 4)
#define EMAC_RXFILCTRL_PERFECT_EN           (1UL << 5)
#define EMAC_RXFILCTRL_MAGIC_PACKET_EN      (1UL << 12)
#define EMAC_RXFILCTRL_RX_FILTER_EN         (1UL << 13)
/** @} */

/**
 * @name    Receive Filter WoL Status bits
 * @{
 */
#define EMAC_RXFILWOLSTAT_UNICAST           (1UL << 0)
#define EMAC_RXFILWOLSTAT_BROADCAST         (1UL << 1)
#define EMAC_RXFILWOLSTAT_MULTICAST         (1UL << 2)
#define EMAC_RXFILWOLSTAT_UNICAST_HASH      (1UL << 3)
#define EMAC_RXFILWOLSTAT_MULTICAST_HASH    (1UL << 4)
#define EMAC_RXFILWOLSTAT_PERFECT           (1UL << 5)
#define EMAC_RXFILWOLSTAT_RX_FILTER         (1UL << 7)
#define EMAC_RXFILWOLSTAT_MAGIC_PACKET      (1UL << 8)
/** @} */

/**
 * @name    Receive Filter WoL Clear bits
 * @{
 */
#define EMAC_RXFILWOLCLR_UNICAST            (1UL << 0)
#define EMAC_RXFILWOLCLR_BROADCAST          (1UL << 1)
#define EMAC_RXFILWOLCLR_MULTICAST          (1UL << 2)
#define EMAC_RXFILWOLCLR_UNICAST_HASH       (1UL << 3)
#define EMAC_RXFILWOLCLR_MULTICAST_HASH     (1UL << 4)
#define EMAC_RXFILWOLCLR_PERFECT            (1UL << 5)
#define EMAC_RXFILWOLCLR_RX_FILTER          (1UL << 7)
#define EMAC_RXFILWOLCLR_MAGIC_PACKET       (1UL << 8)
/** @} */

/**
 * @name    Interrupt Status Register bits
 * @{
 */
#define EMAC_INTSTAT_RX_OVERRUN             (1UL << 0)
#define EMAC_INTSTAT_RX_ERROR               (1UL << 1)
#define EMAC_INTSTAT_RX_FINISHED            (1UL << 2)
#define EMAC_INTSTAT_RX_DONE                (1UL << 3)
#define EMAC_INTSTAT_TX_UNDERRUN            (1UL << 4)
#define EMAC_INTSTAT_TX_ERROR               (1UL << 5)
#define EMAC_INTSTAT_TX_FINISHED            (1UL << 6)
#define EMAC_INTSTAT_TX_DONE                (1UL << 7)
#define EMAC_INTSTAT_SOFT                   (1UL << 12)
#define EMAC_INTSTAT_WAKEUP                 (1UL << 13)
/** @} */

/**
 * @name    Interrupt Enable Register bits
 * @{
 */
#define EMAC_INTEN_RX_OVERRUN               (1UL << 0)
#define EMAC_INTEN_RX_ERROR                 (1UL << 1)
#define EMAC_INTEN_RX_FINISHED              (1UL << 2)
#define EMAC_INTEN_RX_DONE                  (1UL << 3)
#define EMAC_INTEN_TX_UNDERRUN              (1UL << 4)
#define EMAC_INTEN_TX_ERROR                 (1UL << 5)
#define EMAC_INTEN_TX_FINISHED              (1UL << 6)
#define EMAC_INTEN_TX_DONE                  (1UL << 7)
#define EMAC_INTEN_SOFT                     (1UL << 12)
#define EMAC_INTEN_WAKEUP                   (1UL << 13)
/** @} */

/**
 * @name    Interrupt Clear Register bits
 * @{
 */
#define EMAC_INTCLR_RX_OVERRUN              (1UL << 0)
#define EMAC_INTCLR_RX_ERROR                (1UL << 1)
#define EMAC_INTCLR_RX_FINISHED             (1UL << 2)
#define EMAC_INTCLR_RX_DONE                 (1UL << 3)
#define EMAC_INTCLR_TX_UNDERRUN             (1UL << 4)
#define EMAC_INTCLR_TX_ERROR                 (1UL << 5)
#define EMAC_INTCLR_TX_FINISHED             (1UL << 6)
#define EMAC_INTCLR_TX_DONE                 (1UL << 7)
#define EMAC_INTCLR_SOFT                    (1UL << 12)
#define EMAC_INTCLR_WAKEUP                  (1UL << 13)
/** @} */

/**
 * @name    Interrupt Set Register bits
 * @{
 */
#define EMAC_INTSET_RX_OVERRUN              (1UL << 0)
#define EMAC_INTSET_RX_ERROR                (1UL << 1)
#define EMAC_INTSET_RX_FINISHED             (1UL << 2)
#define EMAC_INTSET_RX_DONE                 (1UL << 3)
#define EMAC_INTSET_TX_UNDERRUN             (1UL << 4)
#define EMAC_INTSET_TX_ERROR                (1UL << 5)
#define EMAC_INTSET_TX_FINISHED             (1UL << 6)
#define EMAC_INTSET_TX_DONE                 (1UL << 7)
#define EMAC_INTSET_SOFT                    (1UL << 12)
#define EMAC_INTSET_WAKEUP                  (1UL << 13)
/** @} */

/**
 * @name     Power-Down Register bits
 * @{
 */
#define EMAC_PWRDOWN_MAC_AHB                (1UL << 31)
/** @} */

/**
 * @name     Receive Descriptor Control bits
 * @{
 */
#define EMAC_RXSTATUS_CTRL_SIZE_MASK        0x000007FF
#define EMAC_RXSTATUS_CTRL_LOCK             (1UL << 11)
#define EMAC_RXSTATUS_CTRL_READY            (1UL << 12)
#define EMAC_RXSTATUS_RELEASE               (1UL << 12)
#define EMAC_RXSTATUS_CTRL_INT              (1UL << 31)
/** @} */

/**
 * @name     Receive Descriptor Status bits
 * @{
 */
#define EMAC_RXSTATUS_INFO_SIZE_MASK        0x000007FF
#define EMAC_RXSTATUS_INFO_CTRL_FRAME       (1UL << 18)
#define EMAC_RXSTATUS_INFO_VLAN             (1UL << 19)
#define EMAC_RXSTATUS_INFO_FAIL_FILTER      (1UL << 20)
#define EMAC_RXSTATUS_INFO_MULTICAST        (1UL << 21)
#define EMAC_RXSTATUS_INFO_BROADCAST        (1UL << 22)
#define EMAC_RXSTATUS_INFO_CRC_ERROR        (1UL << 23)
#define EMAC_RXSTATUS_INFO_SYMBOL_ERROR     (1UL << 24)
#define EMAC_RXSTATUS_INFO_LENGTH_ERROR     (1UL << 25)
#define EMAC_RXSTATUS_INFO_RANGE_ERROR      (1UL << 26)
#define EMAC_RXSTATUS_INFO_ALLIG_ERROR      (1UL << 27)
#define EMAC_RXSTATUS_INFO_OVERRUN          (1UL << 28)
#define EMAC_RXSTATUS_INFO_NO_DESCRIPTOR    (1UL << 29)
#define EMAC_RXSTATUS_INFO_LAST_FLAG        (1UL << 30)
#define EMAC_RXSTATUS_INFO_ERROR            (1UL << 31)
/** @} */

/**
 * @name     Transmit Descriptor Control bits
 * @{
 */
#define EMAC_TXSTATUS_CTRL_SIZE_MASK        0x000007FF
#define EMAC_TXSTATUS_CTRL_LOCK             (1UL << 11)
#define EMAC_TXSTATUS_CTRL_OVERRIDE         (1UL << 26)
#define EMAC_TXSTATUS_CTRL_HUGE             (1UL << 27)
#define EMAC_TXSTATUS_CTRL_PAD              (1UL << 28)
#define EMAC_TXSTATUS_CTRL_CRC              (1UL << 29)
#define EMAC_TXSTATUS_CTRL_LAST             (1UL << 30)
#define EMAC_TXSTATUS_CTRL_INT              (1UL << 31)
/** @} */

/**
 * @name     Transmit Descriptor Status bits
 * @{
 */
#define EMAC_RXSTATUS_INFO_COL_COUNT_MASK   (0x0FUL << 21)
#define EMAC_RXSTATUS_INFO_DEFER            (1UL << 25)
#define EMAC_RXSTATUS_INFO_EXC_DEFER        (1UL << 26)
#define EMAC_RXSTATUS_INFO_EXC_COL          (1UL << 27)
#define EMAC_RXSTATUS_INFO_LATE_COL         (1UL << 28)
#define EMAC_RXSTATUS_INFO_UNDERRUN         (1UL << 29)
#define EMAC_RXSTATUS_INFO_NO_DESC          (1UL << 30)
#define EMAC_RXSTATUS_INFO_ERROR            (1UL << 31)
/** @} */

/**
 * @brief Collision Window / Retry register default value
 */
#define LPC17XX_MAC_CLRT_DEF                0x0000370F

/**
 * @brief Non Back-to-Back Inter-Packet-Gap register default value
 */
#define LPC17XX_MAC_IPGR_DEF                0x00000015

/**
 * @brief Back-to-Back Inter-Packet-Gap register default value
 */
#define LPC17XX_MAC_IPGT_FULL_DUPLEX_DEF    0x00000015
#define LPC17XX_MAC_IPGT_HALF_DUPLEX_DEF    0x00000012

/** @} */
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
#if !defined(LPC17xx_MAC_TRANSMIT_BUFFERS) || defined(__DOXYGEN__)
#define LPC17xx_MAC_TRANSMIT_BUFFERS          2
#endif

/**
 * @brief   Number of available receive buffers.
 */
#if !defined(LPC17xx_MAC_RECEIVE_BUFFERS) || defined(__DOXYGEN__)
#define LPC17xx_MAC_RECEIVE_BUFFERS           4
#endif

/**
 * @brief   Maximum supported frame size.
 */
#if !defined(LPC17xx_MAC_BUFFERS_SIZE) || defined(__DOXYGEN__)
#define LPC17xx_MAC_BUFFERS_SIZE              1522
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
#if !defined(LPC17xx_MAC_PHY_TIMEOUT) || defined(__DOXYGEN__)
#define LPC17xx_MAC_PHY_TIMEOUT               100
#endif

/**
 * @brief   Change the PHY power state inside the driver.
 */
#if !defined(LPC17xx_MAC_ETH1_CHANGE_PHY_STATE) || defined(__DOXYGEN__)
#define LPC17xx_MAC_ETH1_CHANGE_PHY_STATE     TRUE
#endif

/**
 * @brief   ETHD1 interrupt priority level setting.
 */
#if !defined(LPC17xx_MAC_ETH_IRQ_PRIORITY) || defined(__DOXYGEN__)
#define LPC17xx_MAC_ETH_IRQ_PRIORITY         13
#endif

/**
 * @brief   MAC number of ticks of delay after reset.
 */
#if !defined(LPC17XX_MAC_RESET_DELAY) || defined(__DOXYGEN__)
#define LPC17XX_MAC_RESET_DELAY               100
#endif

/**
 * @brief   MAC maximum frame length.
 */
#if !defined(LPC17XX_MAC_MAX_FLEN) || defined(__DOXYGEN__)
#define LPC17XX_MAC_MAX_FLEN                  1536
#endif
/** @} */

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

#if (LPC17xx_MAC_PHY_TIMEOUT > 0) && !HAL_IMPLEMENTS_COUNTERS
#error "LPC17xx_MAC_PHY_TIMEOUT requires the realtime counter service"
#endif

/*===========================================================================*/
/* Driver data structures and types.                                         */
/*===========================================================================*/

/**
 * @brief   Type of an LPC17xx Ethernet receive descriptor.
 */
typedef struct {
  volatile uint32_t     packet;
  volatile uint32_t     control;
} lpc17xx_eth_rx_descriptor_t;

/**
 * @brief   Type of an LPC17xx Ethernet transmit descriptor.
 */
typedef struct {
  volatile uint32_t     packet;
  volatile uint32_t     control;
} lpc17xx_eth_tx_descriptor_t;

/**
 * @brief   Type of an LPC17xx Ethernet receive status.
 */
typedef struct {
  volatile uint32_t     info;
  volatile uint32_t     hashcrc;
} lpc17xx_eth_rx_status_t;

/**
 * @brief   Type of an LPC17xx Ethernet transmit status.
 */
typedef volatile uint32_t lpc17xx_eth_tx_status_t;

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
   * @brief PHY address (pre shifted).
   */
  uint32_t phyaddr;
  /**
   * @brief Software receive descriptor number.
   */
  uint32_t rxsoftindex;
  /**
   * @brief Software transmit descriptor number.
   */
  uint32_t txsoftindex;

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
   * @brief Physical descriptor number.
   */
  uint32_t                  txdescn;
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
   * @brief Physical descriptor number.
   */
  uint32_t              rxdescn;
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

  uint32_t mii_read(MACDriver *macp, uint32_t reg);
  void mii_write(MACDriver *macp, uint32_t reg, uint32_t value);
  void mii_find_phy(MACDriver *macp);
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
