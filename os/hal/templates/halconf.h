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
 * @file    templates/halconf.h
 * @brief   HAL configuration header.
 * @details HAL configuration file, this file allows to enable or disable the
 *          various device drivers from your application. You may also use
 *          this file in order to override the device drivers default settings.
 *
 * @addtogroup HAL_CONF
 * @{
 */

/*
 *
 */

#ifndef _HALCONF_H_
#define _HALCONF_H_

#include "mcuconf.h"

/*===========================================================================*/
/* PAL driver related settings.                                              */
/*===========================================================================*/

/**
 * @brief   Enables the PAL subsystem.
 */
#if !defined(CH_HAL_USE_PAL) || defined(__DOXYGEN__)
#define CH_HAL_USE_PAL              TRUE
#endif

/*===========================================================================*/
/* ADC driver related settings.                                              */
/*===========================================================================*/

/**
 * @brief   Enables the ADC subsystem.
 */
#if !defined(CH_HAL_USE_ADC) || defined(__DOXYGEN__)
#define CH_HAL_USE_ADC              TRUE
#endif

/**
 * @brief   Inclusion of the @p adcWaitConversion() function.
 */
#if !defined(ADC_USE_WAIT) || defined(__DOXYGEN__)
#define ADC_USE_WAIT                TRUE
#endif

/*===========================================================================*/
/* CAN driver related settings.                                              */
/*===========================================================================*/

/**
 * @brief   Enables the CAN subsystem.
 */
#if !defined(CH_HAL_USE_CAN) || defined(__DOXYGEN__)
#define CH_HAL_USE_CAN              TRUE
#endif

/**
 * @brief   Sleep mode related APIs inclusion switch.
 */
#if !defined(CAN_USE_SLEEP_MODE) || defined(__DOXYGEN__)
#define CAN_USE_SLEEP_MODE          TRUE
#endif

/*===========================================================================*/
/* MAC driver related settings.                                              */
/*===========================================================================*/

/**
 * @brief   Enables the MAC subsystem.
 */
#if !defined(CH_HAL_USE_MAC) || defined(__DOXYGEN__)
#define CH_HAL_USE_MAC              TRUE
#endif

/*===========================================================================*/
/* PWM driver related settings.                                              */
/*===========================================================================*/

/**
 * @brief   Enables the PWM subsystem.
 */
#if !defined(CH_HAL_USE_PWM) || defined(__DOXYGEN__)
#define CH_HAL_USE_PWM              TRUE
#endif

/*===========================================================================*/
/* SERIAL driver related settings.                                           */
/*===========================================================================*/

/**
 * @brief   Enables the SERIAL subsystem.
 */
#if !defined(CH_HAL_USE_SERIAL) || defined(__DOXYGEN__)
#define CH_HAL_USE_SERIAL           TRUE
#endif

/**
 * @brief   Default bit rate.
 * @details Configuration parameter, this is the baud rate selected for the
 *          default configuration.
 */
#if !defined(SERIAL_DEFAULT_BITRATE) || defined(__DOXYGEN__)
#define SERIAL_DEFAULT_BITRATE      38400
#endif

/**
 * @brief   Serial buffers size.
 * @details Configuration parameter, you can change the depth of the queue
 *          buffers depending on the requirements of your application.
 * @note    The default is 64 bytes for both the transmission and receive
 *          buffers.
 */
#if !defined(SERIAL_BUFFERS_SIZE) || defined(__DOXYGEN__)
#define SERIAL_BUFFERS_SIZE         16
#endif

/*===========================================================================*/
/* SPI driver related settings.                                              */
/*===========================================================================*/

/**
 * @brief   Enables the SPI subsystem.
 */
#if !defined(CH_HAL_USE_SPI) || defined(__DOXYGEN__)
#define CH_HAL_USE_SPI              TRUE
#endif

/**
 * @brief   Enables the "wait" APIs.
 * @note    Disabling this option saves both code and data space.
 */
#if !defined(SPI_USE_WAIT) || defined(__DOXYGEN__)
#define SPI_USE_WAIT                TRUE
#endif

/**
 * @brief   Enables the @p spiAcquireBus() and @p spiReleaseBus() APIs.
 * @note    Disabling this option saves both code and data space.
 */
#if !defined(SPI_USE_MUTUAL_EXCLUSION) || defined(__DOXYGEN__)
#define SPI_USE_MUTUAL_EXCLUSION    TRUE
#endif

/*===========================================================================*/
/* MMC_SPI driver related settings.                                          */
/*===========================================================================*/

/**
 * @brief   Enables the MMC_SPI subsystem.
 */
#if !defined(CH_HAL_USE_MMC_SPI) || defined(__DOXYGEN__)
#define CH_HAL_USE_MMC_SPI          TRUE
#endif

/**
 * @brief   Block size for MMC transfers.
 */
#if !defined(MMC_SECTOR_SIZE) || defined(__DOXYGEN__)
#define MMC_SECTOR_SIZE             512
#endif

/**
 * @brief   Delays insertions.
 * @details If enabled this options inserts delays into the MMC waiting
 *          routines releasing some extra CPU time for the threads with
 *          lower priority, this may slow down the driver a bit however.
 *          This option is recommended also if the SPI driver does not
 *          use a DMA channel and heavily loads the CPU.
 */
#if !defined(MMC_NICE_WAITING) || defined(__DOXYGEN__)
#define MMC_NICE_WAITING            TRUE
#endif

/**
 * @brief   Number of positive insertion queries before generating the
 *          insertion event.
 */
#if !defined(MMC_POLLING_INTERVAL) || defined(__DOXYGEN__)
#define MMC_POLLING_INTERVAL        10
#endif

/**
 * @brief   Interval, in milliseconds, between insertion queries.
 */
#if !defined(MMC_POLLING_DELAY) || defined(__DOXYGEN__)
#define MMC_POLLING_DELAY           10
#endif

/*===========================================================================*/
/* UART driver related settings.                                             */
/*===========================================================================*/

/**
 * @brief   Enables the UART subsystem.
 */
#if !defined(CH_HAL_USE_UART) || defined(__DOXYGEN__)
#define CH_HAL_USE_UART             TRUE
#endif

#endif /* _HALCONF_H_ */

/** @} */
