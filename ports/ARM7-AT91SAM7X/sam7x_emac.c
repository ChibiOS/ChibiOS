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

#include <ch.h>

#include "board.h"
#include "sam7x_emac.h"
#include "mii.h"
#include "at91lib/aic.h"

#define EMAC_RECEIVE_BUFFERS            25
#define EMAC_RECEIVE_BUFFERS_SIZE       128
#define EMAC_TRANSMIT_BUFFERS           2
#define EMAC_TRANSMIT_BUFFERS_SIZE      1518

EventSource EMACFrameTransmitted, EMACFrameReceived;

static uint8_t default_mac[] = {0xAA, 0x55, 0x13, 0x37, 0x01, 0x10};

static BufDescriptorEntry rent[EMAC_RECEIVE_BUFFERS] __attribute__((aligned(4)));
static uint8_t rbuffers[EMAC_RECEIVE_BUFFERS * EMAC_RECEIVE_BUFFERS_SIZE] __attribute__((aligned(4)));
static BufDescriptorEntry tent[EMAC_TRANSMIT_BUFFERS] __attribute__((aligned(4)));
static uint8_t tbuffers[EMAC_TRANSMIT_BUFFERS * EMAC_TRANSMIT_BUFFERS_SIZE] __attribute__((aligned(4)));

#define PHY_ADDRESS 1
#define AT91C_PB15_ERXDV AT91C_PB15_ERXDV_ECRSDV
#define EMAC_PIN_MASK (AT91C_PB1_ETXEN  | AT91C_PB2_ETX0   | \
                       AT91C_PB3_ETX1   | AT91C_PB4_ECRS   | \
                       AT91C_PB5_ERX0   | AT91C_PB6_ERX1   | \
                       AT91C_PB7_ERXER  | AT91C_PB8_EMDC   | \
                       AT91C_PB9_EMDIO  | AT91C_PB10_ETX2  | \
                       AT91C_PB11_ETX3  | AT91C_PB12_ETXER | \
                       AT91C_PB13_ERX2  | AT91C_PB14_ERX3  | \
                       AT91C_PB15_ERXDV | AT91C_PB16_ECOL  | \
                       AT91C_PB17_ERXCK)

#define PHY_LATCHED_PINS (AT91C_PB4_ECRS   | AT91C_PB5_ERX0  | AT91C_PB6_ERX1  | \
                          AT91C_PB7_ERXER  | AT91C_PB13_ERX2 | AT91C_PB14_ERX3 | \
                          AT91C_PB15_ERXDV | AT91C_PB16_ECOL | PIOB_PHY_IRQ)

/*
 * PHY utilities.
 */
static uint32_t phy_get(uint8_t regno) {

  AT91C_BASE_EMAC->EMAC_MAN = (1 << 30) |               // SOF = 01
                              (2 << 28) |               // RW = 10
                              (PHY_ADDRESS << 23) |
                              (regno << 18) |
                              (2 << 16);                // CODE = 10
  while (!( AT91C_BASE_EMAC->EMAC_NSR & AT91C_EMAC_IDLE))
    ;
  return AT91C_BASE_EMAC->EMAC_MAN & 0xFFFF;
}

/*static void phy_put(uint8_t regno, uint32_t val) {

  AT91C_BASE_EMAC->EMAC_MAN = (1 << 30) |               // SOF = 01
                              (1 << 28) |               // RW = 01
                              (PHY_ADDRESS << 23) |
                              (regno << 18) |
                              (2 << 16) |               // CODE = 10
                              val;
  while (!( AT91C_BASE_EMAC->EMAC_NSR & AT91C_EMAC_IDLE))
    ;
}*/

/*
 * Returns TRUE if the link is active else FALSE.
 * It also setup the link-related EMAC registers.
 */
static bool_t check_link_status(void) {
  uint32_t ncfgr, bmsr, bmcr, lpa;

  (void)phy_get(MII_BMSR);
  bmsr = phy_get(MII_BMSR);
  if (!(bmsr & BMSR_LSTATUS))
    return TRUE;

  ncfgr = AT91C_BASE_EMAC->EMAC_NCFGR & ~(AT91C_EMAC_SPD | AT91C_EMAC_FD);
  bmcr = phy_get(MII_BMCR);
  if (bmcr & BMCR_ANENABLE) {
    lpa = phy_get(MII_LPA);
    if (lpa & (LPA_100HALF | LPA_100FULL | LPA_100BASE4))
      ncfgr |= AT91C_EMAC_SPD;
    if (lpa & (LPA_10FULL | LPA_100FULL))
      ncfgr |= AT91C_EMAC_FD;
  }
  else {
    if (bmcr & BMCR_SPEED100)
      ncfgr |= AT91C_EMAC_SPD;
    if (bmcr & BMCR_FULLDPLX)
      ncfgr |= AT91C_EMAC_FD;
  }
  AT91C_BASE_EMAC->EMAC_NCFGR = ncfgr;
  return FALSE;
}

__attribute__((noinline))
static void ServeInterrupt(void) {
}

__attribute__((naked, weak))
void EMACIrqHandler(void) {

  chSysIRQEnterI();

  ServeInterrupt();

  chSysIRQExitI();
}

/*
 * EMAC subsystem initialization.
 */
void InitEMAC(int prio) {
  int i;

  /*
   * Buffers initialization.
   */
  for (i = 0; i < EMAC_RECEIVE_BUFFERS; i++) {
    rent[i].w1 = (uint32_t)&rbuffers[i * EMAC_RECEIVE_BUFFERS_SIZE];
    rent[i].w2 = 0;
  }
  rent[EMAC_RECEIVE_BUFFERS - 1].w1 |= W1_R_WRAP;

  for (i = 0; i < EMAC_TRANSMIT_BUFFERS; i++) {
    rent[i].w1 = ((uint32_t)&tbuffers[i * EMAC_TRANSMIT_BUFFERS_SIZE]) | W2_T_USED;
    tent[i].w2 = 0;
  }
  tent[EMAC_TRANSMIT_BUFFERS - 1].w1 |= W2_T_WRAP;

  /*
   * Disables the pullups on all the pins that are latched on reset by the PHY.
   * The status latched into the PHY is:
   *   PHYADDR  = 00001
   *   PCS_LPBK = 0 (disabled)
   *   ISOLATE  = 0 (disabled)
   *   RMIISEL  = 0 (MII mode)
   *   RMIIBTB  = 0 (BTB mode disabled)
   *   SPEED    = 1 (100mbps)
   *   DUPLEX   = 1 (full duplex)
   *   ANEG_EN  = 1 (auto negotiation enabled)
   */
  AT91C_BASE_PIOB->PIO_PPUDR = PHY_LATCHED_PINS;

  /*
   * PHY power control.
   */
  AT91C_BASE_PIOB->PIO_OER = PIOB_PHY_PD;       // Becomes an output.
  AT91C_BASE_PIOB->PIO_PPUDR = PIOB_PHY_PD;     // Default pullup disabled.
//  AT91C_BASE_PIOB->PIO_CODR = PIOB_PHY_PD;      // Output to low level.
  AT91C_BASE_PIOB->PIO_SODR = PIOB_PHY_PD;      // Output to high level.

  /*
   * PHY reset by pulsing the NRST pin.
   */
  AT91C_BASE_RSTC->RSTC_RMR = 0xA5000100;
  AT91C_BASE_RSTC->RSTC_RCR = 0xA5000000 | AT91C_RSTC_EXTRST;
  while (!(AT91C_BASE_RSTC->RSTC_RSR & AT91C_RSTC_NRSTL))
    ;

  /*
   * EMAC pins setup. Note, PB18 is not included because it is used as #PD
   * control and not as EF100.
   */
  AT91C_BASE_PIOB->PIO_ASR = EMAC_PIN_MASK;
  AT91C_BASE_PIOB->PIO_PDR = EMAC_PIN_MASK;
  AT91C_BASE_PIOB->PIO_PPUDR = EMAC_PIN_MASK; // Really needed ?????

  /*
   * EMAC setup.
   */
//  AT91C_BASE_EMAC->EMAC_NCR = AT91C_EMAC_MPE;           // Enable Management Port
  AT91C_BASE_EMAC->EMAC_NCFGR = 2 << 10;                // MDC-CLK = MCK / 32
//  chThdSleep(5); // It could perform one or more dummy phy_get() instead.
//  (void)phy_get(MII_BMCR);
//  phy_put(MII_BMCR, phy_get(MII_BMCR) & ~BMCR_ISOLATE); // Disable ISOLATE
  AT91C_BASE_EMAC->EMAC_NCR = 0;                        // Disable Management Port

  AT91C_BASE_EMAC->EMAC_USRIO = AT91C_EMAC_CLKEN;       // Enable EMAC in MII mode
  AT91C_BASE_EMAC->EMAC_RBQP = (AT91_REG)rent;          // RX buffers list
  AT91C_BASE_EMAC->EMAC_TBQP = (AT91_REG)tent;          // TX buffers list
  AT91C_BASE_EMAC->EMAC_RSR = AT91C_EMAC_OVR |
                              AT91C_EMAC_REC |
                              AT91C_EMAC_BNA;           // Clears RSR
  AT91C_BASE_EMAC->EMAC_NCFGR |= AT91C_EMAC_CAF |
                                 AT91C_EMAC_NBC |
                                 AT91C_EMAC_DRFCS;      // Initial NCFGR settings
  AT91C_BASE_EMAC->EMAC_NCR |= AT91C_EMAC_TE |
                               AT91C_EMAC_RE |
                               AT91C_EMAC_CLRSTAT;      // Initial NCR settings
  EMACSetAddress(default_mac);

  /*
   * PHY checks and settings.
   */
  AT91C_BASE_EMAC->EMAC_NCR |= AT91C_EMAC_MPE;
  if ((phy_get(MII_PHYSID1) != (MII_MICREL_ID >> 16)) ||
      (phy_get(MII_PHYSID2 & 0xFFF0) != (MII_MICREL_ID & 0xFFF0)))
    chDbgPanic("Wrong PHY identifier");
  if (check_link_status())
    chDbgPanic("No link");
  AT91C_BASE_EMAC->EMAC_NCR &= ~AT91C_EMAC_MPE;

  /*
   * Interrupt setup.
   */
  AIC_ConfigureIT(AT91C_ID_EMAC,
                  AT91C_AIC_SRCTYPE_INT_HIGH_LEVEL | prio,
                  EMACIrqHandler);
  AIC_EnableIT(AT91C_ID_EMAC);

  /*
   * Event sources setup.
   */
  chEvtInit(&EMACFrameTransmitted);
  chEvtInit(&EMACFrameReceived);
}

/*
 * Set the MAC address.
 */
void EMACSetAddress(uint8_t *eaddr) {

  AT91C_BASE_EMAC->EMAC_SA1L = (AT91_REG)((eaddr[3] << 24) | (eaddr[2] << 16) |
                                          (eaddr[1] << 8) | eaddr[0]);
  AT91C_BASE_EMAC->EMAC_SA1H = (AT91_REG)((eaddr[5] << 8) | eaddr[4]);
}

/*
 * Transmits an ethernet frame.
 */
bool_t EMACTransmit(uint8_t *hdr, uint8_t *data, size_t size) {

  return FALSE;
}
