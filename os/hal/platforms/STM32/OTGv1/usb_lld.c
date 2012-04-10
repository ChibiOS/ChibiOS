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
 * @brief   Buffer for the EP0 setup packets.
 */
static uint8_t ep8setup_buffer[8];

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
  &ep0_state.out,
  ep8setup_buffer
};

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

static void otg_core_reset(void) {

  /* Wait AHB idle condition.*/
  while ((OTG->GRSTCTL & GRSTCTL_AHBIDL) == 0)
    ;
  /* Core reset and delay of at least 3 PHY cycles.*/
  OTG->GRSTCTL = GRSTCTL_CSRST;
  while ((OTG->GRSTCTL & GRSTCTL_CSRST) != 0)
    ;
  halPolledDelay(12);
}

static void otg_disable_ep(void) {
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
    OTG->ie[i].DIEPINT = 0xFFFFFFFF;
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
    OTG->oe[i].DOEPINT = 0xFFFFFFFF;
  }
}

static void otg_rxfifo_flush(void) {

  OTG->GRSTCTL = GRSTCTL_RXFFLSH;
  while ((OTG->GRSTCTL & GRSTCTL_RXFFLSH) != 0)
    ;
}

static void otg_txfifo_flush(uint32_t fifo) {

  OTG->GRSTCTL = GRSTCTL_TXFNUM(fifo) | GRSTCTL_TXFFLSH;
  while ((OTG->GRSTCTL & GRSTCTL_TXFFLSH) != 0)
    ;
}

/**
 * @brief   Resets the FIFO RAM memory allocator.
 *
 * @param[in] usbp      pointer to the @p USBDriver object
 *
 * @notapi
 */
static void otg_ram_reset(USBDriver *usbp) {

  usbp->pmnext = STM32_USB_OTG1_RX_FIFO_SIZE / 4;
}

/**
 * @brief   Allocates a block from the FIFO RAM memory.
 *
 * @param[in] usbp      pointer to the @p USBDriver object
 * @param[in] size      size of the packet buffer to allocate in words
 *
 * @notapi
 */
static uint32_t otg_ram_alloc(USBDriver *usbp, size_t size) {
  uint32_t next;

  next = usbp->pmnext;
  usbp->pmnext += size;
  chDbgAssert(usbp->pmnext <= STM32_OTG_FIFO_MEM_SIZE,
              "otg_fifo_alloc(), #1", "FIFO memory overflow");
  return next;
}

/**
 * @brief   Writes to a TX FIFO.
 *
 * @param[in] ep        endpoint number
 * @param[in] buf       buffer where to copy the endpoint data
 * @param[in] n         maximum number of bytes to copy
 * @return              the number of bytes that were effectively written
 *
 * @notapi
 */
static void otg_fifo_write(usbep_t ep, const uint8_t *buf, size_t n) {
  volatile uint32_t *fifop;

  fifop = OTG_FIFO(ep);
  n = (n + 3) / 4;
  while (n) {
    uint32_t dw = (uint32_t)buf[0]          |
                  ((uint32_t)buf[1] << 8)   |
                  ((uint32_t)buf[2] << 16)  |
                  ((uint32_t)buf[3] << 24);
    *fifop = dw;
    n--;
    buf += 4;
  }
}

/**
 * @brief   Reads a packet from the RXFIFO.
 *
 * @param[out] buf      buffer where to copy the endpoint data
 * @param[in] n         number of bytes to pull from the FIFO
 * @param[in] max       number of bytes to copy into the buffer
 *
 * @notapi
 */
static void otg_fifo_read(uint8_t *buf, size_t n, size_t max) {
  volatile uint32_t *fifop;

  fifop = OTG_FIFO(0);
  n = (n + 3) / 4;
  max = (max + 3) / 4;
  while (n) {
    uint32_t dw = *fifop;
    if (max) {
      *buf++ = (uint8_t)dw;
      *buf++ = (uint8_t)(dw >> 8);
      *buf++ = (uint8_t)(dw >> 16);
      *buf++ = (uint8_t)(dw >> 24);
      max--;
    }
    n--;
  }
}

/**
 * @brief   Incoming packets handler.
 *
 * @param[in] usbp      pointer to the @p USBDriver object
 *
 * @notapi
 */
static void otg_rxfifo_handler(USBDriver *usbp) {
  uint32_t sts, cnt, ep;

  sts = OTG->GRXSTSP;
  switch (sts & GRXSTSP_PKTSTS_MASK) {
  case GRXSTSP_SETUP_COMP:
    break;
  case GRXSTSP_SETUP_DATA:
    cnt = (sts & GRXSTSP_BCNT_MASK) >> GRXSTSP_BCNT_OFF;
    ep  = (sts & GRXSTSP_EPNUM_MASK) >> GRXSTSP_EPNUM_OFF;
    otg_fifo_read(usbp->epc[ep]->setup_buf, cnt, 8);
    break;
  case GRXSTSP_OUT_DATA:
    cnt = (sts & GRXSTSP_BCNT_MASK) >> GRXSTSP_BCNT_OFF;
    ep  = (sts & GRXSTSP_EPNUM_MASK) >> GRXSTSP_EPNUM_OFF;
    otg_fifo_read(usbp->epc[ep]->out_state->rxbuf, cnt,
                  usbp->epc[ep]->out_state->rxsize -
                  usbp->epc[ep]->out_state->rxcnt);
    usbp->epc[ep]->out_state->rxbuf += cnt;
    usbp->epc[ep]->out_state->rxcnt += cnt;
    break;
  case GRXSTSP_OUT_GLOBAL_NAK:
  case GRXSTSP_OUT_COMP:
  default:
    ;
  }
}

/**
 * @brief   Outgoing packets handler.
 *
 * @param[in] usbp      pointer to the @p USBDriver object
 * @param[in] ep        endpoint number
 *
 * @notapi
 */
static void otg_txfifo_handler(USBDriver *usbp, usbep_t ep) {
  uint32_t n;

#if 0
  if ((usbp->transmitting & (1 << ep))== 0) {
    /* Nothing to transmit.*/
    /* ????????????????????? */
    OTG->DIEPEMPMSK &= ~DIEPEMPMSK_INEPTXFEM(ep);
    return;
  }

  if (usbp->epc[ep]->in_state->txsize == 0) {
    /* Special case, sending zero size packet.*/
    OTG->ie[ep].DIEPTSIZ = DIEPTSIZ_PKTCNT(1) | DIEPTSIZ_XFRSIZ(0);
  }
  else if (usbp->epc[ep]->in_state->txcnt == 0) {
    /* Transfer initialization.*/
    uint32_t pcnt = (usbp->epc[ep]->in_state->txsize +
                     (usbp->epc[ep]->in_maxsize - 1) /
                      usbp->epc[ep]->in_maxsize);
    OTG->ie[ep].DIEPTSIZ = DIEPTSIZ_PKTCNT(pcnt) |
                           DIEPTSIZ_XFRSIZ(usbp->epc[ep]->in_state->txsize);
  }
#endif

  n = usbp->epc[ep]->in_state->txsize - usbp->epc[ep]->in_state->txcnt;
  if (n > usbp->epc[ep]->in_maxsize)
    n = usbp->epc[ep]->in_maxsize;
  OTG->ie[ep].DIEPCTL |= DIEPCTL_EPENA | DIEPCTL_CNAK;
  otg_fifo_write(ep, usbp->epc[ep]->in_state->txbuf, n);
  usbp->epc[ep]->in_state->txbuf += n;
  usbp->epc[ep]->in_state->txcnt += n;
  if (usbp->epc[ep]->in_state->txcnt >= usbp->epc[ep]->in_state->txsize) {
    /* Transfer finished.*/
    OTG->DIEPEMPMSK &= ~DIEPEMPMSK_INEPTXFEM(ep);
  }
}

/**
 * @brief   Generic endpoint IN handler.
 *
 * @param[in] usbp      pointer to the @p USBDriver object
 * @param[in] ep        endpoint number
 *
 * @notapi
 */
static void otg_epin_handler(USBDriver *usbp, usbep_t ep) {
  uint32_t epint = OTG->ie[ep].DIEPINT;

  if (epint & DIEPINT_TXFE) {
    /* TX FIFO empty or emptying.*/
    otg_txfifo_handler(usbp, ep);
  }
  if (epint & DIEPINT_XFRC) {
    /* Transmit transfer complete.*/
    _usb_isr_invoke_in_cb(usbp, ep);
  }
  if (epint & DIEPINT_TOC) {
    /* Timeouts not handled yet, not sure how to handle.*/
  }
  OTG->ie[ep].DIEPINT = 0xFFFFFFFF;
}

/**
 * @brief   Generic endpoint OUT handler.
 *
 * @param[in] usbp      pointer to the @p USBDriver object
 * @param[in] ep        endpoint number
 *
 * @notapi
 */
static void otg_epout_handler(USBDriver *usbp, usbep_t ep) {
  uint32_t epint = OTG->oe[ep].DOEPINT;

  /* Is it a setup packet?*/
  if (epint & DOEPINT_STUP) {
    /* Setup packets handling, setup packets are handled using a
       specific callback.*/
    _usb_isr_invoke_setup_cb(usbp, ep);
  }
  if (epint & DOEPINT_XFRC) {
    /* Receive transfer complete.*/
    _usb_isr_invoke_out_cb(usbp, ep);
  }
  OTG->oe[ep].DOEPINT = 0xFFFFFFFF;
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
  USBDriver *usbp = &USBD1;
  uint32_t sts;

  CH_IRQ_PROLOGUE();

  sts = OTG->GINTSTS & OTG->GINTMSK;

  /* Reset interrupt handling.*/
  if (sts & GINTSTS_USBRST) {
    _usb_reset(usbp);
    _usb_isr_invoke_event_cb(usbp, USB_EVENT_RESET);
    OTG->GINTSTS = GINTSTS_USBRST;
  }

  /* Enumeration done.*/
  if (sts & GINTSTS_ENUMDNE) {
    (void)OTG->DSTS;
    OTG->GINTSTS = GINTSTS_ENUMDNE;
  }

  /* SOF interrupt handling.*/
  if (sts & GINTSTS_SOF) {
    _usb_isr_invoke_sof_cb(usbp);
    OTG->GINTSTS = GINTSTS_SOF;
  }

  /* RX FIFO not empty handling.*/
  if (sts & GINTMSK_RXFLVLM) {
    otg_rxfifo_handler(usbp);
  }

  /* IN/OUT endpoints event handling, timeout and transfer complete events
     are handled.*/
  if (sts & (GINTSTS_IEPINT | GINTSTS_OEPINT)) {
    uint32_t src = OTG->DAINT;
    if (src & (1 << 0))
      otg_epin_handler(usbp, 0);
    if (src & (1 << 1))
      otg_epin_handler(usbp, 1);
    if (src & (1 << 2))
      otg_epin_handler(usbp, 2);
    if (src & (1 << 3))
      otg_epin_handler(usbp, 3);
    if (src & (1 << 16))
      otg_epout_handler(usbp, 0);
    if (src & (1 << 17))
      otg_epout_handler(usbp, 1);
    if (src & (1 << 18))
      otg_epout_handler(usbp, 2);
    if (src & (1 << 19))
      otg_epout_handler(usbp, 3);
  }

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

    /* Soft core reset.*/
    otg_core_reset();

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

    /* Endpoints re-initialization.*/
    otg_disable_ep();

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
  unsigned i;

  /* Endpoint interrupts all disabled and cleared.*/
  OTG->DAINTMSK = 0;
  OTG->DAINT    = 0xFFFFFFFF;

  /* All endpoints in NAK mode, interrupts cleared.*/
  for (i = 0; i <= USB_MAX_ENDPOINTS; i++) {
    OTG->ie[i].DIEPCTL = DIEPCTL_SNAK;
    OTG->oe[i].DOEPCTL = DOEPCTL_SNAK;
    OTG->ie[i].DIEPINT = 0xFFFFFFFF;
    OTG->oe[i].DOEPINT = 0xFFFFFFFF;
  }

  /* Resets the FIFO memory allocator.*/
  otg_ram_reset(usbp);


  /* Receive FIFO size initialization, the address is always zero.*/
  OTG->GRXFSIZ = STM32_USB_OTG1_RX_FIFO_SIZE / 4;
  otg_rxfifo_flush();

  /* Enables also EP-related interrupt sources.*/
  OTG->GINTMSK  |= GINTMSK_RXFLVLM | GINTMSK_OEPM  | GINTMSK_IEPM;
  OTG->DIEPMSK   = DIEPMSK_TOCM    | DIEPMSK_XFRCM;
  OTG->DOEPMSK   = DOEPMSK_STUPM   | DOEPMSK_XFRCM;
//  OTG->DAINTMSK = DAINTMSK_OutEpMsk(0) | DAINTMSK_InEpMsk(0);

  /* EP0 initialization, it is a special case.*/
  usbp->epc[0] = &ep0config;
  OTG->oe[0].DOEPTSIZ = 0;
  OTG->oe[0].DOEPCTL = DIEPCTL_SD0PID | DIEPCTL_USBAEP | DIEPCTL_EPTYP_CTRL |
                       DOEPCTL_MPSIZ(ep0config.out_maxsize);
  OTG->ie[0].DIEPTSIZ = 0;
  OTG->ie[0].DIEPCTL = DIEPCTL_SD0PID | DIEPCTL_USBAEP | DIEPCTL_EPTYP_CTRL |
                       DIEPCTL_TXFNUM(0) | DIEPCTL_MPSIZ(ep0config.in_maxsize);
  OTG->DIEPTXF0 = DIEPTXF_INEPTXFD(ep0config.in_maxsize / 4) |
                  DIEPTXF_INEPTXSA(otg_ram_alloc(usbp,
                                                 ep0config.in_maxsize / 4));
  otg_txfifo_flush(0);
}

/**
 * @brief   Sets the USB address.
 *
 * @param[in] usbp      pointer to the @p USBDriver object
 *
 * @notapi
 */
void usb_lld_set_address(USBDriver *usbp) {

  OTG->DCFG = (OTG->DCFG & ~DCFG_DAD_MASK) | DCFG_DAD(usbp->address);
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
  uint32_t ctl, fsize;

  /* IN and OUT common parameters.*/
  switch (usbp->epc[ep]->ep_mode & USB_EP_MODE_TYPE) {
  case USB_EP_MODE_TYPE_CTRL:
    ctl = DIEPCTL_SD0PID | DIEPCTL_USBAEP | DIEPCTL_EPTYP_CTRL;
    break;
  case USB_EP_MODE_TYPE_ISOC:
    ctl = DIEPCTL_SD0PID | DIEPCTL_USBAEP | DIEPCTL_EPTYP_ISO;
    break;
  case USB_EP_MODE_TYPE_BULK:
    ctl = DIEPCTL_SD0PID | DIEPCTL_USBAEP | DIEPCTL_EPTYP_BULK;
    break;
  case USB_EP_MODE_TYPE_INTR:
    ctl = DIEPCTL_SD0PID | DIEPCTL_USBAEP | DIEPCTL_EPTYP_INTR;
    break;
  default:
    return;
  }

  /* OUT endpoint activation or deactivation.*/
  OTG->oe[ep].DOEPTSIZ = 0;
  if (usbp->epc[ep]->out_cb != NULL)
    OTG->oe[ep].DOEPCTL = ctl | DOEPCTL_MPSIZ(usbp->epc[ep]->out_maxsize);
  else
    OTG->oe[ep].DOEPCTL &= ~DOEPCTL_USBAEP;

  /* IN endpoint activation or deactivation.*/
  OTG->ie[ep].DIEPTSIZ = 0;
  if (usbp->epc[ep]->in_cb != NULL) {
    /* FIFO allocation for the IN endpoint.*/
    fsize = usbp->epc[ep]->in_maxsize / 4;
    OTG->DIEPTXF[ep - 1] = DIEPTXF_INEPTXFD(fsize) |
                           DIEPTXF_INEPTXSA(otg_ram_alloc(usbp, fsize));
    otg_txfifo_flush(ep);

    OTG->ie[ep].DIEPCTL = ctl |
                          DIEPCTL_TXFNUM(ep) |
                          DIEPCTL_MPSIZ(usbp->epc[ep]->in_maxsize);
  }
  else {
    OTG->DIEPTXF[ep - 1] = 0x02000400; /* Reset value.*/
    otg_txfifo_flush(ep);
    OTG->ie[ep].DIEPCTL &= ~DIEPCTL_USBAEP;
  }
}

/**
 * @brief   Disables all the active endpoints except the endpoint zero.
 *
 * @param[in] usbp      pointer to the @p USBDriver object
 *
 * @notapi
 */
void usb_lld_disable_endpoints(USBDriver *usbp) {

  /* Resets the FIFO memory allocator.*/
  otg_ram_reset(usbp);

  /* Disabling all endpoints.*/
  otg_disable_ep();
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

  (void)usbp;
  (void)ep;

  return 0;
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

  (void)usbp;
  (void)ep;

  return 0;
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

  (void)usbp;
  (void)ep;
  (void)buf;
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

  (void)usbp;
  (void)ep;
  (void)buf;
  (void)n;

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

  (void)usbp;
  (void)ep;
  (void)buf;
  (void)n;
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

  (void)usbp;
  (void)ep;
  (void)buf;
  (void)n;
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

  (void)usbp;
  (void)ep;
  (void)buf;
  (void)n;
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
  (void)ep;
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
  (void)ep;
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

  OTG->oe[ep].DOEPCTL |= DOEPCTL_STALL;
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

  OTG->ie[ep].DIEPCTL |= DIEPCTL_STALL;
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

  OTG->oe[ep].DOEPCTL &= ~DOEPCTL_STALL;
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

  OTG->ie[ep].DIEPCTL &= ~DIEPCTL_STALL;
}

#endif /* HAL_USE_USB */

/** @} */
