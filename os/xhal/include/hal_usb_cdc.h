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
 * @file        hal_usb_cdc.h
 * @brief       Generated USB CDC Services header.
 * @note        This is a generated file, do not edit directly.
 *
 * @addtogroup  HAL_USB_CDC
 * @brief       USB CDC service support classes and helpers.
 * @{
 */

#ifndef HAL_USB_CDC_H
#define HAL_USB_CDC_H

#include "hal_usb.h"

#if (HAL_USE_USB == TRUE) || defined(__DOXYGEN__)

/*===========================================================================*/
/* Module constants.                                                         */
/*===========================================================================*/

/**
 * @name    CDC specific messages
 * @{
 */
#define CDC_SEND_ENCAPSULATED_COMMAND       0x00U
#define CDC_GET_ENCAPSULATED_RESPONSE       0x01U
#define CDC_SET_COMM_FEATURE                0x02U
#define CDC_GET_COMM_FEATURE                0x03U
#define CDC_CLEAR_COMM_FEATURE              0x04U
#define CDC_SET_AUX_LINE_STATE              0x10U
#define CDC_SET_HOOK_STATE                  0x11U
#define CDC_PULSE_SETUP                     0x12U
#define CDC_SEND_PULSE                      0x13U
#define CDC_SET_PULSE_TIME                  0x14U
#define CDC_RING_AUX_JACK                   0x15U
#define CDC_SET_LINE_CODING                 0x20U
#define CDC_GET_LINE_CODING                 0x21U
#define CDC_SET_CONTROL_LINE_STATE          0x22U
#define CDC_SEND_BREAK                      0x23U
#define CDC_SET_RINGER_PARMS                0x30U
#define CDC_GET_RINGER_PARMS                0x31U
#define CDC_SET_OPERATION_PARMS             0x32U
#define CDC_GET_OPERATION_PARMS             0x33U
/** @} */

/**
 * @name    CDC classes
 * @{
 */
#define CDC_COMMUNICATION_INTERFACE_CLASS   0x02U
#define CDC_DATA_INTERFACE_CLASS            0x0AU
/** @} */

/**
 * @name    CDC subclasses
 * @{
 */
#define CDC_ABSTRACT_CONTROL_MODEL          0x02U
/** @} */

/**
 * @name    CDC descriptors
 * @{
 */
#define CDC_CS_INTERFACE                    0x24U
/** @} */

/**
 * @name    CDC subdescriptors
 * @{
 */
#define CDC_HEADER                          0x00U
#define CDC_CALL_MANAGEMENT                 0x01U
#define CDC_ABSTRACT_CONTROL_MANAGEMENT     0x02U
#define CDC_UNION                           0x06U
/** @} */

/**
 * @name    Line control bit definitions
 * @{
 */
#define LC_STOP_1                           0U
#define LC_STOP_1P5                         1U
#define LC_STOP_2                           2U
#define LC_PARITY_NONE                      0U
#define LC_PARITY_ODD                       1U
#define LC_PARITY_EVEN                      2U
#define LC_PARITY_MARK                      3U
#define LC_PARITY_SPACE                     4U
/** @} */

/**
 * @name    Default CDC line coding
 * @{
 */
#define CDC_DEFAULT_DTE_RATE                38400U
#define CDC_DEFAULT_CHAR_FORMAT             LC_STOP_1
#define CDC_DEFAULT_PARITY                  LC_PARITY_NONE
#define CDC_DEFAULT_DATA_BITS               8U
/** @} */

/*===========================================================================*/
/* Module pre-compile time settings.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

/*===========================================================================*/
/* Module macros.                                                            */
/*===========================================================================*/

/*===========================================================================*/
/* Module data structures and types.                                         */
/*===========================================================================*/

typedef enum {
  USB_CDC_SERVICE_UNINIT = 0,
  USB_CDC_SERVICE_STOP   = 1,
  USB_CDC_SERVICE_READY  = 2
} usb_cdc_service_state_t;

typedef struct {
  uint8_t                   dwDTERate[4];
  uint8_t                   bCharFormat;
  uint8_t                   bParityType;
  uint8_t                   bDataBits;
} usb_cdc_linecoding_t;

typedef struct hal_usb_cdc_service hal_usb_cdc_service_c;
typedef struct hal_usb_cdc_config hal_usb_cdc_config_t;

struct hal_usb_cdc_config {
  hal_usb_driver_c         *usbp;
  uint8_t                   control_if;
  usbep_t                   bulk_in;
  usbep_t                   bulk_out;
  usbep_t                   int_in;
  uint16_t                  bulk_in_maxsize;
  uint16_t                  bulk_out_maxsize;
  uint16_t                  int_in_maxsize;
};

/**
 * @class       hal_usb_cdc_service_c
 * @extends     hal_usb_service_c
 *
 * @brief       Abstract CDC-ACM USB service base class.
 * @details     This abstract class implements the common CDC-ACM service logic
 *              shared by higher-level wrappers such as serial-over-USB
 *              channels. The class provides endpoint setup, ownership
 *              metadata, class-request handling, and USB event dispatch.
 *              Derived classes only need to implement the actual
 *              bulk/interrupt data-path behavior.
 *
 * @name        Class @p hal_usb_cdc_service_c structures
 * @{
 */

/**
 * @brief       Type of a USB CDC service class.
 */
typedef struct hal_usb_cdc_service hal_usb_cdc_service_c;

/**
 * @brief       Class @p hal_usb_cdc_service_c virtual methods table.
 */
struct hal_usb_cdc_service_vmt {
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
  /* From hal_usb_cdc_service_c.*/
  void (*configured)(void *ip);
  void (*disconnected)(void *ip);
  void (*wakehook)(void *ip);
  void (*sofhook)(void *ip);
  void (*data_transmitted)(void *ip, usbep_t ep);
  void (*data_received)(void *ip, usbep_t ep);
  void (*irq_transmitted)(void *ip, usbep_t ep);
  void (*control_line_state)(void *ip);
};

/**
 * @brief       Structure representing a USB CDC service class.
 */
struct hal_usb_cdc_service {
  /**
   * @brief       Virtual Methods Table.
   */
  const struct hal_usb_cdc_service_vmt *vmt;
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
  /**
   * @brief       Active CDC service configuration.
   */
  const hal_usb_cdc_config_t * config;
  /**
   * @brief       CDC service state.
   */
  usb_cdc_service_state_t   state;
  /**
   * @brief       Connection state as observed by the USB service logic.
   */
  bool                      connected;
  /**
   * @brief       Current CDC line coding state.
   */
  usb_cdc_linecoding_t      linecoding;
  /**
   * @brief       Current CDC control line state bitmap.
   */
  uint16_t                  control_line_state;
  /**
   * @brief       Bulk IN endpoint state.
   */
  USBInEndpointState        bulk_in_state;
  /**
   * @brief       Bulk OUT endpoint state.
   */
  USBOutEndpointState       bulk_out_state;
  /**
   * @brief       Interrupt IN endpoint state.
   */
  USBInEndpointState        int_in_state;
  /**
   * @brief       Bulk IN endpoint configuration.
   */
  USBEndpointConfig         bulk_in_epc;
  /**
   * @brief       Bulk OUT endpoint configuration.
   */
  USBEndpointConfig         bulk_out_epc;
  /**
   * @brief       Interrupt IN endpoint configuration.
   */
  USBEndpointConfig         int_in_epc;
  /**
   * @brief       Mutable CDC service ownership metadata.
   */
  hal_usb_service_info_t    svcinfo;
};
/** @} */

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#ifdef __cplusplus
extern "C" {
#endif
  /* Methods of hal_usb_cdc_service_c.*/
  void *__cdcsvc_objinit_impl(void *ip, const void *vmt);
  void __cdcsvc_dispose_impl(void *ip);
  msg_t __cdcsvc_bind_impl(void *ip, struct hal_usb_binder *binderp);
  void __cdcsvc_unbind_impl(void *ip);
  void __cdcsvc_reset_impl(void *ip);
  void __cdcsvc_configure_impl(void *ip);
  void __cdcsvc_unconfigure_impl(void *ip);
  void __cdcsvc_suspend_impl(void *ip);
  void __cdcsvc_wakeup_impl(void *ip);
  void __cdcsvc_sof_impl(void *ip);
  void __cdcsvc_in_impl(void *ip, usbep_t ep);
  void __cdcsvc_out_impl(void *ip, usbep_t ep);
  msg_t __cdcsvc_setup_impl(void *ip, bool *handledp);
  void __cdcsvc_configured_impl(void *ip);
  void __cdcsvc_disconnected_impl(void *ip);
  void __cdcsvc_wakehook_impl(void *ip);
  void __cdcsvc_sofhook_impl(void *ip);
  void __cdcsvc_data_transmitted_impl(void *ip, usbep_t ep);
  void __cdcsvc_data_received_impl(void *ip, usbep_t ep);
  void __cdcsvc_irq_transmitted_impl(void *ip, usbep_t ep);
  void __cdcsvc_control_line_state_impl(void *ip);
  /* Regular functions.*/
  msg_t usbCdcServiceStart(void *ip, const hal_usb_cdc_config_t *config);
  void usbCdcServiceStop(void *ip);
  const usb_cdc_linecoding_t *usbCdcServiceGetLineCodingX(void *ip);
  uint16_t usbCdcServiceGetControlLineStateX(void *ip);
  const hal_usb_cdc_config_t *usbCdcServiceGetConfigX(void *ip);
  bool usbCdcServiceIsConnectedX(void *ip);
#ifdef __cplusplus
}
#endif

/*===========================================================================*/
/* Module inline functions.                                                  */
/*===========================================================================*/

/**
 * @name        Virtual methods of hal_usb_cdc_service_c
 * @{
 */
/**
 * @brief       Derived hook after configuration selection.
 * @details     Called with the system lock held after the CDC endpoints have
 *              been initialized and the service state marked connected.
 *
 * @param[in,out] ip            Pointer to a @p hal_usb_cdc_service_c instance.
 *
 * @iclass
 */
CC_FORCE_INLINE
static inline void usbCdcServiceOnConfiguredI(void *ip) {
  hal_usb_cdc_service_c *self = (hal_usb_cdc_service_c *)ip;

  self->vmt->configured(ip);
}

/**
 * @brief       Derived hook after reset, unconfigure, or suspend.
 * @details     Called with the system lock held after the CDC service has been
 *              marked disconnected.
 *
 * @param[in,out] ip            Pointer to a @p hal_usb_cdc_service_c instance.
 *
 * @iclass
 */
CC_FORCE_INLINE
static inline void usbCdcServiceOnDisconnectedI(void *ip) {
  hal_usb_cdc_service_c *self = (hal_usb_cdc_service_c *)ip;

  self->vmt->disconnected(ip);
}

/**
 * @brief       Derived hook after bus wake-up.
 *
 * @param[in,out] ip            Pointer to a @p hal_usb_cdc_service_c instance.
 *
 * @iclass
 */
CC_FORCE_INLINE
static inline void usbCdcServiceOnWakeupHookI(void *ip) {
  hal_usb_cdc_service_c *self = (hal_usb_cdc_service_c *)ip;

  self->vmt->wakehook(ip);
}

/**
 * @brief       Derived hook on each SOF notification.
 *
 * @param[in,out] ip            Pointer to a @p hal_usb_cdc_service_c instance.
 *
 * @iclass
 */
CC_FORCE_INLINE
static inline void usbCdcServiceOnSOFHookI(void *ip) {
  hal_usb_cdc_service_c *self = (hal_usb_cdc_service_c *)ip;

  self->vmt->sofhook(ip);
}

/**
 * @brief       Derived hook on bulk IN completion.
 *
 * @param[in,out] ip            Pointer to a @p hal_usb_cdc_service_c instance.
 * @param[in]     ep            Endpoint number.
 *
 * @iclass
 */
CC_FORCE_INLINE
static inline void usbCdcServiceOnDataTransmittedI(void *ip, usbep_t ep) {
  hal_usb_cdc_service_c *self = (hal_usb_cdc_service_c *)ip;

  self->vmt->data_transmitted(ip, ep);
}

/**
 * @brief       Derived hook on bulk OUT completion.
 *
 * @param[in,out] ip            Pointer to a @p hal_usb_cdc_service_c instance.
 * @param[in]     ep            Endpoint number.
 *
 * @iclass
 */
CC_FORCE_INLINE
static inline void usbCdcServiceOnDataReceivedI(void *ip, usbep_t ep) {
  hal_usb_cdc_service_c *self = (hal_usb_cdc_service_c *)ip;

  self->vmt->data_received(ip, ep);
}

/**
 * @brief       Derived hook on interrupt IN completion.
 *
 * @param[in,out] ip            Pointer to a @p hal_usb_cdc_service_c instance.
 * @param[in]     ep            Endpoint number.
 *
 * @iclass
 */
CC_FORCE_INLINE
static inline void usbCdcServiceOnInterruptTransmittedI(void *ip, usbep_t ep) {
  hal_usb_cdc_service_c *self = (hal_usb_cdc_service_c *)ip;

  self->vmt->irq_transmitted(ip, ep);
}

/**
 * @brief       Derived hook after control line state changes.
 *
 * @param[in,out] ip            Pointer to a @p hal_usb_cdc_service_c instance.
 */
CC_FORCE_INLINE
static inline void usbCdcServiceOnControlLineState(void *ip) {
  hal_usb_cdc_service_c *self = (hal_usb_cdc_service_c *)ip;

  self->vmt->control_line_state(ip);
}
/** @} */

#endif /* HAL_USE_USB == TRUE */

#endif /* HAL_USB_CDC_H */

/** @} */
