/*
    ChibiOS - Copyright (C) 2006..2023 Giovanni Di Sirio

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
 * @file        hal_spi.c
 * @brief       Generated SPI Driver source.
 * @note        This is a generated file, do not edit directly.
 *
 * @addtogroup  HAL_SPI
 * @{
 */

#include "hal.h"

#if (HAL_USE_SPI == TRUE) || defined(__DOXYGEN__)

/*===========================================================================*/
/* Module local definitions.                                                 */
/*===========================================================================*/

/*===========================================================================*/
/* Module local macros.                                                      */
/*===========================================================================*/

/*===========================================================================*/
/* Module exported variables.                                                */
/*===========================================================================*/

/*===========================================================================*/
/* Module local types.                                                       */
/*===========================================================================*/

/*===========================================================================*/
/* Module local variables.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Module local functions.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Module exported functions.                                                */
/*===========================================================================*/

/**
 * @brief       SPI Driver initialization.
 * @note        This function is implicitly invoked by @p halInit(), there is
 *              no need to explicitly initialize the driver.
 *
 * @init
 */
void spiInit(void) {

  spi_lld_init();
}

/*===========================================================================*/
/* Module class "hal_spi_driver_c" methods.                                  */
/*===========================================================================*/

/**
 * @name        Methods implementations of hal_spi_driver_c
 * @{
 */
/**
 * @memberof    hal_spi_driver_c
 * @protected
 *
 * @brief       Implementation of object creation.
 * @note        This function is meant to be used by derived classes.
 *
 * @param[out]    ip            Pointer to a @p hal_spi_driver_c instance to be
 *                              initialized.
 * @param[in]     vmt           VMT pointer for the new object.
 * @return                      A new reference to the object.
 */
void *__spi_objinit_impl(void *ip, const void *vmt) {
  hal_spi_driver_c *self = (hal_spi_driver_c *)ip;

  /* Initialization of the ancestors-defined parts.*/
  __cbdrv_objinit_impl(self, vmt);

  /* Initialization code.*/

  /* Optional, user-defined initializer.*/
#if defined(SPI_DRIVER_EXT_INIT_HOOK)
  SPI_DRIVER_EXT_INIT_HOOK(self);
#endif

  return self;
}

/**
 * @memberof    hal_spi_driver_c
 * @protected
 *
 * @brief       Implementation of object finalization.
 * @note        This function is meant to be used by derived classes.
 *
 * @param[in,out] ip            Pointer to a @p hal_spi_driver_c instance to be
 *                              disposed.
 */
void __spi_dispose_impl(void *ip) {
  hal_spi_driver_c *self = (hal_spi_driver_c *)ip;

  /* No finalization code.*/
  (void)self;

  /* Finalization of the ancestors-defined parts.*/
  __cbdrv_dispose_impl(self);
}

/**
 * @memberof    hal_spi_driver_c
 * @protected
 *
 * @brief       Override of method @p __drv_start().
 *
 * @param[in,out] ip            Pointer to a @p hal_spi_driver_c instance.
 * @return                      The operation status.
 */
msg_t __spi_start_impl(void *ip) {
  hal_spi_driver_c *self = (hal_spi_driver_c *)ip;

  return spi_lld_start(self);
}

/**
 * @memberof    hal_spi_driver_c
 * @protected
 *
 * @brief       Override of method @p __drv_stop().
 *
 * @param[in,out] ip            Pointer to a @p hal_spi_driver_c instance.
 */
void __spi_stop_impl(void *ip) {
  hal_spi_driver_c *self = (hal_spi_driver_c *)ip;

  spi_lld_stop(self);
}

/**
 * @memberof    hal_spi_driver_c
 * @protected
 *
 * @brief       Override of method @p __drv_do_configure().
 *
 * @param[in,out] ip            Pointer to a @p hal_spi_driver_c instance.
 * @param[in]     config        New driver configuration.
 * @return                      The configuration pointer.
 */
const void *__spi_doconf_impl(void *ip, const void *config) {
  hal_spi_driver_c *self = (hal_spi_driver_c *)ip;

  return (const void *)spi_lld_configure(self, (const hal_spi_config_t *)config);
}

/**
 * @memberof    hal_spi_driver_c
 * @protected
 *
 * @brief       Override of method @p drvGetStatusX().
 *
 * @param[in,out] ip            Pointer to a @p hal_spi_driver_c instance.
 */
drv_status_t __spi_gsts_impl(void *ip) {
  hal_spi_driver_c *self = (hal_spi_driver_c *)ip;

  return __cbdrv_gsts_impl(self);
}

/**
 * @memberof    hal_spi_driver_c
 * @protected
 *
 * @brief       Override of method @p drvGetAndClearStatusI().
 *
 * @param[in,out] ip            Pointer to a @p hal_spi_driver_c instance.
 * @param[in]     mask          Flags to be returned and cleared.
 */
drv_status_t __spi_gcsts_impl(void *ip, drv_status_t mask) {
  hal_spi_driver_c *self = (hal_spi_driver_c *)ip;

  return __cbdrv_gcsts_impl(self, mask);
}
/** @} */

/**
 * @brief       VMT structure of SPI driver class.
 * @note        It is public because accessed by the inlined constructor.
 */
const struct hal_spi_driver_vmt __hal_spi_driver_vmt = {
  .dispose                  = __spi_dispose_impl,
  .start                    = __spi_start_impl,
  .stop                     = __spi_stop_impl,
  .doconf                   = __spi_doconf_impl,
  .setcb                    = __cbdrv_setcb_impl,
  .gsts                     = __spi_gsts_impl,
  .gcsts                    = __spi_gcsts_impl
};

/**
 * @name        Regular methods of hal_spi_driver_c
 * @{
 */
/**
 * @memberof    hal_spi_driver_c
 * @public
 *
 * @brief       Ignores data on the SPI bus.
 * @details     This asynchronous function starts the transmission of a series
 *              of idle words on the SPI bus and ignores the received data.
 * @pre         A slave must have been selected using @p spiSelectX().
 * @post        At the end of the operation the callback is invoked, if
 *              enabled.
 *
 * @param[in,out] ip            Pointer to a @p hal_spi_driver_c instance.
 * @param[in]     n             Number of frames to be ignored.
 * @return                      The operation status.
 *
 * @iclass
 */
msg_t spiStartIgnoreI(void *ip, size_t n) {
  hal_spi_driver_c *self = (hal_spi_driver_c *)ip;
  msg_t msg;

  osalDbgCheckClassI();

  osalDbgCheck((self != NULL) && (n > 0U));
#if SPI_SUPPORTS_CIRCULAR
  osalDbgCheck(((__spi_getfield(self, mode) & SPI_MODE_CIRCULAR) == 0U) ||
               ((n & 1U) == 0U));
#endif

  osalDbgAssert(self->state == HAL_DRV_STATE_READY, "not ready");

  self->state = HAL_DRV_STATE_ACTIVE;
  msg = spi_lld_ignore(self, n);

#if SPI_USE_ASSERT_ON_ERROR == TRUE
  osalDbgAssert(msg == HAL_RET_SUCCESS, "function failed");
#endif

  return msg;
}

/**
 * @memberof    hal_spi_driver_c
 * @public
 *
 * @brief       Ignores data on the SPI bus.
 * @details     This asynchronous function starts the transmission of a series
 *              of idle words on the SPI bus and ignores the received data.
 * @pre         A slave must have been selected using @p spiSelectX().
 * @post        At the end of the operation the callback is invoked, if
 *              enabled.
 *
 * @param[in,out] ip            Pointer to a @p hal_spi_driver_c instance.
 * @param[in]     n             Number of frames to be ignored.
 * @return                      The operation status.
 *
 * @api
 */
msg_t spiStartIgnore(void *ip, size_t n) {
  hal_spi_driver_c *self = (hal_spi_driver_c *)ip;
  msg_t msg;

  osalSysLock();
  msg = spiStartIgnoreI(self, n);
  osalSysUnlock();

  return msg;
}

/**
 * @memberof    hal_spi_driver_c
 * @public
 *
 * @brief       Exchanges data on the SPI bus.
 * @details     This asynchronous function starts a simultaneous
 *              transmit/receive operation.
 * @pre         A slave must have been selected using @p spiSelectX().
 * @post        At the end of the operation the callback is invoked, if
 *              enabled.
 * @note        Buffers are organized as uint8_t arrays for frame sizes below
 *              or equal to 8 bits else uint16_t is used.
 *
 * @param[in,out] ip            Pointer to a @p hal_spi_driver_c instance.
 * @param[in]     n             Number of frames to be exchanged.
 * @param[in]     txbuf         Pointer to the transmit buffer.
 * @param[out]    rxbuf         Pointer to the receive buffer.
 * @return                      The operation status.
 *
 * @iclass
 */
msg_t spiStartExchangeI(void *ip, size_t n, const void *txbuf, void *rxbuf) {
  hal_spi_driver_c *self = (hal_spi_driver_c *)ip;
  msg_t msg;

  osalDbgCheckClassI();

  osalDbgCheck((self != NULL) && (n > 0U) &&
               (rxbuf != NULL) && (txbuf != NULL));
#if SPI_SUPPORTS_CIRCULAR
  osalDbgCheck(((__spi_getfield(self, mode) & SPI_MODE_CIRCULAR) == 0U) ||
               ((n & 1U) == 0U));
#endif

  osalDbgAssert(self->state == HAL_DRV_STATE_READY, "not ready");

  self->state = HAL_DRV_STATE_ACTIVE;
  msg = spi_lld_exchange(self, n, txbuf, rxbuf);

#if SPI_USE_ASSERT_ON_ERROR == TRUE
  osalDbgAssert(msg == HAL_RET_SUCCESS, "function failed");
#endif

  return msg;
}

/**
 * @memberof    hal_spi_driver_c
 * @public
 *
 * @brief       Exchanges data on the SPI bus.
 * @details     This asynchronous function starts a simultaneous
 *              transmit/receive operation.
 * @pre         A slave must have been selected using @p spiSelectX().
 * @post        At the end of the operation the callback is invoked, if
 *              enabled.
 * @note        Buffers are organized as uint8_t arrays for frame sizes below
 *              or equal to 8 bits else uint16_t is used.
 *
 * @param[in,out] ip            Pointer to a @p hal_spi_driver_c instance.
 * @param[in]     n             Number of frames to be exchanged.
 * @param[in]     txbuf         Pointer to the transmit buffer.
 * @param[out]    rxbuf         Pointer to the receive buffer.
 * @return                      The operation status.
 *
 * @api
 */
msg_t spiStartExchange(void *ip, size_t n, const void *txbuf, void *rxbuf) {
  hal_spi_driver_c *self = (hal_spi_driver_c *)ip;
  msg_t msg;

  osalSysLock();
  msg = spiStartExchangeI(self, n, txbuf, rxbuf);
  osalSysUnlock();

  return msg;
}

/**
 * @memberof    hal_spi_driver_c
 * @public
 *
 * @brief       Sends data over the SPI bus.
 * @details     This asynchronous function starts a transmit operation.
 * @pre         A slave must have been selected using @p spiSelectX().
 * @post        At the end of the operation the callback is invoked, if
 *              enabled.
 * @note        Buffers are organized as uint8_t arrays for frame sizes below
 *              or equal to 8 bits else uint16_t is used.
 *
 * @param[in,out] ip            Pointer to a @p hal_spi_driver_c instance.
 * @param[in]     n             Number of frames to be exchanged.
 * @param[in]     txbuf         Pointer to the transmit buffer.
 * @return                      The operation status.
 *
 * @iclass
 */
msg_t spiStartSendI(void *ip, size_t n, const void *txbuf) {
  hal_spi_driver_c *self = (hal_spi_driver_c *)ip;
  msg_t msg;

  osalDbgCheckClassI();

  osalDbgCheck((self != NULL) && (n > 0U) && (txbuf != NULL));
#if SPI_SUPPORTS_CIRCULAR
  osalDbgCheck(((__spi_getfield(self, mode) & SPI_MODE_CIRCULAR) == 0U) ||
               ((n & 1U) == 0U));
#endif

  osalDbgAssert(self->state == HAL_DRV_STATE_READY, "not ready");

  self->state = HAL_DRV_STATE_ACTIVE;
  msg = spi_lld_send(self, n, txbuf);

#if SPI_USE_ASSERT_ON_ERROR == TRUE
  osalDbgAssert(msg == HAL_RET_SUCCESS, "function failed");
#endif

  return msg;
}

/**
 * @memberof    hal_spi_driver_c
 * @public
 *
 * @brief       Sends data over the SPI bus.
 * @details     This asynchronous function starts a transmit operation.
 * @pre         A slave must have been selected using @p spiSelectX().
 * @post        At the end of the operation the callback is invoked, if
 *              enabled.
 * @note        Buffers are organized as uint8_t arrays for frame sizes below
 *              or equal to 8 bits else uint16_t is used.
 *
 * @param[in,out] ip            Pointer to a @p hal_spi_driver_c instance.
 * @param[in]     n             Number of frames to be exchanged.
 * @param[in]     txbuf         Pointer to the transmit buffer.
 * @return                      The operation status.
 *
 * @api
 */
msg_t spiStartSend(void *ip, size_t n, const void *txbuf) {
  hal_spi_driver_c *self = (hal_spi_driver_c *)ip;
  msg_t msg;

  osalSysLock();
  msg = spiStartSendI(self, n, txbuf);
  osalSysUnlock();

  return msg;
}

/**
 * @memberof    hal_spi_driver_c
 * @public
 *
 * @brief       Receives data from the SPI bus.
 * @details     This asynchronous function starts a receive operation.
 * @pre         A slave must have been selected using @p spiSelectX().
 * @post        At the end of the operation the callback is invoked, if
 *              enabled.
 * @note        Buffers are organized as uint8_t arrays for frame sizes below
 *              or equal to 8 bits else uint16_t is used.
 *
 * @param[in,out] ip            Pointer to a @p hal_spi_driver_c instance.
 * @param[in]     n             Number of frames to be exchanged.
 * @param[out]    rxbuf         Pointer to the receive buffer.
 * @return                      The operation status.
 *
 * @iclass
 */
msg_t spiStartReceiveI(void *ip, size_t n, void *rxbuf) {
  hal_spi_driver_c *self = (hal_spi_driver_c *)ip;
  msg_t msg;

  osalDbgCheckClassI();

  osalDbgCheck((self != NULL) && (n > 0U) && (rxbuf != NULL));
#if SPI_SUPPORTS_CIRCULAR
  osalDbgCheck(((__spi_getfield(self, mode) & SPI_MODE_CIRCULAR) == 0U) ||
               ((n & 1U) == 0U));
#endif

  osalDbgAssert(self->state == HAL_DRV_STATE_READY, "not ready");

  self->state = HAL_DRV_STATE_ACTIVE;
  msg = spi_lld_receive(self, n, rxbuf);

#if SPI_USE_ASSERT_ON_ERROR == TRUE
  osalDbgAssert(msg == HAL_RET_SUCCESS, "function failed");
#endif

  return msg;
}

/**
 * @memberof    hal_spi_driver_c
 * @public
 *
 * @brief       Receives data from the SPI bus.
 * @details     This asynchronous function starts a receive operation.
 * @pre         A slave must have been selected using @p spiSelectX().
 * @post        At the end of the operation the callback is invoked, if
 *              enabled.
 * @note        Buffers are organized as uint8_t arrays for frame sizes below
 *              or equal to 8 bits else uint16_t is used.
 *
 * @param[in,out] ip            Pointer to a @p hal_spi_driver_c instance.
 * @param[in]     n             Number of frames to be exchanged.
 * @param[out]    rxbuf         Pointer to the receive buffer.
 * @return                      The operation status.
 *
 * @api
 */
msg_t spiStartReceive(void *ip, size_t n, void *rxbuf) {
  hal_spi_driver_c *self = (hal_spi_driver_c *)ip;
  msg_t msg;

  osalSysLock();
  msg = spiStartReceiveI(self, n, rxbuf);
  osalSysUnlock();

  return msg;
}

/**
 * @memberof    hal_spi_driver_c
 * @public
 *
 * @brief       Stops the ongoing SPI operation.
 *
 * @param[in,out] ip            Pointer to a @p hal_spi_driver_c instance.
 * @param[out]    np            Pointer to the counter of frames not yet
 *                              transferred or @p NULL.
 * @return                      The operation status.
 *
 * @iclass
 */
msg_t spiStopTransferI(void *ip, size_t *np) {
  hal_spi_driver_c *self = (hal_spi_driver_c *)ip;
  msg_t msg;

  osalDbgCheckClassI();

  osalDbgCheck(self != NULL);

  osalDbgAssert((self->state == HAL_DRV_STATE_READY) ||
                (self->state == HAL_DRV_STATE_ACTIVE) ||
                (self->state == HAL_DRV_STATE_COMPLETE),
                "invalid state");

  if ((self->state == HAL_DRV_STATE_ACTIVE) ||
      (self->state == HAL_DRV_STATE_COMPLETE)) {

    /* Stopping transfer at low level.*/
    msg = spi_lld_stop_transfer(self, np);
    self->state = HAL_DRV_STATE_READY;

#if SPI_USE_SYNCHRONIZATION == TRUE
    osalThreadResumeI(&self->sync_transfer, MSG_RESET);
#endif
  }
  else {
    msg = HAL_RET_SUCCESS;
  }

  return msg;
}

/**
 * @memberof    hal_spi_driver_c
 * @public
 *
 * @brief       Stops the ongoing SPI operation.
 *
 * @param[in,out] ip            Pointer to a @p hal_spi_driver_c instance.
 * @param[out]    np            Pointer to the counter of frames not yet
 *                              transferred or @p NULL.
 * @return                      The operation status.
 *
 * @api
 */
msg_t spiStopTransfer(void *ip, size_t *np) {
  hal_spi_driver_c *self = (hal_spi_driver_c *)ip;
  msg_t msg;

  osalSysLock();

  msg = spiStopTransferI(self, np);
  osalOsRescheduleS();

  osalSysUnlock();

  return msg;
}

#if (SPI_USE_SYNCHRONIZATION == TRUE) || defined (__DOXYGEN__)
/**
 * @memberof    hal_spi_driver_c
 * @public
 *
 * @note        This function can only be called by a single thread at time.
 *
 * @param[in,out] ip            Pointer to a @p hal_spi_driver_c instance.
 * @param[in]     timeout       Synchronization timeout.
 * @return                      The synchronization result.
 * @retval MSG_OK               If operation completed without errors.
 * @retval MSG_TIMEOUT          If synchronization timed out.
 * @retval MSG_RESET            If the transfer has been stopped.
 *
 * @sclass
 */
msg_t spiSynchronizeS(void *ip, sysinterval_t timeout) {
  hal_spi_driver_c *self = (hal_spi_driver_c *)ip;
  msg_t msg;

  osalDbgCheck(self != NULL);
  osalDbgAssert((self->state == HAL_DRV_STATE_ACTIVE) ||
                (self->state == HAL_DRV_STATE_READY),
                "invalid state");

  if (self->state == HAL_DRV_STATE_ACTIVE) {
    msg = osalThreadSuspendTimeoutS(&self->sync_transfer, timeout);
  }
  else {
    msg = MSG_OK;
  }

  return msg;
}

/**
 * @memberof    hal_spi_driver_c
 * @public
 *
 * @note        This function can only be called by a single thread at time.
 *
 * @param[in,out] ip            Pointer to a @p hal_spi_driver_c instance.
 * @param[in]     timeout       Synchronization timeout.
 * @return                      The synchronization result.
 * @retval MSG_OK               If operation completed without errors.
 * @retval MSG_TIMEOUT          If synchronization timed out.
 * @retval MSG_RESET            If the transfer has been stopped.
 *
 * @api
 */
msg_t spiSynchronize(void *ip, sysinterval_t timeout) {
  hal_spi_driver_c *self = (hal_spi_driver_c *)ip;
  msg_t msg;

  osalSysLock();
  msg = spiSynchronizeS(self, timeout);
  osalSysUnlock();

  return msg;
}

/**
 * @memberof    hal_spi_driver_c
 * @public
 *
 * @brief       Ignores data on the SPI bus.
 * @details     This synchronous function performs the transmission of a series
 *              of idle words on the SPI bus and ignores the received data.
 * @pre         A slave must have been selected using @p spiSelectX().
 *
 * @param[in,out] ip            Pointer to a @p hal_spi_driver_c instance.
 * @param[in]     n             Number of frames to be ignored.
 * @return                      The operation status.
 * @retval MSG_OK               If operation completed without errors.
 * @retval MSG_TIMEOUT          If synchronization timed out.
 * @retval MSG_RESET            If the transfer has been stopped.
 *
 * @api
 */
msg_t spiIgnore(void *ip, size_t n) {
  hal_spi_driver_c *self = (hal_spi_driver_c *)ip;
  msg_t msg;

  osalSysLock();

  msg = spiStartIgnoreI(self, n);
  if (msg == MSG_OK) {
    msg = spiSynchronizeS(self, TIME_INFINITE);
  }

  osalSysUnlock();

  return msg;
}

/**
 * @memberof    hal_spi_driver_c
 * @public
 *
 * @brief       Exchanges data on the SPI bus.
 * @details     This synchronous function performs a simultaneous
 *              transmit/receive operation.
 * @pre         A slave must have been selected using @p spiSelectX().
 * @note        Buffers are organized as uint8_t arrays for frame sizes below
 *              or equal to 8 bits else uint16_t is used.
 *
 * @param[in,out] ip            Pointer to a @p hal_spi_driver_c instance.
 * @param[in]     n             Number of frames to be exchanged.
 * @param[in]     txbuf         Pointer to the transmit buffer.
 * @param[out]    rxbuf         Pointer to the receive buffer.
 * @return                      The operation status.
 * @retval MSG_OK               If operation completed without errors.
 * @retval MSG_TIMEOUT          If synchronization timed out.
 * @retval MSG_RESET            If the transfer has been stopped.
 *
 * @api
 */
msg_t spiExchange(void *ip, size_t n, const void *txbuf, void *rxbuf) {
  hal_spi_driver_c *self = (hal_spi_driver_c *)ip;
  msg_t msg;

  osalSysLock();

  msg = spiStartExchangeI(self, n, txbuf, rxbuf);
  if (msg == MSG_OK) {
    msg = spiSynchronizeS(self, TIME_INFINITE);
  }

  osalSysUnlock();

  return msg;
}

/**
 * @memberof    hal_spi_driver_c
 * @public
 *
 * @brief       Sends data over the SPI bus.
 * @details     This synchronous function performs a transmit operation.
 * @pre         A slave must have been selected using @p spiSelectX().
 * @note        Buffers are organized as uint8_t arrays for frame sizes below
 *              or equal to 8 bits else uint16_t is used.
 *
 * @param[in,out] ip            Pointer to a @p hal_spi_driver_c instance.
 * @param[in]     n             Number of frames to be exchanged.
 * @param[in]     txbuf         Pointer to the transmit buffer.
 * @return                      The operation status.
 * @retval MSG_OK               If operation completed without errors.
 * @retval MSG_TIMEOUT          If synchronization timed out.
 * @retval MSG_RESET            If the transfer has been stopped.
 *
 * @api
 */
msg_t spiSend(void *ip, size_t n, const void *txbuf) {
  hal_spi_driver_c *self = (hal_spi_driver_c *)ip;
  msg_t msg;

  msg = spiStartSendI(self, n, txbuf);
  if (msg == MSG_OK) {
    msg = spiSynchronizeS(self, TIME_INFINITE);
  }

  osalSysUnlock();

  return msg;
}

/**
 * @memberof    hal_spi_driver_c
 * @public
 *
 * @brief       Receives data from the SPI bus.
 * @details     This synchronous function performs a receive operation.
 * @pre         A slave must have been selected using @p spiSelectX().
 * @note        Buffers are organized as uint8_t arrays for frame sizes below
 *              or equal to 8 bits else uint16_t is used.
 *
 * @param[in,out] ip            Pointer to a @p hal_spi_driver_c instance.
 * @param[in]     n             Number of frames to be exchanged.
 * @param[out]    rxbuf         Pointer to the receive buffer.
 * @return                      The operation status.
 * @retval MSG_OK               If operation completed without errors.
 * @retval MSG_TIMEOUT          If synchronization timed out.
 * @retval MSG_RESET            If the transfer has been stopped.
 *
 * @api
 */
msg_t spiReceive(void *ip, size_t n, void *rxbuf) {
  hal_spi_driver_c *self = (hal_spi_driver_c *)ip;
  msg_t msg;

  msg = spiStartReceiveI(self, n, rxbuf);
  if (msg == MSG_OK) {
    msg = spiSynchronizeS(self, TIME_INFINITE);
  }

  osalSysUnlock();

  return msg;
}
#endif /* SPI_USE_SYNCHRONIZATION == TRUE */
/** @} */

#endif /* HAL_USE_SPI == TRUE */

/** @} */
