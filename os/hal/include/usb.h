/*
    ChibiOS/RT - Copyright (C) 2006,2007,2008,2009,2010 Giovanni Di Sirio.

    This file is part of ChibiOS/RT.

    ChibiOS/RT is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 3 of the License, or
    (at your option) any later version.

    ChibiOS/RT is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

/**
 * @file    usb.h
 * @brief   USB Driver macros and structures.
 *
 * @addtogroup USB
 * @{
 */

#ifndef _USB_H_
#define _USB_H_

#if HAL_USE_USB || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver constants.                                                         */
/*===========================================================================*/

#define USB_RTYPE_DIR_MASK                  0x80
#define USB_RTYPE_DIR_HOST2DEV              0x00
#define USB_RTYPE_DIR_DEV2HOST              0x80
#define USB_RTYPE_TYPE_MASK                 0x60
#define USB_RTYPE_TYPE_STD                  0x00
#define USB_RTYPE_TYPE_CLASS                0x20
#define USB_RTYPE_TYPE_VENDOR               0x40
#define USB_RTYPE_TYPE_RESERVED             0x60
#define USB_RTYPE_RECIPIENT_MASK            0x1F
#define USB_RTYPE_RECIPIENT_DEVICE          0x00
#define USB_RTYPE_RECIPIENT_INTERFACE       0x01
#define USB_RTYPE_RECIPIENT_ENDPOINT        0x02
#define USB_RTYPE_RECIPIENT_OTHER           0x03

#define USB_REQ_GET_STATUS                  0
#define USB_REQ_CLEAR_FEATURE               1
#define USB_REQ_SET_FEATURE                 3
#define USB_REQ_SET_ADDRESS                 5
#define USB_REQ_GET_DESCRIPTOR              6
#define USB_REQ_SET_DESCRIPTOR              7
#define USB_REQ_GET_CONFIGURATION           8
#define USB_REQ_SET_CONFIGURATION           9
#define USB_REQ_GET_INTERFACE               10
#define USB_REQ_SET_INTERFACE               11
#define USB_REQ_SYNCH_FRAME                 12

#define USB_DESCRIPTOR_DEVICE               1
#define USB_DESCRIPTOR_CONFIGURATION        2
#define USB_DESCRIPTOR_STRING               3
#define USB_DESCRIPTOR_INTERFACE            4
#define USB_DESCRIPTOR_ENDPOINT             5
#define USB_DESCRIPTOR_DEVICE_QUALIFIER     6
#define USB_DESCRIPTOR_OTHER_SPEED_CFG      7
#define USB_DESCRIPTOR_INTERFACE_POWER      8

#define USB_FEATURE_ENDPOINT_HALT           0
#define USB_FEATURE_DEVICE_REMOTE_WAKEUP    1
#define USB_FEATURE_TEST_MODE               2

#define USB_EARLY_SET_ADDRESS               0
#define USB_LATE_SET_ADDRESS                1

/**
 * @brief   Returned by some functions to report a busy endpoint.
 */
#define USB_ENDPOINT_BUSY                   ((size_t)0xFFFFFFFF)

#define USB_EP_MODE_TYPE                0x0003  /**< Endpoint type mask.    */
#define USB_EP_MODE_TYPE_CTRL           0x0000  /**< Control endpoint.      */
#define USB_EP_MODE_TYPE_ISOC           0x0001  /**< Isochronous endpoint.  */
#define USB_EP_MODE_TYPE_BULK           0x0002  /**< Bulk endpoint.         */
#define USB_EP_MODE_TYPE_INTR           0x0003  /**< Interrupt endpoint.    */
#define USB_EP_MODE_TRANSACTION         0x0000  /**< Transaction mode.      */
#define USB_EP_MODE_PACKET              0x0010  /**< Packet mode enabled.   */

/*===========================================================================*/
/* Driver pre-compile time settings.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

/*===========================================================================*/
/* Driver data structures and types.                                         */
/*===========================================================================*/

/**
 * @brief   Type of a structure representing an USB driver.
 */
typedef struct USBDriver USBDriver;

/**
 * @brief   Type of an endpoint identifier.
 */
typedef uint8_t usbep_t;

/**
 * @brief   Type of a driver state machine possible states.
 */
typedef enum {
  USB_UNINIT   = 0,                     /**< Not initialized.               */
  USB_STOP     = 1,                     /**< Stopped.                       */
  USB_READY    = 2,                     /**< Ready, after bus reset.        */
  USB_SELECTED = 3,                     /**< Address assigned.              */
  USB_ACTIVE   = 4,                     /**< Active, configuration selected.*/
} usbstate_t;

/**
 * @brief   Type of an endpoint status.
 */
typedef enum {
  EP_STATUS_DISABLED = 0,               /**< Endpoint not active.           */
  EP_STATUS_STALLED = 1,                /**< Endpoint opened but stalled.   */
  EP_STATUS_ACTIVE = 2                  /**< Active endpoint.               */
} usbepstatus_t;

/**
 * @brief   Type of an endpoint zero state machine states.
 */
typedef enum {
  USB_EP0_WAITING_SETUP,                /**< Waiting for SETUP data.        */
  USB_EP0_TX,                           /**< Trasmitting.                   */
  USB_EP0_WAITING_STS,                  /**< Waiting status.                */
  USB_EP0_RX,                           /**< Receiving.                     */
  USB_EP0_SENDING_STS,                  /**< Sending status.                */
  USB_EP0_ERROR                         /**< Error, EP0 stalled.            */
} usbep0state_t;

/**
 * @brief   Type of an enumeration of the possible USB events.
 */
typedef enum {
  USB_EVENT_RESET = 0,                  /**< Driver has been reset by host. */
  USB_EVENT_ADDRESS = 1,                /**< Address assigned.              */
  USB_EVENT_CONFIGURED = 2,             /**< Configuration selected.        */
  USB_EVENT_SUSPEND = 3,                /**< Entering suspend mode.         */
  USB_EVENT_RESUME = 4,                 /**< Leaving suspend mode.          */
  USB_EVENT_STALLED = 5,                /**< Endpoint 0 error, stalled.     */
} usbevent_t;

/**
 * @brief   Type of an USB descriptor.
 */
typedef struct {
  /**
   * @brief   Descriptor size in unicode characters.
   */
  size_t                        ud_size;
  /**
   * @brief   Pointer to the descriptor.
   */
  const uint8_t                 *ud_string;
} USBDescriptor;

/**
 * @brief   Type of an USB generic notification callback.
 *
 * @param[in] usbp      pointer to the @p USBDriver object triggering the
 *                      callback
 */
typedef void (*usbcallback_t)(USBDriver *usbp);

/**
 * @brief   Type of an USB endpoint callback.
 *
 * @param[in] usbp      pointer to the @p USBDriver object triggering the
 *                      callback
 * @param[in] ep        endpoint number
 */
typedef void (*usbepcallback_t)(USBDriver *usbp, usbep_t ep);

/**
 * @brief   Type of an USB event notification callback.
 *
 * @param[in] usbp      pointer to the @p USBDriver object triggering the
 *                      callback
 * @param[in] event     event type
 */
typedef void (*usbeventcb_t)(USBDriver *usbp, usbevent_t event);

/**
 * @brief   Type of a requests handler callback.
 * @details The request is encoded in the @p usb_setup buffer.
 *
 * @param[in] usbp      pointer to the @p USBDriver object triggering the
 *                      callback
 * @return              The request handling exit code.
 * @retval FALSE        Request not recognized by the handler.
 * @retval TRUE         Request handled.
 */
typedef bool_t (*usbreqhandler_t)(USBDriver *usbp);

/**
 * @brief   Type of an USB descriptor-retrieving callback.
 */
typedef const USBDescriptor * (*usbgetdescriptor_t)(USBDriver *usbp,
                                                    uint8_t dtype,
                                                    uint8_t dindex,
                                                    uint16_t lang);

#include "usb_lld.h"

/*===========================================================================*/
/* Driver macros.                                                            */
/*===========================================================================*/
/**
 * @brief   Returns the current frame number.
 *
 * @param[in] usbp      pointer to the @p USBDriver object
 * @return              The current frame number.
 *
 * @api
 */
#define usbGetFrameNumber(usbp) usb_lld_get_frame_number(usbp)

/**
 * @brief   Returns the status of an IN endpoint.
 *
 * @param[in] usbp      pointer to the @p USBDriver object
 * @param[in] ep        endpoint number
 * @return              The operation status.
 * @retval FALSE        Endpoint ready.
 * @retval TRUE         Endpoint transmitting.
 *
 * @iclass
 */
#define usbGetTransmitStatusI(usbp, ep) ((usbp)->transmitting & (1 << (ep)))

/**
 * @brief   Returns the status of an OUT endpoint.
 *
 * @param[in] usbp      pointer to the @p USBDriver object
 * @param[in] ep        endpoint number
 * @return              The operation status.
 * @retval FALSE        Endpoint ready.
 * @retval TRUE         Endpoint receiving.
 *
 * @iclass
 */
#define usbGetReceiveStatusI(usbp, ep) ((usbp)->receiving & (1 << (ep)))

/**
 * @brief   Returns the exact size of a receive transaction.
 * @details The received size can be different from the size specified in
 *          @p usbStartReceiveI() because the last packet could have a size
 *          different from the expected one.
 * @pre     The OUT endpoint must have been configured in transaction mode
 *          in order to use this function.
 *
 * @param[in] usbp      pointer to the @p USBDriver object
 * @param[in] ep        endpoint number
 * @return              Received data size.
 *
 * @iclass
 */
#define usbGetReceiveTransactionSizeI(usbp, ep)                             \
  usb_lld_get_transaction_size(usbp, ep)

/**
 * @brief   Returns the exact size of a received packet.
 * @pre     The OUT endpoint must have been configured in packet mode
 *          in order to use this function.
 *
 * @param[in] usbp      pointer to the @p USBDriver object
 * @param[in] ep        endpoint number
 * @return              Received data size.
 *
 * @iclass
 */
#define usbGetReceivePacketSizeI(usbp, ep)                                  \
  usb_lld_get_packet_size(usbp, ep)

/**
 * @brief   Request transfer setup.
 * @details This macro is used by the request handling callbacks in order to
 *          prepare a transaction over the endpoint zero.
 *
 * @param[in] usbp      pointer to the @p USBDriver object
 * @param[in] buf       pointer to a buffer for the transaction data
 * @param[in] n         number of bytes to be transferred
 *
 * @api
 */
#define usbSetupTransfer(usbp, buf, n, endcb) {                             \
  (usbp)->ep0next  = (buf);                                                 \
  (usbp)->ep0n     = (n);                                                   \
  (usbp)->ep0endcb = (endcb);                                               \
}

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#ifdef __cplusplus
extern "C" {
#endif
  void usbInit(void);
  void usbObjectInit(USBDriver *usbp);
  void usbStart(USBDriver *usbp, const USBConfig *config);
  void usbStop(USBDriver *usbp);
  void usbInitEndpointI(USBDriver *usbp, usbep_t ep,
                        const USBEndpointConfig *epcp);
  void usbDisableEndpointsI(USBDriver *usbp);
  size_t usbReadPacketI(USBDriver *usbp, usbep_t ep,
                        uint8_t *buf, size_t n);
  size_t usbWritePacketI(USBDriver *usbp, usbep_t ep,
                         const uint8_t *buf, size_t n);
  bool_t usbStartReceiveI(USBDriver *usbp, usbep_t ep,
                          uint8_t *buf, size_t n);
  bool_t usbStartTransmitI(USBDriver *usbp, usbep_t ep,
                           const uint8_t *buf, size_t n);
  bool_t usbStallReceiveI(USBDriver *usbp, usbep_t ep);
  bool_t usbStallTransmitI(USBDriver *usbp, usbep_t ep);
  void _usb_reset(USBDriver *usbp);
  void _usb_ep0in(USBDriver *usbp, usbep_t ep);
  void _usb_ep0out(USBDriver *usbp, usbep_t ep);
#ifdef __cplusplus
}
#endif

#endif /* HAL_USE_USB */

#endif /* _USB_H_ */

/** @} */
