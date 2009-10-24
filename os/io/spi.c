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
 * @file spi.c
 * @brief SPI Driver code.
 * @addtogroup SPI
 * @{
 */

#include <ch.h>
#include <spi.h>

/**
 * @brief SPI Driver initialization.
 */
void spiInit(void) {

  spi_lld_init();
}

/**
 * @brief Initializes the standard part of a @p SPIDriver structure.
 *
 * @param[in] spip pointer to the @p SPIDriver object
 */
void spiObjectInit(SPIDriver *spip) {

  spip->spd_state = SPI_IDLE;
#if CH_USE_MUTEXES
  chMtxInit(&spip->spd_mutex);
#elif CH_USE_SEMAPHORES
  chSemInit(&spip->spd_semaphore, 1);
#endif
  spip->spd_config = NULL;
}

/**
 * @brief Configures the SPI bus.
 *
 * @param[in] spip pointer to the @p SPIDriver object
 * @param config pointer to the @p SPIConfig object
 */
void spiSetup(SPIDriver *spip, const SPIConfig *config) {

  chDbgAssert(spip->spd_state == SPI_IDLE,
              "spiSetup(), #1",
              "not idle");

  spip->spd_config = config;
  spi_lld_setup(spip);
}

/**
 * @brief Asserts the slave select signal and prepares for transfers.
 *
 * @param[in] spip pointer to the @p SPIDriver object
 */
void spiSelect(SPIDriver *spip) {

  chSysLock();

  chDbgAssert(spip->spd_state == SPI_IDLE,
              "spiSelect(), #1",
              "not idle");

  spi_lld_select(spip);
  spip->spd_state = SPI_ACTIVE;
  chSysUnlock();
}

/**
 * @brief De-asserts the slave select signal.
 * @details The previously selected peripheral is unselected.
 *
 * @param[in] spip pointer to the @p SPIDriver object
 */
void spiUnselect(SPIDriver *spip) {

  chSysLock();

  chDbgAssert(spip->spd_state == SPI_ACTIVE,
              "spiUnselect(), #1",
              "not locked");

  spi_lld_unselect(spip);
  spip->spd_state = SPI_IDLE;
  chSysUnlock();
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
void spiExchange(SPIDriver *spip, size_t n, void *rxbuf, void *txbuf) {

  chDbgAssert(spip->spd_state == SPI_ACTIVE,
              "spiExchange(), #1",
              "not active");

  spi_lld_exchange(spip, n, rxbuf, txbuf);
}

#if SPI_USE_MUTUAL_EXCLUSION || defined(__DOXYGEN__)
/**
 * @brief Gains exclusive access to the SPI bus.
 * @details This function tries to gain ownership to the SPI bus, if the bus
 *          is already being used then the invoking thread is queued.
 *
 * @param[in] spip pointer to the @p SPIDriver object
 *
 * @note This function is only available when the @p SPI_USE_MUTUAL_EXCLUSION
 *       option is set to @p TRUE.
 */
void spiAcquireBus(SPIDriver *spip) {

#if CH_USE_MUTEXES
  chMtxLock(&spip->spd_mutex);
#elif CH_USE_SEMAPHORES
  chSemWait(&spip->spd_semaphore);
#endif
}

/**
 * @brief Releases exclusive access to the SPI bus.
 *
 * @param[in] spip pointer to the @p SPIDriver object
 *
 * @note This function is only available when the @p SPI_USE_MUTUAL_EXCLUSION
 *       option is set to @p TRUE.
 */
void spiReleaseBus(SPIDriver *spip) {

#if CH_USE_MUTEXES
  (void)spip;
  chMtxUnlock();
#elif CH_USE_SEMAPHORES
  chSemSignal(&spip->spd_semaphore);
#endif
}
#endif /*SPI_USE_MUTUAL_EXCLUSION */

/** @} */
