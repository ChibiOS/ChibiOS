[#ftl]
[#--
    ChibiOS - Copyright (C) 2006-2026 Giovanni Di Sirio.

    This file is part of ChibiOS.

    ChibiOS is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation version 3 of the License.

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
[@pp.changeOutputFile name="vioconf.h" /]
/*
[@license.EmitLicenseAsText /]
*/

/**
 * @file    templates/vioconf.h
 * @brief   VIO configuration header.
 * @details VIO configuration file, this file allows to enable or disable the
 *          various virtual peripherals from your application.
 *
 * @addtogroup VIO_CONF
 * @{
 */

#ifndef VIOCONF_H
#define VIOCONF_H

#define __CHIBIOS_VIO_CONF__
#define __CHIBIOS_VIO_CONF_VER_1_0__

/**
 * @brief   Enables the Virtual ADC support.
 */
#if !defined(VIO_CFG_ENABLE_ADC) || defined(__DOXYGEN__)
#define VIO_CFG_ENABLE_ADC                  ${doc.VIO_CFG_ENABLE_ADC!"FALSE"}
#endif

/**
 * @brief   Enables the Virtual ETH support.
 */
#if !defined(VIO_CFG_ENABLE_ETH) || defined(__DOXYGEN__)
#define VIO_CFG_ENABLE_ETH                  ${doc.VIO_CFG_ENABLE_ETH!"FALSE"}
#endif

/**
 * @brief   Enables the Virtual GPT support.
 */
#if !defined(VIO_CFG_ENABLE_GPT) || defined(__DOXYGEN__)
#define VIO_CFG_ENABLE_GPT                  ${doc.VIO_CFG_ENABLE_GPT!"FALSE"}
#endif

/**
 * @brief   Enables the Virtual I2C support.
 */
#if !defined(VIO_CFG_ENABLE_I2C) || defined(__DOXYGEN__)
#define VIO_CFG_ENABLE_I2C                  ${doc.VIO_CFG_ENABLE_I2C!"FALSE"}
#endif

/**
 * @brief   Enables the Virtual GPIO support.
 */
#if !defined(VIO_CFG_ENABLE_GPIO) || defined(__DOXYGEN__)
#define VIO_CFG_ENABLE_GPIO                 ${doc.VIO_CFG_ENABLE_GPIO!"TRUE"}
#endif

/**
 * @brief   Enables the Virtual SPI support.
 */
#if !defined(VIO_CFG_ENABLE_SPI) || defined(__DOXYGEN__)
#define VIO_CFG_ENABLE_SPI                  ${doc.VIO_CFG_ENABLE_SPI!"FALSE"}
#endif

/**
 * @brief   Enables the Virtual UART support.
 */
#if !defined(VIO_CFG_ENABLE_UART) || defined(__DOXYGEN__)
#define VIO_CFG_ENABLE_UART                 ${doc.VIO_CFG_ENABLE_UART!"TRUE"}
#endif

#endif /* VIOCONF_H */

/** @} */
