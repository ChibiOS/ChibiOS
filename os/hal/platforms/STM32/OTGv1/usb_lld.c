/*
    ChibiOS/RT - Copyright (C) 2006,2007,2008,2009,2010,
                 2011,2012 Giovanni Di Sirio.

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
 * @file    STM32/OTGv1/usb_lld.c
 * @brief   STM32 USB subsystem low level driver source.
 *
 * @addtogroup USB
 * @{
 */

#include <string.h>

#include "ch.h"
#include "hal.h"

#if HAL_USE_USB || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver local definitions.                                                 */
/*===========================================================================*/

#define TRDT_VALUE      5

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

/** @brief OTG1 driver identifier.*/
#if STM32_USB_USE_OTG1 || defined(__DOXYGEN__)
USBDriver USBD1;
#endif

/*===========================================================================*/
/* Driver local variables.                                                   */
/*===========================================================================*/

/**
 * @brief   EP0 state.
 * @note    It is an union because IN and OUT endpoints are never used at the
 *          same time for EP0.
 */
static union {
  /**
   * @brief   IN EP0 state.
   */
  USBInEndpointState in;
  /**
   * @brief   OUT EP0 state.
   */
  USBOutEndpointState out;
} ep0_state;

/**
 * @brief   EP0 initialization structure.
 */
static const USBEndpointConfig ep0config = {
  USB_EP_MODE_TYPE_CTRL | USB_EP_MODE_TRANSACTION,
  _usb_ep0setup,
  _usb_ep0in,
  _usb_ep0out,
  0x40,
  0x40,
  &ep0_state.in,
  &ep0_state.out
};

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

static void otg_reset_ep(void) {
  unsigned i;

  for (i = 0; i <= USB_MAX_ENDPOINTS; i++) {
    /* Disable only if enabled because this sentence in the manual:
       "The application must set this bit only if Endpoint Enable is
        already set for this endpoint".*/
    if ((OTG->ie[i].DIEPCTL & DIEPCTL_EPENA) != 0) {
      OTG->ie[i].DIEPCTL = DIEPCTL_EPDIS;
      /* Wait for endpoint disable.*/
      while (!(OTG->ie[i].DIEPINT & DIEPINT_EPDISD))
        ;
    }
    else
      OTG->ie[i].DIEPCTL = 0;
    OTG->ie[i].DIEPTSIZ = 0;
    OTG->ie[i].DIEPINT = DIEPINT_INEPNE | DIEPINT_ITTXFE |
                         DIEPINT_TOC    | DIEPINT_EPDISD |
                         DIEPINT_XFRC;
    /* Disable only if enabled because this sentence in the manual:
       "The application must set this bit only if Endpoint Enable is
        already set for this endpoint".
       Note that the attempt to disable the OUT EP0 is ignored by the
       hardware but the code is simpler this way.*/
    if ((OTG->oe[i].DOEPCTL & DOEPCTL_EPENA) != 0) {
      OTG->oe[i].DOEPCTL = DOEPCTL_EPDIS;
      /* Wait for endpoint disable.*/
      while (!(OTG->oe[i].DOEPINT & DOEPINT_OTEPDIS))
        ;
    }
    else
      OTG->oe[i].DOEPCTL = 0;
    OTG->oe[i].DOEPTSIZ = 0;
    OTG->oe[i].DOEPINT = DOEPINT_B2BSTUP | DOEPINT_OTEPDIS |
                         DOEPINT_STUP    | DOEPINT_EPDISD  |
                         DOEPINT_XFRC;
  }
}

/**
 * @brief   Resets the RX FIFO memory allocator.
 *
 * @param[in] usbp      pointer to the @p USBDriver object
 */
static void rxfifo_reset(USBDriver *usbp) {

}

/**
 * @brief   Resets the RX FIFO memory allocator.
 *
 * @param[in] usbp      pointer to the @p USBDriver object
 * @param[in] size      size of the packet buffer to allocate
 */
static uint32_t rxfifo_alloc(USBDriver *usbp, size_t size) {

  return 0;
}

/*===========================================================================*/
/* Driver interrupt handlers.                                                */
/*===========================================================================*/

#if STM32_USB_USE_OTG1 || defined(__DOXYGEN__)
/**
 * @brief   OTG1 interrupt handler.
 *
 * @isr
 */
CH_IRQ_HANDLER(OTG_FS_IRQHandler) {

  CH_IRQ_PROLOGUE();

  CH_IRQ_EPILOGUE();
}
#endif

/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

/**
 * @brief   Low level USB driver initialization.
 *
 * @notapi
 */
void usb_lld_init(void) {

  /* Driver initialization.*/
  usbObjectInit(&USBD1);
}

/**
 * @brief   Configures and activates the USB peripheral.
 * @note    Starting the ORG cell can be a slow operation carried out with
 *          interrupts disabled, perform it before starting time-critical
 *          operations.
 *
 * @param[in] usbp      pointer to the @p USBDriver object
 *
 * @notapi
 */
void usb_lld_start(USBDriver *usbp) {

  if (usbp->state == USB_STOP) {
    /* Clock activation.*/
#if STM32_USB_USE_OTG1
    if (&USBD1 == usbp) {
      /* OTG FS clock enable and reset.*/
      rccEnableOTG_FS(FALSE);
      rccResetOTG_FS();

      /* Enables IRQ vector.*/
      nvicEnableVector(OTG_FS_IRQn,
                       CORTEX_PRIORITY_MASK(STM32_USB_OTG1_IRQ_PRIORITY));
    }
#endif

    /* Wait AHB idle condition.*/
    while ((OTG->GRSTCTL & GRSTCTL_AHBIDL) == 0)
      ;

    /* Core reset and delay of at least 3 PHY cycles.*/
    OTG->GRSTCTL = GRSTCTL_CSRST;
    while ((OTG->GRSTCTL & GRSTCTL_CSRST) != 0)
      ;
    halPolledDelay(12);

    /* - Forced device mode.
       - USB turn-around time = TRDT_VALUE.
       - Full Speed 1.1 PHY.*/
    OTG->GUSBCFG = GUSBCFG_FDMOD | GUSBCFG_TRDT(TRDT_VALUE) | GUSBCFG_PHYSEL;

    /* Interrupt on TXFIFOs empty.*/
    OTG->GAHBCFG = GAHBCFG_PTXFELVL | GAHBCFG_TXFELVL;

    /* 48MHz 1.1 PHY.*/
    OTG->DCFG = 0x02200000 |  DCFG_PFIVL(0) | DCFG_DSPD_FS11;

    /* PHY enabled.*/
    OTG->PCGCCTL = 0;

    /* Receive FIFO size initialization, the address is always zero.*/
    OTG->GRXFSIZ = STM32_USB_OTG1_RX_FIFO_SIZE / 4;

    /* Endpoints re-initialization.*/
    otg_reset_ep();

    /* Clear all pending Device Interrupts, only the USB Reset interrupt
       is required initially.*/
    OTG->DIEPMSK  = 0;
    OTG->DOEPMSK  = 0;
    OTG->DAINTMSK = 0;
    OTG->GINTMSK  = GINTMSK_ENUMDNEM | GINTMSK_USBRSTM | GINTMSK_USBSUSPM |
                    GINTMSK_ESUSPM   | GINTMSK_SOFM;
    OTG->GINTSTS  = 0xFFFFFFFF;         /* Clears all pending IRQs, if any. */

    /* Global interrupts enable.*/
    OTG->GAHBCFG |= GAHBCFG_GINTMSK;
  }
}

/**
 * @brief   Deactivates the USB peripheral.
 *
 * @param[in] usbp      pointer to the @p USBDriver object
 *
 * @notapi
 */
void usb_lld_stop(USBDriver *usbp) {

  /* If in ready state then disables the USB clock.*/
  if (usbp->state == USB_STOP) {
#if STM32_USB_USE_USB1
    if (&USBD1 == usbp) {
      nvicDisableVector(OTG_FS_IRQn);
      rccDisableOTG1(FALSE);
    }
#endif
  }
}

/**
 * @brief   USB low level reset routine.
 *
 * @param[in] usbp      pointer to the @p USBDriver object
 *
 * @notapi
 */
void usb_lld_reset(USBDriver *usbp) {
  uint32_t cntr;

  /* Post reset initialization.*/

  /* Resets the packet memory allocator.*/
  rxfifo_reset(usbp);

  /* EP0 initialization.*/
  usbp->epc[0] = &ep0config;
  usb_lld_init_endpoint(usbp, 0);
}

/**
 * @brief   Sets the USB address.
 *
 * @param[in] usbp      pointer to the @p USBDriver object
 *
 * @notapi
 */
void usb_lld_set_address(USBDriver *usbp) {

}

/**
 * @brief   Enables an endpoint.
 *
 * @param[in] usbp      pointer to the @p USBDriver object
 * @param[in] ep        endpoint number
 *
 * @notapi
 */
void usb_lld_init_endpoint(USBDriver *usbp, usbep_t ep) {

}

/**
 * @brief   Disables all the active endpoints except the endpoint zero.
 *
 * @param[in] usbp      pointer to the @p USBDriver object
 *
 * @notapi
 */
void usb_lld_disable_endpoints(USBDriver *usbp) {

}

/**
 * @brief   Returns the status of an OUT endpoint.
 *
 * @param[in] usbp      pointer to the @p USBDriver object
 * @param[in] ep        endpoint number
 * @return              The endpoint status.
 * @retval EP_STATUS_DISABLED The endpoint is not active.
 * @retval EP_STATUS_STALLED  The endpoint is stalled.
 * @retval EP_STATUS_ACTIVE   The endpoint is active.
 *
 * @notapi
 */
usbepstatus_t usb_lld_get_status_out(USBDriver *usbp, usbep_t ep) {

}

/**
 * @brief   Returns the status of an IN endpoint.
 *
 * @param[in] usbp      pointer to the @p USBDriver object
 * @param[in] ep        endpoint number
 * @return              The endpoint status.
 * @retval EP_STATUS_DISABLED The endpoint is not active.
 * @retval EP_STATUS_STALLED  The endpoint is stalled.
 * @retval EP_STATUS_ACTIVE   The endpoint is active.
 *
 * @notapi
 */
usbepstatus_t usb_lld_get_status_in(USBDriver *usbp, usbep_t ep) {

}

/**
 * @brief   Reads a setup packet from the dedicated packet buffer.
 * @details This function must be invoked in the context of the @p setup_cb
 *          callback in order to read the received setup packet.
 * @pre     In order to use this function the endpoint must have been
 *          initialized as a control endpoint.
 * @post    The endpoint is ready to accept another packet.
 *
 * @param[in] usbp      pointer to the @p USBDriver object
 * @param[in] ep        endpoint number
 * @param[out] buf      buffer where to copy the packet data
 *
 * @notapi
 */
void usb_lld_read_setup(USBDriver *usbp, usbep_t ep, uint8_t *buf) {

}

/**
 * @brief   Reads from a dedicated packet buffer.
 * @pre     In order to use this function he endpoint must have been
 *          initialized in packet mode.
 * @note    This function can be invoked both in thread and IRQ context.
 *
 * @param[in] usbp      pointer to the @p USBDriver object
 * @param[in] ep        endpoint number
 * @param[out] buf      buffer where to copy the packet data
 * @param[in] n         maximum number of bytes to copy. This value must
 *                      not exceed the maximum packet size for this endpoint.
 * @return              The received packet size regardless the specified
 *                      @p n parameter.
 * @retval 0            Zero size packet received.
 *
 * @notapi
 */
size_t usb_lld_read_packet_buffer(USBDriver *usbp, usbep_t ep,
                                  uint8_t *buf, size_t n) {

  return 0;
}

/**
 * @brief   Writes to a dedicated packet buffer.
 * @pre     In order to use this function he endpoint must have been
 *          initialized in packet mode.
 * @note    This function can be invoked both in thread and IRQ context.
 *
 * @param[in] usbp      pointer to the @p USBDriver object
 * @param[in] ep        endpoint number
 * @param[in] buf       buffer where to fetch the packet data
 * @param[in] n         maximum number of bytes to copy. This value must
 *                      not exceed the maximum packet size for this endpoint.
 *
 * @notapi
 */
void usb_lld_write_packet_buffer(USBDriver *usbp, usbep_t ep,
                                 const uint8_t *buf, size_t n) {

}

/**
 * @brief   Prepares for a receive operation.
 *
 * @param[in] usbp      pointer to the @p USBDriver object
 * @param[in] ep        endpoint number
 * @param[out] buf      buffer where to copy the received data
 * @param[in] n         maximum number of bytes to copy
 *
 * @notapi
 */
void usb_lld_prepare_receive(USBDriver *usbp, usbep_t ep,
                             uint8_t *buf, size_t n) {

}

/**
 * @brief   Prepares for a transmit operation.
 *
 * @param[in] usbp      pointer to the @p USBDriver object
 * @param[in] ep        endpoint number
 * @param[in] buf       buffer where to fetch the data to be transmitted
 * @param[in] n         maximum number of bytes to copy
 *
 * @notapi
 */
void usb_lld_prepare_transmit(USBDriver *usbp, usbep_t ep,
                              const uint8_t *buf, size_t n) {

}

/**
 * @brief   Starts a receive operation on an OUT endpoint.
 *
 * @param[in] usbp      pointer to the @p USBDriver object
 * @param[in] ep        endpoint number
 *
 * @notapi
 */
void usb_lld_start_out(USBDriver *usbp, usbep_t ep) {

  (void)usbp;

}

/**
 * @brief   Starts a transmit operation on an IN endpoint.
 *
 * @param[in] usbp      pointer to the @p USBDriver object
 * @param[in] ep        endpoint number
 *
 * @notapi
 */
void usb_lld_start_in(USBDriver *usbp, usbep_t ep) {

  (void)usbp;

}

/**
 * @brief   Brings an OUT endpoint in the stalled state.
 *
 * @param[in] usbp      pointer to the @p USBDriver object
 * @param[in] ep        endpoint number
 *
 * @notapi
 */
void usb_lld_stall_out(USBDriver *usbp, usbep_t ep) {

  (void)usbp;

}

/**
 * @brief   Brings an IN endpoint in the stalled state.
 *
 * @param[in] usbp      pointer to the @p USBDriver object
 * @param[in] ep        endpoint number
 *
 * @notapi
 */
void usb_lld_stall_in(USBDriver *usbp, usbep_t ep) {

  (void)usbp;

}

/**
 * @brief   Brings an OUT endpoint in the active state.
 *
 * @param[in] usbp      pointer to the @p USBDriver object
 * @param[in] ep        endpoint number
 *
 * @notapi
 */
void usb_lld_clear_out(USBDriver *usbp, usbep_t ep) {

  (void)usbp;

}

/**
 * @brief   Brings an IN endpoint in the active state.
 *
 * @param[in] usbp      pointer to the @p USBDriver object
 * @param[in] ep        endpoint number
 *
 * @notapi
 */
void usb_lld_clear_in(USBDriver *usbp, usbep_t ep) {

  (void)usbp;

}

#endif /* HAL_USE_USB */

/** @} */
