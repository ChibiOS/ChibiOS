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
 * @file    sb/host/sb.h
 * @brief   ARM sandbox macros and structures.
 *
 * @addtogroup ARM_SANDBOX
 * @{
 */

#ifndef SB_H
#define SB_H

#include "sberr.h"
#include "sbhost.h"
#include "sbapi.h"
#include "sbposix.h"

/*===========================================================================*/
/* Module constants.                                                         */
/*===========================================================================*/

/**
 * @brief   ChibiOS/SB identification macro.
 */
#define _CHIBIOS_SB_

/**
 * @brief   Stable release flag.
 */
#define CH_SB_STABLE            0

/**
 * @name    ChibiOS/SE version identification
 * @{
 */
/**
 * @brief   Safety Extensions version string.
 */
#define CH_SB_VERSION           "1.0.0"

/**
 * @brief   Safety Extensions version major number.
 */
#define CH_SB_MAJOR             1

/**
 * @brief   Safety Extensions version minor number.
 */
#define CH_SB_MINOR             0

/**
 * @brief   Safety Extensions version patch number.
 */
#define CH_SB_PATCH             0
/** @} */

/*===========================================================================*/
/* Module pre-compile time settings.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

/* License checks.*/
#if !defined(CH_CUSTOMER_LIC_SB) || !defined(CH_LICENSE_FEATURES)
#error "malformed chlicense.h"
#endif

#if CH_CUSTOMER_LIC_SB == FALSE
#error "ChibiOS/SB not licensed"
#endif

#if CH_CFG_ST_RESOLUTION != 32
#error "SandBox requires CH_CFG_ST_RESOLUTION == 32"
#endif

#if CH_CFG_INTERVALS_SIZE != 32
#error "SandBox requires CH_CFG_INTERVALS_SIZE == 32"
#endif

#if PORT_USE_SYSCALL == FALSE
#error "SandBox requires PORT_USE_SYSCALL == TRUE"
#endif

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

#endif /* SBHOST_H */

/** @} */
