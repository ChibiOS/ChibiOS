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

#include "lpc214x.h"
#include "lpc214x_ssp.h"

#ifdef SSP_USE_MUTEX
static Semaphore me;
#endif

void sspAcquireBus(void) {

#ifdef SSP_USE_MUTEX
  chSemWait(&me);
#endif
}

void sspReleaseBus(void) {

#ifdef SSP_USE_MUTEX
  chSemSignal(&me);
#endif
}

/*
 * Synchronous SSP transfer.
 * @param in pointer to the incoming data buffer, if this parameter is set to
 *           \p NULL then the incoming data is discarded.
 * @param out pointer to the outgoing data buffer, if this parameter is set to
 *           \p NULL then 0xFF bytes will be output.
 * @param n the number of bytes to be transferred
 * @note The transfer is performed in a software loop and is not interrupt
 *       driven for performance reasons, this function should be invoked
 *       by a low priority thread in order to "play nice" with the
 *       rest of the system. This kind of peripheral would really need a
 *       dedicated DMA channel.
 */
void sspRW(BYTE8 *in, BYTE8 *out, t_size n) {
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

/*
 * SSP setup.
 */
void SetSSP(int cpsr, int cr0, int cr1) {
  SSP *ssp = SSPBase;

  ssp->SSP_CR1 = 0;
  ssp->SSP_CR0 = cr0;
  ssp->SSP_CPSR = cpsr;
  ssp->SSP_CR1 = cr1 | CR1_SSE;
}

/*
 * SSP subsystem initialization.
 */
void InitSSP(void) {

  /* Enables the SPI1 clock */
  PCONP = (PCONP & PCALL) | PCSPI1;

  /* Clock = PCLK / 2 (fastest). */
  SetSSP(2, CR0_DSS8BIT | CR0_FRFSPI | CR0_CLOCKRATE(0), CR1_LBM);

#ifdef SSP_USE_MUTEX
  chSemInit(&me, 1);
#endif
}
