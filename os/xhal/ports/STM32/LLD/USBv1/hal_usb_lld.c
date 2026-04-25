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
 * @file    USBv1/hal_usb_lld.c
 * @brief   STM32 USB subsystem low level driver source.
 *
 * @addtogroup USB
 * @{
 */

#include <string.h>

#include "hal.h"

#if HAL_USE_USB || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver local definitions.                                                 */
/*===========================================================================*/

#define BTABLE_ADDR     0x0000U

#define EPR_EP_TYPE_IS_ISO(bits) ((bits & EPR_EP_TYPE_MASK) == EPR_EP_TYPE_ISO)

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

#if STM32_USB_USE_USB1 || defined(__DOXYGEN__)
hal_usb_driver_c USBD1;

static const hal_usb_config_t default_usb_config = {
};
#endif

/*===========================================================================*/
/* Driver local variables and types.                                         */
/*===========================================================================*/

static union {
  USBInEndpointState in;
  USBOutEndpointState out;
} ep0_state;

static uint8_t ep0setup_buffer[8];

static const USBEndpointConfig ep0config = {
  .ep_mode          = USB_EP_MODE_TYPE_CTRL,
  .setup_cb         = _usb_ep0setup,
  .in_cb            = _usb_ep0in,
  .out_cb           = _usb_ep0out,
  .in_maxsize       = 0x40U,
  .out_maxsize      = 0x40U,
  .in_state         = &ep0_state.in,
  .out_state        = &ep0_state.out,
  .ep_buffers       = 1U,
  .setup_buf        = ep0setup_buffer
};

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

static void usb_pm_reset(hal_usb_driver_c *usbp) {

  /* The first 64 bytes are reserved for the descriptors table. The effective
     available RAM for endpoint buffers is just 448 bytes.*/
  usbp->pmnext = 64U;
}

static uint32_t usb_pm_alloc(hal_usb_driver_c *usbp, size_t size) {
  uint32_t next;

  next = usbp->pmnext;
  usbp->pmnext += (uint32_t)((size + 1U) & ~1U);

  osalDbgAssert(usbp->pmnext <= STM32_USB_PMA_SIZE, "PMA overflow");

  return next;
}

static size_t usb_packet_read_to_buffer(usbep_t ep, uint8_t *buf) {
  size_t i, n;
  stm32_usb_descriptor_t *udp = USB_GET_DESCRIPTOR(ep);
  stm32_usb_pma_t *pmap = USB_ADDR2PTR(udp->RXADDR0);
#if STM32_USB_USE_ISOCHRONOUS
  uint32_t epr = STM32_USB->EPR[ep];

  if (EPR_EP_TYPE_IS_ISO(epr) && ((epr & EPR_DTOG_RX) != 0U)) {
    n = (size_t)udp->RXCOUNT1 & RXCOUNT_COUNT_MASK;
  }
  else {
    n = (size_t)udp->RXCOUNT0 & RXCOUNT_COUNT_MASK;
  }
#else
  n = (size_t)udp->RXCOUNT0 & RXCOUNT_COUNT_MASK;
#endif

  i = n;

#if STM32_USB_USE_FAST_COPY
  while (i >= 16U) {
    uint32_t w;

    w = *(pmap + 0);
    *(buf + 0) = (uint8_t)w;
    *(buf + 1) = (uint8_t)(w >> 8);
    w = *(pmap + 1);
    *(buf + 2) = (uint8_t)w;
    *(buf + 3) = (uint8_t)(w >> 8);
    w = *(pmap + 2);
    *(buf + 4) = (uint8_t)w;
    *(buf + 5) = (uint8_t)(w >> 8);
    w = *(pmap + 3);
    *(buf + 6) = (uint8_t)w;
    *(buf + 7) = (uint8_t)(w >> 8);
    w = *(pmap + 4);
    *(buf + 8) = (uint8_t)w;
    *(buf + 9) = (uint8_t)(w >> 8);
    w = *(pmap + 5);
    *(buf + 10) = (uint8_t)w;
    *(buf + 11) = (uint8_t)(w >> 8);
    w = *(pmap + 6);
    *(buf + 12) = (uint8_t)w;
    *(buf + 13) = (uint8_t)(w >> 8);
    w = *(pmap + 7);
    *(buf + 14) = (uint8_t)w;
    *(buf + 15) = (uint8_t)(w >> 8);

    i -= 16U;
    buf += 16U;
    pmap += 8U;
  }
#endif /* STM32_USB_USE_FAST_COPY */

  while (i >= 2U) {
    uint32_t w = *pmap++;
    *buf++ = (uint8_t)w;
    *buf++ = (uint8_t)(w >> 8);
    i -= 2U;
  }

  if (i >= 1U) {
    *buf = (uint8_t)*pmap;
  }

  return n;
}

static void usb_packet_write_from_buffer(usbep_t ep,
                                         const uint8_t *buf,
                                         size_t n) {
  stm32_usb_descriptor_t *udp = USB_GET_DESCRIPTOR(ep);
  stm32_usb_pma_t *pmap = USB_ADDR2PTR(udp->TXADDR0);
  int i = (int)n;

#if STM32_USB_USE_ISOCHRONOUS
  uint32_t epr = STM32_USB->EPR[ep];

  if (EPR_EP_TYPE_IS_ISO(epr) && ((epr & EPR_DTOG_TX) != 0U)) {
    udp->TXCOUNT1 = (stm32_usb_pma_t)n;
  }
  else {
    udp->TXCOUNT0 = (stm32_usb_pma_t)n;
  }
#else
  udp->TXCOUNT0 = (stm32_usb_pma_t)n;
#endif

#if STM32_USB_USE_FAST_COPY
  while (i >= 16) {
    uint32_t w;

    w  = *(buf + 0);
    w |= *(buf + 1) << 8;
    *(pmap + 0) = (stm32_usb_pma_t)w;
    w  = *(buf + 2);
    w |= *(buf + 3) << 8;
    *(pmap + 1) = (stm32_usb_pma_t)w;
    w  = *(buf + 4);
    w |= *(buf + 5) << 8;
    *(pmap + 2) = (stm32_usb_pma_t)w;
    w  = *(buf + 6);
    w |= *(buf + 7) << 8;
    *(pmap + 3) = (stm32_usb_pma_t)w;
    w  = *(buf + 8);
    w |= *(buf + 9) << 8;
    *(pmap + 4) = (stm32_usb_pma_t)w;
    w  = *(buf + 10);
    w |= *(buf + 11) << 8;
    *(pmap + 5) = (stm32_usb_pma_t)w;
    w  = *(buf + 12);
    w |= *(buf + 13) << 8;
    *(pmap + 6) = (stm32_usb_pma_t)w;
    w  = *(buf + 14);
    w |= *(buf + 15) << 8;
    *(pmap + 7) = (stm32_usb_pma_t)w;

    i -= 16;
    buf += 16U;
    pmap += 8U;
  }
#endif /* STM32_USB_USE_FAST_COPY */

  while (i >= 2) {
    uint32_t w;

    w  = *buf++;
    w |= *buf++ << 8;
    *pmap++ = (stm32_usb_pma_t)w;
    i -= 2;
  }

  if (i != 0) {
    *pmap = (stm32_usb_pma_t)(*buf);
  }
}

static void usb_serve_endpoints(hal_usb_driver_c *usbp, uint32_t istr) {
  size_t n;
  uint32_t ep = istr & ISTR_EP_ID_MASK;
  uint32_t epr = STM32_USB->EPR[ep];
  const USBEndpointConfig *epcp = usbp->epc[ep];

  if ((istr & ISTR_DIR) == 0U) {
    USBInEndpointState *isp = epcp->in_state;

    EPR_CLEAR_CTR_TX(ep);

    isp->txcnt += isp->txlast;
    n = isp->txsize - isp->txcnt;
    if (n > 0U) {
      if (n > epcp->in_maxsize) {
        n = epcp->in_maxsize;
      }

      isp->txbuf += isp->txlast;
      isp->txlast = n;
      usb_packet_write_from_buffer((usbep_t)ep, isp->txbuf, n);

      EPR_SET_STAT_TX(ep, EPR_STAT_TX_VALID);
    }
    else {
      _usb_isr_invoke_in_cb(usbp, ep);
    }
  }
  else {
    EPR_CLEAR_CTR_RX(ep);

    if ((epr & EPR_SETUP) != 0U) {
      _usb_isr_invoke_setup_cb(usbp, ep);
    }
    else {
      USBOutEndpointState *osp = epcp->out_state;

      n = usb_packet_read_to_buffer((usbep_t)ep, osp->rxbuf);
      osp->rxbuf += n;

      osp->rxcnt  += n;
      osp->rxsize -= n;
      osp->rxpkts -= 1U;

      if ((n < epcp->out_maxsize) || (osp->rxpkts == 0U)) {
        _usb_isr_invoke_out_cb(usbp, ep);
      }
      else {
        EPR_SET_STAT_RX(ep, EPR_STAT_RX_VALID);
      }
    }
  }
}

/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

void usb_lld_init(void) {

  usbObjectInit(&USBD1);
}

const hal_usb_config_t *usb_lld_setcfg(hal_usb_driver_c *usbp,
                                       const hal_usb_config_t *config) {
  (void)usbp;

  if (config == NULL) {
    config = &default_usb_config;
  }

  return config;
}

const hal_usb_config_t *usb_lld_selcfg(hal_usb_driver_c *usbp,
                                       unsigned cfgnum) {
#if USB_USE_CONFIGURATIONS == TRUE
  if (cfgnum < usb_configurations.cfgsnum) {
    return usb_lld_setcfg(usbp, &usb_configurations.cfgs[cfgnum]);
  }
#else
  (void)usbp;
  (void)cfgnum;
#endif

  return NULL;
}

msg_t usb_lld_start(hal_usb_driver_c *usbp) {

  if (usbp->config == NULL) {
    usbp->config = &default_usb_config;
  }

  {
#if STM32_USB_USE_USB1
    if (&USBD1 == usbp) {

      osalDbgAssert((STM32_USBCLK >= (48000000U - STM32_USB_48MHZ_DELTA)) &&
                    (STM32_USBCLK <= (48000000U + STM32_USB_48MHZ_DELTA)),
                    "invalid clock frequency");

      rccEnableUSB(true);
      rccResetUSB();

      STM32_USB->CNTR = CNTR_FRES;
      STM32_USB->CNTR = 0U;
    }
#endif
    usb_lld_reset(usbp);
  }

  return HAL_RET_SUCCESS;
}

void usb_lld_stop(hal_usb_driver_c *usbp) {

  if (usbp->state != HAL_DRV_STATE_STOP) {
#if STM32_USB_USE_USB1
    if (&USBD1 == usbp) {

      STM32_USB->CNTR = CNTR_PDWN | CNTR_FRES;
      rccDisableUSB();
    }
#endif
  }
}

void usb_lld_reset(hal_usb_driver_c *usbp) {
  uint32_t cntr;

  STM32_USB->BTABLE = BTABLE_ADDR;
  STM32_USB->ISTR   = 0U;
  STM32_USB->DADDR  = DADDR_EF;
  cntr              = CNTR_RESETM | CNTR_SUSPM | CNTR_WKUPM |
                      CNTR_ERRM | CNTR_CTRM;
  if (usbp->binder != NULL) {
    cntr |= CNTR_SOFM;
  }
  STM32_USB->CNTR = cntr;

  usb_pm_reset(usbp);

  usbp->epc[0] = &ep0config;
  usb_lld_init_endpoint(usbp, 0U);
}

void usb_lld_set_address(hal_usb_driver_c *usbp) {

  (void)usbp;
  STM32_USB->DADDR = (uint32_t)(usbp->address) | DADDR_EF;
}

void usb_lld_init_endpoint(hal_usb_driver_c *usbp, usbep_t ep) {
  uint16_t epr;
  stm32_usb_descriptor_t *dp;
  const USBEndpointConfig *epcp = usbp->epc[ep];

  switch (epcp->ep_mode & USB_EP_MODE_TYPE) {
  case USB_EP_MODE_TYPE_ISOC:
#if STM32_USB_USE_ISOCHRONOUS
    osalDbgAssert((epcp->in_state == NULL) || (epcp->out_state == NULL),
                  "isochronous EP cannot be IN and OUT");
    epr = EPR_EP_TYPE_ISO;
    break;
#else
    osalDbgAssert(false, "isochronous support disabled");
#endif
  case USB_EP_MODE_TYPE_BULK:
    epr = EPR_EP_TYPE_BULK;
    break;
  case USB_EP_MODE_TYPE_INTR:
    epr = EPR_EP_TYPE_INTERRUPT;
    break;
  default:
    epr = EPR_EP_TYPE_CONTROL;
  }

  dp = USB_GET_DESCRIPTOR(ep);

  if (epcp->in_state != NULL) {
    dp->TXCOUNT0 = 0U;
    dp->TXADDR0  = (stm32_usb_pma_t)usb_pm_alloc(usbp, epcp->in_maxsize);

#if STM32_USB_USE_ISOCHRONOUS
    if (epr == EPR_EP_TYPE_ISO) {
      epr |= EPR_STAT_TX_VALID;
      dp->TXCOUNT1 = dp->TXCOUNT0;
      dp->TXADDR1  = dp->TXADDR0;
    }
    else {
      epr |= EPR_STAT_TX_NAK;
    }
#else
    epr |= EPR_STAT_TX_NAK;
#endif
  }

  if (epcp->out_state != NULL) {
    uint16_t nblocks;

    if (epcp->out_maxsize > 62U) {
      nblocks = (uint16_t)(((((uint32_t)epcp->out_maxsize - 1U) | 0x1FU) /
                            32U) << 10) | 0x8000U;
    }
    else {
      nblocks = (uint16_t)(((((uint32_t)(epcp->out_maxsize - 1U) | 1U) + 1U) /
                            2U) << 10);
    }
    dp->RXCOUNT0 = nblocks;
    dp->RXADDR0  = (stm32_usb_pma_t)usb_pm_alloc(usbp, epcp->out_maxsize);

#if STM32_USB_USE_ISOCHRONOUS
    if (epr == EPR_EP_TYPE_ISO) {
      epr |= EPR_STAT_RX_VALID;
      dp->RXCOUNT1 = dp->RXCOUNT0;
      dp->RXADDR1  = dp->RXADDR0;
    }
    else {
      epr |= EPR_STAT_RX_NAK;
    }
#else
    epr |= EPR_STAT_RX_NAK;
#endif
  }

  STM32_USB->EPR[ep] = STM32_USB->EPR[ep];
  STM32_USB->EPR[ep] = (uint32_t)epr | ep;
}

void usb_lld_disable_endpoints(hal_usb_driver_c *usbp) {
  unsigned i;

  usb_pm_reset(usbp);

  for (i = 1U; i <= USB_ENDPOINTS_NUMBER; i++) {
    STM32_USB->EPR[i] = STM32_USB->EPR[i];
    STM32_USB->EPR[i] = 0U;
  }
}

usbepstatus_t usb_lld_get_status_out(hal_usb_driver_c *usbp, usbep_t ep) {

  (void)usbp;
  switch (STM32_USB->EPR[ep] & EPR_STAT_RX_MASK) {
  case EPR_STAT_RX_DIS:
    return EP_STATUS_DISABLED;
  case EPR_STAT_RX_STALL:
    return EP_STATUS_STALLED;
  default:
    return EP_STATUS_ACTIVE;
  }
}

usbepstatus_t usb_lld_get_status_in(hal_usb_driver_c *usbp, usbep_t ep) {

  (void)usbp;
  switch (STM32_USB->EPR[ep] & EPR_STAT_TX_MASK) {
  case EPR_STAT_TX_DIS:
    return EP_STATUS_DISABLED;
  case EPR_STAT_TX_STALL:
    return EP_STATUS_STALLED;
  default:
    return EP_STATUS_ACTIVE;
  }
}

void usb_lld_read_setup(hal_usb_driver_c *usbp, usbep_t ep, uint8_t *buf) {
  stm32_usb_pma_t *pmap;
  stm32_usb_descriptor_t *udp;
  uint32_t n;

  (void)usbp;

  udp = USB_GET_DESCRIPTOR(ep);
  pmap = USB_ADDR2PTR(udp->RXADDR0);
  for (n = 0U; n < 4U; n++) {
    *(uint16_t *)(void *)buf = (uint16_t)*pmap++;
    buf += 2;
  }
}

void usb_lld_start_out(hal_usb_driver_c *usbp, usbep_t ep) {
  USBOutEndpointState *osp = usbp->epc[ep]->out_state;

  if (osp->rxsize == 0U) {
    osp->rxpkts = 1U;
  }
  else {
    osp->rxpkts = (uint16_t)((osp->rxsize + usbp->epc[ep]->out_maxsize - 1U) /
                             usbp->epc[ep]->out_maxsize);
  }

  EPR_SET_STAT_RX(ep, EPR_STAT_RX_VALID);
}

void usb_lld_start_in(hal_usb_driver_c *usbp, usbep_t ep) {
  size_t n;
  USBInEndpointState *isp = usbp->epc[ep]->in_state;

  n = isp->txsize;
  if (n > (size_t)usbp->epc[ep]->in_maxsize) {
    n = (size_t)usbp->epc[ep]->in_maxsize;
  }

  isp->txlast = n;
  usb_packet_write_from_buffer(ep, isp->txbuf, n);

  EPR_SET_STAT_TX(ep, EPR_STAT_TX_VALID);
}

void usb_lld_stall_out(hal_usb_driver_c *usbp, usbep_t ep) {

  (void)usbp;
  EPR_SET_STAT_RX(ep, EPR_STAT_RX_STALL);
}

void usb_lld_stall_in(hal_usb_driver_c *usbp, usbep_t ep) {

  (void)usbp;
  EPR_SET_STAT_TX(ep, EPR_STAT_TX_STALL);
}

void usb_lld_clear_out(hal_usb_driver_c *usbp, usbep_t ep) {

  (void)usbp;
  if ((STM32_USB->EPR[ep] & EPR_STAT_RX_MASK) != EPR_STAT_RX_VALID) {
    EPR_SET_STAT_RX(ep, EPR_STAT_RX_NAK);
  }
}

void usb_lld_clear_in(hal_usb_driver_c *usbp, usbep_t ep) {

  (void)usbp;
  if ((STM32_USB->EPR[ep] & EPR_STAT_TX_MASK) != EPR_STAT_TX_VALID) {
    EPR_SET_STAT_TX(ep, EPR_STAT_TX_NAK);
  }
}

void usb_lld_serve_endpoints_interrupt(hal_usb_driver_c *usbp) {
  uint32_t istr;

  istr = STM32_USB->ISTR;
  while ((istr & ISTR_CTR) != 0U) {
    usb_serve_endpoints(usbp, istr);
    istr = STM32_USB->ISTR;
  }
}

void usb_lld_serve_interrupt(hal_usb_driver_c *usbp) {
  uint32_t istr;

  istr = STM32_USB->ISTR;
  STM32_USB->ISTR = ~istr;

  if ((istr & ISTR_RESET) != 0U) {
    _usb_reset(usbp);
  }

  if ((istr & ISTR_SUSP) != 0U) {
    STM32_USB->CNTR |= CNTR_FSUSP;
#if STM32_USB_LOW_POWER_ON_SUSPEND
    STM32_USB->CNTR |= CNTR_LP_MODE;
#endif
    _usb_suspend(usbp);
  }

  if ((istr & ISTR_WKUP) != 0U) {
    uint32_t fnr = STM32_USB->FNR;
    if ((fnr & FNR_RXDP) == 0U) {
      STM32_USB->CNTR &= ~CNTR_FSUSP;
      _usb_wakeup(usbp);
    }
#if STM32_USB_LOW_POWER_ON_SUSPEND
    else {
      STM32_USB->CNTR |= CNTR_LP_MODE;
    }
#endif
  }

  if ((istr & ISTR_SOF) != 0U) {
    _usb_isr_invoke_sof_cb(usbp);
  }

  if ((istr & ISTR_ERR) != 0U) {
    /* CHTODO */
  }

  while ((istr & ISTR_CTR) != 0U) {
    usb_serve_endpoints(usbp, istr);
    istr = STM32_USB->ISTR;
  }
}

#endif /* HAL_USE_USB */

/** @} */
