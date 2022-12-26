/*
    ChibiOS - Copyright (C) 2006,2007,2008,2009,2010,2011,2012,2013,2014,
              2015,2016,2017,2018,2019,2020,2021 Giovanni Di Sirio.

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
*/

/**
 * @file    chversion.h
 * @brief   Version Module macros and structures.
 *
 * @addtogroup chibios_version
 * @details This module contains information about the ChibiOS release, it
 *          is common to all subsystems.
 * @{
 */

#ifndef CHVERSION_H
#define CHVERSION_H

/*===========================================================================*/
/* Module constants.                                                         */
/*===========================================================================*/

/**
 * @brief   ChibiOS product identification macro.
 */
#define __CHIBIOS__

/**
 * @brief   Stable release flag.
 */
#define CH_VERSION_STABLE       1

/**
 * @name    ChibiOS version identification
 * @{
 */
/**
 * @brief   ChibiOS version string.
 */
#define CH_VERSION              "2021.11.3"

/**
 * @brief   ChibiOS version release year.
 */
#define CH_VERSION_YEAR         21

/**
 * @brief   ChibiOS version release month.
 */
#define CH_VERSION_MONTH        11

/**
 * @brief   ChibiOS version patch number.
 */
#define CH_VERSION_PATCH        3

/**
 * @brief   ChibiOS version nickname.
 */
#define CH_VERSION_NICKNAME     "Montepertuso"
/** @} */

/*===========================================================================*/
/* Module pre-compile time settings.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

/**
 * @brief   Current version date in numeric form (yyyymm).
 */
#define CH_VERSION_DATE                                                     \
  (((CH_VERSION_YEAR + 2000) * 100) + CH_VERSION_MONTH)

/*===========================================================================*/
/* Module data structures and types.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Module macros.                                                            */
/*===========================================================================*/

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

/*===========================================================================*/
/* Module inline functions.                                                  */
/*===========================================================================*/

#endif /* CHVERSION_H */

/** @} */
