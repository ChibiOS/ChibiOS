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
 * @file    usb_msc.c
 * @brief   USB Mass Storage Class code.
 *
 * @addtogroup USB_MSC
 * @{
 */

#include "ch.h"
#include "hal.h"

#include "usb_msc.h"

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local variables.                                                   */
/*===========================================================================*/

/**
 * @brief   Zero-filled constant buffer.
 */
static const uint8_t zerobuf[4] = {0, 0, 0, 0};

/**
 * @brief   MSC state machine current state.
 */
static mscstate_t msc_state;

/**
 * @brief   Transfer lenght specified in the CBW.
 */
static uint32_t cbw_length;

/**
 * @brief   Tag specified in the CBW.
 */
static uint32_t cbw_tag;

/**
 * @brief   Transmitted lenght.
 */
static uint32_t csw_sent;

/**
 * @brief   Status .
 */
static uint8_t csw_status;

/**
 * @brief   Multi purpose I/O buffer.
 */
static union {
  uint8_t           buf[512];
  msccbw_t          CBW;
  msccsw_t          CSW;
} u;

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

/**
 * @brief   MSC state machine initialization.
 *
 * @param[in] usbp      pointer to the @p USBDriver object
 */
static void msc_reset(USBDriver *usbp) {

  msc_state = MSC_IDLE;
  chSysLockFromIsr();
  usbStartReceiveI(usbp, MSC_DATA_OUT_EP, u.buf, sizeof(u.buf));
  chSysUnlockFromIsr();
}

/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

/**
 * @brief   Default requests hook.
 * @details The application must use this function as callback for the
 *          messages hook.
 *          The following requests are emulated:
 *          - MSC_GET_MAX_LUN_COMMAND.
 *          - MSC_MASS_STORAGE_RESET_COMMAND.
 *          .
 *
 * @param[in] usbp      pointer to the @p USBDriver object
 * @return              The hook status.
 * @retval TRUE         Message handled internally.
 * @retval FALSE        Message not handled.
 */
bool_t mscRequestsHook(USBDriver *usbp) {

  if ((usbp->setup[0] & (USB_RTYPE_TYPE_MASK | USB_RTYPE_RECIPIENT_MASK)) ==
       (USB_RTYPE_TYPE_CLASS | USB_RTYPE_RECIPIENT_INTERFACE)) {
    switch (usbp->setup[1]) {
    case MSC_GET_MAX_LUN_COMMAND:
      usbSetupTransfer(usbp, (uint8_t *)zerobuf, 1, NULL);
      return TRUE;
    case MSC_MASS_STORAGE_RESET_COMMAND:
      msc_reset(usbp);
      usbSetupTransfer(usbp, NULL, 0, NULL);
      return TRUE;
    default:
      return FALSE;
    }
  }
  return FALSE;
}

/**
 * @brief   Default data transmitted callback.
 * @details The application must use this function as callback for the IN
 *          data endpoint.
 *
 * @param[in] usbp      pointer to the @p USBDriver object
 * @param[in] ep        endpoint number
 */
void mscDataTransmitted(USBDriver *usbp, usbep_t ep) {

  switch (msc_state) {
  case MSC_DATA_IN:
    u.CSW.dCSWSignature = MSC_CSW_SIGNATURE;
    u.CSW.dCSWTag = cbw_tag;
    u.CSW.dCSWDataResidue = cbw_length - csw_sent;
    u.CSW.bCSWStatus = csw_status;
    chSysLockFromIsr();
    usbStartTransmitI(usbp, ep, (uint8_t *)&u.CSW, sizeof(u.CSW));
    chSysUnlockFromIsr();
    msc_state = MSC_SENDING_CSW;
    break;
  case MSC_SENDING_CSW:
    chSysLockFromIsr();
    usbStartReceiveI(usbp, MSC_DATA_OUT_EP, u.buf, sizeof(u.buf));
    chSysUnlockFromIsr();
    msc_state = MSC_IDLE;
    break;
  default:
    ;
  }
}

/**
 * @brief   Default data received callback.
 * @details The application must use this function as callback for the OUT
 *          data endpoint.
 *
 * @param[in] usbp      pointer to the @p USBDriver object
 * @param[in] ep        endpoint number
 */
void mscDataReceived(USBDriver *usbp, usbep_t ep) {
  size_t n;

  n = usbGetReceiveTransactionSizeI(usbp, ep);
  switch (msc_state) {
  case MSC_IDLE:
    if ((n != sizeof(msccbw_t)) ||
        (u.CBW.dCBWSignature != MSC_CBW_SIGNATURE))
      goto stallout; /* 6.6.1 */

    cbw_length = u.CBW.dCBWDataTransferLength;
    cbw_tag = u.CBW.dCBWTag;
    if (u.CBW.bmCBWFlags & 0x80) {
      /* IN, Device to Host.*/
/*      if (scsi_decode_in(usbp))
        goto stallout;*/
      msc_state = MSC_DATA_IN;
    }
    else {
      /* OUT, Host to Device.*/
/*      if (scsi_decode_out(usbp))
        goto stallout;*/
      msc_state = MSC_DATA_OUT;
    }
    break;
  default:
    ;
  }
  return;
stallout:
  msc_state = MSC_ERROR;
  usbStallReceiveI(usbp, ep);
  return;
}
