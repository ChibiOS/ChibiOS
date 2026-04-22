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
 * @file    hal_serial_usb.h
 * @brief   Serial over USB Driver macros and structures.
 *
 * @addtogroup HAL_SERIAL_USB
 * @{
 */

#ifndef HAL_SERIAL_USB_H
#define HAL_SERIAL_USB_H

#include "hal.h"

#if !defined(SERIAL_USB_USE_MODULE) || defined(__DOXYGEN__)
#define SERIAL_USB_USE_MODULE            TRUE
#endif

#if (SERIAL_USB_USE_MODULE == TRUE) || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver constants.                                                         */
/*===========================================================================*/

#if !defined(CDC_SET_LINE_CODING) || defined(__DOXYGEN__)
#define CDC_SET_LINE_CODING             0x20U
#endif

#if !defined(CDC_GET_LINE_CODING) || defined(__DOXYGEN__)
#define CDC_GET_LINE_CODING             0x21U
#endif

#if !defined(CDC_SET_CONTROL_LINE_STATE) || defined(__DOXYGEN__)
#define CDC_SET_CONTROL_LINE_STATE      0x22U
#endif

#define SERIAL_USB_DEFAULT_RATE         38400U

/*===========================================================================*/
/* Driver pre-compile time settings.                                         */
/*===========================================================================*/

/**
 * @name    SERIAL_USB configuration options
 * @{
 */
#if !defined(SERIAL_USB_BUFFERS_SIZE) || defined(__DOXYGEN__)
#define SERIAL_USB_BUFFERS_SIZE         256U
#endif

#if !defined(SERIAL_USB_BUFFERS_NUMBER) || defined(__DOXYGEN__)
#define SERIAL_USB_BUFFERS_NUMBER       2U
#endif

#if !defined(SERIAL_USB_SEND_ZLP) || defined(__DOXYGEN__)
#define SERIAL_USB_SEND_ZLP             TRUE
#endif

#if !defined(SERIAL_USB_RX_PACKET_MODE) || defined(__DOXYGEN__)
#define SERIAL_USB_RX_PACKET_MODE       FALSE
#endif
/** @} */

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

#if (HAL_USE_USB == FALSE)
#error "Serial USB requires HAL_USE_USB"
#endif

#if (SERIAL_USB_SEND_ZLP != FALSE) && (SERIAL_USB_SEND_ZLP != TRUE)
#error "invalid SERIAL_USB_SEND_ZLP value"
#endif

#if (SERIAL_USB_RX_PACKET_MODE != FALSE) && (SERIAL_USB_RX_PACKET_MODE != TRUE)
#error "invalid SERIAL_USB_RX_PACKET_MODE value"
#endif

/*===========================================================================*/
/* Driver data structures and types.                                         */
/*===========================================================================*/

/**
 * @brief Driver state machine possible states.
 */
typedef enum {
  SDU_UNINIT = 0,
  SDU_STOP   = 1,
  SDU_READY  = 2
} sdustate_t;

/**
 * @brief   Type of CDC line coding structure.
 */
typedef struct {
  uint8_t                   dwDTERate[4];
  uint8_t                   bCharFormat;
  uint8_t                   bParityType;
  uint8_t                   bDataBits;
} sdu_linecoding_t;

/**
 * @brief   Structure representing a serial over USB driver.
 */
typedef struct hal_serial_usb_driver SerialUSBDriver;
typedef struct hal_serial_usb_driver hal_serial_usb_driver_c;

/**
 * @brief   Serial over USB Driver configuration structure.
 * @details The USB driver and descriptors remain externally owned. This
 *          structure only describes the CDC-ACM function wiring used by the
 *          serial layer.
 */
typedef struct hal_serial_usb_config {
  /**
   * @brief   USB driver to use.
   */
  hal_usb_driver_c         *usbp;
  /**
   * @brief   Control interface number associated to this CDC function.
   */
  uint8_t                   control_if;
  /**
   * @brief   Bulk IN endpoint used for outgoing data transfer.
   */
  usbep_t                   bulk_in;
  /**
   * @brief   Bulk OUT endpoint used for incoming data transfer.
   */
  usbep_t                   bulk_out;
  /**
   * @brief   Interrupt IN endpoint used for notifications.
   * @note    If set to zero then the endpoint is considered absent.
   */
  usbep_t                   int_in;
  /**
   * @brief   Bulk IN endpoint maximum packet size.
   */
  uint16_t                  bulk_in_maxsize;
  /**
   * @brief   Bulk OUT endpoint maximum packet size.
   */
  uint16_t                  bulk_out_maxsize;
  /**
   * @brief   Interrupt IN endpoint maximum packet size.
   * @note    Ignored when @p int_in is zero.
   */
  uint16_t                  int_in_maxsize;
} SerialUSBConfig;

/**
 * @brief   Structure representing a serial over USB driver.
 */
struct hal_serial_usb_driver {
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
   * @brief       Cached channel flags.
   */
  volatile chnflags_t       flags;
  /**
   * @brief       Driver state.
   */
  sdustate_t                state;
  /**
   * @brief       Driver connectivity state.
   */
  bool                      connected;
  /**
   * @brief       Current configuration data.
   */
  const SerialUSBConfig     *config;
  /**
   * @brief       Current line coding.
   */
  sdu_linecoding_t          linecoding;
  /**
   * @brief       Notification endpoint line state cache.
   */
  uint16_t                  control_line_state;
  /**
   * @brief       Last transmitted packet size.
   */
  size_t                    last_txsize;
  /**
   * @brief       Synchronization point for TX completion.
   */
  thread_reference_t        txsync;
  /**
   * @brief       Input queue backing storage.
   */
  uint8_t                   ib[SERIAL_USB_BUFFERS_NUMBER *
                               SERIAL_USB_BUFFERS_SIZE];
  /**
   * @brief       Output queue backing storage.
   */
  uint8_t                   ob[SERIAL_USB_BUFFERS_NUMBER *
                               SERIAL_USB_BUFFERS_SIZE];
  /**
   * @brief       Staging buffer for OUT transactions.
   */
  uint8_t                   rxbuf[SERIAL_USB_BUFFERS_SIZE];
  /**
   * @brief       Staging buffer for IN transactions.
   */
  uint8_t                   txbuf[SERIAL_USB_BUFFERS_SIZE];
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
};

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#ifdef __cplusplus
extern "C" {
#endif
  void sduObjectInit(SerialUSBDriver *sdup);
  msg_t sduStart(SerialUSBDriver *sdup, const SerialUSBConfig *config);
  void sduStop(SerialUSBDriver *sdup);
  void sduSuspendHookI(SerialUSBDriver *sdup);
  void sduWakeupHookI(SerialUSBDriver *sdup);
  void sduConfigureHookI(SerialUSBDriver *sdup);
  msg_t sduRequestsHook(SerialUSBDriver *sdup, bool *handledp);
  void sduSOFHookI(SerialUSBDriver *sdup);
  void sduDataTransmitted(hal_usb_driver_c *usbp, usbep_t ep);
  void sduDataReceived(hal_usb_driver_c *usbp, usbep_t ep);
  void sduInterruptTransmitted(hal_usb_driver_c *usbp, usbep_t ep);
#ifdef __cplusplus
}
#endif

/*===========================================================================*/
/* Driver inline functions.                                                  */
/*===========================================================================*/

/**
 * @brief   Returns the current line coding.
 *
 * @param[in] sdup      pointer to a @p SerialUSBDriver object
 * @return              Pointer to the cached line coding.
 */
CC_FORCE_INLINE
static inline const sdu_linecoding_t *sduGetLineCodingX(SerialUSBDriver *sdup) {
  return &sdup->linecoding;
}

/**
 * @brief   Returns the event source associated to the driver.
 *
 * @param[in] sdup      pointer to a @p SerialUSBDriver object
 * @return              Pointer to the event source.
 */
CC_FORCE_INLINE
static inline event_source_t *sduGetEventSourceX(SerialUSBDriver *sdup) {
  return &sdup->event;
}

#endif /* SERIAL_USB_USE_MODULE == TRUE */

#endif /* HAL_SERIAL_USB_H */

/** @} */
