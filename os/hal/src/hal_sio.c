/*
    ChibiOS - Copyright (C) 2006..2018 Giovanni Di Sirio

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
 * @file    hal_sio.c
 * @brief   SIO Driver code.
 *
 * @addtogroup SIO
 * @{
 */

#include "hal.h"

#if (HAL_USE_SIO == TRUE) || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver local definitions.                                                 */
/*===========================================================================*/

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local variables and types.                                         */
/*===========================================================================*/

static const SIOOperation default_operation = {
  .rx_cb      = NULL,
  .rx_idle_cb = NULL,
  .tx_cb      = NULL,
  .tx_end_cb  = NULL,
  .rx_evt_cb  = NULL
};

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

#if (SIO_USE_SYNCHRONIZATION == TRUE) || defined(__DOXYGEN__)
static size_t sync_write(void *ip, const uint8_t *bp, size_t n,
                         sysinterval_t timeout) {
  SIODriver *siop = (SIODriver *)ip;
  size_t i;

  i = 0U;
  while (i < n) {
    size_t written;
    msg_t msg;

    msg = sioSynchronizeTX(siop, timeout);
    if (msg != MSG_OK) {
      break;
    }

    written = sioAsyncWrite(siop, bp, n - i);
    i += written;
    bp += written;
  }
  return n;
}

static size_t sync_read(void *ip, uint8_t *bp, size_t n,
                        sysinterval_t timeout) {
  SIODriver *siop = (SIODriver *)ip;
  size_t i;

  i = 0U;
  while (i < n) {
    size_t read;
    msg_t msg;

    msg = sioSynchronizeTX(siop, timeout);
    if (msg != MSG_OK) {
      break;
    }

    read = sioAsyncRead(siop, bp, n - i);
    i += read;
    bp += read;
  }
  return n;
}

/*
 * Interface implementation, the following functions just invoke the equivalent
 * queue-level function or macro.
 */

static size_t __write(void *ip, const uint8_t *bp, size_t n) {

  return sync_write(ip, bp, n, TIME_INFINITE);
}

static size_t __read(void *ip, uint8_t *bp, size_t n) {

  return sync_read(ip, bp, n, TIME_INFINITE);
}

static msg_t __put(void *ip, uint8_t b) {
  SIODriver *siop = (SIODriver *)ip;
  msg_t msg;

  msg = sioSynchronizeTX(siop, TIME_INFINITE);
  if (msg != MSG_OK) {
    return msg;
  }

  sioPutX(siop, b);
  return MSG_OK;
}

static msg_t __get(void *ip) {
  SIODriver *siop = (SIODriver *)ip;
  msg_t msg;

  msg = sioSynchronizeRX(siop, TIME_INFINITE);
  if (msg != MSG_OK) {
    return msg;
  }

  return sioGetX(siop);
}

static msg_t __putt(void *ip, uint8_t b, sysinterval_t timeout) {
  SIODriver *siop = (SIODriver *)ip;
  msg_t msg;

  msg = sioSynchronizeTX(siop, timeout);
  if (msg != MSG_OK) {
    return MSG_RESET;
  }

  sioPutX(siop, b);
  return MSG_OK;
}

static msg_t __gett(void *ip, sysinterval_t timeout) {
  SIODriver *siop = (SIODriver *)ip;
  msg_t msg;

  msg = sioSynchronizeRX(siop, timeout);
  if (msg != MSG_OK) {
    return MSG_RESET;
  }

  return sioGetX(siop);
}

static size_t __writet(void *ip, const uint8_t *bp, size_t n,
                       sysinterval_t timeout) {

  return sync_write(ip, bp, n, timeout);
}

static size_t __readt(void *ip, uint8_t *bp, size_t n,
                      sysinterval_t timeout) {

  return sync_read(ip, bp, n, timeout);
}

static msg_t __ctl(void *ip, unsigned int operation, void *arg) {
  SIODriver *siop = (SIODriver *)ip;

  osalDbgCheck(siop != NULL);

  switch (operation) {
  case CHN_CTL_NOP:
    osalDbgCheck(arg == NULL);
    break;
  case CHN_CTL_INVALID:
    osalDbgAssert(false, "invalid CTL operation");
    break;
  default:
    break;
  }
  return MSG_OK;
}

static const struct sio_driver_vmt vmt = {
  (size_t)0,
  __write, __read, __put,    __get,
  __putt,  __gett, __writet, __readt,
  __ctl
};
#endif

/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

/**
 * @brief   SIO Driver initialization.
 * @note    This function is implicitly invoked by @p halInit(), there is
 *          no need to explicitly initialize the driver.
 *
 * @init
 */
void sioInit(void) {

  sio_lld_init();
}

/**
 * @brief   Initializes the standard part of a @p SIODriver structure.
 *
 * @param[out] siop     pointer to the @p SIODriver object
 *
 * @init
 */
void sioObjectInit(SIODriver *siop) {

#if SIO_USE_SYNCHRONIZATION == TRUE
  siop->vmt     = &vmt;
#endif
  siop->state   = SIO_STOP;
  siop->config  = NULL;

  /* Optional, user-defined initializer.*/
#if defined(SIO_DRIVER_EXT_INIT_HOOK)
  SIO_DRIVER_EXT_INIT_HOOK(siop);
#endif
}

/**
 * @brief   Configures and activates the SIO peripheral.
 *
 * @param[in] siop      pointer to the @p SIODriver object
 * @param[in] config    pointer to the @p SIOConfig object, can be @p NULL
 *                      if the default configuration is desired
 * @return              The operation status.
 * @retval false        if the driver has been correctly started.
 * @retval true         if an error occurred.
 *
 * @api
 */
bool sioStart(SIODriver *siop, const SIOConfig *config) {
  bool result;

  osalDbgCheck(siop != NULL);

  osalSysLock();

  osalDbgAssert((siop->state == SIO_STOP) || (siop->state == SIO_READY),
                "invalid state");

  siop->config = config;
  result = sio_lld_start(siop);
  siop->state = SIO_READY;

  osalSysUnlock();

  return result;
}

/**
 * @brief   Deactivates the SIO peripheral.
 *
 * @param[in] siop      pointer to the @p SIODriver object
 *
 * @api
 */
void sioStop(SIODriver *siop) {

  osalDbgCheck(siop != NULL);

  osalSysLock();

  osalDbgAssert((siop->state == SIO_STOP) || (siop->state == SIO_READY),
                "invalid state");

  sio_lld_stop(siop);
  siop->config  = NULL;
  siop->state   = SIO_STOP;

  osalSysUnlock();
}

/**
 * @brief   Starts a SIO operation.
 *
 * @param[in] siop          pointer to an @p SIODriver structure
 * @param[in] operation     pointer to an @p SIOOperation structure, can
 *                          be @p NULL if callbacks are not required
 *                          encoding the operation to be performed
 *
 * @api
 */
void sioStartOperation(SIODriver *siop, const SIOOperation *operation) {

  osalDbgCheck(siop != NULL);

  osalSysLock();

  osalDbgAssert(siop->state == SIO_READY, "invalid state");

  /* The application can pass NULL if it is not interested in callbacks,
     attaching a default operation structure.*/
  if (operation != NULL) {
    siop->operation = operation;
  }
  else {
    siop->operation = &default_operation;
  }
  siop->state     = SIO_ACTIVE;

  sio_lld_start_operation(siop);

  osalSysUnlock();
}

/**
 * @brief   Stops an ongoing SIO operation, if any.
 *
 * @param[in] siop      pointer to an @p SIODriver structure
 *
 * @api
 */
void sioStopOperation(SIODriver *siop) {

  osalDbgCheck(siop != NULL);

  osalSysLock();

  osalDbgAssert(siop->state == SIO_ACTIVE, "invalid state");

#if SIO_USE_SYNCHRONIZATION == TRUE
  /* Informing waiting threads, if any.*/
  osalThreadResumeI(&siop->sync_rx, MSG_RESET);
  osalThreadResumeI(&siop->sync_tx, MSG_RESET);
  osalThreadResumeI(&siop->sync_txend, MSG_RESET);
#endif

  sio_lld_stop_operation(siop);

  siop->operation = NULL;
  siop->state     = SIO_READY;

  osalSysUnlock();
}

/**
 * @brief   Return the pending SIO events flags.
 *
 * @param[in] siop      pointer to the @p SIODriver object
 * @return              The pending event flags.
 *
 * @api
 */
sio_events_mask_t sioGetAndClearEvents(SIODriver *siop) {
  sio_events_mask_t evtmask;

  osalDbgCheck(siop != NULL);

  osalSysLock();

  evtmask = sioGetAndClearEventsI(siop);

  osalSysUnlock();

  return evtmask;
}

/**
 * @brief   Reads data from the RX FIFO.
 * @details This function is non-blocking, data is read if present and the
 *          effective amount is returned.
 * @note    This function can be called from any context but it is meant to
 *          be called from the @p rxne_cb callback handler.
 *
 * @param[in] siop      pointer to the @p SIODriver object
 * @param[in] buffer    buffer for the received data
 * @param[in] n         maximum number of frames to read
 * @return              The number of received frames.
 *
 * @api
 */
size_t sioAsyncRead(SIODriver *siop, uint8_t *buffer, size_t n) {

  osalDbgCheck((siop != NULL) && (buffer != NULL));

  osalSysLock();

  n = sioAsyncReadI(siop, buffer, n);

  osalSysUnlock();

  return n;
}

/**
 * @brief   Writes data into the TX FIFO.
 * @details This function is non-blocking, data is written if there is space
 *          in the FIFO and the effective amount is returned.
 * @note    This function can be called from any context but it is meant to
 *          be called from the @p txnf_cb callback handler.
 *
 * @param[in] siop      pointer to the @p SIODriver object
 * @param[out] buffer   buffer containing the data to be transmitted
 * @param[in] n         maximum number of frames to read
 * @return              The number of transmitted frames.
 *
 * @api
 */
size_t sioAsyncWrite(SIODriver *siop, const uint8_t *buffer, size_t n) {

  osalDbgCheck((siop != NULL) && (buffer != NULL));

  osalSysLock();

  n = sioAsyncWriteI(siop, buffer, n);

  osalSysUnlock();

  return n;
}

#if (SIO_USE_SYNCHRONIZATION == TRUE) || defined(__DOXYGEN__)
/**
 * @brief   Synchronizes with RX FIFO data availability.
 * @note    The exact behavior depends on low level FIFO settings such
 *          as thresholds, etc.
 * @note    This function can only be called by a single thread at time.
 *
 * @param[in] siop          pointer to an @p SIODriver structure
 * @param[in] timeout       synchronization timeout
 * @return                  The synchronization result.
 * @retval MSG_OK           if there is data in the RX FIFO.
 * @retval MSG_TIMEOUT      if synchronization timed out.
 * @retval MSG_RESET        operation has been stopped while waiting.
 * @retval SIO_MSG_IDLE     if RX line went idle.
 * @retval SIO_MSG_ERRORS   if RX errors occurred during wait.
 */
msg_t sioSynchronizeRX(SIODriver *siop, sysinterval_t timeout) {
  msg_t msg;

  osalDbgCheck(siop != NULL);

  osalSysLock();

  osalDbgAssert(siop->state == SIO_ACTIVE, "invalid state");

  if (sio_lld_is_rx_empty(siop)) {
    msg = osalThreadSuspendTimeoutS(&siop->sync_rx, timeout);
  }
  else {
    msg = MSG_OK;
  }

  osalSysUnlock();

  return msg;
}

/**
 * @brief   Synchronizes with TX FIFO space availability.
 * @note    The exact behavior depends on low level FIFO settings such
 *          as thresholds, etc.
 * @note    This function can only be called by a single thread at time.
 *
 * @param[in] siop          pointer to an @p SIODriver structure
 * @param[in] timeout       synchronization timeout
 * @return                  The synchronization result.
 * @retval MSG_OK           if there is space in the TX FIFO.
 * @retval MSG_TIMEOUT      if synchronization timed out.
 * @retval MSG_RESET        operation has been stopped while waiting.
 */
msg_t sioSynchronizeTX(SIODriver *siop, sysinterval_t timeout) {
  msg_t msg;

  osalDbgCheck(siop != NULL);

  osalSysLock();

  osalDbgAssert(siop->state == SIO_ACTIVE, "invalid state");

  if (sio_lld_is_tx_full(siop)) {
    msg = osalThreadSuspendTimeoutS(&siop->sync_tx, timeout);
  }
  else {
    msg = MSG_OK;
  }

  osalSysUnlock();

  return msg;
}

/**
 * @brief   Synchronizes with TX completion.
 * @note    This function can only be called by a single thread at time.
 *
 * @param[in] siop          pointer to an @p SIODriver structure
 * @param[in] timeout       synchronization timeout
 * @return                  The synchronization result.
 * @retval MSG_OK           if TX operation finished.
 * @retval MSG_TIMEOUT      if synchronization timed out.
 */
msg_t sioSynchronizeTXEnd(SIODriver *siop, sysinterval_t timeout) {
  msg_t msg;

  osalDbgCheck(siop != NULL);

  osalSysLock();

  osalDbgAssert(siop->state == SIO_ACTIVE, "invalid state");

  if (sio_lld_is_tx_ongoing(siop)) {
    msg = osalThreadSuspendTimeoutS(&siop->sync_txend, timeout);
  }
  else {
    msg = MSG_OK;
  }

  osalSysUnlock();

  return msg;
}
#endif /* SIO_USE_SYNCHRONIZATION == TRUE */

#endif /* HAL_USE_SIO == TRUE */

/** @} */
