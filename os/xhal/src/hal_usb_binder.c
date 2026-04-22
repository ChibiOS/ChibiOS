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
 * @file    hal_usb_binder.c
 * @brief   USB binders base class code.
 *
 * @addtogroup HAL_USB_BINDER
 * @{
 */

#include "hal_usb_binder.h"
#include "hal_usb_service.h"

#if (HAL_USE_USB == TRUE) || defined(__DOXYGEN__)

/*===========================================================================*/
/* Module local functions.                                                   */
/*===========================================================================*/

static msg_t __usbbnd_bind_impl(void *ip, hal_usb_driver_c *usbp) {
  hal_usb_binder_c *self = (hal_usb_binder_c *)ip;

  (void)self;
  (void)usbp;

  return HAL_RET_SUCCESS;
}

static void __usbbnd_unbind_impl(void *ip) {
  hal_usb_binder_c *self = (hal_usb_binder_c *)ip;

  (void)self;
}

static const usb_descriptor_t * __usbbnd_get_descriptor_impl(void *ip,
                                                             uint8_t dtype,
                                                             uint8_t dindex,
                                                             uint16_t lang) {
  hal_usb_binder_c *self = (hal_usb_binder_c *)ip;

  (void)self;
  (void)dtype;
  (void)dindex;
  (void)lang;

  return NULL;
}

static void __usbbnd_reset_impl(void *ip) {
  hal_usb_binder_c *self = (hal_usb_binder_c *)ip;
  hal_usb_service_c *servicep;

  for (servicep = self->services; servicep != NULL; servicep = servicep->next) {
    usbServiceResetI(servicep);
  }
}

static void __usbbnd_configure_impl(void *ip) {
  hal_usb_binder_c *self = (hal_usb_binder_c *)ip;
  hal_usb_service_c *servicep;

  for (servicep = self->services; servicep != NULL; servicep = servicep->next) {
    usbServiceConfigureI(servicep);
  }
}

static void __usbbnd_unconfigure_impl(void *ip) {
  hal_usb_binder_c *self = (hal_usb_binder_c *)ip;
  hal_usb_service_c *servicep;

  for (servicep = self->services; servicep != NULL; servicep = servicep->next) {
    usbServiceUnconfigureI(servicep);
  }
}

static void __usbbnd_suspend_impl(void *ip) {
  hal_usb_binder_c *self = (hal_usb_binder_c *)ip;
  hal_usb_service_c *servicep;

  for (servicep = self->services; servicep != NULL; servicep = servicep->next) {
    usbServiceSuspendI(servicep);
  }
}

static void __usbbnd_wakeup_impl(void *ip) {
  hal_usb_binder_c *self = (hal_usb_binder_c *)ip;
  hal_usb_service_c *servicep;

  for (servicep = self->services; servicep != NULL; servicep = servicep->next) {
    usbServiceWakeupI(servicep);
  }
}

static void __usbbnd_sof_impl(void *ip) {
  hal_usb_binder_c *self = (hal_usb_binder_c *)ip;
  hal_usb_service_c *servicep;

  for (servicep = self->services; servicep != NULL; servicep = servicep->next) {
    usbServiceSOFI(servicep);
  }
}

static void __usbbnd_in_impl(void *ip, usbep_t ep) {
  hal_usb_binder_c *self = (hal_usb_binder_c *)ip;
  hal_usb_service_c *servicep;

  for (servicep = self->services; servicep != NULL; servicep = servicep->next) {
    if (usbServiceOwnsInEndpointX(servicep, ep)) {
      usbServiceInI(servicep, ep);
      return;
    }
  }
}

static void __usbbnd_out_impl(void *ip, usbep_t ep) {
  hal_usb_binder_c *self = (hal_usb_binder_c *)ip;
  hal_usb_service_c *servicep;

  for (servicep = self->services; servicep != NULL; servicep = servicep->next) {
    if (usbServiceOwnsOutEndpointX(servicep, ep)) {
      usbServiceOutI(servicep, ep);
      return;
    }
  }
}

static msg_t __usbbnd_setup_impl(void *ip, bool *handledp) {
  hal_usb_binder_c *self = (hal_usb_binder_c *)ip;
  hal_usb_service_c *servicep;

  *handledp = false;
  for (servicep = self->services; servicep != NULL; servicep = servicep->next) {
    msg_t msg;
    bool handled;

    handled = false;
    msg = usbServiceSetup(servicep, &handled);
    if ((msg != HAL_RET_SUCCESS) || handled) {
      *handledp = handled;
      return msg;
    }
  }

  return HAL_RET_SUCCESS;
}

static bool usbbnd_if_conflict(const hal_usb_service_info_t *ap,
                               const hal_usb_service_info_t *bp) {
  unsigned abase, alimit, bbase, blimit;

  if ((ap->if_count == 0U) || (bp->if_count == 0U)) {
    return false;
  }

  abase  = (unsigned)ap->if_base;
  alimit = abase + (unsigned)ap->if_count;
  bbase  = (unsigned)bp->if_base;
  blimit = bbase + (unsigned)bp->if_count;

  return (abase < blimit) && (bbase < alimit);
}

static bool usbbnd_service_conflict(const hal_usb_service_info_t *ap,
                                    const hal_usb_service_info_t *bp) {
  if (usbbnd_if_conflict(ap, bp)) {
    return true;
  }
  if ((ap->in_ep_mask & bp->in_ep_mask) != 0U) {
    return true;
  }
  if ((ap->out_ep_mask & bp->out_ep_mask) != 0U) {
    return true;
  }

  return false;
}

/*===========================================================================*/
/* Module exported functions.                                                */
/*===========================================================================*/

void *__usbbnd_objinit_impl(void *ip, const void *vmt) {
  hal_usb_binder_c *self = (hal_usb_binder_c *)ip;

  __bo_objinit_impl(self, vmt);

  self->usbp         = NULL;
  self->services     = NULL;
  self->service_last = NULL;
  self->services_num = 0U;
  self->arg          = NULL;

  return self;
}

void __usbbnd_dispose_impl(void *ip) {
  hal_usb_binder_c *self = (hal_usb_binder_c *)ip;

  (void)self;

  __bo_dispose_impl(self);
}

msg_t usbBinderBind(void *ip, hal_usb_driver_c *usbp) {
  hal_usb_binder_c *self = (hal_usb_binder_c *)ip;
  hal_usb_service_c *servicep;
  msg_t msg;

  osalDbgCheck((self != NULL) && (usbp != NULL));
  osalDbgAssert(self->usbp == NULL, "already bound");

  self->usbp = usbp;
  msg = self->vmt->bind(self, usbp);
  if (msg != HAL_RET_SUCCESS) {
    self->usbp = NULL;
    return msg;
  }

  for (servicep = self->services; servicep != NULL; servicep = servicep->next) {
    msg = usbServiceBind(servicep, self);
    if (msg != HAL_RET_SUCCESS) {
      hal_usb_service_c *sp;

      for (sp = self->services; sp != servicep; sp = sp->next) {
        usbServiceUnbind(sp);
      }
      self->vmt->unbind(self);
      self->usbp = NULL;
      return msg;
    }
  }

  return HAL_RET_SUCCESS;
}

void usbBinderUnbind(void *ip) {
  hal_usb_binder_c *self = (hal_usb_binder_c *)ip;
  hal_usb_service_c *servicep;

  osalDbgCheck(self != NULL);

  for (servicep = self->services; servicep != NULL; servicep = servicep->next) {
    if (usbServiceGetBinderX(servicep) == self) {
      usbServiceUnbind(servicep);
    }
  }

  self->vmt->unbind(self);
  self->usbp = NULL;
}

msg_t usbBinderRegisterService(void *ip, hal_usb_service_c *servicep) {
  hal_usb_binder_c *self = (hal_usb_binder_c *)ip;
  hal_usb_service_c *sp;

  osalDbgCheck((self != NULL) && (servicep != NULL));
  osalDbgAssert(self->usbp == NULL, "registration while bound");
  osalDbgAssert(usbServiceGetBinderX(servicep) == NULL, "service already bound");
  osalDbgAssert((servicep->next == NULL) && (servicep->prev == NULL),
                "service already linked");
  osalDbgAssert(usbServiceGetInfoX(servicep) != NULL, "missing service info");

  for (sp = self->services; sp != NULL; sp = sp->next) {
    osalDbgAssert(sp != servicep, "service already registered");
    if (usbbnd_service_conflict(usbServiceGetInfoX(servicep),
                                usbServiceGetInfoX(sp))) {
      return HAL_RET_INV_STATE;
    }
  }

  servicep->prev = self->service_last;
  servicep->next = NULL;
  if (self->service_last != NULL) {
    self->service_last->next = servicep;
  }
  else {
    self->services = servicep;
  }
  self->service_last = servicep;
  self->services_num++;

  return HAL_RET_SUCCESS;
}

msg_t usbBinderUnregisterService(void *ip, hal_usb_service_c *servicep) {
  hal_usb_binder_c *self = (hal_usb_binder_c *)ip;
  hal_usb_service_c *sp;

  osalDbgCheck((self != NULL) && (servicep != NULL));
  osalDbgAssert(self->usbp == NULL, "unregistration while bound");

  for (sp = self->services; sp != NULL; sp = sp->next) {
    if (sp == servicep) {
      break;
    }
  }
  if (sp == NULL) {
    return HAL_RET_INV_STATE;
  }

  if (servicep->prev != NULL) {
    servicep->prev->next = servicep->next;
  }
  else {
    self->services = servicep->next;
  }
  if (servicep->next != NULL) {
    servicep->next->prev = servicep->prev;
  }
  else {
    self->service_last = servicep->prev;
  }
  servicep->next = NULL;
  servicep->prev = NULL;
  self->services_num--;

  return HAL_RET_SUCCESS;
}

hal_usb_service_c *usbBinderGetNextServiceX(void *ip) {
  hal_usb_service_c *self = (hal_usb_service_c *)ip;

  osalDbgCheck(self != NULL);

  return self->next;
}

const usb_descriptor_t *usbBinderGetDescriptor(void *ip,
                                               uint8_t dtype,
                                               uint8_t dindex,
                                               uint16_t lang) {
  hal_usb_binder_c *self = (hal_usb_binder_c *)ip;

  osalDbgCheck(self != NULL);

  return self->vmt->get_descriptor(self, dtype, dindex, lang);
}

void usbBinderResetI(void *ip) {
  hal_usb_binder_c *self = (hal_usb_binder_c *)ip;

  osalDbgCheckClassI();
  osalDbgCheck(self != NULL);

  self->vmt->reset(self);
}

void usbBinderConfigureI(void *ip) {
  hal_usb_binder_c *self = (hal_usb_binder_c *)ip;

  osalDbgCheckClassI();
  osalDbgCheck(self != NULL);

  self->vmt->configure(self);
}

void usbBinderUnconfigureI(void *ip) {
  hal_usb_binder_c *self = (hal_usb_binder_c *)ip;

  osalDbgCheckClassI();
  osalDbgCheck(self != NULL);

  self->vmt->unconfigure(self);
}

void usbBinderSuspendI(void *ip) {
  hal_usb_binder_c *self = (hal_usb_binder_c *)ip;

  osalDbgCheckClassI();
  osalDbgCheck(self != NULL);

  self->vmt->suspend(self);
}

void usbBinderWakeupI(void *ip) {
  hal_usb_binder_c *self = (hal_usb_binder_c *)ip;

  osalDbgCheckClassI();
  osalDbgCheck(self != NULL);

  self->vmt->wakeup(self);
}

void usbBinderSOFI(void *ip) {
  hal_usb_binder_c *self = (hal_usb_binder_c *)ip;

  osalDbgCheckClassI();
  osalDbgCheck(self != NULL);

  self->vmt->sof(self);
}

void usbBinderInI(void *ip, usbep_t ep) {
  hal_usb_binder_c *self = (hal_usb_binder_c *)ip;

  osalDbgCheckClassI();
  osalDbgCheck((self != NULL) && (ep <= (usbep_t)USB_MAX_ENDPOINTS));

  self->vmt->in(self, ep);
}

void usbBinderOutI(void *ip, usbep_t ep) {
  hal_usb_binder_c *self = (hal_usb_binder_c *)ip;

  osalDbgCheckClassI();
  osalDbgCheck((self != NULL) && (ep <= (usbep_t)USB_MAX_ENDPOINTS));

  self->vmt->out(self, ep);
}

msg_t usbBinderSetup(void *ip, bool *handledp) {
  hal_usb_binder_c *self = (hal_usb_binder_c *)ip;

  osalDbgCheck((self != NULL) && (handledp != NULL));

  return self->vmt->setup(self, handledp);
}

/*===========================================================================*/
/* Module exported variables.                                                */
/*===========================================================================*/

const struct hal_usb_binder_vmt __hal_usb_binder_vmt = {
  .dispose                  = __usbbnd_dispose_impl,
  .bind                     = __usbbnd_bind_impl,
  .unbind                   = __usbbnd_unbind_impl,
  .get_descriptor           = __usbbnd_get_descriptor_impl,
  .reset                    = __usbbnd_reset_impl,
  .configure                = __usbbnd_configure_impl,
  .unconfigure              = __usbbnd_unconfigure_impl,
  .suspend                  = __usbbnd_suspend_impl,
  .wakeup                   = __usbbnd_wakeup_impl,
  .sof                      = __usbbnd_sof_impl,
  .in                       = __usbbnd_in_impl,
  .out                      = __usbbnd_out_impl,
  .setup                    = __usbbnd_setup_impl
};

/** @} */

#endif /* HAL_USE_USB == TRUE */
