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
 * @file templates/spi_lld.c
 * @brief SPI Driver subsystem low level driver source template.
 * @addtogroup SPI_LLD
 * @{
 */

#include "ch.h"
#include "hal.h"

#if CH_HAL_USE_SPI || defined(__DOXYGEN__)

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
 * @brief Low level SPI driver initialization.
 */
void spi_lld_init(void) {

}

/**
 * @brief Configures and activates the SPI peripheral.
 *
 * @param[in] spip      pointer to the @p SPIDriver object
 */
void spi_lld_start(SPIDriver *spip) {

  if (spip->spd_state == SPI_STOP) {
    /* Clock activation.*/
  }
  /* Configuration.*/
}

/**
 * @brief Deactivates the SPI peripheral.
 *
 * @param[in] spip      pointer to the @p SPIDriver object
 */
void spi_lld_stop(SPIDriver *spip) {

}

/**
 * @brief Asserts the slave select signal and prepares for transfers.
 *
 * @param[in] spip      pointer to the @p SPIDriver object
 */
void spi_lld_select(SPIDriver *spip) {

}

/**
 * @brief Deasserts the slave select signal.
 * @details The previously selected peripheral is unselected.
 *
 * @param[in] spip      pointer to the @p SPIDriver object
 */
void spi_lld_unselect(SPIDriver *spip) {

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

}

#endif /* CH_HAL_USE_SPI */

/** @} */
