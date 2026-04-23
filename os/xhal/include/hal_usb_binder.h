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
 * @file    hal_usb_binder.h
 * @brief   USB binders base class header.
 *
 * @addtogroup HAL_USB_BINDER
 * @{
 */

#ifndef HAL_USB_BINDER_H
#define HAL_USB_BINDER_H

#include "hal.h"
#include "oop_base_object.h"

#if (HAL_USE_USB == TRUE) || defined(__DOXYGEN__)

typedef struct hal_usb_service hal_usb_service_c;

/**
 * @class     hal_usb_binder_c
 * @extends   base_object_c
 *
 * @brief     Base class for USB device binders.
 *
 * @details   A binder is the device-composition object sitting between a USB
 *            driver instance and one or more USB services. The binder owns the
 *            final descriptors and dispatches USB events to the registered
 *            services.
 *
 * @name      Class @p hal_usb_binder_c structures
 * @{
 */

/**
 * @brief     Type of a USB binder base class.
 */
typedef struct hal_usb_binder hal_usb_binder_c;

/**
 * @brief     Class @p hal_usb_binder_c virtual methods table.
 */
struct hal_usb_binder_vmt {
  /* From base_object_c.*/
  void (*dispose)(void *ip);
  /* From hal_usb_binder_c.*/
  msg_t (*bind)(void *ip, hal_usb_driver_c *usbp);
  void (*unbind)(void *ip);
  const usb_descriptor_t * (*get_descriptor)(void *ip,
                                             uint8_t dtype,
                                             uint8_t dindex,
                                             uint16_t lang);
  void (*reset)(void *ip);
  void (*configure)(void *ip);
  void (*unconfigure)(void *ip);
  void (*suspend)(void *ip);
  void (*wakeup)(void *ip);
  void (*sof)(void *ip);
  void (*in)(void *ip, usbep_t ep);
  void (*out)(void *ip, usbep_t ep);
  msg_t (*setup)(void *ip, bool *handledp);
};

/**
 * @brief     Structure representing a USB binder base class.
 */
struct hal_usb_binder {
  /**
   * @brief   Virtual Methods Table.
   */
  const struct hal_usb_binder_vmt *vmt;
  /**
   * @brief   Currently bound USB driver, can be @p NULL.
   */
  hal_usb_driver_c               *usbp;
  /**
   * @brief   Head of the registered services list.
   */
  hal_usb_service_c              *services;
  /**
   * @brief   Tail of the registered services list.
   */
  hal_usb_service_c              *service_last;
  /**
   * @brief   Number of registered services.
   */
  unsigned                       services_num;
  /**
   * @brief   Application-defined binder argument.
   */
  void                           *arg;
};
/** @} */

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#ifdef __cplusplus
extern "C" {
#endif
  /* Methods of hal_usb_binder_c.*/
  void *__usbbnd_objinit_impl(void *ip, const void *vmt);
  void __usbbnd_dispose_impl(void *ip);
  msg_t usbBinderBind(void *ip, hal_usb_driver_c *usbp);
  void usbBinderUnbind(void *ip);
  msg_t usbBinderRegisterService(void *ip, hal_usb_service_c *servicep);
  msg_t usbBinderUnregisterService(void *ip, hal_usb_service_c *servicep);
  hal_usb_service_c *usbBinderGetNextServiceX(void *ip);
  const usb_descriptor_t *usbBinderGetDescriptor(void *ip,
                                                 uint8_t dtype,
                                                 uint8_t dindex,
                                                 uint16_t lang);
  void usbBinderResetI(void *ip);
  void usbBinderConfigureI(void *ip);
  void usbBinderUnconfigureI(void *ip);
  void usbBinderSuspendI(void *ip);
  void usbBinderWakeupI(void *ip);
  void usbBinderSOFI(void *ip);
  void usbBinderInI(void *ip, usbep_t ep);
  void usbBinderOutI(void *ip, usbep_t ep);
  msg_t usbBinderSetup(void *ip, bool *handledp);
#ifdef __cplusplus
}
#endif

/*===========================================================================*/
/* Module inline functions.                                                  */
/*===========================================================================*/

/**
 * @name      Default constructor of hal_usb_binder_c
 * @{
 */
/**
 * @brief     Default initialization function of @p hal_usb_binder_c.
 *
 * @param[out] self          Pointer to a @p hal_usb_binder_c instance to be
 *                           initialized.
 * @return                   Pointer to the initialized object.
 *
 * @objinit
 */
CC_FORCE_INLINE
static inline hal_usb_binder_c *usbBinderObjectInit(hal_usb_binder_c *self) {
  extern const struct hal_usb_binder_vmt __hal_usb_binder_vmt;

  return __usbbnd_objinit_impl(self, &__hal_usb_binder_vmt);
}
/** @} */

/**
 * @name      Inline methods of hal_usb_binder_c
 * @{
 */
/**
 * @brief     Returns the currently bound USB driver.
 *
 * @param[in] ip            Pointer to a @p hal_usb_binder_c instance.
 * @return                  The bound USB driver or @p NULL.
 *
 * @xclass
 */
CC_FORCE_INLINE
static inline hal_usb_driver_c *usbBinderGetDriverX(void *ip) {
  hal_usb_binder_c *self = (hal_usb_binder_c *)ip;

  return self->usbp;
}

/**
 * @brief     Returns the first registered service.
 *
 * @param[in] ip            Pointer to a @p hal_usb_binder_c instance.
 * @return                  Pointer to the first service or @p NULL.
 *
 * @xclass
 */
CC_FORCE_INLINE
static inline hal_usb_service_c *usbBinderGetFirstServiceX(void *ip) {
  hal_usb_binder_c *self = (hal_usb_binder_c *)ip;

  return self->services;
}

/**
 * @brief     Returns the number of registered services.
 *
 * @param[in] ip            Pointer to a @p hal_usb_binder_c instance.
 * @return                  The number of registered services.
 *
 * @xclass
 */
CC_FORCE_INLINE
static inline unsigned usbBinderGetServicesNumX(void *ip) {
  hal_usb_binder_c *self = (hal_usb_binder_c *)ip;

  return self->services_num;
}

/**
 * @brief     Sets the application-defined binder argument.
 *
 * @param[in,out] ip        Pointer to a @p hal_usb_binder_c instance.
 * @param[in]     arg       The new application-defined argument.
 *
 * @xclass
 */
CC_FORCE_INLINE
static inline void usbBinderSetArgumentX(void *ip, void *arg) {
  hal_usb_binder_c *self = (hal_usb_binder_c *)ip;

  self->arg = arg;
}

/**
 * @brief     Returns the application-defined binder argument.
 *
 * @param[in] ip            Pointer to a @p hal_usb_binder_c instance.
 * @return                  The application-defined argument.
 *
 * @xclass
 */
CC_FORCE_INLINE
static inline void *usbBinderGetArgumentX(void *ip) {
  hal_usb_binder_c *self = (hal_usb_binder_c *)ip;

  return self->arg;
}
/** @} */

#endif /* HAL_USB_BINDER_H */

/** @} */

#endif /* HAL_USE_USB == TRUE */
