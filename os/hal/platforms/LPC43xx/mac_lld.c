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
 * @file    LPC43xx/mac_lld.c
 * @brief   LPC43xx low level MAC driver code.
 *
 * @addtogroup MAC
 * @{
 */

#include <string.h>

#include "ch.h"
#include "hal.h"
#include "mii.h"

#if HAL_USE_MAC || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver local definitions.                                                 */
/*===========================================================================*/

#define BUFFER_SIZE ((((LPC_MAC_BUFFERS_SIZE - 1) | 3) + 1) / 4)

/* MII divider optimal value.*/
#if (LPC_CLK_M4_ETHERNET >= 150000000)
#define MAC_MII_ADDR_CR         ETH_MAC_MII_ADDR_CR_DIV_102
#elif (LPC_CLK_M4_ETHERNET >= 100000000)
#define MAC_MII_ADDR_CR         ETH_MAC_MII_ADDR_CR_DIV_62
#elif (LPC_CLK_M4_ETHERNET >= 60000000)
#define MAC_MII_ADDR_CR         ETH_MAC_MII_ADDR_CR_DIV_42
#elif (LPC_CLK_M4_ETHERNET >= 35000000)
#define MAC_MII_ADDR_CR         ETH_MAC_MII_ADDR_CR_DIV_26
#elif (LPC_CLK_M4_ETHERNET >= 20000000)
#define MAC_MII_ADDR_CR         ETH_MAC_MII_ADDR_CR_DIV_16
#else
#error "LPC_CLK_M4_ETHERNET below minimum frequency for ETH operations (20MHz)"
#endif

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

/**
 * @brief   Ethernet driver 1.
 */
MACDriver ETHD1;

/*===========================================================================*/
/* Driver local variables and types.                                         */
/*===========================================================================*/

static const uint8_t default_mac_address[] = {0xAA, 0x55, 0x13,
                                              0x37, 0x01, 0x10};

static lpc_eth_rx_descriptor_t rd[LPC_MAC_RECEIVE_BUFFERS];
static lpc_eth_tx_descriptor_t td[LPC_MAC_TRANSMIT_BUFFERS];

static uint32_t rb[LPC_MAC_RECEIVE_BUFFERS][BUFFER_SIZE];
static uint32_t tb[LPC_MAC_TRANSMIT_BUFFERS][BUFFER_SIZE];

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

/**
 * @brief   Enable MAC clock.
 */
static void mac_clk_enable(void) {

    LPC_CGU->BASE_PHY_RX_CLK &= ~(1UL << 0);
    LPC_CGU->BASE_PHY_TX_CLK &= ~(1UL << 0);
    LPC_CCU1->CLK_M4_ETHERNET_CFG = 1;
}

/**
 * @brief   Disable MAC clock.
 */
static void mac_clk_disable(void) {

    LPC_CGU->BASE_PHY_RX_CLK |= (1UL << 0);
    LPC_CGU->BASE_PHY_TX_CLK |= (1UL << 0);
    LPC_CCU1->CLK_M4_ETHERNET_CFG = 0;
}

/**
 * @brief   Writes a PHY register.
 *
 * @param[in] macp      pointer to the @p MACDriver object
 * @param[in] reg       register number
 * @param[in] value     new register value
 */
static void mii_write(MACDriver *macp, uint32_t reg, uint32_t value) { // OK

  LPC_ETHERNET->MAC_MII_DATA = value;
  LPC_ETHERNET->MAC_MII_ADDR = macp->phyaddr | (reg << 6) | MAC_MII_ADDR_CR |
      ETH_MAC_MII_ADDR_W | ETH_MAC_MII_ADDR_GB;
  while ((LPC_ETHERNET->MAC_MII_ADDR & ETH_MAC_MII_ADDR_GB) != 0)
    ;
}

/**
 * @brief   Reads a PHY register.
 *
 * @param[in] macp      pointer to the @p MACDriver object
 * @param[in] reg       register number
 *
 * @return              The PHY register content.
 */
static uint32_t mii_read(MACDriver *macp, uint32_t reg) { // OK

  LPC_ETHERNET->MAC_MII_ADDR = macp->phyaddr | (reg << 6) | MAC_MII_ADDR_CR | ETH_MAC_MII_ADDR_GB;
  while ((LPC_ETHERNET->MAC_MII_ADDR & ETH_MAC_MII_ADDR_GB) != 0)
    ;
  return LPC_ETHERNET->MAC_MII_DATA;
}

#if !defined(BOARD_PHY_ADDRESS)
/**
 * @brief   PHY address detection.
 *
 * @param[in] macp      pointer to the @p MACDriver object
 */
static void mii_find_phy(MACDriver *macp) { // OK
  uint32_t i;

#if LPC_MAC_PHY_TIMEOUT > 0
  halrtcnt_t start = halGetCounterValue();
  halrtcnt_t timeout  = start + MS2RTT(LPC_MAC_PHY_TIMEOUT);
  while (halIsCounterWithin(start, timeout)) {
#endif
    for (i = 0; i < 31; i++) {
      macp->phyaddr = i << 11;
      LPC_ETHERNET->MAC_MII_DATA = (i << 6) | MAC_MII_ADDR_CR;
      if ((mii_read(macp, MII_PHYSID1) == (BOARD_PHY_ID >> 16)) &&
          ((mii_read(macp, MII_PHYSID2) & 0xFFF0) == (BOARD_PHY_ID & 0xFFF0))) {
        return;
      }
    }
#if LPC_MAC_PHY_TIMEOUT > 0
  }
#endif
  /* Wrong or defective board.*/
  chSysHalt();
}
#endif

/**
 * @brief   MAC address setup.
 *
 * @param[in] p         pointer to a six bytes buffer containing the MAC
 *                      address
 */
static void mac_lld_set_address(const uint8_t *p) { // OK

  /* MAC address configuration, only a single address comparator is used,
     hash table not used.*/
  LPC_ETHERNET->MAC_ADDR0_HIGH   = ((uint32_t)p[5] << 8) |
                   ((uint32_t)p[4] << 0);
  LPC_ETHERNET->MAC_ADDR0_LOW  = ((uint32_t)p[3] << 24) |
                   ((uint32_t)p[2] << 16) |
                   ((uint32_t)p[1] << 8) |
                   ((uint32_t)p[0] << 0);
  LPC_ETHERNET->MAC_HASHTABLE_HIGH = 0;
  LPC_ETHERNET->MAC_HASHTABLE_LOW = 0;
}

/*===========================================================================*/
/* Driver interrupt handlers.                                                */
/*===========================================================================*/

CH_IRQ_HANDLER(Vector54) {  // OK
  uint32_t dmasr;

  CH_IRQ_PROLOGUE();

  dmasr = LPC_ETHERNET->DMA_STAT;
  LPC_ETHERNET->DMA_STAT = dmasr; /* Clear status bits.*/

  if (dmasr & ETH_DMA_STAT_RI) {
    /* Data Received.*/
    chSysLockFromIsr();
    chSemResetI(&ETHD1.rdsem, 0);
#if MAC_USE_EVENTS
    chEvtBroadcastI(&ETHD1.rdevent);
#endif
    chSysUnlockFromIsr();
  }

  if (dmasr & ETH_DMA_STAT_TI) {
    /* Data Transmitted.*/
    chSysLockFromIsr();
    chSemResetI(&ETHD1.tdsem, 0);
    chSysUnlockFromIsr();
  }

  CH_IRQ_EPILOGUE();
}

/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

/**
 * @brief   Low level MAC initialization.
 *
 * @notapi
 */
void mac_lld_init(void) {
  unsigned i;

  macObjectInit(&ETHD1);
  ETHD1.link_up = FALSE;

  /* Descriptor tables are initialized in chained mode, note that the first
     word is not initialized here but in mac_lld_start().*/
  for (i = 0; i < LPC_MAC_RECEIVE_BUFFERS; i++) {
    rd[i].rdes1 = LPC_RDES1_RCH | LPC_MAC_BUFFERS_SIZE;
    rd[i].rdes2 = (uint32_t)rb[i];
    rd[i].rdes3 = (uint32_t)&rd[(i + 1) % LPC_MAC_RECEIVE_BUFFERS];
  }
  for (i = 0; i < LPC_MAC_TRANSMIT_BUFFERS; i++) {
    td[i].tdes1 = 0;
    td[i].tdes2 = (uint32_t)tb[i];
    td[i].tdes3 = (uint32_t)&td[(i + 1) % LPC_MAC_TRANSMIT_BUFFERS];
  }

  /* Selection of the RMII or MII mode based on info exported by board.h.*/

#if defined(BOARD_PHY_RMII)
  LPC_CREG->CREG6 |= (4UL << 0);
#else
  LPC_CREG->CREG6 &= ~(7UL << 0);
#endif

  /* Reset of the MAC core.*/
  LPC_RGU->RESET_CTRL0 = (1UL << 22);

  for (i = 0; i < (LPC_BASE_M4_CLK/IRCOSCCLK + 1); i++)
    __NOP();        /* Wait. */

  /* MAC clocks temporary activation.*/
  mac_clk_enable();

  /* PHY address setup.*/
#if defined(BOARD_PHY_ADDRESS)
  ETHD1.phyaddr = BOARD_PHY_ADDRESS << 11;
#else
  mii_find_phy(&ETHD1);
#endif

#if defined(BOARD_PHY_RESET)
  /* PHY board-specific reset procedure.*/
  BOARD_PHY_RESET();
#else
  /* PHY soft reset procedure.*/
  mii_write(&ETHD1, MII_BMCR, BMCR_RESET);
#if defined(BOARD_PHY_RESET_DELAY)
  halPolledDelay(BOARD_PHY_RESET_DELAY);
#endif
  while (mii_read(&ETHD1, MII_BMCR) & BMCR_RESET)
    ;
#endif

#if LPC_MAC_ETH1_CHANGE_PHY_STATE
  /* PHY in power down mode until the driver will be started.*/
  mii_write(&ETHD1, MII_BMCR, mii_read(&ETHD1, MII_BMCR) | BMCR_PDOWN);
#endif

  /* MAC clocks stopped again.*/
  mac_clk_disable();
}

/**
 * @brief   Configures and activates the MAC peripheral.
 *
 * @param[in] macp      pointer to the @p MACDriver object
 *
 * @notapi
 */
void mac_lld_start(MACDriver *macp) {
  unsigned i;

  /* Resets the state of all descriptors.*/
  for (i = 0; i < LPC_MAC_RECEIVE_BUFFERS; i++)
    rd[i].rdes0 = LPC_RDES0_OWN;
  macp->rxptr = (lpc_eth_rx_descriptor_t *)rd;
  for (i = 0; i < LPC_MAC_TRANSMIT_BUFFERS; i++)
    td[i].tdes0 = LPC_TDES0_TCH;
  macp->txptr = (lpc_eth_tx_descriptor_t *)td;

  /* MAC clocks activation and commanded reset procedure.*/
  mac_clk_enable();

  /* PHY in power up mode.*/
  mii_write(macp, MII_BMCR, mii_read(macp, MII_BMCR) & ~BMCR_PDOWN);

  LPC_ETHERNET->DMA_BUS_MODE |= ETH_DMA_BUS_MODE_SWR;     /* Software reset. */
  while(LPC_ETHERNET->DMA_BUS_MODE & ETH_DMA_BUS_MODE_SWR)
    ;

  /* ISR vector enabled.*/
  nvicEnableVector(ETHERNET_IRQn,
                   CORTEX_PRIORITY_MASK(LPC_MAC_ETH1_IRQ_PRIORITY));

  /* MAC configuration.*/
  LPC_ETHERNET->MAC_FRAME_FILTER = 0;
  LPC_ETHERNET->MAC_FLOW_CTRL = 0;
  LPC_ETHERNET->MAC_VLAN_TAG = 0;

  /* MAC address setup.*/
  if (macp->config->mac_address == NULL)
    mac_lld_set_address(default_mac_address);
  else
    mac_lld_set_address(macp->config->mac_address);

  /* Transmitter and receiver enabled.
     Note that the complete setup of the MAC is performed when the link
     status is detected.*/
  LPC_ETHERNET->MAC_CONFIG = ETH_MAC_CONFIG_RE | ETH_MAC_CONFIG_TE;

  /* DMA configuration:
     Descriptor chains pointers.*/
  LPC_ETHERNET->DMA_REC_DES_ADDR = (uint32_t)rd;
  LPC_ETHERNET->DMA_TRANS_DES_ADDR = (uint32_t)td;

  /* Enabling required interrupt sources.*/
  LPC_ETHERNET->DMA_STAT = LPC_ETHERNET->DMA_STAT;
  LPC_ETHERNET->DMA_INT_EN = ETH_DMA_INT_EN_NIE | ETH_DMA_INT_EN_RIE | ETH_DMA_INT_EN_TIE;

  /* DMA general settings.*/
  LPC_ETHERNET->DMA_BUS_MODE = ETH_DMA_BUS_MODE_AAL | ETH_DMA_BUS_MODE_RPBL(1UL) |
      ETH_DMA_BUS_MODE_PBL(1UL);

  /* Transmit FIFO flush.*/
  LPC_ETHERNET->DMA_OP_MODE = ETH_DMA_OP_MODE_FTF;
  while (LPC_ETHERNET->DMA_OP_MODE & ETH_DMA_OP_MODE_FTF)
    ;

  /* DMA final configuration and start.*/
  LPC_ETHERNET->DMA_OP_MODE = ETH_DMA_OP_MODE_TTC_256 | ETH_DMA_OP_MODE_ST |
      ETH_DMA_OP_MODE_RTC_128 | ETH_DMA_OP_MODE_SR;
}

/**
 * @brief   Deactivates the MAC peripheral.
 *
 * @param[in] macp      pointer to the @p MACDriver object
 *
 * @notapi
 */
void mac_lld_stop(MACDriver *macp) {

  if (macp->state != MAC_STOP) {

    /* MAC and DMA stopped.*/
    LPC_ETHERNET->MAC_CONFIG = 0;
    LPC_ETHERNET->DMA_OP_MODE = 0;
    LPC_ETHERNET->DMA_INT_EN = 0;
    LPC_ETHERNET->DMA_STAT = LPC_ETHERNET->DMA_STAT;

    /* PHY in power down mode until the driver will be restarted.*/
    mii_write(macp, MII_BMCR, mii_read(macp, MII_BMCR) | BMCR_PDOWN);

    /* MAC clocks stopped.*/
    mac_clk_disable();

    /* ISR vector disabled.*/
    nvicDisableVector(ETHERNET_IRQn);
  }
}

/**
 * @brief   Returns a transmission descriptor.
 * @details One of the available transmission descriptors is locked and
 *          returned.
 *
 * @param[in] macp      pointer to the @p MACDriver object
 * @param[out] tdp      pointer to a @p MACTransmitDescriptor structure
 * @return              The operation status.
 * @retval RDY_OK       the descriptor has been obtained.
 * @retval RDY_TIMEOUT  descriptor not available.
 *
 * @notapi
 */
msg_t mac_lld_get_transmit_descriptor(MACDriver *macp,
                                      MACTransmitDescriptor *tdp) {
  lpc_eth_tx_descriptor_t *tdes;

  if (!macp->link_up)
    return RDY_TIMEOUT;

  chSysLock();

  /* Get Current TX descriptor.*/
  tdes = macp->txptr;

  /* Ensure that descriptor isn't owned by the Ethernet DMA or locked by
     another thread.*/
  if (tdes->tdes0 & (LPC_TDES0_OWN | LPC_TDES0_LOCKED)) {
    chSysUnlock();
    return RDY_TIMEOUT;
  }

  /* Marks the current descriptor as locked using a reserved bit.*/
  tdes->tdes0 |= LPC_TDES0_LOCKED;

  /* Next TX descriptor to use.*/
  macp->txptr = (lpc_eth_tx_descriptor_t *)tdes->tdes3;

  chSysUnlock();

  /* Set the buffer size and configuration.*/
  tdp->offset   = 0;
  tdp->size     = LPC_MAC_BUFFERS_SIZE;
  tdp->physdesc = tdes;

  return RDY_OK;
}

/**
 * @brief   Releases a transmit descriptor and starts the transmission of the
 *          enqueued data as a single frame.
 *
 * @param[in] tdp       the pointer to the @p MACTransmitDescriptor structure
 *
 * @notapi
 */
void mac_lld_release_transmit_descriptor(MACTransmitDescriptor *tdp) {

  chDbgAssert(!(tdp->physdesc->tdes0 & LPC_TDES0_OWN),
              "mac_lld_release_transmit_descriptor(), #1",
              "attempt to release descriptor already owned by DMA");

  chSysLock();

  /* Unlocks the descriptor and returns it to the DMA engine.*/
  tdp->physdesc->tdes1 = tdp->offset;
  tdp->physdesc->tdes0 = LPC_TDES0_CIC(LPC_MAC_IP_CHECKSUM_OFFLOAD) |
                         LPC_TDES0_IC | LPC_TDES0_LS | LPC_TDES0_FS |
                         LPC_TDES0_TCH | LPC_TDES0_OWN;

  /* If the DMA engine is stalled then a restart request is issued.*/
  if ((LPC_ETHERNET->DMA_STAT & ETH_DMA_STAT_TS) == ETH_DMA_STAT_TS_SUSPEND) {
    LPC_ETHERNET->DMA_STAT = ETH_DMA_STAT_TU;
    LPC_ETHERNET->DMA_TRANS_POLL_DEMAND = ETH_DMA_STAT_TU; /* Any value is OK.*/
  }

  chSysUnlock();
}

/**
 * @brief   Returns a receive descriptor.
 *
 * @param[in] macp      pointer to the @p MACDriver object
 * @param[out] rdp      pointer to a @p MACReceiveDescriptor structure
 * @return              The operation status.
 * @retval RDY_OK       the descriptor has been obtained.
 * @retval RDY_TIMEOUT  descriptor not available.
 *
 * @notapi
 */
msg_t mac_lld_get_receive_descriptor(MACDriver *macp,
                                     MACReceiveDescriptor *rdp) {
  lpc_eth_rx_descriptor_t *rdes;

  chSysLock();

  /* Get Current RX descriptor.*/
  rdes = macp->rxptr;

  /* Iterates through received frames until a valid one is found, invalid
     frames are discarded.*/
  while (!(rdes->rdes0 & LPC_RDES0_OWN)) {
    if (!(rdes->rdes0 & (LPC_RDES0_AFM | LPC_RDES0_ES))
#if LPC_MAC_IP_CHECKSUM_OFFLOAD
        && (rdes->rdes0 & LPC_RDES0_FT)
        && !(rdes->rdes0 & (LPC_RDES0_IPHCE | LPC_RDES0_PCE))
#endif
        && (rdes->rdes0 & LPC_RDES0_FS) && (rdes->rdes0 & LPC_RDES0_LS)) {
      /* Found a valid one.*/
      rdp->offset   = 0;
      rdp->size     = ((rdes->rdes0 & LPC_RDES0_FL_MASK) >> 16) - 4;
      rdp->physdesc = rdes;
      macp->rxptr   = (lpc_eth_rx_descriptor_t *)rdes->rdes3;

      chSysUnlock();
      return RDY_OK;
    }
    /* Invalid frame found, purging.*/
    rdes->rdes0 = LPC_RDES0_OWN;
    rdes = (lpc_eth_rx_descriptor_t *)rdes->rdes3;
  }

  /* Next descriptor to check.*/
  macp->rxptr = rdes;

  chSysUnlock();
  return RDY_TIMEOUT;
}

/**
 * @brief   Releases a receive descriptor.
 * @details The descriptor and its buffer are made available for more incoming
 *          frames.
 *
 * @param[in] rdp       the pointer to the @p MACReceiveDescriptor structure
 *
 * @notapi
 */
void mac_lld_release_receive_descriptor(MACReceiveDescriptor *rdp) {

  chDbgAssert(!(rdp->physdesc->rdes0 & LPC_RDES0_OWN),
              "mac_lld_release_receive_descriptor(), #1",
              "attempt to release descriptor already owned by DMA");

  chSysLock();

  /* Give buffer back to the Ethernet DMA.*/
  rdp->physdesc->rdes0 = LPC_RDES0_OWN;

  /* If the DMA engine is stalled then a restart request is issued.*/
  if ((LPC_ETHERNET->DMA_STAT & ETH_DMA_STAT_RS) == ETH_DMA_STAT_RS_SUSPEND) {
    LPC_ETHERNET->DMA_STAT   = ETH_DMA_STAT_RU;
    LPC_ETHERNET->DMA_REC_POLL_DEMAND = ETH_DMA_STAT_RU; /* Any value is OK.*/
  }

  chSysUnlock();
}

/**
 * @brief   Updates and returns the link status.
 *
 * @param[in] macp      pointer to the @p MACDriver object
 * @return              The link status.
 * @retval TRUE         if the link is active.
 * @retval FALSE        if the link is down.
 *
 * @notapi
 */
bool_t mac_lld_poll_link_status(MACDriver *macp) {
  uint32_t maccr, bmsr, bmcr;

  maccr = LPC_ETHERNET->MAC_CONFIG;

  /* PHY CR and SR registers read.*/
  (void)mii_read(macp, MII_BMSR);
  bmsr = mii_read(macp, MII_BMSR);
  bmcr = mii_read(macp, MII_BMCR);

  /* Check on auto-negotiation mode.*/
  if (bmcr & BMCR_ANENABLE) {
    uint32_t lpa;

    /* Auto-negotiation must be finished without faults and link established.*/
    if ((bmsr & (BMSR_LSTATUS | BMSR_RFAULT | BMSR_ANEGCOMPLETE)) !=
        (BMSR_LSTATUS | BMSR_ANEGCOMPLETE))
      return macp->link_up = FALSE;

    /* Auto-negotiation enabled, checks the LPA register.*/
    lpa = mii_read(macp, MII_LPA);

    /* Check on link speed.*/
    if (lpa & (LPA_100HALF | LPA_100FULL | LPA_100BASE4))
      maccr |= ETH_MAC_CONFIG_FES;
    else
      maccr &= ~ETH_MAC_CONFIG_FES;

    /* Check on link mode.*/
    if (lpa & (LPA_10FULL | LPA_100FULL))
      maccr |= ETH_MAC_CONFIG_DM;
    else
      maccr &= ~ETH_MAC_CONFIG_DM;
  }
  else {
    /* Link must be established.*/
    if (!(bmsr & BMSR_LSTATUS))
      return macp->link_up = FALSE;

    /* Check on link speed.*/
    if (bmcr & BMCR_SPEED100)
      maccr |= ETH_MAC_CONFIG_FES;
    else
      maccr &= ~ETH_MAC_CONFIG_FES;

    /* Check on link mode.*/
    if (bmcr & BMCR_FULLDPLX)
      maccr |= ETH_MAC_CONFIG_DM;
    else
      maccr &= ~ETH_MAC_CONFIG_DM;
  }

  /* Changes the mode in the MAC.*/
  LPC_ETHERNET->MAC_CONFIG = maccr;

  /* Returns the link status.*/
  return macp->link_up = TRUE;
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

  chDbgAssert(!(tdp->physdesc->tdes0 & LPC_TDES0_OWN),
              "mac_lld_write_transmit_descriptor(), #1",
              "attempt to write descriptor already owned by DMA");

  if (size > tdp->size - tdp->offset)
    size = tdp->size - tdp->offset;

  if (size > 0) {
    memcpy((uint8_t *)(tdp->physdesc->tdes2) + tdp->offset, buf, size);
    tdp->offset += size;
  }
  return size;
}

/**
 * @brief   Reads from a receive descriptor's stream.
 *
 * @param[in] rdp       pointer to a @p MACReceiveDescriptor structure
 * @param[in] buf       pointer to the buffer that will receive the read data
 * @param[in] size      number of bytes to be read
 * @return              The number of bytes read from the descriptor's
 *                      stream, this value can be less than the amount
 *                      specified in the parameter @p size if there are
 *                      no more bytes to read.
 *
 * @notapi
 */
size_t mac_lld_read_receive_descriptor(MACReceiveDescriptor *rdp,
                                       uint8_t *buf,
                                       size_t size) {

  chDbgAssert(!(rdp->physdesc->rdes0 & LPC_RDES0_OWN),
              "mac_lld_read_receive_descriptor(), #1",
              "attempt to read descriptor already owned by DMA");

  if (size > rdp->size - rdp->offset)
    size = rdp->size - rdp->offset;

  if (size > 0) {
    memcpy(buf, (uint8_t *)(rdp->physdesc->rdes2) + rdp->offset, size);
    rdp->offset += size;
  }
  return size;
}

#if MAC_USE_ZERO_COPY || defined(__DOXYGEN__)
/**
 * @brief   Returns a pointer to the next transmit buffer in the descriptor
 *          chain.
 * @note    The API guarantees that enough buffers can be requested to fill
 *          a whole frame.
 *
 * @param[in] tdp       pointer to a @p MACTransmitDescriptor structure
 * @param[in] size      size of the requested buffer. Specify the frame size
 *                      on the first call then scale the value down subtracting
 *                      the amount of data already copied into the previous
 *                      buffers.
 * @param[out] sizep    pointer to variable receiving the buffer size, it is
 *                      zero when the last buffer has already been returned.
 *                      Note that a returned size lower than the amount
 *                      requested means that more buffers must be requested
 *                      in order to fill the frame data entirely.
 * @return              Pointer to the returned buffer.
 * @retval NULL         if the buffer chain has been entirely scanned.
 *
 * @notapi
 */
uint8_t *mac_lld_get_next_transmit_buffer(MACTransmitDescriptor *tdp,
                                          size_t size,
                                          size_t *sizep) {

  if (tdp->offset == 0) {
    *sizep      = tdp->size;
    tdp->offset = size;
    return (uint8_t *)tdp->physdesc->tdes2;
  }
  *sizep = 0;
  return NULL;
}

/**
 * @brief   Returns a pointer to the next receive buffer in the descriptor
 *          chain.
 * @note    The API guarantees that the descriptor chain contains a whole
 *          frame.
 *
 * @param[in] rdp       pointer to a @p MACReceiveDescriptor structure
 * @param[out] sizep    pointer to variable receiving the buffer size, it is
 *                      zero when the last buffer has already been returned.
 * @return              Pointer to the returned buffer.
 * @retval NULL         if the buffer chain has been entirely scanned.
 *
 * @notapi
 */
const uint8_t *mac_lld_get_next_receive_buffer(MACReceiveDescriptor *rdp,
                                               size_t *sizep) {

  if (rdp->size > 0) {
    *sizep      = rdp->size;
    rdp->offset = rdp->size;
    rdp->size   = 0;
    return (uint8_t *)rdp->physdesc->rdes2;
  }
  *sizep = 0;
  return NULL;
}
#endif /* MAC_USE_ZERO_COPY */

#endif /* HAL_USE_MAC */

/** @} */
