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
 * @file AT91SAM7X/serial_lld.c
 * @brief AT91SAM7X low level serial driver code
 * @addtogroup AT91SAM7X_SERIAL
 * @{
 */

#include <ch.h>
#include <serial.h>

#include "at91lib/aic.h"

/** @brief Driver default configuration.*/
static const SerialDriverConfig default_config = {
};

/*===========================================================================*/
/* Low Level Driver local functions.                                         */
/*===========================================================================*/

/**
 * @brief USART initialization.
 * @param[in] u pointer to an USART I/O block
 * @param[in] config the architecture-dependent serial driver configuration
 */
void usart_init(AT91PS_USART u, const SerialDriverConfig *config) {

  /* Disables IRQ sources and stop operations.*/
  u->US_IDR = 0xFFFFFFFF;
  u->US_CR = AT91C_US_RSTRX | AT91C_US_RSTTX | AT91C_US_RSTSTA;

  /* New parameters setup.*/
  if (config->mr & AT91C_US_OVER)
    u->US_BRGR = MCK / (config->speed * 8);
  else
    u->US_BRGR = MCK / (config->speed * 16);
  u->US_MR = config->mr;
  u->US_RTOR = 0;
  u->US_TTGR = 0;

  /* Enables operations and IRQ sources.*/
  u->US_CR = AT91C_US_RXEN | AT91C_US_TXEN | AT91C_US_DTREN | AT91C_US_RTSEN;
  u->US_IER = AT91C_US_RXRDY | AT91C_US_OVRE | AT91C_US_FRAME | AT91C_US_PARE |
              AT91C_US_RXBRK;
}

/**
 * @brief USART de-initialization.
 * @param[in] u pointer to an USART I/O block
 */
void usart_deinit(AT91PS_USART u) {

}


/*===========================================================================*/
/* Low Level Driver interrupt handlers.                                      */
/*===========================================================================*/

/*===========================================================================*/
/* Low Level Driver exported functions.                                      */
/*===========================================================================*/

/**
 * Low level serial driver initialization.
 */
void sd_lld_init(void) {

}

/**
 * @brief Low level serial driver configuration and (re)start.
 *
 * @param[in] sdp pointer to a @p SerialDriver object
 * @param[in] config the architecture-dependent serial driver configuration.
 *                   If this parameter is set to @p NULL then a default
 *                   configuration is used.
 */
void sd_lld_start(SerialDriver *sdp, const SerialDriverConfig *config) {

  if (config == NULL)
    config = &default_config;

}

/**
 * @brief Low level serial driver stop.
 * @details De-initializes the USART, stops the associated clock, resets the
 *          interrupt vector.
 *
 * @param[in] sdp pointer to a @p SerialDriver object
 */
void sd_lld_stop(SerialDriver *sdp) {

}

/** @} */
