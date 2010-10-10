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
 *          clock pulses without asserting any slave, while this is not
 *          usually required by the SPI protocol it is required by
 *          initialization procedure of MMC/SD cards in SPI mode.
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

  chDbgCheck((spip != NULL) && (n > 0), "spiIgnore");

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
              "not active");
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
              "not active");
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
              "not active");
  spiReceiveI(spip, n, rxbuf);
  chSysUnlock();
}

#if SPI_USE_WAIT || defined(__DOXYGEN__)
/**
 * @brief   Awakens the thread waiting for operation completion, if any.
 *
 * @param[in] spip      pointer to the @p SPIDriver object
 *
 * @notapi
 */
void _spi_wakeup(SPIDriver *spip) {

  if (spip->spd_thread != NULL) {
    Thread *tp = spip->spd_thread;
    spip->spd_thread = NULL;
    tp->p_u.rdymsg = RDY_RESET;
    chSchReadyI(tp);
  }
}

/**
 * @brief   Wait for operation completion.
 * @details This function waits for the driver to complete the current
 *          operation, if an operation is not running when the function is
 *          invoked then it immediately returns.
 * @note    No more than one thread can wait on a SPI driver using
 *          this function.
 *
 * @param[in] spip      pointer to the @p SPIDriver object
 * @return              The wait status.
 * @retval RDY_OK       There was not operation running when the function
 *                      has been invoked.
 * @retval RDY_RESET    The operation completed.
 */
msg_t spiWait(SPIDriver *spip) {
  msg_t msg;

  chDbgCheck(spip != NULL, "spiWait");

  chSysLock();
  chDbgAssert((spip->spd_state == SPI_READY) ||
              (spip->spd_state == SPI_SELECTED) ||
              (spip->spd_state == SPI_ACTIVE) ||
              (spip->spd_state == SPI_SYNC),
              "spiUnselect(), #1",
              "invalid state");
  chDbgAssert(spip->spd_thread == NULL, "spiWait(), #3", "already waiting");
  if ((spip->spd_state == SPI_ACTIVE) || (spip->spd_state == SPI_SYNC)) {
    spip->spd_thread = chThdSelf();
    chSchGoSleepS(spip->spd_thread, THD_STATE_SUSPENDED);
    msg = chThdSelf()->p_u.rdymsg;
  }
  else
    msg = RDY_OK;
  chSysUnlock();
  return msg;
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
