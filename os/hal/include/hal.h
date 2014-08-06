/*
    ChibiOS/HAL - Copyright (C) 2006,2007,2008,2009,2010,
                  2011,2012,2013,2014 Giovanni Di Sirio.

    This file is part of ChibiOS/HAL 

    ChibiOS/HAL is free software; you can redistribute it and/or modify
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
 * @file    hal.h
 * @brief   HAL subsystem header.
 *
 * @addtogroup HAL
 * @{
 */

#ifndef _HAL_H_
#define _HAL_H_

#include "osal.h"
#include "board.h"
#include "halconf.h"

#include "hal_lld.h"

/* Abstract interfaces.*/
#include "hal_streams.h"
#include "hal_channels.h"
#include "hal_ioblock.h"
#include "hal_mmcsd.h"

/* Shared headers.*/
#include "hal_queues.h"

/* Normal drivers.*/
#include "pal.h"
#include "adc.h"
#include "can.h"
#include "dac.h"
#include "ext.h"
#include "gpt.h"
#include "i2c.h"
#include "i2s.h"
#include "icu.h"
#include "mac.h"
#include "mii.h"
#include "pwm.h"
#include "rtc.h"
#include "serial.h"
#include "sdc.h"
#include "spi.h"
#include "st.h"
#include "uart.h"
#include "usb.h"
#include "nand.h"

/* Complex drivers.*/
#include "mmc_spi.h"
#include "serial_usb.h"

/*===========================================================================*/
/* Driver constants.                                                         */
/*===========================================================================*/

/**
 * @name    Return codes
 * @{
 */
#define HAL_SUCCESS                         false
#define HAL_FAILED                          true
/** @} */

/*===========================================================================*/
/* Driver pre-compile time settings.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

/*===========================================================================*/
/* Driver data structures and types.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Driver macros.                                                            */
/*===========================================================================*/

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#ifdef __cplusplus
extern "C" {
#endif
  void halInit(void);
#ifdef __cplusplus
}
#endif

#endif /* _HAL_H_ */

/** @} */
