/*
    ChibiOS/RT - Copyright (C) 2010 Giovanni Di Sirio.

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
 * @file LPC214x/spi_lld.c
 * @brief LPC214x low level SPI driver code.
 * @addtogroup LPC214x_SPI
 * @{
 */

#include "ch.h"
#include "hal.h"

#if CH_HAL_USE_SPI || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

#if USE_LPC214x_SPI1 || defined(__DOXYGEN__)
/** @brief SPI1 driver identifier.*/
SPIDriver SPID1;
#endif

/*===========================================================================*/
/* Driver local variables.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

/**
 * @brief Synchronous SSP transfer.
 *
 * @param[in] n         number of bytes to be exchanged
 *
 * @param[in] txbuf     the pointer to the transmit buffer or @p NULL
 * @param[out] rxbuf    the pointer to the receive buffer or @p NULL
 */
void rw8(size_t n, const uint8_t *txbuf, uint8_t *rxbuf) {
  size_t ntx = n;

  while (n > 0) {
    uint32_t sr = SSPBase->SSP_SR;
    if (sr & SR_RNE) {
      uint8_t w = SSPBase->SSP_DR;
      if (rxbuf != NULL)
        *rxbuf++ = w;
      n--;
      continue; /* Priority over transmission. */
    }
    if ((ntx > 0) && (sr & SR_TNF)) {
      if (txbuf != NULL)
        SSPBase->SSP_DR = *txbuf++;
      else
        SSPBase->SSP_DR = 0xFFFFFFFF;
      ntx--;
    }
  }
}

/*===========================================================================*/
/* Driver interrupt handlers.                                                */
/*===========================================================================*/

/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

/**
 * @brief Low level SPI driver initialization.
 */
void spi_lld_init(void) {

#if USE_LPC214x_SPI1
  spiObjectInit(&SPID1);
#endif
}

/**
 * @brief Configures and activates the SPI peripheral.
 *
 * @param[in] spip      pointer to the @p SPIDriver object
 */
void spi_lld_start(SPIDriver *spip) {

  if (spip->spd_state == SPI_STOP) {
    /* Clock activation.*/
    PCONP = (PCONP & PCALL) | PCSPI1;
  }
  /* Configuration.*/
  SSPBase->SSP_CR1  = 0;
  /* Emptying the receive FIFO, it happens to not be empty while debugging.*/
  while (SSPBase->SSP_SR & SR_RNE)
    (void) SSPBase->SSP_DR;
  SSPBase->SSP_CR0  = spip->spd_config->spc_cr0;
  SSPBase->SSP_CPSR = spip->spd_config->spc_cpsr;
  SSPBase->SSP_CR1  = spip->spd_config->spc_cr1 | CR1_SSE;
}

/**
 * @brief Deactivates the SPI peripheral.
 *
 * @param[in] spip      pointer to the @p SPIDriver object
 */
void spi_lld_stop(SPIDriver *spip) {

  if (spip->spd_state != SPI_STOP) {
    SSPBase->SSP_CR1  = 0;
    SSPBase->SSP_CR0  = 0;
    SSPBase->SSP_CPSR = 0;
    PCONP &= ~PCSPI1;
  }
}

/**
 * @brief Asserts the slave select signal and prepares for transfers.
 *
 * @param[in] spip      pointer to the @p SPIDriver object
 */
void spi_lld_select(SPIDriver *spip) {

  palClearPad(spip->spd_config->spc_ssport, spip->spd_config->spc_sspad);
}

/**
 * @brief Deasserts the slave select signal.
 * @details The previously selected peripheral is unselected.
 *
 * @param[in] spip      pointer to the @p SPIDriver object
 */
void spi_lld_unselect(SPIDriver *spip) {

  palSetPad(spip->spd_config->spc_ssport, spip->spd_config->spc_sspad);
}

/**
 * @brief Ignores data on the SPI bus.
 * @details This function transmits a series of idle words on the SPI bus and
 *          ignores the received data. This function can be invoked even
 *          when a slave select signal has not been yet asserted.
 *
 * @param[in] spip      pointer to the @p SPIDriver object
 * @param[in] n         number of words to be ignored
 */
void spi_lld_ignore(SPIDriver *spip, size_t n) {

  (void)spip;
  rw8(n, NULL, NULL);
}

/**
 * @brief Exchanges data on the SPI bus.
 * @details This function performs a simultaneous transmit/receive operation.
 *
 * @param[in] spip      pointer to the @p SPIDriver object
 * @param[in] n         number of words to be exchanged
 * @param[in] txbuf     the pointer to the transmit buffer
 * @param[out] rxbuf    the pointer to the receive buffer
 *
 * @note The buffers are organized as uint8_t arrays for data sizes below or
 *       equal to 8 bits else it is organized as uint16_t arrays.
 */
void spi_lld_exchange(SPIDriver *spip, size_t n,
                      const void *txbuf, void *rxbuf) {

  (void)spip;
  rw8(n, txbuf, rxbuf);
}

/**
 * @brief Sends data ever the SPI bus.
 *
 * @param[in] spip      pointer to the @p SPIDriver object
 * @param[in] n         number of words to send
 * @param[in] txbuf     the pointer to the transmit buffer
 *
 * @note The buffers are organized as uint8_t arrays for data sizes below or
 *       equal to 8 bits else it is organized as uint16_t arrays.
 */
void spi_lld_send(SPIDriver *spip, size_t n, const void *txbuf) {

  (void)spip;
  rw8(n, txbuf, NULL);
}

/**
 * @brief Receives data from the SPI bus.
 *
 * @param[in] spip      pointer to the @p SPIDriver object
 * @param[in] n         number of words to receive
 * @param[out] rxbuf    the pointer to the receive buffer
 *
 * @note The buffers are organized as uint8_t arrays for data sizes below or
 *       equal to 8 bits else it is organized as uint16_t arrays.
 */
void spi_lld_receive(SPIDriver *spip, size_t n, void *rxbuf) {

  (void)spip;
  rw8(n, NULL, rxbuf);
}

#endif /* CH_HAL_USE_SPI */

/** @} */
