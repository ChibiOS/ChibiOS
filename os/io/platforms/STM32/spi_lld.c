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

#include "nvic.h"
#include "board.h"

#if USE_STM32_SPI1 || defined(__DOXYGEN__)
/** @brief SPI1 driver identifier.*/
SPIDriver SPID1;
#endif

#if USE_STM32_SPI2 || defined(__DOXYGEN__)
/** @brief SPI2 driver identifier.*/
SPIDriver SPID2;
#endif

/**
 * @brief Low level SPI driver initialization.
 */
void spi_lld_init(void) {

#if USE_STM32_SPI1
  spiObjectInit(&SPID1);
  SPID1.spd_spi = SPI1;
  SPID1.spd_dmarx = DMA1_Channel2;
  SPID1.spd_dmatx = DMA1_Channel3;
  RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;
  GPIOA->CRH = (GPIOA->CRH & 0x000FFFFF) | 0xB4B00000;
#endif

#if USE_STM32_SPI2
  spiObjectInit(&SPID2);
  SPID2.spd_spi = SPI2;
  SPID2.spd_dmarx = DMA1_Channel4;
  SPID2.spd_dmatx = DMA1_Channel5;
  RCC->APB1ENR |= RCC_APB1ENR_SPI2EN;
  GPIOB->CRL = (GPIOB->CRL & 0x000FFFFF) | 0xB4B00000;
#endif
}

/**
 * @brief Low level SPI bus setup.
 *
 * @param[in] spip pointer to the @p SPIDriver object
 */
void spi_lld_setup(SPIDriver *spip) {

}

/**
 * @brief Asserts the chip select signal and prepares for transfers.
 *
 * @param[in] spip pointer to the @p SPIDriver object
 */
void spi_lld_select(SPIDriver *spip) {

}

/**
 * @brief De-asserts the chip select signal.
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
 * @param n number of words to be exchanged
 * @param rxbuf the pointer to the receive buffer, if @p NULL is specified then
 *              the input data is discarded.
 *              Note that the buffer is organized as an uint8_t array for
 *              data sizes below or equal to 8 bits else it is organized as
 *              an uint16_t array.
 * @param txbuf the pointer to the transmit buffer, if @p NULL is specified all
 *              ones are transmitted.
 *              Note that the buffer is organized as an uint8_t array for
 *              data sizes below or equal to 8 bits else it is organized as
 *              an uint16_t array.
 */
void spi_lld_exchange(SPIDriver *spip, size_t n, void *rxbuf, void *txbuf) {

}

/** @} */
