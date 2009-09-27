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
 * @file templates/mac_lld.c
 * @brief MAC Driver subsystem low level driver source template
 * @addtogroup MAC_LLD
 * @{
 */

#include <ch.h>
#include <mac.h>
#include <phy.h>

#include "mii.h"
#include "at91lib/aic.h"

/**
 * @brief EMAC object.
 */
MACDriver MAC1;

#define EMAC_PIN_MASK (AT91C_PB0_ETXCK_EREFCK  | AT91C_PB1_ETXEN         | \
                       AT91C_PB2_ETX0          | AT91C_PB3_ETX1          | \
                       AT91C_PB4_ECRS          | AT91C_PB5_ERX0          | \
                       AT91C_PB6_ERX1          | AT91C_PB7_ERXER         | \
                       AT91C_PB8_EMDC          | AT91C_PB9_EMDIO         | \
                       AT91C_PB10_ETX2         | AT91C_PB11_ETX3         | \
                       AT91C_PB12_ETXER        | AT91C_PB13_ERX2         | \
                       AT91C_PB14_ERX3         | AT91C_PB15_ERXDV_ECRSDV | \
                       AT91C_PB16_ECOL         | AT91C_PB17_ERXCK)

#define RSR_BITS (AT91C_EMAC_BNA | AT91C_EMAC_REC | AT91C_EMAC_OVR)

#define TSR_BITS (AT91C_EMAC_UBR | AT91C_EMAC_COL | AT91C_EMAC_RLES | \
                  AT91C_EMAC_BEX | AT91C_EMAC_COMP | AT91C_EMAC_UND)

#ifndef __DOXYGEN__
static bool_t link_up;

static uint8_t default_mac[] = {0xAA, 0x55, 0x13, 0x37, 0x01, 0x10};

static MACReceiveDescriptor rd[EMAC_RECEIVE_BUFFERS] __attribute__((aligned(8)));
static uint8_t rb[EMAC_RECEIVE_BUFFERS * EMAC_RECEIVE_BUFFERS_SIZE] __attribute__((aligned(8)));
static MACReceiveDescriptor *rxptr;

static MACTransmitDescriptor td[EMAC_TRANSMIT_BUFFERS] __attribute__((aligned(8)));
static uint8_t tb[EMAC_TRANSMIT_BUFFERS * EMAC_TRANSMIT_BUFFERS_SIZE] __attribute__((aligned(8)));
static MACTransmitDescriptor *txptr;
#endif

/**
 * @brief IRQ handler.
 */
/** @cond never*/
__attribute__((noinline))
/** @endcond*/
static void serve_interrupt(void) {
  uint32_t isr, rsr, tsr;

  /* Fix for the EMAC errata */
  isr = AT91C_BASE_EMAC->EMAC_ISR;
  rsr = AT91C_BASE_EMAC->EMAC_RSR;
  tsr = AT91C_BASE_EMAC->EMAC_TSR;

  if ((isr & AT91C_EMAC_RCOMP) || (rsr & RSR_BITS)) {
    if (rsr & AT91C_EMAC_REC) {
      chSysLockFromIsr();
      chSemSignalI(&MAC1.md_rdsem);
      chSysUnlockFromIsr();
    }
    AT91C_BASE_EMAC->EMAC_RSR = RSR_BITS;
  }

  if ((isr & AT91C_EMAC_TCOMP) || (tsr & TSR_BITS)) {
    if (tsr & AT91C_EMAC_COMP) {
      chSysLockFromIsr();
      chSemSignalI(&MAC1.md_tdsem);
      chSysUnlockFromIsr();
    }
    AT91C_BASE_EMAC->EMAC_TSR = TSR_BITS;
  }
  AT91C_BASE_AIC->AIC_EOICR = 0;
}

/**
 * @brief EMAC IRQ veneer handler.
 */
CH_IRQ_HANDLER(irq_handler) {

  CH_IRQ_PROLOGUE();

  serve_interrupt();

  CH_IRQ_EPILOGUE();
}

/**
 * @brief Low level MAC initialization.
 */
void mac_lld_init(void) {
  unsigned i;

  macObjectInit(&MAC1);

  /*
   * Buffers initialization.
   */
  for (i = 0; i < EMAC_RECEIVE_BUFFERS; i++) {
    rd[i].w1 = (uint32_t)&rb[i * EMAC_RECEIVE_BUFFERS_SIZE];
    rd[i].w2 = 0;
  }
  rd[EMAC_RECEIVE_BUFFERS - 1].w1 |= W1_R_WRAP;
  rxptr = rd;
  for (i = 0; i < EMAC_TRANSMIT_BUFFERS; i++) {
    td[i].w1 = (uint32_t)&tb[i * EMAC_TRANSMIT_BUFFERS_SIZE];
    td[i].w2 = EMAC_TRANSMIT_BUFFERS_SIZE | W2_T_LAST_BUFFER | W2_T_USED;
  }
  td[EMAC_TRANSMIT_BUFFERS - 1].w2 |= W2_T_WRAP;
  txptr = td;

  /*
   * Associated PHY initialization.
   */
  phyReset(&MAC1);

  /*
   * EMAC pins setup and clock enable. Note, PB18 is not included because it is
   * used as #PD control and not as EF100.
   */
  AT91C_BASE_PMC->PMC_PCER = 1 << AT91C_ID_EMAC;
  AT91C_BASE_PIOB->PIO_ASR = EMAC_PIN_MASK;
  AT91C_BASE_PIOB->PIO_PDR = EMAC_PIN_MASK;
  AT91C_BASE_PIOB->PIO_PPUDR = EMAC_PIN_MASK;

  /*
   * EMAC Initial setup.
   */
  AT91C_BASE_EMAC->EMAC_NCR = 0;                /* Stopped but MCE active.*/
  AT91C_BASE_EMAC->EMAC_NCFGR = 2 << 10;        /* MDC-CLK = MCK / 32 */
  AT91C_BASE_EMAC->EMAC_USRIO = AT91C_EMAC_CLKEN;/* Enable EMAC in MII mode.*/
  AT91C_BASE_EMAC->EMAC_RBQP = (AT91_REG)rd;    /* RX descriptors list.*/
  AT91C_BASE_EMAC->EMAC_TBQP = (AT91_REG)td;    /* TX descriptors list.*/
  AT91C_BASE_EMAC->EMAC_RSR = AT91C_EMAC_OVR |
                              AT91C_EMAC_REC |
                              AT91C_EMAC_BNA;   /* Clears RSR.*/
  AT91C_BASE_EMAC->EMAC_NCFGR |= AT91C_EMAC_DRFCS;/* Initial NCFGR settings.*/
  AT91C_BASE_EMAC->EMAC_NCR |= AT91C_EMAC_TE |
                               AT91C_EMAC_RE |
                               AT91C_EMAC_CLRSTAT;/* Initial NCR settings.*/
  mac_lld_set_address(&MAC1, default_mac);

#if PHY_HARDWARE == PHY_MICREL_KS8721
  /*
   * PHY device identification.
   */
  AT91C_BASE_EMAC->EMAC_NCR |= AT91C_EMAC_MPE;
  if ((phyGet(&MAC1, MII_PHYSID1) != (MII_KS8721_ID >> 16)) ||
      ((phyGet(&MAC1, MII_PHYSID2) & 0xFFF0) != (MII_KS8721_ID & 0xFFF0)))
    chSysHalt();
  AT91C_BASE_EMAC->EMAC_NCR &= ~AT91C_EMAC_MPE;
#endif

  /*
   * Interrupt configuration.
   */
  AIC_ConfigureIT(AT91C_ID_EMAC,
                  AT91C_AIC_SRCTYPE_INT_HIGH_LEVEL | EMAC_INTERRUPT_PRIORITY,
                  irq_handler);
  AIC_EnableIT(AT91C_ID_EMAC);
}

/**
 * @brief Low level MAC address setup.
 *
 * @param[in] macp pointer to the @p MACDriver object
 * @param[in] p pointer to a six bytes buffer containing the MAC address. If
 *            this parameter is set to @p NULL then a system default MAC is
 *            used. The MAC address must be aligned with the most significant
 *            byte first.
 */
void mac_lld_set_address(MACDriver *macp, uint8_t *p) {

  AT91C_BASE_EMAC->EMAC_SA1L = (AT91_REG)((p[2] << 24) | (p[3] << 16) |
                                          (p[4] << 8) | p[5]);
  AT91C_BASE_EMAC->EMAC_SA1H = (AT91_REG)((p[0] << 8) | p[1]);
}

/**
 * @brief Returns a transmission descriptor.
 * @details One of the available transmission descriptors is locked and
 *          returned.
 *
 * @param[in] macp pointer to the @p MACDriver object
 * @param[in] size size of the frame to be transmitted
 * @return A pointer to a @p MACTransmitDescriptor structure or @p NULL if
 *         a descriptor is not available.
 */
MACTransmitDescriptor *max_lld_get_transmit_descriptor(MACDriver *macp,
                                                       size_t size) {
  MACTransmitDescriptor *tdp;

  chDbgAssert(size <= EMAC_TRANSMIT_BUFFERS_SIZE,
              "max_lld_get_transmit_descriptor(), #1",
              "unexpected size");

  if (!link_up)
    return NULL;

  chSysLock();
  tdp = txptr;
  chDbgAssert((tdp->w2 & W2_T_USED) && !(tdp->w2 & W2_T_LOCKED),
              "max_lld_get_transmit_descriptor(), #2", "buffer not available");
  if (!(tdp->w2 & W2_T_USED) || (tdp->w2 & W2_T_LOCKED)) {
    chSysUnlock();
    return NULL;
  }
  /*
   * Set the buffer size and configuration, the buffer is also marked
   * as locked.
   */
  tdp->w2 = size | W2_T_LOCKED | W2_T_USED | W2_T_LAST_BUFFER;
  if (++txptr >= &td[EMAC_TRANSMIT_BUFFERS]) {
    tdp->w2 |= W2_T_WRAP;
    txptr = td;
  }
  chSysUnlock();
  return tdp;
}

/**
 * @brief Releases a transmit descriptor and starts the transmission of the
 *        enqueued data as a single frame.
 *
 * @param[in] macp pointer to the @p MACDriver object
 * @param[in] tdp the pointer to the @p MACTransmitDescriptor structure
 * @param[in]
 */
void mac_lld_release_transmit_descriptor(MACDriver *macp,
                                         MACTransmitDescriptor *tdp) {

  chSysLock();
  tdp->w2 &= ~(W2_T_LOCKED | W2_T_USED);
  AT91C_BASE_EMAC->EMAC_NCR |= AT91C_EMAC_TSTART;
  chSysUnlock();
}

/**
 * @brief Returns the buffer associated to a @p MACTransmitDescriptor.
 *
 * @param[in] tdp the pointer to the @p MACTransmitDescriptor structure
 * @return The pointer to the transmit buffer.
 */
uint8_t *mac_lld_get_transmit_buffer(MACTransmitDescriptor *tdp) {

  return (uint8_t *)(tdp->w1 & W1_T_ADDRESS_MASK);
}

/**
 * @brief Returns a received frame.
 *
 * @param[in] macp pointer to the @p MACDriver object
 * @param[out szp size of the received frame
 * @return A pointer to a @p MACReceiveDescriptor structure or @p NULL if
 *         the operation timed out or some transient error happened.
 */
MACReceiveDescriptor *max_lld_get_receive_descriptor(MACDriver *macp,
                                                     size_t *szp) {
  unsigned n;
  MACReceiveDescriptor *rdp;

  n = EMAC_RECEIVE_BUFFERS;

  /*
   * Skips unused buffers, if any.
   */
skip:
  while ((n > 0) && !(rxptr->w1 & W1_R_OWNERSHIP)) {
    if (++rxptr >= &rd[EMAC_RECEIVE_BUFFERS])
      rxptr = rd;
    n--;
  }

  /*
   * Skips fragments, if any, cleaning them up.
   */
  while ((n > 0) && (rxptr->w1 & W1_R_OWNERSHIP) &&
                    !(rxptr->w2 & W2_R_FRAME_START)) {
    rxptr->w1 &= ~W1_R_OWNERSHIP;
    if (++rxptr >= &rd[EMAC_RECEIVE_BUFFERS])
      rxptr = rd;
    n--;
  }

  /*
   * Now compute the total frame size skipping eventual incomplete frames
   * or holes...
   */
restart:
  rdp = rxptr;
  while (n > 0) {
    if (!(rxptr->w1 & W1_R_OWNERSHIP))
      goto skip;        /* Empty buffer for some reason... */

    /*
     * End Of Frame found.
     */
    if (rxptr->w2 & W2_R_FRAME_END) {
      *szp = rxptr->w2 & W2_T_LENGTH_MASK;
      return rdp;
    }

    if ((rdp != rxptr) && (rxptr->w2 & W2_R_FRAME_START)) {
      /* Found another start... cleaning up the incomplete frame.*/
      do {
        rdp->w1 &= ~W1_R_OWNERSHIP;
        if (++rdp >= &rd[EMAC_RECEIVE_BUFFERS])
          rdp = rd;
      }
      while (rdp != rxptr);
      goto restart;     /* Another start buffer for some reason... */
    }

    if (++rxptr >= &rd[EMAC_RECEIVE_BUFFERS])
      rxptr = rd;
    n--;
  }
  return NULL;
}

/**
 * @brief Releases a receive descriptor.
 * @details The descriptor and its buffer is made available for more incoming
 *          frames.
 *
 * @param[in] macp pointer to the @p MACDriver object
 * @param[in] rdp the pointer to the @p MACReceiveDescriptor structure
 */
void mac_lld_release_receive_descriptor(MACDriver *macp,
                                        MACReceiveDescriptor *rdp) {

  unsigned n = EMAC_RECEIVE_BUFFERS;
  do {
    rdp->w1 &= ~W1_R_OWNERSHIP;
    if (++rdp >= &rd[EMAC_RECEIVE_BUFFERS])
      rdp = rd;
    n--;
  }
  while ((n > 0) || !(rxptr->w2 & W2_R_FRAME_END));
}

/**
 * @brief Returns the buffer associated to a @p MACTransmitDescriptor.
 *
 * @param[in] tdp the pointer to the @p MACTransmitDescriptor structure
 * @return The pointer to the transmit buffer.
 */
uint8_t *mac_lld_get_receive_buffer(MACReceiveDescriptor *rdp) {

  return (uint8_t *)(rdp->w1 & W1_R_ADDRESS_MASK);
}

/** @} */
