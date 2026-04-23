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
 * @file        hal_usb_cdc.c
 * @brief       Generated USB CDC Services source.
 * @note        This is a generated file, do not edit directly.
 *
 * @addtogroup  HAL_USB_CDC
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

static void cdcsvc_set_default_linecoding(hal_usb_cdc_service_c *self) {
  self->linecoding.dwDTERate[0] = (uint8_t)(CDC_DEFAULT_DTE_RATE);
  self->linecoding.dwDTERate[1] = (uint8_t)(CDC_DEFAULT_DTE_RATE >> 8);
  self->linecoding.dwDTERate[2] = (uint8_t)(CDC_DEFAULT_DTE_RATE >> 16);
  self->linecoding.dwDTERate[3] = (uint8_t)(CDC_DEFAULT_DTE_RATE >> 24);
  self->linecoding.bCharFormat = CDC_DEFAULT_CHAR_FORMAT;
  self->linecoding.bParityType = CDC_DEFAULT_PARITY;
  self->linecoding.bDataBits = CDC_DEFAULT_DATA_BITS;
}

static void cdcsvc_endpoint_in_cb(hal_usb_driver_c *usbp, usbep_t ep) {
  hal_usb_binder_c *binderp;

  binderp = usbGetBinderX(usbp);
  if (binderp != NULL) {
    osalSysLockFromISR();
    usbBinderInI(binderp, ep);
    osalSysUnlockFromISR();
  }
}

static void cdcsvc_endpoint_out_cb(hal_usb_driver_c *usbp, usbep_t ep) {
  hal_usb_binder_c *binderp;

  binderp = usbGetBinderX(usbp);
  if (binderp != NULL) {
    osalSysLockFromISR();
    usbBinderOutI(binderp, ep);
    osalSysUnlockFromISR();
  }
}

/*===========================================================================*/
/* Module exported functions.                                                */
/*===========================================================================*/

/**
 * @brief       Starts a CDC service instance.
 *
 * @param[in,out] ip            Pointer to a @p hal_usb_cdc_service_c instance.
 * @param[in]     config        CDC service configuration.
 * @return                      The operation status.
 *
 * @api
 */
msg_t usbCdcServiceStart(void *ip, const hal_usb_cdc_config_t *config) {
  hal_usb_cdc_service_c *self = (hal_usb_cdc_service_c *)ip;
  hal_usb_driver_c *usbp;

  osalDbgCheck((self != NULL) && (config != NULL) && (config->usbp != NULL));
  osalDbgCheck((config->bulk_in > 0U) &&
               (config->bulk_in <= (usbep_t)USB_MAX_ENDPOINTS));
  osalDbgCheck((config->bulk_out > 0U) &&
               (config->bulk_out <= (usbep_t)USB_MAX_ENDPOINTS));
  osalDbgCheck(config->bulk_in_maxsize > 0U);
  osalDbgCheck(config->bulk_out_maxsize > 0U);
  if (config->int_in > 0U) {
    osalDbgCheck((config->int_in <= (usbep_t)USB_MAX_ENDPOINTS) &&
                 (config->int_in_maxsize > 0U));
  }
  osalDbgAssert(usbServiceGetBinderX(self) == NULL, "service bound");

  osalSysLock();
  osalDbgAssert((self->state == USB_CDC_SERVICE_STOP) ||
                (self->state == USB_CDC_SERVICE_READY), "invalid state");

  usbp = config->usbp;
  usbp->in_params[config->bulk_in - 1U] = self;
  usbp->out_params[config->bulk_out - 1U] = self;
  if (config->int_in > 0U) {
    usbp->in_params[config->int_in - 1U] = self;
  }

  self->bulk_in_epc.in_maxsize = config->bulk_in_maxsize;
  self->bulk_out_epc.out_maxsize = config->bulk_out_maxsize;
  self->int_in_epc.in_maxsize = config->int_in_maxsize;

  if (config->bulk_in == config->bulk_out) {
    self->bulk_in_epc.out_cb = cdcsvc_endpoint_out_cb;
    self->bulk_in_epc.out_state = &self->bulk_out_state;
    self->bulk_in_epc.out_maxsize = config->bulk_out_maxsize;
  }
  else {
    self->bulk_in_epc.out_cb = NULL;
    self->bulk_in_epc.out_state = NULL;
    self->bulk_in_epc.out_maxsize = 0U;
  }

  self->config = config;
  self->connected = false;
  self->svcinfo.if_base = config->control_if;
  self->svcinfo.if_count = 2U;
  self->svcinfo.in_ep_mask =
      (uint16_t)((uint16_t)1U << (unsigned)config->bulk_in);
  if (config->int_in > 0U) {
    self->svcinfo.in_ep_mask |=
        (uint16_t)((uint16_t)1U << (unsigned)config->int_in);
  }
  self->svcinfo.out_ep_mask =
      (uint16_t)((uint16_t)1U << (unsigned)config->bulk_out);
  self->state = USB_CDC_SERVICE_READY;
  osalSysUnlock();

  return HAL_RET_SUCCESS;
}

/**
 * @brief       Stops a CDC service instance.
 *
 * @param[in,out] ip            Pointer to a @p hal_usb_cdc_service_c instance.
 *
 * @api
 */
void usbCdcServiceStop(void *ip) {
  hal_usb_cdc_service_c *self = (hal_usb_cdc_service_c *)ip;
  hal_usb_driver_c *usbp = NULL;

  osalDbgCheck(self != NULL);
  osalDbgAssert(usbServiceGetBinderX(self) == NULL, "service bound");

  osalSysLock();
  osalDbgAssert((self->state == USB_CDC_SERVICE_STOP) ||
                (self->state == USB_CDC_SERVICE_READY), "invalid state");

  if (self->config != NULL) {
    usbp = self->config->usbp;
    usbp->in_params[self->config->bulk_in - 1U] = NULL;
    usbp->out_params[self->config->bulk_out - 1U] = NULL;
    if (self->config->int_in > 0U) {
      usbp->in_params[self->config->int_in - 1U] = NULL;
    }
  }

  self->config = NULL;
  self->connected = false;
  self->svcinfo.if_base = 0U;
  self->svcinfo.if_count = 0U;
  self->svcinfo.in_ep_mask = 0U;
  self->svcinfo.out_ep_mask = 0U;
  self->state = USB_CDC_SERVICE_STOP;
  osalSysUnlock();
}

/**
 * @brief       Returns the current CDC line coding structure.
 *
 * @param[in,out] ip            Pointer to a @p hal_usb_cdc_service_c instance.
 * @return                      Pointer to the current line coding structure.
 *
 * @xclass
 */
const usb_cdc_linecoding_t *usbCdcServiceGetLineCodingX(void *ip) {
  hal_usb_cdc_service_c *self = (hal_usb_cdc_service_c *)ip;

  osalDbgCheck(self != NULL);

  return &self->linecoding;
}

/**
 * @brief       Returns the current control line state bitmap.
 *
 * @param[in,out] ip            Pointer to a @p hal_usb_cdc_service_c instance.
 * @return                      The current control line state bitmap.
 *
 * @xclass
 */
uint16_t usbCdcServiceGetControlLineStateX(void *ip) {
  hal_usb_cdc_service_c *self = (hal_usb_cdc_service_c *)ip;

  osalDbgCheck(self != NULL);

  return self->control_line_state;
}

/**
 * @brief       Returns the active CDC configuration.
 *
 * @param[in,out] ip            Pointer to a @p hal_usb_cdc_service_c instance.
 * @return                      The active CDC configuration or @p NULL.
 *
 * @xclass
 */
const hal_usb_cdc_config_t *usbCdcServiceGetConfigX(void *ip) {
  hal_usb_cdc_service_c *self = (hal_usb_cdc_service_c *)ip;

  osalDbgCheck(self != NULL);

  return self->config;
}

/**
 * @brief       Returns the current logical connection state.
 *
 * @param[in,out] ip            Pointer to a @p hal_usb_cdc_service_c instance.
 * @return                      The current logical connection state.
 *
 * @xclass
 */
bool usbCdcServiceIsConnectedX(void *ip) {
  hal_usb_cdc_service_c *self = (hal_usb_cdc_service_c *)ip;

  osalDbgCheck(self != NULL);

  return self->connected;
}

/*===========================================================================*/
/* Module class "hal_usb_cdc_service_c" methods.                             */
/*===========================================================================*/

/**
 * @name        Methods implementations of hal_usb_cdc_service_c
 * @{
 */
/**
 * @brief       Implementation of object creation.
 * @note        This function is meant to be used by derived classes.
 *
 * @param[out]    ip            Pointer to a @p hal_usb_cdc_service_c instance
 *                              to be initialized.
 * @param[in]     vmt           VMT pointer for the new object.
 * @return                      A new reference to the object.
 */
void *__cdcsvc_objinit_impl(void *ip, const void *vmt) {
  hal_usb_cdc_service_c *self = (hal_usb_cdc_service_c *)ip;

  /* Initialization code.*/
  usbServiceObjectInit((void *)self, &self->svcinfo,
                       (const struct hal_usb_service_vmt *)vmt);
  self->config = NULL;
  self->state = USB_CDC_SERVICE_STOP;
  self->connected = false;
  self->control_line_state = 0U;
  memset(&self->bulk_in_state, 0, sizeof(self->bulk_in_state));
  memset(&self->bulk_out_state, 0, sizeof(self->bulk_out_state));
  memset(&self->int_in_state, 0, sizeof(self->int_in_state));
  memset(&self->bulk_in_epc, 0, sizeof(self->bulk_in_epc));
  memset(&self->bulk_out_epc, 0, sizeof(self->bulk_out_epc));
  memset(&self->int_in_epc, 0, sizeof(self->int_in_epc));

  self->bulk_in_epc.ep_mode    = USB_EP_MODE_TYPE_BULK;
  self->bulk_in_epc.in_cb      = cdcsvc_endpoint_in_cb;
  self->bulk_in_epc.in_state   = &self->bulk_in_state;

  self->bulk_out_epc.ep_mode   = USB_EP_MODE_TYPE_BULK;
  self->bulk_out_epc.out_cb    = cdcsvc_endpoint_out_cb;
  self->bulk_out_epc.out_state = &self->bulk_out_state;

  self->int_in_epc.ep_mode     = USB_EP_MODE_TYPE_INTR;
  self->int_in_epc.in_cb       = cdcsvc_endpoint_in_cb;
  self->int_in_epc.in_state    = &self->int_in_state;

  self->svcinfo.if_base = 0U;
  self->svcinfo.if_count = 0U;
  self->svcinfo.in_ep_mask = 0U;
  self->svcinfo.out_ep_mask = 0U;
  cdcsvc_set_default_linecoding(self);

  return self;
}

/**
 * @brief       Implementation of object finalization.
 * @note        This function is meant to be used by derived classes.
 *
 * @param[in,out] ip            Pointer to a @p hal_usb_cdc_service_c instance
 *                              to be disposed.
 */
void __cdcsvc_dispose_impl(void *ip) {
  hal_usb_cdc_service_c *self = (hal_usb_cdc_service_c *)ip;

  /* No finalization code.*/
  (void)self;

  /* Finalization of the ancestors-defined parts.*/
  __usbsvc_dispose_impl(self);
}

/**
 * @brief       Override of method @p usbServiceOnBind().
 *
 * @param[in,out] ip            Pointer to a @p hal_usb_cdc_service_c instance.
 * @param[in]     binderp       Binder owning the service.
 * @return                      The operation status.
 */
msg_t __cdcsvc_bind_impl(void *ip, struct hal_usb_binder *binderp) {
  hal_usb_cdc_service_c *self = (hal_usb_cdc_service_c *)ip;
  if ((self->config == NULL) || (binderp == NULL)) {
    return HAL_RET_INV_STATE;
  }
  if (binderp->usbp != self->config->usbp) {
    return HAL_RET_INV_STATE;
  }

  return HAL_RET_SUCCESS;
}

/**
 * @brief       Override of method @p usbServiceOnUnbind().
 *
 * @param[in,out] ip            Pointer to a @p hal_usb_cdc_service_c instance.
 */
void __cdcsvc_unbind_impl(void *ip) {
  hal_usb_cdc_service_c *self = (hal_usb_cdc_service_c *)ip;
  self->connected = false;
}

/**
 * @brief       Override of method @p usbServiceOnResetI().
 *
 * @param[in,out] ip            Pointer to a @p hal_usb_cdc_service_c instance.
 */
void __cdcsvc_reset_impl(void *ip) {
  hal_usb_cdc_service_c *self = (hal_usb_cdc_service_c *)ip;
  self->connected = false;
  usbCdcServiceOnDisconnectedI(self);
}

/**
 * @brief       Override of method @p usbServiceOnConfigureI().
 *
 * @param[in,out] ip            Pointer to a @p hal_usb_cdc_service_c instance.
 */
void __cdcsvc_configure_impl(void *ip) {
  hal_usb_cdc_service_c *self = (hal_usb_cdc_service_c *)ip;
  osalDbgAssert(self->config != NULL, "service not started");

  usbInitEndpointI(self->config->usbp, self->config->bulk_in,
                   &self->bulk_in_epc);
  if (self->config->bulk_out != self->config->bulk_in) {
    usbInitEndpointI(self->config->usbp, self->config->bulk_out,
                     &self->bulk_out_epc);
  }
  if (self->config->int_in > 0U) {
    usbInitEndpointI(self->config->usbp, self->config->int_in,
                     &self->int_in_epc);
  }
  self->connected = true;
  usbCdcServiceOnConfiguredI(self);
}

/**
 * @brief       Override of method @p usbServiceOnUnconfigureI().
 *
 * @param[in,out] ip            Pointer to a @p hal_usb_cdc_service_c instance.
 */
void __cdcsvc_unconfigure_impl(void *ip) {
  hal_usb_cdc_service_c *self = (hal_usb_cdc_service_c *)ip;
  self->connected = false;
  usbCdcServiceOnDisconnectedI(self);
}

/**
 * @brief       Override of method @p usbServiceOnSuspendI().
 *
 * @param[in,out] ip            Pointer to a @p hal_usb_cdc_service_c instance.
 */
void __cdcsvc_suspend_impl(void *ip) {
  hal_usb_cdc_service_c *self = (hal_usb_cdc_service_c *)ip;
  self->connected = false;
  usbCdcServiceOnDisconnectedI(self);
}

/**
 * @brief       Override of method @p usbServiceOnWakeupI().
 *
 * @param[in,out] ip            Pointer to a @p hal_usb_cdc_service_c instance.
 */
void __cdcsvc_wakeup_impl(void *ip) {
  hal_usb_cdc_service_c *self = (hal_usb_cdc_service_c *)ip;
  self->connected = true;
  usbCdcServiceOnWakeupHookI(self);
}

/**
 * @brief       Override of method @p usbServiceOnSOFI().
 *
 * @param[in,out] ip            Pointer to a @p hal_usb_cdc_service_c instance.
 */
void __cdcsvc_sof_impl(void *ip) {
  hal_usb_cdc_service_c *self = (hal_usb_cdc_service_c *)ip;
  usbCdcServiceOnSOFHookI(self);
}

/**
 * @brief       Override of method @p usbServiceOnInI().
 *
 * @param[in,out] ip            Pointer to a @p hal_usb_cdc_service_c instance.
 * @param[in]     ep            Endpoint number.
 */
void __cdcsvc_in_impl(void *ip, usbep_t ep) {
  hal_usb_cdc_service_c *self = (hal_usb_cdc_service_c *)ip;
  if ((self->config != NULL) && (ep == self->config->bulk_in)) {
    usbCdcServiceOnDataTransmittedI(self, ep);
  }
  else if ((self->config != NULL) && (self->config->int_in > 0U) &&
           (ep == self->config->int_in)) {
    usbCdcServiceOnInterruptTransmittedI(self, ep);
  }
}

/**
 * @brief       Override of method @p usbServiceOnOutI().
 *
 * @param[in,out] ip            Pointer to a @p hal_usb_cdc_service_c instance.
 * @param[in]     ep            Endpoint number.
 */
void __cdcsvc_out_impl(void *ip, usbep_t ep) {
  hal_usb_cdc_service_c *self = (hal_usb_cdc_service_c *)ip;
  if ((self->config != NULL) && (ep == self->config->bulk_out)) {
    usbCdcServiceOnDataReceivedI(self, ep);
  }
}

/**
 * @brief       Override of method @p usbServiceOnSetup().
 *
 * @param[in,out] ip            Pointer to a @p hal_usb_cdc_service_c instance.
 * @param[out]    handledp      Handled flag.
 * @return                      The operation status.
 */
msg_t __cdcsvc_setup_impl(void *ip, bool *handledp) {
  hal_usb_cdc_service_c *self = (hal_usb_cdc_service_c *)ip;
  uint8_t *setup;

  osalDbgAssert((handledp != NULL) && (self->config != NULL),
                "invalid setup context");

  *handledp = false;
  setup = self->config->usbp->setup;
  if ((setup[0] & USB_RTYPE_TYPE_MASK) != USB_RTYPE_TYPE_CLASS) {
    return HAL_RET_SUCCESS;
  }
  if ((setup[0] & USB_RTYPE_RECIPIENT_MASK) != USB_RTYPE_RECIPIENT_INTERFACE) {
    return HAL_RET_SUCCESS;
  }
  if (setup[4] != self->config->control_if) {
    return HAL_RET_SUCCESS;
  }

  switch (setup[1]) {
  case CDC_GET_LINE_CODING:
    *handledp = true;
    return usbEp0Reply(self->config->usbp,
                       (const uint8_t *)&self->linecoding,
                       sizeof(self->linecoding));
  case CDC_SET_LINE_CODING:
    *handledp = true;
    return usbEp0Receive(self->config->usbp,
                         (uint8_t *)&self->linecoding,
                         sizeof(self->linecoding));
  case CDC_SET_CONTROL_LINE_STATE:
    *handledp = true;
    self->control_line_state = (uint16_t)setup[2] |
                               ((uint16_t)setup[3] << 8U);
    usbCdcServiceOnControlLineState(self);
    return usbEp0Acknowledge(self->config->usbp);
  default:
    break;
  }

  return HAL_RET_SUCCESS;
}

/**
 * @brief       Implementation of method @p usbCdcServiceOnConfiguredI().
 * @note        This function is meant to be used by derived classes.
 *
 * @param[in,out] ip            Pointer to a @p hal_usb_cdc_service_c instance.
 */
void __cdcsvc_configured_impl(void *ip) {
  hal_usb_cdc_service_c *self = (hal_usb_cdc_service_c *)ip;
  (void)self;
}

/**
 * @brief       Implementation of method @p usbCdcServiceOnDisconnectedI().
 * @note        This function is meant to be used by derived classes.
 *
 * @param[in,out] ip            Pointer to a @p hal_usb_cdc_service_c instance.
 */
void __cdcsvc_disconnected_impl(void *ip) {
  hal_usb_cdc_service_c *self = (hal_usb_cdc_service_c *)ip;
  (void)self;
}

/**
 * @brief       Implementation of method @p usbCdcServiceOnWakeupHookI().
 * @note        This function is meant to be used by derived classes.
 *
 * @param[in,out] ip            Pointer to a @p hal_usb_cdc_service_c instance.
 */
void __cdcsvc_wakehook_impl(void *ip) {
  hal_usb_cdc_service_c *self = (hal_usb_cdc_service_c *)ip;
  (void)self;
}

/**
 * @brief       Implementation of method @p usbCdcServiceOnSOFHookI().
 * @note        This function is meant to be used by derived classes.
 *
 * @param[in,out] ip            Pointer to a @p hal_usb_cdc_service_c instance.
 */
void __cdcsvc_sofhook_impl(void *ip) {
  hal_usb_cdc_service_c *self = (hal_usb_cdc_service_c *)ip;
  (void)self;
}

/**
 * @brief       Implementation of method @p usbCdcServiceOnDataTransmittedI().
 * @note        This function is meant to be used by derived classes.
 *
 * @param[in,out] ip            Pointer to a @p hal_usb_cdc_service_c instance.
 * @param[in]     ep            Endpoint number.
 */
void __cdcsvc_data_transmitted_impl(void *ip, usbep_t ep) {
  hal_usb_cdc_service_c *self = (hal_usb_cdc_service_c *)ip;
  (void)self;
  (void)ep;
}

/**
 * @brief       Implementation of method @p usbCdcServiceOnDataReceivedI().
 * @note        This function is meant to be used by derived classes.
 *
 * @param[in,out] ip            Pointer to a @p hal_usb_cdc_service_c instance.
 * @param[in]     ep            Endpoint number.
 */
void __cdcsvc_data_received_impl(void *ip, usbep_t ep) {
  hal_usb_cdc_service_c *self = (hal_usb_cdc_service_c *)ip;
  (void)self;
  (void)ep;
}

/**
 * @brief       Implementation of method @p
 *              usbCdcServiceOnInterruptTransmittedI().
 * @note        This function is meant to be used by derived classes.
 *
 * @param[in,out] ip            Pointer to a @p hal_usb_cdc_service_c instance.
 * @param[in]     ep            Endpoint number.
 */
void __cdcsvc_irq_transmitted_impl(void *ip, usbep_t ep) {
  hal_usb_cdc_service_c *self = (hal_usb_cdc_service_c *)ip;
  (void)self;
  (void)ep;
}

/**
 * @brief       Implementation of method @p usbCdcServiceOnControlLineState().
 * @note        This function is meant to be used by derived classes.
 *
 * @param[in,out] ip            Pointer to a @p hal_usb_cdc_service_c instance.
 */
void __cdcsvc_control_line_state_impl(void *ip) {
  hal_usb_cdc_service_c *self = (hal_usb_cdc_service_c *)ip;
  (void)self;
}
/** @} */

#endif /* HAL_USE_USB == TRUE */

/** @} */
