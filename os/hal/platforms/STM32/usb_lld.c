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
 * @file    STM32/usb_lld.c
 * @brief   STM32 USB subsystem low level driver source.
 *
 * @addtogroup USB
 * @{
 */

#include <string.h>

#include "ch.h"
#include "hal.h"
#include "usb.h"

#if HAL_USE_USB || defined(__DOXYGEN__)

#define BTABLE_ADDR     0x0000

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

/** @brief USB1 driver identifier.*/
#if STM32_USB_USE_USB1 || defined(__DOXYGEN__)
USBDriver USBD1;
#endif


/*===========================================================================*/
/* Driver local variables.                                                   */
/*===========================================================================*/

/**
 * @brief   EP0 state.
 */
static USBEndpointState ep0state;

/**
 * @brief   EP0 initialization structure.
 */
static const USBEndpointConfig ep0config = {
  _usb_ep0in,
  _usb_ep0out,
  0x40,
  0x40,
  EPR_EP_TYPE_CONTROL | EPR_STAT_TX_NAK | EPR_STAT_RX_VALID,
  0x40,
  0x80
};

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

/**
 * @brief   Copies a packet from memory into a packet buffer.
 *
 * @param[in] ep        endpoint number
 * @param[in] buf       buffer where to fetch the endpoint data
 * @param[in] n         maximum number of bytes to copy
 */
static void write_packet(usbep_t ep, const uint8_t *buf, size_t n){
  uint32_t *pmap;
  stm32_usb_descriptor_t *udp;
  size_t count;

  udp = USB_GET_DESCRIPTOR(ep);
  pmap = USB_ADDR2PTR(udp->TXADDR);
  udp->TXCOUNT = n;
  count = (n + 1) / 2;
  while (count) {
    *pmap++ = *(uint16_t *)buf;
    buf += 2;
    count--;
  }
  EPR_SET_STAT_TX(ep, EPR_STAT_TX_VALID);
}

/**
 * @brief   Copies a packet from a packet buffer into memory.
 *
 * @param[in] ep        endpoint number
 * @param[in] buf       buffer where to copy the endpoint data
 * @param[in] n         maximum number of bytes to copy
 * @return              The packet size.
 * @retval 0            Special case, zero sized packet.
 */
static size_t read_packet(usbep_t ep, uint8_t *buf, size_t n){
  uint32_t *pmap;
  stm32_usb_descriptor_t *udp;
  size_t count;

  udp = USB_GET_DESCRIPTOR(ep);
  pmap = USB_ADDR2PTR(udp->RXADDR);
  count = udp->RXCOUNT & RXCOUNT_COUNT_MASK;
  if (n > count)
    n = count;
  count = (n + 1) / 2;
  while (count) {
    *(uint16_t *)buf = (uint16_t)*pmap++;
    buf += 2;
    count--;
  }
  return n;
}

/*===========================================================================*/
/* Driver interrupt handlers.                                                */
/*===========================================================================*/

#if STM32_USB_USE_USB1 || defined(__DOXYGEN__)
/**
 * @brief   USB high priority interrupt handler.
 *
 * @isr
 */
CH_IRQ_HANDLER(USB_HP_IRQHandler) {

  CH_IRQ_PROLOGUE();

  CH_IRQ_EPILOGUE();
}

/**
 * @brief   USB low priority interrupt handler.
 *
 * @isr
 */
CH_IRQ_HANDLER(USB_LP_IRQHandler) {
  uint32_t istr;
  size_t n;
  USBDriver *usbp = &USBD1;

  CH_IRQ_PROLOGUE();

  istr = STM32_USB->ISTR;

  /* USB bus reset condition handling.*/
  if (istr & ISTR_RESET) {
    _usb_reset(usbp);
    if (usbp->config->event_cb)
      usbp->config->event_cb(usbp, USB_EVENT_RESET);
    STM32_USB->ISTR = ~ISTR_RESET;
  }

  /* SOF handling.*/
  if (istr & ISTR_SOF) {
    if (usbp->config->sof_cb)
      usbp->config->sof_cb(usbp);
    STM32_USB->ISTR = ~ISTR_SOF;
  }

  /* Endpoint events handling.*/
  while (istr & ISTR_CTR) {
    uint32_t ep;
    uint32_t epr = STM32_USB->EPR[ep = istr & ISTR_EP_ID_MASK];
    const USBEndpointConfig *epcp = usbp->ep[ep]->config;

    if (epr & EPR_CTR_TX) {
      /* IN endpoint, transmission.*/
      EPR_CLEAR_CTR_TX(ep);
      n = USB_GET_DESCRIPTOR(ep)->TXCOUNT;
      usbp->ep[ep]->txbuf  += n;
      usbp->ep[ep]->txcnt  += n;
      usbp->ep[ep]->txsize -= n;
      if (usbp->ep[ep]->txsize > 0) {
        /* Transfer not completed, there are more packets to send.*/
        if (usbp->ep[ep]->txsize > epcp->in_maxsize)
          n = epcp->in_maxsize;
        else
          n = usbp->ep[ep]->txsize;
        write_packet(ep, usbp->ep[ep]->txbuf, n);
      }
      else {
        /* Transfer completed, invoking the callback, if defined.*/
        if (epcp->in_cb)
          epcp->in_cb(usbp, ep);
      }
    }
    if (epr & EPR_CTR_RX) {
      EPR_CLEAR_CTR_RX(ep);
      /* OUT endpoint, receive.*/
      if ((epr & EPR_SETUP) && (ep == 0)) {
        /* Special case, setup packet for EP0, enforcing a reset of the
           EP0 state machine for robustness.*/
        usbp->ep0state = USB_EP0_WAITING_SETUP;
        read_packet(0, usbp->setup, 8);
        epcp->out_cb(usbp, ep);
      }
      else {
        n = read_packet(ep, usbp->ep[ep]->rxbuf, usbp->ep[ep]->rxsize);
        usbp->ep[ep]->rxbuf  += n;
        usbp->ep[ep]->rxcnt  += n;
        usbp->ep[ep]->rxsize -= n;
        usbp->ep[ep]->rxpkts -= 1;
        if (usbp->ep[ep]->rxpkts > 0) {
          /* Transfer not completed, there are more packets to receive.*/
          EPR_SET_STAT_RX(ep, EPR_STAT_RX_VALID);
        }
        else {
          /* Transfer completed, invoking the callback, if defined.*/
          if (epcp->out_cb)
            epcp->out_cb(usbp, ep);
        }
      }
    }
    istr = STM32_USB->ISTR;
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

  /* USB reset, ensures reset state in order to avoid trouble with JTAGs.*/
  RCC->APB1RSTR = RCC_APB1RSTR_USBRST;
  RCC->APB1RSTR = 0;

  /* Driver initialization.*/
  usbObjectInit(&USBD1);
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
    /* Clock activation.*/
#if STM32_USB_USE_USB1
    if (&USBD1 == usbp) {
      /* USB clock enabled.*/
      RCC->APB1ENR |= RCC_APB1ENR_USBEN;
      /* Powers up the transceiver while holding the USB in reset state.*/
      STM32_USB->CNTR = CNTR_FRES;
      /* Enabling the USB IRQ vectors, this also gives enough time to allow
         the transceiver power up (1uS).*/
      NVICEnableVector(USB_HP_CAN1_TX_IRQn,
                       CORTEX_PRIORITY_MASK(STM32_USB_USB1_HP_IRQ_PRIORITY));
      NVICEnableVector(USB_LP_CAN1_RX0_IRQn,
                       CORTEX_PRIORITY_MASK(STM32_USB_USB1_LP_IRQ_PRIORITY));

      /* Reset procedure enforced on driver start.*/
      _usb_reset(&USBD1);
    }
#endif
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
  if (usbp->state == USB_STOP) {
#if STM32_ADC_USE_ADC1
    if (&USBD1 == usbp) {
      NVICDisableVector(USB_HP_CAN1_TX_IRQn);
      NVICDisableVector(USB_LP_CAN1_RX0_IRQn);
      RCC->APB1ENR &= ~RCC_APB1ENR_USBEN;
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

  /* Powers up the transceiver while holding the USB in reset state.*/
  STM32_USB->CNTR   = CNTR_FRES;

  /* Releases the USB reset, BTABLE is reset to zero.*/
  STM32_USB->CNTR   = 0;
  STM32_USB->ISTR   = 0;
  STM32_USB->DADDR  = DADDR_EF;
  cntr              = /*CNTR_ESOFM | */ CNTR_RESETM  | /*CNTR_SUSPM |*/
                      /*CNTR_WKUPM | CNTR_ERRM | CNTR_PMAOVRM |*/ CNTR_CTRM;
  /* The SOF interrupt is only enabled if a callback is defined for
     this service because it is an high rate source.*/
  if (usbp->config->sof_cb != NULL)
    cntr |= CNTR_SOFM;
  STM32_USB->CNTR = cntr;

  /* EP0 initialization.*/
  memset(&ep0state, 0, sizeof ep0state);
  ep0state.config = &ep0config;
  usbp->ep[0] = &ep0state;
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

  STM32_USB->DADDR = (uint32_t)(usbp->address) | DADDR_EF;
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
  uint16_t nblocks;
  stm32_usb_descriptor_t *dp;
  const USBEndpointConfig *epcp = usbp->ep[ep]->config;

  /* EPxR register setup.*/
  EPR_SET(ep, epcp->epr | ep);
  EPR_TOGGLE(ep, epcp->epr);

  /* Endpoint size and address initialization.*/
  if (epcp->out_maxsize > 62)
    nblocks = (((((epcp->out_maxsize - 1) | 0x1f) + 1) / 32) << 10) |
              0x8000;
  else
    nblocks = ((((epcp->out_maxsize - 1) | 1) + 1) / 2) << 10;
  dp = USB_GET_DESCRIPTOR(ep);
  dp->TXCOUNT = 0;
  dp->RXCOUNT = nblocks;
  dp->TXADDR  = epcp->inaddr;
  dp->RXADDR  = epcp->outaddr;
}

/**
 * @brief   Disables all the active endpoints except the endpoint zero.
 *
 * @param[in] usbp      pointer to the @p USBDriver object
 *
 * @notapi
 */
void usb_lld_disable_endpoints(USBDriver *usbp) {
  unsigned i;

  (void)usbp;
  for (i = 1; i <= USB_ENDOPOINTS_NUMBER; i++) {
    EPR_TOGGLE(i, 0);
    EPR_SET(i, 0);
  }

}

/**
 * @brief   Returns the number of bytes readable from the receive packet
 *          buffer.
 *
 * @param[in] usbp      pointer to the @p USBDriver object
 * @param[in] ep        endpoint number
 * @return              The number of bytes that are effectively available.
 * @retval 0            Data not yet available.
 *
 * @notapi
 */
size_t usb_lld_get_readable(USBDriver *usbp, usbep_t ep) {

  (void)usbp;
  if ((STM32_USB->EPR[ep] & EPR_STAT_RX_MASK) != EPR_STAT_RX_NAK)
    return 0;
  return (size_t)(USB_GET_DESCRIPTOR(ep)->RXCOUNT & RXCOUNT_COUNT_MASK);
}

/**
 * @brief   Endpoint read.
 * @details The buffered packet is copied into the user buffer and then
 *          the endpoint is brought to the valid state in order to allow
 *          reception of more data.
 *
 * @param[in] usbp      pointer to the @p USBDriver object
 * @param[in] ep        endpoint number
 * @param[out] buf      buffer where to copy the endpoint data
 * @param[in] n         maximum number of bytes to copy
 * @return              The number of bytes that were effectively available.
 * @retval 0            Data not yet available.
 *
 * @notapi
 */
size_t usb_lld_read(USBDriver *usbp, usbep_t ep, uint8_t *buf, size_t n) {
  uint32_t *pmap;
  stm32_usb_descriptor_t *udp;
  size_t count;

  (void)usbp;
  if ((STM32_USB->EPR[ep] & EPR_STAT_RX_MASK) != EPR_STAT_RX_NAK)
    return 0;

  udp = USB_GET_DESCRIPTOR(ep);
  pmap = USB_ADDR2PTR(udp->RXADDR);
  count = udp->RXCOUNT & RXCOUNT_COUNT_MASK;
  if (n > count)
    n = count;
  count = (n + 1) / 2;
  while (count) {
    *(uint16_t *)buf = (uint16_t)*pmap++;
    buf += 2;
    count--;
  }
  EPR_SET_STAT_RX(ep, EPR_STAT_RX_VALID);
  return n;
}
/**
 * @brief   Returns the number of bytes writeable to the transmit packet
 *          buffer.
 *
 * @param[in] usbp      pointer to the @p USBDriver object
 * @param[in] ep        endpoint number
 * @return              The number of bytes that can be written.
 * @retval 0            Endpoint not ready for transmission.
 *
 * @iclass
 */
size_t usb_lld_get_writeable(USBDriver *usbp, usbep_t ep) {

  if ((STM32_USB->EPR[ep] & EPR_STAT_TX_MASK) != EPR_STAT_TX_NAK)
    return 0;
  return (size_t)usbp->ep[ep]->config->in_maxsize;
}

/**
 * @brief   Endpoint write.
 * @details The user data is copied in the packer memory and then
 *          the endpoint is brought to the valid state in order to allow
 *          transmission.
 *
 * @param[in] usbp      pointer to the @p USBDriver object
 * @param[in] ep        endpoint number
 * @param[in] buf       buffer where to copy the endpoint data
 * @param[in] n         maximum number of bytes to copy
 * @return              The number of bytes that were effectively written.
 * @retval 0            Endpoint not ready for transmission.
 *
 * @notapi
 */
size_t usb_lld_write(USBDriver *usbp, usbep_t ep,
                     const uint8_t *buf,
                     size_t n) {
  uint32_t *pmap;
  stm32_usb_descriptor_t *udp;
  size_t count;

  (void)usbp;
  if ((STM32_USB->EPR[ep] & EPR_STAT_TX_MASK) != EPR_STAT_TX_NAK)
    return 0;

  udp = USB_GET_DESCRIPTOR(ep);
  pmap = USB_ADDR2PTR(udp->TXADDR);
  udp->TXCOUNT = n;
  count = (n + 1) / 2;
  while (count) {
    *pmap++ = *(uint16_t *)buf;
    buf += 2;
    count--;
  }
  EPR_SET_STAT_TX(ep, EPR_STAT_TX_VALID);
  return n;
}

/**
 * @brief   Returns the status of an OUT endpoint.
 *
 * @param[in] usbp      pointer to the @p USBDriver object
 * @param[in] ep        endpoint number
 *
 * @notapi
 */
usbepstatus_t usb_lld_get_status_out(USBDriver *usbp, usbep_t ep) {

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

/**
 * @brief   Returns the status of an IN endpoint.
 *
 * @param[in] usbp      pointer to the @p USBDriver object
 * @param[in] ep        endpoint number
 *
 * @notapi
 */
usbepstatus_t usb_lld_get_status_in(USBDriver *usbp, usbep_t ep) {

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

/**
 * @brief   Starts a receive operation on an OUT endpoint.
 *
 * @param[in] usbp      pointer to the @p USBDriver object
 * @param[in] ep        endpoint number
 * @param[out] buf      buffer where to copy the endpoint data
 * @param[in] n         maximum number of bytes to copy in the buffer
 *
 * @notapi
 */
void usb_lld_start_out(USBDriver *usbp, usbep_t ep,
                       uint8_t *buf, size_t n) {
  USBEndpointState *uesp = usbp->ep[ep];

  uesp->rxbuf  = buf;
  uesp->rxsize = n;
  uesp->rxcnt  = 0;
  if (uesp->rxsize == 0)    /* Special case for zero sized packets.*/
    uesp->rxpkts = 1;
  else
    uesp->rxpkts = (uint16_t)((n + uesp->config->out_maxsize - 1) /
                              uesp->config->out_maxsize);
  EPR_SET_STAT_RX(ep, EPR_STAT_RX_VALID);
}

/**
 * @brief   Starts a transmit operation on an IN endpoint.
 *
 * @param[in] usbp      pointer to the @p USBDriver object
 * @param[in] ep        endpoint number
 * @param[in] buf       buffer where to fetch the endpoint data
 * @param[in] n         maximum number of bytes to copy
 *
 * @notapi
 */
void usb_lld_start_in(USBDriver *usbp, usbep_t ep,
                      const uint8_t *buf, size_t n) {
  USBEndpointState *uesp = usbp->ep[ep];

  uesp->txbuf  = buf;
  uesp->txsize = n;
  uesp->txcnt  = 0;
  if (n > (size_t)uesp->config->in_maxsize)
    n = (size_t)uesp->config->in_maxsize;
  write_packet(ep, buf, n);
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
  EPR_SET_STAT_RX(ep, EPR_STAT_RX_STALL);
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
  EPR_SET_STAT_TX(ep, EPR_STAT_TX_STALL);
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

  /* Makes sure to not put to NAK an endpoint that is already
     transferring.*/
  if ((STM32_USB->EPR[ep] & EPR_STAT_RX_MASK) != EPR_STAT_RX_VALID)
    EPR_SET_STAT_TX(ep, EPR_STAT_RX_NAK);
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

  /* Makes sure to not put to NAK an endpoint that is already
     transferring.*/
  if ((STM32_USB->EPR[ep] & EPR_STAT_TX_MASK) != EPR_STAT_TX_VALID)
    EPR_SET_STAT_TX(ep, EPR_STAT_TX_NAK);
}

#endif /* HAL_USE_USB */

/** @} */
