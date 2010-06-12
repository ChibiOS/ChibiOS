/*
    ChibiOS/RT - Copyright (C) 2006,2007,2008,2009,2010 Giovanni Di Sirio.

    This file is part of ChibiOS/RT.

    ChibiOS/RT is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 3 of the License, or
    (at your option) any later version.

    ChibiOS/RT is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program. If not, see <http://www.gnu.org/licenses/>.

                                      ---

    A special exception to the GPL can be applied should you wish to distribute
    a combined work that includes ChibiOS/RT, without being obliged to provide
    the source code for any proprietary components. See the file exception.txt
    for full details of how and when the exception can be applied.
*/

/**
 * @file AT91SAM7/at91sam7_mii.c
 * @brief AT91SAM7 low level MII driver code.
 * @addtogroup AT91SAM7_MII
 * @{
 */

#include "ch.h"
#include "hal.h"
#include "at91sam7_mii.h"

#if CH_HAL_USE_MAC || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local variables.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Driver interrupt handlers.                                                */
/*===========================================================================*/

/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

/**
 * @brief Low level MII driver initialization.
 */
void miiInit(void) {

}

/**
 * @brief Resets a PHY device.
 *
 * @param[in] macp pointer to the @p MACDriver object
 */
void miiReset(MACDriver *macp) {

  (void)macp;

  /*
   * Disables the pullups on all the pins that are latched on reset by the PHY.
   */
  AT91C_BASE_PIOB->PIO_PPUDR = PHY_LATCHED_PINS;

#ifdef PIOB_PHY_PD_MASK
  /*
   * PHY power control.
   */
  AT91C_BASE_PIOB->PIO_OER = PIOB_PHY_PD_MASK;       // Becomes an output.
  AT91C_BASE_PIOB->PIO_PPUDR = PIOB_PHY_PD_MASK;     // Default pullup disabled.
#if (PHY_HARDWARE == PHY_DAVICOM_9161)
  AT91C_BASE_PIOB->PIO_CODR = PIOB_PHY_PD_MASK;      // Output to low level.
#else
  AT91C_BASE_PIOB->PIO_SODR = PIOB_PHY_PD_MASK;      // Output to high level.
#endif
#endif // PIOB_PHY_PD_MASK

  /*
   * PHY reset by pulsing the NRST pin.
   */
  AT91C_BASE_RSTC->RSTC_RMR = 0xA5000100;
  AT91C_BASE_RSTC->RSTC_RCR = 0xA5000000 | AT91C_RSTC_EXTRST;
  while (!(AT91C_BASE_RSTC->RSTC_RSR & AT91C_RSTC_NRSTL))
    ;
}

/**
 * @brief Reads a PHY register through the MII interface.
 *
 * @param[in] macp pointer to the @p MACDriver object
 * @param addr the register address
 * @return The register value.
 */
phyreg_t miiGet(MACDriver *macp, phyaddr_t addr) {

  (void)macp;
  AT91C_BASE_EMAC->EMAC_MAN = (0b01 << 30) |            /* SOF */
                              (0b10 << 28) |            /* RW */
                              (PHY_ADDRESS << 23) |     /* PHYA */
                              (addr << 18) |            /* REGA */
                              (0b10 << 16);             /* CODE */
  while (!( AT91C_BASE_EMAC->EMAC_NSR & AT91C_EMAC_IDLE))
    ;
  return (phyreg_t)(AT91C_BASE_EMAC->EMAC_MAN & 0xFFFF);
}

/**
 * @brief Writes a PHY register through the MII interface.
 *
 * @param[in] macp pointer to the @p MACDriver object
 * @param addr the register address
 * @param value the new register value
 */
void miiPut(MACDriver *macp, phyaddr_t addr, phyreg_t value) {

  (void)macp;
  AT91C_BASE_EMAC->EMAC_MAN = (0b01 << 30) |            /* SOF */
                              (0b01 << 28) |            /* RW */
                              (PHY_ADDRESS << 23) |     /* PHYA */
                              (addr << 18) |            /* REGA */
                              (0b10 << 16) |            /* CODE */
                              value;
  while (!( AT91C_BASE_EMAC->EMAC_NSR & AT91C_EMAC_IDLE))
    ;
}

#endif /* CH_HAL_USE_MAC */

/** @} */
