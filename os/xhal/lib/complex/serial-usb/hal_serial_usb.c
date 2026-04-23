/*
    ChibiOS - Copyright (C) 2006-2026 Giovanni Di Sirio.

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
 * @file        hal_serial_usb.c
 * @brief       Generated Serial over USB Driver source.
 * @note        This is a generated file, do not edit directly.
 *
 * @addtogroup  HAL_SERIAL_USB
 * @{
 */

#include "hal_serial_usb.h"

#if (!defined(SERIAL_USB_USE_MODULE) || (SERIAL_USB_USE_MODULE == TRUE)) || defined(__DOXYGEN__)

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

static void sdu_add_flags_i(SerialUSBDriver *self, chnflags_t flags) {
  self->flags |= flags;
  osalEventBroadcastFlagsI(&self->event, flags);
}

static void sdu_resume_tx_waiter_i(SerialUSBDriver *self, msg_t msg) {
  osalThreadResumeI(&self->txsync, msg);
}

static bool sdu_is_usb_active_x(SerialUSBDriver *self) {
  const SerialUSBConfig *cfg = (const SerialUSBConfig *)self->config;

  return (cfg != NULL) &&
         (usbGetDriverStateX(cfg->usbp) == USB_ACTIVE) &&
         (self->state == SDU_READY) &&
         self->connected;
}

static bool sdu_is_transmitting_i(SerialUSBDriver *self) {
  const SerialUSBConfig *cfg = (const SerialUSBConfig *)self->config;
  uint16_t mask;

  if (cfg == NULL) {
    return false;
  }

  mask = (uint16_t)((unsigned)1U << (unsigned)cfg->bulk_in);

  return (bool)((cfg->usbp->transmitting & mask) != 0U);
}

static bool sdu_is_receiving_i(SerialUSBDriver *self) {
  const SerialUSBConfig *cfg = (const SerialUSBConfig *)self->config;
  uint16_t mask;

  if (cfg == NULL) {
    return false;
  }

  mask = (uint16_t)((unsigned)1U << (unsigned)cfg->bulk_out);

  return (bool)((cfg->usbp->receiving & mask) != 0U);
}

static size_t sdu_tx_collect_i(SerialUSBDriver *self, size_t n) {
  size_t i;

  for (i = 0U; i < n; i++) {
    msg_t msg = oqGetI(&self->oqueue);

    osalDbgAssert(msg >= MSG_OK, "queue unexpectedly empty");
    self->txbuf[i] = (uint8_t)msg;
  }

  return n;
}

static bool sdu_start_receive_i(SerialUSBDriver *self) {
  const SerialUSBConfig *cfg = (const SerialUSBConfig *)self->config;
  size_t n;

  if ((cfg == NULL) || !sdu_is_usb_active_x(self) || sdu_is_receiving_i(self)) {
    return true;
  }

#if (SERIAL_USB_RX_PACKET_MODE == TRUE)
  n = (size_t)cfg->bulk_out_maxsize;
  osalDbgAssert(n <= (size_t)SERIAL_USB_BUFFERS_SIZE,
                "SERIAL_USB_BUFFERS_SIZE too small");
#else
  n = (size_t)SERIAL_USB_BUFFERS_SIZE;
#endif

  if (iqGetEmptyI(&self->iqueue) < n) {
    return true;
  }

  usbStartReceiveI(cfg->usbp, cfg->bulk_out, self->rxbuf, n);

  return false;
}

static bool sdu_try_transmit_i(SerialUSBDriver *self, bool flush) {
  const SerialUSBConfig *cfg = (const SerialUSBConfig *)self->config;
  size_t n;

  if ((cfg == NULL) || !sdu_is_usb_active_x(self) || sdu_is_transmitting_i(self)) {
    return false;
  }

  n = oqGetFullI(&self->oqueue);
  if (n == 0U) {
    return false;
  }

  if (!flush && (n < (size_t)SERIAL_USB_BUFFERS_SIZE)) {
    return false;
  }

  if (n > (size_t)SERIAL_USB_BUFFERS_SIZE) {
    n = (size_t)SERIAL_USB_BUFFERS_SIZE;
  }

  n = sdu_tx_collect_i(self, n);
  self->last_txsize = n;
  usbStartTransmitI(cfg->usbp, cfg->bulk_in, self->txbuf, n);

  return true;
}

static void sdu_data_transmitted_i(SerialUSBDriver *self,
                                   hal_usb_driver_c *usbp,
                                   usbep_t ep) {
  const SerialUSBConfig *cfg = (const SerialUSBConfig *)self->config;

  sdu_add_flags_i(self, CHN_FL_TX_NOTFULL);
  if (!sdu_try_transmit_i(self, false)) {
#if (SERIAL_USB_SEND_ZLP == TRUE)
    if ((cfg != NULL) &&
        (self->last_txsize > 0U) &&
        ((self->last_txsize % (size_t)cfg->bulk_in_maxsize) == 0U)) {
      self->last_txsize = 0U;
      usbStartTransmitI(usbp, ep, usbp->setup, 0U);
      return;
    }
#endif
    sdu_add_flags_i(self, CHN_FL_TX_END);
    sdu_resume_tx_waiter_i(self, MSG_OK);
  }
}

static void sdu_data_received_i(SerialUSBDriver *self,
                                hal_usb_driver_c *usbp,
                                usbep_t ep) {
  size_t i;
  size_t n;

  n = usbGetReceiveTransactionSizeX(usbp, ep);
  for (i = 0U; i < n; i++) {
    if (iqPutI(&self->iqueue, self->rxbuf[i]) != MSG_OK) {
      sdu_add_flags_i(self, CHN_FL_BUFFER_FULL_ERR);
      break;
    }
  }
  if (n > 0U) {
    sdu_add_flags_i(self, CHN_FL_RX_NOTEMPTY);
  }
  (void)sdu_start_receive_i(self);
}

static void sdu_reset_queues_i(SerialUSBDriver *self, msg_t msg) {
  self->connected = false;
  iqResetI(&self->iqueue);
  oqResetI(&self->oqueue);
  sdu_resume_tx_waiter_i(self, msg);
}

static void sdu_inotify(io_queue_t *qp) {
  SerialUSBDriver *self = (SerialUSBDriver *)qGetLink(qp);

  if (self != NULL) {
    (void)sdu_start_receive_i(self);
  }
}

static void sdu_onotify(io_queue_t *qp) {
  SerialUSBDriver *self = (SerialUSBDriver *)qGetLink(qp);

  if (self != NULL) {
    (void)sdu_try_transmit_i(self, false);
  }
}

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
 * @brief       Returns the current CDC line coding.
 *
 * @param[in,out] ip            Pointer to a @p SerialUSBDriver instance.
 * @return                      Pointer to the current CDC line coding.
 *
 * @xclass
 */
const sdu_linecoding_t *sduGetLineCodingX(void *ip) {
  SerialUSBDriver *self = (SerialUSBDriver *)ip;

  osalDbgCheck(self != NULL);

  return (const sdu_linecoding_t *)usbCdcServiceGetLineCodingX((void *)self);
}

/**
 * @brief       Returns the channel event source.
 *
 * @param[in,out] ip            Pointer to a @p SerialUSBDriver instance.
 * @return                      Pointer to the event source.
 *
 * @xclass
 */
event_source_t *sduGetEventSourceX(void *ip) {
  SerialUSBDriver *self = (SerialUSBDriver *)ip;

  osalDbgCheck(self != NULL);

  return &self->event;
}

/**
 * @brief       Returns the USB service view of the serial driver.
 *
 * @param[in,out] ip            Pointer to a @p SerialUSBDriver instance.
 * @return                      Pointer to the embedded USB service base.
 *
 * @xclass
 */
hal_usb_service_c *sduGetServiceX(void *ip) {
  SerialUSBDriver *self = (SerialUSBDriver *)ip;

  osalDbgCheck(self != NULL);

  return (hal_usb_service_c *)self;
}

/**
 * @brief       Starts a serial USB driver instance.
 *
 * @param[in,out] ip            Pointer to a @p SerialUSBDriver instance.
 * @param[in]     config        Driver configuration.
 * @return                      The operation status.
 *
 * @api
 */
msg_t sduStart(void *ip, const SerialUSBConfig *config) {
  SerialUSBDriver *self = (SerialUSBDriver *)ip;
  msg_t msg;

  osalDbgCheck((self != NULL) && (config != NULL));
  osalDbgCheck((config->bulk_in_maxsize > 0U) &&
               (config->bulk_in_maxsize <= SERIAL_USB_BUFFERS_SIZE));
  osalDbgCheck((config->bulk_out_maxsize > 0U) &&
               (config->bulk_out_maxsize <= SERIAL_USB_BUFFERS_SIZE));
  if (config->int_in > 0U) {
    osalDbgCheck((config->int_in_maxsize > 0U) &&
                 (config->int_in_maxsize <= SERIAL_USB_BUFFERS_SIZE));
  }

  msg = usbCdcServiceStart((void *)self,
                           (const hal_usb_cdc_config_t *)config);
  if (msg == HAL_RET_SUCCESS) {
    osalSysLock();
    self->flags = CHN_FL_NONE;
    self->last_txsize = 0U;
    iqResetI(&self->iqueue);
    oqResetI(&self->oqueue);
    osalSysUnlock();
  }

  return msg;
}

/**
 * @brief       Stops a serial USB driver instance.
 *
 * @param[in,out] ip            Pointer to a @p SerialUSBDriver instance.
 *
 * @api
 */
void sduStop(void *ip) {
  SerialUSBDriver *self = (SerialUSBDriver *)ip;

  osalDbgCheck(self != NULL);

  osalSysLock();
  sdu_add_flags_i(self, CHN_FL_DISCONNECTED);
  sdu_reset_queues_i(self, MSG_RESET);
  osalOsRescheduleS();
  osalSysUnlock();

  usbCdcServiceStop((void *)self);
}

/*===========================================================================*/
/* Module class "hal_serial_usb_driver_c" methods.                           */
/*===========================================================================*/

/**
 * @name        Interfaces implementation of hal_serial_usb_driver_c
 * @{
 */
/**
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
static size_t __sdu_chn_write_impl(void *ip, const uint8_t *bp, size_t n) {
  hal_serial_usb_driver_c *self = oopIfGetOwner(hal_serial_usb_driver_c, ip);
  if ((self->state != SDU_READY) || (self->config == NULL) || !self->connected) {
    return 0U;
  }

  return oqWriteTimeout(&self->oqueue, bp, n, TIME_INFINITE);
}

/**
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
static size_t __sdu_chn_read_impl(void *ip, uint8_t *bp, size_t n) {
  hal_serial_usb_driver_c *self = oopIfGetOwner(hal_serial_usb_driver_c, ip);
  if ((self->state != SDU_READY) || (self->config == NULL)) {
    return 0U;
  }
  if (!self->connected && iqIsEmptyI(&self->iqueue)) {
    return 0U;
  }

  return iqReadTimeout(&self->iqueue, bp, n, TIME_INFINITE);
}

/**
 * @brief       Implementation of interface method @p stmPut().
 *
 * @param[in,out] ip            Pointer to the @p asynchronous_channel_i class
 *                              interface.
 * @param[in]     b             The byte value to be written to the stream.
 * @return                      The operation status.
 */
static int __sdu_chn_put_impl(void *ip, uint8_t b) {
  hal_serial_usb_driver_c *self = oopIfGetOwner(hal_serial_usb_driver_c, ip);
  if ((self->state != SDU_READY) || (self->config == NULL) || !self->connected) {
    return STM_RESET;
  }

  return oqPutTimeout(&self->oqueue, b, TIME_INFINITE);
}

/**
 * @brief       Implementation of interface method @p stmGet().
 *
 * @param[in,out] ip            Pointer to the @p asynchronous_channel_i class
 *                              interface.
 * @return                      A byte value from the stream.
 */
static int __sdu_chn_get_impl(void *ip) {
  hal_serial_usb_driver_c *self = oopIfGetOwner(hal_serial_usb_driver_c, ip);
  if ((self->state != SDU_READY) || (self->config == NULL)) {
    return STM_RESET;
  }
  if (!self->connected && iqIsEmptyI(&self->iqueue)) {
    return STM_RESET;
  }

  return iqGetTimeout(&self->iqueue, TIME_INFINITE);
}

/**
 * @brief       Implementation of interface method @p stmUnget().
 *
 * @param[in,out] ip            Pointer to the @p asynchronous_channel_i class
 *                              interface.
 * @param[in]     b             The byte value to be pushed back to the stream.
 * @return                      The operation status.
 */
static int __sdu_chn_unget_impl(void *ip, int b) {
  hal_serial_usb_driver_c *self = oopIfGetOwner(hal_serial_usb_driver_c, ip);
  (void)self;
  (void)b;

  return STM_RESET;
}

/**
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
static size_t __sdu_chn_writet_impl(void *ip, const uint8_t *bp, size_t n,
                                    sysinterval_t timeout) {
  hal_serial_usb_driver_c *self = oopIfGetOwner(hal_serial_usb_driver_c, ip);
  if ((self->state != SDU_READY) || (self->config == NULL) || !self->connected) {
    return 0U;
  }

  return oqWriteTimeout(&self->oqueue, bp, n, timeout);
}

/**
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
static size_t __sdu_chn_readt_impl(void *ip, uint8_t *bp, size_t n,
                                   sysinterval_t timeout) {
  hal_serial_usb_driver_c *self = oopIfGetOwner(hal_serial_usb_driver_c, ip);
  if ((self->state != SDU_READY) || (self->config == NULL)) {
    return 0U;
  }
  if (!self->connected && iqIsEmptyI(&self->iqueue)) {
    return 0U;
  }

  return iqReadTimeout(&self->iqueue, bp, n, timeout);
}

/**
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
static msg_t __sdu_chn_putt_impl(void *ip, uint8_t b, sysinterval_t timeout) {
  hal_serial_usb_driver_c *self = oopIfGetOwner(hal_serial_usb_driver_c, ip);
  if ((self->state != SDU_READY) || (self->config == NULL) || !self->connected) {
    return STM_RESET;
  }

  return oqPutTimeout(&self->oqueue, b, timeout);
}

/**
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
static msg_t __sdu_chn_gett_impl(void *ip, sysinterval_t timeout) {
  hal_serial_usb_driver_c *self = oopIfGetOwner(hal_serial_usb_driver_c, ip);
  if ((self->state != SDU_READY) || (self->config == NULL)) {
    return STM_RESET;
  }
  if (!self->connected && iqIsEmptyI(&self->iqueue)) {
    return STM_RESET;
  }

  return iqGetTimeout(&self->iqueue, timeout);
}

/**
 * @brief       Implementation of interface method @p chnGetAndClearFlags().
 *
 * @param[in,out] ip            Pointer to the @p asynchronous_channel_i class
 *                              interface.
 * @param[in]     mask          Mask of flags to be returned and cleared.
 * @return                      The cleared event flags.
 */
static chnflags_t __sdu_chn_getclr_impl(void *ip, chnflags_t mask) {
  hal_serial_usb_driver_c *self = oopIfGetOwner(hal_serial_usb_driver_c, ip);
  chnflags_t flags;
  syssts_t sts;

  sts = osalSysGetStatusAndLockX();
  flags = self->flags & mask;
  self->flags &= ~mask;
  osalSysRestoreStatusX(sts);

  return flags;
}

/**
 * @brief       Implementation of interface method @p chnControl().
 *
 * @param[in,out] ip            Pointer to the @p asynchronous_channel_i class
 *                              interface.
 * @param[in]     operation     Control operation code
 * @param[in,out] arg           Operation argument.
 * @return                      The operation status.
 */
static msg_t __sdu_chn_ctl_impl(void *ip, unsigned int operation, void *arg) {
  hal_serial_usb_driver_c *self = oopIfGetOwner(hal_serial_usb_driver_c, ip);
  switch (operation) {
  case CHN_CTL_NOP:
    osalDbgCheck(arg == NULL);
    return HAL_RET_SUCCESS;
  case CHN_CTL_TX_WAIT:
    osalDbgCheck(arg == NULL);
    osalSysLock();
    if ((self->state != SDU_READY) || (self->config == NULL) ||
        !self->connected) {
      osalSysUnlock();
      return MSG_RESET;
    }
    if ((oqGetFullI(&self->oqueue) == 0U) && !sdu_is_transmitting_i(self)) {
      osalSysUnlock();
      return MSG_OK;
    }
    self->txsync = NULL;
    {
      msg_t msg = osalThreadSuspendS(&self->txsync);
      osalSysUnlock();
      return msg;
    }
  default:
    break;
  }

  return HAL_RET_UNKNOWN_CTL;
}
/** @} */

/**
 * @name        Methods implementations of hal_serial_usb_driver_c
 * @{
 */
/**
 * @brief       Implementation of object creation.
 * @note        This function is meant to be used by derived classes.
 *
 * @param[out]    ip            Pointer to a @p hal_serial_usb_driver_c
 *                              instance to be initialized.
 * @param[in]     vmt           VMT pointer for the new object.
 * @return                      A new reference to the object.
 */
void *__sdu_objinit_impl(void *ip, const void *vmt) {
  hal_serial_usb_driver_c *self = (hal_serial_usb_driver_c *)ip;

  /* Initialization of interface asynchronous_channel_i.*/
  {
    static const struct asynchronous_channel_vmt sdu_chn_vmt = {
      .instance_offset      = offsetof(hal_serial_usb_driver_c, chn),
      .write                = __sdu_chn_write_impl,
      .read                 = __sdu_chn_read_impl,
      .put                  = __sdu_chn_put_impl,
      .get                  = __sdu_chn_get_impl,
      .unget                = __sdu_chn_unget_impl,
      .writet               = __sdu_chn_writet_impl,
      .readt                = __sdu_chn_readt_impl,
      .putt                 = __sdu_chn_putt_impl,
      .gett                 = __sdu_chn_gett_impl,
      .getclr               = __sdu_chn_getclr_impl,
      .ctl                  = __sdu_chn_ctl_impl
    };
    oopIfObjectInit(&self->chn, &sdu_chn_vmt);
  }

  /* Initialization code.*/
  __cdcsvc_objinit_impl((void *)self, vmt);
  iqObjectInit(&self->iqueue, self->ib, sizeof(self->ib), sdu_inotify, self);
  oqObjectInit(&self->oqueue, self->ob, sizeof(self->ob), sdu_onotify, self);
  osalEventObjectInit(&self->event);
  self->flags = CHN_FL_NONE;
  self->last_txsize = 0U;
  self->txsync = NULL;

  return self;
}

/**
 * @brief       Implementation of object finalization.
 * @note        This function is meant to be used by derived classes.
 *
 * @param[in,out] ip            Pointer to a @p hal_serial_usb_driver_c
 *                              instance to be disposed.
 */
void __sdu_dispose_impl(void *ip) {
  hal_serial_usb_driver_c *self = (hal_serial_usb_driver_c *)ip;

  /* No finalization code.*/
  (void)self;

  /* Finalization of the ancestors-defined parts.*/
  __cdcsvc_dispose_impl(self);
}

/**
 * @brief       Override of method @p usbServiceOnUnbind().
 *
 * @param[in,out] ip            Pointer to a @p hal_serial_usb_driver_c
 *                              instance.
 */
void __sdu_unbind_impl(void *ip) {
  hal_serial_usb_driver_c *self = (hal_serial_usb_driver_c *)ip;
  osalSysLock();
  sdu_add_flags_i(self, CHN_FL_DISCONNECTED);
  sdu_reset_queues_i(self, MSG_RESET);
  osalOsRescheduleS();
  osalSysUnlock();
}

/**
 * @brief       Override of method @p usbCdcServiceOnConfiguredI().
 *
 * @param[in,out] ip            Pointer to a @p hal_serial_usb_driver_c
 *                              instance.
 */
void __sdu_configured_impl(void *ip) {
  hal_serial_usb_driver_c *self = (hal_serial_usb_driver_c *)ip;
  iqResetI(&self->iqueue);
  oqResetI(&self->oqueue);
  self->last_txsize = 0U;
  sdu_add_flags_i(self, CHN_FL_CONNECTED);
  (void)sdu_start_receive_i(self);
}

/**
 * @brief       Override of method @p usbCdcServiceOnDisconnectedI().
 *
 * @param[in,out] ip            Pointer to a @p hal_serial_usb_driver_c
 *                              instance.
 */
void __sdu_disconnected_impl(void *ip) {
  hal_serial_usb_driver_c *self = (hal_serial_usb_driver_c *)ip;
  sdu_add_flags_i(self, CHN_FL_DISCONNECTED);
  sdu_reset_queues_i(self, MSG_RESET);
}

/**
 * @brief       Override of method @p usbCdcServiceOnWakeupHookI().
 *
 * @param[in,out] ip            Pointer to a @p hal_serial_usb_driver_c
 *                              instance.
 */
void __sdu_wakehook_impl(void *ip) {
  hal_serial_usb_driver_c *self = (hal_serial_usb_driver_c *)ip;
  sdu_add_flags_i(self, CHN_FL_CONNECTED);
  (void)sdu_start_receive_i(self);
}

/**
 * @brief       Override of method @p usbCdcServiceOnSOFHookI().
 *
 * @param[in,out] ip            Pointer to a @p hal_serial_usb_driver_c
 *                              instance.
 */
void __sdu_sofhook_impl(void *ip) {
  hal_serial_usb_driver_c *self = (hal_serial_usb_driver_c *)ip;
  (void)sdu_try_transmit_i(self, true);
}

/**
 * @brief       Override of method @p usbCdcServiceOnDataTransmittedI().
 *
 * @param[in,out] ip            Pointer to a @p hal_serial_usb_driver_c
 *                              instance.
 * @param[in]     ep            Endpoint number.
 */
void __sdu_data_transmitted_impl(void *ip, usbep_t ep) {
  hal_serial_usb_driver_c *self = (hal_serial_usb_driver_c *)ip;
  sdu_data_transmitted_i(self, self->config->usbp, ep);
}

/**
 * @brief       Override of method @p usbCdcServiceOnDataReceivedI().
 *
 * @param[in,out] ip            Pointer to a @p hal_serial_usb_driver_c
 *                              instance.
 * @param[in]     ep            Endpoint number.
 */
void __sdu_data_received_impl(void *ip, usbep_t ep) {
  hal_serial_usb_driver_c *self = (hal_serial_usb_driver_c *)ip;
  sdu_data_received_i(self, self->config->usbp, ep);
}

/**
 * @brief       Override of method @p usbCdcServiceOnInterruptTransmittedI().
 *
 * @param[in,out] ip            Pointer to a @p hal_serial_usb_driver_c
 *                              instance.
 * @param[in]     ep            Endpoint number.
 */
void __sdu_irq_transmitted_impl(void *ip, usbep_t ep) {
  hal_serial_usb_driver_c *self = (hal_serial_usb_driver_c *)ip;
  (void)self;
  (void)ep;
}

/**
 * @brief       Override of method @p usbCdcServiceOnControlLineState().
 *
 * @param[in,out] ip            Pointer to a @p hal_serial_usb_driver_c
 *                              instance.
 */
void __sdu_control_line_state_impl(void *ip) {
  hal_serial_usb_driver_c *self = (hal_serial_usb_driver_c *)ip;
  (void)self;
}
/** @} */

/**
 * @brief       VMT structure of serial over USB driver class.
 * @note        It is public because accessed by the inlined constructor.
 */
const struct hal_serial_usb_driver_vmt __hal_serial_usb_driver_vmt = {
  .dispose                  = __sdu_dispose_impl,
  .bind                     = __cdcsvc_bind_impl,
  .unbind                   = __sdu_unbind_impl,
  .reset                    = __cdcsvc_reset_impl,
  .configure                = __cdcsvc_configure_impl,
  .unconfigure              = __cdcsvc_unconfigure_impl,
  .suspend                  = __cdcsvc_suspend_impl,
  .wakeup                   = __cdcsvc_wakeup_impl,
  .sof                      = __cdcsvc_sof_impl,
  .in                       = __cdcsvc_in_impl,
  .out                      = __cdcsvc_out_impl,
  .setup                    = __cdcsvc_setup_impl,
  .configured               = __sdu_configured_impl,
  .disconnected             = __sdu_disconnected_impl,
  .wakehook                 = __sdu_wakehook_impl,
  .sofhook                  = __sdu_sofhook_impl,
  .data_transmitted         = __sdu_data_transmitted_impl,
  .data_received            = __sdu_data_received_impl,
  .irq_transmitted          = __sdu_irq_transmitted_impl,
  .control_line_state       = __sdu_control_line_state_impl
};

#endif /* !defined(SERIAL_USB_USE_MODULE) || (SERIAL_USB_USE_MODULE == TRUE) */

/** @} */
