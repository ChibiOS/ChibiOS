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

/*-*
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
 * @brief   Answer to the INQUIRY command.
 */
static const uint8_t scsi_inquiry_data[] = {
  0x00,             /* Direct Access Device.      */
  0x80,             /* RMB = 1: Removable Medium. */
  0x02,             /* ISO, ECMA, ANSI = 2.       */
  0x00,             /* UFI response format.       */

  36 - 4,           /* Additional Length.         */
  0x00,
  0x00,
  0x00,
  /* Vendor Identification */
  'C', 'h', 'i', 'b', 'i', 'O', 'S', ' ',
  /* Product Identification */
  'S', 'D', ' ', 'F', 'l', 'a', 's', 'h',
  ' ', 'D', 'i', 's', 'k', ' ', ' ', ' ',
  /* Product Revision Level */
  '1', '.', '0', ' '
};

/**
 * @brief   Generic buffer.
 */
uint8_t buf[16];

/*===========================================================================*/
/* MMC interface code.                                                       */
/*===========================================================================*/

/*===========================================================================*/
/* SCSI emulation code.                                                      */
/*===========================================================================*/

static uint8_t scsi_read_format_capacities(uint32_t *nblocks,
                                           uint32_t *secsize) {

  *nblocks = 1024;
  *secsize = 512;
  return 3; /* No Media.*/
}

/*===========================================================================*/
/* Mass Storage Class related code.                                          */
/*===========================================================================*/

/**
 * @brief   MSC state machine current state.
 */
static mscstate_t msc_state;

/**
 * @brief   Received CBW.
 */
static msccbw_t CBW;

/**
 * @brief   CSW to be transmitted.
 */
static msccsw_t CSW;

/**
 * @brief   MSC state machine initialization.
 *
 * @param[in] usbp      pointer to the @p USBDriver object
 */
static void msc_reset(USBDriver *usbp) {

  msc_state = MSC_IDLE;
  chSysLockFromIsr();
  usbStartReceiveI(usbp, MSC_DATA_OUT_EP, (uint8_t *)&CBW, sizeof CBW);
  chSysUnlockFromIsr();
}

static void msc_transmit(USBDriver *usbp, const uint8_t *p, size_t n) {

  if (n > CBW.dCBWDataTransferLength)
    n = CBW.dCBWDataTransferLength;
  CSW.dCSWDataResidue = CBW.dCBWDataTransferLength - (uint32_t)n;
  chSysLockFromIsr();
  usbStartTransmitI(usbp, MSC_DATA_IN_EP, p, n);
  chSysUnlockFromIsr();
}

static void msc_sendstatus(USBDriver *usbp) {

  msc_state = MSC_SENDING_CSW;
  chSysLockFromIsr();
  usbStartTransmitI(usbp, MSC_DATA_IN_EP, (uint8_t *)&CSW, sizeof CSW);
  chSysUnlockFromIsr();
}

static bool_t msc_decode(USBDriver *usbp) {
  uint32_t nblocks, secsize;

  switch (CBW.CBWCB[0]) {
  case SCSI_REQUEST_SENSE:
    break;
  case SCSI_INQUIRY:
    msc_transmit(usbp, (uint8_t *)&scsi_inquiry_data,
                 sizeof scsi_inquiry_data);
    CSW.bCSWStatus = MSC_CSW_STATUS_PASSED;
    break;
  case SCSI_READ_FORMAT_CAPACITIES:
    buf[8]  = scsi_read_format_capacities(&nblocks, &secsize);
    buf[0]  = buf[1] = buf[2] = 0;
    buf[3]  = 8;
    buf[4]  = (uint8_t)(nblocks >> 24);
    buf[5]  = (uint8_t)(nblocks >> 16);
    buf[6]  = (uint8_t)(nblocks >> 8);
    buf[7]  = (uint8_t)(nblocks >> 0);
    buf[9]  = (uint8_t)(secsize >> 16);
    buf[10] = (uint8_t)(secsize >> 8);
    buf[11] = (uint8_t)(secsize >> 0);
    msc_transmit(usbp, buf, 12);
    CSW.bCSWStatus = MSC_CSW_STATUS_PASSED;
    break;
  default:
    return TRUE;
  }
  return FALSE;
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
    CSW.dCSWSignature = MSC_CSW_SIGNATURE;
    CSW.dCSWTag = CBW.dCBWTag;
    chSysLockFromIsr();
    usbStartTransmitI(usbp, ep, (uint8_t *)&CSW, sizeof CSW);
    chSysUnlockFromIsr();
    msc_state = MSC_SENDING_CSW;
    break;
  case MSC_SENDING_CSW:
    chSysLockFromIsr();
    usbStartReceiveI(usbp, MSC_DATA_OUT_EP, (uint8_t *)&CBW, sizeof CBW);
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
    if ((n != sizeof(msccbw_t)) || (CBW.dCBWSignature != MSC_CBW_SIGNATURE))
      goto stall_out; /* 6.6.1 */

    /* Decoding SCSI command.*/
    if (msc_decode(usbp)) {
      if (CBW.dCBWDataTransferLength == 0) {
        CSW.bCSWStatus = MSC_CSW_STATUS_FAILED;
        CSW.dCSWDataResidue = 0;
        msc_sendstatus(usbp);
        return;
      }
      goto stall_both;
    }

    /* Commands with zero transfer length, 5.1.*/
    if (CBW.dCBWDataTransferLength == 0) {
      msc_sendstatus(usbp);
      return;
    }

    /* Transfer direction.*/
    if (CBW.bmCBWFlags & 0x80) {
      /* IN, Device to Host.*/
      msc_state = MSC_DATA_IN;
    }
    else {
      /* OUT, Host to Device.*/
      msc_state = MSC_DATA_OUT;
    }
    break;
  case MSC_DATA_OUT:
    break;
  default:
    ;
  }
  return;
stall_out:
  msc_state = MSC_ERROR;
  chSysLockFromIsr();
  usbStallReceiveI(usbp, ep);
  chSysUnlockFromIsr();
  return;
stall_both:
  msc_state = MSC_ERROR;
  chSysLockFromIsr();
  usbStallTransmitI(usbp, ep);
  usbStallReceiveI(usbp, ep);
  chSysUnlockFromIsr();
  return;
}

/** @} */
