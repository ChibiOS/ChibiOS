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
 * @file        hal_usb.c
 * @brief       Generated USB Driver source.
 * @note        This is a generated file, do not edit directly.
 *
 * @addtogroup  HAL_USB
 * @{
 */

#include "string.h"
#include "hal.h"

#if (HAL_USE_USB == TRUE) || defined(__DOXYGEN__)

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

static uint16_t get_hword(uint8_t *p) {
  uint16_t hw;

  hw  = (uint16_t)*p++;
  hw |= (uint16_t)*p << 8U;

  return hw;
}

static void usb_invoke_event_cb(hal_usb_driver_c *usbp, usbeventflags_t flags) {
  usbp->events |= flags;
}

static void setup_reset(hal_usb_driver_c *usbp) {
  usbp->receiving &= ~1U;
  usbp->transmitting &= ~1U;
  usbp->ep0n = 0U;
  usbp->ep0state = USB_EP0_STP_WAITING;
}

static void setup_error(hal_usb_driver_c *usbp) {
  usb_lld_stall_in(usbp, 0U);
  usb_lld_stall_out(usbp, 0U);
  usb_invoke_event_cb(usbp, USB_FLAGS_STALLED);
  usbp->receiving &= ~1U;
  usbp->transmitting &= ~1U;
  usbp->ep0n = 0U;
  usbp->ep0state = USB_EP0_ERROR;
}

static void ep0_resume_waiterI(hal_usb_driver_c *usbp, msg_t msg) {
  osalThreadResumeI(&usbp->ep0thread, msg);
}

static void ep0_signal_resetI(hal_usb_driver_c *usbp) {
  usbp->ep0setup = 0U;
  usbp->ep0reset = 1U;
  usbp->ep0seq++;
  ep0_resume_waiterI(usbp, MSG_RESET);
}

static void ep0_signal_setupI(hal_usb_driver_c *usbp, msg_t msg) {
  usbp->ep0setup = 1U;
  usbp->ep0reset = 0U;
  usbp->ep0seq++;
  ep0_resume_waiterI(usbp, msg);
}

static void set_address_thread(hal_usb_driver_c *usbp) {
  usbp->address = usbp->setup[2];
  usb_lld_set_address(usbp);
  usb_invoke_event_cb(usbp, USB_FLAGS_ADDRESS);
  usbp->state = USB_SELECTED;
}

static msg_t ep0_reply_or_ack(hal_usb_driver_c *usbp, const uint8_t *buf,
                              size_t n) {
  msg_t msg;
  size_t max;

  osalSysLock();
  if ((usbp->state == USB_STOP) || (usbp->ep0rseq != usbp->ep0seq)) {
    osalSysUnlock();
    return MSG_RESET;
  }

  max = (size_t)get_hword(&usbp->setup[6]);
  if (n > max) {
    n = max;
  }

  usbp->ep0next = (uint8_t *)buf;
  usbp->ep0n = n;
  if (n != 0U) {
    usbp->ep0state = USB_EP0_IN_TX;
    usbStartTransmitI(usbp, 0U, buf, n);
  }
  else {
    usbp->ep0state = USB_EP0_OUT_WAITING_STS;
    usbStartReceiveI(usbp, 0U, NULL, 0U);
  }

  msg = osalThreadSuspendS(&usbp->ep0thread);
  if (usbp->ep0rseq != usbp->ep0seq) {
    msg = MSG_RESET;
  }
  osalSysUnlock();

  return msg;
}

static msg_t ep0_receive_or_status(hal_usb_driver_c *usbp, uint8_t *buf,
                                   size_t n) {
  msg_t msg;
  size_t max;

  osalSysLock();
  if ((usbp->state == USB_STOP) || (usbp->ep0rseq != usbp->ep0seq)) {
    osalSysUnlock();
    return MSG_RESET;
  }

  max = (size_t)get_hword(&usbp->setup[6]);
  if (n > max) {
    n = max;
  }

  usbp->ep0next = buf;
  usbp->ep0n = n;
  if (n != 0U) {
    usbp->ep0state = USB_EP0_OUT_RX;
    usbStartReceiveI(usbp, 0U, buf, n);
  }
  else {
    usbp->ep0state = USB_EP0_IN_SENDING_STS;
    usbStartTransmitI(usbp, 0U, NULL, 0U);
  }

  msg = osalThreadSuspendS(&usbp->ep0thread);
  if (usbp->ep0rseq != usbp->ep0seq) {
    msg = MSG_RESET;
  }
  osalSysUnlock();

  return msg;
}

static const uint8_t zero_status[] = {0x00U, 0x00U};
static const uint8_t active_status[] = {0x00U, 0x00U};
static const uint8_t halted_status[] = {0x01U, 0x00U};

/*===========================================================================*/
/* Module exported functions.                                                */
/*===========================================================================*/

/**
 * @brief       USB Driver initialization.
 *
 * @init
 */
void usbInit(void) {
  usb_lld_init();
}

/**
 * @brief       Common USB reset handler invoked by the LLD.
 *
 * @param[in,out] usbp          USB driver instance.
 *
 * @notapi
 */
void _usb_reset(hal_usb_driver_c *usbp) {
  unsigned i;

  usbp->state         = USB_READY;
  usbp->status        = 0U;
  usbp->address       = 0U;
  usbp->configuration = 0U;
  usbp->ep0n          = 0U;
  usbp->transmitting  = 0U;
  usbp->receiving     = 0U;
  for (i = 0U; i <= (unsigned)USB_MAX_ENDPOINTS; i++) {
#if USB_USE_WAIT == TRUE
    if (usbp->epc[i] != NULL) {
      osalSysLockFromISR();
      if (usbp->epc[i]->in_state != NULL) {
        osalThreadResumeI(&usbp->epc[i]->in_state->thread, MSG_RESET);
      }
      if (usbp->epc[i]->out_state != NULL) {
        osalThreadResumeI(&usbp->epc[i]->out_state->thread, MSG_RESET);
      }
      osalSysUnlockFromISR();
    }
#endif
    usbp->epc[i] = NULL;
  }

  usbp->ep0state = USB_EP0_STP_WAITING;
  usbp->ep0setup = 0U;
  usbp->ep0reset = 0U;
  usb_lld_reset(usbp);
  if (usbp->binder != NULL) {
    usbBinderResetI(usbp->binder);
  }
  osalSysLockFromISR();
  ep0_signal_resetI(usbp);
  osalSysUnlockFromISR();
  _usb_isr_invoke_event_cb(usbp, USB_FLAGS_RESET);
}

/**
 * @brief       Common USB suspend handler invoked by the LLD.
 *
 * @param[in,out] usbp          USB driver instance.
 *
 * @notapi
 */
void _usb_suspend(hal_usb_driver_c *usbp) {
  if (usbp->state != USB_SUSPENDED) {
    unsigned i;

    usbp->saved_state = usbp->state;
    usbp->state = USB_SUSPENDED;
    if (usbp->binder != NULL) {
      usbBinderSuspendI(usbp->binder);
    }
    _usb_isr_invoke_event_cb(usbp, USB_FLAGS_SUSPEND);
    usbp->transmitting = 0U;
    usbp->receiving = 0U;
#if USB_USE_WAIT == TRUE
    for (i = 0U; i <= (unsigned)USB_MAX_ENDPOINTS; i++) {
      if (usbp->epc[i] != NULL) {
        osalSysLockFromISR();
        if (usbp->epc[i]->in_state != NULL) {
          osalThreadResumeI(&usbp->epc[i]->in_state->thread, MSG_RESET);
        }
        if (usbp->epc[i]->out_state != NULL) {
          osalThreadResumeI(&usbp->epc[i]->out_state->thread, MSG_RESET);
        }
        osalSysUnlockFromISR();
      }
    }
#endif
    osalSysLockFromISR();
    ep0_signal_resetI(usbp);
    osalSysUnlockFromISR();
  }
}

/**
 * @brief       Common USB wake-up handler invoked by the LLD.
 *
 * @param[in,out] usbp          USB driver instance.
 *
 * @notapi
 */
void _usb_wakeup(hal_usb_driver_c *usbp) {
  if (usbp->state == USB_SUSPENDED) {
    usbp->state = usbp->saved_state;
    if (usbp->binder != NULL) {
      usbBinderWakeupI(usbp->binder);
    }
    _usb_isr_invoke_event_cb(usbp, USB_FLAGS_WAKEUP);
  }
}

/**
 * @brief       Common endpoint-zero SETUP handler invoked by the LLD.
 *
 * @param[in,out] usbp          USB driver instance.
 * @param[in]     ep            Endpoint number.
 *
 * @notapi
 */
void _usb_ep0setup(hal_usb_driver_c *usbp, usbep_t ep) {
  msg_t msg = MSG_OK;

  osalDbgAssert(ep == 0U, "EP not zero");

  if (usbp->ep0state != USB_EP0_STP_WAITING) {
    setup_reset(usbp);
    msg = MSG_RESET;
  }

  usbReadSetupI(usbp, 0U, usbp->setup);
  osalSysLockFromISR();
  ep0_signal_setupI(usbp, msg);
  osalSysUnlockFromISR();
}

/**
 * @brief       Common endpoint-zero IN handler invoked by the LLD.
 *
 * @param[in,out] usbp          USB driver instance.
 * @param[in]     ep            Endpoint number.
 *
 * @notapi
 */
void _usb_ep0in(hal_usb_driver_c *usbp, usbep_t ep) {
  size_t max;

  osalDbgAssert(ep == 0U, "EP not zero");

  switch (usbp->ep0state) {
  case USB_EP0_IN_TX:
    max = (size_t)get_hword(&usbp->setup[6]);
    if ((usbp->ep0n < max) &&
        ((usbp->ep0n % usbp->epc[0]->in_maxsize) == 0U)) {
      osalSysLockFromISR();
      usbStartTransmitI(usbp, 0U, NULL, 0U);
      osalSysUnlockFromISR();
      usbp->ep0state = USB_EP0_IN_WAITING_TX0;
      return;
    }
    /* Falls through. */
  case USB_EP0_IN_WAITING_TX0:
    usbp->ep0state = USB_EP0_OUT_WAITING_STS;
    osalSysLockFromISR();
    usbStartReceiveI(usbp, 0U, NULL, 0U);
    osalSysUnlockFromISR();
    return;
  case USB_EP0_IN_SENDING_STS:
    setup_reset(usbp);
    osalSysLockFromISR();
    ep0_resume_waiterI(usbp, MSG_OK);
    osalSysUnlockFromISR();
    return;
  case USB_EP0_OUT_WAITING_STS:
    return;
  case USB_EP0_ERROR:
    setup_error(usbp);
    osalSysLockFromISR();
    ep0_signal_resetI(usbp);
    osalSysUnlockFromISR();
    return;
  default:
    osalDbgAssert(false, "EP0 state machine error");
  }
}

/**
 * @brief       Common endpoint-zero OUT handler invoked by the LLD.
 *
 * @param[in,out] usbp          USB driver instance.
 * @param[in]     ep            Endpoint number.
 *
 * @notapi
 */
void _usb_ep0out(hal_usb_driver_c *usbp, usbep_t ep) {
  osalDbgAssert(ep == 0U, "EP not zero");

  switch (usbp->ep0state) {
  case USB_EP0_OUT_RX:
    usbp->ep0state = USB_EP0_IN_SENDING_STS;
    osalSysLockFromISR();
    usbStartTransmitI(usbp, 0U, NULL, 0U);
    osalSysUnlockFromISR();
    return;
  case USB_EP0_OUT_WAITING_STS:
    if (usbGetReceiveTransactionSizeX(usbp, 0U) != 0U) {
      break;
    }
    setup_reset(usbp);
    osalSysLockFromISR();
    ep0_resume_waiterI(usbp, MSG_OK);
    osalSysUnlockFromISR();
    return;
  case USB_EP0_IN_TX:
    return;
  case USB_EP0_ERROR:
    setup_error(usbp);
    osalSysLockFromISR();
    ep0_signal_resetI(usbp);
    osalSysUnlockFromISR();
    return;
  default:
    osalDbgAssert(false, "EP0 state machine error");
  }
}

/*===========================================================================*/
/* Module class "hal_usb_driver_c" methods.                                  */
/*===========================================================================*/

/**
 * @name        Methods implementations of hal_usb_driver_c
 * @{
 */
/**
 * @brief       Implementation of object creation.
 * @note        This function is meant to be used by derived classes.
 *
 * @param[out]    ip            Pointer to a @p hal_usb_driver_c instance to be
 *                              initialized.
 * @param[in]     vmt           VMT pointer for the new object.
 * @return                      A new reference to the object.
 */
void *__usb_objinit_impl(void *ip, const void *vmt) {
  hal_usb_driver_c *self = (hal_usb_driver_c *)ip;

  /* Initialization of the ancestors-defined parts.*/
  __drv_objinit_impl(self, vmt);

  /* Initialization code.*/
  unsigned i;

  self->events        = (usbeventflags_t)0U;
  self->binder        = NULL;
  self->transmitting  = 0U;
  self->receiving     = 0U;
  self->ep0state      = USB_EP0_STP_WAITING;
  self->ep0next       = NULL;
  self->ep0n          = 0U;
  self->ep0endcb      = NULL;
  self->ep0thread     = NULL;
  self->ep0seq        = 0U;
  self->ep0rseq       = 0U;
  self->ep0setup      = 0U;
  self->ep0reset      = 0U;
  self->status        = 0U;
  self->address       = 0U;
  self->configuration = 0U;
  self->saved_state   = USB_STOP;
  for (i = 0U; i <= (unsigned)USB_MAX_ENDPOINTS; i++) {
    self->epc[i] = NULL;
  }
  for (i = 0U; i < (unsigned)USB_MAX_ENDPOINTS; i++) {
    self->in_params[i] = NULL;
    self->out_params[i] = NULL;
  }

  return self;
}

/**
 * @brief       Implementation of object finalization.
 * @note        This function is meant to be used by derived classes.
 *
 * @param[in,out] ip            Pointer to a @p hal_usb_driver_c instance to be
 *                              disposed.
 */
void __usb_dispose_impl(void *ip) {
  hal_usb_driver_c *self = (hal_usb_driver_c *)ip;

  /* No finalization code.*/
  (void)self;

  /* Finalization of the ancestors-defined parts.*/
  __drv_dispose_impl(self);
}

/**
 * @brief       Override of method @p __drv_start().
 *
 * @param[in,out] ip            Pointer to a @p hal_usb_driver_c instance.
 * @return                      The operation status.
 */
msg_t __usb_start_impl(void *ip) {
  hal_usb_driver_c *self = (hal_usb_driver_c *)ip;
  msg_t msg;
  unsigned i;

  self->events        = (usbeventflags_t)0U;
  self->transmitting  = 0U;
  self->receiving     = 0U;
  self->ep0state      = USB_EP0_STP_WAITING;
  self->ep0next       = NULL;
  self->ep0n          = 0U;
  self->ep0endcb      = NULL;
  self->ep0thread     = NULL;
  self->ep0seq        = 0U;
  self->ep0rseq       = 0U;
  self->ep0setup      = 0U;
  self->ep0reset      = 0U;
  self->status        = 0U;
  self->address       = 0U;
  self->configuration = 0U;
  self->saved_state   = USB_STOP;
  for (i = 0U; i <= (unsigned)USB_MAX_ENDPOINTS; i++) {
    self->epc[i] = NULL;
  }

  msg = usb_lld_start(self);
  if (msg != HAL_RET_SUCCESS) {
    self->config = NULL;
  }

  return msg;
}

/**
 * @brief       Override of method @p __drv_stop().
 *
 * @param[in,out] ip            Pointer to a @p hal_usb_driver_c instance.
 */
void __usb_stop_impl(void *ip) {
  hal_usb_driver_c *self = (hal_usb_driver_c *)ip;
  hal_usb_binder_c *binderp;
  unsigned i;

  usb_lld_stop(self);
  binderp             = self->binder;
  self->binder        = NULL;
  if (binderp != NULL) {
    usbBinderUnbind(binderp);
  }
  self->events        = (usbeventflags_t)0U;
  self->transmitting  = 0U;
  self->receiving     = 0U;
  self->ep0state      = USB_EP0_STP_WAITING;
  self->ep0next       = NULL;
  self->ep0n          = 0U;
  self->ep0endcb      = NULL;
  self->ep0thread     = NULL;
  self->ep0seq        = 0U;
  self->ep0rseq       = 0U;
  self->ep0setup      = 0U;
  self->ep0reset      = 0U;
  self->status        = 0U;
  self->address       = 0U;
  self->configuration = 0U;
  self->saved_state   = USB_STOP;
  for (i = 0U; i <= (unsigned)USB_MAX_ENDPOINTS; i++) {
#if USB_USE_WAIT == TRUE
    if (self->epc[i] != NULL) {
      if (self->epc[i]->in_state != NULL) {
        osalThreadResumeI(&self->epc[i]->in_state->thread, MSG_RESET);
      }
      if (self->epc[i]->out_state != NULL) {
        osalThreadResumeI(&self->epc[i]->out_state->thread, MSG_RESET);
      }
    }
#endif
    self->epc[i] = NULL;
  }
}

/**
 * @brief       Override of method @p __drv_set_cfg().
 *
 * @param[in,out] ip            Pointer to a @p hal_usb_driver_c instance.
 * @param[in]     config        New driver configuration.
 * @return                      The configuration pointer.
 */
const void *__usb_setcfg_impl(void *ip, const void *config) {
  hal_usb_driver_c *self = (hal_usb_driver_c *)ip;
  if ((self->state == USB_SELECTED) ||
      (self->state == USB_ACTIVE) ||
      (self->state == USB_SUSPENDED)) {
    return NULL;
  }

  return (const void *)usb_lld_setcfg(self,
                                      (const hal_usb_config_t *)config);
}

/**
 * @brief       Override of method @p __drv_sel_cfg().
 *
 * @param[in,out] ip            Pointer to a @p hal_usb_driver_c instance.
 * @param[in]     cfgnum        Driver configuration number.
 * @return                      The configuration pointer.
 */
const void *__usb_selcfg_impl(void *ip, unsigned cfgnum) {
  hal_usb_driver_c *self = (hal_usb_driver_c *)ip;
  if ((self->state == USB_SELECTED) ||
      (self->state == USB_ACTIVE) ||
      (self->state == USB_SUSPENDED)) {
    return NULL;
  }

  return (const void *)usb_lld_selcfg(self, cfgnum);
}
/** @} */

/**
 * @brief       VMT structure of USB driver class.
 * @note        It is public because accessed by the inlined constructor.
 */
const struct hal_usb_driver_vmt __hal_usb_driver_vmt = {
  .dispose                  = __usb_dispose_impl,
  .start                    = __usb_start_impl,
  .stop                     = __usb_stop_impl,
  .setcfg                   = __usb_setcfg_impl,
  .selcfg                   = __usb_selcfg_impl
};

/**
 * @name        Regular methods of hal_usb_driver_c
 * @{
 */
/**
 * @brief       Configures and activates the USB peripheral.
 *
 * @param[in,out] ip            Pointer to a @p hal_usb_driver_c instance.
 * @param[in]     config        Hardware configuration or @p NULL.
 * @return                      The operation status.
 *
 * @api
 */
msg_t usbStart(void *ip, const hal_usb_config_t *config) {
  hal_usb_driver_c *self = (hal_usb_driver_c *)ip;
  msg_t msg;

  osalDbgCheck(self != NULL);

  if (config != NULL) {
    msg = drvSetCfgX(self, config);
    if (msg != HAL_RET_SUCCESS) {
      return msg;
    }
  }

  return drvStart(self);
}

/**
 * @brief       Deactivates the USB peripheral.
 *
 * @param[in,out] ip            Pointer to a @p hal_usb_driver_c instance.
 *
 * @api
 */
void usbStop(void *ip) {
  hal_usb_driver_c *self = (hal_usb_driver_c *)ip;
  osalDbgCheck(self != NULL);

  drvStop(self);
}

/**
 * @brief       Binds a USB binder object to the USB driver.
 *
 * @param[in,out] ip            Pointer to a @p hal_usb_driver_c instance.
 * @param[in]     binderp       USB binder object.
 * @return                      The operation status.
 *
 * @api
 */
msg_t usbBind(void *ip, hal_usb_binder_c *binderp) {
  hal_usb_driver_c *self = (hal_usb_driver_c *)ip;
  msg_t msg;

  osalDbgCheck((self != NULL) && (binderp != NULL));

  osalSysLock();
  if ((self->binder != NULL) ||
      (self->state == USB_SELECTED) ||
      (self->state == USB_ACTIVE) ||
      (self->state == USB_SUSPENDED)) {
    osalSysUnlock();
    return HAL_RET_INV_STATE;
  }
  osalSysUnlock();

  msg = usbBinderBind(binderp, self);
  if (msg == HAL_RET_SUCCESS) {
    self->binder = binderp;
  }

  return msg;
}

/**
 * @brief       Removes the currently bound USB binder.
 *
 * @param[in,out] ip            Pointer to a @p hal_usb_driver_c instance.
 * @return                      The operation status.
 *
 * @api
 */
msg_t usbUnbind(void *ip) {
  hal_usb_driver_c *self = (hal_usb_driver_c *)ip;
  hal_usb_binder_c *binderp;
  osalDbgCheck(self != NULL);

  osalSysLock();
  if ((self->state == USB_SELECTED) ||
      (self->state == USB_ACTIVE) ||
      (self->state == USB_SUSPENDED)) {
    osalSysUnlock();
    return HAL_RET_INV_STATE;
  }
  binderp = self->binder;
  self->binder = NULL;
  osalSysUnlock();

  if (binderp != NULL) {
    usbBinderUnbind(binderp);
  }

  return HAL_RET_SUCCESS;
}

/**
 * @brief       Connects the USB device to the bus.
 *
 * @param[in,out] ip            Pointer to a @p hal_usb_driver_c instance.
 *
 * @api
 */
void usbConnectBus(void *ip) {
  hal_usb_driver_c *self = (hal_usb_driver_c *)ip;
  osalDbgCheck(self != NULL);
  osalDbgAssert(self->binder != NULL, "no binder");

  usb_lld_connect_bus(self);
}

/**
 * @brief       Disconnects the USB device from the bus.
 *
 * @param[in,out] ip            Pointer to a @p hal_usb_driver_c instance.
 *
 * @api
 */
void usbDisconnectBus(void *ip) {
  hal_usb_driver_c *self = (hal_usb_driver_c *)ip;
  osalDbgCheck(self != NULL);

  usb_lld_disconnect_bus(self);
}

/**
 * @brief       Enables an endpoint.
 *
 * @param[in,out] ip            Pointer to a @p hal_usb_driver_c instance.
 * @param[in]     ep            Endpoint number.
 * @param[in]     epcp          Endpoint configuration.
 *
 * @iclass
 */
void usbInitEndpointI(void *ip, usbep_t ep, const USBEndpointConfig *epcp) {
  hal_usb_driver_c *self = (hal_usb_driver_c *)ip;
  osalDbgCheckClassI();
  osalDbgCheck((self != NULL) && (epcp != NULL) &&
               (ep <= (usbep_t)USB_MAX_ENDPOINTS));
  osalDbgAssert(self->state == USB_ACTIVE, "invalid state");
  osalDbgAssert(self->epc[ep] == NULL, "already initialized");

  self->epc[ep] = epcp;
  if (epcp->in_state != NULL) {
    memset(epcp->in_state, 0, sizeof(USBInEndpointState));
  }
  if (epcp->out_state != NULL) {
    memset(epcp->out_state, 0, sizeof(USBOutEndpointState));
  }

  usb_lld_init_endpoint(self, ep);
}

/**
 * @brief       Disables all active endpoints except endpoint zero.
 *
 * @param[in,out] ip            Pointer to a @p hal_usb_driver_c instance.
 *
 * @iclass
 */
void usbDisableEndpointsI(void *ip) {
  hal_usb_driver_c *self = (hal_usb_driver_c *)ip;
  unsigned i;

  osalDbgCheckClassI();
  osalDbgCheck(self != NULL);
  osalDbgAssert(self->state == USB_ACTIVE, "invalid state");

  self->transmitting &= 1U;
  self->receiving    &= 1U;
  for (i = 1U; i <= (unsigned)USB_MAX_ENDPOINTS; i++) {
#if USB_USE_WAIT == TRUE
    if (self->epc[i] != NULL) {
      if (self->epc[i]->in_state != NULL) {
        osalThreadResumeI(&self->epc[i]->in_state->thread, MSG_RESET);
      }
      if (self->epc[i]->out_state != NULL) {
        osalThreadResumeI(&self->epc[i]->out_state->thread, MSG_RESET);
      }
    }
#endif
    self->epc[i] = NULL;
  }

  usb_lld_disable_endpoints(self);
}

/**
 * @brief       Reads a setup packet from endpoint zero.
 *
 * @param[in,out] ip            Pointer to a @p hal_usb_driver_c instance.
 * @param[in]     ep            Endpoint number.
 * @param[out]    buf           Setup packet buffer.
 *
 * @iclass
 */
void usbReadSetupI(void *ip, usbep_t ep, uint8_t *buf) {
  hal_usb_driver_c *self = (hal_usb_driver_c *)ip;
  osalDbgCheckClassI();
  osalDbgCheck((self != NULL) && (buf != NULL) &&
               (ep <= (usbep_t)USB_MAX_ENDPOINTS));

  usb_lld_read_setup(self, ep, buf);
}

/**
 * @brief       Starts a receive transaction on an OUT endpoint.
 *
 * @param[in,out] ip            Pointer to a @p hal_usb_driver_c instance.
 * @param[in]     ep            Endpoint number.
 * @param[out]    buf           Receive buffer.
 * @param[in]     n             Transaction size.
 *
 * @iclass
 */
void usbStartReceiveI(void *ip, usbep_t ep, uint8_t *buf, size_t n) {
  hal_usb_driver_c *self = (hal_usb_driver_c *)ip;
  USBOutEndpointState *osp;

  osalDbgCheckClassI();
  osalDbgCheck((self != NULL) && (ep <= (usbep_t)USB_MAX_ENDPOINTS));
  osalDbgAssert((self->epc[ep] != NULL) && (self->epc[ep]->out_state != NULL),
                "endpoint not configured");
  osalDbgAssert((self->receiving &
                 (uint16_t)((unsigned)1U << (unsigned)ep)) == 0U,
                "already receiving");

  self->receiving |= (uint16_t)((unsigned)1U << (unsigned)ep);
  osp = self->epc[ep]->out_state;
  osp->rxbuf  = buf;
  osp->rxsize = n;
  osp->rxcnt  = 0U;
#if USB_USE_WAIT == TRUE
  osp->thread = NULL;
#endif

  usb_lld_start_out(self, ep);
}

/**
 * @brief       Starts a transmit transaction on an IN endpoint.
 *
 * @param[in,out] ip            Pointer to a @p hal_usb_driver_c instance.
 * @param[in]     ep            Endpoint number.
 * @param[in]     buf           Transmit buffer.
 * @param[in]     n             Transaction size.
 *
 * @iclass
 */
void usbStartTransmitI(void *ip, usbep_t ep, const uint8_t *buf, size_t n) {
  hal_usb_driver_c *self = (hal_usb_driver_c *)ip;
  USBInEndpointState *isp;

  osalDbgCheckClassI();
  osalDbgCheck((self != NULL) && (ep <= (usbep_t)USB_MAX_ENDPOINTS));
  osalDbgAssert((self->epc[ep] != NULL) && (self->epc[ep]->in_state != NULL),
                "endpoint not configured");
  osalDbgAssert((self->transmitting &
                 (uint16_t)((unsigned)1U << (unsigned)ep)) == 0U,
                "already transmitting");

  self->transmitting |= (uint16_t)((unsigned)1U << (unsigned)ep);
  isp = self->epc[ep]->in_state;
  isp->txbuf  = buf;
  isp->txsize = n;
  isp->txcnt  = 0U;
#if USB_USE_WAIT == TRUE
  isp->thread = NULL;
#endif

  usb_lld_start_in(self, ep);
}

#if (USB_USE_WAIT == TRUE) || defined (__DOXYGEN__)
/**
 * @brief       Performs a blocking receive transaction.
 *
 * @param[in,out] ip            Pointer to a @p hal_usb_driver_c instance.
 * @param[in]     ep            Endpoint number.
 * @param[out]    buf           Receive buffer.
 * @param[in]     n             Transaction size.
 * @return                      The received size or @p MSG_RESET.
 *
 * @api
 */
msg_t usbReceive(void *ip, usbep_t ep, uint8_t *buf, size_t n) {
  hal_usb_driver_c *self = (hal_usb_driver_c *)ip;
  msg_t msg;

  osalSysLock();
  if (self->state != USB_ACTIVE) {
    osalSysUnlock();
    return MSG_RESET;
  }

  usbStartReceiveI(self, ep, buf, n);
  msg = osalThreadSuspendS(&self->epc[ep]->out_state->thread);
  osalSysUnlock();

  return msg;
}

/**
 * @brief       Performs a blocking transmit transaction.
 *
 * @param[in,out] ip            Pointer to a @p hal_usb_driver_c instance.
 * @param[in]     ep            Endpoint number.
 * @param[in]     buf           Transmit buffer.
 * @param[in]     n             Transaction size.
 * @return                      The operation status.
 *
 * @api
 */
msg_t usbTransmit(void *ip, usbep_t ep, const uint8_t *buf, size_t n) {
  hal_usb_driver_c *self = (hal_usb_driver_c *)ip;
  msg_t msg;

  osalSysLock();
  if (self->state != USB_ACTIVE) {
    osalSysUnlock();
    return MSG_RESET;
  }

  usbStartTransmitI(self, ep, buf, n);
  msg = osalThreadSuspendS(&self->epc[ep]->in_state->thread);
  osalSysUnlock();

  return msg;
}
#endif /* USB_USE_WAIT == TRUE */

/**
 * @brief       Waits for a new endpoint-zero setup packet.
 *
 * @param[in,out] ip            Pointer to a @p hal_usb_driver_c instance.
 * @return                      @p MSG_OK if a setup packet is available.
 *
 * @api
 */
msg_t usbEp0WaitSetup(void *ip) {
  hal_usb_driver_c *self = (hal_usb_driver_c *)ip;
  msg_t msg;

  osalDbgCheck(self != NULL);

  osalSysLock();
  if ((self->state == USB_STOP) || (self->state == USB_UNINIT)) {
    osalSysUnlock();
    return MSG_RESET;
  }
  if (self->ep0reset != 0U) {
    self->ep0reset = 0U;
    osalSysUnlock();
    return MSG_RESET;
  }
  if (self->ep0setup != 0U) {
    self->ep0setup = 0U;
    self->ep0rseq = self->ep0seq;
    osalSysUnlock();
    return MSG_OK;
  }

  msg = osalThreadSuspendS(&self->ep0thread);
  if (msg == MSG_OK) {
    self->ep0setup = 0U;
    self->ep0rseq = self->ep0seq;
  }
  else {
    self->ep0reset = 0U;
  }
  osalSysUnlock();

  return msg;
}

/**
 * @brief       Replies to the current endpoint-zero request.
 *
 * @param[in,out] ip            Pointer to a @p hal_usb_driver_c instance.
 * @param[in]     buf           Reply buffer.
 * @param[in]     n             Reply size.
 * @return                      The operation status.
 *
 * @api
 */
msg_t usbEp0Reply(void *ip, const uint8_t *buf, size_t n) {
  hal_usb_driver_c *self = (hal_usb_driver_c *)ip;
  osalDbgCheck(self != NULL);

  return ep0_reply_or_ack(self, buf, n);
}

/**
 * @brief       Receives the data phase of the current endpoint-zero request.
 *
 * @param[in,out] ip            Pointer to a @p hal_usb_driver_c instance.
 * @param[out]    buf           Receive buffer.
 * @param[in]     n             Receive size.
 * @return                      The operation status.
 *
 * @api
 */
msg_t usbEp0Receive(void *ip, uint8_t *buf, size_t n) {
  hal_usb_driver_c *self = (hal_usb_driver_c *)ip;
  osalDbgCheck(self != NULL);

  return ep0_receive_or_status(self, buf, n);
}

/**
 * @brief       Acknowledges the current endpoint-zero request.
 *
 * @param[in,out] ip            Pointer to a @p hal_usb_driver_c instance.
 * @return                      The operation status.
 *
 * @api
 */
msg_t usbEp0Acknowledge(void *ip) {
  hal_usb_driver_c *self = (hal_usb_driver_c *)ip;
  osalDbgCheck(self != NULL);

  return ep0_receive_or_status(self, NULL, 0U);
}

/**
 * @brief       Stalls endpoint zero.
 *
 * @param[in,out] ip            Pointer to a @p hal_usb_driver_c instance.
 *
 * @api
 */
void usbEp0Stall(void *ip) {
  hal_usb_driver_c *self = (hal_usb_driver_c *)ip;
  osalDbgCheck(self != NULL);

  osalSysLock();
  usb_lld_stall_in(self, 0);
  usb_lld_stall_out(self, 0);
  self->receiving &= ~1U;
  self->transmitting &= ~1U;
  self->ep0n = 0U;
  self->ep0state = USB_EP0_ERROR;
  osalSysUnlock();

  usb_invoke_event_cb(self, USB_FLAGS_STALLED);
}

/**
 * @brief       Handles the current standard endpoint-zero request.
 *
 * @param[in,out] ip            Pointer to a @p hal_usb_driver_c instance.
 * @param[out]    handledp      Standard-request handled flag.
 * @return                      The operation status.
 *
 * @api
 */
msg_t usbEp0HandleStandardRequest(void *ip, bool *handledp) {
  hal_usb_driver_c *self = (hal_usb_driver_c *)ip;
  msg_t msg = MSG_OK;
  uint16_t type;
  uint16_t recipient;
  uint16_t request;
  const usb_descriptor_t *dp;

  osalDbgCheck((self != NULL) && (handledp != NULL));

  *handledp = false;

  if ((self->setup[0] & USB_RTYPE_TYPE_MASK) != USB_RTYPE_TYPE_STD) {
    return MSG_OK;
  }

  *handledp = true;
  recipient = self->setup[0] & USB_RTYPE_RECIPIENT_MASK;
  request = self->setup[1];
  type = recipient | (uint16_t)(request << 8U);

  switch (type) {
  case (uint32_t)USB_RTYPE_RECIPIENT_DEVICE |
       ((uint32_t)USB_REQ_GET_STATUS << 8):
    msg = usbEp0Reply(self, (const uint8_t *)&self->status, 2U);
    break;
  case (uint32_t)USB_RTYPE_RECIPIENT_DEVICE |
       ((uint32_t)USB_REQ_CLEAR_FEATURE << 8):
    if (self->setup[2] == USB_FEATURE_DEVICE_REMOTE_WAKEUP) {
      self->status &= ~2U;
      msg = usbEp0Acknowledge(self);
    }
    else {
      usbEp0Stall(self);
    }
    break;
  case (uint32_t)USB_RTYPE_RECIPIENT_DEVICE |
       ((uint32_t)USB_REQ_SET_FEATURE << 8):
    if (self->setup[2] == USB_FEATURE_DEVICE_REMOTE_WAKEUP) {
      self->status |= 2U;
      msg = usbEp0Acknowledge(self);
    }
    else {
      usbEp0Stall(self);
    }
    break;
  case (uint32_t)USB_RTYPE_RECIPIENT_DEVICE |
       ((uint32_t)USB_REQ_SET_ADDRESS << 8):
    msg = usbEp0Acknowledge(self);
    if (msg == MSG_OK) {
      set_address_thread(self);
    }
    break;
  case (uint32_t)USB_RTYPE_RECIPIENT_DEVICE |
       ((uint32_t)USB_REQ_GET_DESCRIPTOR << 8):
  case (uint32_t)USB_RTYPE_RECIPIENT_INTERFACE |
       ((uint32_t)USB_REQ_GET_DESCRIPTOR << 8):
    if (self->binder == NULL) {
      usbEp0Stall(self);
      break;
    }
    dp = usbBinderGetDescriptor(self->binder, self->setup[3],
                                self->setup[2],
                                get_hword(&self->setup[4]));
    if (dp != NULL) {
      msg = usbEp0Reply(self, dp->ud_string, dp->ud_size);
    }
    else {
      usbEp0Stall(self);
    }
    break;
  case (uint32_t)USB_RTYPE_RECIPIENT_DEVICE |
       ((uint32_t)USB_REQ_GET_CONFIGURATION << 8):
    msg = usbEp0Reply(self, &self->configuration, 1U);
    break;
  case (uint32_t)USB_RTYPE_RECIPIENT_DEVICE |
       ((uint32_t)USB_REQ_SET_CONFIGURATION << 8):
    if (self->state == USB_ACTIVE) {
      osalSysLock();
      usbDisableEndpointsI(self);
      if (self->binder != NULL) {
        usbBinderUnconfigureI(self->binder);
      }
      osalSysUnlock();
      self->configuration = 0U;
      self->state = USB_SELECTED;
      usb_invoke_event_cb(self, USB_FLAGS_UNCONFIGURED);
    }
    if (self->setup[2] != 0U) {
      self->configuration = self->setup[2];
      self->state = USB_ACTIVE;
      osalSysLock();
      if (self->binder != NULL) {
        usbBinderConfigureI(self->binder);
      }
      osalSysUnlock();
      usb_invoke_event_cb(self, USB_FLAGS_CONFIGURED);
    }
    msg = usbEp0Acknowledge(self);
    break;
  case (uint32_t)USB_RTYPE_RECIPIENT_INTERFACE |
       ((uint32_t)USB_REQ_GET_STATUS << 8):
  case (uint32_t)USB_RTYPE_RECIPIENT_ENDPOINT |
       ((uint32_t)USB_REQ_SYNCH_FRAME << 8):
    msg = usbEp0Reply(self, zero_status, 2U);
    break;
  case (uint32_t)USB_RTYPE_RECIPIENT_ENDPOINT |
       ((uint32_t)USB_REQ_GET_STATUS << 8):
    if ((self->setup[4] & 0x80U) != 0U) {
      switch (usb_lld_get_status_in(self, self->setup[4] & 0x0FU)) {
      case EP_STATUS_STALLED:
        msg = usbEp0Reply(self, halted_status, 2U);
        break;
      case EP_STATUS_ACTIVE:
        msg = usbEp0Reply(self, active_status, 2U);
        break;
      case EP_STATUS_DISABLED:
      default:
        usbEp0Stall(self);
        break;
      }
    }
    else {
      switch (usb_lld_get_status_out(self, self->setup[4] & 0x0FU)) {
      case EP_STATUS_STALLED:
        msg = usbEp0Reply(self, halted_status, 2U);
        break;
      case EP_STATUS_ACTIVE:
        msg = usbEp0Reply(self, active_status, 2U);
        break;
      case EP_STATUS_DISABLED:
      default:
        usbEp0Stall(self);
        break;
      }
    }
    break;
  case (uint32_t)USB_RTYPE_RECIPIENT_ENDPOINT |
       ((uint32_t)USB_REQ_CLEAR_FEATURE << 8):
    if (self->setup[2] == USB_FEATURE_ENDPOINT_HALT) {
      osalSysLock();
      if ((self->setup[4] & 0x0FU) != 0U) {
        if ((self->setup[4] & 0x80U) != 0U) {
          usb_lld_clear_in(self, self->setup[4] & 0x0FU);
        }
        else {
          usb_lld_clear_out(self, self->setup[4] & 0x0FU);
        }
      }
      osalSysUnlock();
      msg = usbEp0Acknowledge(self);
    }
    else {
      usbEp0Stall(self);
    }
    break;
  case (uint32_t)USB_RTYPE_RECIPIENT_ENDPOINT |
       ((uint32_t)USB_REQ_SET_FEATURE << 8):
    if (self->setup[2] == USB_FEATURE_ENDPOINT_HALT) {
      osalSysLock();
      if ((self->setup[4] & 0x0FU) != 0U) {
        if ((self->setup[4] & 0x80U) != 0U) {
          usb_lld_stall_in(self, self->setup[4] & 0x0FU);
        }
        else {
          usb_lld_stall_out(self, self->setup[4] & 0x0FU);
        }
      }
      osalSysUnlock();
      msg = usbEp0Acknowledge(self);
    }
    else {
      usbEp0Stall(self);
    }
    break;
  default:
    *handledp = false;
    break;
  }

  return msg;
}

/**
 * @brief       Stalls an OUT endpoint.
 *
 * @param[in,out] ip            Pointer to a @p hal_usb_driver_c instance.
 * @param[in]     ep            Endpoint number.
 * @return                      @p true if the endpoint is busy and was not
 *                              stalled.
 *
 * @iclass
 */
bool usbStallReceiveI(void *ip, usbep_t ep) {
  hal_usb_driver_c *self = (hal_usb_driver_c *)ip;
  osalDbgCheckClassI();
  osalDbgCheck(self != NULL);

  if (((self->receiving &
        (uint16_t)((unsigned)1U << (unsigned)ep))) != 0U) {
    return true;
  }

  usb_lld_stall_out(self, ep);
  return false;
}

/**
 * @brief       Stalls an IN endpoint.
 *
 * @param[in,out] ip            Pointer to a @p hal_usb_driver_c instance.
 * @param[in]     ep            Endpoint number.
 * @return                      @p true if the endpoint is busy and was not
 *                              stalled.
 *
 * @iclass
 */
bool usbStallTransmitI(void *ip, usbep_t ep) {
  hal_usb_driver_c *self = (hal_usb_driver_c *)ip;
  osalDbgCheckClassI();
  osalDbgCheck(self != NULL);

  if (((self->transmitting &
        (uint16_t)((unsigned)1U << (unsigned)ep))) != 0U) {
    return true;
  }

  usb_lld_stall_in(self, ep);
  return false;
}

/**
 * @brief       Starts the host wake-up procedure.
 *
 * @param[in,out] ip            Pointer to a @p hal_usb_driver_c instance.
 *
 * @api
 */
void usbWakeupHost(void *ip) {
  hal_usb_driver_c *self = (hal_usb_driver_c *)ip;
  osalDbgCheck(self != NULL);

  if (self->state == USB_SUSPENDED) {
    usb_lld_wakeup_host(self);
  }
}
/** @} */

#endif /* HAL_USE_USB == TRUE */

/** @} */
