/*
    ChibiOS/RT - Copyright (C) 2006,2007,2008,2009,2010,
                 2011 Giovanni Di Sirio.

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
 * @file    usb_cdc.h
 * @brief   USB Communication Device Class support header.
 *
 * @addtogroup USB_CDC
 * @{
 */

#ifndef _USB_CDC_H_
#define _USB_CDC_H_

/*===========================================================================*/
/* Driver constants.                                                         */
/*===========================================================================*/

/**
 * @name    CDC specific messages.
 * @{
 */
#define CDC_SEND_ENCAPSULATED_COMMAND   0x00
#define CDC_GET_ENCAPSULATED_RESPONSE   0x01
#define CDC_SET_COMM_FEATURE            0x02
#define CDC_GET_COMM_FEATURE            0x03
#define CDC_CLEAR_COMM_FEATURE          0x04
#define CDC_SET_AUX_LINE_STATE          0x10
#define CDC_SET_HOOK_STATE              0x11
#define CDC_PULSE_SETUP                 0x12
#define CDC_SEND_PULSE                  0x13
#define CDC_SET_PULSE_TIME              0x14
#define CDC_RING_AUX_JACK               0x15
#define CDC_SET_LINE_CODING             0x20
#define CDC_GET_LINE_CODING             0x21
#define CDC_SET_CONTROL_LINE_STATE      0x22
#define CDC_SEND_BREAK                  0x23
#define CDC_SET_RINGER_PARMS            0x30
#define CDC_GET_RINGER_PARMS            0x31
#define CDC_SET_OPERATION_PARMS         0x32
#define CDC_GET_OPERATION_PARMS         0x33
/** @} */

/**
 * @name    Line Control bit definitions.
 * @{
 */
#define LC_STOP_1                       0
#define LC_STOP_1P5                     1
#define LC_STOP_2                       2

#define LC_PARITY_NONE                  0
#define LC_PARITY_ODD                   1
#define LC_PARITY_EVEN                  2
#define LC_PARITY_MARK                  3
#define LC_PARITY_SPACE                 4
/** @} */

/*===========================================================================*/
/* Driver pre-compile time settings.                                         */
/*===========================================================================*/

/**
 * @name    USB_CDC configuration options
 * @{
 */
/**
 * @brief   Endpoint number for bulk IN.
 */
#if !defined(USB_CDC_DATA_REQUEST_EP) || defined(__DOXYGEN__)
#define USB_CDC_DATA_REQUEST_EP         1
#endif

/**
 * @brief   Endpoint number for interrupt IN.
 */
#if !defined(USB_CDC_INTERRUPT_REQUEST_EP) || defined(__DOXYGEN__)
#define USB_CDC_INTERRUPT_REQUEST_EP    2
#endif

/**
 * @brief   Endpoint number for bulk OUT.
 */
#if !defined(USB_CDC_DATA_AVAILABLE_EP) || defined(__DOXYGEN__)
#define USB_CDC_DATA_AVAILABLE_EP       3
#endif
/** @} */

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

/*===========================================================================*/
/* Driver data structures and types.                                         */
/*===========================================================================*/

/**
 * @brief   Type of Line Coding structure.
 */
typedef struct {
  uint8_t                       dwDTERate[4];
  uint8_t                       bCharFormat;
  uint8_t                       bParityType;
  uint8_t                       bDataBits;
} cdc_linecoding_t;

/*===========================================================================*/
/* Driver macros.                                                            */
/*===========================================================================*/

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#endif /* _USB_CDC_H_ */

/** @} */
