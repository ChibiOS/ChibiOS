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
 * @file        hal_serial_usb.h
 * @brief       Generated Serial over USB Driver header.
 * @note        This is a generated file, do not edit directly.
 *
 * @addtogroup  HAL_SERIAL_USB
 * @brief       Serial over USB driver.
 * @details     This module implements a CDC-ACM serial channel on top of the
 *              generic USB CDC service class. The USB driver and device binder
 *              remain external; this module adds buffered channel semantics,
 *              queue handling, and serial-oriented flow control on top of the
 *              CDC transport.
 * @{
 */

#ifndef HAL_SERIAL_USB_H
#define HAL_SERIAL_USB_H

#include "hal.h"

#if (!defined(SERIAL_USB_USE_MODULE) || (SERIAL_USB_USE_MODULE == TRUE)) || defined(__DOXYGEN__)

/*===========================================================================*/
/* Module constants.                                                         */
/*===========================================================================*/

/**
 * @name    Serial USB state aliases
 * @{
 */
#define SDU_UNINIT                          USB_CDC_SERVICE_UNINIT
#define SDU_STOP                            USB_CDC_SERVICE_STOP
#define SDU_READY                           USB_CDC_SERVICE_READY
/** @} */

/*===========================================================================*/
/* Module pre-compile time settings.                                         */
/*===========================================================================*/

/**
 * @name    Configuration options
 * @{
 */
/**
 * @brief       Serial USB driver enable switch.
 */
#if !defined(SERIAL_USB_USE_MODULE) || defined(__DOXYGEN__)
#define SERIAL_USB_USE_MODULE               TRUE
#endif

/**
 * @brief       Size of each TX/RX staging area.
 */
#if !defined(SERIAL_USB_BUFFERS_SIZE) || defined(__DOXYGEN__)
#define SERIAL_USB_BUFFERS_SIZE             256U
#endif

/**
 * @brief       Number of queue-sized staging buffers.
 */
#if !defined(SERIAL_USB_BUFFERS_NUMBER) || defined(__DOXYGEN__)
#define SERIAL_USB_BUFFERS_NUMBER           2U
#endif

/**
 * @brief       Enables zero-length packet transmission when required.
 */
#if !defined(SERIAL_USB_SEND_ZLP) || defined(__DOXYGEN__)
#define SERIAL_USB_SEND_ZLP                 TRUE
#endif

/**
 * @brief       Limits OUT transfers to endpoint packet size.
 */
#if !defined(SERIAL_USB_RX_PACKET_MODE) || defined(__DOXYGEN__)
#define SERIAL_USB_RX_PACKET_MODE           FALSE
#endif
/** @} */

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

#if (SERIAL_USB_SEND_ZLP != FALSE) && (SERIAL_USB_SEND_ZLP != TRUE)
#error "invalid SERIAL_USB_SEND_ZLP value"
#endif

#if (SERIAL_USB_RX_PACKET_MODE != FALSE) && (SERIAL_USB_RX_PACKET_MODE != TRUE)
#error "invalid SERIAL_USB_RX_PACKET_MODE value"
#endif

/*===========================================================================*/
/* Module macros.                                                            */
/*===========================================================================*/

/*===========================================================================*/
/* Module data structures and types.                                         */
/*===========================================================================*/

typedef usb_cdc_service_state_t sdustate_t;
typedef usb_cdc_linecoding_t sdu_linecoding_t;
typedef hal_usb_cdc_config_t SerialUSBConfig;
typedef struct hal_serial_usb_driver SerialUSBDriver;

/**
 * @class       hal_serial_usb_driver_c
 * @extends     hal_usb_cdc_service_c
 * @implements  asynchronous_channel_i
 *
 * @brief       CDC serial channel service class.
 * @details     This class adds buffered asynchronous-channel behavior on top
 *              of the generic USB CDC service base class.
 *
 * @name        Class @p hal_serial_usb_driver_c structures
 * @{
 */

/**
 * @brief       Type of a serial over USB driver class.
 */
typedef struct hal_serial_usb_driver hal_serial_usb_driver_c;

/**
 * @brief       Class @p hal_serial_usb_driver_c virtual methods table.
 */
struct hal_serial_usb_driver_vmt {
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
  /* From hal_serial_usb_driver_c.*/
};

/**
 * @brief       Structure representing a serial over USB driver class.
 */
struct hal_serial_usb_driver {
  /**
   * @brief       Virtual Methods Table.
   */
  const struct hal_serial_usb_driver_vmt *vmt;
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
  /**
   * @brief       Implemented interface @p asynchronous_channel_i.
   */
  asynchronous_channel_i    chn;
  /**
   * @brief       Input queue.
   */
  input_queue_t             iqueue;
  /**
   * @brief       Output queue.
   */
  output_queue_t            oqueue;
  /**
   * @brief       I/O condition event source.
   */
  event_source_t            event;
  /**
   * @brief       Pending channel flags.
   */
  volatile chnflags_t       flags;
  /**
   * @brief       Last transmitted bulk transfer size.
   */
  size_t                    last_txsize;
  /**
   * @brief       Optional thread waiting for TX drain.
   */
  thread_reference_t        txsync;
  /**
   * @brief       Input queue storage.
   */
  uint8_t                   ib[SERIAL_USB_BUFFERS_NUMBER * SERIAL_USB_BUFFERS_SIZE];
  /**
   * @brief       Output queue storage.
   */
  uint8_t                   ob[SERIAL_USB_BUFFERS_NUMBER * SERIAL_USB_BUFFERS_SIZE];
  /**
   * @brief       Temporary receive staging buffer.
   */
  uint8_t                   rxbuf[SERIAL_USB_BUFFERS_SIZE];
  /**
   * @brief       Temporary transmit staging buffer.
   */
  uint8_t                   txbuf[SERIAL_USB_BUFFERS_SIZE];
};
/** @} */

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#ifdef __cplusplus
extern "C" {
#endif
  /* Methods of hal_serial_usb_driver_c.*/
  void *__sdu_objinit_impl(void *ip, const void *vmt);
  void __sdu_dispose_impl(void *ip);
  void __sdu_unbind_impl(void *ip);
  void __sdu_configured_impl(void *ip);
  void __sdu_disconnected_impl(void *ip);
  void __sdu_wakehook_impl(void *ip);
  void __sdu_sofhook_impl(void *ip);
  void __sdu_data_transmitted_impl(void *ip, usbep_t ep);
  void __sdu_data_received_impl(void *ip, usbep_t ep);
  void __sdu_irq_transmitted_impl(void *ip, usbep_t ep);
  void __sdu_control_line_state_impl(void *ip);
  /* Regular functions.*/
  const sdu_linecoding_t *sduGetLineCodingX(void *ip);
  event_source_t *sduGetEventSourceX(void *ip);
  hal_usb_service_c *sduGetServiceX(void *ip);
  msg_t sduStart(void *ip, const SerialUSBConfig *config);
  void sduStop(void *ip);
#ifdef __cplusplus
}
#endif

/*===========================================================================*/
/* Module inline functions.                                                  */
/*===========================================================================*/

/**
 * @name        Default constructor of hal_serial_usb_driver_c
 * @{
 */
/**
 * @brief       Default initialization function of @p hal_serial_usb_driver_c.
 *
 * @param[out]    self          Pointer to a @p hal_serial_usb_driver_c
 *                              instance to be initialized.
 * @return                      Pointer to the initialized object.
 *
 * @objinit
 */
CC_FORCE_INLINE
static inline hal_serial_usb_driver_c *sduObjectInit(hal_serial_usb_driver_c *self) {
  extern const struct hal_serial_usb_driver_vmt __hal_serial_usb_driver_vmt;

  return __sdu_objinit_impl(self, &__hal_serial_usb_driver_vmt);
}
/** @} */

#endif /* !defined(SERIAL_USB_USE_MODULE) || (SERIAL_USB_USE_MODULE == TRUE) */

#endif /* HAL_SERIAL_USB_H */

/** @} */
