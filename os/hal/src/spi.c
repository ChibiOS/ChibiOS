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
 * @file    spi.c
 * @brief   SPI Driver code.
 *
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
 * @brief   SPI Driver initialization.
 *
 * @init
 */
void spiInit(void) {

  spi_lld_init();
}

/**
 * @brief   Initializes the standard part of a @p SPIDriver structure.
 *
 * @param[in] spip      pointer to the @p SPIDriver object
 *
 * @init
 */
void spiObjectInit(SPIDriver *spip) {

  spip->spd_state = SPI_STOP;
#if SPI_USE_WAIT
  spip->spd_thread = NULL;
#endif /* SPI_USE_WAIT */
#if SPI_USE_MUTUAL_EXCLUSION
#if CH_USE_MUTEXES
  chMtxInit(&spip->spd_mutex);
#else
  chSemInit(&spip->spd_semaphore, 1);
#endif
#endif /* SPI_USE_MUTUAL_EXCLUSION */
  spip->spd_config = NULL;
  /* Optional, user-defined initializer.*/
#if defined(SPI_DRIVER_EXT_INIT_HOOK)
  SPI_DRIVER_EXT_INIT_HOOK(spip);
#endif
}

/**
 * @brief   Configures and activates the SPI peripheral.
 *
 * @param[in] spip      pointer to the @p SPIDriver object
 * @param[in] config    pointer to the @p SPIConfig object
 *
 * @api
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
 *
 * @api
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
 * @brief   Asserts the slave select signal and prepares for transfers.
 *
 * @param[in] spip      pointer to the @p SPIDriver object
 *
 * @api
 */
void spiSelect(SPIDriver *spip) {

  chDbgCheck(spip != NULL, "spiSelect");

  chSysLock();
  chDbgAssert((spip->spd_state == SPI_READY) ||
              (spip->spd_state == SPI_SELECTED),
              "spiSelect(), #1",
              "not idle");
  spiSelectI(spip);
  chSysUnlock();
}

/**
 * @brief   Deasserts the slave select signal.
 * @details The previously selected peripheral is unselected.
 *
 * @param[in] spip      pointer to the @p SPIDriver object
 *
 * @api
 */
void spiUnselect(SPIDriver *spip) {

  chDbgCheck(spip != NULL, "spiUnselect");

  chSysLock();
  chDbgAssert((spip->spd_state == SPI_READY) ||
              (spip->spd_state == SPI_SELECTED),
              "spiUnselect(), #1",
              "not locked");
  spiUnselectI(spip);
  chSysUnlock();
}

/**
 * @brief   Emits a train of clock pulses on the SPI bus.
 * @details This asynchronous function starts the emission of a train of
 *          clock pulses without asserting any slave.
 * @note    This functionality is not usually required by the SPI protocol
 *          but it is required by initialization procedure of MMC/SD cards
 *          in SPI mode.
 * @post    At the end of the operation the configured callback is invoked.
 *
 * @param[in] spip      pointer to the @p SPIDriver object
 * @param[in] n         number of words to be clocked. The number of pulses
 *                      is equal to the number of words multiplied to the
 *                      configured word size in bits.
 *
 * @api
 */
void spiSynchronize(SPIDriver *spip, size_t n) {

  chDbgCheck((spip != NULL) && (n > 0), "spiSynchronize");

  chSysLock();
  chDbgAssert(spip->spd_state == SPI_READY,
              "spiSynchronize(), #1",
              "not ready");
  spiSynchronizeI(spip, n);
  chSysUnlock();
}

/**
 * @brief   Ignores data on the SPI bus.
 * @details This asynchronous function starts the transmission of a series of
 *          idle words on the SPI bus and ignores the received data.
 * @pre     A slave must have been selected using @p spiSelect() or
 *          @p spiSelectI().
 * @post    At the end of the operation the configured callback is invoked.
 *
 * @param[in] spip      pointer to the @p SPIDriver object
 * @param[in] n         number of words to be ignored
 *
 * @api
 */
void spiIgnore(SPIDriver *spip, size_t n) {

  chDbgCheck((spip != NULL) && (n > 0), "spiIgnore");

  chSysLock();
  chDbgAssert(spip->spd_state == SPI_SELECTED,
              "spiIgnore(), #1",
              "not selected");
  spiIgnoreI(spip, n);
  chSysUnlock();
}

/**
 * @brief   Exchanges data on the SPI bus.
 * @details This asynchronous function starts a simultaneous transmit/receive
 *          operation.
 * @pre     A slave must have been selected using @p spiSelect() or
 *          @p spiSelectI().
 * @post    At the end of the operation the configured callback is invoked.
 * @note    The buffers are organized as uint8_t arrays for data sizes below
 *          or equal to 8 bits else it is organized as uint16_t arrays.
 *
 * @param[in] spip      pointer to the @p SPIDriver object
 * @param[in] n         number of words to be exchanged
 * @param[in] txbuf     the pointer to the transmit buffer
 * @param[out] rxbuf    the pointer to the receive buffer
 *
 * @api
 */
void spiExchange(SPIDriver *spip, size_t n, const void *txbuf, void *rxbuf) {

  chDbgCheck((spip != NULL) && (n > 0) && (rxbuf != NULL) && (txbuf != NULL),
             "spiExchange");

  chSysLock();
  chDbgAssert(spip->spd_state == SPI_SELECTED,
              "spiExchange(), #1",
              "not selected");
  spiExchangeI(spip, n, txbuf, rxbuf);
  chSysUnlock();
}

/**
 * @brief   Sends data over the SPI bus.
 * @details This asynchronous function starts a transmit operation.
 * @pre     A slave must have been selected using @p spiSelect() or
 *          @p spiSelectI().
 * @post    At the end of the operation the configured callback is invoked.
 * @note    The buffers are organized as uint8_t arrays for data sizes below
 *          or equal to 8 bits else it is organized as uint16_t arrays.
 *
 * @param[in] spip      pointer to the @p SPIDriver object
 * @param[in] n         number of words to send
 * @param[in] txbuf     the pointer to the transmit buffer
 *
 * @api
 */
void spiSend(SPIDriver *spip, size_t n, const void *txbuf) {

  chDbgCheck((spip != NULL) && (n > 0) && (txbuf != NULL),
             "spiSend");

  chSysLock();
  chDbgAssert(spip->spd_state == SPI_SELECTED,
              "spiSend(), #1",
              "not selected");
  spiSendI(spip, n, txbuf);
  chSysUnlock();
}

/**
 * @brief   Receives data from the SPI bus.
 * @details This asynchronous function starts a receive operation.
 * @pre     A slave must have been selected using @p spiSelect() or
 *          @p spiSelectI().
 * @post    At the end of the operation the configured callback is invoked.
 * @note    The buffers are organized as uint8_t arrays for data sizes below
 *          or equal to 8 bits else it is organized as uint16_t arrays.
 *
 * @param[in] spip      pointer to the @p SPIDriver object
 * @param[in] n         number of words to receive
 * @param[out] rxbuf    the pointer to the receive buffer
 *
 * @api
 */
void spiReceive(SPIDriver *spip, size_t n, void *rxbuf) {

  chDbgCheck((spip != NULL) && (n > 0) && (rxbuf != NULL),
             "spiReceive");

  chSysLock();
  chDbgAssert(spip->spd_state == SPI_SELECTED,
              "spiReceive(), #1",
              "not selected");
  spiReceiveI(spip, n, rxbuf);
  chSysUnlock();
}

#if SPI_USE_WAIT || defined(__DOXYGEN__)
/**
 * @brief   Waits for operation completion.
 * @details This function waits for the driver to complete the current
 *          operation, if an operation is not running when the function is
 *          invoked then it immediately returns.
 * @pre     In order to use this function the option @p SPI_USE_WAIT must be
 *          enabled.
 * @post    On exit the SPI driver is ready to accept more commands.
 * @note    No more than one thread can wait on a SPI driver using
 *          this function.
 *
 * @param[in] spip      pointer to the @p SPIDriver object
 *
 * @api
 */
void spiWait(SPIDriver *spip) {

  chDbgCheck(spip != NULL, "spiWait");

  chSysLock();
  chDbgAssert((spip->spd_state == SPI_READY) ||
              (spip->spd_state == SPI_SELECTED) ||
              (spip->spd_state == SPI_ACTIVE) ||
              (spip->spd_state == SPI_SYNC),
              "spiWait(), #1",
              "invalid state");
  if ((spip->spd_state == SPI_ACTIVE) || (spip->spd_state == SPI_SYNC))
    spiWaitS(spip);
  chSysUnlock();
}

/**
 * @brief   Emits a train of clock pulses on the SPI bus.
 * @details This synchronous function performs the emission of a train of
 *          clock pulses without asserting any slave.
 * @pre     In order to use this function the option @p SPI_USE_WAIT must be
 *          enabled.
 * @post    At the end of the operation the configured callback is invoked.
 * @note    This functionality is not usually required by the SPI protocol
 *          but it is required by initialization procedure of MMC/SD cards
 *          in SPI mode.
 *
 * @param[in] spip      pointer to the @p SPIDriver object
 * @param[in] n         number of words to be clocked. The number of pulses
 *                      is equal to the number of words multiplied to the
 *                      configured word size in bits.
 *
 * @api
 */
void spiSynchronizeWait(SPIDriver *spip, size_t n) {

  chDbgCheck((spip != NULL) && (n > 0), "spiSynchronizeWait");

  chSysLock();
  chDbgAssert(spip->spd_state == SPI_READY,
              "spiSynchronizeWait(), #1",
              "not ready");
  spiSynchronizeI(spip, n);
  spiWaitS(spip);
  chSysUnlock();
}

/**
 * @brief   Ignores data on the SPI bus.
 * @details This synchronous function performs the transmission of a series of
 *          idle words on the SPI bus and ignores the received data.
 * @pre     A slave must have been selected using @p spiSelect() or
 *          @p spiSelectI().
 * @pre     In order to use this function the option @p SPI_USE_WAIT must be
 *          enabled.
 * @post    At the end of the operation the configured callback is invoked.
 *
 * @param[in] spip      pointer to the @p SPIDriver object
 * @param[in] n         number of words to be ignored
 *
 * @api
 */
void spiIgnoreWait(SPIDriver *spip, size_t n) {

  chDbgCheck((spip != NULL) && (n > 0), "spiIgnoreWait");

  chSysLock();
  chDbgAssert(spip->spd_state == SPI_SELECTED,
              "spiIgnoreWait(), #1",
              "not selected");
  spiIgnoreI(spip, n);
  spiWaitS(spip);
  chSysUnlock();
}

/**
 * @brief   Exchanges data on the SPI bus.
 * @details This synchronous function performs a simultaneous transmit/receive
 *          operation.
 * @pre     A slave must have been selected using @p spiSelect() or
 *          @p spiSelectI().
 * @pre     In order to use this function the option @p SPI_USE_WAIT must be
 *          enabled.
 * @post    At the end of the operation the configured callback is invoked.
 * @note    The buffers are organized as uint8_t arrays for data sizes below
 *          or equal to 8 bits else it is organized as uint16_t arrays.
 *
 * @param[in] spip      pointer to the @p SPIDriver object
 * @param[in] n         number of words to be exchanged
 * @param[in] txbuf     the pointer to the transmit buffer
 * @param[out] rxbuf    the pointer to the receive buffer
 *
 * @api
 */
void spiExchangeWait(SPIDriver *spip, size_t n,
                     const void *txbuf, void *rxbuf) {

  chDbgCheck((spip != NULL) && (n > 0) && (rxbuf != NULL) && (txbuf != NULL),
             "spiExchangeWait");

  chSysLock();
  chDbgAssert(spip->spd_state == SPI_SELECTED,
              "spiExchangeWait(), #1",
              "not selected");
  spiExchangeI(spip, n, txbuf, rxbuf);
  spiWaitS(spip);
  chSysUnlock();
}

/**
 * @brief   Sends data over the SPI bus.
 * @details This synchronous function performs a transmit operation.
 * @pre     A slave must have been selected using @p spiSelect() or
 *          @p spiSelectI().
 * @pre     In order to use this function the option @p SPI_USE_WAIT must be
 *          enabled.
 * @post    At the end of the operation the configured callback is invoked.
 * @note    The buffers are organized as uint8_t arrays for data sizes below
 *          or equal to 8 bits else it is organized as uint16_t arrays.
 *
 * @param[in] spip      pointer to the @p SPIDriver object
 * @param[in] n         number of words to send
 * @param[in] txbuf     the pointer to the transmit buffer
 *
 * @api
 */
void spiSendWait(SPIDriver *spip, size_t n, const void *txbuf) {

  chDbgCheck((spip != NULL) && (n > 0) && (txbuf != NULL),
             "spiSendWait");

  chSysLock();
  chDbgAssert(spip->spd_state == SPI_SELECTED,
              "spiSendWait(), #1",
              "not selected");
  spiSendI(spip, n, txbuf);
  spiWaitS(spip);
  chSysUnlock();
}

/**
 * @brief   Receives data from the SPI bus.
 * @details This synchronous function performs a receive operation.
 * @pre     A slave must have been selected using @p spiSelect() or
 *          @p spiSelectI().
 * @pre     In order to use this function the option @p SPI_USE_WAIT must be
 *          enabled.
 * @post    At the end of the operation the configured callback is invoked.
 * @note    The buffers are organized as uint8_t arrays for data sizes below
 *          or equal to 8 bits else it is organized as uint16_t arrays.
 *
 * @param[in] spip      pointer to the @p SPIDriver object
 * @param[in] n         number of words to receive
 * @param[out] rxbuf    the pointer to the receive buffer
 *
 * @api
 */
void spiReceiveWait(SPIDriver *spip, size_t n, void *rxbuf) {

  chDbgCheck((spip != NULL) && (n > 0) && (rxbuf != NULL),
             "spiReceiveWait");

  chSysLock();
  chDbgAssert(spip->spd_state == SPI_SELECTED,
              "spiReceiveWait(), #1",
              "not selected");
  spiReceiveI(spip, n, rxbuf);
  spiWaitS(spip);
  chSysUnlock();
}
#endif /* SPI_USE_WAIT */

#if SPI_USE_MUTUAL_EXCLUSION || defined(__DOXYGEN__)
/**
 * @brief   Gains exclusive access to the SPI bus.
 * @details This function tries to gain ownership to the SPI bus, if the bus
 *          is already being used then the invoking thread is queued.
 * @pre     In order to use this function the option @p SPI_USE_MUTUAL_EXCLUSION
 *          must be enabled.
 *
 * @param[in] spip      pointer to the @p SPIDriver object
 *
 * @api
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
 * @brief   Releases exclusive access to the SPI bus.
 * @pre     In order to use this function the option @p SPI_USE_MUTUAL_EXCLUSION
 *          must be enabled.
 *
 * @param[in] spip      pointer to the @p SPIDriver object
 *
 * @api
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
