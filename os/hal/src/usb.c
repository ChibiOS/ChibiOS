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
 * @file    usb.c
 * @brief   USB Driver code.
 *
 * @addtogroup USB
 * @{
 */

#include <string.h>

#include "ch.h"
#include "hal.h"
#include "usb.h"

#if HAL_USE_USB || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local variables.                                                   */
/*===========================================================================*/

static const uint8_t zero_status[] = {0x00, 0x00};
static const uint8_t active_status[] ={0x00, 0x00};
static const uint8_t halted_status[] = {0x01, 0x00};

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

/**
 * @brief  SET ADDRESS transaction callback.
 *
 * @param[in] usbp      pointer to the @p USBDriver object
 */
void set_address(USBDriver *usbp) {

  usbp->usb_address = usbp->usb_setup[2];
  usb_lld_set_address(usbp, usbp->usb_address);
  if (usbp->usb_config->uc_event_cb)
    usbp->usb_config->uc_event_cb(usbp, USB_EVENT_ADDRESS);
  usbp->usb_state = USB_SELECTED;
}

/**
 * @brief   Starts a receive phase on the endpoint zero.
 *
 * @param[in] usbp      pointer to the @p USBDriver object
 */
static void start_rx_ep0(USBDriver *usbp) {

  if (usbp->usb_ep0n > 0) {
    /* The received data cannot exceed the available amount.*/
    if (usbp->usb_ep0n > usbp->usb_ep0max)
      usbp->usb_ep0n = usbp->usb_ep0max;

    /* Determines the maximum amount that can be received using a
       single packet.*/
    if (usbp->usb_ep0n > usbp->usb_ep[0]->uep_config->uepc_out_maxsize)
      usbp->usb_ep0lastsize = usbp->usb_ep[0]->uep_config->uepc_out_maxsize;
    else
      usbp->usb_ep0lastsize = usbp->usb_ep0n;
    usbp->usb_ep0state = USB_EP0_RX;
  }
  else {
    /* Sending zero sized status packet.*/
    usb_lld_write(usbp, 0, NULL, 0);
    usbp->usb_ep0state = USB_EP0_SENDING_STS;
  }
}

/**
 * @brief   Starts a transmission phase on the endpoint zero.
 *
 * @param[in] usbp      pointer to the @p USBDriver object
 */
static void start_tx_ep0(USBDriver *usbp) {

  if (usbp->usb_ep0n > 0) {
    /* The transmitted data cannot exceed the requested amount.*/
    if (usbp->usb_ep0n > usbp->usb_ep0max)
      usbp->usb_ep0n = usbp->usb_ep0max;

    /* Determines the maximum amount that can be transmitted using a
       single packet.*/
    if (usbp->usb_ep0n > usbp->usb_ep[0]->uep_config->uepc_in_maxsize)
      usbp->usb_ep0lastsize = usbp->usb_ep[0]->uep_config->uepc_in_maxsize;
    else
      usbp->usb_ep0lastsize = usbp->usb_ep0n;

    /* Starts transmission.*/
    usb_lld_write(usbp, 0, usbp->usb_ep0next, usbp->usb_ep0lastsize);
    usbp->usb_ep0state = USB_EP0_TX;
  }
  else
    usbp->usb_ep0state = USB_EP0_WAITING_STS;
}

/**
 * @brief   Standard requests handler.
 * @details This is the standard requests default handler, most standard
 *          requests are handled here, the user can override the standard
 *          handling using the @p uc_requests_hook_cb hook in the
 *          @p USBConfig structure.
 *
 * @param[in] usbp      pointer to the @p USBDriver object
 * @return              The request handling exit code.
 * @retval FALSE        Request not recognized by the handler or error.
 * @retval TRUE         Request handled.
 */
static bool_t default_handler(USBDriver *usbp) {
  const USBDescriptor *dp;

  /* Decoding the request.*/
  switch (((usbp->usb_setup[0] & (USB_RTYPE_RECIPIENT_MASK |
                                 USB_RTYPE_TYPE_MASK)) |
           (usbp->usb_setup[1] << 8))) {
  case USB_RTYPE_RECIPIENT_DEVICE | (USB_REQ_GET_STATUS << 8):
    /* Just returns the current status word.*/
    usbSetupTransfer(usbp, (uint8_t *)&usbp->usb_status, 2, NULL);
    return TRUE;
  case USB_RTYPE_RECIPIENT_DEVICE | (USB_REQ_CLEAR_FEATURE << 8):
    /* Only the DEVICE_REMOTE_WAKEUP is handled here, any other feature
       number is handled as an error.*/
    if (usbp->usb_setup[2] == USB_FEATURE_DEVICE_REMOTE_WAKEUP) {
      usbp->usb_status &= ~2;
      usbSetupTransfer(usbp, NULL, 0, NULL);
      return TRUE;
    }
    return FALSE;
  case USB_RTYPE_RECIPIENT_DEVICE | (USB_REQ_SET_FEATURE << 8):
    /* Only the DEVICE_REMOTE_WAKEUP is handled here, any other feature
       number is handled as an error.*/
    if (usbp->usb_setup[2] == USB_FEATURE_DEVICE_REMOTE_WAKEUP) {
      usbp->usb_status |= 2;
      usbSetupTransfer(usbp, NULL, 0, NULL);
      return TRUE;
    }
    return FALSE;
  case USB_RTYPE_RECIPIENT_DEVICE | (USB_REQ_SET_ADDRESS << 8):
    /* The handling is posponed to after the status phase in order to allow
       the proper completion of the transaction.*/
    usbSetupTransfer(usbp, NULL, 0, set_address);
    return TRUE;
  case USB_RTYPE_RECIPIENT_DEVICE | (USB_REQ_GET_DESCRIPTOR << 8):
    /* Handling descriptor requests from the host.*/
    dp = usbp->usb_config->uc_get_descriptor_cb(
           usbp, usbp->usb_setup[3], usbp->usb_setup[2],
           usb_lld_fetch_word(&usbp->usb_setup[4]));
    if (dp == NULL)
      return FALSE;
    usbSetupTransfer(usbp, (uint8_t *)dp->ud_string, dp->ud_size, NULL);
    return TRUE;
  case USB_RTYPE_RECIPIENT_DEVICE | (USB_REQ_GET_CONFIGURATION << 8):
    /* Returning the last selected configuration.*/
    usbSetupTransfer(usbp, &usbp->usb_configuration, 1, NULL);
    return TRUE;
  case USB_RTYPE_RECIPIENT_DEVICE | (USB_REQ_SET_CONFIGURATION << 8):
    /* Handling configuration selection from the host.*/
    usbp->usb_configuration = usbp->usb_setup[2];
    if (usbp->usb_configuration == 0)
      usbp->usb_state = USB_SELECTED;
    else
      usbp->usb_state = USB_ACTIVE;
    if (usbp->usb_config->uc_event_cb)
      usbp->usb_config->uc_event_cb(usbp, USB_EVENT_CONFIGURED);
    usbSetupTransfer(usbp, NULL, 0, NULL);
    return TRUE;
  case USB_RTYPE_RECIPIENT_INTERFACE | (USB_REQ_GET_STATUS << 8):
  case USB_RTYPE_RECIPIENT_ENDPOINT | (USB_REQ_SYNCH_FRAME << 8):
    /* Just sending two zero bytes, the application can change the behavior
       using a hook..*/
    usbSetupTransfer(usbp, (uint8_t *)zero_status, 2, NULL);
    return TRUE;
  case USB_RTYPE_RECIPIENT_ENDPOINT | (USB_REQ_GET_STATUS << 8):
    /* Sending the EP status.*/
    if (usbp->usb_setup[4] & 0x80) {
      switch (usb_lld_get_status_in(usbp, usbp->usb_setup[4] & 0x0F)) {
      case EP_STATUS_STALLED:
        usbSetupTransfer(usbp, (uint8_t *)halted_status, 2, NULL);
        return TRUE;
      case EP_STATUS_ACTIVE:
        usbSetupTransfer(usbp, (uint8_t *)active_status, 2, NULL);
        return TRUE;
      default:
        return FALSE;
      }
    }
    else {
      switch (usb_lld_get_status_out(usbp, usbp->usb_setup[4] & 0x0F)) {
      case EP_STATUS_STALLED:
        usbSetupTransfer(usbp, (uint8_t *)halted_status, 2, NULL);
        return TRUE;
      case EP_STATUS_ACTIVE:
        usbSetupTransfer(usbp, (uint8_t *)active_status, 2, NULL);
        return TRUE;
      default:
        return FALSE;
      }
    }
  case USB_RTYPE_RECIPIENT_ENDPOINT | (USB_REQ_CLEAR_FEATURE << 8):
    /* Only ENDPOINT_HALT is handled as feature.*/
    if (usbp->usb_setup[2] != USB_FEATURE_ENDPOINT_HALT)
      return FALSE;
    /* Clearing the EP status, not valid for EP0, it is ignored in that case.*/
    if ((usbp->usb_setup[4] & 0x0F) > 0) {
      if (usbp->usb_setup[4] & 0x80)
        usb_lld_clear_in(usbp, usbp->usb_setup[4] & 0x0F);
      else
        usb_lld_clear_out(usbp, usbp->usb_setup[4] & 0x0F);
    }
    usbSetupTransfer(usbp, NULL, 0, NULL);
    return TRUE;
  case USB_RTYPE_RECIPIENT_ENDPOINT | (USB_REQ_SET_FEATURE << 8):
    /* Only ENDPOINT_HALT is handled as feature.*/
    if (usbp->usb_setup[2] != USB_FEATURE_ENDPOINT_HALT)
      return FALSE;
    /* Stalling the EP, not valid for EP0, it is ignored in that case.*/
    if ((usbp->usb_setup[4] & 0x0F) > 0) {
      if (usbp->usb_setup[4] & 0x80)
        usb_lld_stall_in(usbp, usbp->usb_setup[4] & 0x0F);
      else
        usb_lld_stall_out(usbp, usbp->usb_setup[4] & 0x0F);
    }
    usbSetupTransfer(usbp, NULL, 0, NULL);
    return TRUE;
  case USB_RTYPE_RECIPIENT_DEVICE | (USB_REQ_SET_DESCRIPTOR << 8):
  case USB_RTYPE_RECIPIENT_INTERFACE | (USB_REQ_CLEAR_FEATURE << 8):
  case USB_RTYPE_RECIPIENT_INTERFACE | (USB_REQ_SET_FEATURE << 8):
  case USB_RTYPE_RECIPIENT_INTERFACE | (USB_REQ_GET_INTERFACE << 8):
  case USB_RTYPE_RECIPIENT_INTERFACE | (USB_REQ_SET_INTERFACE << 8):
    /* All the above requests are not handled here, if you need them then
       use the hook mechanism and provide handling.*/
  default:
    return FALSE;
  }
}

/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

/**
 * @brief   USB Driver initialization.
 * @note    This function is implicitly invoked by @p halInit(), there is
 *          no need to explicitly initialize the driver.
 *
 * @init
 */
void usbInit(void) {

  usb_lld_init();
}

/**
 * @brief   Initializes the standard part of a @p USBDriver structure.
 *
 * @param[out] usbp     pointer to the @p USBDriver object
 *
 * @init
 */
void usbObjectInit(USBDriver *usbp) {

  usbp->usb_state    = USB_STOP;
  usbp->usb_config   = NULL;
  usbp->usb_param    = NULL;
}

/**
 * @brief   Configures and activates the USB peripheral.
 *
 * @param[in] usbp      pointer to the @p USBDriver object
 * @param[in] config    pointer to the @p USBConfig object
 *
 * @api
 */
void usbStart(USBDriver *usbp, const USBConfig *config) {
  unsigned i;

  chDbgCheck((usbp != NULL) && (config != NULL), "usbStart");

  chSysLock();
  chDbgAssert((usbp->usb_state == USB_STOP) || (usbp->usb_state == USB_READY),
              "usbStart(), #1", "invalid state");
  usbp->usb_config = config;
  for (i = 0; i <= USB_MAX_ENDPOINTS; i++)
    usbp->usb_ep[i] = NULL;
  usb_lld_start(usbp);
  usbp->usb_state = USB_READY;
  chSysUnlock();
}

/**
 * @brief   Deactivates the USB peripheral.
 *
 * @param[in] usbp      pointer to the @p USBDriver object
 *
 * @api
 */
void usbStop(USBDriver *usbp) {

  chDbgCheck(usbp != NULL, "usbStop");

  chSysLock();
  chDbgAssert((usbp->usb_state == USB_STOP) || (usbp->usb_state == USB_READY),
              "usbStop(), #1",
              "invalid state");
  usb_lld_stop(usbp);
  usbp->usb_state = USB_STOP;
  chSysUnlock();
}

/**
 * @brief   Enables an endpoint.
 * @details This function enables an endpoint, both IN and/or OUT directions
 *          depending on the configuration structure.
 * @note    This function must be invoked in response of a SET_CONFIGURATION
 *          or SET_INTERFACE message.
 *
 * @param[in] usbp      pointer to the @p USBDriver object
 * @param[in] ep        endpoint number
 * @param[out] epp      pointer to an endpoint state descriptor structure
 * @param[in] epcp      the endpoint configuration
 *
 * @iclass
 */
void usbInitEndpointI(USBDriver *usbp, usbep_t ep, USBEndpointState *epp,
                      const USBEndpointConfig *epcp) {

  chDbgAssert(usbp->usb_state == USB_ACTIVE,
              "usbEnableEndpointI(), #1", "invalid state");
  chDbgAssert(usbp->usb_ep[ep] != NULL,
              "usbEnableEndpointI(), #2", "already initialized");

  /* Logically enabling the endpoint in the USBDriver structure.*/
  memset(epp, 0, sizeof(USBEndpointState));
  epp->uep_config  = epcp;
  usbp->usb_ep[ep] = epp;

  /* Low level endpoint activation.*/
  usb_lld_init_endpoint(usbp, ep);
}

/**
 * @brief   Disables all the active endpoints.
 * @details This function disables all the active endpoints except the
 *          endpoint zero.
 * @note    This function must be invoked in response of a SET_CONFIGURATION
 *          message with configuration number zero.
 *
 * @param[in] usbp      pointer to the @p USBDriver object
 *
 * @iclass
 */
void usbDisableEndpointsI(USBDriver *usbp) {
  unsigned i;

  chDbgAssert(usbp->usb_state == USB_SELECTED,
              "usbDisableEndpointsI(), #1", "invalid state");

  for (i = 1; i <= USB_MAX_ENDPOINTS; i++)
    usbp->usb_ep[i] = NULL;

  /* Low level endpoints deactivation.*/
  usb_lld_disable_endpoints(usbp);
}

/**
 * @brief   USB reset routine.
 *
 * @param[in] usbp      pointer to the @p USBDriver object
 *
 * @notapi
 */
void _usb_reset(USBDriver *usbp) {
  unsigned i;

  usbp->usb_state         = USB_READY;
  usbp->usb_status        = 0;
  usbp->usb_address       = 0;
  usbp->usb_configuration = 0;

  /* Invalidates all endpoints into the USBDriver structure.*/
  for (i = 0; i <= USB_MAX_ENDPOINTS; i++)
    usbp->usb_ep[i] = NULL;

  /* EP0 state machine initialization.*/
  usbp->usb_ep0state     = USB_EP0_WAITING_SETUP;

  /* Low level reset.*/
  usb_lld_reset(usbp);

  /* Endpoint zero initialization.*/
/*  usbp->usb_ep[0].uep_config = &usb_lld_ep0config;
  usb_lld_init_endpoint(usbp, 0, &usb_lld_ep0config);*/
}

/**
 * @brief   Default EP0 IN callback.
 * @details This function is used by the low level driver as default handler
 *          for EP0 IN events.
 *
 * @param[in] usbp      pointer to the @p USBDriver object
 * @param[in] ep        endpoint number, always zero
 *
 * @notapi
 */
void _usb_ep0in(USBDriver *usbp, usbep_t ep) {

  (void)ep;
  switch (usbp->usb_ep0state) {
  case USB_EP0_TX:
    usbp->usb_ep0next += usbp->usb_ep0lastsize;
    usbp->usb_ep0max  -= usbp->usb_ep0lastsize;
    usbp->usb_ep0n    -= usbp->usb_ep0lastsize;

    /* The final condition is when the requested size has been transmitted or
       when a packet has been sent with size less than the maximum packet
       size.*/
    if ((usbp->usb_ep0max == 0) ||
        (usbp->usb_ep0lastsize < usbp->usb_ep[0]->uep_config->uepc_in_maxsize))
      usbp->usb_ep0state = USB_EP0_WAITING_STS;
    else {
      usbp->usb_ep0lastsize =
          usbp->usb_ep0n > usbp->usb_ep[0]->uep_config->uepc_in_maxsize ?
                           usbp->usb_ep[0]->uep_config->uepc_in_maxsize :
                           usbp->usb_ep0n;
      usb_lld_write(usbp, 0, usbp->usb_ep0next, usbp->usb_ep0lastsize);
    }
    return;
  case USB_EP0_SENDING_STS:
    if (usbp->usb_ep0endcb)
      usbp->usb_ep0endcb(usbp);

    usbp->usb_ep0state = USB_EP0_WAITING_SETUP;
    return;
  default:
    ;
  }
  /* Error response.*/
  usb_lld_stall_in(usbp, 0);
  usb_lld_stall_out(usbp, 0);
  if (usbp->usb_config->uc_event_cb)
    usbp->usb_config->uc_event_cb(usbp, USB_EVENT_STALLED);
  usbp->usb_ep0state = USB_EP0_WAITING_SETUP;
}

/**
 * @brief   Default EP0 OUT callback.
 * @details This function is used by the low level driver as default handler
 *          for EP0 OUT events.
 *
 * @param[in] usbp      pointer to the @p USBDriver object
 * @param[in] ep        endpoint number, always zero
 *
 * @notapi
 */
void _usb_ep0out(USBDriver *usbp, usbep_t ep) {
  size_t n, size;
  uint8_t buf[1];

  (void)ep;
  switch (usbp->usb_ep0state) {
  case USB_EP0_WAITING_SETUP:
    /* SETUP packet handling.*/
    n = usb_lld_read(usbp, 0, usbp->usb_setup, 8);
    if (n != 8)
      break;

    /* First verify if the application has an handler installed for this
       request.*/
    if (!(usbp->usb_config->uc_requests_hook_cb) ||
        !(usbp->usb_config->uc_requests_hook_cb(usbp))) {
      /* Invoking the default handler, if this fails then stalls the
         endpoint zero as error.*/
      if (((usbp->usb_setup[0] & USB_RTYPE_TYPE_MASK) != USB_RTYPE_TYPE_STD) ||
          !default_handler(usbp))
        break;
    }

    /* Transfer preparation. The request handler must have populated
       correctly the fields usb_ep0next, usb_ep0n and usb_ep0endcb using
       the macro usbSetupTransfer().*/
    usbp->usb_ep0max = usb_lld_fetch_word(&usbp->usb_setup[6]);
    if ((usbp->usb_setup[0] & USB_RTYPE_DIR_MASK) == USB_RTYPE_DIR_DEV2HOST)
      start_tx_ep0(usbp);
    else
      start_rx_ep0(usbp);
    return;
  case USB_EP0_RX:
    /* Check for buffer overflow.*/
    n = size = usb_lld_get_readable(usbp, 0);
    if (n > usbp->usb_ep0n)
      n = usbp->usb_ep0n;
    /* Fetching received data packet.*/
    n = usb_lld_read(usbp, 0, usbp->usb_ep0next, n);
    if (n > usbp->usb_ep0max)
      break;
    usbp->usb_ep0max  -= size;
    usbp->usb_ep0n    -= n;
    usbp->usb_ep0next += n;
    if (usbp->usb_ep0max == 0) {
      usb_lld_write(usbp, 0, NULL, 0);
      usbp->usb_ep0state = USB_EP0_SENDING_STS;
    }
    return;
  case USB_EP0_WAITING_STS:
    /* STATUS received packet handling, it must be zero sized.*/
    n = usb_lld_read(usbp, 0, buf, 1);
    if (n != 0)
      break;
    if (usbp->usb_ep0endcb)
      usbp->usb_ep0endcb(usbp);
    usbp->usb_ep0state = USB_EP0_WAITING_SETUP;
    return;
  default:
    ;
  }
  /* Error response.*/
  usb_lld_stall_in(usbp, 0);
  usb_lld_stall_out(usbp, 0);
  if (usbp->usb_config->uc_event_cb)
    usbp->usb_config->uc_event_cb(usbp, USB_EVENT_STALLED);
  usbp->usb_ep0state = USB_EP0_WAITING_SETUP;
}

#endif /* HAL_USE_USB */

/** @} */
