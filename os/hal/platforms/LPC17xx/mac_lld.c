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
 * @file    LPC17xx/mac_lld.c
 * @brief   LPC17xx low level MAC driver code.
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

#define BUFFER_SIZE ((((LPC17xx_MAC_BUFFERS_SIZE - 1) | 3) + 1) / 4)

/* MII divider optimal value.*/
#if (LPC17xx_CCLK <= 50000000)
#define MAC_MII_MCFG_CLK_SEL      EMAC_MCFG_CLK_DIV_20
#elif (LPC17xx_CCLK <= 70000000)
#define MAC_MII_MCFG_CLK_SEL      EMAC_MCFG_CLK_DIV_28
#elif (LPC17xx_CCLK <= 80000000)
#define MAC_MII_MCFG_CLK_SEL      EMAC_MCFG_CLK_DIV_36
#elif (LPC17xx_CCLK <= 90000000)
#define MAC_MII_MCFG_CLK_SEL      EMAC_MCFG_CLK_DIV_40
#elif (LPC17xx_CCLK <= 100000000)
#define MAC_MII_MCFG_CLK_SEL      EMAC_MCFG_CLK_DIV_44
#elif (LPC17xx_CCLK <= 120000000)
#define MAC_MII_MCFG_CLK_SEL      EMAC_MCFG_CLK_DIV_48
#else
#error "LPC17xx_CCLK over maximum frequency for ETH operations (120MHz)"
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

static lpc17xx_eth_rx_descriptor_t rd[LPC17xx_MAC_RECEIVE_BUFFERS]
  __attribute__((aligned(4))) __attribute__((section(".eth_ram")));
static lpc17xx_eth_tx_descriptor_t td[LPC17xx_MAC_TRANSMIT_BUFFERS]
  __attribute__((aligned(4))) __attribute__((section(".eth_ram")));

static lpc17xx_eth_tx_descriptor_t td_tmp[LPC17xx_MAC_TRANSMIT_BUFFERS]
  __attribute__((aligned(4))) __attribute__((section(".eth_ram")));

static lpc17xx_eth_rx_status_t rd_stat[LPC17xx_MAC_RECEIVE_BUFFERS]
  __attribute__((aligned(8))) __attribute__((section(".eth_ram")));
static lpc17xx_eth_tx_status_t td_stat[LPC17xx_MAC_TRANSMIT_BUFFERS]
  __attribute__((aligned(4))) __attribute__((section(".eth_ram")));

static uint32_t rb[LPC17xx_MAC_RECEIVE_BUFFERS][BUFFER_SIZE]
  __attribute__((aligned(4))) __attribute__((section(".eth_ram")));
static uint32_t tb[LPC17xx_MAC_TRANSMIT_BUFFERS][BUFFER_SIZE]
  __attribute__((aligned(4))) __attribute__((section(".eth_ram")));

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

/**
 * @brief   Writes a PHY register.
 *
 * @param[in] macp      pointer to the @p MACDriver object
 * @param[in] reg       register number
 * @param[in] value     new register value
 */
void mii_write(MACDriver *macp, uint32_t reg, uint32_t value) {

  LPC_EMAC->MCMD = 0;
  LPC_EMAC->MADR = macp->phyaddr | reg; /* Write PHY address and register address */
  LPC_EMAC->MWTD = value;               /* Write data */
  while((LPC_EMAC->MIND & EMAC_MIND_BUSY) != 0)
    ;                                   /* Wait for busy bit to be cleared in MIND */

}

/**
 * @brief   Reads a PHY register.
 *
 * @param[in] macp      pointer to the @p MACDriver object
 * @param[in] reg       register number
 *
 * @return              The PHY register content.
 */
uint32_t mii_read(MACDriver *macp, uint32_t reg) {

  LPC_EMAC->MCMD = 1;
  LPC_EMAC->MADR = macp->phyaddr | reg; /* Write PHY address and register address */
  while((LPC_EMAC->MIND & EMAC_MIND_BUSY) != 0)
    ;                                   /* Wait for busy bit to be cleared in MIND */
  LPC_EMAC->MCMD = 0;
  return LPC_EMAC->MRDD;
}

#if !defined(BOARD_PHY_ADDRESS)
/**
 * @brief   PHY address detection.
 *
 * @param[in] macp      pointer to the @p MACDriver object
 */
void mii_find_phy(MACDriver *macp) {
  uint32_t i;

#if LPC17xx_MAC_PHY_TIMEOUT > 0
  halrtcnt_t start = halGetCounterValue();
  halrtcnt_t timeout  = start + MS2RTT(LPC17xx_MAC_PHY_TIMEOUT);
  while (halIsCounterWithin(start, timeout)) {
#endif
    for (i = 0; i < 31; i++) {
      macp->phyaddr = i << 8;
      if ((mii_read(macp, MII_PHYSID1) == (BOARD_PHY_ID >> 16)) &&
          ((mii_read(macp, MII_PHYSID2) & 0xFFF0) == (BOARD_PHY_ID & 0xFFF0))) {
        return;
      }
    }
#if LPC17xx_MAC_PHY_TIMEOUT > 0
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
static void mac_lld_set_address(const uint8_t *p) {

  /* MAC address configuration, only a single address comparator is used,
     hash table not used.*/

  LPC_EMAC->SA0 = ((uint32_t)p[5] << 8) |
                  ((uint32_t)p[4] << 0);
  LPC_EMAC->SA1 = ((uint32_t)p[3] << 8) |
                  ((uint32_t)p[2] << 0);
  LPC_EMAC->SA2 = ((uint32_t)p[1] << 8) |
                  ((uint32_t)p[0] << 0);

  LPC_EMAC->HashFilterL = 0;
  LPC_EMAC->HashFilterH = 0;

  LPC_EMAC->RxFilterCtrl = EMAC_RXFILCTRL_PERFECT_EN | EMAC_RXFILCTRL_BROADCAST_EN;

}

/*===========================================================================*/
/* Driver interrupt handlers.                                                */
/*===========================================================================*/

CH_IRQ_HANDLER(VectorB0) {

  uint32_t intstatus;
  uint32_t consume_index;
  uint32_t txdescn;

  CH_IRQ_PROLOGUE();

  intstatus = LPC_EMAC->IntStatus;
  LPC_EMAC->IntClear = intstatus;

  if (intstatus & EMAC_INTSTAT_RX_OVERRUN) {
    /* Reset receive logic. */
    LPC_EMAC->MAC1 |= EMAC_MAC1_RESET_RX | EMAC_MAC1_RESET_MCS_RX;
    __NOP();
    LPC_EMAC->MAC1 &= ~(EMAC_MAC1_RESET_RX | EMAC_MAC1_RESET_MCS_RX);
  }

  if (intstatus & EMAC_INTSTAT_TX_UNDERRUN) {
    /* Reset transmit logic. */
    LPC_EMAC->MAC1 |= EMAC_MAC1_RESET_TX | EMAC_MAC1_RESET_MCS_TX;
    __NOP();
    LPC_EMAC->MAC1 &= ~(EMAC_MAC1_RESET_TX | EMAC_MAC1_RESET_MCS_TX);
  }

  if (intstatus & EMAC_INTSTAT_RX_DONE) {
    /* Data Received.*/
    chSysLockFromIsr();
    chSemResetI(&ETHD1.rdsem, 0);
#if MAC_USE_EVENTS
    chEvtBroadcastI(&ETHD1.rdevent);
#endif
    chSysUnlockFromIsr();
  }

  if (intstatus & EMAC_INTSTAT_TX_DONE) {
    /* Data Transmitted.*/
    consume_index = LPC_EMAC->TxConsumeIndex;
    if (consume_index == 0)
      consume_index = LPC17xx_MAC_TRANSMIT_BUFFERS - 1;
    else
      consume_index--;
    txdescn = (td[consume_index].control >> 12) & 0x000000FF;
    td_tmp[txdescn].control = 0;    /* Unlock temporary descriptor. */
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

  LPC_SC->PCONP |= (1 << 30);  /* Power up MAC */

  /* Rx desciptor and Rx status tables are initialized. */
  for (i = 0; i < LPC17xx_MAC_RECEIVE_BUFFERS; i++) {
    rd[i].packet = (uint32_t) &rb[i];
    rd[i].control = EMAC_RXSTATUS_CTRL_INT | (LPC17xx_MAC_BUFFERS_SIZE - 1);

    rd_stat[i].info = 0;
    rd_stat[i].hashcrc = 0;
  }

  /* Tx desciptor and Tx status tables are cleared. They are set up every time
  when transmission starts. */
  for (i = 0; i < LPC17xx_MAC_TRANSMIT_BUFFERS; i++) {
    td[i].packet = 0;
    td[i].control = 0;
    td_stat[i] = 0;

    td_tmp[i].packet = (uint32_t) &tb[i];
    td_tmp[i].control = 0;
  }

  /* Reset all EMAC internal modules. */
  LPC_EMAC->MAC1 = EMAC_MAC1_RESET_TX | EMAC_MAC1_RESET_MCS_TX | EMAC_MAC1_RESET_RX |
      EMAC_MAC1_RESET_MCS_RX  | EMAC_MAC1_SIM_RESET | EMAC_MAC1_SOFT_RESET;
  LPC_EMAC->Command = EMAC_COMMAND_REG_RESET | EMAC_COMMAND_TX_RESET | EMAC_COMMAND_RX_RESET;
  LPC_EMAC->MCFG = EMAC_MCFG_RESET_MII_MGMT;        /* Reset MII Managment hardware. */

  halPolledDelay(LPC17XX_MAC_RESET_DELAY);

  LPC_EMAC->MAC1 = 0;                           /* MAC reset de-asserted. */

  /* Selection of the RMII or MII mode based on info exported by board.h.*/
#if defined(BOARD_PHY_RMII)
  LPC_EMAC->Command = EMAC_COMMAND_RMII | EMAC_COMMAND_PASS_RUN_FRAME;
#else
  LPC_EMAC->Command = EMAC_COMMAND_PASS_RUN_FRAME;
#endif

  LPC_EMAC->MCFG = MAC_MII_MCFG_CLK_SEL;        /* Set MII clock divider. */

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

}

/**
 * @brief   Configures and activates the MAC peripheral.
 *
 * @param[in] macp      pointer to the @p MACDriver object
 *
 * @notapi
 */
void mac_lld_start(MACDriver *macp) {

  /* PHY in power up mode.*/
  mii_write(macp, MII_BMCR, mii_read(macp, MII_BMCR) & ~BMCR_PDOWN);

  /* MAC registers configuration.*/
  LPC_EMAC->MAC1 = EMAC_MAC1_PASS_ALL_REC;
  LPC_EMAC->MAC2 = EMAC_MAC2_CRC_EN | EMAC_MAC2_PAD_CRC_EN;
  LPC_EMAC->MAXF = LPC17XX_MAC_MAX_FLEN;
  LPC_EMAC->CLRT = LPC17XX_MAC_CLRT_DEF;
  LPC_EMAC->IPGR = LPC17XX_MAC_IPGR_DEF;

  /* MAC descriptor registers configuration. */
  macp->rxsoftindex = 0;
  LPC_EMAC->RxDescriptor = (uint32_t) &rd[0];
  LPC_EMAC->RxDescriptorNumber = LPC17xx_MAC_RECEIVE_BUFFERS - 1;
  LPC_EMAC->RxConsumeIndex = 0;
  LPC_EMAC->RxStatus = (uint32_t) &rd_stat[0];

  macp->txsoftindex = 0;
  LPC_EMAC->TxDescriptor = (uint32_t) &td[0];
  LPC_EMAC->TxDescriptorNumber = LPC17xx_MAC_TRANSMIT_BUFFERS - 1;
  LPC_EMAC->TxProduceIndex = 0;
  LPC_EMAC->TxStatus = (uint32_t) &td_stat[0];

  /* MAC address setup.*/
  if (macp->config->mac_address == NULL)
    mac_lld_set_address(default_mac_address);
  else
    mac_lld_set_address(macp->config->mac_address);

  /* Enable EMAC interrupts. */
  LPC_EMAC->IntEnable = EMAC_INTEN_TX_DONE | EMAC_INTEN_RX_DONE |
      EMAC_INTEN_RX_OVERRUN | EMAC_INTEN_TX_UNDERRUN;

  /* Reset all interrupts. */
  LPC_EMAC->IntClear  = 0xFFFF;

  /* ISR vector enabled.*/
  nvicEnableVector(ENET_IRQn, CORTEX_PRIORITY_MASK(LPC17xx_MAC_ETH_IRQ_PRIORITY));

  /* Enable receive and transmit mode. */
  LPC_EMAC->Command  |= EMAC_COMMAND_TX_ENABLE | EMAC_COMMAND_RX_ENABLE;
  LPC_EMAC->MAC1     |= EMAC_MAC1_RECEIVE_EN;

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
    LPC_EMAC->MAC1 = 0;
    LPC_EMAC->Command = 0;
    LPC_EMAC->IntEnable = 0;

    /* PHY in power down mode until the driver will be restarted.*/
    mii_write(macp, MII_BMCR, mii_read(macp, MII_BMCR) | BMCR_PDOWN);

    /* ISR vector disabled.*/
    nvicDisableVector(ENET_IRQn);
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
  uint32_t produce_index;
  uint32_t produce_index_next;

  if (!macp->link_up)
    return RDY_TIMEOUT;

  chSysLock();

  /* Get Current TX descriptor.*/
  produce_index = macp->txsoftindex;

  produce_index_next = (produce_index + 1) % LPC17xx_MAC_TRANSMIT_BUFFERS;
  if (produce_index_next == LPC_EMAC->TxConsumeIndex) {
    /* Full */
    chSysUnlock();
    return RDY_TIMEOUT;
  }

  /* Ensure that descriptor isn't locked by the Ethernet DMA or
     another thread.*/
  if (td_tmp[produce_index].control == EMAC_TXSTATUS_CTRL_LOCK) {
    chSysUnlock();
    return RDY_TIMEOUT;
  }

  /* Marks the current descriptor as locked using a reserved bit.*/
  td_tmp[produce_index].control = EMAC_TXSTATUS_CTRL_LOCK;

  /* Next TX descriptor to use.*/
  macp->txsoftindex = produce_index_next;

  chSysUnlock();

  /* Set the buffer size and configuration.*/
  tdp->offset = 0;
  tdp->size = LPC17xx_MAC_BUFFERS_SIZE;
  tdp->txdescn = produce_index;

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

  uint32_t produce_index;
  uint32_t descn;

  chSysLock();

  produce_index = LPC_EMAC->TxProduceIndex;
  descn = tdp->txdescn;

  /* Set control bits and save temporary descriptor number. */
  td[produce_index].control = EMAC_TXSTATUS_CTRL_INT | EMAC_TXSTATUS_CTRL_LAST | (descn << 12) |
     (tdp->offset - 1);

  td[produce_index].packet = td_tmp[descn].packet;
  LPC_EMAC->TxProduceIndex = (LPC_EMAC->TxProduceIndex + 1) % LPC17xx_MAC_TRANSMIT_BUFFERS;
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
  uint32_t consume_index;
  uint32_t status;

  chSysLock();

  /* Get current rx descriptor number.*/
  consume_index = macp->rxsoftindex;

  if (LPC_EMAC->RxProduceIndex != consume_index) {
    status = rd_stat[consume_index].info;
    if (status != EMAC_RXSTATUS_INFO_ERROR) {
      /* Found a valid one.*/
      rdp->offset   = 0;
      rdp->size     = (status & EMAC_RXSTATUS_INFO_SIZE_MASK) + 1;
      rdp->rxdescn = consume_index;

      macp->rxsoftindex = (consume_index + 1) % LPC17xx_MAC_RECEIVE_BUFFERS;

      chSysUnlock();
      return RDY_OK;
    }
    else {
      /* Invalid frame found. */
      if (LPC_EMAC->RxConsumeIndex == consume_index) {
        consume_index = (consume_index + 1) % LPC17xx_MAC_RECEIVE_BUFFERS;
        LPC_EMAC->RxConsumeIndex = consume_index;
      }
      else {
        rd[consume_index].control |= EMAC_RXSTATUS_CTRL_READY;
        consume_index = (consume_index + 1) % LPC17xx_MAC_RECEIVE_BUFFERS;
      }
      macp->rxsoftindex = consume_index;
    }
  }

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

  uint32_t consume_index;

  chSysLock();

  /* Only descriptor with number match RxConsumeIndex can release receive descriptors. */
  consume_index = LPC_EMAC->RxConsumeIndex;

  if (rdp->rxdescn == consume_index) {
    consume_index = (consume_index + 1) % LPC17xx_MAC_RECEIVE_BUFFERS;

    while (LPC_EMAC->RxProduceIndex != consume_index) {
      if (rd[consume_index].control & EMAC_RXSTATUS_CTRL_READY) {
        rd[consume_index].control &= ~EMAC_RXSTATUS_CTRL_READY;
        consume_index = (consume_index + 1) % LPC17xx_MAC_RECEIVE_BUFFERS;
      }
      else {
        break;
      }
    }

  LPC_EMAC->RxConsumeIndex = consume_index;
  }
  else {
    rd[rdp->rxdescn].control |= EMAC_RXSTATUS_CTRL_READY;
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
  uint32_t mac2, cmd, ipgt, supp, bmsr, bmcr;

  mac2 = LPC_EMAC->MAC2;
  cmd = LPC_EMAC->Command;

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
      supp = EMAC_SUPP_SPEED;
    else
      supp = 0;

    /* Check on link mode.*/
    if (lpa & (LPA_10FULL | LPA_100FULL)) {
      mac2 |= EMAC_MAC2_FULL_DUPLEX;
      cmd  |= EMAC_COMMAND_FULL_DUPLEX;
      ipgt = LPC17XX_MAC_IPGT_FULL_DUPLEX_DEF;
    }
    else {
      mac2 &= ~EMAC_MAC2_FULL_DUPLEX;
      cmd  &= ~EMAC_COMMAND_FULL_DUPLEX;
      ipgt = LPC17XX_MAC_IPGT_HALF_DUPLEX_DEF;
    }
  }
  else {
    /* Link must be established.*/
    if (!(bmsr & BMSR_LSTATUS))
      return macp->link_up = FALSE;

    /* Check on link speed.*/
    if (bmcr & BMCR_SPEED100)
      supp = EMAC_SUPP_SPEED;
    else
      supp = 0;

    /* Check on link mode.*/
    if (bmcr & BMCR_FULLDPLX) {
      mac2 |= EMAC_MAC2_FULL_DUPLEX;
      cmd  |= EMAC_COMMAND_FULL_DUPLEX;
      ipgt = LPC17XX_MAC_IPGT_FULL_DUPLEX_DEF;
    }
    else {
      mac2 &= ~EMAC_MAC2_FULL_DUPLEX;
      cmd  &= ~EMAC_COMMAND_FULL_DUPLEX;
      ipgt = LPC17XX_MAC_IPGT_HALF_DUPLEX_DEF;
    }
  }

  LPC_EMAC->MAC2 = mac2;
  LPC_EMAC->SUPP = supp;
  LPC_EMAC->Command = cmd;
  LPC_EMAC->IPGT = ipgt;

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

  if (size > tdp->size - tdp->offset)
    size = tdp->size - tdp->offset;

  if (size > 0) {
    memcpy((uint8_t *)td_tmp[tdp->txdescn].packet + tdp->offset, buf, size);
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

  if (size > rdp->size - rdp->offset)
    size = rdp->size - rdp->offset;

  if (size > 0) {
    memcpy(buf, (uint8_t *)rd[rdp->rxdescn].packet + rdp->offset, size);
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
    return (uint8_t *)td_tmp[tdp->txdescn].packet;
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
    return (uint8_t *)rd[rdp->rxdescn].packet;
  }
  *sizep = 0;
  return NULL;
}
#endif /* MAC_USE_ZERO_COPY */

#endif /* HAL_USE_MAC */

/** @} */
