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
 * @file    STM32/usb_lld.h
 * @brief   STM32 USB subsystem low level driver header.
 *
 * @addtogroup USB
 * @{
 */

#ifndef _USB_LLD_H_
#define _USB_LLD_H_

#if HAL_USE_USB || defined(__DOXYGEN__)

#include "stm32_usb.h"

/*===========================================================================*/
/* Driver constants.                                                         */
/*===========================================================================*/

/**
 * @brief   Maximum endpoint address.
 */
#define USB_MAX_ENDPOINTS                   USB_ENDOPOINTS_NUMBER

/*===========================================================================*/
/* Driver pre-compile time settings.                                         */
/*===========================================================================*/

/**
 * @brief   USB1 driver enable switch.
 * @details If set to @p TRUE the support for USB1 is included.
 * @note    The default is @p TRUE.
 */
#if !defined(STM32_USB_USE_USB1) || defined(__DOXYGEN__)
#define STM32_USB_USE_USB1                  TRUE
#endif

/**
 * @brief   USB1 interrupt priority level setting.
 */
#if !defined(STM32_USB_USB1_HP_IRQ_PRIORITY) || defined(__DOXYGEN__)
#define STM32_USB_USB1_HP_IRQ_PRIORITY      6
#endif

/**
 * @brief   USB1 interrupt priority level setting.
 */
#if !defined(STM32_USB_USB1_LP_IRQ_PRIORITY) || defined(__DOXYGEN__)
#define STM32_USB_USB1_LP_IRQ_PRIORITY      14
#endif

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

#if STM32_USB_USE_USB1 && !STM32_HAS_USB
#error "USB not present in the selected device"
#endif

#if !STM32_USB_USE_USB1
#error "USB driver activated but no USB peripheral assigned"
#endif

#if STM32_USBCLK != 48000000
#error "the USB driver requires a 48MHz clock"
#endif

/*===========================================================================*/
/* Driver data structures and types.                                         */
/*===========================================================================*/

/**
 * @brief   Type of an USB Endpoint configuration structure.
 * @note    Platform specific restrictions may apply to endpoints.
 */
typedef struct {
  /**
   * @brief   IN endpoint notification callback.
   */
  usbepcallback_t               uepc_in_cb;
  /**
   * @brief   OUT endpoint notification callback.
   */
  usbepcallback_t               uepc_out_cb;
  /**
   * @brief   IN endpoint maximum packet size.
   */
  uint16_t                      uepc_in_maxsize;
  /**
   * @brief   OUT endpoint maximum packet size.
   */
  uint16_t                      uepc_out_maxsize;
  /* End of the mandatory fields.*/
  /**
   * @brief   EPxR register initialization value.
   * @note    Do not specify the EA field, leave it to zero.
   */
  uint16_t                      uepc_epr;
  /**
   * @brief   Endpoint IN buffer address as offset in the PMA.
   */
  uint16_t                      uepc_inaddr;
  /**
   * @brief   Endpoint OUT buffer address as offset in the PMA.
   */
  uint16_t                      uepc_outaddr;
} USBEndpointConfig;

/**
 * @brief   Type of an USB driver configuration structure.
 */
typedef struct {
  /**
   * @brief   USB events callback.
   * @details This callback is invoked when an USB driver event is registered.
   */
  usbeventcb_t                  uc_event_cb;
  /**
   * @brief   Device GET_DESCRIPTOR request callback.
   * @note    This callback is mandatory and cannot be set to @p NULL.
   */
  usbgetdescriptor_t            uc_get_descriptor_cb;
  /**
   * @brief   Requests hook callback.
   * @details This hook allows to be notified of standard requests or to
   *          handle non standard requests.
   */
  usbreqhandler_t               uc_requests_hook_cb;
  /**
   * @brief   Start Of Frame callback.
   */
  usbcallback_t                 uc_sof_cb;
  /* End of the mandatory fields.*/
} USBConfig;

/**
 * @brief   Structure representing an USB driver.
 */
struct USBDriver {
  /**
   * @brief   Driver state.
   */
  usbstate_t                    usb_state;
  /**
   * @brief   Current configuration data.
   */
  const USBConfig               *usb_config;
  /**
   * @brief   Field available to user, it can be used to associate an
   *          application-defined handler to the USB driver.
   */
  void                          *usb_param;
  /**
   * @brief   Active endpoints configurations.
   */
  const USBEndpointConfig       *usb_epc[USB_MAX_ENDPOINTS + 1];
  /**
   * @brief   Endpoint 0 state.
   */
  usbep0state_t                 usb_ep0state;
  /**
   * @brief   Next position in the buffer to be transferred through endpoint 0.
   */
  uint8_t                       *usb_ep0next;
  /**
   * @brief   Maximum number of bytes to be tranferred through endpoint 0.
   */
  size_t                        usb_ep0max;
  /**
   * @brief   Number of bytes yet to be tranferred through endpoint 0.
   */
  size_t                        usb_ep0n;
  /**
   * @brief   Size of the last packet transferred through endpoint 0.
   */
  size_t                        usb_ep0lastsize;
  /**
   * @brief   Endpoint 0 end transaction callback.
   */
  usbcallback_t                 usb_ep0endcb;
  /**
   * @brief   Setup packet buffer.
   */
  uint8_t                       usb_setup[8];
  /**
   * @brief   Current USB device status.
   */
  uint16_t                      usb_status;
  /**
   * @brief   Assigned USB address.
   */
  uint8_t                       usb_address;
  /**
   * @brief   Current USB device configuration.
   */
  uint8_t                       usb_configuration;
  /* End of the mandatory fields.*/
};

/*===========================================================================*/
/* Driver macros.                                                            */
/*===========================================================================*/

/**
 * @brief   Fetches a 16 bits word value from an USB message.
 *
 * @param[in] p         pointer to the 16 bits word
 *
 * @notapi
 */
#define usb_lld_fetch_word(p) (*(uint16_t *)p)

/**
 * @brief   Returns the current frame number.
 *
 * @param[in] usbp      pointer to the @p USBDriver object
 * @return              The current frame number.
 *
 * @notapi
 */
#define usb_lld_get_frame_number(usbp) (STM32_USB->FNR & FNR_FN_MASK)

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#if STM32_USB_USE_USB1 && !defined(__DOXYGEN__)
extern USBDriver USBD1;
#endif

#if !defined(__DOXYGEN__)
extern const USBEndpointConfig usb_lld_ep0config;
#endif

#ifdef __cplusplus
extern "C" {
#endif
  void usb_lld_init(void);
  void usb_lld_start(USBDriver *usbp);
  void usb_lld_stop(USBDriver *usbp);
  void usb_lld_reset(USBDriver *usbp);
  void usb_lld_set_address(USBDriver *usbp, uint8_t addr);
  void usb_lld_enable_endpoint(USBDriver *usbp, usbep_t ep,
                               const USBEndpointConfig *epcp);
  void usb_lld_disable_endpoints(USBDriver *usbp);
  size_t usb_lld_get_readable(USBDriver *usbp, usbep_t ep);
  size_t usb_lld_read(USBDriver *usbp, usbep_t ep,
                      uint8_t *buf, size_t n);
  size_t usb_lld_write(USBDriver *usbp, usbep_t ep,
                       const uint8_t *buf, size_t n);
  usbepstatus_t usb_lld_get_status_in(USBDriver *usbp, usbep_t ep);
  usbepstatus_t usb_lld_get_status_out(USBDriver *usbp, usbep_t ep);
  void usb_lld_stall_in(USBDriver *usbp, usbep_t ep);
  void usb_lld_stall_out(USBDriver *usbp, usbep_t ep);
  void usb_lld_clear_in(USBDriver *usbp, usbep_t ep);
  void usb_lld_clear_out(USBDriver *usbp, usbep_t ep);
#ifdef __cplusplus
}
#endif

#endif /* HAL_USE_USB */

#endif /* _USB_LLD_H_ */

/** @} */
