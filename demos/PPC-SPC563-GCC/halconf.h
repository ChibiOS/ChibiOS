/*
    ChibiOS/RT - Copyright (C) 2006,2007,2008,2009,2010 Giovanni Di Sirio.

    This file is part of ChibiOS/RT.

    ChibiOS/RT is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 3 of the License, or
    (at your option) any later version.

    ChibiOS/RT is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program. If not, see <http://www.gnu.org/licenses/>.

                                      ---

    A special exception to the GPL can be applied should you wish to distribute
    a combined work that includes ChibiOS/RT, without being obliged to provide
    the source code for any proprietary components. See the file exception.txt
    for full details of how and when the exception can be applied.
*/

/**
 * @file templates/halconf.h
 * @brief HAL configuration header.
 * @addtogroup HAL_CONF
 * @{
 */

/*
 * HAL configuration file, this file allows to enable or disable the various
 * device drivers from your application. You may also use this file in order
 * to override the device drivers default settings.
 */

#ifndef _HALCONF_H_
#define _HALCONF_H_

/*
 * Uncomment the following line in order to include a mcu-related
 * settings file. This file can be used to include platform specific
 * header files or to override the low level drivers settings.
 */
/*#include "mcuconf.h"*/

/*===========================================================================*/
/* PAL driver related settings.                                              */
/*===========================================================================*/

/**
 * @brief Enables the PAL subsystem.
 */
#if !defined(CH_HAL_USE_PAL) || defined(__DOXYGEN__)
#define CH_HAL_USE_PAL              FALSE
#endif

/*===========================================================================*/
/* ADC driver related settings.                                              */
/*===========================================================================*/

/**
 * @brief Enables the ADC subsystem.
 */
#if !defined(CH_HAL_USE_ADC) || defined(__DOXYGEN__)
#define CH_HAL_USE_ADC              FALSE
#endif

/*===========================================================================*/
/* CAN driver related settings.                                              */
/*===========================================================================*/

/**
 * @brief Enables the CAN subsystem.
 */
#if !defined(CH_HAL_USE_CAN) || defined(__DOXYGEN__)
#define CH_HAL_USE_CAN              FALSE
#endif

/*===========================================================================*/
/* MAC driver related settings.                                              */
/*===========================================================================*/

/**
 * @brief Enables the MAC subsystem.
 */
#if !defined(CH_HAL_USE_MAC) || defined(__DOXYGEN__)
#define CH_HAL_USE_MAC              FALSE
#endif

/*===========================================================================*/
/* PWM driver related settings.                                              */
/*===========================================================================*/

/**
 * @brief Enables the PWM subsystem.
 */
#if !defined(CH_HAL_USE_PWM) || defined(__DOXYGEN__)
#define CH_HAL_USE_PWM              FALSE
#endif

/*===========================================================================*/
/* SERIAL driver related settings.                                           */
/*===========================================================================*/

/**
 * @brief Enables the SERIAL subsystem.
 */
#if !defined(CH_HAL_USE_SERIAL) || defined(__DOXYGEN__)
#define CH_HAL_USE_SERIAL           TRUE
#endif

/*
 * Default SERIAL settings overrides (uncomment to override).
 */
/*#define SERIAL_DEFAULT_BITRATE      38400*/
/*#define SERIAL_BUFFERS_SIZE         64*/

/*===========================================================================*/
/* SPI driver related settings.                                              */
/*===========================================================================*/

/**
 * @brief Enables the SPI subsystem.
 */
#if !defined(CH_HAL_USE_SPI) || defined(__DOXYGEN__)
#define CH_HAL_USE_SPI              FALSE
#endif

/*
 * Default SPI settings overrides (uncomment to override).
 */
/*#define SPI_USE_MUTUAL_EXCLUSION    TRUE*/

/*===========================================================================*/
/* MMC_SPI driver related settings.                                          */
/*===========================================================================*/

/**
 * @brief Enables the MMC_SPI subsystem.
 */
#if !defined(CH_HAL_USE_MMC_SPI) || defined(__DOXYGEN__)
#define CH_HAL_USE_MMC_SPI          FALSE
#endif

/*
 * Default MMC_SPI settings overrides (uncomment to override).
 */
/*#define MMC_SECTOR_SIZE             512*/
/*#define MMC_NICE_WAITING            TRUE*/
/*#define MMC_POLLING_INTERVAL        10*/
/*#define MMC_POLLING_DELAY           10*/

#endif /* _HALCONF_H_ */

/** @} */
