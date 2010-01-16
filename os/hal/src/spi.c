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
 * @file spi.c
 * @brief SPI Driver code.
 * @addtogroup SPI
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
/* Driver exported functions.                                                */
/*===========================================================================*/

/**
 * @brief SPI Driver initialization.
 */
void spiInit(void) {

  spi_lld_init();
}

/**
 * @brief Initializes the standard part of a @p SPIDriver structure.
 *
 * @param[in] spip      pointer to the @p SPIDriver object
 */
void spiObjectInit(SPIDriver *spip) {

  spip->spd_state = SPI_STOP;
#if CH_USE_MUTEXES
  chMtxInit(&spip->spd_mutex);
#elif CH_USE_SEMAPHORES
  chSemInit(&spip->spd_semaphore, 1);
#endif
  spip->spd_config = NULL;
}

/**
 * @brief Configures and activates the SPI peripheral.
 *
 * @param[in] spip      pointer to the @p SPIDriver object
 * @param[in] config    pointer to the @p SPIConfig object
 */
void spiStart(SPIDriver *spip, const SPIConfig *config) {

  chDbgCheck((spip != NULL) && (config != NULL), "spiStart");

  chSysLock();
  chDbgAssert((spip->spd_state == SPI_STOP) || (spip->spd_state == SPI_READY),
              "spiStart(), #1",
              "invalid state");
  spip->spd_config = config;
  spi_lld_start(spip);
  spip->spd_state = SPI_READY;
  chSysUnlock();
}

/**
 * @brief Deactivates the SPI peripheral.
 *
 * @param[in] spip      pointer to the @p SPIDriver object
 */
void spiStop(SPIDriver *spip) {

  chDbgCheck(spip != NULL, "spiStop");

  chSysLock();
  chDbgAssert((spip->spd_state == SPI_STOP) || (spip->spd_state == SPI_READY),
              "spiStop(), #1",
              "invalid state");
  spi_lld_stop(spip);
  spip->spd_state = SPI_STOP;
  chSysUnlock();
}

/**
 * @brief Asserts the slave select signal and prepares for transfers.
 *
 * @param[in] spip      pointer to the @p SPIDriver object
 */
void spiSelect(SPIDriver *spip) {

  chDbgCheck(spip != NULL, "spiSelect");

  chSysLock();
  chDbgAssert((spip->spd_state == SPI_READY) ||
              (spip->spd_state == SPI_ACTIVE),
              "spiSelect(), #1",
              "not idle");
  spi_lld_select(spip);
  spip->spd_state = SPI_ACTIVE;
  chSysUnlock();
}

/**
 * @brief Deasserts the slave select signal.
 * @details The previously selected peripheral is unselected.
 *
 * @param[in] spip      pointer to the @p SPIDriver object
 */
void spiUnselect(SPIDriver *spip) {

  chDbgCheck(spip != NULL, "spiUnselect");

  chSysLock();
  chDbgAssert((spip->spd_state == SPI_READY) ||
              (spip->spd_state == SPI_ACTIVE),
              "spiUnselect(), #1",
              "not locked");
  spi_lld_unselect(spip);
  spip->spd_state = SPI_READY;
  chSysUnlock();
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
void spiIgnore(SPIDriver *spip, size_t n) {

  chDbgCheck((spip != NULL) && (n > 0), "spiIgnore");
  chDbgAssert((spip->spd_state == SPI_READY) || (spip->spd_state == SPI_ACTIVE),
              "spiIgnore(), #1",
              "not active");

  spi_lld_ignore(spip, n);
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
void spiExchange(SPIDriver *spip, size_t n, const void *txbuf, void *rxbuf) {

  chDbgCheck((spip != NULL) && (n > 0) && (rxbuf != NULL) && (txbuf != NULL),
             "spiExchange");
  chDbgAssert(spip->spd_state == SPI_ACTIVE,
              "spiExchange(), #1",
              "not active");

  spi_lld_exchange(spip, n, txbuf, rxbuf);
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
void spiSend(SPIDriver *spip, size_t n, const void *txbuf) {

  chDbgCheck((spip != NULL) && (n > 0) && (txbuf != NULL),
             "spiSend");
  chDbgAssert(spip->spd_state == SPI_ACTIVE,
              "spiSend(), #1",
              "not active");

  spi_lld_send(spip, n, txbuf);
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
void spiReceive(SPIDriver *spip, size_t n, void *rxbuf) {

  chDbgCheck((spip != NULL) && (n > 0) && (rxbuf != NULL),
             "spiReceive");
  chDbgAssert(spip->spd_state == SPI_ACTIVE,
              "spiReceive(), #1",
              "not active");

  spi_lld_receive(spip, n, rxbuf);
}

#if SPI_USE_MUTUAL_EXCLUSION || defined(__DOXYGEN__)
/**
 * @brief Gains exclusive access to the SPI bus.
 * @details This function tries to gain ownership to the SPI bus, if the bus
 *          is already being used then the invoking thread is queued.
 *
 * @param[in] spip      pointer to the @p SPIDriver object
 *
 * @note This function is only available when the @p SPI_USE_MUTUAL_EXCLUSION
 *       option is set to @p TRUE.
 */
void spiAcquireBus(SPIDriver *spip) {

  chDbgCheck(spip != NULL, "spiAcquireBus");

#if CH_USE_MUTEXES
  chMtxLock(&spip->spd_mutex);
#elif CH_USE_SEMAPHORES
  chSemWait(&spip->spd_semaphore);
#endif
}

/**
 * @brief Releases exclusive access to the SPI bus.
 *
 * @param[in] spip      pointer to the @p SPIDriver object
 *
 * @note This function is only available when the @p SPI_USE_MUTUAL_EXCLUSION
 *       option is set to @p TRUE.
 */
void spiReleaseBus(SPIDriver *spip) {

  chDbgCheck(spip != NULL, "spiReleaseBus");

#if CH_USE_MUTEXES
  (void)spip;
  chMtxUnlock();
#elif CH_USE_SEMAPHORES
  chSemSignal(&spip->spd_semaphore);
#endif
}
#endif /* SPI_USE_MUTUAL_EXCLUSION */

#endif /* CH_HAL_USE_SPI */

/** @} */
