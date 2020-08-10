/*
    ChibiOS - Copyright (C) 2006..2019 Giovanni Di Sirio.

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
*/

/**
 * @file    seregs.h
 * @brief   Safety Extensions macros and structures.
 *
 * @addtogroup SE
 * @{
 */

#ifndef SE_H
#define SE_H

/*===========================================================================*/
/* Module constants.                                                         */
/*===========================================================================*/

/**
 * @brief   ChibiOS/SE identification macro.
 */
#define __CHIBIOS_SE__

/**
 * @brief   Stable release flag.
 */
#define CH_SE_STABLE            0

/**
 * @name    ChibiOS/SE version identification
 * @{
 */
/**
 * @brief   Safety Extensions version string.
 */
#define CH_SE_VERSION           "1.0.0"

/**
 * @brief   Safety Extensions version major number.
 */
#define CH_SE_MAJOR             1

/**
 * @brief   Safety Extensions version minor number.
 */
#define CH_SE_MINOR             0

/**
 * @brief   Safety Extensions version patch number.
 */
#define CH_SE_PATCH             0
/** @} */

/*===========================================================================*/
/* Module pre-compile time settings.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

/*===========================================================================*/
/* Module data structures and types.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Module macros.                                                            */
/*===========================================================================*/

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __cplusplus
}
#endif

/*===========================================================================*/
/* Module inline functions.                                                  */
/*===========================================================================*/

/*===========================================================================*/
/* Late inclusions.                                                          */
/*===========================================================================*/

/* Safety Extensions headers.*/
#include "seregs.h"
#include "seloops.h"

#endif /* SE_H */

/** @} */
