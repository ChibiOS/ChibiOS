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
 * @file    hal_usb_service.h
 * @brief   USB services base class header.
 *
 * @addtogroup HAL_USB_SERVICE
 * @{
 */

#ifndef HAL_USB_SERVICE_H
#define HAL_USB_SERVICE_H

#include "hal.h"
#include "oop_base_object.h"

#if (HAL_USE_USB == TRUE) || defined(__DOXYGEN__)

/*===========================================================================*/
/* Module constants.                                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Module pre-compile time settings.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

/*===========================================================================*/
/* Module data structures and types.                                         */
/*===========================================================================*/

/**
 * @brief   Type of structure representing USB service ownership metadata.
 */
typedef struct hal_usb_service_info hal_usb_service_info_t;

/**
 * @brief   Type of structure representing a USB service base class.
 */
typedef struct hal_usb_service hal_usb_service_c;

/**
 * @brief   Forward declaration of the USB binder base class.
 */
typedef struct hal_usb_binder hal_usb_binder_c;

/**
 * @brief   USB service ownership metadata.
 * @details This structure identifies the interfaces and endpoints managed by a
 *          USB service instance. The service does not own endpoint zero.
 */
struct hal_usb_service_info {
  /**
   * @brief   First interface number owned by the service.
   */
  uint8_t                   if_base;
  /**
   * @brief   Number of consecutive interfaces owned by the service.
   */
  uint8_t                   if_count;
  /**
   * @brief   Bit mask of owned IN endpoints, bit N corresponds to endpoint N.
   */
  uint16_t                  in_ep_mask;
  /**
   * @brief   Bit mask of owned OUT endpoints, bit N corresponds to endpoint N.
   */
  uint16_t                  out_ep_mask;
};

/**
 * @class     hal_usb_service_c
 * @extends   base_object_c
 *
 * @brief     Base class for composable USB services.
 *
 * @details   This class represents a USB function/service managed by a USB
 *            binder. The service exposes static ownership metadata plus
 *            USB-specific lifecycle and dispatch hooks. The service itself is
 *            not a driver and does not own the USB peripheral lifecycle.
 *
 * @name      Class @p hal_usb_service_c structures
 * @{
 */

/**
 * @brief     Class @p hal_usb_service_c virtual methods table.
 */
struct hal_usb_service_vmt {
  /* From base_object_c.*/
  void (*dispose)(void *ip);
  /* From hal_usb_service_c.*/
  msg_t (*bind)(void *ip, hal_usb_binder_c *binderp);
  void (*unbind)(void *ip);
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
 * @brief     Structure representing a USB service base class.
 */
struct hal_usb_service {
  /**
   * @brief   Virtual Methods Table.
   */
  const struct hal_usb_service_vmt *vmt;
  /**
   * @brief   Binder currently owning the service, can be @p NULL.
   */
  hal_usb_binder_c                *binder;
  /**
   * @brief   Pointer to the immutable service ownership metadata.
   */
  const hal_usb_service_info_t    *info;
  /**
   * @brief   Application-defined service argument.
   */
  void                            *arg;
  /**
   * @brief   Next service in the binder registration list.
   */
  hal_usb_service_c               *next;
  /**
   * @brief   Previous service in the binder registration list.
   */
  hal_usb_service_c               *prev;
};
/** @} */

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#ifdef __cplusplus
extern "C" {
#endif
  /* Methods of hal_usb_service_c.*/
  void *__usbsvc_objinit_impl(void *ip, const void *vmt,
                              const hal_usb_service_info_t *infop);
  void __usbsvc_dispose_impl(void *ip);
  msg_t usbServiceBind(void *ip, hal_usb_binder_c *binderp);
  void usbServiceUnbind(void *ip);
  void usbServiceResetI(void *ip);
  void usbServiceConfigureI(void *ip);
  void usbServiceUnconfigureI(void *ip);
  void usbServiceSuspendI(void *ip);
  void usbServiceWakeupI(void *ip);
  void usbServiceSOFI(void *ip);
  void usbServiceInI(void *ip, usbep_t ep);
  void usbServiceOutI(void *ip, usbep_t ep);
  msg_t usbServiceSetup(void *ip, bool *handledp);
#ifdef __cplusplus
}
#endif

/*===========================================================================*/
/* Module inline functions.                                                  */
/*===========================================================================*/

/**
 * @name      Default constructor of hal_usb_service_c
 * @{
 */
/**
 * @brief     Default initialization function of @p hal_usb_service_c.
 *
 * @param[out] self          Pointer to a @p hal_usb_service_c instance to be
 *                           initialized.
 * @param[in]  infop         Pointer to the service ownership metadata.
 * @return                   Pointer to the initialized object.
 *
 * @objinit
 */
CC_FORCE_INLINE
static inline hal_usb_service_c *usbServiceObjectInit(
    hal_usb_service_c *self, const hal_usb_service_info_t *infop) {
  extern const struct hal_usb_service_vmt __hal_usb_service_vmt;

  return __usbsvc_objinit_impl(self, &__hal_usb_service_vmt, infop);
}
/** @} */

/**
 * @name      Inline methods of hal_usb_service_c
 * @{
 */
/**
 * @brief     Returns the current binder owning the service.
 *
 * @param[in] ip            Pointer to a @p hal_usb_service_c instance.
 * @return                  The bound binder or @p NULL.
 *
 * @xclass
 */
CC_FORCE_INLINE
static inline hal_usb_binder_c *usbServiceGetBinderX(void *ip) {
  hal_usb_service_c *self = (hal_usb_service_c *)ip;

  return self->binder;
}

/**
 * @brief     Returns the service ownership metadata.
 *
 * @param[in] ip            Pointer to a @p hal_usb_service_c instance.
 * @return                  Pointer to the immutable metadata.
 *
 * @xclass
 */
CC_FORCE_INLINE
static inline const hal_usb_service_info_t *usbServiceGetInfoX(void *ip) {
  hal_usb_service_c *self = (hal_usb_service_c *)ip;

  return self->info;
}

/**
 * @brief     Sets the application-defined service argument.
 *
 * @param[in,out] ip        Pointer to a @p hal_usb_service_c instance.
 * @param[in]     arg       The new application-defined argument.
 *
 * @xclass
 */
CC_FORCE_INLINE
static inline void usbServiceSetArgumentX(void *ip, void *arg) {
  hal_usb_service_c *self = (hal_usb_service_c *)ip;

  self->arg = arg;
}

/**
 * @brief     Returns the application-defined service argument.
 *
 * @param[in] ip            Pointer to a @p hal_usb_service_c instance.
 * @return                  The application-defined argument.
 *
 * @xclass
 */
CC_FORCE_INLINE
static inline void *usbServiceGetArgumentX(void *ip) {
  hal_usb_service_c *self = (hal_usb_service_c *)ip;

  return self->arg;
}

/**
 * @brief     Checks whether the service owns a specific interface number.
 *
 * @param[in] ip            Pointer to a @p hal_usb_service_c instance.
 * @param[in] ifn           Interface number to be tested.
 * @return                  Ownership result.
 *
 * @xclass
 */
CC_FORCE_INLINE
static inline bool usbServiceOwnsInterfaceX(void *ip, uint8_t ifn) {
  hal_usb_service_c *self = (hal_usb_service_c *)ip;
  unsigned base, limit;

  if ((self->info == NULL) || (self->info->if_count == 0U)) {
    return false;
  }

  base  = (unsigned)self->info->if_base;
  limit = base + (unsigned)self->info->if_count;

  return ((unsigned)ifn >= base) && ((unsigned)ifn < limit);
}

/**
 * @brief     Checks whether the service owns a specific IN endpoint.
 *
 * @param[in] ip            Pointer to a @p hal_usb_service_c instance.
 * @param[in] ep            Endpoint number to be tested.
 * @return                  Ownership result.
 *
 * @xclass
 */
CC_FORCE_INLINE
static inline bool usbServiceOwnsInEndpointX(void *ip, usbep_t ep) {
  hal_usb_service_c *self = (hal_usb_service_c *)ip;

  if ((self->info == NULL) || (ep == 0U) || (ep > (usbep_t)USB_MAX_ENDPOINTS)) {
    return false;
  }

  return (bool)((self->info->in_ep_mask &
                 (uint16_t)((unsigned)1U << (unsigned)ep)) != 0U);
}

/**
 * @brief     Checks whether the service owns a specific OUT endpoint.
 *
 * @param[in] ip            Pointer to a @p hal_usb_service_c instance.
 * @param[in] ep            Endpoint number to be tested.
 * @return                  Ownership result.
 *
 * @xclass
 */
CC_FORCE_INLINE
static inline bool usbServiceOwnsOutEndpointX(void *ip, usbep_t ep) {
  hal_usb_service_c *self = (hal_usb_service_c *)ip;

  if ((self->info == NULL) || (ep == 0U) || (ep > (usbep_t)USB_MAX_ENDPOINTS)) {
    return false;
  }

  return (bool)((self->info->out_ep_mask &
                 (uint16_t)((unsigned)1U << (unsigned)ep)) != 0U);
}
/** @} */

#endif /* HAL_USB_SERVICE_H */

/** @} */

#endif /* HAL_USE_USB == TRUE */
