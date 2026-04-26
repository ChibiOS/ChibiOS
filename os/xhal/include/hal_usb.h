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
 * @file        hal_usb.h
 * @brief       Generated USB Driver header.
 * @note        This is a generated file, do not edit directly.
 *
 * @addtogroup  HAL_USB
 * @brief       USB Driver macros and structures.
 * @{
 */

#ifndef HAL_USB_H
#define HAL_USB_H

#include "hal_base_driver.h"

#if (HAL_USE_USB == TRUE) || defined(__DOXYGEN__)

/*===========================================================================*/
/* Module constants.                                                         */
/*===========================================================================*/

/**
 * @name    Endpoint direction helpers
 * @{
 */
/**
 * @brief       Builds an OUT endpoint address.
 */
#define USB_ENDPOINT_OUT                    (ep)

/**
 * @brief       Builds an IN endpoint address.
 */
#define USB_ENDPOINT_IN                     ((ep) | 0x80U)
/** @} */

/**
 * @name    Request type encoding
 * @{
 */
#define USB_RTYPE_DIR_MASK                  0x80U
#define USB_RTYPE_DIR_HOST2DEV              0x00U
#define USB_RTYPE_DIR_DEV2HOST              0x80U
#define USB_RTYPE_TYPE_MASK                 0x60U
#define USB_RTYPE_TYPE_STD                  0x00U
#define USB_RTYPE_TYPE_CLASS                0x20U
#define USB_RTYPE_TYPE_VENDOR               0x40U
#define USB_RTYPE_TYPE_RESERVED             0x60U
#define USB_RTYPE_RECIPIENT_MASK            0x1FU
#define USB_RTYPE_RECIPIENT_DEVICE          0x00U
#define USB_RTYPE_RECIPIENT_INTERFACE       0x01U
#define USB_RTYPE_RECIPIENT_ENDPOINT        0x02U
#define USB_RTYPE_RECIPIENT_OTHER           0x03U
/** @} */

/**
 * @name    Standard requests
 * @{
 */
#define USB_REQ_GET_STATUS                  0U
#define USB_REQ_CLEAR_FEATURE               1U
#define USB_REQ_SET_FEATURE                 3U
#define USB_REQ_SET_ADDRESS                 5U
#define USB_REQ_GET_DESCRIPTOR              6U
#define USB_REQ_SET_DESCRIPTOR              7U
#define USB_REQ_GET_CONFIGURATION           8U
#define USB_REQ_SET_CONFIGURATION           9U
#define USB_REQ_GET_INTERFACE               10U
#define USB_REQ_SET_INTERFACE               11U
#define USB_REQ_SYNCH_FRAME                 12U
/** @} */

/**
 * @name    Descriptor identifiers
 * @{
 */
#define USB_DESCRIPTOR_DEVICE               1U
#define USB_DESCRIPTOR_CONFIGURATION        2U
#define USB_DESCRIPTOR_STRING               3U
#define USB_DESCRIPTOR_INTERFACE            4U
#define USB_DESCRIPTOR_ENDPOINT             5U
#define USB_DESCRIPTOR_DEVICE_QUALIFIER     6U
#define USB_DESCRIPTOR_OTHER_SPEED_CFG      7U
#define USB_DESCRIPTOR_INTERFACE_POWER      8U
#define USB_DESCRIPTOR_INTERFACE_ASSOCIATION 11U
/** @} */

/**
 * @name    Feature selectors
 * @{
 */
#define USB_FEATURE_ENDPOINT_HALT           0U
#define USB_FEATURE_DEVICE_REMOTE_WAKEUP    1U
#define USB_FEATURE_TEST_MODE               2U
/** @} */

/**
 * @name    Device addressing modes
 * @{
 */
#define USB_EARLY_SET_ADDRESS               0
#define USB_LATE_SET_ADDRESS                1
/** @} */

/**
 * @name    Endpoint-zero status handling modes
 * @{
 */
#define USB_EP0_STATUS_STAGE_SW             0
#define USB_EP0_STATUS_STAGE_HW             1
/** @} */

/**
 * @name    Set-address acknowledge handling modes
 * @{
 */
#define USB_SET_ADDRESS_ACK_SW              0
#define USB_SET_ADDRESS_ACK_HW              1
/** @} */

/**
 * @name    Endpoint types
 * @{
 */
#define USB_EP_MODE_TYPE                    0x0003U
#define USB_EP_MODE_TYPE_CTRL               0x0000U
#define USB_EP_MODE_TYPE_ISOC               0x0001U
#define USB_EP_MODE_TYPE_BULK               0x0002U
#define USB_EP_MODE_TYPE_INTR               0x0003U
/** @} */

/**
 * @name    USB event flags
 * @{
 */
/**
 * @brief       Bus reset event flag.
 */
#define USB_FLAGS_RESET                     (1U << 0)

/**
 * @brief       Set-address event flag.
 */
#define USB_FLAGS_ADDRESS                   (1U << 1)

/**
 * @brief       Configuration-selected event flag.
 */
#define USB_FLAGS_CONFIGURED                (1U << 2)

/**
 * @brief       Configuration-cleared event flag.
 */
#define USB_FLAGS_UNCONFIGURED              (1U << 3)

/**
 * @brief       Suspend event flag.
 */
#define USB_FLAGS_SUSPEND                   (1U << 4)

/**
 * @brief       Wake-up event flag.
 */
#define USB_FLAGS_WAKEUP                    (1U << 5)

/**
 * @brief       Endpoint-zero stall event flag.
 */
#define USB_FLAGS_STALLED                   (1U << 6)
/** @} */

/*===========================================================================*/
/* Module pre-compile time settings.                                         */
/*===========================================================================*/

/**
 * @name    Configuration options
 * @{
 */
/**
 * @brief       Support for thread synchronization API.
 */
#if !defined(USB_USE_SYNCHRONIZATION) || defined(__DOXYGEN__)
#define USB_USE_SYNCHRONIZATION             TRUE
#endif

/**
 * @brief       Support for user-provided USB hardware configurations.
 * @note        When enabled the user must provide a variable named @p
 *              usb_configurations of type @p usb_configurations_t.
 */
#if !defined(USB_USE_CONFIGURATIONS) || defined(__DOXYGEN__)
#define USB_USE_CONFIGURATIONS              FALSE
#endif
/** @} */

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

/* Checks on USB_USE_SYNCHRONIZATION configuration.*/
#if (USB_USE_SYNCHRONIZATION != FALSE) && (USB_USE_SYNCHRONIZATION != TRUE)
#error "invalid USB_USE_SYNCHRONIZATION value"
#endif

/* Checks on USB_USE_CONFIGURATIONS configuration.*/
#if (USB_USE_CONFIGURATIONS != FALSE) && (USB_USE_CONFIGURATIONS != TRUE)
#error "invalid USB_USE_CONFIGURATIONS value"
#endif

/*===========================================================================*/
/* Module macros.                                                            */
/*===========================================================================*/

/**
 * @name    Low level driver helper macros
 * @{
 */
/**
 * @brief       Common ISR code, USB event notification.
 *
 * @param[in,out] usbp          Pointer to the USB driver instance.
 * @param[in]     flags         Event flags to be posted.
 *
 * @notapi
 */
#define _usb_isr_invoke_event_cb(usbp, flags)                               \
  do {                                                                      \
    (usbp)->events |= (usbeventflags_t)(flags);                             \
  } while (false)

/**
 * @brief       Common ISR code, SOF notification.
 *
 * @param[in,out] usbp          Pointer to the USB driver instance.
 *
 * @notapi
 */
#define _usb_isr_invoke_sof_cb(usbp)                                        \
  do {                                                                      \
    if ((usbp)->binder != NULL) {                                           \
      osalSysLockFromISR();                                                 \
      usbBinderSOFI((usbp)->binder);                                        \
      osalSysUnlockFromISR();                                               \
    }                                                                       \
  } while (false)

/**
 * @brief       Common ISR code, setup packet callback.
 *
 * @param[in,out] usbp          Pointer to the USB driver instance.
 * @param[in]     ep            Endpoint number.
 *
 * @notapi
 */
#define _usb_isr_invoke_setup_cb(usbp, ep)                                  \
  do {                                                                      \
    if ((usbp)->epc[ep]->setup_cb != NULL) {                                \
      (usbp)->epc[ep]->setup_cb(usbp, ep);                                  \
    }                                                                       \
  } while (false)

#if (USB_USE_SYNCHRONIZATION == TRUE) || defined (__DOXYGEN__)
/**
 * @brief       Common ISR code, IN endpoint callback.
 *
 * @param[in,out] usbp          Pointer to the USB driver instance.
 * @param[in]     ep            Endpoint number.
 *
 * @notapi
 */
#define _usb_isr_invoke_in_cb(usbp, ep)                                     \
  do {                                                                      \
    (usbp)->transmitting &= ~(uint16_t)((unsigned)1U << (unsigned)(ep));    \
    if ((usbp)->epc[ep]->in_cb != NULL) {                                   \
      (usbp)->epc[ep]->in_cb(usbp, ep);                                     \
    }                                                                       \
    osalSysLockFromISR();                                                   \
    osalThreadResumeI(&(usbp)->epc[ep]->in_state->thread, MSG_OK);          \
    osalSysUnlockFromISR();                                                 \
  } while (false)

/**
 * @brief       Common ISR code, OUT endpoint callback.
 *
 * @param[in,out] usbp          Pointer to the USB driver instance.
 * @param[in]     ep            Endpoint number.
 *
 * @notapi
 */
#define _usb_isr_invoke_out_cb(usbp, ep)                                    \
  do {                                                                      \
    (usbp)->receiving &= ~(uint16_t)((unsigned)1U << (unsigned)(ep));       \
    if ((usbp)->epc[ep]->out_cb != NULL) {                                  \
      (usbp)->epc[ep]->out_cb(usbp, ep);                                    \
    }                                                                       \
    osalSysLockFromISR();                                                   \
    osalThreadResumeI(&(usbp)->epc[ep]->out_state->thread,                  \
                      usbGetReceiveTransactionSizeX(usbp, ep));             \
    osalSysUnlockFromISR();                                                 \
  } while (false)

#else
#define _usb_isr_invoke_in_cb(usbp, ep)                                     \
  do {                                                                      \
    (usbp)->transmitting &= ~(uint16_t)((unsigned)1U << (unsigned)(ep));    \
    if ((usbp)->epc[ep]->in_cb != NULL) {                                   \
      (usbp)->epc[ep]->in_cb(usbp, ep);                                     \
    }                                                                       \
  } while (false)
#define _usb_isr_invoke_out_cb(usbp, ep)                                    \
  do {                                                                      \
    (usbp)->receiving &= ~(uint16_t)((unsigned)1U << (unsigned)(ep));       \
    if ((usbp)->epc[ep]->out_cb != NULL) {                                  \
      (usbp)->epc[ep]->out_cb(usbp, ep);                                    \
    }                                                                       \
  } while (false)
#endif /* USB_USE_SYNCHRONIZATION == TRUE */
/** @} */

/*===========================================================================*/
/* Module data structures and types.                                         */
/*===========================================================================*/

/**
 * @brief       Type of USB event flags.
 */
typedef uint32_t usbeventflags_t;

/**
 * @brief       Type of structure representing a USB driver.
 */
typedef struct hal_usb_driver hal_usb_driver_c;

/**
 * @brief       Type of structure representing a USB hardware configuration.
 */
typedef struct hal_usb_config hal_usb_config_t;

typedef uint8_t usbep_t;
typedef driver_state_t usbstate_t;
struct hal_usb_service;
struct hal_usb_binder;

#define USB_SELECTED                       (HAL_DRV_STATE_ACTIVE + 1U)
#define USB_ACTIVE                         (HAL_DRV_STATE_ACTIVE + 2U)
#define USB_SUSPENDED                      (HAL_DRV_STATE_ACTIVE + 3U)

typedef enum {
  EP_STATUS_DISABLED = 0,
  EP_STATUS_STALLED  = 1,
  EP_STATUS_ACTIVE   = 2
} usbepstatus_t;

typedef enum {
  USB_EP0_STP_WAITING      = 0U,
  USB_EP0_IN_TX            = 1U,
  USB_EP0_IN_WAITING_TX0   = 2U,
  USB_EP0_IN_SENDING_STS   = 3U,
  USB_EP0_OUT_WAITING_STS  = 4U,
  USB_EP0_OUT_RX           = 5U,
  USB_EP0_ERROR            = 6U
} usbep0state_t;

typedef struct {
  size_t                    ud_size;
  const uint8_t            *ud_string;
} usb_descriptor_t;

typedef void (*usbcallback_t)(hal_usb_driver_c *usbp);
typedef void (*usbepcallback_t)(hal_usb_driver_c *usbp, usbep_t ep);

typedef struct {
  size_t                    txsize;
  size_t                    txcnt;
  size_t                    txlast;
  const uint8_t            *txbuf;
#if (USB_USE_SYNCHRONIZATION == TRUE) || defined(__DOXYGEN__)
  thread_reference_t        thread;
#endif
} USBInEndpointState;

typedef struct {
  size_t                    rxsize;
  size_t                    rxcnt;
  size_t                    rxpkts;
  uint8_t                  *rxbuf;
#if (USB_USE_SYNCHRONIZATION == TRUE) || defined(__DOXYGEN__)
  thread_reference_t        thread;
#endif
} USBOutEndpointState;

typedef struct {
  uint32_t                  ep_mode;
  usbepcallback_t           setup_cb;
  usbepcallback_t           in_cb;
  usbepcallback_t           out_cb;
  uint16_t                  in_maxsize;
  uint16_t                  out_maxsize;
  USBInEndpointState       *in_state;
  USBOutEndpointState      *out_state;
  unsigned                  ep_buffers;
  uint8_t                  *setup_buf;
} USBEndpointConfig;

/* Inclusion of LLD header.*/
#include "hal_usb_lld.h"

/**
 * @brief       USB hardware configuration structure.
 * @note        Protocol and class behavior are configured separately through
 *              the USB binder object.
 */
struct hal_usb_config {
  /* End of the mandatory fields.*/
  usb_lld_config_fields;
#if (defined(USB_CONFIG_EXT_FIELDS)) || defined (__DOXYGEN__)
  USB_CONFIG_EXT_FIELDS
#endif /* defined(USB_CONFIG_EXT_FIELDS) */
};

typedef struct usb_configurations usb_configurations_t;
struct usb_configurations {
  unsigned                  cfgsnum;
  hal_usb_config_t          cfgs[];
};

/**
 * @brief       Type of structure representing USB service ownership metadata.
 */
typedef struct hal_usb_service_info hal_usb_service_info_t;

/**
 * @brief       USB service ownership metadata.
 * @details     This structure identifies the interfaces and endpoints managed
 *              by a USB service instance. Endpoint zero is never owned by a
 *              service.
 */
struct hal_usb_service_info {
  /**
   * @brief       First interface number owned by the service.
   */
  uint8_t                   if_base;
  /**
   * @brief       Number of consecutive interfaces owned by the service.
   */
  uint8_t                   if_count;
  /**
   * @brief       Bit mask of owned IN endpoints, bit N corresponds to endpoint
   *              N.
   */
  uint16_t                  in_ep_mask;
  /**
   * @brief       Bit mask of owned OUT endpoints, bit N corresponds to
   *              endpoint N.
   */
  uint16_t                  out_ep_mask;
};

/**
 * @class       hal_usb_service_c
 * @extends     base_object_c
 *
 * @brief       Base class for composable USB services.
 * @details     This class represents a reusable USB function or service
 *              managed by a USB binder. A service exposes static ownership
 *              metadata together with USB-specific lifecycle and dispatch
 *              hooks, but it does not own the USB peripheral lifecycle. The
 *              virtual methods carrying the @p I suffix must be invoked with
 *              the system lock already held by the caller.
 *
 * @name        Class @p hal_usb_service_c structures
 * @{
 */

/**
 * @brief       Type of a USB service class.
 */
typedef struct hal_usb_service hal_usb_service_c;

/**
 * @brief       Class @p hal_usb_service_c virtual methods table.
 */
struct hal_usb_service_vmt {
  /* From base_object_c.*/
  void (*dispose)(void *ip);
  /* From hal_usb_service_c.*/
  msg_t (*bind)(void *ip, struct hal_usb_binder *binderp);
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
 * @brief       Structure representing a USB service class.
 */
struct hal_usb_service {
  /**
   * @brief       Virtual Methods Table.
   */
  const struct hal_usb_service_vmt *vmt;
  /**
   * @brief       Binder currently owning the service, can be @p NULL.
   */
  struct hal_usb_binder *   binder;
  /**
   * @brief       Pointer to the immutable service ownership metadata.
   */
  const hal_usb_service_info_t * info;
  /**
   * @brief       Application-defined service argument.
   */
  void *                    arg;
  /**
   * @brief       Next service in the binder registration list.
   */
  hal_usb_service_c *       next;
  /**
   * @brief       Previous service in the binder registration list.
   */
  hal_usb_service_c *       prev;
};
/** @} */

/**
 * @class       hal_usb_binder_c
 * @extends     base_object_c
 *
 * @brief       Base class for USB device binders.
 * @details     A binder is the device-composition object sitting between a USB
 *              driver instance and one or more USB services. The binder owns
 *              the final descriptors and dispatches USB events to the
 *              registered services. The virtual methods carrying the @p I
 *              suffix must be invoked with the system lock already held by the
 *              caller.
 *
 * @name        Class @p hal_usb_binder_c structures
 * @{
 */

/**
 * @brief       Type of a USB binder class.
 */
typedef struct hal_usb_binder hal_usb_binder_c;

/**
 * @brief       Class @p hal_usb_binder_c virtual methods table.
 */
struct hal_usb_binder_vmt {
  /* From base_object_c.*/
  void (*dispose)(void *ip);
  /* From hal_usb_binder_c.*/
  msg_t (*bind)(void *ip, hal_usb_driver_c *usbp);
  void (*unbind)(void *ip);
  const usb_descriptor_t * (*get_descriptor)(void *ip, uint8_t dtype, uint8_t dindex, uint16_t lang);
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
 * @brief       Structure representing a USB binder class.
 */
struct hal_usb_binder {
  /**
   * @brief       Virtual Methods Table.
   */
  const struct hal_usb_binder_vmt *vmt;
  /**
   * @brief       Currently bound USB driver, can be @p NULL.
   */
  hal_usb_driver_c *        usbp;
  /**
   * @brief       Head of the registered services list.
   */
  hal_usb_service_c *       services;
  /**
   * @brief       Tail of the registered services list.
   */
  hal_usb_service_c *       service_last;
  /**
   * @brief       Number of registered services.
   */
  unsigned                  services_num;
  /**
   * @brief       Application-defined binder argument.
   */
  void *                    arg;
};
/** @} */

/**
 * @class       hal_usb_driver_c
 * @extends     hal_base_driver_c
 *
 * @brief       Class of a USB driver.
 *
 * @name        Class @p hal_usb_driver_c structures
 * @{
 */

/**
 * @brief       Type of a USB driver class.
 */
typedef struct hal_usb_driver hal_usb_driver_c;

/**
 * @brief       Class @p hal_usb_driver_c virtual methods table.
 */
struct hal_usb_driver_vmt {
  /* From base_object_c.*/
  void (*dispose)(void *ip);
  /* From hal_base_driver_c.*/
  msg_t (*start)(void *ip, const void *config);
  void (*stop)(void *ip);
  const void * (*setcfg)(void *ip, const void *config);
  const void * (*selcfg)(void *ip, unsigned cfgnum);
  /* From hal_usb_driver_c.*/
};

/**
 * @brief       Structure representing a USB driver class.
 */
struct hal_usb_driver {
  /**
   * @brief       Virtual Methods Table.
   */
  const struct hal_usb_driver_vmt *vmt;
  /**
   * @brief       Driver state.
   */
  driver_state_t            state;
  /**
   * @brief       Associated configuration structure.
   */
  const void                *config;
  /**
   * @brief       Driver argument.
   */
  void                      *arg;
#if (HAL_USE_MUTUAL_EXCLUSION == TRUE) || defined (__DOXYGEN__)
  /**
   * @brief       Driver mutex.
   */
  mutex_t                   mutex;
#endif /* HAL_USE_MUTUAL_EXCLUSION == TRUE */
#if (HAL_USE_REGISTRY == TRUE) || defined (__DOXYGEN__)
  /**
   * @brief       Driver identifier.
   */
  unsigned int              id;
  /**
   * @brief       Driver name.
   */
  const char                *name;
  /**
   * @brief       Registry link structure.
   */
  hal_regent_t              regent;
#endif /* HAL_USE_REGISTRY == TRUE */
  /**
   * @brief       Cached USB event flags.
   */
  volatile usbeventflags_t  events;
  hal_usb_binder_c          *binder;
  uint16_t                    transmitting;
  uint16_t                    receiving;
  const USBEndpointConfig    *epc[USB_MAX_ENDPOINTS + 1U];
  void                       *in_params[USB_MAX_ENDPOINTS];
  void                       *out_params[USB_MAX_ENDPOINTS];
  usbep0state_t               ep0state;
  uint8_t                    *ep0next;
  size_t                      ep0n;
  usbcallback_t               ep0endcb;
  thread_reference_t          ep0thread;
  uint8_t                     ep0seq;
  uint8_t                     ep0rseq;
  uint8_t                     ep0setup;
  uint8_t                     ep0reset;
  uint8_t                     setup[8];
  uint16_t                    status;
  uint8_t                     address;
  uint8_t                     configuration;
  usbstate_t                  saved_state;
#if defined(USB_DRIVER_EXT_FIELDS)
  USB_DRIVER_EXT_FIELDS
#endif
  /* End of the mandatory fields.*/
  usb_lld_driver_fields;
};
/** @} */

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#ifdef __cplusplus
extern "C" {
#endif
  /* Methods of hal_usb_service_c.*/
  void *__usbsvc_objinit_impl(void *ip, const void *vmt);
  void __usbsvc_dispose_impl(void *ip);
  msg_t __usbsvc_bind_impl(void *ip, struct hal_usb_binder *binderp);
  void __usbsvc_unbind_impl(void *ip);
  void __usbsvc_reset_impl(void *ip);
  void __usbsvc_configure_impl(void *ip);
  void __usbsvc_unconfigure_impl(void *ip);
  void __usbsvc_suspend_impl(void *ip);
  void __usbsvc_wakeup_impl(void *ip);
  void __usbsvc_sof_impl(void *ip);
  void __usbsvc_in_impl(void *ip, usbep_t ep);
  void __usbsvc_out_impl(void *ip, usbep_t ep);
  msg_t __usbsvc_setup_impl(void *ip, bool *handledp);
  /* Methods of hal_usb_binder_c.*/
  void *__usbbnd_objinit_impl(void *ip, const void *vmt);
  void __usbbnd_dispose_impl(void *ip);
  msg_t __usbbnd_bind_impl(void *ip, hal_usb_driver_c *usbp);
  void __usbbnd_unbind_impl(void *ip);
  const usb_descriptor_t *__usbbnd_get_descriptor_impl(void *ip, uint8_t dtype,
                                                       uint8_t dindex,
                                                       uint16_t lang);
  void __usbbnd_reset_impl(void *ip);
  void __usbbnd_configure_impl(void *ip);
  void __usbbnd_unconfigure_impl(void *ip);
  void __usbbnd_suspend_impl(void *ip);
  void __usbbnd_wakeup_impl(void *ip);
  void __usbbnd_sof_impl(void *ip);
  void __usbbnd_in_impl(void *ip, usbep_t ep);
  void __usbbnd_out_impl(void *ip, usbep_t ep);
  msg_t __usbbnd_setup_impl(void *ip, bool *handledp);
  /* Methods of hal_usb_driver_c.*/
  void *__usb_objinit_impl(void *ip, const void *vmt);
  void __usb_dispose_impl(void *ip);
  msg_t __usb_start_impl(void *ip, const void *config);
  void __usb_stop_impl(void *ip);
  const void *__usb_setcfg_impl(void *ip, const void *config);
  const void *__usb_selcfg_impl(void *ip, unsigned cfgnum);
  msg_t usbBind(void *ip, hal_usb_binder_c *binderp);
  msg_t usbUnbind(void *ip);
  void usbConnectBus(void *ip);
  void usbDisconnectBus(void *ip);
  void usbInitEndpointI(void *ip, usbep_t ep, const USBEndpointConfig *epcp);
  void usbDisableEndpointsI(void *ip);
  void usbReadSetupI(void *ip, usbep_t ep, uint8_t *buf);
  void usbStartReceiveI(void *ip, usbep_t ep, uint8_t *buf, size_t n);
  void usbStartTransmitI(void *ip, usbep_t ep, const uint8_t *buf, size_t n);
#if (USB_USE_SYNCHRONIZATION == TRUE) || defined (__DOXYGEN__)
  msg_t usbReceive(void *ip, usbep_t ep, uint8_t *buf, size_t n);
  msg_t usbTransmit(void *ip, usbep_t ep, const uint8_t *buf, size_t n);
#endif /* USB_USE_SYNCHRONIZATION == TRUE */
  msg_t usbEp0WaitSetup(void *ip);
  msg_t usbEp0Reply(void *ip, const uint8_t *buf, size_t n);
  msg_t usbEp0Receive(void *ip, uint8_t *buf, size_t n);
  msg_t usbEp0Acknowledge(void *ip);
  void usbEp0Stall(void *ip);
  msg_t usbEp0HandleStandardRequest(void *ip, bool *handledp);
  bool usbStallReceiveI(void *ip, usbep_t ep);
  bool usbStallTransmitI(void *ip, usbep_t ep);
  void usbWakeupHost(void *ip);
  /* Regular functions.*/
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
  msg_t usbBinderBind(void *ip, hal_usb_driver_c *usbp);
  void usbBinderUnbind(void *ip);
  msg_t usbBinderRegisterService(void *ip, hal_usb_service_c *servicep);
  msg_t usbBinderUnregisterService(void *ip, hal_usb_service_c *servicep);
  hal_usb_service_c *usbBinderGetNextServiceX(void *ip);
  const usb_descriptor_t *usbBinderGetDescriptor(void *ip, uint8_t dtype,
                                                 uint8_t dindex, uint16_t lang);
  void usbBinderResetI(void *ip);
  void usbBinderConfigureI(void *ip);
  void usbBinderUnconfigureI(void *ip);
  void usbBinderSuspendI(void *ip);
  void usbBinderWakeupI(void *ip);
  void usbBinderSOFI(void *ip);
  void usbBinderInI(void *ip, usbep_t ep);
  void usbBinderOutI(void *ip, usbep_t ep);
  msg_t usbBinderSetup(void *ip, bool *handledp);
  void usbInit(void);
  void _usb_reset(hal_usb_driver_c *usbp);
  void _usb_suspend(hal_usb_driver_c *usbp);
  void _usb_wakeup(hal_usb_driver_c *usbp);
  void _usb_ep0setup(hal_usb_driver_c *usbp, usbep_t ep);
  void _usb_ep0in(hal_usb_driver_c *usbp, usbep_t ep);
  void _usb_ep0out(hal_usb_driver_c *usbp, usbep_t ep);
#ifdef __cplusplus
}
#endif

/*===========================================================================*/
/* Module inline functions.                                                  */
/*===========================================================================*/

/**
 * @name        Virtual methods of hal_usb_service_c
 * @{
 */
/**
 * @brief       Service-specific binding hook.
 *
 * @param[in,out] ip            Pointer to a @p hal_usb_service_c instance.
 * @param[in]     binderp       Binder owning the service.
 * @return                      The operation status.
 */
CC_FORCE_INLINE
static inline msg_t usbServiceOnBind(void *ip, struct hal_usb_binder *binderp) {
  hal_usb_service_c *self = (hal_usb_service_c *)ip;

  return self->vmt->bind(ip, binderp);
}

/**
 * @brief       Service-specific unbind hook.
 *
 * @param[in,out] ip            Pointer to a @p hal_usb_service_c instance.
 */
CC_FORCE_INLINE
static inline void usbServiceOnUnbind(void *ip) {
  hal_usb_service_c *self = (hal_usb_service_c *)ip;

  self->vmt->unbind(ip);
}

/**
 * @brief       Service-specific reset notification hook.
 *
 * @param[in,out] ip            Pointer to a @p hal_usb_service_c instance.
 *
 * @iclass
 */
CC_FORCE_INLINE
static inline void usbServiceOnResetI(void *ip) {
  hal_usb_service_c *self = (hal_usb_service_c *)ip;

  self->vmt->reset(ip);
}

/**
 * @brief       Service-specific configure notification hook.
 *
 * @param[in,out] ip            Pointer to a @p hal_usb_service_c instance.
 *
 * @iclass
 */
CC_FORCE_INLINE
static inline void usbServiceOnConfigureI(void *ip) {
  hal_usb_service_c *self = (hal_usb_service_c *)ip;

  self->vmt->configure(ip);
}

/**
 * @brief       Service-specific unconfigure notification hook.
 *
 * @param[in,out] ip            Pointer to a @p hal_usb_service_c instance.
 *
 * @iclass
 */
CC_FORCE_INLINE
static inline void usbServiceOnUnconfigureI(void *ip) {
  hal_usb_service_c *self = (hal_usb_service_c *)ip;

  self->vmt->unconfigure(ip);
}

/**
 * @brief       Service-specific suspend notification hook.
 *
 * @param[in,out] ip            Pointer to a @p hal_usb_service_c instance.
 *
 * @iclass
 */
CC_FORCE_INLINE
static inline void usbServiceOnSuspendI(void *ip) {
  hal_usb_service_c *self = (hal_usb_service_c *)ip;

  self->vmt->suspend(ip);
}

/**
 * @brief       Service-specific wake-up notification hook.
 *
 * @param[in,out] ip            Pointer to a @p hal_usb_service_c instance.
 *
 * @iclass
 */
CC_FORCE_INLINE
static inline void usbServiceOnWakeupI(void *ip) {
  hal_usb_service_c *self = (hal_usb_service_c *)ip;

  self->vmt->wakeup(ip);
}

/**
 * @brief       Service-specific SOF notification hook.
 *
 * @param[in,out] ip            Pointer to a @p hal_usb_service_c instance.
 *
 * @iclass
 */
CC_FORCE_INLINE
static inline void usbServiceOnSOFI(void *ip) {
  hal_usb_service_c *self = (hal_usb_service_c *)ip;

  self->vmt->sof(ip);
}

/**
 * @brief       Service-specific IN endpoint notification hook.
 *
 * @param[in,out] ip            Pointer to a @p hal_usb_service_c instance.
 * @param[in]     ep            Endpoint number.
 *
 * @iclass
 */
CC_FORCE_INLINE
static inline void usbServiceOnInI(void *ip, usbep_t ep) {
  hal_usb_service_c *self = (hal_usb_service_c *)ip;

  self->vmt->in(ip, ep);
}

/**
 * @brief       Service-specific OUT endpoint notification hook.
 *
 * @param[in,out] ip            Pointer to a @p hal_usb_service_c instance.
 * @param[in]     ep            Endpoint number.
 *
 * @iclass
 */
CC_FORCE_INLINE
static inline void usbServiceOnOutI(void *ip, usbep_t ep) {
  hal_usb_service_c *self = (hal_usb_service_c *)ip;

  self->vmt->out(ip, ep);
}

/**
 * @brief       Service-specific endpoint-zero request hook.
 *
 * @param[in,out] ip            Pointer to a @p hal_usb_service_c instance.
 * @param[out]    handledp      Handled flag.
 * @return                      The operation status.
 */
CC_FORCE_INLINE
static inline msg_t usbServiceOnSetup(void *ip, bool *handledp) {
  hal_usb_service_c *self = (hal_usb_service_c *)ip;

  return self->vmt->setup(ip, handledp);
}
/** @} */

/**
 * @name        Inline methods of hal_usb_service_c
 * @{
 */
/**
 * @brief       Initializes the base USB service part of a derived object.
 *
 * @param[in,out] ip            Pointer to a @p hal_usb_service_c instance.
 * @param[in]     infop         Service ownership metadata.
 * @param[in]     vmt           VMT of the concrete derived service.
 * @return                      Pointer to the initialized object.
 *
 * @objinit
 */
CC_FORCE_INLINE
static inline hal_usb_service_c *usbServiceObjectInit(void *ip,
                                                      const hal_usb_service_info_t *infop,
                                                      const struct hal_usb_service_vmt *vmt) {
  hal_usb_service_c *self = (hal_usb_service_c *)ip;
  self = __usbsvc_objinit_impl(self, vmt);
  self->info = infop;

  return self;
}

/**
 * @brief       Returns the current binder owning the service.
 *
 * @param[in,out] ip            Pointer to a @p hal_usb_service_c instance.
 * @return                      The bound binder or @p NULL.
 *
 * @xclass
 */
CC_FORCE_INLINE
static inline hal_usb_binder_c *usbServiceGetBinderX(void *ip) {
  hal_usb_service_c *self = (hal_usb_service_c *)ip;
  return (hal_usb_binder_c *)self->binder;
}

/**
 * @brief       Returns the service ownership metadata.
 *
 * @param[in,out] ip            Pointer to a @p hal_usb_service_c instance.
 * @return                      Pointer to the immutable metadata.
 *
 * @xclass
 */
CC_FORCE_INLINE
static inline const hal_usb_service_info_t *usbServiceGetInfoX(void *ip) {
  hal_usb_service_c *self = (hal_usb_service_c *)ip;
  return self->info;
}

/**
 * @brief       Sets the application-defined service argument.
 *
 * @param[in,out] ip            Pointer to a @p hal_usb_service_c instance.
 * @param[in]     arg           The new application-defined argument.
 *
 * @xclass
 */
CC_FORCE_INLINE
static inline void usbServiceSetArgumentX(void *ip, void *arg) {
  hal_usb_service_c *self = (hal_usb_service_c *)ip;
  self->arg = arg;
}

/**
 * @brief       Returns the application-defined service argument.
 *
 * @param[in,out] ip            Pointer to a @p hal_usb_service_c instance.
 * @return                      The application-defined argument.
 *
 * @xclass
 */
CC_FORCE_INLINE
static inline void *usbServiceGetArgumentX(void *ip) {
  hal_usb_service_c *self = (hal_usb_service_c *)ip;
  return self->arg;
}

/**
 * @brief       Checks whether the service owns a specific interface number.
 *
 * @param[in,out] ip            Pointer to a @p hal_usb_service_c instance.
 * @param[in]     ifn           Interface number to be tested.
 * @return                      Ownership result.
 *
 * @xclass
 */
CC_FORCE_INLINE
static inline bool usbServiceOwnsInterfaceX(void *ip, uint8_t ifn) {
  hal_usb_service_c *self = (hal_usb_service_c *)ip;
  unsigned base;
  unsigned limit;

  if ((self->info == NULL) || (self->info->if_count == 0U)) {
    return false;
  }

  base  = (unsigned)self->info->if_base;
  limit = base + (unsigned)self->info->if_count;

  return ((unsigned)ifn >= base) && ((unsigned)ifn < limit);
}

/**
 * @brief       Checks whether the service owns a specific IN endpoint.
 *
 * @param[in,out] ip            Pointer to a @p hal_usb_service_c instance.
 * @param[in]     ep            Endpoint number.
 * @return                      Ownership result.
 *
 * @xclass
 */
CC_FORCE_INLINE
static inline bool usbServiceOwnsInEndpointX(void *ip, usbep_t ep) {
  hal_usb_service_c *self = (hal_usb_service_c *)ip;
  if ((self->info == NULL) || (ep > (usbep_t)15U)) {
    return false;
  }

  return (bool)((self->info->in_ep_mask &
                 (uint16_t)((unsigned)1U << (unsigned)ep)) != 0U);
}

/**
 * @brief       Checks whether the service owns a specific OUT endpoint.
 *
 * @param[in,out] ip            Pointer to a @p hal_usb_service_c instance.
 * @param[in]     ep            Endpoint number.
 * @return                      Ownership result.
 *
 * @xclass
 */
CC_FORCE_INLINE
static inline bool usbServiceOwnsOutEndpointX(void *ip, usbep_t ep) {
  hal_usb_service_c *self = (hal_usb_service_c *)ip;
  if ((self->info == NULL) || (ep > (usbep_t)15U)) {
    return false;
  }

  return (bool)((self->info->out_ep_mask &
                 (uint16_t)((unsigned)1U << (unsigned)ep)) != 0U);
}
/** @} */

/**
 * @name        Virtual methods of hal_usb_binder_c
 * @{
 */
/**
 * @brief       Binder-specific bind hook.
 *
 * @param[in,out] ip            Pointer to a @p hal_usb_binder_c instance.
 * @param[in]     usbp          USB driver instance.
 * @return                      The operation status.
 */
CC_FORCE_INLINE
static inline msg_t usbBinderOnBind(void *ip, hal_usb_driver_c *usbp) {
  hal_usb_binder_c *self = (hal_usb_binder_c *)ip;

  return self->vmt->bind(ip, usbp);
}

/**
 * @brief       Binder-specific unbind hook.
 *
 * @param[in,out] ip            Pointer to a @p hal_usb_binder_c instance.
 */
CC_FORCE_INLINE
static inline void usbBinderOnUnbind(void *ip) {
  hal_usb_binder_c *self = (hal_usb_binder_c *)ip;

  self->vmt->unbind(ip);
}

/**
 * @brief       Binder-specific descriptor lookup hook.
 *
 * @param[in,out] ip            Pointer to a @p hal_usb_binder_c instance.
 * @param[in]     dtype         Descriptor type.
 * @param[in]     dindex        Descriptor index.
 * @param[in]     lang          Language identifier.
 * @return                      Descriptor lookup result.
 */
CC_FORCE_INLINE
static inline const usb_descriptor_t *usbBinderOnGetDescriptor(void *ip,
                                                               uint8_t dtype,
                                                               uint8_t dindex,
                                                               uint16_t lang) {
  hal_usb_binder_c *self = (hal_usb_binder_c *)ip;

  return self->vmt->get_descriptor(ip, dtype, dindex, lang);
}

/**
 * @brief       Default reset notification dispatcher.
 *
 * @param[in,out] ip            Pointer to a @p hal_usb_binder_c instance.
 *
 * @iclass
 */
CC_FORCE_INLINE
static inline void usbBinderOnResetI(void *ip) {
  hal_usb_binder_c *self = (hal_usb_binder_c *)ip;

  self->vmt->reset(ip);
}

/**
 * @brief       Default configure notification dispatcher.
 *
 * @param[in,out] ip            Pointer to a @p hal_usb_binder_c instance.
 *
 * @iclass
 */
CC_FORCE_INLINE
static inline void usbBinderOnConfigureI(void *ip) {
  hal_usb_binder_c *self = (hal_usb_binder_c *)ip;

  self->vmt->configure(ip);
}

/**
 * @brief       Default unconfigure notification dispatcher.
 *
 * @param[in,out] ip            Pointer to a @p hal_usb_binder_c instance.
 *
 * @iclass
 */
CC_FORCE_INLINE
static inline void usbBinderOnUnconfigureI(void *ip) {
  hal_usb_binder_c *self = (hal_usb_binder_c *)ip;

  self->vmt->unconfigure(ip);
}

/**
 * @brief       Default suspend notification dispatcher.
 *
 * @param[in,out] ip            Pointer to a @p hal_usb_binder_c instance.
 *
 * @iclass
 */
CC_FORCE_INLINE
static inline void usbBinderOnSuspendI(void *ip) {
  hal_usb_binder_c *self = (hal_usb_binder_c *)ip;

  self->vmt->suspend(ip);
}

/**
 * @brief       Default wake-up notification dispatcher.
 *
 * @param[in,out] ip            Pointer to a @p hal_usb_binder_c instance.
 *
 * @iclass
 */
CC_FORCE_INLINE
static inline void usbBinderOnWakeupI(void *ip) {
  hal_usb_binder_c *self = (hal_usb_binder_c *)ip;

  self->vmt->wakeup(ip);
}

/**
 * @brief       Default SOF notification dispatcher.
 *
 * @param[in,out] ip            Pointer to a @p hal_usb_binder_c instance.
 *
 * @iclass
 */
CC_FORCE_INLINE
static inline void usbBinderOnSOFI(void *ip) {
  hal_usb_binder_c *self = (hal_usb_binder_c *)ip;

  self->vmt->sof(ip);
}

/**
 * @brief       Default IN endpoint notification dispatcher.
 *
 * @param[in,out] ip            Pointer to a @p hal_usb_binder_c instance.
 * @param[in]     ep            Endpoint number.
 *
 * @iclass
 */
CC_FORCE_INLINE
static inline void usbBinderOnInI(void *ip, usbep_t ep) {
  hal_usb_binder_c *self = (hal_usb_binder_c *)ip;

  self->vmt->in(ip, ep);
}

/**
 * @brief       Default OUT endpoint notification dispatcher.
 *
 * @param[in,out] ip            Pointer to a @p hal_usb_binder_c instance.
 * @param[in]     ep            Endpoint number.
 *
 * @iclass
 */
CC_FORCE_INLINE
static inline void usbBinderOnOutI(void *ip, usbep_t ep) {
  hal_usb_binder_c *self = (hal_usb_binder_c *)ip;

  self->vmt->out(ip, ep);
}

/**
 * @brief       Default endpoint-zero request dispatcher.
 *
 * @param[in,out] ip            Pointer to a @p hal_usb_binder_c instance.
 * @param[out]    handledp      Handled flag.
 * @return                      The operation status.
 */
CC_FORCE_INLINE
static inline msg_t usbBinderOnSetup(void *ip, bool *handledp) {
  hal_usb_binder_c *self = (hal_usb_binder_c *)ip;

  return self->vmt->setup(ip, handledp);
}
/** @} */

/**
 * @name        Inline methods of hal_usb_binder_c
 * @{
 */
/**
 * @brief       Initializes the base USB binder part of a derived object.
 *
 * @param[in,out] ip            Pointer to a @p hal_usb_binder_c instance.
 * @param[in]     vmt           VMT of the concrete derived binder.
 * @return                      Pointer to the initialized object.
 *
 * @objinit
 */
CC_FORCE_INLINE
static inline hal_usb_binder_c *usbBinderObjectInit(void *ip,
                                                    const struct hal_usb_binder_vmt *vmt) {
  hal_usb_binder_c *self = (hal_usb_binder_c *)ip;
  return __usbbnd_objinit_impl(self, vmt);
}

/**
 * @brief       Returns the currently bound USB driver.
 *
 * @param[in,out] ip            Pointer to a @p hal_usb_binder_c instance.
 * @return                      The bound USB driver or @p NULL.
 *
 * @xclass
 */
CC_FORCE_INLINE
static inline hal_usb_driver_c *usbBinderGetDriverX(void *ip) {
  hal_usb_binder_c *self = (hal_usb_binder_c *)ip;
  return self->usbp;
}

/**
 * @brief       Returns the first registered service.
 *
 * @param[in,out] ip            Pointer to a @p hal_usb_binder_c instance.
 * @return                      Pointer to the first service or @p NULL.
 *
 * @xclass
 */
CC_FORCE_INLINE
static inline hal_usb_service_c *usbBinderGetFirstServiceX(void *ip) {
  hal_usb_binder_c *self = (hal_usb_binder_c *)ip;
  return self->services;
}

/**
 * @brief       Returns the number of registered services.
 *
 * @param[in,out] ip            Pointer to a @p hal_usb_binder_c instance.
 * @return                      The number of registered services.
 *
 * @xclass
 */
CC_FORCE_INLINE
static inline unsigned usbBinderGetServicesNumX(void *ip) {
  hal_usb_binder_c *self = (hal_usb_binder_c *)ip;
  return self->services_num;
}

/**
 * @brief       Sets the application-defined binder argument.
 *
 * @param[in,out] ip            Pointer to a @p hal_usb_binder_c instance.
 * @param[in]     arg           The new application-defined argument.
 *
 * @xclass
 */
CC_FORCE_INLINE
static inline void usbBinderSetArgumentX(void *ip, void *arg) {
  hal_usb_binder_c *self = (hal_usb_binder_c *)ip;
  self->arg = arg;
}

/**
 * @brief       Returns the application-defined binder argument.
 *
 * @param[in,out] ip            Pointer to a @p hal_usb_binder_c instance.
 * @return                      The application-defined argument.
 *
 * @xclass
 */
CC_FORCE_INLINE
static inline void *usbBinderGetArgumentX(void *ip) {
  hal_usb_binder_c *self = (hal_usb_binder_c *)ip;
  return self->arg;
}
/** @} */

/**
 * @name        Default constructor of hal_usb_driver_c
 * @{
 */
/**
 * @brief       Default initialization function of @p hal_usb_driver_c.
 *
 * @param[out]    self          Pointer to a @p hal_usb_driver_c instance to be
 *                              initialized.
 * @return                      Pointer to the initialized object.
 *
 * @objinit
 */
CC_FORCE_INLINE
static inline hal_usb_driver_c *usbObjectInit(hal_usb_driver_c *self) {
  extern const struct hal_usb_driver_vmt __hal_usb_driver_vmt;

  return __usb_objinit_impl(self, &__hal_usb_driver_vmt);
}
/** @} */

/**
 * @name        Inline methods of hal_usb_driver_c
 * @{
 */
/**
 * @brief       Returns the current USB driver state.
 *
 * @param[in,out] ip            Pointer to a @p hal_usb_driver_c instance.
 * @return                      The current USB driver state.
 *
 * @xclass
 */
CC_FORCE_INLINE
static inline usbstate_t usbGetDriverStateX(void *ip) {
  hal_usb_driver_c *self = (hal_usb_driver_c *)ip;
  return (usbstate_t)self->state;
}

/**
 * @brief       Returns the current USB binder.
 *
 * @param[in,out] ip            Pointer to a @p hal_usb_driver_c instance.
 * @return                      The current USB binder or @p NULL.
 *
 * @xclass
 */
CC_FORCE_INLINE
static inline hal_usb_binder_c *usbGetBinderX(void *ip) {
  hal_usb_driver_c *self = (hal_usb_driver_c *)ip;
  return self->binder;
}

/**
 * @brief       Returns the cached USB event flags.
 *
 * @param[in,out] ip            Pointer to a @p hal_usb_driver_c instance.
 * @return                      The current cached event flags.
 *
 * @xclass
 */
CC_FORCE_INLINE
static inline usbeventflags_t usbGetEventsX(void *ip) {
  hal_usb_driver_c *self = (hal_usb_driver_c *)ip;
  return self->events;
}

/**
 * @brief       Gets and clears cached USB event flags.
 *
 * @param[in,out] ip            Pointer to a @p hal_usb_driver_c instance.
 * @param[in]     mask          Mask of events to be returned and cleared.
 * @return                      The selected pending event flags.
 *
 * @xclass
 */
CC_FORCE_INLINE
static inline usbeventflags_t usbGetAndClearEventsX(void *ip,
                                                    usbeventflags_t mask) {
  hal_usb_driver_c *self = (hal_usb_driver_c *)ip;
  usbeventflags_t flags;
  syssts_t sts;

  sts = osalSysGetStatusAndLockX();
  flags = self->events & mask;
  self->events &= ~mask;
  osalSysRestoreStatusX(sts);

  return flags;
}

/**
 * @brief       Returns the current USB frame number.
 *
 * @param[in,out] ip            Pointer to a @p hal_usb_driver_c instance.
 * @return                      The current USB frame number.
 *
 * @xclass
 */
CC_FORCE_INLINE
static inline uint16_t usbGetFrameNumberX(void *ip) {
  hal_usb_driver_c *self = (hal_usb_driver_c *)ip;
  (void)self;
  return usb_lld_get_frame_number(self);
}

/**
 * @brief       Returns the received size of the last OUT transaction.
 *
 * @param[in,out] ip            Pointer to a @p hal_usb_driver_c instance.
 * @param[in]     ep            Endpoint number.
 * @return                      The received size.
 *
 * @xclass
 */
CC_FORCE_INLINE
static inline size_t usbGetReceiveTransactionSizeX(void *ip, usbep_t ep) {
  hal_usb_driver_c *self = (hal_usb_driver_c *)ip;
  return usb_lld_get_transaction_size(self, ep);
}
/** @} */

#endif /* HAL_USE_USB == TRUE */

#endif /* HAL_USB_H */

/** @} */
