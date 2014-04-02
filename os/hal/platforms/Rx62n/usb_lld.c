/*
    ChibiOS/RT - Copyright (C) 2006-2013 Giovanni Di Sirio

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
 * @file    RX62N/usb_lld.c
 * @brief   RX62N USB Driver subsystem low level driver source.
 *
 * @addtogroup USB
 * @{
 */

#include "ch.h"
#include "hal.h"


#if HAL_USE_USB || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver local definitions.                                                 */
/*===========================================================================*/

/**
 * @brief   PIPEnCTR register union.
 */
typedef union {
  unsigned short WORD;
  struct {
    unsigned short PID:2;
    unsigned short  :3;
    unsigned short PBUSY:1;
    unsigned short SQMON:1;
    unsigned short SQSET:1;
    unsigned short SQCLR:1;
    unsigned short ACLRM:1;
    unsigned short ATREPM:1;
    unsigned short  :3;
    unsigned short INBUFM:1;
    unsigned short BSTS:1;
  } BIT;
} pipectr_t;;

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

/** @brief USB1 driver identifier.*/
#if RX62N_USE_USB0 || defined(__DOXYGEN__)
USBDriver USBD1;
#endif

/** @brief USB2 driver identifier.*/
#if RX62N_USE_USB1 || defined(__DOXYGEN__)
USBDriver USBD2;
#error "USBD2 is not supported yet"
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
static uint8_t ep0setup_buffer[8];

/**
 * @brief   EP0 initialization structure.
 */
static const USBEndpointConfig ep0config = {
  USB_EP_MODE_TYPE_CTRL,
  _usb_ep0setup,
  _usb_ep0in,
  _usb_ep0out,
  USB_EP0_PACKET_SIZE,
  USB_EP0_PACKET_SIZE,
  &ep0_state.in,
  &ep0_state.out,
  1,
  ep0setup_buffer
};

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

pipectr_t usb_pipectr_read(USBDriver *usbp, usbep_t ep) {
  volatile pipectr_t *pipe = (pipectr_t *)&usbp->usb->PIPE1CTR;

  return pipe[ep-1];
}

void usb_pipectr_write(USBDriver *usbp, usbep_t ep, uint16_t p) {
  volatile pipectr_t *pipe = (pipectr_t *)&usbp->usb->PIPE1CTR;

  pipe[ep-1].WORD = p;
}

/**
 * @brief   Reads from a dedicated packet buffer.
 *
 * @param[in] udp       pointer to a @p stm32_usb_descriptor_t
 * @param[out] buf      buffer where to copy the packet data
 * @param[in] n         maximum number of bytes to copy. This value must
 *                      not exceed the maximum packet size for this endpoint.
 *
 * @notapi
 */
static void usb_packet_read_to_buffer(USBDriver *usbp, usbep_t ep,
                                      uint8_t *buf, size_t n) {
  uint32_t i;

  usbp->epc[ep]->in_state->transmitted = n;
  if (ep == 0) {
    for (i = 0; i < n; i++)
      usbp->epc[ep]->out_state->mode.linear.rxbuf[i] = usbp->usb->CFIFO.BYTE.L;
  }
  else {
    for (i = 0; i < n; i++)
      usbp->epc[ep]->out_state->mode.linear.rxbuf[i] = usbp->usb->D0FIFO.BYTE.L;
  }
}

/**
 * @brief   Reads from a dedicated packet buffer.
 *
 * @param[in] udp       pointer to a @p stm32_usb_descriptor_t
 * @param[in] iqp       pointer to an @p InputQueue object
 * @param[in] n         maximum number of bytes to copy. This value must
 *                      not exceed the maximum packet size for this endpoint.
 *
 * @notapi
 */
static void usb_packet_read_to_queue(USBDriver *usbp, usbep_t ep, InputQueue *iqp, size_t n) {
  size_t nhw = n;
  uint8_t c;

  while (nhw > 0) {
    if (ep == 0) {
        c = usbp->usb->CFIFO.BYTE.L;
    }
	else {
        /* ep 1..9 */
        c = usbp->usb->D0FIFO.BYTE.L;;
    }
    *iqp->q_wrptr++ = c;
    if (iqp->q_wrptr >= iqp->q_top)
      iqp->q_wrptr = iqp->q_buffer;
    nhw--;
  }

  /* Updating queue.*/
  chSysLockFromIsr();
  iqp->q_counter += n;
  while (notempty(&iqp->q_waiting))
    chSchReadyI(fifo_remove(&iqp->q_waiting))->p_u.rdymsg = Q_OK;
  chSysUnlockFromIsr();
}

/**
 * @brief   Writes to a dedicated packet buffer.
 *
 * @param[in] buf       buffer where to fetch the packet data
 * @param[in] n         maximum number of bytes to copy. This value must
 *                      not exceed the maximum packet size for this endpoint.
 *
 * @notapi
 */
static void usb_packet_write_from_buffer(USBDriver *usbp, usbep_t ep,
                                         const uint8_t *buf, size_t n) {

  usbp->epc[ep]->in_state->transmitted = n;
  if (ep == 0) {
    while (n--) {
      usbp->usb->CFIFO.BYTE.L = *buf++;
    }
  }
  else {
    /* ep 1..9 */
    usbp->usb->D1FIFO.BYTE.L = *buf++;
  }
}

/**
 * @brief   Writes to a dedicated packet buffer.
 *
 * @param[in] udp       pointer to a @p stm32_usb_descriptor_t
 * @param[in] buf       buffer where to fetch the packet data
 * @param[in] n         maximum number of bytes to copy. This value must
 *                      not exceed the maximum packet size for this endpoint.
 *
 * @notapi
 */
static void usb_packet_write_from_queue(USBDriver *usbp, usbep_t ep,
                                        OutputQueue *oqp, size_t n) {
  size_t nhw = n;
  uint8_t c;

  usbp->epc[ep]->in_state->transmitted = n;
  while (nhw > 0) {
    c = *oqp->q_rdptr++;;
    if (ep == 0) {
      usbp->usb->CFIFO.BYTE.L = c;
    }
	else {
      /* ep 1..9 */
      usbp->usb->D1FIFO.BYTE.L = c;
    }
    c = c;
    if (oqp->q_rdptr >= oqp->q_top)
      oqp->q_rdptr = oqp->q_buffer;
    nhw--;
  }

  /* Updating queue. Note, the lock is done in this unusual way because this
     function can be called from both ISR and thread context so the kind
     of lock function to be invoked cannot be decided beforehand.*/

  /*
    TODO: port_lock() doesn't work.*/
  if (port_enabled()) {
    dbg_enter_lock();
    chSysLock();

    oqp->q_counter += n;
    while (notempty(&oqp->q_waiting))
      chSchReadyI(fifo_remove(&oqp->q_waiting))->p_u.rdymsg = Q_OK;

    chSysUnlock();
    dbg_leave_lock();
  }
  else {
    oqp->q_counter += n;
    while (notempty(&oqp->q_waiting))
      chSchReadyI(fifo_remove(&oqp->q_waiting))->p_u.rdymsg = Q_OK;
  }
}

/*===========================================================================*/
/* Driver interrupt handlers.                                                */
/*===========================================================================*/

#if RX62N_USE_USB0 || defined(__DOXYGEN__)
void usb_lld_serve_interrupt(USBDriver *usbp) {
  uint16_t sts0, sts0clr;
  uint16_t ep = 0;

  sts0 = usbp->usb->INTSTS0.WORD;
  sts0clr = (~(sts0 & usbp->usb->INTENB0.WORD))|
    INTSTS0_VALID|INTSTS0_DVST|INTSTS0_CTRT;

  if (sts0 & INTSTS0_RESM) {
/*    _usb_isr_invoke_event_cb(usbp, USB_EVENT_RESUME);*/
  }

  /* USB bus WAKEUP condition handling.*/
/*
  if (isr & AT91C_UDP_WAKEUP) {
    _usb_isr_invoke_event_cb(usbp, USB_EVENT_WAKEUP);
  }
*/
  /* SOF handling.*/
/*
  if (isr & AT91C_UDP_SOFINT) {
    _usb_isr_invoke_sof_cb(usbp);
  }
*/
  if (sts0 & INTSTS0_DVST) {
    usbp->usb->INTSTS0.BIT.DVST = 0;
    switch (INTSTS0_DVSQ(sts0)) {
      case DVSQ_POWERED:
        break;
      case DVSQ_DEFAULT:
        if ( !(sts0 & INTSTS0_RESM) && (usbp->last_dvst != DVSQ_ADDRESS) &&
          (usbp->last_dvst != DVSQ_SUSPENDED_DEF)) {
          _usb_reset(usbp);
          _usb_isr_invoke_event_cb(usbp, USB_EVENT_RESET);
        }
        break;
      case DVSQ_ADDRESS:
        break;
      case DVSQ_CONFIGURED:
        break;
      default:
        /* Default is suspened state */
        _usb_isr_invoke_event_cb(usbp, USB_EVENT_SUSPEND);
        break;
    }
    usbp->last_dvst = INTSTS0_DVSQ(sts0);
  }

  if (sts0 & INTSTS0_CTRT) {
    switch (INTSTS0_CTSQ(sts0)) {
      case CTSQ_IDLE_OR_SETUP:
        if (usbp->last_ctsq == CTSQ_READ_STATUS) {
            _usb_isr_invoke_out_cb(usbp, ep);
        }
        else if (usbp->last_ctsq == CTSQ_WRITE_STATUS) {
            _usb_isr_invoke_in_cb(usbp, ep);
        }
        else if (usbp->last_ctsq == CTSQ_NODATA_STATUS) {
            _usb_isr_invoke_in_cb(usbp, ep);
        }
        break;
      default:
        break;
    }
    usbp->last_ctsq = INTSTS0_CTSQ(sts0);
    usbp->usb->INTSTS0.BIT.CTRT = 0;
  }

  if (sts0 & INTSTS0_VALID) {
    usbp->usb->INTSTS0.BIT.VALID = 0;
    _usb_isr_invoke_setup_cb(usbp, ep);
    if (INTSTS0_CTSQ(sts0) == CTSQ_NODATA_STATUS) {
      usbp->usb->DCPCTR.BIT.PID = PID_BUF;
      usbp->usb->DCPCTR.BIT.CCPL = 1;
    }
  }

  if (sts0 & INTSTS0_BRDY) {
    /* BRDY */
    uint16_t brdysts = usbp->usb->BRDYSTS.WORD;
    const USBEndpointConfig *epcp;
    size_t n;
    for (ep=0; ep<USB_MAX_ENDPOINTS; ep++) {
      if (brdysts & (1<<ep)) {
        epcp = usbp->epc[ep];
        if (ep == 0) {
            n = usbp->usb->CFIFOCTR.BIT.DTLN;
        }
        else {
          if (epcp->out_state) {
            usbp->usb->PIPESEL.BIT.PIPESEL = ep;
            do {
              usbp->usb->D0FIFOSEL.WORD = ep;
            } while (usbp->usb->D0FIFOSEL.WORD != ep);
            while (!usbp->usb->D0FIFOCTR.BIT.FRDY);
            n = usbp->usb->D0FIFOCTR.BIT.DTLN;
          }
          else {
            /* in_state */
            usbp->usb->PIPESEL.BIT.PIPESEL = ep;
            do {
              usbp->usb->D1FIFOSEL.WORD = ep;
            } while (usbp->usb->D1FIFOSEL.WORD != ep);
            while (!usbp->usb->D1FIFOCTR.BIT.FRDY);
            n = 0; /* Omit compiler warning */
          }
        }
        if (epcp->out_state) {
          /* Reads the packet into the defined buffer.*/
          if (epcp->out_state->rxqueued) {
            usb_packet_read_to_queue(usbp, ep,
              epcp->out_state->mode.queue.rxqueue, n);
          }
          else {
            usb_packet_read_to_buffer(usbp, ep,
              epcp->out_state->mode.linear.rxbuf, n);
            epcp->out_state->mode.linear.rxbuf  += n;
          }
          /* Transaction data updated.*/
          epcp->out_state->rxcnt                += n;
          epcp->out_state->rxsize               -= n;
          epcp->out_state->rxpkts               -= 1;
          if (epcp->out_state->rxpkts > 0) {
            /* Transfer not completed, there are more packets to receive.*/
          }
          else {
            /* Transfer completed, invokes the callback.*/
            /* Set pipe to NAK */
            usb_pipectr_write(usbp, ep, PID_NAK);
            _usb_isr_invoke_out_cb(usbp, ep);
          }
        }
        else {
          epcp->in_state->txcnt  += epcp->in_state->transmitted;
          epcp->in_state->txsize -= epcp->in_state->transmitted;
          if (epcp->in_state->txsize > 0) {
            /* Transfer not completed, there are more packets to send.*/
            if (epcp->in_state->txsize > epcp->in_maxsize)
              n = epcp->in_maxsize;
            else
              n = epcp->in_state->txsize;

            if (epcp->in_state->txqueued) {
              usb_packet_write_from_queue(usbp, ep,
                epcp->in_state->mode.queue.txqueue, n);
            }
            else {
              epcp->in_state->mode.linear.txbuf += epcp->in_state->transmitted;
              usb_packet_write_from_buffer(usbp, ep,
                epcp->in_state->mode.linear.txbuf, n);
            }
            chSysLockFromIsr();
            usb_lld_start_in(usbp, ep);
            chSysUnlockFromIsr();
          }
          else {
            /* Transfer completed, invokes the callback.*/
            _usb_isr_invoke_in_cb(usbp, ep);
          }
        }
      }
    }
    usbp->usb->BRDYSTS.WORD = ~brdysts;
  }
  if (sts0 & INTSTS0_BEMP) {
    /* BEMP */
    uint16_t bempsts = usbp->usb->BEMPSTS.WORD;
    const USBEndpointConfig *epcp;
    size_t n;
    ep = 0;
    epcp = usbp->epc[ep];
    epcp->in_state->txcnt  += epcp->in_state->transmitted;
    epcp->in_state->txsize -= epcp->in_state->transmitted;
    if (epcp->in_state->txsize > 0) {
      /* Transfer not completed, there are more packets to send.*/
      if (epcp->in_state->txsize > epcp->in_maxsize)
        n = epcp->in_maxsize;
      else
        n = epcp->in_state->txsize;
      if (epcp->in_state->txqueued) {
        usb_packet_write_from_queue(usbp, ep,
        epcp->in_state->mode.queue.txqueue, n);
      }
      else {
        epcp->in_state->mode.linear.txbuf += epcp->in_state->transmitted;
        usb_packet_write_from_buffer(usbp, ep,
        epcp->in_state->mode.linear.txbuf, n);
      }
      chSysLockFromIsr();
      usb_lld_start_in(usbp, ep);
      chSysUnlockFromIsr();
    }
    else {
      /* Transfer completed, invokes the callback.*/
      _usb_isr_invoke_in_cb(usbp, ep);
    }
    usbp->usb->BEMPSTS.WORD = ~bempsts;
  }
  usbp->usb->INTSTS0.WORD = sts0clr;
}
#endif

/**
 * @brief   USB interrupt handler.
 *
 * @isr
 */
#if RX62N_USE_USB0 || defined(__DOXYGEN__)
CH_IRQ_HANDLER(Excep_USB0_USBI0) {

  CH_IRQ_PROLOGUE();
  usb_lld_serve_interrupt(&USBD1);
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

#if RX62N_USE_USB0 || defined(__DOXYGEN__)
  usbObjectInit(&USBD1);
  USBD1.usb = &USB0;
#endif
#if RX62N_USE_USB1 || defined(__DOXYGEN__)
  usbObjectInit(&USBD2);
  USBD2.usb = &USB1;
#endif
}

/**
 * @brief   Configures and activates the USB peripheral.
 *
 * @param[in] usbp      pointer to the @p USBDriver object
 *
 * @notapi
 */
void usb_lld_start(USBDriver *usbp) {

  if (usbp->state == USB_STOP) {
    /* Enable USB peripheral. */
    if (usbp == &USBD1) {
      MSTP(USB0) = 0;
      /* Enable GPIO Pull-up */
      PORT1.DDR.BIT.B4 = 1;
      /* Enable USB0 pins functions */
      IOPORT.PFKUSB.BYTE = 0x14;
    }
    /* USB clock activation. */
    usbp->usb->SYSCFG.BIT.SCKE = 1;
    /* Select function. */
    usbp->usb->SYSCFG.BIT.DCFM = 0;
    /* Enable module operation. */
    usbp->usb->SYSCFG.BIT.USBE = 1;

    /* Default as powered state */
    usbp->last_dvst = 0;
    /* Default as idle stage */
    usbp->last_ctsq = 0;
    /* Current FIFO */
    usbp->current_fifo = 0;

    /* Reset procedure enforced on driver start.*/
    _usb_reset(usbp);
  }
  /* Configuration.*/
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
  if (usbp->state != USB_STOP) {
/*    IEN(USB0,USBI0) = 0;*/
    /* Disable module operation. */
    usbp->usb->SYSCFG.BIT.USBE = 0;
    /* Disable USB clock.*/
    usbp->usb->SYSCFG.BIT.SCKE = 0;
    /* Disable USB peripheral.*/
    if (usbp == &USBD1) {
      MSTP(USB0) = 1;
    }
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
  uint16_t ien = 0;

  if (usbp == &USBD1) {
    IEN(USB0,USBI0) = 0;
  }
  /* Disable interrupts */
  usbp->usb->INTENB0.WORD = 0;
  usbp->usb->INTENB1.WORD = 0;
  usbp->usb->BRDYENB.WORD = 0;
  usbp->usb->NRDYENB.WORD = 0;
  usbp->usb->BEMPENB.WORD = 0;
  /* Clear interrupt status bits */
  usbp->usb->INTSTS0.WORD = (uint16_t)(~(INTSTS0_VALID|INTSTS0_BRDY|
    INTSTS0_BEMP|INTSTS0_CTRT|INTSTS0_DVST|INTSTS0_SOFR|INTSTS0_RESM|
    INTSTS0_VBINT));
  usbp->usb->INTSTS1.WORD = (uint16_t)(~((1<<4)|(1<<5)|(1<<6)|(1<<11)|(1<<12)|
    (1<<14)|(1<<15)));

  if (usbp->config->sof_cb != NULL) {
    ien = INTENB0_SOFE;
  }

  /* EP0 initialization.*/
  usbp->epc[0] = &ep0config;
  usbp->usb->DCPCFG.WORD = 0;
  usbp->usb->DCPMAXP.WORD = USB_EP0_PACKET_SIZE;
  usbp->usb->CFIFOSEL.WORD = 0;

  /* Enable the USB interrupts - other interrupts get enabled as the
  enumeration process progresses. */
  if (usbp == &USBD1) {
    IEN(USB0,USBI0) = 1;
    IPR(USB0,USBI0) = RX62N_USB0_IRQ_PRIORITY;
  }
  usbp->usb->INTENB0.WORD =
    INTENB0_BRDYE|
    INTENB0_BEMPE|
    INTENB0_CTRE|
    INTENB0_DVSE|
    INTENB0_RSME|
    ien|
    0;
  usbp->usb->BRDYENB.BIT.PIPE0BRDYE = 1;
  usbp->usb->BEMPENB.BIT.PIPE0BEMPE = 1;
}

/**
 * @brief   Sets the USB address.
 *
 * @param[in] usbp      pointer to the @p USBDriver object
 *
 * @notapi
 */
void usb_lld_set_address(USBDriver *usbp) {

  (void)usbp;
}

/**
 * @brief   Finalizes the USB configuration process.
 *
 * @param[in] usbp      pointer to the @p USBDriver object
 *
 * @notapi
 */
void usb_lld_end_configuration(USBDriver *usbp) {

  (void)usbp;
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
  uint16_t pipecfg;
  const USBEndpointConfig *epcp = usbp->epc[ep];

  chDbgAssert(ep > 0, "usb_lld_init_endpoint(), #1", "invalid endpoint");
  if (ep == 0)
    return;

/*  usbp->usb->CFIFOSEL.BIT.CURPIPE = 0;*/
  usbp->usb->PIPESEL.WORD = ep;

  /* Set PID to NAK */
  usb_pipectr_write(usbp, ep, 0);
  while (usb_pipectr_read(usbp, ep).BIT.PBUSY);
  usb_pipectr_write(usbp, ep, PIPECTR_SQCLR|PIPECTR_ACLRM);
  usb_pipectr_write(usbp, ep, 0);
/*
  if (epcp->out_cb != NULL)
    epcp->out_state->currentBank = 0;
*/
  /* Setting the endpoint type.*/
  switch (epcp->ep_mode & USB_EP_MODE_TYPE) {
  case USB_EP_MODE_TYPE_ISOC:
    pipecfg = PIPECFG_TYPE_ISO;
    break;
  case USB_EP_MODE_TYPE_BULK:
    pipecfg = PIPECFG_TYPE_BULK;
    break;
  case USB_EP_MODE_TYPE_INTR:
    pipecfg = PIPECFG_TYPE_INT;
    break;
  default:
    /* No control type allowed */
    chDbgAssert((epcp->ep_mode & USB_EP_MODE_TYPE) != USB_EP_MODE_TYPE_CTRL,
      "usb_lld_init_endpoint(), #2", "invalid endpoint");
    return;
  }

  /* Set direction */
  if (epcp->in_cb != NULL) {
    usbp->usb->PIPEMAXP.BIT.MXPS = epcp->in_maxsize;
    pipecfg |= PIPECFG_DIR_IN;
  }
  else {
    usbp->usb->PIPEMAXP.BIT.MXPS = epcp->out_maxsize;
    pipecfg |= PIPECFG_DIR_OUT;
  }
  pipecfg |= ep;
  usbp->usb->PIPECFG.WORD = pipecfg;

  /* Clear interrupt status flag */
  usbp->usb->BRDYSTS.WORD &= ~(1<<ep);
  /* Enable interrupts */
  usbp->usb->BRDYENB.WORD |= (1<<ep);

  usbp->usb->PIPESEL.WORD = 0;
}

/**
 * @brief   Disables all the active endpoints except the endpoint zero.
 *
 * @param[in] usbp      pointer to the @p USBDriver object
 *
 * @notapi
 */
void usb_lld_disable_endpoints(USBDriver *usbp) {
  usbep_t i;

  /* Disable endpoints interrupt */
  usbp->usb->BRDYENB.WORD = 1;
  usbp->usb->BRDYSTS.WORD = ~1;
  /* Disable endpoints */
  for (i=0; i<USB_MAX_ENDPOINTS; i++) {
    usbp->usb->PIPESEL.WORD = i+1;
    usbp->usb->PIPECFG.BIT.EPNUM = 0;
  }
  usbp->usb->PIPESEL.WORD = 0;
  /* Clear FIFO */
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
  usbep_t epPrev = usbp->usb->PIPESEL.BIT.PIPESEL;

  if (ep == 0)
    return EP_STATUS_ACTIVE;
  usbp->usb->PIPESEL.BIT.PIPESEL = ep;
  usbep_t epNum = usbp->usb->PIPECFG.BIT.EPNUM;
  usbp->usb->PIPESEL.BIT.PIPESEL = epPrev;
  if (epNum) {
    if (usb_pipectr_read(usbp, ep).BIT.PID & 2)
      return EP_STATUS_STALLED;
  }
  return EP_STATUS_DISABLED;
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

  return usb_lld_get_status_out(usbp, ep);
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

  buf[0] = usbp->usb->USBREQ.WORD & 0xFF;
  buf[1] = (usbp->usb->USBREQ.WORD >> 8) & 0xFF;
  buf[2] = usbp->usb->USBVAL & 0xFF;
  buf[3] = (usbp->usb->USBVAL >> 8) & 0xFF;
  buf[4] = usbp->usb->USBINDX & 0xFF;
  buf[5] = (usbp->usb->USBINDX >> 8) & 0xFF;
  buf[6] = usbp->usb->USBLENG & 0xFF;
  buf[7] = (usbp->usb->USBLENG >> 8) & 0xFF;
}

/**
 * @brief   Prepares for a receive operation.
 *
 * @param[in] usbp      pointer to the @p USBDriver object
 * @param[in] ep        endpoint number
 *
 * @notapi
 */
void usb_lld_prepare_receive(USBDriver *usbp, usbep_t ep) {
  USBOutEndpointState *osp = usbp->epc[ep]->out_state;

  /* Transfer initialization.*/
  if (ep == 0) {
    do {
      usbp->usb->CFIFOSEL.BIT.ISEL = 0;
    } while (usbp->usb->CFIFOSEL.BIT.ISEL);
    usbp->usb->DCPCTR.BIT.PID = PID_BUF;
    while (!usbp->usb->CFIFOCTR.BIT.FRDY);
  }
  if (osp->rxsize == 0)         /* Special case for zero sized packets.*/
    osp->rxpkts = 1;
  else
    osp->rxpkts = (uint16_t)((osp->rxsize + usbp->epc[ep]->out_maxsize - 1) /
                             usbp->epc[ep]->out_maxsize);
}

/**
 * @brief   Prepares for a transmit operation.
 *
 * @param[in] usbp      pointer to the @p USBDriver object
 * @param[in] ep        endpoint number
 *
 * @notapi
 */
void usb_lld_prepare_transmit(USBDriver *usbp, usbep_t ep) {
  size_t n;
  USBInEndpointState *isp = usbp->epc[ep]->in_state;

  /* Transfer initialization.*/
  n = isp->txsize;
  if (n > (size_t)usbp->epc[ep]->in_maxsize)
    n = (size_t)usbp->epc[ep]->in_maxsize;
  if (ep == 0) {
    do {
      usbp->usb->CFIFOSEL.BIT.ISEL = 1;
    } while (!usbp->usb->CFIFOSEL.BIT.ISEL);
    usbp->usb->DCPCTR.BIT.PID = PID_BUF;
    while (!usbp->usb->CFIFOCTR.BIT.FRDY);
    if (n == 0)
      usbp->usb->CFIFOCTR.BIT.BCLR = 1;
  }
  else {
    usbp->usb->PIPESEL.BIT.PIPESEL = ep;
    do {
        usbp->usb->D1FIFOSEL.WORD = ep;
    } while (usbp->usb->D1FIFOSEL.WORD != ep);
    while (!usbp->usb->D1FIFOCTR.BIT.FRDY);
    do {
        usb_pipectr_write(usbp, ep, PID_BUF);
    } while (usb_pipectr_read(usbp, ep).BIT.PID != PID_BUF);
  }
  if (isp->txqueued) {
    usb_packet_write_from_queue(usbp, ep, isp->mode.queue.txqueue, n);
  }
  else {
    usb_packet_write_from_buffer(usbp, ep, isp->mode.linear.txbuf, n);
  }
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

  if (ep)
    usb_pipectr_write(usbp, ep, PID_BUF);
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

  if (ep == 0) {
    if (usbp->epc[ep]->in_state->transmitted < usbp->epc[ep]->in_maxsize)
      usbp->usb->CFIFOCTR.BIT.BVAL = 1;
  }
  else {
    if (usbp->epc[ep]->in_state->transmitted < usbp->epc[ep]->in_maxsize)
      usbp->usb->D1FIFOCTR.BIT.BVAL = 1;
  }
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

  if (ep == 0) {
    if (usbp->usb->DCPCTR.BIT.PID == PID_NAK)
      usbp->usb->DCPCTR.BIT.PID = PID_STALL10;
    else
      usbp->usb->DCPCTR.BIT.PID = PID_STALL11;
  }
  else {
    if ( usb_pipectr_read(usbp, ep).BIT.PID == PID_NAK)
      usb_pipectr_write(usbp, ep, PID_STALL10);
    else
      usb_pipectr_write(usbp, ep, PID_STALL11);
  }
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

  usb_lld_stall_out(usbp, ep);
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

  if (ep == 0) {
    if (usbp->usb->DCPCTR.BIT.PID == PID_STALL11)
      usbp->usb->DCPCTR.BIT.PID = PID_STALL10;
    usbp->usb->DCPCTR.BIT.PID = PID_NAK;
  }
  else {
    if ( usb_pipectr_read(usbp, ep).BIT.PID == PID_STALL11)
        usb_pipectr_write(usbp, ep, PID_STALL10);
    usb_pipectr_write(usbp, ep, PID_NAK);
  }
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

  usb_lld_clear_out(usbp, ep);
}

#endif /* HAL_USE_USB */

/** @} */
