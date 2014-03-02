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
  Ported from ChibiOS STM32 mac_lld driver.
*/

/**
 * @file    LPC43xx/mac_lld.h
 * @brief   LPC43xx low level MAC driver header.
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
#define MAC_SUPPORTS_ZERO_COPY      TRUE

/**
 * @name    MAC Configuration register bits
 * @{
 */
#define ETH_MAC_CONFIG_RE                       (1UL << 2)
#define ETH_MAC_CONFIG_TE                       (1UL << 3)
#define ETH_MAC_CONFIG_DF                       (1UL << 4)
#define ETH_MAC_CONFIG_BL_MIN_N_10              (0UL << 5)
#define ETH_MAC_CONFIG_BL_MIN_N_8               (1UL << 5)
#define ETH_MAC_CONFIG_BL_MIN_N_4               (2UL << 5)
#define ETH_MAC_CONFIG_BL_MIN_N_1               (3UL << 5)
#define ETH_MAC_CONFIG_ACS                      (1UL << 7)
#define ETH_MAC_CONFIG_DR                       (1UL << 9)
#define ETH_MAC_CONFIG_DM                       (1UL << 11)
#define ETH_MAC_CONFIG_LM                       (1UL << 12)
#define ETH_MAC_CONFIG_DO                       (1UL << 13)
#define ETH_MAC_CONFIG_FES                      (1UL << 14)
#define ETH_MAC_CONFIG_PS                       (1UL << 15)
#define ETH_MAC_CONFIG_DCRS                     (1UL << 16)
#define ETH_MAC_CONFIG_IFG_96                   (0UL << 17)
#define ETH_MAC_CONFIG_IFG_88                   (1UL << 17)
#define ETH_MAC_CONFIG_IFG_80                   (2UL << 17)
#define ETH_MAC_CONFIG_IFG_72                   (3UL << 17)
#define ETH_MAC_CONFIG_IFG_64                   (4UL << 17)
#define ETH_MAC_CONFIG_IFG_56                   (5UL << 17)
#define ETH_MAC_CONFIG_IFG_48                   (6UL << 17)
#define ETH_MAC_CONFIG_IFG_40                   (7UL << 17)
#define ETH_MAC_CONFIG_JE                       (1UL << 20)
#define ETH_MAC_CONFIG_JD                       (1UL << 22)
#define ETH_MAC_CONFIG_WD                       (1UL << 23)
/** @} */

/**
 * @name    MAC Frame filter register bits
 * @{
 */
#define ETH_MAC_FRAME_FILTER_PR                 (1UL << 0)
#define ETH_MAC_FRAME_FILTER_HUC                (1UL << 1)
#define ETH_MAC_FRAME_FILTER_HMC                (1UL << 2)
#define ETH_MAC_FRAME_FILTER_DAIF               (1UL << 3)
#define ETH_MAC_FRAME_FILTER_PM                 (1UL << 4)
#define ETH_MAC_FRAME_FILTER_DBF                (1UL << 5)
#define ETH_MAC_FRAME_FILTER_PCF_FILTER_ALL     (1UL << 6)
#define ETH_MAC_FRAME_FILTER_PCF_FOR_ALL        (1UL << 6)
#define ETH_MAC_FRAME_FILTER_PCF_FOR_FAIL_ADDR  (1UL << 6)
#define ETH_MAC_FRAME_FILTER_PCF_FOR_PASS_ADDR  (1UL << 6)
#define ETH_MAC_FRAME_FILTER_HPF                (1UL << 10)
#define ETH_MAC_FRAME_FILTER_RA                 (1UL << 31)
/** @} */

/**
 * @name    MAC MII Address register bits
 * @{
 */
#define ETH_MAC_MII_ADDR_GB                     (1UL << 0)
#define ETH_MAC_MII_ADDR_W                      (1UL << 1)
#define ETH_MAC_MII_ADDR_CR_DIV_42              (0UL << 2)
#define ETH_MAC_MII_ADDR_CR_DIV_62              (1UL << 2)
#define ETH_MAC_MII_ADDR_CR_DIV_16              (2UL << 2)
#define ETH_MAC_MII_ADDR_CR_DIV_26              (3UL << 2)
#define ETH_MAC_MII_ADDR_CR_DIV_102             (4UL << 2)
#define ETH_MAC_MII_ADDR_CR_DIV_124             (5UL << 2)
#define ETH_MAC_MII_ADDR_GR(reg)                ((reg) << 6)
#define ETH_MAC_MII_ADDR_PA(addr)               ((addr) << 11)
/** @} */

/**
 * @name     MAC Flow control register bits
 * @{
 */
#define ETH_MAC_FLOW_CTRL_FCB                   (1UL << 0)
#define ETH_MAC_FLOW_CTRL_TFE                   (1UL << 1)
#define ETH_MAC_FLOW_CTRL_RFE                   (1UL << 2)
#define ETH_MAC_FLOW_CTRL_UP                    (1UL << 3)
#define ETH_MAC_FLOW_CTRL_PLT(t)                ((t) << 4)
#define ETH_MAC_FLOW_CTRL_DZPQ                  (1UL << 7)
#define ETH_MAC_FLOW_CTRL_PT(t)                 ((t) << 16)
/** @} */

/**
 * @name     MAC VLAN tag register bits
 * @{
 */
#define ETH_MAC_VLAN_TAG_VL(t)                  ((t) << 0)
#define ETH_MAC_FLOW_CTRL_ETV                   (1UL << 16)
/** @} */

/**
 * @name     MAC Debug register bits
 * @{
 */
#define ETH_MAC_DEBUG_RXIDLESTAT                (1UL << 0)
#define ETH_MAC_DEBUG_FIFOSTAT0                 (1UL << 1)
#define ETH_MAC_DEBUG_RXFIFOSTAT1               (1UL << 4)
#define ETH_MAC_DEBUG_RXFIFOSTAT                (3UL << 5)
#define ETH_MAC_DEBUG_RXFIFOSTAT_IDLE           (0UL << 5)
#define ETH_MAC_DEBUG_RXFIFOSTAT_RD_D           (1UL << 5)
#define ETH_MAC_DEBUG_RXFIFOSTAT_RD_ST          (2UL << 5)
#define ETH_MAC_DEBUG_RXFIFOSTAT_FLUSH          (3UL << 5)
#define ETH_MAC_DEBUG_RXFIFOLVL                 (3UL << 8)
#define ETH_MAC_DEBUG_RXFIFOLVL_EMPTY           (0UL << 8)
#define ETH_MAC_DEBUG_RXFIFOLVL_BELOW           (1UL << 8)
#define ETH_MAC_DEBUG_RXFIFOLVL_ABOVE           (2UL << 8)
#define ETH_MAC_DEBUG_RXFIFOLVL_FULL            (3UL << 8)
#define ETH_MAC_DEBUG_TXIDLESTAT                (1UL << 16)
#define ETH_MAC_DEBUG_TXSTAT                    (3UL << 17)
#define ETH_MAC_DEBUG_TXSTAT_IDLE               (0UL << 17)
#define ETH_MAC_DEBUG_TXSTAT_WAIT               (1UL << 17)
#define ETH_MAC_DEBUG_TXSTAT_GEN                (2UL << 17)
#define ETH_MAC_DEBUG_TXSTAT_TRAN               (3UL << 17)
#define ETH_MAC_DEBUG_PAUSE                     (1UL << 19)
#define ETH_MAC_DEBUG_TXFIFOSTAT                (3UL << 20)
#define ETH_MAC_DEBUG_TXFIFOSTAT_IDLE           (0UL << 20)
#define ETH_MAC_DEBUG_TXFIFOSTAT_READ           (1UL << 20)
#define ETH_MAC_DEBUG_TXFIFOSTAT_WAIT           (1UL << 20)
#define ETH_MAC_DEBUG_TXFIFOSTAT_WRITE          (1UL << 20)
#define ETH_MAC_DEBUG_TXFIFOSTAT1               (1UL << 22)
#define ETH_MAC_DEBUG_TXFIFOLVL                 (1UL << 24)
#define ETH_MAC_DEBUG_TXFIFOFULL                (1UL << 25)
/** @} */

/**
 * @name      MAC PMT control and status register bits
 * @{
 */
#define ETH_MAC_PMT_CTRL_STAT_PD                (1UL << 0)
#define ETH_MAC_PMT_CTRL_STAT_MPE               (1UL << 1)
#define ETH_MAC_PMT_CTRL_STAT_WFE               (1UL << 2)
#define ETH_MAC_PMT_CTRL_STAT_MPR               (1UL << 5)
#define ETH_MAC_PMT_CTRL_STAT_WFR               (1UL << 6)
#define ETH_MAC_PMT_CTRL_STAT_GU                (1UL << 9)
#define ETH_MAC_PMT_CTRL_STAT_WFFRPR            (1UL << 31)
/** @} */

/**
 * @name       MAC Interrupt status register bits
 * @{
 */
#define ETH_MAC_INTR_PMT                        (1UL << 3)
#define ETH_MAC_INTR_TS                         (1UL << 9)
/** @} */

/**
 * @name       MAC Interrupt mask register bits
 * @{
 */
#define ETH_MAC_INTR_MASK_PMTIM                 (1UL << 3)
#define ETH_MAC_INTR_MASK_TSIM                  (1UL << 9)
/** @} */

/**
 * @name        MAC IEEE1588 time stamp control register bits
 * @{
 */
#define ETH_MAC_TIMESTP_CTRL_TSENA              (1UL << 0)
#define ETH_MAC_TIMESTP_CTRL_TSCFUPDT           (1UL << 1)
#define ETH_MAC_TIMESTP_CTRL_TSINIT             (1UL << 2)
#define ETH_MAC_TIMESTP_CTRL_TSUPDT             (1UL << 3)
#define ETH_MAC_TIMESTP_CTRL_TSTRIG             (1UL << 4)
#define ETH_MAC_TIMESTP_CTRL_TSADDREG           (1UL << 5)
#define ETH_MAC_TIMESTP_CTRL_TSENALL            (1UL << 8)
#define ETH_MAC_TIMESTP_CTRL_TSCTRLSSR          (1UL << 9)
#define ETH_MAC_TIMESTP_CTRL_TSVER2ENA          (1UL << 10)
#define ETH_MAC_TIMESTP_CTRL_TSIPENA            (1UL << 11)
#define ETH_MAC_TIMESTP_CTRL_TSIPV6ENA          (1UL << 12)
#define ETH_MAC_TIMESTP_CTRL_TSIPV4ENA          (1UL << 13)
#define ETH_MAC_TIMESTP_CTRL_TSEVNTENA          (1UL << 14)
#define ETH_MAC_TIMESTP_CTRL_TSMSTRENA          (1UL << 15)
#define ETH_MAC_TIMESTP_CTRL_TSCLKTYPE          (3UL << 16)
#define ETH_MAC_TIMESTP_CTRL_TSCLKTYPE_OCLK     (0UL << 16)
#define ETH_MAC_TIMESTP_CTRL_TSCLKTYPE_BCLK     (1UL << 16)
#define ETH_MAC_TIMESTP_CTRL_TSCLKTYPE_ETETCLK  (2UL << 16)
#define ETH_MAC_TIMESTP_CTRL_TSCLKTYPE_PTPTCLK  (3UL << 16)
#define ETH_MAC_TIMESTP_CTRL_TSENMACADDR        (1UL << 18)
/** @} */

/**
 * @name        System time nanoseconds register bits
 * @{
 */
#define ETH_NANOSECONDS_PSNT                    (1UL << 31)
/** @} */

/**
 * @name        System time nanoseconds update register bits
 * @{
 */
#define ETH_NANOSECONDSUPDATE_ADDSUB            (1UL << 31)
/** @} */

/**
 * @name        Time stamp status register bits
 * @{
 */
#define ETH_TIMESTAMPSTAT_TSSOVF                (1UL << 0)
#define ETH_TIMESTAMPSTAT_TSTARGT               (1UL << 1)
/** @} */

/**
 * @name        DMA Bus mode register bits
 * @{
 */
#define ETH_DMA_BUS_MODE_SWR                    (1UL << 0)
#define ETH_DMA_BUS_MODE_DA                     (1UL << 1)
#define ETH_DMA_BUS_MODE_DSL(len)               ((len) << 2)
#define ETH_DMA_BUS_MODE_ATDS                   (1UL << 7)
#define ETH_DMA_BUS_MODE_PBL(len)               ((len) << 8)
#define ETH_DMA_BUS_MODE_PR                     (3UL << 14)
#define ETH_DMA_BUS_MODE_PR_1T1                 (0UL << 14)
#define ETH_DMA_BUS_MODE_PR_2T1                 (1UL << 14)
#define ETH_DMA_BUS_MODE_PR_3T1                 (2UL << 14)
#define ETH_DMA_BUS_MODE_PR_4T1                 (3UL << 14)
#define ETH_DMA_BUS_MODE_FB                     (1UL << 16)
#define ETH_DMA_BUS_MODE_RPBL(n)                ((n) << 17)
#define ETH_DMA_BUS_MODE_USP                    (1UL << 23)
#define ETH_DMA_BUS_MODE_PBL8X                  (1UL << 24)
#define ETH_DMA_BUS_MODE_AAL                    (1UL << 25)
#define ETH_DMA_BUS_MODE_MB                     (1UL << 26)
#define ETH_DMA_BUS_MODE_TXPR                   (1UL << 27)
/** @} */

/**
 * @name         DMA Status register bits
 * @{
 */
#define ETH_DMA_STAT_TI                         (1UL << 0)
#define ETH_DMA_STAT_TPS                        (1UL << 1)
#define ETH_DMA_STAT_TU                         (1UL << 2)
#define ETH_DMA_STAT_TJT                        (1UL << 3)
#define ETH_DMA_STAT_OVF                        (1UL << 4)
#define ETH_DMA_STAT_UNF                        (1UL << 5)
#define ETH_DMA_STAT_RI                         (1UL << 6)
#define ETH_DMA_STAT_RU                         (1UL << 7)
#define ETH_DMA_STAT_RPS                        (1UL << 8)
#define ETH_DMA_STAT_RWT                        (1UL << 9)
#define ETH_DMA_STAT_ETI                        (1UL << 10)
#define ETH_DMA_STAT_FBI                        (1UL << 13)
#define ETH_DMA_STAT_ERI                        (1UL << 14)
#define ETH_DMA_STAT_AIE                        (1UL << 15)
#define ETH_DMA_STAT_NIS                        (1UL << 16)
#define ETH_DMA_STAT_RS                         (7UL << 17)
#define ETH_DMA_STAT_RS_STOP                    (0UL << 17)
#define ETH_DMA_STAT_RS_RUN_FETCH               (1UL << 17)
#define ETH_DMA_STAT_RS_RUN_WAIT                (3UL << 17)
#define ETH_DMA_STAT_RS_SUSPEND                 (4UL << 17)
#define ETH_DMA_STAT_RS_RUN_CLOSE               (5UL << 17)
#define ETH_DMA_STAT_RS_TIME_STAMP              (6UL << 17)
#define ETH_DMA_STAT_RS_RUN_TRANSFER            (7UL << 17)
#define ETH_DMA_STAT_TS                         (7UL << 20)
#define ETH_DMA_STAT_TS_STOP                    (0UL << 20)
#define ETH_DMA_STAT_TS_RUN_FETCH               (1UL << 20)
#define ETH_DMA_STAT_TS_RUN_WAIT                (2UL << 20)
#define ETH_DMA_STAT_TS_RUN_READ                (3UL << 20)
#define ETH_DMA_STAT_TS_TIME_STAMP              (4UL << 20)
#define ETH_DMA_STAT_TS_SUSPEND                 (6UL << 20)
#define ETH_DMA_STAT_TS_RUN_CLOSE               (7UL << 20)
#define ETH_DMA_STAT_EB1                        (1UL << 23)
#define ETH_DMA_STAT_EB2                        (1UL << 24)
#define ETH_DMA_STAT_EB3                        (1UL << 25)
/** @} */

/**
 * @name         DMA Operation mode register bits
 * @{
 */
#define ETH_DMA_OP_MODE_SR                      (1UL << 1)
#define ETH_DMA_OP_MODE_OSF                     (1UL << 2)
#define ETH_DMA_OP_MODE_RTC                     (3UL << 3)
#define ETH_DMA_OP_MODE_RTC_64                  (0UL << 3)
#define ETH_DMA_OP_MODE_RTC_32                  (1UL << 3)
#define ETH_DMA_OP_MODE_RTC_96                  (2UL << 3)
#define ETH_DMA_OP_MODE_RTC_128                 (3UL << 3)
#define ETH_DMA_OP_MODE_FUF                     (1UL << 6)
#define ETH_DMA_OP_MODE_FEF                     (1UL << 7)
#define ETH_DMA_OP_MODE_ST                      (1UL << 13)
#define ETH_DMA_OP_MODE_TTC                     (7UL << 14)
#define ETH_DMA_OP_MODE_TTC_64                  (0UL << 14)
#define ETH_DMA_OP_MODE_TTC_128                 (1UL << 14)
#define ETH_DMA_OP_MODE_TTC_192                 (2UL << 14)
#define ETH_DMA_OP_MODE_TTC_256                 (3UL << 14)
#define ETH_DMA_OP_MODE_TTC_40                  (4UL << 14)
#define ETH_DMA_OP_MODE_TTC_32                  (5UL << 14)
#define ETH_DMA_OP_MODE_TTC_24                  (6UL << 14)
#define ETH_DMA_OP_MODE_TTC_16                  (7UL << 14)
#define ETH_DMA_OP_MODE_FTF                     (1UL << 20)
#define ETH_DMA_OP_MODE_DFF                     (1UL << 24)
/** @} */

/**
 * @name         DMA Interrupt enable register bits
 * @{
 */
#define ETH_DMA_INT_EN_TIE                      (1UL << 0)
#define ETH_DMA_INT_EN_TSE                      (1UL << 1)
#define ETH_DMA_INT_EN_TUE                      (1UL << 2)
#define ETH_DMA_INT_EN_TJE                      (1UL << 3)
#define ETH_DMA_INT_EN_OVE                      (1UL << 4)
#define ETH_DMA_INT_EN_UNE                      (1UL << 5)
#define ETH_DMA_INT_EN_RIE                      (1UL << 6)
#define ETH_DMA_INT_EN_RUE                      (1UL << 7)
#define ETH_DMA_INT_EN_RSE                      (1UL << 8)
#define ETH_DMA_INT_EN_RWE                      (1UL << 9)
#define ETH_DMA_INT_EN_ETE                      (1UL << 10)
#define ETH_DMA_INT_EN_FBE                      (1UL << 13)
#define ETH_DMA_INT_EN_ERE                      (1UL << 14)
#define ETH_DMA_INT_EN_AIE                      (1UL << 15)
#define ETH_DMA_INT_EN_NIE                      (1UL << 16)
/** @} */

/**
 * @name         DMA Missed frame and buffer overflow counter register bits
 * @{
 */
#define ETH_DMA_MFRM_BUFOF_FMC                  0x0000FFFF
#define ETH_DMA_MFRM_BUFOF_OC                   (1UL << 16)
#define ETH_DMA_MFRM_BUFOF_FMA                  0x0FFE0000
#define ETH_DMA_MFRM_BUFOF_OF                   (1UL << 28)
/** @} */

/**
 * @name    RDES0 constants
 * @{
 */
#define LPC_RDES0_OWN             0x80000000
#define LPC_RDES0_AFM             0x40000000
#define LPC_RDES0_FL_MASK         0x3FFF0000
#define LPC_RDES0_ES              0x00008000
#define LPC_RDES0_DESERR          0x00004000
#define LPC_RDES0_SAF             0x00002000
#define LPC_RDES0_LE              0x00001000
#define LPC_RDES0_OE              0x00000800
#define LPC_RDES0_VLAN            0x00000400
#define LPC_RDES0_FS              0x00000200
#define LPC_RDES0_LS              0x00000100
#define LPC_RDES0_IPHCE           0x00000080
#define LPC_RDES0_LCO             0x00000040
#define LPC_RDES0_FT              0x00000020
#define LPC_RDES0_RWT             0x00000010
#define LPC_RDES0_RE              0x00000008
#define LPC_RDES0_DE              0x00000004
#define LPC_RDES0_CE              0x00000002
#define LPC_RDES0_PCE             0x00000001
/** @} */

/**
 * @name    RDES1 constants
 * @{
 */
#define LPC_RDES1_DIC             0x80000000
#define LPC_RDES1_RBS2_MASK       0x1FFF0000
#define LPC_RDES1_RER             0x00008000
#define LPC_RDES1_RCH             0x00004000
#define LPC_RDES1_RBS1_MASK       0x00001FFF
/** @} */

/**
 * @name    TDES0 constants
 * @{
 */
#define LPC_TDES0_OWN             0x80000000
#define LPC_TDES0_IC              0x40000000
#define LPC_TDES0_LS              0x20000000
#define LPC_TDES0_FS              0x10000000
#define LPC_TDES0_DC              0x08000000
#define LPC_TDES0_DP              0x04000000
#define LPC_TDES0_TTSE            0x02000000
#define LPC_TDES0_LOCKED          0x01000000 /* NOTE: Pseudo flag.        */
#define LPC_TDES0_CIC_MASK        0x00C00000
#define LPC_TDES0_CIC(n)          ((n) << 22)
#define LPC_TDES0_TER             0x00200000
#define LPC_TDES0_TCH             0x00100000
#define LPC_TDES0_TTSS            0x00020000
#define LPC_TDES0_IHE             0x00010000
#define LPC_TDES0_ES              0x00008000
#define LPC_TDES0_JT              0x00004000
#define LPC_TDES0_FF              0x00002000
#define LPC_TDES0_IPE             0x00001000
#define LPC_TDES0_LCA             0x00000800
#define LPC_TDES0_NC              0x00000400
#define LPC_TDES0_LCO             0x00000200
#define LPC_TDES0_EC              0x00000100
#define LPC_TDES0_VF              0x00000080
#define LPC_TDES0_CC_MASK         0x00000078
#define LPC_TDES0_ED              0x00000004
#define LPC_TDES0_UF              0x00000002
#define LPC_TDES0_DB              0x00000001
/** @} */

/**
 * @name    TDES1 constants
 * @{
 */
#define LPC_TDES1_TBS2_MASK       0x1FFF0000
#define LPC_TDES1_TBS1_MASK       0x00001FFF
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
#if !defined(LPC_MAC_TRANSMIT_BUFFERS) || defined(__DOXYGEN__)
#define LPC_MAC_TRANSMIT_BUFFERS          2
#endif

/**
 * @brief   Number of available receive buffers.
 */
#if !defined(LPC_MAC_RECEIVE_BUFFERS) || defined(__DOXYGEN__)
#define LPC_MAC_RECEIVE_BUFFERS           4
#endif

/**
 * @brief   Maximum supported frame size.
 */
#if !defined(LPC_MAC_BUFFERS_SIZE) || defined(__DOXYGEN__)
#define LPC_MAC_BUFFERS_SIZE              1522
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
#if !defined(LPC_MAC_PHY_TIMEOUT) || defined(__DOXYGEN__)
#define LPC_MAC_PHY_TIMEOUT               100
#endif

/**
 * @brief   Change the PHY power state inside the driver.
 */
#if !defined(LPC_MAC_ETH1_CHANGE_PHY_STATE) || defined(__DOXYGEN__)
#define LPC_MAC_ETH1_CHANGE_PHY_STATE     TRUE
#endif

/**
 * @brief   ETHD1 interrupt priority level setting.
 */
#if !defined(LPC_MAC_ETH1_IRQ_PRIORITY) || defined(__DOXYGEN__)
#define LPC_MAC_ETH1_IRQ_PRIORITY         13
#endif

/**
 * @brief   IP checksum offload.
 * @details The following modes are available:
 *          - 0 Function disabled.
 *          - 1 Only IP header checksum calculation and insertion are enabled.
 *          - 2 IP header checksum and payload checksum calculation and
 *              insertion are enabled, but pseudo-header checksum is not
 *              calculated in hardware.
 *          - 3 IP Header checksum and payload checksum calculation and
 *              insertion are enabled, and pseudo-header checksum is
 *              calculated in hardware.
 *          .
 */
#if !defined(LPC_MAC_IP_CHECKSUM_OFFLOAD) || defined(__DOXYGEN__)
#define LPC_MAC_IP_CHECKSUM_OFFLOAD       0
#endif
/** @} */

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

#if (LPC_MAC_PHY_TIMEOUT > 0) && !HAL_IMPLEMENTS_COUNTERS
#error "LPC_MAC_PHY_TIMEOUT requires the realtime counter service"
#endif

/*===========================================================================*/
/* Driver data structures and types.                                         */
/*===========================================================================*/

/**
 * @brief   Type of an LPC Ethernet receive descriptor.
 */
typedef struct {
  volatile uint32_t     rdes0;
  volatile uint32_t     rdes1;
  volatile uint32_t     rdes2;
  volatile uint32_t     rdes3;
} lpc_eth_rx_descriptor_t;

/**
 * @brief   Type of an LPC Ethernet transmit descriptor.
 */
typedef struct {
  volatile uint32_t     tdes0;
  volatile uint32_t     tdes1;
  volatile uint32_t     tdes2;
  volatile uint32_t     tdes3;
} lpc_eth_tx_descriptor_t;

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
   * @brief Receive next frame pointer.
   */
  lpc_eth_rx_descriptor_t *rxptr;
  /**
   * @brief Transmit next frame pointer.
   */
  lpc_eth_tx_descriptor_t *txptr;
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
  lpc_eth_tx_descriptor_t *physdesc;
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
  lpc_eth_rx_descriptor_t *physdesc;
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
