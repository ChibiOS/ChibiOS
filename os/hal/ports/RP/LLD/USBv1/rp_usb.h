/*
    Copyright (C) 2006-2026 Giovanni Di Sirio.

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
 * @file    USBv1/rp_usb.h
 * @brief   RP2040/RP2350 USB registers layout header.
 * @note    This file provides unified USB register definitions for both
 *          RP2040 and RP2350 platforms.
 *
 * @addtogroup USB
 * @{
 */

#ifndef RP_USB_H
#define RP_USB_H

/*===========================================================================*/
/* Platform detection and configuration.                                     */
/*===========================================================================*/

#if defined(RP2040)
#define RP_USBCTRL_IRQ_NUMBER           5
#define RP_USBCTRL_IRQ_HANDLER          Vector54
#elif defined(RP2350)
#define RP_USBCTRL_IRQ_NUMBER           14
#define RP_USBCTRL_IRQ_HANDLER          Vector78
#else
#error "Unknown RP platform for USB driver"
#endif

/*===========================================================================*/
/* Driver constants.                                                         */
/*===========================================================================*/

/*===========================================================================*/
/* DPRAM memory structure.                                                   */
/*===========================================================================*/

/**
 * @brief   USB DPRAM base address.
 */
#define USB_DPRAM_BASE                  0x50100000U

/**
 * @brief   USB DPRAM size in bytes.
 */
#define USB_DPRAM_SIZE                  4096U

/**
 * @brief   USB DPRAM data buffer region size.
 * @details This is the size of the DATA region used for EP1-15 buffers.
 *          EP0 has separate dedicated buffers (EP0BUF0/EP0BUF1) and does
 *          not consume space from this region.
 */
#define USB_DPRAM_DATA_SIZE             3712U

/**
 * @brief   USB DPRAM structure for device mode.
 */
typedef struct {
  __IO uint8_t      SETUPPACKET[8];     /**< @brief Setup packet buffer.   */
  struct {
    __IO uint32_t   IN;                 /**< @brief IN endpoint control.   */
    __IO uint32_t   OUT;                /**< @brief OUT endpoint control.  */
  } EPCTRL[15];                         /**< @brief EP1-15 control regs.   */
  struct {
    __IO uint32_t   IN;                 /**< @brief IN buffer control.     */
    __IO uint32_t   OUT;                /**< @brief OUT buffer control.    */
  } BUFCTRL[16];                        /**< @brief EP0-15 buffer control. */
  __IO uint8_t      EP0BUF0[64];        /**< @brief EP0 buffer A.          */
  __IO uint8_t      EP0BUF1[64];        /**< @brief EP0 buffer B.          */
  __IO uint8_t      DATA[USB_DPRAM_DATA_SIZE]; /**< @brief EP1-15 data buffers. */
} USB_DPRAM_TypeDef;

/**
 * @brief   USB DPRAM pointer.
 */
#define USB_DPSRAM          ((USB_DPRAM_TypeDef *)USB_DPRAM_BASE)

/*===========================================================================*/
/* Address and endpoint register bits.                                       */
/*===========================================================================*/

#define USB_ADDR_ENDP0_ENDPOINT_Pos         16U
#define USB_ADDR_ENDP0_ENDPOINT_Msk         (0xFU << USB_ADDR_ENDP0_ENDPOINT_Pos)
#define USB_ADDR_ENDP0_ADDRESS_Pos          0U
#define USB_ADDR_ENDP0_ADDRESS_Msk          (0x7FU << USB_ADDR_ENDP0_ADDRESS_Pos)

#define USB_ADDR_ENDP_INTEP_PREAMBLE        (1U << 26)
#define USB_ADDR_ENDP_INTEP_DIR             (1U << 25)
#define USB_ADDR_ENDP_ENDPOINT_Pos          16U
#define USB_ADDR_ENDP_ENDPOINT_Msk          (0xFU << USB_ADDR_ENDP_ENDPOINT_Pos)
#define USB_ADDR_ENDP_ADDRESS_Pos           0U
#define USB_ADDR_ENDP_ADDRESS_Msk           (0x7FU << USB_ADDR_ENDP_ADDRESS_Pos)

/*===========================================================================*/
/* Main control register bits.                                               */
/*===========================================================================*/

#define USB_MAIN_CTRL_SIM_TIMING            (1U << 31)
#define USB_MAIN_CTRL_HOST_NDEVICE          (1U << 1)
#define USB_MAIN_CTRL_CONTROLLER_EN         (1U << 0)

/*===========================================================================*/
/* SOF register bits.                                                        */
/*===========================================================================*/

#define USB_SOF_WR_COUNT_Pos                0U
#define USB_SOF_WR_COUNT_Msk                (0x3FFU << USB_SOF_WR_COUNT_Pos)

#define USB_SOF_RD_COUNT_Pos                0U
#define USB_SOF_RD_COUNT_Msk                (0x3FFU << USB_SOF_RD_COUNT_Pos)

/*===========================================================================*/
/* SIE control register bits.                                                */
/*===========================================================================*/

#define USB_SIE_CTRL_EP0_INT_STALL          (1U << 31)
#define USB_SIE_CTRL_EP0_DOUBLE_BUF         (1U << 30)
#define USB_SIE_CTRL_EP0_INT_1BUF           (1U << 29)
#define USB_SIE_CTRL_EP0_INT_2BUF           (1U << 28)
#define USB_SIE_CTRL_EP0_INT_NAK            (1U << 27)
#define USB_SIE_CTRL_DIRECT_EN              (1U << 26)
#define USB_SIE_CTRL_DIRECT_DP              (1U << 25)
#define USB_SIE_CTRL_DIRECT_DM              (1U << 24)
#define USB_SIE_CTRL_TRANSCEIVER_PD         (1U << 18)
#define USB_SIE_CTRL_RPU_OPT                (1U << 17)
#define USB_SIE_CTRL_PULLUP_EN              (1U << 16)
#define USB_SIE_CTRL_PULLDOWN_EN            (1U << 15)
#define USB_SIE_CTRL_RESET_BUS              (1U << 13)
#define USB_SIE_CTRL_RESUME                 (1U << 12)
#define USB_SIE_CTRL_VBUS_EN                (1U << 11)
#define USB_SIE_CTRL_KEEP_ALIVE_EN          (1U << 10)
#define USB_SIE_CTRL_SOF_EN                 (1U << 9)
#define USB_SIE_CTRL_SOF_SYNC               (1U << 8)
#define USB_SIE_CTRL_PREAMBLE_EN            (1U << 6)
#define USB_SIE_CTRL_STOP_TRANS             (1U << 4)
#define USB_SIE_CTRL_RECEIVE_DATA           (1U << 3)
#define USB_SIE_CTRL_SEND_DATA              (1U << 2)
#define USB_SIE_CTRL_SEND_SETUP             (1U << 1)
#define USB_SIE_CTRL_START_TRANS            (1U << 0)

/*===========================================================================*/
/* SIE status register bits.                                                 */
/*===========================================================================*/

#define USB_SIE_STATUS_DATA_SEQ_ERROR       (1U << 31)
#define USB_SIE_STATUS_ACK_REC              (1U << 30)
#define USB_SIE_STATUS_STALL_REC            (1U << 29)
#define USB_SIE_STATUS_NAK_REC              (1U << 28)
#define USB_SIE_STATUS_RX_TIMEOUT           (1U << 27)
#define USB_SIE_STATUS_RX_OVERFLOW          (1U << 26)
#define USB_SIE_STATUS_BIT_STUFF_ERROR      (1U << 25)
#define USB_SIE_STATUS_CRC_ERROR            (1U << 24)
#define USB_SIE_STATUS_BUS_RESET            (1U << 19)
#define USB_SIE_STATUS_TRANS_COMPLETE       (1U << 18)
#define USB_SIE_STATUS_SETUP_REC            (1U << 17)
#define USB_SIE_STATUS_CONNECTED            (1U << 16)
#define USB_SIE_STATUS_RESUME               (1U << 11)
#define USB_SIE_STATUS_VBUS_OVER_CURR       (1U << 10)
#define USB_SIE_STATUS_SPEED_Pos            8U
#define USB_SIE_STATUS_SPEED_Msk            (0x3U << USB_SIE_STATUS_SPEED_Pos)
#define USB_SIE_STATUS_SUSPENDED            (1U << 4)
#define USB_SIE_STATUS_LINE_STATE_Pos       2U
#define USB_SIE_STATUS_LINE_STATE_Msk       (0x3U << USB_SIE_STATUS_LINE_STATE_Pos)
#define USB_SIE_STATUS_VBUS_DETECTED        (1U << 0)

/*===========================================================================*/
/* Buffer status register bits.                                              */
/*===========================================================================*/

#define USB_BUFF_STATUS_EP15_OUT            (1U << 31)
#define USB_BUFF_STATUS_EP15_IN             (1U << 30)
#define USB_BUFF_STATUS_EP14_OUT            (1U << 29)
#define USB_BUFF_STATUS_EP14_IN             (1U << 28)
#define USB_BUFF_STATUS_EP13_OUT            (1U << 27)
#define USB_BUFF_STATUS_EP13_IN             (1U << 26)
#define USB_BUFF_STATUS_EP12_OUT            (1U << 25)
#define USB_BUFF_STATUS_EP12_IN             (1U << 24)
#define USB_BUFF_STATUS_EP11_OUT            (1U << 23)
#define USB_BUFF_STATUS_EP11_IN             (1U << 22)
#define USB_BUFF_STATUS_EP10_OUT            (1U << 21)
#define USB_BUFF_STATUS_EP10_IN             (1U << 20)
#define USB_BUFF_STATUS_EP9_OUT             (1U << 19)
#define USB_BUFF_STATUS_EP9_IN              (1U << 18)
#define USB_BUFF_STATUS_EP8_OUT             (1U << 17)
#define USB_BUFF_STATUS_EP8_IN              (1U << 16)
#define USB_BUFF_STATUS_EP7_OUT             (1U << 15)
#define USB_BUFF_STATUS_EP7_IN              (1U << 14)
#define USB_BUFF_STATUS_EP6_OUT             (1U << 13)
#define USB_BUFF_STATUS_EP6_IN              (1U << 12)
#define USB_BUFF_STATUS_EP5_OUT             (1U << 11)
#define USB_BUFF_STATUS_EP5_IN              (1U << 10)
#define USB_BUFF_STATUS_EP4_OUT             (1U << 9)
#define USB_BUFF_STATUS_EP4_IN              (1U << 8)
#define USB_BUFF_STATUS_EP3_OUT             (1U << 7)
#define USB_BUFF_STATUS_EP3_IN              (1U << 6)
#define USB_BUFF_STATUS_EP2_OUT             (1U << 5)
#define USB_BUFF_STATUS_EP2_IN              (1U << 4)
#define USB_BUFF_STATUS_EP1_OUT             (1U << 3)
#define USB_BUFF_STATUS_EP1_IN              (1U << 2)
#define USB_BUFF_STATUS_EP0_OUT             (1U << 1)
#define USB_BUFF_STATUS_EP0_IN              (1U << 0)

/*===========================================================================*/
/* EP stall arm register bits.                                               */
/*===========================================================================*/

#define USB_EP_STALL_ARM_EP0_OUT            (1U << 1)
#define USB_EP_STALL_ARM_EP0_IN             (1U << 0)

/*===========================================================================*/
/* NAK poll register bits.                                                   */
/*===========================================================================*/

#define USB_NAK_POLL_DELAY_FS_Pos           16U
#define USB_NAK_POLL_DELAY_FS_Msk           (0x3FFU << USB_NAK_POLL_DELAY_FS_Pos)
#define USB_NAK_POLL_DELAY_LS_Pos           0U
#define USB_NAK_POLL_DELAY_LS_Msk           (0x3FFU << USB_NAK_POLL_DELAY_LS_Pos)

/*===========================================================================*/
/* USB muxing register bits.                                                 */
/*===========================================================================*/

#define USB_USB_MUXING_SOFTCON              (1U << 3)
#define USB_USB_MUXING_TO_DIGITAL_PAD       (1U << 2)
#define USB_USB_MUXING_TO_EXTPHY            (1U << 1)
#define USB_USB_MUXING_TO_PHY               (1U << 0)

/*===========================================================================*/
/* USB power register bits.                                                  */
/*===========================================================================*/

#define USB_USB_PWR_OVERCURR_DETECT_EN      (1U << 5)
#define USB_USB_PWR_OVERCURR_DETECT         (1U << 4)
#define USB_USB_PWR_VBUS_DETECT_OVERRIDE_EN (1U << 3)
#define USB_USB_PWR_VBUS_DETECT             (1U << 2)
#define USB_USB_PWR_VBUS_EN_OVERRIDE_EN     (1U << 1)
#define USB_USB_PWR_VBUS_EN                 (1U << 0)

/*===========================================================================*/
/* Interrupt register bits.                                                  */
/*===========================================================================*/

#define USB_INTR_EP_STALL_NAK               (1U << 19)
#define USB_INTR_ABORT_DONE                 (1U << 18)
#define USB_INTR_DEV_SOF                    (1U << 17)
#define USB_INTR_SETUP_REQ                  (1U << 16)
#define USB_INTR_DEV_RESUME_FROM_HOST       (1U << 15)
#define USB_INTR_DEV_SUSPEND                (1U << 14)
#define USB_INTR_DEV_CONN_DIS               (1U << 13)
#define USB_INTR_BUS_RESET                  (1U << 12)
#define USB_INTR_VBUS_DETECT                (1U << 11)
#define USB_INTR_STALL                      (1U << 10)
#define USB_INTR_ERROR_CRC                  (1U << 9)
#define USB_INTR_ERROR_BIT_STUFF            (1U << 8)
#define USB_INTR_ERROR_RX_OVERFLOW          (1U << 7)
#define USB_INTR_ERROR_RX_TIMEOUT           (1U << 6)
#define USB_INTR_ERROR_DATA_SEQ             (1U << 5)
#define USB_INTR_BUFF_STATUS                (1U << 4)
#define USB_INTR_TRANS_COMPLETE             (1U << 3)
#define USB_INTR_HOST_SOF                   (1U << 2)
#define USB_INTR_HOST_RESUME                (1U << 1)
#define USB_INTR_HOST_CONN_DIS              (1U << 0)

#define USB_INTE_EP_STALL_NAK               (1U << 19)
#define USB_INTE_ABORT_DONE                 (1U << 18)
#define USB_INTE_DEV_SOF                    (1U << 17)
#define USB_INTE_SETUP_REQ                  (1U << 16)
#define USB_INTE_DEV_RESUME_FROM_HOST       (1U << 15)
#define USB_INTE_DEV_SUSPEND                (1U << 14)
#define USB_INTE_DEV_CONN_DIS               (1U << 13)
#define USB_INTE_BUS_RESET                  (1U << 12)
#define USB_INTE_VBUS_DETECT                (1U << 11)
#define USB_INTE_STALL                      (1U << 10)
#define USB_INTE_ERROR_CRC                  (1U << 9)
#define USB_INTE_ERROR_BIT_STUFF            (1U << 8)
#define USB_INTE_ERROR_RX_OVERFLOW          (1U << 7)
#define USB_INTE_ERROR_RX_TIMEOUT           (1U << 6)
#define USB_INTE_ERROR_DATA_SEQ             (1U << 5)
#define USB_INTE_BUFF_STATUS                (1U << 4)
#define USB_INTE_TRANS_COMPLETE             (1U << 3)
#define USB_INTE_HOST_SOF                   (1U << 2)
#define USB_INTE_HOST_RESUME                (1U << 1)
#define USB_INTE_HOST_CONN_DIS              (1U << 0)

#define USB_INTF_EP_STALL_NAK               (1U << 19)
#define USB_INTF_ABORT_DONE                 (1U << 18)
#define USB_INTF_DEV_SOF                    (1U << 17)
#define USB_INTF_SETUP_REQ                  (1U << 16)
#define USB_INTF_DEV_RESUME_FROM_HOST       (1U << 15)
#define USB_INTF_DEV_SUSPEND                (1U << 14)
#define USB_INTF_DEV_CONN_DIS               (1U << 13)
#define USB_INTF_BUS_RESET                  (1U << 12)
#define USB_INTF_VBUS_DETECT                (1U << 11)
#define USB_INTF_STALL                      (1U << 10)
#define USB_INTF_ERROR_CRC                  (1U << 9)
#define USB_INTF_ERROR_BIT_STUFF            (1U << 8)
#define USB_INTF_ERROR_RX_OVERFLOW          (1U << 7)
#define USB_INTF_ERROR_RX_TIMEOUT           (1U << 6)
#define USB_INTF_ERROR_DATA_SEQ             (1U << 5)
#define USB_INTF_BUFF_STATUS                (1U << 4)
#define USB_INTF_TRANS_COMPLETE             (1U << 3)
#define USB_INTF_HOST_SOF                   (1U << 2)
#define USB_INTF_HOST_RESUME                (1U << 1)
#define USB_INTF_HOST_CONN_DIS              (1U << 0)

#define USB_INTS_EP_STALL_NAK               (1U << 19)
#define USB_INTS_ABORT_DONE                 (1U << 18)
#define USB_INTS_DEV_SOF                    (1U << 17)
#define USB_INTS_SETUP_REQ                  (1U << 16)
#define USB_INTS_DEV_RESUME_FROM_HOST       (1U << 15)
#define USB_INTS_DEV_SUSPEND                (1U << 14)
#define USB_INTS_DEV_CONN_DIS               (1U << 13)
#define USB_INTS_BUS_RESET                  (1U << 12)
#define USB_INTS_VBUS_DETECT                (1U << 11)
#define USB_INTS_STALL                      (1U << 10)
#define USB_INTS_ERROR_CRC                  (1U << 9)
#define USB_INTS_ERROR_BIT_STUFF            (1U << 8)
#define USB_INTS_ERROR_RX_OVERFLOW          (1U << 7)
#define USB_INTS_ERROR_RX_TIMEOUT           (1U << 6)
#define USB_INTS_ERROR_DATA_SEQ             (1U << 5)
#define USB_INTS_BUFF_STATUS                (1U << 4)
#define USB_INTS_TRANS_COMPLETE             (1U << 3)
#define USB_INTS_HOST_SOF                   (1U << 2)
#define USB_INTS_HOST_RESUME                (1U << 1)
#define USB_INTS_HOST_CONN_DIS              (1U << 0)

/*===========================================================================*/
/* Endpoint control register bits (DPRAM).                                   */
/*===========================================================================*/

#define USB_EP_EN                           (1U << 31)
#define USB_EP_BUFFER_DOUBLE                (1U << 30)
#define USB_EP_BUFFER_IRQ_EN                (1U << 29)
#define USB_EP_BUFFER_IRQ_DOUBLE_EN         (1U << 28)
#define USB_EP_TYPE_Pos                     26U
#define USB_EP_TYPE_Msk                     (0x3U << USB_EP_TYPE_Pos)
#define USB_EP_IRQ_STALL                    (1U << 17)
#define USB_EP_IRQ_NAK                      (1U << 16)
#define USB_EP_ADDR_OFFSET_Pos              0U
#define USB_EP_ADDR_OFFSET_Msk              (0xFFFFU << USB_EP_ADDR_OFFSET_Pos)

/*===========================================================================*/
/* Buffer control register bits (DPRAM).                                     */
/*===========================================================================*/

#define USB_BUFFER_BUFFER1_FULL             (1U << 31)
#define USB_BUFFER_BUFFER1_LAST             (1U << 30)
#define USB_BUFFER_BUFFER1_DATA_PID         (1U << 29)
#define USB_BUFFER_DOUBLE_BUFFER_OFFSET_Pos 27U
#define USB_BUFFER_DOUBLE_BUFFER_OFFSET_Msk (0x3U << USB_BUFFER_DOUBLE_BUFFER_OFFSET_Pos)
#define USB_BUFFER_BUFFER1_AVAILABLE        (1U << 26)
#define USB_BUFFER_BUFFER1_TRANS_LENGTH_Pos 16U
#define USB_BUFFER_BUFFER1_TRANS_LENGTH_Msk (0x3FFU << USB_BUFFER_BUFFER1_TRANS_LENGTH_Pos)
#define USB_BUFFER_BUFFER0_FULL             (1U << 15)
#define USB_BUFFER_BUFFER0_LAST             (1U << 14)
#define USB_BUFFER_BUFFER0_DATA_PID         (1U << 13)
#define USB_BUFFER_RESET_BUFFER             (1U << 12)
#define USB_BUFFER_STALL                    (1U << 11)
#define USB_BUFFER_BUFFER0_AVAILABLE        (1U << 10)
#define USB_BUFFER_BUFFER0_TRANS_LENGTH_Pos 0U
#define USB_BUFFER_BUFFER0_TRANS_LENGTH_Msk (0x3FFU << USB_BUFFER_BUFFER0_TRANS_LENGTH_Pos)

#endif /* RP_USB_H */

/** @} */
