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
 * @file        hal_buffered_serial.c
 * @brief       Generated Buffered Serial Driver source.
 * @note        This is a generated file, do not edit directly.
 *
 * @addtogroup  HAL_BUFFERED_SERIAL
 * @{
 */

#include "hal.h"

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

/*===========================================================================*/
/* Module class "hal_buffered_serial_c" methods.                             */
/*===========================================================================*/

/**
 * @name        Interfaces implementation of hal_buffered_serial_c
 * @{
 */
/**
 * @memberof    hal_buffered_serial_c
 * @private
 *
 * @brief       Implementation of interface method @p stmWrite().
 *
 * @param[in,out] ip            Pointer to the @p asynchronous_channel_i class
 *                              interface.
 * @param[in]     bp            Pointer to the data buffer.
 * @param[in]     n             The maximum amount of data to be transferred.
 * @return                      The number of bytes transferred. The returned
 *                              value can be less than the specified number of
 *                              bytes if an end-of-file condition has been met.
 */
static size_t __bs_chn_write_impl(void *ip, const uint8_t *bp, size_t n) {
  hal_buffered_serial_c *self = oopIfGetOwner(hal_buffered_serial_c, ip);

  return oqWriteTimeout(&self->oqueue, bp, n, TIME_INFINITE);
}

/**
 * @memberof    hal_buffered_serial_c
 * @private
 *
 * @brief       Implementation of interface method @p stmRead().
 *
 * @param[in,out] ip            Pointer to the @p asynchronous_channel_i class
 *                              interface.
 * @param[out]    bp            Pointer to the data buffer.
 * @param[in]     n             The maximum amount of data to be transferred.
 * @return                      The number of bytes transferred. The returned
 *                              value can be less than the specified number of
 *                              bytes if an end-of-file condition has been met.
 */
static size_t __bs_chn_read_impl(void *ip, uint8_t *bp, size_t n) {
  hal_buffered_serial_c *self = oopIfGetOwner(hal_buffered_serial_c, ip);

  return iqReadTimeout(&self->iqueue, bp, n, TIME_INFINITE);
}

/**
 * @memberof    hal_buffered_serial_c
 * @private
 *
 * @brief       Implementation of interface method @p stmPut().
 *
 * @param[in,out] ip            Pointer to the @p asynchronous_channel_i class
 *                              interface.
 * @param[in]     b             The byte value to be written to the stream.
 * @return                      The operation status.
 */
static msg_t __bs_chn_put_impl(void *ip, uint8_t b) {
  hal_buffered_serial_c *self = oopIfGetOwner(hal_buffered_serial_c, ip);

  return oqPutTimeout(&self->oqueue, b, TIME_INFINITE);
}

/**
 * @memberof    hal_buffered_serial_c
 * @private
 *
 * @brief       Implementation of interface method @p stmGet().
 *
 * @param[in,out] ip            Pointer to the @p asynchronous_channel_i class
 *                              interface.
 * @return                      A byte value from the stream.
 */
static msg_t __bs_chn_get_impl(void *ip) {
  hal_buffered_serial_c *self = oopIfGetOwner(hal_buffered_serial_c, ip);

  return iqGetTimeout(&self->iqueue, TIME_INFINITE);
}

/**
 * @memberof    hal_buffered_serial_c
 * @private
 *
 * @brief       Implementation of interface method @p chnWriteTimeout().
 *
 * @param[in,out] ip            Pointer to the @p asynchronous_channel_i class
 *                              interface.
 * @param[in]     bp            Pointer to the data buffer.
 * @param[in]     n             The maximum amount of data to be transferred.
 * @param[in]     timeout       The number of ticks before the operation
 *                              timeouts, the following special values are
 *                              allowed:
 *                              - @a TIME_IMMEDIATE immediate timeout.
 *                              - @a TIME_INFINITE no timeout.
 *                              .
 * @return                      The number of bytes transferred.
 */
static size_t __bs_chn_writet_impl(void *ip, const uint8_t *bp, size_t n,
                                   sysinterval_t timeout) {
  hal_buffered_serial_c *self = oopIfGetOwner(hal_buffered_serial_c, ip);

  return oqWriteTimeout(&self->oqueue, bp, n, timeout);
}

/**
 * @memberof    hal_buffered_serial_c
 * @private
 *
 * @brief       Implementation of interface method @p chnReadTimeout().
 *
 * @param[in,out] ip            Pointer to the @p asynchronous_channel_i class
 *                              interface.
 * @param[in]     bp            Pointer to the data buffer.
 * @param[in]     n             The maximum amount of data to be transferred.
 * @param[in]     timeout       The number of ticks before the operation
 *                              timeouts, the following special values are
 *                              allowed:
 *                              - @a TIME_IMMEDIATE immediate timeout.
 *                              - @a TIME_INFINITE no timeout.
 *                              .
 * @return                      The number of bytes transferred.
 */
static size_t __bs_chn_readt_impl(void *ip, uint8_t *bp, size_t n,
                                  sysinterval_t timeout) {
  hal_buffered_serial_c *self = oopIfGetOwner(hal_buffered_serial_c, ip);

  return iqReadTimeout(&self->iqueue, bp, n, timeout);
}

/**
 * @memberof    hal_buffered_serial_c
 * @private
 *
 * @brief       Implementation of interface method @p chnPutTimeout().
 *
 * @param[in,out] ip            Pointer to the @p asynchronous_channel_i class
 *                              interface.
 * @param[in]     b             The byte value to be written to the channel.
 * @param[in]     timeout       The number of ticks before the operation
 *                              timeouts, the following special values are
 *                              allowed:
 *                              - @a TIME_IMMEDIATE immediate timeout.
 *                              - @a TIME_INFINITE no timeout.
 *                              .
 * @return                      The operation status.
 */
static msg_t __bs_chn_putt_impl(void *ip, uint8_t b, sysinterval_t timeout) {
  hal_buffered_serial_c *self = oopIfGetOwner(hal_buffered_serial_c, ip);

  return oqPutTimeout(&self->oqueue, b, timeout);
}

/**
 * @memberof    hal_buffered_serial_c
 * @private
 *
 * @brief       Implementation of interface method @p chnGetTimeout().
 *
 * @param[in,out] ip            Pointer to the @p asynchronous_channel_i class
 *                              interface.
 * @param[in]     timeout       The number of ticks before the operation
 *                              timeouts, the following special values are
 *                              allowed:
 *                              - @a TIME_IMMEDIATE immediate timeout.
 *                              - @a TIME_INFINITE no timeout.
 *                              .
 * @return                      A byte value from the channel.
 */
static msg_t __bs_chn_gett_impl(void *ip, sysinterval_t timeout) {
  hal_buffered_serial_c *self = oopIfGetOwner(hal_buffered_serial_c, ip);

  return iqGetTimeout(&self->iqueue, timeout);
}

/**
 * @memberof    hal_buffered_serial_c
 * @private
 *
 * @brief       Implementation of interface method @p chnGetAndClearFlags().
 *
 * @param[in,out] ip            Pointer to the @p asynchronous_channel_i class
 *                              interface.
 * @param[in]     mask          Mask of flags to be returned and cleared.
 * @return                      The cleared event flags.
 */
static chnflags_t __bs_chn_getclr_impl(void *ip, chnflags_t mask) {
  hal_buffered_serial_c *self = oopIfGetOwner(hal_buffered_serial_c, ip);

  (void)self;
  (void)mask;

  return 0;
}

/**
 * @memberof    hal_buffered_serial_c
 * @private
 *
 * @brief       Implementation of interface method @p chnControl().
 *
 * @param[in,out] ip            Pointer to the @p asynchronous_channel_i class
 *                              interface.
 * @param[in]     operation     Control operation code
 * @param[in,out] arg           Operation argument.
 * @return                      The operation status.
 */
static msg_t __bs_chn_ctl_impl(void *ip, unsigned int operation, void *arg) {
  hal_buffered_serial_c *self = oopIfGetOwner(hal_buffered_serial_c, ip);

  (void)self;
  (void)operation;
  (void)arg;

  return 0;
}
/** @} */

/**
 * @name        Methods implementations of hal_buffered_serial_c
 * @{
 */
/**
 * @memberof    hal_buffered_serial_c
 * @protected
 *
 * @brief       Implementation of object creation.
 * @note        This function is meant to be used by derived classes.
 *
 * @param[out]    ip            Pointer to a @p hal_buffered_serial_c instance
 *                              to be initialized.
 * @param[in]     vmt           VMT pointer for the new object.
 * @param[in]     ib            Pointer to the input buffer
 * @param[in]     ibsize        Size of the input buffer
 * @param[in]     inotify       Pointer to a callback function that is invoked
 *                              when some data is read from the input queue.
 *                              The value can be @p NULL
 * @param[in]     iarg          Parameter for the input notification callback
 * @param[in]     ob            Pointer to the output buffer
 * @param[in]     obsize        Size of the output buffer
 * @param[in]     onotify       Pointer to a callback function that is invoked
 *                              when some data is written to the output queue.
 *                              The value can be @p NULL
 * @param[in]     oarg          Parameter for the output notification callback
 * @return                      A new reference to the object.
 */
void *__bs_objinit_impl(void *ip, const void *vmt, uint8_t *ib, size_t ibsize,
                        qnotify_t inotify, void *iarg, uint8_t *ob,
                        size_t obsize, qnotify_t onotify, void *oarg) {
  hal_buffered_serial_c *self = (hal_buffered_serial_c *)ip;

  /* Initialization of the ancestors-defined parts.*/
  __drv_objinit_impl(self, vmt);

  /* Initialization of interface asynchronous_channel_i.*/
  {
    static const struct asynchronous_channel_vmt bs_chn_vmt = {
      .instance_offset      = offsetof(hal_buffered_serial_c, chn),
      .write                = __bs_chn_write_impl,
      .read                 = __bs_chn_read_impl,
      .put                  = __bs_chn_put_impl,
      .get                  = __bs_chn_get_impl,
      .writet               = __bs_chn_writet_impl,
      .readt                = __bs_chn_readt_impl,
      .putt                 = __bs_chn_putt_impl,
      .gett                 = __bs_chn_gett_impl,
      .getclr               = __bs_chn_getclr_impl,
      .ctl                  = __bs_chn_ctl_impl
    };
    oopIfObjectInit(&self->chn, &bs_chn_vmt);
  }

  /* Initialization code.*/

  osalEventObjectInit(&self->event);
  iqObjectInit(&self->iqueue, ib, ibsize, inotify, iarg);
  oqObjectInit(&self->oqueue, ob, obsize, onotify, oarg);

  return self;
}

/**
 * @memberof    hal_buffered_serial_c
 * @protected
 *
 * @brief       Implementation of object finalization.
 * @note        This function is meant to be used by derived classes.
 *
 * @param[in,out] ip            Pointer to a @p hal_buffered_serial_c instance
 *                              to be disposed.
 */
void __bs_dispose_impl(void *ip) {
  hal_buffered_serial_c *self = (hal_buffered_serial_c *)ip;

  /* No finalization code.*/
  (void)self;

  /* Finalization of the ancestors-defined parts.*/
  __drv_dispose_impl(self);
}
/** @} */

/**
 * @name        Regular methods of hal_buffered_serial_c
 * @{
 */
/**
 * @memberof    hal_buffered_serial_c
 * @public
 *
 * @brief       Handles incoming data.
 * @details     This function must be called from the input interrupt service
 *              routine in order to enqueue incoming data and generate the
 *              related events.
 * @note        The incoming data event is only generated when the input queue
 *              becomes non-empty.
 *
 * @param[in,out] ip            Pointer to a @p hal_buffered_serial_c instance.
 * @param[in]     b             The byte to be written to the driver's Input
 *                              Queue
 */
void bsIncomingDataI(void *ip, uint8_t b) {
  hal_buffered_serial_c *self = (hal_buffered_serial_c *)ip;

  osalDbgCheckClassI();
  osalDbgCheck(self != NULL);

  if (iqIsEmptyI(&self->iqueue)) {
    bsAddFlagsI(self, CHN_FL_RX_NOTEMPTY);
  }

  if (iqPutI(&self->iqueue, b) < MSG_OK) {
    bsAddFlagsI(self, CHN_FL_BUFFER_FULL_ERR);
  }
}

/**
 * @memberof    hal_buffered_serial_c
 * @public
 *
 * @brief       Handles outgoing data.
 * @details     Must be called from the output interrupt service routine in
 *              order to get the next byte to be transmitted.
 *
 * @param[in,out] ip            Pointer to a @p hal_buffered_serial_c instance.
 * @return                      The byte value read from the driver's output
 *                              queue.
 * @retval MSG_TIMEOUT          If the queue is empty.
 */
msg_t bsRequestDataI(void *ip) {
  hal_buffered_serial_c *self = (hal_buffered_serial_c *)ip;
  msg_t  b;

  osalDbgCheckClassI();
  osalDbgCheck(self != NULL);

  b = oqGetI(&self->oqueue);
  if (b < MSG_OK) {
    /* Note, this event is only added when the buffer becomes fully empty in
       order to avoid continuous reporting.*/
    bsAddFlagsI(self, CHN_FL_TX_NOTFULL);
  }

  return b;
}
/** @} */

/** @} */
