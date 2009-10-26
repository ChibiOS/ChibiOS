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
 * @file templates/spi_lld.c
 * @brief SPI Driver subsystem low level driver source template
 * @addtogroup SPI_LLD
 * @{
 */

#include <ch.h>
#include <spi.h>

/*===========================================================================*/
/* Low Level Driver local functions.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Low Level Driver interrupt handlers.                                      */
/*===========================================================================*/

/*===========================================================================*/
/* Low Level Driver exported functions.                                      */
/*===========================================================================*/

/**
 * @brief Low level SPI driver initialization.
 */
void spi_lld_init(void) {

}

/**
 * @brief Low level SPI bus setup.
 *
 * @param[in] spip pointer to the @p SPIDriver object
 */
void spi_lld_setup(SPIDriver *spip) {

}

/**
 * @brief Asserts the slave select signal and prepares for transfers.
 *
 * @param[in] spip pointer to the @p SPIDriver object
 */
void spi_lld_select(SPIDriver *spip) {

}

/**
 * @brief De-asserts the slave select signal.
 * @details The previously selected peripheral is unselected.
 *
 * @param[in] spip pointer to the @p SPIDriver object
 */
void spi_lld_unselect(SPIDriver *spip) {

}

/**
 * @brief Exchanges data on the SPI bus.
 * @details This function performs a simultaneous transmit/receive operation.
 *
 * @param[in] spip pointer to the @p SPIDriver object
 * @param n number of words to exchange
 * @param rxbuf the pointer to the receive buffer
 * @param txbuf the pointer to the transmit buffer
 * @return The operation status is returned.
 * @retval RDY_OK operation complete.
 * @retval RDY_RESET hardware failure.
 *
 * @note The buffers are organized as uint8_t arrays for data sizes below or
 *       equal to 8 bits else it is organized as uint16_t arrays.
 */
msg_t spi_lld_exchange(SPIDriver *spip, size_t n, void *rxbuf, void *txbuf) {

}

/**
 * @brief Sends data ever the SPI bus.
 *
 * @param[in] spip pointer to the @p SPIDriver object
 * @param n number of words to send
 * @param txbuf the pointer to the transmit buffer
 * @return The operation status is returned.
 * @retval RDY_OK operation complete.
 * @retval RDY_RESET hardware failure.
 *
 * @note The buffers are organized as uint8_t arrays for data sizes below or
 *       equal to 8 bits else it is organized as uint16_t arrays.
 */
msg_t spi_lld_send(SPIDriver *spip, size_t n, void *txbuf) {

}

/**
 * @brief Receives data from the SPI bus.
 *
 * @param[in] spip pointer to the @p SPIDriver object
 * @param n number of words to receive
 * @param rxbuf the pointer to the receive buffer
 * @return The operation status is returned.
 * @retval RDY_OK operation complete.
 * @retval RDY_RESET hardware failure.
 *
 * @note The buffers are organized as uint8_t arrays for data sizes below or
 *       equal to 8 bits else it is organized as uint16_t arrays.
 */
msg_t spi_lld_receive(SPIDriver *spip, size_t n, void *rxbuf) {

}

/** @} */
