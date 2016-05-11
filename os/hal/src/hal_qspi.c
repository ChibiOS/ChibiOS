/*
    ChibiOS - Copyright (C) 2006..2016 Giovanni Di Sirio

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

        http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.
*/

/**
 * @file    hal_qspi.c
 * @brief   QSPI Driver code.
 *
 * @addtogroup QSPI
 * @{
 */

#include "hal.h"

#if (HAL_USE_QSPI == TRUE) || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver local definitions.                                                 */
/*===========================================================================*/

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local variables and types.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

/**
 * @brief   QSPI Driver initialization.
 * @note    This function is implicitly invoked by @p halInit(), there is
 *          no need to explicitly initialize the driver.
 *
 * @init
 */
void qspiInit(void) {

  qspi_lld_init();
}

/**
 * @brief   Initializes the standard part of a @p QSPIDriver structure.
 *
 * @param[out] qspip     pointer to the @p QSPIDriver object
 *
 * @init
 */
void qspiObjectInit(QSPIDriver *qspip) {

  qspip->state = QSPI_STOP;
  qspip->config = NULL;
#if QSPI_USE_WAIT == TRUE
  qspip->thread = NULL;
#endif
#if QSPI_USE_MUTUAL_EXCLUSION == TRUE
  osalMutexObjectInit(&qspip->mutex);
#endif
#if defined(QSPI_DRIVER_EXT_INIT_HOOK)
  QSPI_DRIVER_EXT_INIT_HOOK(qspip);
#endif
}

/**
 * @brief   Configures and activates the QSPI peripheral.
 *
 * @param[in] qspip     pointer to the @p QSPIDriver object
 * @param[in] config    pointer to the @p QSPIConfig object
 *
 * @api
 */
void qspiStart(QSPIDriver *qspip, const QSPIConfig *config) {

  osalDbgCheck((qspip != NULL) && (config != NULL));

  osalSysLock();
  osalDbgAssert((qspip->state == QSPI_STOP) || (qspip->state == QSPI_READY),
                "invalid state");
  qspip->config = config;
  qspi_lld_start(qspip);
  qspip->state = QSPI_READY;
  osalSysUnlock();
}

/**
 * @brief   Deactivates the QSPI peripheral.
 * @note    Deactivating the peripheral also enforces a release of the slave
 *          select line.
 *
 * @param[in] qspip     pointer to the @p QSPIDriver object
 *
 * @api
 */
void qspiStop(QSPIDriver *qspip) {

  osalDbgCheck(qspip != NULL);

  osalSysLock();
  osalDbgAssert((qspip->state == QSPI_STOP) || (qspip->state == QSPI_READY),
                "invalid state");
  qspi_lld_stop(qspip);
  qspip->state = QSPI_STOP;
  osalSysUnlock();
}

/**
 * @brief   Sends data over the QSPI bus.
 * @details This asynchronous function starts a transmit operation.
 * @post    At the end of the operation the configured callback is invoked.
 *
 * @param[in] qspip     pointer to the @p QSPIDriver object
 * @param[in] n         number of words to send
 * @param[in] txbuf     the pointer to the transmit buffer
 *
 * @api
 */
void qspiStartSend(QSPIDriver *qspip, size_t n, const void *txbuf) {

  osalDbgCheck((qspip != NULL) && (n > 0U) && (txbuf != NULL));

  osalSysLock();
  osalDbgAssert(qspip->state == QSPI_READY, "not ready");
  qspiStartSendI(qspip, n, txbuf);
  osalSysUnlock();
}

/**
 * @brief   Receives data from the QSPI bus.
 * @details This asynchronous function starts a receive operation.
 * @post    At the end of the operation the configured callback is invoked.
 *
 * @param[in] qspip     pointer to the @p QSPIDriver object
 * @param[in] n         number of words to receive
 * @param[out] rxbuf    the pointer to the receive buffer
 *
 * @api
 */
void qspiStartReceive(QSPIDriver *qspip, size_t n, void *rxbuf) {

  osalDbgCheck((qspip != NULL) && (n > 0U) && (rxbuf != NULL));

  osalSysLock();
  osalDbgAssert(qspip->state == QSPI_READY, "not ready");
  qspiStartReceiveI(qspip, n, rxbuf);
  osalSysUnlock();
}

#if (QSPI_USE_WAIT == TRUE) || defined(__DOXYGEN__)
/**
 * @brief   Sends data over the QSPI bus.
 * @details This synchronous function performs a transmit operation.
 * @pre     In order to use this function the option @p QSPI_USE_WAIT must be
 *          enabled.
 * @pre     In order to use this function the driver must have been configured
 *          without callbacks (@p end_cb = @p NULL).
 * @note    The buffers are organized as uint8_t arrays for data sizes below
 *          or equal to 8 bits else it is organized as uint16_t arrays.
 *
 * @param[in] qspip     pointer to the @p QSPIDriver object
 * @param[in] n         number of words to send
 * @param[in] txbuf     the pointer to the transmit buffer
 *
 * @api
 */
void qspiSend(QSPIDriver *qspip, size_t n, const void *txbuf) {

  osalDbgCheck((qspip != NULL) && (n > 0U) && (txbuf != NULL));

  osalSysLock();
  osalDbgAssert(qspip->state == QSPI_READY, "not ready");
  osalDbgAssert(qspip->config->end_cb == NULL, "has callback");
  qspiStartSendI(qspip, n, txbuf);
  (void) osalThreadSuspendS(&qspip->thread);
  osalSysUnlock();
}

/**
 * @brief   Receives data from the QSPI bus.
 * @details This synchronous function performs a receive operation.
 * @pre     In order to use this function the option @p QSPI_USE_WAIT must be
 *          enabled.
 * @pre     In order to use this function the driver must have been configured
 *          without callbacks (@p end_cb = @p NULL).
 * @note    The buffers are organized as uint8_t arrays for data sizes below
 *          or equal to 8 bits else it is organized as uint16_t arrays.
 *
 * @param[in] qspip     pointer to the @p QSPIDriver object
 * @param[in] n         number of words to receive
 * @param[out] rxbuf    the pointer to the receive buffer
 *
 * @api
 */
void qspiReceive(QSPIDriver *qspip, size_t n, void *rxbuf) {

  osalDbgCheck((qspip != NULL) && (n > 0U) && (rxbuf != NULL));

  osalSysLock();
  osalDbgAssert(qspip->state == QSPI_READY, "not ready");
  osalDbgAssert(qspip->config->end_cb == NULL, "has callback");
  qspiStartReceiveI(qspip, n, rxbuf);
  (void) osalThreadSuspendS(&qspip->thread);
  osalSysUnlock();
}
#endif /* QSPI_USE_WAIT == TRUE */

#if (QSPI_USE_MUTUAL_EXCLUSION == TRUE) || defined(__DOXYGEN__)
/**
 * @brief   Gains exclusive access to the QSPI bus.
 * @details This function tries to gain ownership to the QSPI bus, if the bus
 *          is already being used then the invoking thread is queued.
 * @pre     In order to use this function the option @p QSPI_USE_MUTUAL_EXCLUSION
 *          must be enabled.
 *
 * @param[in] qspip     pointer to the @p QSPIDriver object
 *
 * @api
 */
void qspiAcquireBus(QSPIDriver *qspip) {

  osalDbgCheck(qspip != NULL);

  osalMutexLock(&qspip->mutex);
}

/**
 * @brief   Releases exclusive access to the QSPI bus.
 * @pre     In order to use this function the option @p QSPI_USE_MUTUAL_EXCLUSION
 *          must be enabled.
 *
 * @param[in] qspip     pointer to the @p QSPIDriver object
 *
 * @api
 */
void qspiReleaseBus(QSPIDriver *qspip) {

  osalDbgCheck(qspip != NULL);

  osalMutexUnlock(&qspip->mutex);
}
#endif /* QSPI_USE_MUTUAL_EXCLUSION == TRUE */

#endif /* HAL_USE_QSPI == TRUE */

/** @} */
