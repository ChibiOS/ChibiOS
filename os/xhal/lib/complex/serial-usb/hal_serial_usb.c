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
 * @file    hal_serial_usb.c
 * @brief   Serial over USB Driver code.
 *
 * @addtogroup HAL_SERIAL_USB
 * @{
 */

#include <string.h>

#include "hal_serial_usb.h"

#if (SERIAL_USB_USE_MODULE == TRUE) || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver local definitions.                                                 */
/*===========================================================================*/

#define SDU_DEFAULT_STOP_BITS            0U
#define SDU_DEFAULT_PARITY               0U
#define SDU_DEFAULT_DATA_BITS            8U

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

static void sdu_set_default_linecoding(SerialUSBDriver *sdup) {
  sdup->linecoding.dwDTERate[0] = (uint8_t)(SERIAL_USB_DEFAULT_RATE);
  sdup->linecoding.dwDTERate[1] = (uint8_t)(SERIAL_USB_DEFAULT_RATE >> 8);
  sdup->linecoding.dwDTERate[2] = (uint8_t)(SERIAL_USB_DEFAULT_RATE >> 16);
  sdup->linecoding.dwDTERate[3] = (uint8_t)(SERIAL_USB_DEFAULT_RATE >> 24);
  sdup->linecoding.bCharFormat  = SDU_DEFAULT_STOP_BITS;
  sdup->linecoding.bParityType  = SDU_DEFAULT_PARITY;
  sdup->linecoding.bDataBits    = SDU_DEFAULT_DATA_BITS;
}

static void sdu_add_flags_i(SerialUSBDriver *sdup, chnflags_t flags) {
  sdup->flags |= flags;
  osalEventBroadcastFlagsI(&sdup->event, flags);
}

static void sdu_resume_tx_waiter_i(SerialUSBDriver *sdup, msg_t msg) {
  osalThreadResumeI(&sdup->txsync, msg);
}

static bool sdu_is_usb_active_x(SerialUSBDriver *sdup) {
  const SerialUSBConfig *cfg = sdup->config;

  return (cfg != NULL) &&
         (usbGetDriverStateX(cfg->usbp) == USB_ACTIVE) &&
         (sdup->state == SDU_READY) &&
         sdup->connected;
}

static bool sdu_is_transmitting_i(SerialUSBDriver *sdup) {
  const SerialUSBConfig *cfg = sdup->config;
  uint16_t mask;

  if (cfg == NULL) {
    return false;
  }

  mask = (uint16_t)((unsigned)1U << (unsigned)cfg->bulk_in);

  return (bool)((cfg->usbp->transmitting & mask) != 0U);
}

static bool sdu_is_receiving_i(SerialUSBDriver *sdup) {
  const SerialUSBConfig *cfg = sdup->config;
  uint16_t mask;

  if (cfg == NULL) {
    return false;
  }

  mask = (uint16_t)((unsigned)1U << (unsigned)cfg->bulk_out);

  return (bool)((cfg->usbp->receiving & mask) != 0U);
}

static size_t sdu_tx_collect_i(SerialUSBDriver *sdup, size_t n) {
  size_t i;

  for (i = 0U; i < n; i++) {
    msg_t msg = oqGetI(&sdup->oqueue);

    osalDbgAssert(msg >= MSG_OK, "queue unexpectedly empty");
    sdup->txbuf[i] = (uint8_t)msg;
  }

  return n;
}

static bool sdu_start_receive_i(SerialUSBDriver *sdup) {
  size_t n;

  if (!sdu_is_usb_active_x(sdup) || sdu_is_receiving_i(sdup)) {
    return true;
  }

#if (SERIAL_USB_RX_PACKET_MODE == TRUE)
  n = (size_t)sdup->config->bulk_out_maxsize;
  osalDbgAssert(n <= (size_t)SERIAL_USB_BUFFERS_SIZE,
                "SERIAL_USB_BUFFERS_SIZE too small");
#else
  n = (size_t)SERIAL_USB_BUFFERS_SIZE;
#endif

  if (iqGetEmptyI(&sdup->iqueue) < n) {
    return true;
  }

  usbStartReceiveI(sdup->config->usbp, sdup->config->bulk_out, sdup->rxbuf, n);

  return false;
}

static bool sdu_try_transmit_i(SerialUSBDriver *sdup, bool flush) {
  size_t n;

  if (!sdu_is_usb_active_x(sdup) || sdu_is_transmitting_i(sdup)) {
    return false;
  }

  n = oqGetFullI(&sdup->oqueue);
  if (n == 0U) {
    return false;
  }

  if (!flush && (n < (size_t)SERIAL_USB_BUFFERS_SIZE)) {
    return false;
  }

  if (n > (size_t)SERIAL_USB_BUFFERS_SIZE) {
    n = (size_t)SERIAL_USB_BUFFERS_SIZE;
  }

  n = sdu_tx_collect_i(sdup, n);
  sdup->last_txsize = n;
  usbStartTransmitI(sdup->config->usbp, sdup->config->bulk_in, sdup->txbuf, n);

  return true;
}

static void sdu_reset_queues_i(SerialUSBDriver *sdup, msg_t msg) {
  sdup->connected = false;
  iqResetI(&sdup->iqueue);
  oqResetI(&sdup->oqueue);
  sdu_resume_tx_waiter_i(sdup, msg);
}

static void sdu_inotify(io_queue_t *qp) {
  SerialUSBDriver *sdup = (SerialUSBDriver *)qGetLink(qp);

  (void)sdup;

  if (sdup != NULL) {
    sdu_start_receive_i(sdup);
  }
}

static void sdu_onotify(io_queue_t *qp) {
  SerialUSBDriver *sdup = (SerialUSBDriver *)qGetLink(qp);

  if (sdup != NULL) {
    sdu_try_transmit_i(sdup, false);
  }
}

/*===========================================================================*/
/* Interface implementation.                                                 */
/*===========================================================================*/

static size_t __sdu_chn_write_impl(void *ip, const uint8_t *bp, size_t n) {
  SerialUSBDriver *sdup = oopIfGetOwner(SerialUSBDriver, ip);

  if ((sdup->state != SDU_READY) || (sdup->config == NULL) || !sdup->connected) {
    return 0U;
  }

  return oqWriteTimeout(&sdup->oqueue, bp, n, TIME_INFINITE);
}

static size_t __sdu_chn_read_impl(void *ip, uint8_t *bp, size_t n) {
  SerialUSBDriver *sdup = oopIfGetOwner(SerialUSBDriver, ip);

  if ((sdup->state != SDU_READY) || (sdup->config == NULL)) {
    return 0U;
  }
  if (!sdup->connected && iqIsEmptyI(&sdup->iqueue)) {
    return 0U;
  }

  return iqReadTimeout(&sdup->iqueue, bp, n, TIME_INFINITE);
}

static int __sdu_chn_put_impl(void *ip, uint8_t b) {
  SerialUSBDriver *sdup = oopIfGetOwner(SerialUSBDriver, ip);

  if ((sdup->state != SDU_READY) || (sdup->config == NULL) || !sdup->connected) {
    return STM_RESET;
  }

  return oqPutTimeout(&sdup->oqueue, b, TIME_INFINITE);
}

static int __sdu_chn_get_impl(void *ip) {
  SerialUSBDriver *sdup = oopIfGetOwner(SerialUSBDriver, ip);

  if ((sdup->state != SDU_READY) || (sdup->config == NULL)) {
    return STM_RESET;
  }
  if (!sdup->connected && iqIsEmptyI(&sdup->iqueue)) {
    return STM_RESET;
  }

  return iqGetTimeout(&sdup->iqueue, TIME_INFINITE);
}

static int __sdu_chn_unget_impl(void *ip, int b) {
  SerialUSBDriver *sdup = oopIfGetOwner(SerialUSBDriver, ip);

  (void)sdup;
  (void)b;

  return STM_RESET;
}

static size_t __sdu_chn_writet_impl(void *ip, const uint8_t *bp, size_t n,
                                    sysinterval_t timeout) {
  SerialUSBDriver *sdup = oopIfGetOwner(SerialUSBDriver, ip);

  if ((sdup->state != SDU_READY) || (sdup->config == NULL) || !sdup->connected) {
    return 0U;
  }

  return oqWriteTimeout(&sdup->oqueue, bp, n, timeout);
}

static size_t __sdu_chn_readt_impl(void *ip, uint8_t *bp, size_t n,
                                   sysinterval_t timeout) {
  SerialUSBDriver *sdup = oopIfGetOwner(SerialUSBDriver, ip);

  if ((sdup->state != SDU_READY) || (sdup->config == NULL)) {
    return 0U;
  }
  if (!sdup->connected && iqIsEmptyI(&sdup->iqueue)) {
    return 0U;
  }

  return iqReadTimeout(&sdup->iqueue, bp, n, timeout);
}

static msg_t __sdu_chn_putt_impl(void *ip, uint8_t b, sysinterval_t timeout) {
  SerialUSBDriver *sdup = oopIfGetOwner(SerialUSBDriver, ip);

  if ((sdup->state != SDU_READY) || (sdup->config == NULL) || !sdup->connected) {
    return STM_RESET;
  }

  return oqPutTimeout(&sdup->oqueue, b, timeout);
}

static msg_t __sdu_chn_gett_impl(void *ip, sysinterval_t timeout) {
  SerialUSBDriver *sdup = oopIfGetOwner(SerialUSBDriver, ip);

  if ((sdup->state != SDU_READY) || (sdup->config == NULL)) {
    return STM_RESET;
  }
  if (!sdup->connected && iqIsEmptyI(&sdup->iqueue)) {
    return STM_RESET;
  }

  return iqGetTimeout(&sdup->iqueue, timeout);
}

static chnflags_t __sdu_chn_getclr_impl(void *ip, chnflags_t mask) {
  SerialUSBDriver *sdup = oopIfGetOwner(SerialUSBDriver, ip);
  chnflags_t flags;
  syssts_t sts;

  sts = osalSysGetStatusAndLockX();
  flags = sdup->flags & mask;
  sdup->flags &= ~mask;
  osalSysRestoreStatusX(sts);

  return flags;
}

static msg_t __sdu_chn_ctl_impl(void *ip, unsigned int operation, void *arg) {
  SerialUSBDriver *sdup = oopIfGetOwner(SerialUSBDriver, ip);

  switch (operation) {
  case CHN_CTL_NOP:
    osalDbgCheck(arg == NULL);
    return HAL_RET_SUCCESS;
  case CHN_CTL_TX_WAIT:
    osalDbgCheck(arg == NULL);
    osalSysLock();
    if ((sdup->state != SDU_READY) || (sdup->config == NULL) ||
        !sdup->connected) {
      osalSysUnlock();
      return MSG_RESET;
    }
    if ((oqGetFullI(&sdup->oqueue) == 0U) && !sdu_is_transmitting_i(sdup)) {
      osalSysUnlock();
      return MSG_OK;
    }
    sdup->txsync = NULL;
    {
      msg_t msg = osalThreadSuspendS(&sdup->txsync);
      osalSysUnlock();
      return msg;
    }
  default:
    break;
  }

  return HAL_RET_UNKNOWN_CTL;
}

static const struct asynchronous_channel_vmt sdu_chn_vmt = {
  .instance_offset            = offsetof(SerialUSBDriver, chn),
  .write                      = __sdu_chn_write_impl,
  .read                       = __sdu_chn_read_impl,
  .put                        = __sdu_chn_put_impl,
  .get                        = __sdu_chn_get_impl,
  .unget                      = __sdu_chn_unget_impl,
  .writet                     = __sdu_chn_writet_impl,
  .readt                      = __sdu_chn_readt_impl,
  .putt                       = __sdu_chn_putt_impl,
  .gett                       = __sdu_chn_gett_impl,
  .getclr                     = __sdu_chn_getclr_impl,
  .ctl                        = __sdu_chn_ctl_impl
};

/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

void sduObjectInit(SerialUSBDriver *sdup) {
  osalDbgCheck(sdup != NULL);

  oopIfObjectInit(&sdup->chn, &sdu_chn_vmt);
  iqObjectInit(&sdup->iqueue, sdup->ib, sizeof(sdup->ib), sdu_inotify, sdup);
  oqObjectInit(&sdup->oqueue, sdup->ob, sizeof(sdup->ob), sdu_onotify, sdup);
  osalEventObjectInit(&sdup->event);
  sdup->flags = CHN_FL_NONE;
  sdup->state = SDU_STOP;
  sdup->connected = false;
  sdup->config = NULL;
  sdup->control_line_state = 0U;
  sdup->last_txsize = 0U;
  sdup->txsync = NULL;
  memset(&sdup->bulk_in_state, 0, sizeof(sdup->bulk_in_state));
  memset(&sdup->bulk_out_state, 0, sizeof(sdup->bulk_out_state));
  memset(&sdup->int_in_state, 0, sizeof(sdup->int_in_state));
  memset(&sdup->bulk_in_epc, 0, sizeof(sdup->bulk_in_epc));
  memset(&sdup->bulk_out_epc, 0, sizeof(sdup->bulk_out_epc));
  memset(&sdup->int_in_epc, 0, sizeof(sdup->int_in_epc));

  sdup->bulk_in_epc.ep_mode    = USB_EP_MODE_TYPE_BULK;
  sdup->bulk_in_epc.in_cb      = sduDataTransmitted;
  sdup->bulk_in_epc.in_state   = &sdup->bulk_in_state;

  sdup->bulk_out_epc.ep_mode   = USB_EP_MODE_TYPE_BULK;
  sdup->bulk_out_epc.out_cb    = sduDataReceived;
  sdup->bulk_out_epc.out_state = &sdup->bulk_out_state;

  sdup->int_in_epc.ep_mode     = USB_EP_MODE_TYPE_INTR;
  sdup->int_in_epc.in_cb       = sduInterruptTransmitted;
  sdup->int_in_epc.in_state    = &sdup->int_in_state;

  sdu_set_default_linecoding(sdup);
}

msg_t sduStart(SerialUSBDriver *sdup, const SerialUSBConfig *config) {
  hal_usb_driver_c *usbp;

  osalDbgCheck((sdup != NULL) && (config != NULL) && (config->usbp != NULL));
  osalDbgCheck((config->bulk_in > 0U) &&
               (config->bulk_in <= (usbep_t)USB_MAX_ENDPOINTS));
  osalDbgCheck((config->bulk_out > 0U) &&
               (config->bulk_out <= (usbep_t)USB_MAX_ENDPOINTS));
  osalDbgCheck((config->bulk_in_maxsize > 0U) &&
               (config->bulk_in_maxsize <= SERIAL_USB_BUFFERS_SIZE));
  osalDbgCheck((config->bulk_out_maxsize > 0U) &&
               (config->bulk_out_maxsize <= SERIAL_USB_BUFFERS_SIZE));
  if (config->int_in > 0U) {
    osalDbgCheck((config->int_in <= (usbep_t)USB_MAX_ENDPOINTS) &&
                 (config->int_in_maxsize > 0U) &&
                 (config->int_in_maxsize <= SERIAL_USB_BUFFERS_SIZE));
  }

  osalSysLock();
  osalDbgAssert((sdup->state == SDU_STOP) || (sdup->state == SDU_READY),
                "invalid state");

  usbp = config->usbp;
  usbp->in_params[config->bulk_in - 1U]   = sdup;
  usbp->out_params[config->bulk_out - 1U] = sdup;
  if (config->int_in > 0U) {
    usbp->in_params[config->int_in - 1U]  = sdup;
  }

  sdup->bulk_in_epc.in_maxsize   = config->bulk_in_maxsize;
  sdup->bulk_out_epc.out_maxsize = config->bulk_out_maxsize;
  sdup->int_in_epc.in_maxsize    = config->int_in_maxsize;

  if (config->bulk_in == config->bulk_out) {
    sdup->bulk_in_epc.out_cb      = sduDataReceived;
    sdup->bulk_in_epc.out_state   = &sdup->bulk_out_state;
    sdup->bulk_in_epc.out_maxsize = config->bulk_out_maxsize;
  }
  else {
    sdup->bulk_in_epc.out_cb      = NULL;
    sdup->bulk_in_epc.out_state   = NULL;
    sdup->bulk_in_epc.out_maxsize = 0U;
  }

  sdup->config      = config;
  sdup->connected   = false;
  sdup->flags       = CHN_FL_NONE;
  sdup->last_txsize = 0U;
  sdup->state       = SDU_READY;

  iqResetI(&sdup->iqueue);
  oqResetI(&sdup->oqueue);
  osalSysUnlock();

  return HAL_RET_SUCCESS;
}

void sduStop(SerialUSBDriver *sdup) {
  hal_usb_driver_c *usbp = NULL;

  osalDbgCheck(sdup != NULL);

  osalSysLock();
  osalDbgAssert((sdup->state == SDU_STOP) || (sdup->state == SDU_READY),
                "invalid state");

  if (sdup->config != NULL) {
    usbp = sdup->config->usbp;
    usbp->in_params[sdup->config->bulk_in - 1U]   = NULL;
    usbp->out_params[sdup->config->bulk_out - 1U] = NULL;
    if (sdup->config->int_in > 0U) {
      usbp->in_params[sdup->config->int_in - 1U] = NULL;
    }
  }

  sdup->config = NULL;
  sdup->state  = SDU_STOP;
  sdu_add_flags_i(sdup, CHN_FL_DISCONNECTED);
  sdu_reset_queues_i(sdup, MSG_RESET);
  osalOsRescheduleS();
  osalSysUnlock();
}

void sduSuspendHookI(SerialUSBDriver *sdup) {
  osalDbgCheckClassI();
  osalDbgCheck(sdup != NULL);

  if (!sdup->connected) {
    return;
  }

  sdu_add_flags_i(sdup, CHN_FL_DISCONNECTED);
  sdu_reset_queues_i(sdup, MSG_RESET);
}

void sduWakeupHookI(SerialUSBDriver *sdup) {
  osalDbgCheckClassI();
  osalDbgCheck(sdup != NULL);

  if ((sdup->state != SDU_READY) || (sdup->config == NULL)) {
    return;
  }

  sdup->connected = true;
  sdu_add_flags_i(sdup, CHN_FL_CONNECTED);
  (void)sdu_start_receive_i(sdup);
}

void sduConfigureHookI(SerialUSBDriver *sdup) {
  osalDbgCheckClassI();
  osalDbgCheck((sdup != NULL) && (sdup->config != NULL));

  usbInitEndpointI(sdup->config->usbp, sdup->config->bulk_in, &sdup->bulk_in_epc);
  if (sdup->config->bulk_out != sdup->config->bulk_in) {
    usbInitEndpointI(sdup->config->usbp, sdup->config->bulk_out,
                     &sdup->bulk_out_epc);
  }
  if (sdup->config->int_in > 0U) {
    usbInitEndpointI(sdup->config->usbp, sdup->config->int_in, &sdup->int_in_epc);
  }

  iqResetI(&sdup->iqueue);
  oqResetI(&sdup->oqueue);
  sdup->connected = true;
  sdup->last_txsize = 0U;
  sdu_add_flags_i(sdup, CHN_FL_CONNECTED);
  (void)sdu_start_receive_i(sdup);
}

msg_t sduRequestsHook(SerialUSBDriver *sdup, bool *handledp) {
  hal_usb_driver_c *usbp;
  uint8_t *setup;

  osalDbgCheck((sdup != NULL) && (handledp != NULL) && (sdup->config != NULL));

  *handledp = false;
  usbp = sdup->config->usbp;
  setup = usbp->setup;

  if ((setup[0] & USB_RTYPE_TYPE_MASK) != USB_RTYPE_TYPE_CLASS) {
    return HAL_RET_SUCCESS;
  }
  if ((setup[0] & USB_RTYPE_RECIPIENT_MASK) != USB_RTYPE_RECIPIENT_INTERFACE) {
    return HAL_RET_SUCCESS;
  }
  if (setup[4] != sdup->config->control_if) {
    return HAL_RET_SUCCESS;
  }

  switch (setup[1]) {
  case CDC_GET_LINE_CODING:
    *handledp = true;
    return usbEp0Reply(usbp, (const uint8_t *)&sdup->linecoding,
                       sizeof(sdup->linecoding));
  case CDC_SET_LINE_CODING:
    *handledp = true;
    return usbEp0Receive(usbp, (uint8_t *)&sdup->linecoding,
                         sizeof(sdup->linecoding));
  case CDC_SET_CONTROL_LINE_STATE:
    *handledp = true;
    sdup->control_line_state = (uint16_t)setup[2] |
                               ((uint16_t)setup[3] << 8U);
    return usbEp0Acknowledge(usbp);
  default:
    break;
  }

  return HAL_RET_SUCCESS;
}

void sduSOFHookI(SerialUSBDriver *sdup) {
  osalDbgCheckClassI();
  osalDbgCheck(sdup != NULL);

  (void)sdu_try_transmit_i(sdup, true);
}

void sduDataTransmitted(hal_usb_driver_c *usbp, usbep_t ep) {
  SerialUSBDriver *sdup;

  osalDbgAssert(ep != 0U, "invalid endpoint");
  if (ep == 0U) {
    return;
  }

  sdup = (SerialUSBDriver *)usbp->in_params[ep - 1U];
  if (sdup == NULL) {
    return;
  }

  osalSysLockFromISR();
  sdu_add_flags_i(sdup, CHN_FL_TX_NOTFULL);
  if (!sdu_try_transmit_i(sdup, false)) {
#if (SERIAL_USB_SEND_ZLP == TRUE)
    if ((sdup->last_txsize > 0U) &&
        ((sdup->last_txsize % (size_t)sdup->config->bulk_in_maxsize) == 0U)) {
      sdup->last_txsize = 0U;
      usbStartTransmitI(usbp, ep, usbp->setup, 0U);
      osalSysUnlockFromISR();
      return;
    }
#endif
    sdu_add_flags_i(sdup, CHN_FL_TX_END);
    sdu_resume_tx_waiter_i(sdup, MSG_OK);
  }
  osalSysUnlockFromISR();
}

void sduDataReceived(hal_usb_driver_c *usbp, usbep_t ep) {
  SerialUSBDriver *sdup;
  size_t i;
  size_t n;

  osalDbgAssert(ep != 0U, "invalid endpoint");
  if (ep == 0U) {
    return;
  }

  sdup = (SerialUSBDriver *)usbp->out_params[ep - 1U];
  if (sdup == NULL) {
    return;
  }

  n = usbGetReceiveTransactionSizeX(usbp, ep);
  osalSysLockFromISR();
  for (i = 0U; i < n; i++) {
    if (iqPutI(&sdup->iqueue, sdup->rxbuf[i]) != MSG_OK) {
      sdu_add_flags_i(sdup, CHN_FL_BUFFER_FULL_ERR);
      break;
    }
  }
  if (n > 0U) {
    sdu_add_flags_i(sdup, CHN_FL_RX_NOTEMPTY);
  }
  (void)sdu_start_receive_i(sdup);
  osalSysUnlockFromISR();
}

void sduInterruptTransmitted(hal_usb_driver_c *usbp, usbep_t ep) {
  (void)usbp;
  (void)ep;
}

#endif /* SERIAL_USB_USE_MODULE == TRUE */

/** @} */
