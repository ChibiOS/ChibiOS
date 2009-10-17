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
 * @file Win32/serial_lld.c
 * @brief Win32 low level simulated serial driver code
 * @addtogroup WIN32_SERIAL
 * @{
 */

#include <windows.h>
#include <stdio.h>

#include <ch.h>
#include <serial.h>

/** @brief Test serial driver identifier.*/
SerialDriver SD1;

/** @brief Driver default configuration.*/
static const SerialDriverConfig default_config = {
};

/*===========================================================================*/
/* Low Level Driver local functions.                                         */
/*===========================================================================*/

static bool_t sd1_conn_chkint(void) {

  return FALSE;
}

static bool_t sd1_in_chkint(void) {

  return FALSE;
}

static bool_t sd1_out_chkint(void) {
  msg_t n;
  bool_t rflag = FALSE;

  while (TRUE) {
    n = sdRequestDataI(&SD1);
    if (n < 0) {
      fflush(stdout);
      return rflag;
    }
    fputc(n, stdout);
    rflag = TRUE;
  }
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

  sdObjectInit(&SD1, NULL, NULL);
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

  (void)sdp;
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

  (void)sdp;
}

bool_t sd_lld_interrupt_pending(void) {

  return sd1_conn_chkint() || sd1_in_chkint() || sd1_out_chkint();
}

/** @} */
