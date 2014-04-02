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
 * @file    RX62N/mac_lld.c
 * @brief   RX62N low level MAC driver code.
 *
 * @addtogroup MAC
 * @{
 */

#include <string.h>

#include "ch.h"
#include "hal.h"
#include "mii.h"
#include "rx62n_mii.h"

#if HAL_USE_MAC || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver local definitions.                                                 */
/*===========================================================================*/

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

__attribute__((aligned(16)))
static rx62n_eth_rx_descriptor_t rd[EDMAC_RECEIVE_DESCRIPTORS];
__attribute__((aligned(16)))
static rx62n_eth_tx_descriptor_t td[EDMAC_TRANSMIT_DESCRIPTORS];

__attribute__((aligned(32)))
static uint8_t rb[EDMAC_RECEIVE_DESCRIPTORS * EDMAC_RECEIVE_BUFFERS_SIZE];
__attribute__((aligned(32)))
static uint8_t tb[EDMAC_TRANSMIT_DESCRIPTORS * EDMAC_TRANSMIT_BUFFERS_SIZE];

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

#if !defined(BOARD_PHY_ADDRESS)
/**
 * @brief   PHY address detection.
 *
 * @param[in] macp      pointer to the @p MACDriver object
 */
static void mii_find_phy(MACDriver *macp) {
  uint32_t i;
#if RX62N_MAC_PHY_TIMEOUT > 0
  halrtcnt_t start = halGetCounterValue();
  halrtcnt_t timeout  = start + MS2RTT(RX62N_MAC_PHY_TIMEOUT);
  while (halIsCounterWithin(start, timeout)) {
#endif
    for (i = 0; i < 31; i++) {
      if ((miiGet(macp, MII_PHYSID1) == (BOARD_PHY_ID >> 16)) &&
          ((miiGet(macp, MII_PHYSID2) & 0xFFF0) == (BOARD_PHY_ID & 0xFFF0))) {
        return;
      }
    }
#if RX62N_MAC_PHY_TIMEOUT > 0
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
  ETHERC.MAHR      = ((uint32_t)p[0] << 24) |
                     ((uint32_t)p[1] << 16) |
                     ((uint32_t)p[2] << 8) |
                     ((uint32_t)p[3] << 0);
  ETHERC.MALR.LONG = ((uint32_t)p[4] << 8) |
                     ((uint32_t)p[5] << 0);
}

/*===========================================================================*/
/* Driver interrupt handlers.                                                */
/*===========================================================================*/

static void serve_interrupt_ether1(void) {
  uint32_t eesr = EDMAC.EESR.LONG;

  if (eesr & (1<<18)) {
    /* Frame received.*/
    chSysLockFromIsr();
    chSemResetI(&ETHD1.rdsem, 0);
#if MAC_USE_EVENTS
    chEvtBroadcastI(&ETHD1.rdevent);
#endif
    chSysUnlockFromIsr();
  }
  if (eesr & (1<<21)) {
    /* Frame transmitted.*/
    chSysLockFromIsr();
    chSemResetI(&ETHD1.tdsem, 0);
    chSysUnlockFromIsr();
  }
  EDMAC.EESR.LONG = 0x47FF0F9F;
}

CH_IRQ_HANDLER(Excep_ETHER_EINT) {

  CH_IRQ_PROLOGUE();

  serve_interrupt_ether1();

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

  macObjectInit(&ETHD1);
  ETHD1.link_up = FALSE;

  /* Selection of the RMII or MII mode based on info exported by board.h.*/
#if defined(BOARD_PHY_RMII)
  IOPORT.PFENET.BYTE = (1<<7)|(0<<4)|(1<<1);
#else
  IOPORT.PFENET.BYTE = (1<<7)|(1<<4)|(1<<1);
#endif

  /* MAC clocks temporary activation.*/
  MSTP(EDMAC) = 0;

  /* Reset of the MAC core.*/
  EDMAC.EDMR.BIT.SWR = 1;
  /* TODO: wait 64 cycles, here is 1ms delay */
  asm volatile ("mov.l   #24000,r2                                  \n\t"
                "1:                                                 \n\t"
                "sub     #1,r2                                      \n\t"
                "bne.b   1b                                         \n\t");
  EDMAC.EDMR.BIT.DE = 1;
  /* Clear registers not affected by reset */
  EDMAC.RMFCR.LONG = 0;
  EDMAC.TFUCR.LONG = 0;
  EDMAC.RFOCR.LONG = 0;

  /* PHY address setup.*/
#if defined(BOARD_PHY_ADDRESS)
  ETHD1.phyaddr = BOARD_PHY_ADDRESS;
#else
  mii_find_phy(&ETHD1);
#endif

#if defined(BOARD_PHY_RESET)
  /* PHY board-specific reset procedure.*/
  BOARD_PHY_RESET();
#else
  /* PHY soft reset procedure.*/
  miiPut(&ETHD1, MII_BMCR, BMCR_RESET);
#if defined(BOARD_PHY_RESET_DELAY)
  halPolledDelay(BOARD_PHY_RESET_DELAY);
#endif
  while (miiGet(&ETHD1, MII_BMCR) & BMCR_RESET)
    ;
#endif

#if RX62N_MAC_ETH1_CHANGE_PHY_STATE
  /* PHY in power down mode until the driver will be started.*/
  miiPut(&ETHD1, MII_BMCR, miiGet(&ETHD1, MII_BMCR) | BMCR_PDOWN);
#endif

  /* MAC clocks stopped again.*/
  MSTP(EDMAC) = 1;
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
  for (i = 0; i < EDMAC_RECEIVE_DESCRIPTORS; i++) {
    rd[i].rd0 = /*RX62N_RD0_RFP_ONE|*/RX62N_RD0_RACT;
    rd[i].rd1 = EDMAC_RECEIVE_BUFFERS_SIZE<<16;
    rd[i].rd2 = (uint32_t)&rb[i * EDMAC_RECEIVE_BUFFERS_SIZE];
  }
  rd[i-1].rd0 |= RX62N_RD0_RDLE;
  macp->rxptr = (rx62n_eth_rx_descriptor_t *)rd;
  for (i = 0; i < EDMAC_TRANSMIT_DESCRIPTORS; i++) {
    td[i].td0 = RX62N_TD0_TFP_ONE;
    td[i].td1 = EDMAC_TRANSMIT_BUFFERS_SIZE<<16;
    td[i].td2 = (uint32_t)&tb[i * EDMAC_TRANSMIT_BUFFERS_SIZE];
  }
  td[i-1].td0 |= RX62N_TD0_TDLE;
  macp->txptr = (rx62n_eth_tx_descriptor_t *)td;

  /* MAC clocks activation.*/
  MSTP(EDMAC) = 0;

#if RX62N_MAC_ETH1_CHANGE_PHY_STATE
  /* PHY in power up mode.*/
  miiPut(macp, MII_BMCR, miiGet(macp, MII_BMCR) & ~BMCR_PDOWN);
#endif

  /* MAC configuration.*/
  ETHERC.RFLR.LONG = RX62N_MAC_BUFFERS_SIZE;
  /* Clear all status flags.*/
  ETHERC.ECSR.LONG = 0x37;
  ETHERC.ECSIPR.LONG = 0;
  ETHERC.IPGR.LONG = 0x14; /* Initial value.*/

  /* MAC address setup.*/
  if (macp->config->mac_address == NULL)
    mac_lld_set_address(default_mac_address);
  else
    mac_lld_set_address(macp->config->mac_address);

  /* Transmitter and receiver enabled.
     Note that the complete setup of the MAC is performed when the link
     status is detected.*/
  ETHERC.ECMR.LONG |= (ETHERC_ECMR_RE | ETHERC_ECMR_TE);

  /* ISR vector enabled.*/
  IEN(ETHER,EINT) = 1;
  IPR(ETHER,EINT) = RX62N_MAC_ETH1_IRQ_PRIORITY;

  /* DMA configuration:
     Descriptor list pointers.*/
  EDMAC.RDLAR = (void *)rd;
  EDMAC.TDLAR = (void *)td;

  /* Clear all status flags.*/
  EDMAC.EESR.LONG = 0x47FF0F9F;
  EDMAC.TFTR.LONG = 0;
  /* Set FIFO size to 2048.*/
  EDMAC.FDR.LONG = 0x707;
  EDMAC.RMCR.LONG = 1;
  EDMAC.RPADIR.LONG = 0;

  EDMAC.EESIPR.LONG = (1<<21)|(1<<18);

  EDMAC.EDRRR.BIT.RR = 1;
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
#if RX62N_MAC_ETH1_CHANGE_PHY_STATE
    /* PHY in power down mode until the driver will be restarted.*/
    miiPut(macp, MII_BMCR, miiGet(macp, MII_BMCR) | BMCR_PDOWN);
#endif

    /* MAC and DMA stopped.*/
    ETHERC.ECMR.LONG = 0;

    /* MAC clocks stopped.*/
    MSTP(EDMAC) = 1;

    /* ISR vector disabled.*/
    IEN(ETHER,EINT) = 0;
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
  rx62n_eth_tx_descriptor_t *tdes;

  if (!macp->link_up)
    return RDY_TIMEOUT;

  chSysLock();

  /* Get Current TX descriptor.*/
  tdes = macp->txptr;

  /* Ensure that descriptor isn't owned by the Ethernet DMA or locked by
     another thread.*/
  if ( (tdes->td0 & RX62N_TD0_TACT) || (tdes->td0 & RX62N_TD0_TLOCKED) ) {
    chSysUnlock();
    return RDY_TIMEOUT;
  }

  /* Marks the current descriptor as locked using a reserved bit.*/
  tdes->td0 |= RX62N_TD0_TLOCKED;

  /* Next TX descriptor to use.*/
  if (++macp->txptr >= &td[RX62N_MAC_TRANSMIT_BUFFERS]) {
    macp->txptr = &td[0];
  }

  chSysUnlock();

  /* Set the buffer size and configuration.*/
  tdp->offset   = 0;
  tdp->size     = RX62N_MAC_BUFFERS_SIZE;
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

  chSysLock();

  /* Unlocks the descriptor and returns it to the DMA engine.*/
  tdp->physdesc->td1 = tdp->offset<<16;
  tdp->physdesc->td0 &= ~RX62N_TD0_TLOCKED;
  tdp->physdesc->td0 |= RX62N_TD0_TACT;
  if (EDMAC.EDTRR.BIT.TR == 0)
    EDMAC.EDTRR.BIT.TR = 1;

  chSysUnlock();
}

/**
 * @brief   Cleans an incomplete frame.
 *
 * @param[in] from      the start position of the incomplete frame
 */
static void cleanup(MACDriver *macp, rx62n_eth_rx_descriptor_t *from) {

  while (from != macp->rxptr) {
    from->rd0 = (from->rd0 & RX62N_RD0_RDLE) | RX62N_RD0_RACT;
    if (++from >= &rd[EDMAC_RECEIVE_DESCRIPTORS])
      from = rd;
  }
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

  unsigned n;
  rx62n_eth_rx_descriptor_t *edp;
  n = EDMAC_RECEIVE_DESCRIPTORS;
  chSysLock();

  /*
   * Skips active buffers, if any.
   */
skip:
  while ((n > 0) && (macp->rxptr->rd0 & RX62N_RD0_RACT)) {
    if (++macp->rxptr >= &rd[EDMAC_RECEIVE_DESCRIPTORS])
      macp->rxptr = rd;
    n--;
  }
#if 0
  /*
   * Skips fragments, if any, cleaning them up.
   */
  while ((n > 0) && !(macp->rxptr->rd0 & RX62N_RD0_RACT) &&
                    !(macp->rxptr->rd0 & RX62N_RD0_RFP_SOF)) {
    macp->rxptr->rd0 = (macp->rxptr->rd0 & RX62N_RD0_RDLE) | RX62N_RD0_RACT;
    if (++macp->rxptr >= &rd[EDMAC_RECEIVE_DESCRIPTORS])
      macp->rxptr = rd;
    n--;
  }
#endif
  /*
   * Now compute the total frame size skipping eventual incomplete frames
   * or holes...
   */
restart:
  edp = macp->rxptr;
  while (n > 0) {

    if (macp->rxptr->rd0 & RX62N_RD0_RACT) {
      /* Empty buffer for some reason... cleaning up the incomplete frame.*/
      cleanup(macp, edp);
      goto skip;
    }
    if ( ( macp->rxptr->rd0 & (RX62N_RD0_RFP_EOF|RX62N_RD0_RFE) )
          == (RX62N_RD0_RFP_EOF|RX62N_RD0_RFE) ) {
      /* End Of Frame found, but erroneous, so cleaning.*/
      if (++macp->rxptr >= &rd[EDMAC_RECEIVE_DESCRIPTORS])
        macp->rxptr = rd;
      n++;
      cleanup(macp, edp);
      goto skip;
    }
    /*
     * End Of Frame found.
     */
    if (macp->rxptr->rd0 & RX62N_RD0_RFP_EOF) {
      rdp->offset = 0;
      rdp->size = macp->rxptr->rd1 & 0xFFFF;
      rdp->physdesc = edp;
      if (++macp->rxptr >= &rd[EDMAC_RECEIVE_DESCRIPTORS])
        macp->rxptr = rd;
      if (EDMAC.EDRRR.BIT.RR == 0)
        EDMAC.EDRRR.BIT.RR = 1;
      chSysUnlock();
      return RDY_OK;
    }

    if ( (macp->rxptr != edp) && (macp->rxptr->rd0 & RX62N_RD0_RFP_MASK) ) {
      /* Found another start or ... cleaning up the incomplete frame.*/
      cleanup(macp, edp);
      goto restart;     /* Another start buffer for some reason... */
    }

    if (++macp->rxptr >= &rd[EDMAC_RECEIVE_DESCRIPTORS])
      macp->rxptr = rd;
    n--;
  }
  if (EDMAC.EDRRR.BIT.RR == 0)
    EDMAC.EDRRR.BIT.RR = 1;
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

  bool_t done;
  rx62n_eth_rx_descriptor_t *edp = rdp->physdesc;

  unsigned n = EDMAC_RECEIVE_DESCRIPTORS;
  chSysLock();
  do {
    done = ((edp->rd0 & RX62N_RD0_RFP_EOF) != 0);
    edp->rd1 = EDMAC_RECEIVE_BUFFERS_SIZE<<16;
    edp->rd0 = (edp->rd0 & RX62N_RD0_RDLE) | RX62N_RD0_RACT;
    if (++edp >= &rd[EDMAC_RECEIVE_DESCRIPTORS])
      edp = rd;
    n--;
  }
  while ((n > 0) && !done);
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
  uint32_t ecmr, bmsr, bmcr;

  ecmr = ETHERC.ECMR.LONG;

  /* PHY CR and SR registers read.*/
  (void)miiGet(macp, MII_BMSR);
  bmsr = miiGet(macp, MII_BMSR);
  bmcr = miiGet(macp, MII_BMCR);

  /* Check on auto-negotiation mode.*/
  if (bmcr & BMCR_ANENABLE) {
    uint32_t lpa;
    /* Auto-negotiation must be finished without faults and link established.*/
    if ((bmsr & (BMSR_LSTATUS | BMSR_RFAULT | BMSR_ANEGCOMPLETE)) !=
        (BMSR_LSTATUS | BMSR_ANEGCOMPLETE))
      return macp->link_up = FALSE;

    /* Auto-negotiation enabled, checks the LPA register.*/
    lpa = miiGet(macp, MII_LPA);

    /* Check on link speed.*/
    if (lpa & (LPA_100HALF | LPA_100FULL | LPA_100BASE4))
      ecmr |= ETHERC_ECMR_RTM;
    else
      ecmr &= ~ETHERC_ECMR_RTM;

    /* Check on link mode.*/
    if (lpa & (LPA_10FULL | LPA_100FULL))
      ecmr |= ETHERC_ECMR_DM;
    else
      ecmr &= ~ETHERC_ECMR_DM;
  }
  else {
    /* Link must be established.*/
    if (!(bmsr & BMSR_LSTATUS))
      return macp->link_up = FALSE;

    /* Check on link speed.*/
    if (bmcr & BMCR_SPEED100)
      ecmr |= ETHERC_ECMR_RTM;
    else
      ecmr &= ~ETHERC_ECMR_RTM;

    /* Check on link mode.*/
    if (bmcr & BMCR_FULLDPLX)
      ecmr |= ETHERC_ECMR_DM;
    else
      ecmr &= ~ETHERC_ECMR_DM;
  }

  /* Changes the mode in the MAC.*/
  ETHERC.ECMR.LONG = ecmr;

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

  chDbgAssert(!(tdp->physdesc->td0 & RX62N_TD0_TACT),
              "mac_lld_write_transmit_descriptor(), #1",
              "attempt to write descriptor already owned by DMA");

  if (size > tdp->size - tdp->offset)
    size = tdp->size - tdp->offset;

  if (size > 0) {
    memcpy((uint8_t *)(tdp->physdesc->td2) + tdp->offset, buf, size);
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
/*
  chDbgAssert(!(rdp->physdesc->rd0 & RX62N_RD0_RACT),
              "mac_lld_read_receive_descriptor(), #1",
              "attempt to read descriptor already owned by DMA");
*/
  if (size > rdp->size - rdp->offset)
    size = rdp->size - rdp->offset;
  if (size > 0) {
    uint8_t *src = (uint8_t *)(rdp->physdesc->rd2) +
                   rdp->offset;
    uint8_t *limit = (uint8_t *)&rb[EDMAC_RECEIVE_DESCRIPTORS * EDMAC_RECEIVE_BUFFERS_SIZE];
    if (src >= limit) {
      src -= EDMAC_RECEIVE_DESCRIPTORS * EDMAC_RECEIVE_BUFFERS_SIZE;
    }
    if (src + size > limit ) {
      memcpy(buf, src, (size_t)(limit - src));
      memcpy(buf + (size_t)(limit - src), rb, size - (size_t)(limit - src));
    }
    else
      memcpy(buf, src, size);
    rdp->offset += size;
  }
  return size;

}

#endif /* HAL_USE_MAC */

/** @} */
