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

/*-*
 * @file    usb_msc.h
 * @brief   USB Mass Storage Class header.
 *
 * @addtogroup USB_MSC
 * @{
 */

#ifndef _USB_MSC_H_
#define _USB_MSC_H_

/*===========================================================================*/
/* Driver constants.                                                         */
/*===========================================================================*/

#define MSC_CBW_SIGNATURE               0x43425355
#define MSC_CSW_SIGNATURE               0x53425355

#define MSC_GET_MAX_LUN_COMMAND         0xFE
#define MSC_MASS_STORAGE_RESET_COMMAND  0xFF

#define MSC_CSW_STATUS_PASSED           0
#define MSC_CSW_STATUS_FAILED           1
#define MSC_CSW_STATUS_PHASE_ERROR      2


#define SCSI_FORMAT_UNIT            0x04
#define SCSI_INQUIRY                0x12
#define SCSI_MODE_SELECT6           0x15
#define SCSI_MODE_SELECT10          0x55
#define SCSI_MODE_SENSE6            0x1A
#define SCSI_MODE_SENSE10           0x5A
#define SCSI_ALLOW_MEDIUM_REMOVAL   0x1E
#define SCSI_READ6                  0x08
#define SCSI_READ10                 0x28
#define SCSI_READ12                 0xA8
#define SCSI_READ16                 0x88

#define SCSI_READ_CAPACITY10        0x25
#define SCSI_READ_CAPACITY16        0x9E

#define SCSI_REQUEST_SENSE          0x03
#define SCSI_START_STOP_UNIT        0x1B
#define SCSI_TEST_UNIT_READY        0x00
#define SCSI_WRITE6                 0x0A
#define SCSI_WRITE10                0x2A
#define SCSI_WRITE12                0xAA
#define SCSI_WRITE16                0x8A

#define SCSI_VERIFY10               0x2F
#define SCSI_VERIFY12               0xAF
#define SCSI_VERIFY16               0x8F

#define SCSI_SEND_DIAGNOSTIC        0x1D
#define SCSI_READ_FORMAT_CAPACITIES 0x23

/*===========================================================================*/
/* Driver pre-compile time settings.                                         */
/*===========================================================================*/

/**
 * @brief   Endpoint number for bulk IN.
 */
#if !defined(MSC_DATA_IN_EP) || defined(__DOXYGEN__)
#define MSC_DATA_IN_EP          1
#endif

/**
 * @brief   Endpoint number for bulk OUT.
 */
#if !defined(MSC_DATA_OUT_EP) || defined(__DOXYGEN__)
#define MSC_DATA_OUT_EP         2
#endif

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

/*===========================================================================*/
/* Driver data structures and types.                                         */
/*===========================================================================*/

/**
 * @brief   Type of the MSC possible states.
 */
typedef enum {
  MSC_IDLE = 0,
  MSC_DATA_OUT,
  MSC_DATA_IN,
  MSC_SENDING_CSW,
  MSC_ERROR
} mscstate_t;

/**
 * @brief   CBW structure.
 */
struct CBW {
  uint32_t          dCBWSignature;
  uint32_t          dCBWTag;
  uint32_t          dCBWDataTransferLength;
  uint8_t           bmCBWFlags;
  uint8_t           bCBWLUN;
  uint8_t           bCBWCBLength;
  uint8_t           CBWCB[16];
};

/**
 * @brief   CSW structure.
 */
struct CSW {
  uint32_t          dCSWSignature;
  uint32_t          dCSWTag;
  uint32_t          dCSWDataResidue;
  uint8_t           bCSWStatus;
};

/**
 * @brief   Type of a CBW structure.
 */
typedef struct CBW msccbw_t;

/**
 * @brief   Type of a CSW structure.
 */
typedef struct CSW msccsw_t;

/*===========================================================================*/
/* Driver macros.                                                            */
/*===========================================================================*/

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#ifdef __cplusplus
extern "C" {
#endif
  bool_t mscRequestsHook(USBDriver *usbp);
  void mscDataTransmitted(USBDriver *usbp, usbep_t ep);
  void mscDataReceived(USBDriver *usbp, usbep_t ep);
#ifdef __cplusplus
}
#endif

#endif  /* _USB_MSC_H_ */

/** @} */
