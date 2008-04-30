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

static BufDescriptorEntry rent[EMAC_RECEIVE_BUFFERS] __attribute__((aligned(4)));
//static BufDescriptorEntry tent[EMAC_TRANSMIT_BUFFERS] __attribute__((aligned(4)));
static uint8_t rbuffers[128 * EMAC_RECEIVE_BUFFERS] __attribute__((aligned(4)));

EventSource TransmitDone, ReceiveNotEmpty;

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

static void phy_put(uint8_t regno, uint32_t val) {

  AT91C_BASE_EMAC->EMAC_MAN = (1 << 30) |               // SOF = 01
                              (1 << 28) |               // RW = 01
                              (PHY_ADDRESS << 23) |
                              (regno << 18) |
                              (2 << 16) |               // CODE = 10
                              val;
  while (!( AT91C_BASE_EMAC->EMAC_NSR & AT91C_EMAC_IDLE))
    ;
}

/*
 * EMAC subsystem initialization.
 */
void InitEMAC(int prio) {
  int i;

  for (i = 0; i < EMAC_RECEIVE_BUFFERS; i++) {
    rent[i].w1 = (uint32_t)&rbuffers[i * 128];
    rent[i].w2 = 0;
  }
  rent[EMAC_RECEIVE_BUFFERS - 1].w1 |= W1_R_WRAP;

  /*
   * Disables default pullups, the PHY has an internal pulldowns.
   * Selects MII mode.
   */
  AT91C_BASE_PIOB->PIO_PPUDR = AT91C_PB15_ERXDV | AT91C_PB16_ECOL;

  /*
   * PHY powerdown.
   */
  AT91C_BASE_PIOB->PIO_OER = PIOB_PHY_PD;       // Becomes an output.
  AT91C_BASE_PIOB->PIO_PPUDR = PIOB_PHY_PD;     // Default pullup disabled.
  AT91C_BASE_PIOB->PIO_CODR = PIOB_PHY_PD;      // Output to low level.

  /*
   * PHY reset by pulsing the NRST pin.
   */
  AT91C_BASE_RSTC->RSTC_RMR = 0xA5000100;
  AT91C_BASE_RSTC->RSTC_RCR = 0xA5000000 | AT91C_RSTC_EXTRST;
  while (!(AT91C_BASE_RSTC->RSTC_RSR & AT91C_RSTC_NRSTL))
    ;

  /*
   * EMAC pins setup.
   */
  AT91C_BASE_PIOB->PIO_ASR = EMAC_PIN_MASK;
  AT91C_BASE_PIOB->PIO_PDR = EMAC_PIN_MASK;
  AT91C_BASE_PIOB->PIO_PPUDR = EMAC_PIN_MASK; // ?????

  /*
   * EMAC setup.
   */
  AT91C_BASE_EMAC->EMAC_NCR |= AT91C_EMAC_MPE;          // Enable Management Port
  AT91C_BASE_EMAC->EMAC_NCFGR |= 2 << 10;               // CLK = MCK / 32

  chThdSleep(5);

  (void)phy_get(MII_BMCR);
  phy_put(MII_BMCR, phy_get(MII_BMCR) & ~BMCR_ISOLATE); // Disable ISOLATE

  AT91C_BASE_EMAC->EMAC_NCR &= ~AT91C_EMAC_MPE;         // Disable Management Port

  AT91C_BASE_EMAC->EMAC_USRIO = AT91C_EMAC_CLKEN;       // Enable EMAC in MII mode


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
 * Transmits a data buffer (whole ethernet frame).
 */
//bool_t EMACTransmit(uint8_t *buf, size_t size) {
//}
