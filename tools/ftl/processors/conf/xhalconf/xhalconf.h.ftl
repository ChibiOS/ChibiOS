[#ftl]
[#--
    Copyright (C) 2006-2026 Giovanni Di Sirio.

    This file is part of ChibiOS.

    ChibiOS is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 3 of the License, or
    (at your option) any later version.

    ChibiOS is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
  --]
[@pp.dropOutputFile /]
[#import "/@lib/libutils.ftl" as utils /]
[#import "/@lib/liblicense.ftl" as license /]
[@pp.changeOutputFile name="xhalconf.h" /]
/*
[@license.EmitLicenseAsText /]
*/

/**
 * @file    templates/xhalconf.h
 * @brief   XHAL configuration header.
 * @details XHAL configuration file, this file allows to enable or disable the
 *          various device drivers from your application. You may also use
 *          this file in order to override the device drivers default settings.
 *
 * @addtogroup XHAL_CONF
 * @{
 */

#ifndef XHALCONF_H
#define XHALCONF_H

#define __CHIBIOS_XHAL_CONF__
#define __CHIBIOS_XHAL_CONF_VER_1_0__

#include "xmcuconf.h"

/*===========================================================================*/
/* HAL general settings.                                                     */
/*===========================================================================*/

#define HAL_USE_PAL                         ${doc.HAL_USE_PAL!"TRUE"}
#define HAL_USE_MMC_SPI                     ${doc.HAL_USE_MMC_SPI!"FALSE"}
#define HAL_USE_SIO                         ${doc.HAL_USE_SIO!"FALSE"}
#define HAL_USE_SPI                         ${doc.HAL_USE_SPI!"FALSE"}

/*===========================================================================*/
/* PAL driver settings.                                                      */
/*===========================================================================*/

#define PAL_USE_CALLBACKS                   ${doc.PAL_USE_CALLBACKS!"TRUE"}
#define PAL_USE_WAIT                        ${doc.PAL_USE_WAIT!"TRUE"}

/*===========================================================================*/
/* SIO driver settings.                                                      */
/*===========================================================================*/

#define SIO_DEFAULT_BITRATE                 ${doc.SIO_DEFAULT_BITRATE!"38400"}
#define SIO_USE_SYNCHRONIZATION             ${doc.SIO_USE_SYNCHRONIZATION!"TRUE"}
#define SIO_USE_STREAMS_INTERFACE           ${doc.SIO_USE_STREAMS_INTERFACE!"SIO_USE_SYNCHRONIZATION"}
#define SIO_USE_BUFFERING                   ${doc.SIO_USE_BUFFERING!"FALSE"}
#define SIO_USE_CONFIGURATIONS              ${doc.SIO_USE_CONFIGURATIONS!"FALSE"}

/*===========================================================================*/
/* SPI driver settings.                                                      */
/*===========================================================================*/

#define SPI_USE_SYNCHRONIZATION             ${doc.SPI_USE_SYNCHRONIZATION!"TRUE"}
#define SPI_USE_ASSERT_ON_ERROR             ${doc.SPI_USE_ASSERT_ON_ERROR!"FALSE"}
#define SPI_USE_CONFIGURATIONS              ${doc.SPI_USE_CONFIGURATIONS!"FALSE"}

#endif /* XHALCONF_H */

/** @} */
