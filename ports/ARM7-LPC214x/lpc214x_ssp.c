/*
    ChibiOS/RT - Copyright (C) 2009 Giovanni Di Sirio.

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

                                      ---

    A special exception to the GPL can be applied should you wish to distribute
    a combined work that includes ChibiOS/RT, without being obliged to provide
    the source code for any proprietary components. See the file exception.txt
    for full details of how and when the exception can be applied.
*/

/**
 * @file ports/ARM7-LPC214x/lpc214x_ssp.c
 * @brief LPC214x SSP driver code.
 * @addtogroup LPC214x_SSP
 * @{
 */

#include <ch.h>

#include "lpc214x.h"
#include "lpc214x_ssp.h"

#if LPC214x_SSP_USE_MUTEX
static Semaphore me;
#endif

/**
 * @brief Aquires access to the SSP bus.
 * @note This function also handles the mutual exclusion on the SSP bus if
 *       the @p LPC214x_SSP_USE_MUTEX option is enabled.
 */
void sspAcquireBus(void) {

#if LPC214x_SSP_USE_MUTEX
  chSemWait(&me);
#endif
  IO0CLR = 1 << 20;
}

/**
 * @brief Releases the SSP bus.
 * @note This function also handles the mutual exclusion on the SSP bus if
 *       the @p LPC214x_SSP_USE_MUTEX option is enabled.
 */
void sspReleaseBus(void) {

  IO0SET = 1 << 20;
#if LPC214x_SSP_USE_MUTEX
  chSemSignal(&me);
#endif
}

/**
 * @brief Synchronous SSP transfer.
 * @param[in] in pointer to the incoming data buffer, if this parameter is set
 *               to @p NULL then the incoming data is discarded
 * @param[out] out pointer to the outgoing data buffer, if this parameter is
 *                 set to @p NULL then 0xFF bytes will be output
 * @param[in] n the number of bytes to be transferred
 * @note The transfer is performed in a software loop and is not interrupt
 *       driven for performance reasons, this function should be invoked
 *       by a low priority thread in order to "play nice" with the
 *       rest of the system. This kind of peripheral would really need a
 *       dedicated DMA channel.
 */
void sspRW(uint8_t *in, uint8_t *out, size_t n) {
  int icnt, ocnt;

  SSP *ssp = SSPBase;
  icnt = ocnt = n;
  while (icnt) {

    if (ssp->SSP_SR & SR_RNE) {
      if (in)
        *in++ = ssp->SSP_DR;
      else
        ssp->SSP_DR;
      icnt--;
      continue; /* Priority over transmission. */
    }

    if (ocnt && (ssp->SSP_SR & SR_TNF)) {
      if (out)
        ssp->SSP_DR = *out++;
      else
        ssp->SSP_DR = 0xFF;
      ocnt--;
    }
  }
}

/**
 * @brief SSP setup.
 * @param[in] cpsr the value for the @p CPSR register
 * @param[in] cr0 the value for the @p CR0 register
 * @param[in] cr1 the value for the @p CR1 register
 */
void ssp_setup(int cpsr, int cr0, int cr1) {

  SSP *ssp = SSPBase;
  ssp->SSP_CR1 = 0;
  ssp->SSP_CR0 = cr0;
  ssp->SSP_CPSR = cpsr;
  ssp->SSP_CR1 = cr1 | CR1_SSE;
}

/**
 * @brief SSP subsystem initialization.
 */
void ssp_init(void) {

  /* Enables the SPI1 clock */
  PCONP = (PCONP & PCALL) | PCSPI1;

  /* Clock = PCLK / 2 (fastest). */
  ssp_setup(2, CR0_DSS8BIT | CR0_FRFSPI | CR0_CLOCKRATE(0), 0);

#if LPC214x_SSP_USE_MUTEX
  chSemInit(&me, 1);
#endif
}

/** @} */
