/*
    ChibiOS - Copyright (C) 2006..2025 Giovanni Di Sirio

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
 * @file    hal_usb_lld.c
 * @brief   RP USB subsystem low level driver source.
 * @note    This driver is based on the RP2040 USB LLD originally created by
 *          @hanya and @xyzz wth significant improvements by @KarlK90. It has
 *          been updated for the RP2350 and to fix a few defects and errata.
 *
 * @addtogroup USB
 * @{
 */

#include <string.h>

#include "hal.h"

#if (HAL_USE_USB == TRUE) || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver local definitions.                                                 */
/*===========================================================================*/

/**
 * @brief   Get endpoint control register.
 */
#define EP_CTRL(ep)       (USB_DPSRAM->EPCTRL[ep - 1])
/**
 * @brief   Get buffer control register for endpoint.
 */
#define BUF_CTRL(ep)      (USB_DPSRAM->BUFCTRL[ep])

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

/**
 * @brief   USB1 driver identifier.
 */
#if (RP_USB_USE_USB1 == TRUE) || defined(__DOXYGEN__)
USBDriver USBD1;
#endif

/*===========================================================================*/
/* Driver local variables and types.                                         */
/*===========================================================================*/

/**
 * @brief   EP0 state.
 * @note    Both IN and OUT states are needed separately for control transfers
 *          where SETUP, DATA, and STATUS phases use different directions.
 */
static struct {
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
  .ep_mode          = USB_EP_MODE_TYPE_CTRL,
  .setup_cb         = _usb_ep0setup,
  .in_cb            = _usb_ep0in,
  .out_cb           = _usb_ep0out,
  .in_maxsize       = 0x40U,
  .out_maxsize      = 0x40U,
  .in_state         = &ep0_state.in,
  .out_state        = &ep0_state.out
};

/*===========================================================================*/
/* Driver local variables and types.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

#if defined(__ARM_ARCH_8M_MAIN__) || defined(__riscv)
/**
 * @brief   Copy memory using byte accesses.
 * @note    On RP2350 USB DPRAM requires strict alignment.
 */
static inline void usb_dpram_memcpy(void *dst, const void *src, size_t n) {
  volatile uint8_t *d = (volatile uint8_t *)dst;
  const volatile uint8_t *s = (const volatile uint8_t *)src;
  while (n-- > 0) {
    *d++ = *s++;
  }
}
#else
/**
 * @brief   Copy memory using memcpy
 * @note    On RP2040 USB DPRAM is normal memory.
 */
#define usb_dpram_memcpy(dst, src, n) memcpy(dst, src, n)
#endif

/**
 * @brief   Buffer mode for isochronous in buffer control register.
 */
static uint16_t usb_isochronous_buffer_mode(uint16_t size) {
  switch (size) {
    case 128:
      return 0;
    case 256:
      return 1;
    case 512:
      return 2;
    case 1024:
      return 3;
    default:
      return 0;
  }
}

/**
 * @brief   Calculate isochronous buffer size in valid step.
 * @details Valid buffer size is one of 128, 256, 512 or 1024.
 */
static uint16_t usb_isochronous_buffer_size(uint16_t max_size) {
  uint16_t size;

  /* Double buffer offset must be one of 128, 256, 512 or 1024. */
  size = ((max_size - 1) / 128 + 1) * 128;
  if (size == 384) {
    size = 512;
  } else if (size == 640 || size == 768 || size > 1024) {
    size = 1024;
  }
  return size;
}

/**
 * @brief   Calculate next offset for buffer data, 64 bytes aligned.
 *
 * @param[in] usbp      pointer to the @p USBDriver object
 * @param[in] size      buffer size to allocate
 * @param[in] is_double true if double-buffered (allocates size * 2)
 * @return              offset into DATA region for the buffer
 */
static uint16_t usb_buffer_next_offset(USBDriver *usbp, uint16_t size, bool is_double) {
  uint16_t offset;
  uint16_t alloc;

  offset = usbp->noffset;
  alloc = is_double ? (uint16_t)(size * 2U) : size;

  osalDbgAssert((uint32_t)offset + alloc <= USB_DPRAM_DATA_SIZE,
                "USB DPRAM buffer overflow");

  usbp->noffset += alloc;

  return offset;
}

/**
 * @brief   Reset endpoint 0.
 */
static void reset_ep0(USBDriver *usbp) {
  usbp->epc[0]->out_state->next_pid = 1U;
  usbp->epc[0]->in_state->next_pid = 1U;
}

/**
 * @brief   Prepare buffer for receiving data.
 */
static uint32_t usb_prepare_out_ep_buffer(USBDriver *usbp, usbep_t ep, uint8_t buffer_index) {
    uint32_t buf_ctrl = 0;
    const USBEndpointConfig *epcp = usbp->epc[ep];
    USBOutEndpointState *oesp = usbp->epc[ep]->out_state;

    /* PID */
    buf_ctrl |= oesp->next_pid ? USB_BUFFER_BUFFER0_DATA_PID : 0;
    oesp->next_pid ^= 1U;

    uint16_t buf_len = oesp->rxsize < epcp->out_maxsize ? oesp->rxsize : epcp->out_maxsize;
    buf_ctrl |= USB_BUFFER_BUFFER0_AVAILABLE | buf_len;
    buf_ctrl &= ~USB_BUFFER_BUFFER0_FULL;

    if (oesp->rxcnt + buf_len >= oesp->rxsize) {
        /* Last buffer */
        buf_ctrl |= USB_BUFFER_BUFFER0_LAST;
    }

    if (buffer_index) {
      buf_ctrl = buf_ctrl << 16;
    }

    return buf_ctrl;
}

/**
 * @brief   Prepare for receiving data from host.
 */
static void usb_prepare_out_ep(USBDriver *usbp, usbep_t ep) {
  uint32_t buf_ctrl;
  uint32_t ep_ctrl;

  if (ep == 0) {
    ep_ctrl = USB->SIECTRL;
  } else {
    ep_ctrl = EP_CTRL(ep).OUT;
  }

  /* Fill first buffer */
  buf_ctrl = usb_prepare_out_ep_buffer(usbp, ep, 0);

  /* To avoid short packet, we use single buffered here. */
  /* Single buffered */
  ep_ctrl &= ~(USB_EP_BUFFER_DOUBLE | USB_EP_BUFFER_IRQ_DOUBLE_EN);
  ep_ctrl |= USB_EP_BUFFER_IRQ_EN;

  if (ep == 0) {
    USB->SIECTRL = ep_ctrl;
  } else {
    EP_CTRL(ep).OUT = ep_ctrl;
  }

  BUF_CTRL(ep).OUT = buf_ctrl;
  __DSB();
}

/**
 * @brief   Prepare buffer for sending data.
 */
static uint32_t usb_prepare_in_ep_buffer(USBDriver *usbp, usbep_t ep, uint8_t buffer_index) {
    uint8_t *buff;
    uint16_t buf_len;
    uint32_t buf_ctrl = 0;
    const USBEndpointConfig *epcp = usbp->epc[ep];
    USBInEndpointState *iesp = usbp->epc[ep]->in_state;

    /* txsize - txlast gives size of data to be sent but not yet in the buffer */
    buf_len = epcp->in_maxsize < iesp->txsize - iesp->txlast ?
              epcp->in_maxsize : iesp->txsize - iesp->txlast;

    iesp->txlast += buf_len;

    /* Host only? */
    if (iesp->txsize <= iesp->txlast) {
      /* Last buffer */
      buf_ctrl |= USB_BUFFER_BUFFER0_LAST;
    }

    /* PID */
    buf_ctrl |= iesp->next_pid ? USB_BUFFER_BUFFER0_DATA_PID : 0;
    iesp->next_pid ^= 1U;

    /* Copy data into hardware buffer */
    buff = (uint8_t*)iesp->hw_buf + (buffer_index == 0 ? 0 : iesp->buf_size);
    usb_dpram_memcpy((void *)buff, (void *)iesp->txbuf, buf_len);
    iesp->txbuf += buf_len;

    buf_ctrl |= USB_BUFFER_BUFFER0_FULL |
                USB_BUFFER_BUFFER0_AVAILABLE |
                buf_len;

    if (buffer_index) {
      buf_ctrl = buf_ctrl << 16;
    }

    return buf_ctrl;
}

/**
 * @brief   Prepare endpoint for sending data.
 */
static void usb_prepare_in_ep(USBDriver *usbp, usbep_t ep) {
  uint32_t buf_ctrl;
  uint32_t ep_ctrl;
  USBInEndpointState *iesp = usbp->epc[ep]->in_state;

  if (ep == 0) {
    ep_ctrl = USB->SIECTRL;
  } else {
    ep_ctrl = EP_CTRL(ep).IN;
  }

  /* Fill first buffer */
  buf_ctrl = usb_prepare_in_ep_buffer(usbp, ep, 0);

  /* Second buffer if required */
  /* iesp->txsize - iesp->txlast gives size even not in buffer */
  if (iesp->txsize - iesp->txlast > 0) {
    buf_ctrl |= usb_prepare_in_ep_buffer(usbp, ep, 1);
  }

  if (buf_ctrl & USB_BUFFER_BUFFER1_AVAILABLE) {
    /* Double buffered */
    ep_ctrl &= ~USB_EP_BUFFER_IRQ_EN;
    ep_ctrl |= USB_EP_BUFFER_DOUBLE | USB_EP_BUFFER_IRQ_DOUBLE_EN;
  } else {
    /* Single buffered */
    ep_ctrl &= ~(USB_EP_BUFFER_DOUBLE | USB_EP_BUFFER_IRQ_DOUBLE_EN);
    ep_ctrl |= USB_EP_BUFFER_IRQ_EN;
  }

  if (ep == 0) {
    USB->SIECTRL = ep_ctrl;
  } else {
    EP_CTRL(ep).IN = ep_ctrl;
  }

  BUF_CTRL(ep).IN = buf_ctrl;
  __DSB();
}

/**
 * @brief   Work on an endpoint after transfer.
 */
static void usb_serve_endpoint(USBDriver *usbp, usbep_t ep, bool is_in) {
  const USBEndpointConfig *epcp = usbp->epc[ep];
  USBOutEndpointState *oesp;
  USBInEndpointState *iesp;
  uint16_t n;

  if (is_in) {
    /* IN endpoint */
    iesp = usbp->epc[ep]->in_state;

    /* txlast is size sent + size of last buffer */
    iesp->txcnt = iesp->txlast;
    n = iesp->txsize - iesp->txcnt;
    if (n > 0) {
      /* Transfer not completed, there are more packets to send. */
      usb_prepare_in_ep(usbp, ep);
    } else {
      /* Transfer complete */
      _usb_isr_invoke_in_cb(usbp, ep);
    }
  } else {
    /* OUT endpoint */
    oesp = usbp->epc[ep]->out_state;

    /* Length received */
    n = BUF_CTRL(ep).OUT & USB_BUFFER_BUFFER0_TRANS_LENGTH_Msk;

    /* Copy received data into user buffer */
    usb_dpram_memcpy((void *)oesp->rxbuf, (void *)oesp->hw_buf, n);
    oesp->rxbuf += n;
    oesp->rxcnt += n;
    oesp->rxsize -= n;

    oesp->rxpkts -= 1;

    /* Short packet or all packetes have been received. */
    if (oesp->rxpkts == 0 || n < epcp->out_maxsize) {
      /* Transifer complete */
      _usb_isr_invoke_out_cb(usbp, ep);
    } else {
      /* Receive remained data */
      usb_prepare_out_ep(usbp, ep);
    }
  }
}

/*===========================================================================*/
/* Driver interrupt handlers and threads.                                    */
/*===========================================================================*/

#if RP_USB_USE_USB1 || defined(__DOXYGEN__)

/**
 * @brief   USB interrupt handler.
 *
 * @isr
 */
OSAL_IRQ_HANDLER(RP_USBCTRL_IRQ_HANDLER) {
  USBDriver *usbp = &USBD1;
  uint32_t ints;

  OSAL_IRQ_PROLOGUE();

  ints = USB->INTS;

  /* USB setup packet handling. */
  if (ints & USB_INTS_SETUP_REQ) {
    USB->CLR.SIESTATUS = USB_SIE_STATUS_SETUP_REC;

    reset_ep0(usbp);

    _usb_isr_invoke_setup_cb(usbp, 0);
  }

  /* USB bus reset condition handling. */
  if (ints & USB_INTS_BUS_RESET) {
    USB->CLR.SIESTATUS = USB_SIE_STATUS_BUS_RESET;

    _usb_reset(usbp);
  }

  /* USB bus SUSPEND condition handling.*/
  if (ints & USB_INTS_DEV_SUSPEND) {
    USB->CLR.SIESTATUS = USB_SIE_STATUS_SUSPENDED;

    _usb_suspend(usbp);
  }

  /* Resume condition handling */
  if (ints & USB_INTS_DEV_RESUME_FROM_HOST) {
    USB->CLR.SIESTATUS = USB_SIE_STATUS_RESUME;

    _usb_wakeup(usbp);
  }

  /* SOF handling.*/
  if (ints & USB_INTS_DEV_SOF) {
    /* SOF interrupt was used to detect resume of the USB bus after issuing a
     * remote wake up of the host, therefore we disable it again. */
    if (usbp->config->sof_cb == NULL) {
      USB->CLR.INTE = USB_INTE_DEV_SOF;
    }
    if (usbp->state == USB_SUSPENDED) {
      _usb_wakeup(usbp);
    }

    _usb_isr_invoke_sof_cb(usbp);

    /*
     * Clear SOF flag by reading SOF_RD
     * This helps avoid the RP2040-E15 Errata and is harmless on the RP2350.
     */
    (void)USB->SOFRD;
  }

  /* Endpoint events handling.*/
  if (ints & USB_INTS_BUFF_STATUS) {
    uint32_t buf_status = USB->BUFSTATUS;
    uint32_t bit = 1U;
    for (uint8_t i = 0; buf_status && i < 32; i++) {
      if (buf_status & bit) {
        /* Clear flag */
        USB->CLR.BUFSTATUS = bit;
        /* Finish on the endpoint or transfer remained data */
        usb_serve_endpoint(&USBD1, i >> 1U, (i & 1U) == 0);

        buf_status &= ~bit;
      }
      bit <<= 1U;
    }
  }

#if RP_USB_USE_ERROR_DATA_SEQ_INTR == TRUE
  if (ints & USB_INTE_ERROR_DATA_SEQ) {
    USB->CLR.SIESTATUS = USB_SIE_STATUS_DATA_SEQ_ERROR;
  }
#endif /* RP_USB_USE_ERROR_DATA_SEQ_INTR */

  OSAL_IRQ_EPILOGUE();
}

#endif /* RP_USB_USE_USB1 */

/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

/**
 * @brief   Low level USB driver initialization.
 *
 * @notapi
 */
void usb_lld_init(void) {
#if RP_USB_USE_USB1 == TRUE
  /* Driver initialization.*/
  usbObjectInit(&USBD1);

  /* Reset buffer offset. */
  USBD1.noffset = 0;
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
#if RP_USB_USE_USB1 == TRUE
  if (&USBD1 == usbp) {
    if (usbp->state == USB_STOP) {
      /* Reset usb controller */
      hal_lld_peripheral_reset(RESETS_ALLREG_USBCTRL);
      hal_lld_peripheral_unreset(RESETS_ALLREG_USBCTRL);

      /* Clear any previos state in dpram and hw regs */
      memset(USB, 0, sizeof(*USB));
      memset(USB_DPSRAM, 0, sizeof(*USB_DPSRAM));

      /* Mux the controller to the onboard usb phy */
      USB->MUXING = USB_USB_MUXING_SOFTCON | USB_USB_MUXING_TO_PHY;

#if RP_USB_FORCE_VBUS_DETECT == TRUE
      /* Force VBUS detect so the device thinks it is plugged into a host */
      USB->PWR = USB_USB_PWR_VBUS_DETECT_OVERRIDE_EN | USB_USB_PWR_VBUS_DETECT;
#else
#if RP_USE_EXTERNAL_VBUS_DETECT == TRUE
      /* If VBUS is detected by pin without USB VBUS DET pin,
       * define usb_vbus_detect which returns true if VBUS is enabled.
       */
      if (usb_vbus_detect()) {
        USB->PWR = USB_USB_PWR_VBUS_DETECT_OVERRIDE_EN | USB_USB_PWR_VBUS_DETECT;
      }
#endif /* RP_USE_EXTERNAL_VBUS_DETECT */
#endif /* RP_USB_FORCE_VBUS_DETECT */

      /* Reset procedure enforced on driver start.*/
      usb_lld_reset(usbp);

      /* Enable the USB controller in device mode. */
      USB->MAINCTRL = USB_MAIN_CTRL_CONTROLLER_EN;

      /* Enable an interrupt per EP0 transaction */
      USB->SIECTRL = USB_SIE_CTRL_EP0_INT_1BUF;

      /* Enable interrupts */
      USB->INTE = USB_INTE_SETUP_REQ |
                  USB_INTE_DEV_RESUME_FROM_HOST |
                  USB_INTE_DEV_SUSPEND |
                  USB_INTE_BUS_RESET |
                  USB_INTE_BUFF_STATUS;

      if (usbp->config->sof_cb != NULL) {
        USB->SET.INTE = USB_INTE_DEV_SOF;
      }

#if RP_USB_USE_ERROR_DATA_SEQ_INTR == TRUE
      USB->SET.INTE = USB_INTE_ERROR_DATA_SEQ;
#endif /* RP_USB_USE_ERROR_DATA_SEQ_INTR */

      /* Enable USB interrupt. */
      nvicEnableVector(RP_USBCTRL_IRQ_NUMBER, RP_IRQ_USB0_PRIORITY);
    }
  }
#endif
}

/**
 * @brief   Deactivates the USB peripheral.
 *
 * @param[in] usbp      pointer to the @p USBDriver object
 *
 * @notapi
 */
void usb_lld_stop(USBDriver *usbp) {
#if RP_USB_USE_USB1 == TRUE
  if (&USBD1 == usbp) {
    if (usbp->state != USB_STOP) {
      /* Disable USB interrupt */
      USB->INTE = 0;
      nvicDisableVector(RP_USBCTRL_IRQ_NUMBER);

      /* Disable controller */
      USB->CLR.MAINCTRL = USB_MAIN_CTRL_CONTROLLER_EN;
    }
  }
#endif
}

/**
 * @brief   USB low level reset routine.
 *
 * @param[in] usbp      pointer to the @p USBDriver object
 *
 * @notapi
 */
void usb_lld_reset(USBDriver *usbp) {
    /* EP0 initialization.*/
    usbp->epc[0] = &ep0config;
    usb_lld_init_endpoint(usbp, 0U);

    /* Reset device address. */
    USB->DEVADDRCTRL = 0U;

    /* Reset USB memory */
    usbp->noffset = 0U;

    /* Clear all non control endpoint registers */
    for (int ep = 1; ep <= USB_MAX_ENDPOINTS; ep++) {
        EP_CTRL(ep).IN   = 0U;
        BUF_CTRL(ep).IN  = 0U;
        EP_CTRL(ep).OUT  = 0U;
        BUF_CTRL(ep).OUT = 0U;
    }
}

/**
 * @brief   Sets the USB address.
 *
 * @param[in] usbp      pointer to the @p USBDriver object
 *
 * @notapi
 */
void usb_lld_set_address(USBDriver *usbp) {
  /* Set address to hardware here. */
  USB->DEVADDRCTRL = USB_ADDR_ENDP0_ADDRESS_Msk & (usbp->address << USB_ADDR_ENDP0_ADDRESS_Pos);
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
    uint16_t                 buf_size;
    uint16_t                 buf_offset;
    uint32_t                 buf_ctrl;
    const USBEndpointConfig *epcp = usbp->epc[ep];

    if (ep == 0) {
        epcp->in_state->hw_buf    = (uint8_t *)&USB_DPSRAM->EP0BUF0;
        epcp->in_state->buf_size  = 64;
        epcp->in_state->next_pid  = 0U;
        epcp->out_state->hw_buf   = (uint8_t *)&USB_DPSRAM->EP0BUF0;
        epcp->out_state->buf_size = 64;
        epcp->out_state->next_pid = 0U;
        USB->SET.SIECTRL = USB_EP_BUFFER_IRQ_EN;
        return;
    }

    if (epcp->in_state) {
        buf_ctrl                 = 0U;
        BUF_CTRL(ep).IN          = buf_ctrl;
        epcp->in_state->next_pid = 0U;

        if (epcp->ep_mode == USB_EP_MODE_TYPE_ISOC) {
            buf_size = usb_isochronous_buffer_size(epcp->in_maxsize);
            buf_ctrl |= usb_isochronous_buffer_mode(buf_size) << USB_BUFFER_DOUBLE_BUFFER_OFFSET_Pos;
        } else {
            buf_size = 64;
        }
        buf_offset               = usb_buffer_next_offset(usbp, buf_size, true);
        epcp->in_state->hw_buf   = (uint8_t *)&USB_DPSRAM->DATA[buf_offset];
        epcp->in_state->buf_size = buf_size;

        EP_CTRL(ep).IN  = USB_EP_EN | (epcp->ep_mode << USB_EP_TYPE_Pos) | ((uint8_t *)epcp->in_state->hw_buf - (uint8_t *)USB_DPSRAM);
        BUF_CTRL(ep).IN = buf_ctrl;
    }

    if (epcp->out_state) {
        buf_ctrl                  = 0U;
        BUF_CTRL(ep).OUT          = buf_ctrl;
        epcp->out_state->next_pid = 0U;

        if (epcp->ep_mode == USB_EP_MODE_TYPE_ISOC) {
            buf_size = usb_isochronous_buffer_size(epcp->out_maxsize);
            buf_ctrl |= usb_isochronous_buffer_mode(buf_size) << USB_BUFFER_DOUBLE_BUFFER_OFFSET_Pos;
        } else {
            buf_size = 64;
        }
        buf_offset                = usb_buffer_next_offset(usbp, buf_size, false);
        epcp->out_state->hw_buf   = (uint8_t *)&USB_DPSRAM->DATA[buf_offset];
        epcp->out_state->buf_size = buf_size;

        EP_CTRL(ep).OUT  = USB_EP_EN | (epcp->ep_mode << USB_EP_TYPE_Pos) | ((uint8_t *)epcp->out_state->hw_buf - (uint8_t *)USB_DPSRAM);
        BUF_CTRL(ep).OUT = buf_ctrl;
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

  /* Reset the packet memory allocator. */
  usbp->noffset = 0U;

  for (uint8_t ep = 1; ep <= USB_MAX_ENDPOINTS; ep++) {
    EP_CTRL(ep).IN &= ~USB_EP_EN;
    EP_CTRL(ep).OUT &= ~USB_EP_EN;
  }
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

  if (BUF_CTRL(ep).OUT & USB_BUFFER_STALL) {
    return EP_STATUS_STALLED;
  }
  /* EP0 is always active once USB is started - it has no EPCTRL entry. */
  if (ep == 0U) {
    return EP_STATUS_ACTIVE;
  }
  if (EP_CTRL(ep).OUT & USB_EP_EN) {
    return EP_STATUS_ACTIVE;
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
  (void)usbp;

  if (BUF_CTRL(ep).IN & USB_BUFFER_STALL) {
    return EP_STATUS_STALLED;
  }
  /* EP0 is always active once USB is started - it has no EPCTRL entry. */
  if (ep == 0U) {
    return EP_STATUS_ACTIVE;
  }
  if (EP_CTRL(ep).IN & USB_EP_EN) {
    return EP_STATUS_ACTIVE;
  }
  return EP_STATUS_DISABLED;
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
  /* Copy data from hardware buffer to user buffer */
  usb_dpram_memcpy((void *)buf, (void *)USB_DPSRAM->SETUPPACKET, 8);
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
  USBOutEndpointState *oesp = usbp->epc[ep]->out_state;

  /* Transfer initialization.*/
  if (oesp->rxsize == 0U) {
    /* Special case for zero sized packets.*/
    oesp->rxpkts = 1U;
  } else {
    oesp->rxpkts = (uint16_t)((oesp->rxsize + usbp->epc[ep]->out_maxsize - 1) /
                             usbp->epc[ep]->out_maxsize);
  }

  usb_prepare_out_ep(usbp, ep);
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
  USBInEndpointState *iesp = usbp->epc[ep]->in_state;
  iesp->txlast = 0;

  /* Prepare IN endpoint. */
  usb_prepare_in_ep(usbp, ep);
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

    if (ep == 0) {
        USB->SET.EPSTALLARM = USB_EP_STALL_ARM_EP0_OUT;
    }
    BUF_CTRL(ep).OUT |= USB_BUFFER_STALL;
    __DSB();
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

    if (ep == 0) {
        USB->SET.EPSTALLARM = USB_EP_STALL_ARM_EP0_IN;
    }
    BUF_CTRL(ep).IN |= USB_BUFFER_STALL;
    __DSB();
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
    if (ep > 0) {
        BUF_CTRL(ep).OUT &= ~USB_BUFFER_STALL;
    }
    usbp->epc[ep]->out_state->next_pid = 0U;
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
    if (ep > 0) {
        BUF_CTRL(ep).IN &= ~USB_BUFFER_STALL;
    }
    usbp->epc[ep]->in_state->next_pid = 0U;
}

#endif /* HAL_USE_USB == TRUE */

/** @} */
