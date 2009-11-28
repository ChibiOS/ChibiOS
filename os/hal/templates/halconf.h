/*
    ChibiOS/RT - Copyright (C) 2006-2007 Giovanni Di Sirio.

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
 * @file templates/halconf.h
 * @brief HAL configuration header.
 * @addtogroup HAL
 * @{
 */

#ifndef _HALCONF_H_
#define _HALCONF_H_

/**
 * @brief Enables the PAL subsystem.
 */
#if !defined(CH_HAL_USE_PAL) || defined(__DOXYGEN__)
#define CH_HAL_USE_PAL                  TRUE
#endif

/**
 * @brief Enables the ADC subsystem.
 */
#if !defined(CH_HAL_USE_ADC) || defined(__DOXYGEN__)
#define CH_HAL_USE_ADC                  TRUE
#endif

/**
 * @brief Enables the CAN subsystem.
 */
#if !defined(CH_HAL_USE_CAN) || defined(__DOXYGEN__)
#define CH_HAL_USE_CAN                  TRUE
#endif

/**
 * @brief Enables the MAC subsystem.
 */
#if !defined(CH_HAL_USE_MAC) || defined(__DOXYGEN__)
#define CH_HAL_USE_MAC                  TRUE
#endif

/**
 * @brief Enables the MII subsystem.
 */
#if !defined(CH_HAL_USE_MII) || defined(__DOXYGEN__)
#define CH_HAL_USE_MII                  TRUE
#endif

/**
 * @brief Enables the SERIAL subsystem.
 */
#if !defined(CH_HAL_USE_SERIAL) || defined(__DOXYGEN__)
#define CH_HAL_USE_SERIAL               TRUE
#endif

/**
 * @brief Enables the SPI subsystem.
 */
#if !defined(CH_HAL_USE_SPI) || defined(__DOXYGEN__)
#define CH_HAL_USE_SPI                  TRUE
#endif

/**
 * @brief Enables the MMC_SPI subsystem.
 */
#if !defined(CH_HAL_USE_MMC_SPI) || defined(__DOXYGEN__)
#define CH_HAL_USE_MMC_SPI              TRUE
#endif

#endif /* _HALCONF_H_ */

/** @} */
