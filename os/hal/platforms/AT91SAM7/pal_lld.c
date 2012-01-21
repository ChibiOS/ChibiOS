/*
    ChibiOS/RT - Copyright (C) 2006,2007,2008,2009,2010,
                 2011,2012 Giovanni Di Sirio.

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
 * @file    AT91SAM7/pal_lld.c
 * @brief   AT91SAM7 PIO low level driver code.
 *
 * @addtogroup PAL
 * @{
 */

#include "ch.h"
#include "hal.h"

#if HAL_USE_PAL || defined(__DOXYGEN__)

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
 * @brief   AT91SAM7 I/O ports configuration.
 * @details PIO registers initialization.
 *
 * @param[in] config    the AT91SAM7 ports configuration
 *
 * @notapi
 */
void _pal_lld_init(const PALConfig *config) {

  uint32_t ports = (1 << AT91C_ID_PIOA);
#if (SAM7_PLATFORM == SAM7X128) || (SAM7_PLATFORM == SAM7X256) || \
    (SAM7_PLATFORM == SAM7X512)
  ports |= (1 << AT91C_ID_PIOB);
#endif
  AT91C_BASE_PMC->PMC_PCER = ports;

  /*
   * PIOA setup.
   */
  AT91C_BASE_PIOA->PIO_PPUER  = config->P0Data.pusr;    /* Pull-up as spec.*/
  AT91C_BASE_PIOA->PIO_PPUDR  = ~config->P0Data.pusr;
  AT91C_BASE_PIOA->PIO_PER  = 0xFFFFFFFF;               /* PIO enabled.*/
  AT91C_BASE_PIOA->PIO_ODSR = config->P0Data.odsr;      /* Data as specified.*/
  AT91C_BASE_PIOA->PIO_OER  = config->P0Data.osr;       /* Dir. as specified.*/
  AT91C_BASE_PIOA->PIO_ODR  = ~config->P0Data.osr;
  AT91C_BASE_PIOA->PIO_IFDR = 0xFFFFFFFF;               /* Filter disabled.*/
  AT91C_BASE_PIOA->PIO_IDR  = 0xFFFFFFFF;               /* Int. disabled.*/
  AT91C_BASE_PIOA->PIO_MDDR = 0xFFFFFFFF;               /* Push Pull drive.*/
  AT91C_BASE_PIOA->PIO_ASR  = 0xFFFFFFFF;               /* Peripheral A.*/
  AT91C_BASE_PIOA->PIO_OWER = 0xFFFFFFFF;               /* Write enabled.*/

  /*
   * PIOB setup.
   */
#if (SAM7_PLATFORM == SAM7X128) || (SAM7_PLATFORM == SAM7X256) || \
    (SAM7_PLATFORM == SAM7X512)
  AT91C_BASE_PIOB->PIO_PPUER  = config->P1Data.pusr;    /* Pull-up as spec.*/
  AT91C_BASE_PIOB->PIO_PPUDR  = ~config->P1Data.pusr;
  AT91C_BASE_PIOB->PIO_PER  = 0xFFFFFFFF;               /* PIO enabled.*/
  AT91C_BASE_PIOB->PIO_ODSR = config->P1Data.odsr;      /* Data as specified.*/
  AT91C_BASE_PIOB->PIO_OER  = config->P1Data.osr;       /* Dir. as specified.*/
  AT91C_BASE_PIOB->PIO_ODR  = ~config->P1Data.osr;
  AT91C_BASE_PIOB->PIO_IFDR = 0xFFFFFFFF;               /* Filter disabled.*/
  AT91C_BASE_PIOB->PIO_IDR  = 0xFFFFFFFF;               /* Int. disabled.*/
  AT91C_BASE_PIOB->PIO_MDDR = 0xFFFFFFFF;               /* Push Pull drive.*/
  AT91C_BASE_PIOB->PIO_ASR  = 0xFFFFFFFF;               /* Peripheral A.*/
  AT91C_BASE_PIOB->PIO_OWER = 0xFFFFFFFF;               /* Write enabled.*/
#endif
}

/**
 * @brief   Pads mode setup.
 * @details This function programs a pads group belonging to the same port
 *          with the specified mode.
 * @note    This function is not meant to be invoked directly from the
 *          application code.
 * @note    @p PAL_MODE_RESET is implemented as input with pull-up.
 * @note    @p PAL_MODE_UNCONNECTED is implemented as push pull output with
 *          high state.
 * @note    @p PAL_MODE_OUTPUT_OPENDRAIN also enables the pull-up resistor.
 *
 * @param[in] port      the port identifier
 * @param[in] mask      the group mask
 * @param[in] mode      the mode
 *
 * @notapi
 */
void _pal_lld_setgroupmode(ioportid_t port,
                           ioportmask_t mask,
                           iomode_t mode) {

  switch (mode) {
  case PAL_MODE_RESET:
  case PAL_MODE_INPUT_PULLUP:
    port->PIO_PPUER = mask;
    port->PIO_ODR = mask;
    break;
  case PAL_MODE_INPUT:
  case PAL_MODE_INPUT_ANALOG:
    port->PIO_PPUDR = mask;
    port->PIO_ODR = mask;
    break;
  case PAL_MODE_UNCONNECTED:
    port->PIO_SODR = mask;
    /* Falls in */
  case PAL_MODE_OUTPUT_PUSHPULL:
    port->PIO_PPUDR = mask;
    port->PIO_OER = mask;
    port->PIO_MDDR = mask;
    break;
  case PAL_MODE_OUTPUT_OPENDRAIN:
    port->PIO_PPUER = mask;
    port->PIO_OER = mask;
    port->PIO_MDER = mask;
  }
}

#endif /* HAL_USE_PAL */

/** @} */
