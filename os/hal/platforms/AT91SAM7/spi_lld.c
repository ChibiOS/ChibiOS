/*
    ChibiOS/RT - Copyright (C) 2006,2007,2008,2009,2010 Giovanni Di Sirio.

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
 * @file AT91SAM7/spi_lld.c
 * @brief AT91SAM7 low level SPI driver code.
 * @addtogroup AT91SAM7_SPI
 * @{
 */

#include "ch.h"
#include "hal.h"

#if CH_HAL_USE_SPI || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

#if USE_AT91SAM7_SPI || defined(__DOXYGEN__)
/** @brief SPI driver identifier.*/
SPIDriver SPID;
#endif

/*===========================================================================*/
/* Driver local variables.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

/**
 * @brief Synchronous SPI transfer.
 *
 * @param[in] n         number of bytes to be exchanged
 *
 * @param[in] txbuf     the pointer to the transmit buffer or @p NULL
 * @param[out] rxbuf    the pointer to the receive buffer or @p NULL
 */
void rw8(size_t n, const uint8_t *txbuf, uint8_t *rxbuf) {
  size_t ntx = n;

  while (n > 0) {
    uint32_t sr = AT91C_BASE_SPI->SPI_SR;
    if (sr & AT91C_SPI_RDRF) {
      uint8_t w = AT91C_BASE_SPI->SPI_RDR;
      if (rxbuf != NULL)
        *rxbuf++ = w;
      n--;
      continue; /* Priority over transmission. */
    }
    if ((ntx > 0) && (sr & AT91C_SPI_TDRE)) {
      if (txbuf != NULL)
        AT91C_BASE_SPI->SPI_TDR = *txbuf++;
      else
        AT91C_BASE_SPI->SPI_TDR = 0xFF;
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

#if USE_AT91SAM7_SPI
  spiObjectInit(&SPID);
#endif
}

/**
 * @brief Configures and activates the SPI peripheral.
 *
 * @param[in] spip      pointer to the @p SPIDriver object
 */
void spi_lld_start(SPIDriver *spip) {

  if (spip->spd_state == SPI_STOP) {
    /* disable general-purpose I/O for SPI pins */
    AT91C_BASE_PIOA->PIO_PDR = SPI_MISO | SPI_MOSI | SPI_SCK;
    /* select perepheral A for SPI pins*/
    AT91C_BASE_PIOA->PIO_ASR = SPI_MISO | SPI_MOSI | SPI_SCK;
    /* Clock activation.*/
    AT91C_BASE_PMC->PMC_PCER = (1 << AT91C_ID_SPI);
  }
  /* software reset must be written twice (errata for revision B parts) */
  AT91C_BASE_SPI->SPI_CR = AT91C_SPI_SWRST;
  AT91C_BASE_SPI->SPI_CR = AT91C_SPI_SWRST;
  /* Configuration.*/
  AT91C_BASE_SPI->SPI_MR = spip->spd_config->spc_mr;
  AT91C_BASE_SPI->SPI_CSR[MMC_CSR_NUM] = spip->spd_config->spc_csr;
  /* Enable SPI */
  AT91C_BASE_SPI->SPI_CR = AT91C_SPI_SPIEN;
}

/**
 * @brief Deactivates the SPI peripheral.
 *
 * @param[in] spip      pointer to the @p SPIDriver object
 */
void spi_lld_stop(SPIDriver *spip) {

  if (spip->spd_state != SPI_STOP) {
    /* disable SPI */
    AT91C_BASE_SPI->SPI_CR = AT91C_SPI_SPIDIS;
    /* disable clocks to SPI perepheral */
    AT91C_BASE_PMC->PMC_PCDR = (1 << AT91C_ID_SPI);
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
 * @brief Sends data over the SPI bus.
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
