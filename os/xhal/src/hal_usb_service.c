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
 * @file    hal_usb_service.c
 * @brief   USB services base class code.
 *
 * @addtogroup HAL_USB_SERVICE
 * @{
 */

#include "hal_usb_service.h"

#if (HAL_USE_USB == TRUE) || defined(__DOXYGEN__)

/*===========================================================================*/
/* Module local functions.                                                   */
/*===========================================================================*/

static msg_t __usbsvc_bind_impl(void *ip, hal_usb_binder_c *binderp) {
  hal_usb_service_c *self = (hal_usb_service_c *)ip;

  (void)self;
  (void)binderp;

  return HAL_RET_SUCCESS;
}

static void __usbsvc_unbind_impl(void *ip) {
  hal_usb_service_c *self = (hal_usb_service_c *)ip;

  (void)self;
}

static void __usbsvc_reset_impl(void *ip) {
  hal_usb_service_c *self = (hal_usb_service_c *)ip;

  (void)self;
}

static void __usbsvc_configure_impl(void *ip) {
  hal_usb_service_c *self = (hal_usb_service_c *)ip;

  (void)self;
}

static void __usbsvc_unconfigure_impl(void *ip) {
  hal_usb_service_c *self = (hal_usb_service_c *)ip;

  (void)self;
}

static void __usbsvc_suspend_impl(void *ip) {
  hal_usb_service_c *self = (hal_usb_service_c *)ip;

  (void)self;
}

static void __usbsvc_wakeup_impl(void *ip) {
  hal_usb_service_c *self = (hal_usb_service_c *)ip;

  (void)self;
}

static void __usbsvc_sof_impl(void *ip) {
  hal_usb_service_c *self = (hal_usb_service_c *)ip;

  (void)self;
}

static void __usbsvc_in_impl(void *ip, usbep_t ep) {
  hal_usb_service_c *self = (hal_usb_service_c *)ip;

  (void)self;
  (void)ep;
}

static void __usbsvc_out_impl(void *ip, usbep_t ep) {
  hal_usb_service_c *self = (hal_usb_service_c *)ip;

  (void)self;
  (void)ep;
}

static msg_t __usbsvc_setup_impl(void *ip, bool *handledp) {
  hal_usb_service_c *self = (hal_usb_service_c *)ip;

  (void)self;

  if (handledp != NULL) {
    *handledp = false;
  }

  return HAL_RET_SUCCESS;
}

/*===========================================================================*/
/* Module exported functions.                                                */
/*===========================================================================*/

void *__usbsvc_objinit_impl(void *ip, const void *vmt,
                            const hal_usb_service_info_t *infop) {
  hal_usb_service_c *self = (hal_usb_service_c *)ip;

  __bo_objinit_impl(self, vmt);

  self->binder = NULL;
  self->info = infop;
  self->arg  = NULL;
  self->next = NULL;
  self->prev = NULL;

  return self;
}

void __usbsvc_dispose_impl(void *ip) {
  hal_usb_service_c *self = (hal_usb_service_c *)ip;

  (void)self;

  __bo_dispose_impl(self);
}

msg_t usbServiceBind(void *ip, hal_usb_binder_c *binderp) {
  hal_usb_service_c *self = (hal_usb_service_c *)ip;
  msg_t msg;

  osalDbgCheck((self != NULL) && (binderp != NULL));
  osalDbgAssert(self->binder == NULL, "already bound");

  self->binder = binderp;
  msg = self->vmt->bind(self, binderp);
  if (msg != HAL_RET_SUCCESS) {
    self->binder = NULL;
  }

  return msg;
}

void usbServiceUnbind(void *ip) {
  hal_usb_service_c *self = (hal_usb_service_c *)ip;

  osalDbgCheck(self != NULL);

  self->vmt->unbind(self);
  self->binder = NULL;
}

void usbServiceResetI(void *ip) {
  hal_usb_service_c *self = (hal_usb_service_c *)ip;

  osalDbgCheckClassI();
  osalDbgCheck(self != NULL);

  self->vmt->reset(self);
}

void usbServiceConfigureI(void *ip) {
  hal_usb_service_c *self = (hal_usb_service_c *)ip;

  osalDbgCheckClassI();
  osalDbgCheck(self != NULL);

  self->vmt->configure(self);
}

void usbServiceUnconfigureI(void *ip) {
  hal_usb_service_c *self = (hal_usb_service_c *)ip;

  osalDbgCheckClassI();
  osalDbgCheck(self != NULL);

  self->vmt->unconfigure(self);
}

void usbServiceSuspendI(void *ip) {
  hal_usb_service_c *self = (hal_usb_service_c *)ip;

  osalDbgCheckClassI();
  osalDbgCheck(self != NULL);

  self->vmt->suspend(self);
}

void usbServiceWakeupI(void *ip) {
  hal_usb_service_c *self = (hal_usb_service_c *)ip;

  osalDbgCheckClassI();
  osalDbgCheck(self != NULL);

  self->vmt->wakeup(self);
}

void usbServiceSOFI(void *ip) {
  hal_usb_service_c *self = (hal_usb_service_c *)ip;

  osalDbgCheckClassI();
  osalDbgCheck(self != NULL);

  self->vmt->sof(self);
}

void usbServiceInI(void *ip, usbep_t ep) {
  hal_usb_service_c *self = (hal_usb_service_c *)ip;

  osalDbgCheckClassI();
  osalDbgCheck((self != NULL) && (ep <= (usbep_t)USB_MAX_ENDPOINTS));

  self->vmt->in(self, ep);
}

void usbServiceOutI(void *ip, usbep_t ep) {
  hal_usb_service_c *self = (hal_usb_service_c *)ip;

  osalDbgCheckClassI();
  osalDbgCheck((self != NULL) && (ep <= (usbep_t)USB_MAX_ENDPOINTS));

  self->vmt->out(self, ep);
}

msg_t usbServiceSetup(void *ip, bool *handledp) {
  hal_usb_service_c *self = (hal_usb_service_c *)ip;

  osalDbgCheck((self != NULL) && (handledp != NULL));

  return self->vmt->setup(self, handledp);
}

/*===========================================================================*/
/* Module exported variables.                                                */
/*===========================================================================*/

const struct hal_usb_service_vmt __hal_usb_service_vmt = {
  .dispose                  = __usbsvc_dispose_impl,
  .bind                     = __usbsvc_bind_impl,
  .unbind                   = __usbsvc_unbind_impl,
  .reset                    = __usbsvc_reset_impl,
  .configure                = __usbsvc_configure_impl,
  .unconfigure              = __usbsvc_unconfigure_impl,
  .suspend                  = __usbsvc_suspend_impl,
  .wakeup                   = __usbsvc_wakeup_impl,
  .sof                      = __usbsvc_sof_impl,
  .in                       = __usbsvc_in_impl,
  .out                      = __usbsvc_out_impl,
  .setup                    = __usbsvc_setup_impl
};

/** @} */

#endif /* HAL_USE_USB == TRUE */
