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
 * @file    sbvio.h
 * @brief   ARM SandBox host Virtual I/O macros and structures.
 *
 * @addtogroup ARM_SANDBOX_HOST_VIO
 * @{
 */

#ifndef SBVIO_H
#define SBVIO_H

#if (SB_CFG_ENABLE_VIO == TRUE) || defined(__DOXYGEN__)

#include "vioconf.h"
#include "sbvio_gpio.h"

/*===========================================================================*/
/* Module constants.                                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Module pre-compile time settings.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

/*===========================================================================*/
/* Module data structures and types.                                         */
/*===========================================================================*/

/**
 * @brief   Type of a VIO instance configuration structure.
 */
typedef struct vio_conf {
#if (VIO_CFG_ENABLE_GPIO == TRUE) || defined(__DOXYGEN__)
  /**
   * @brief   VPAL configuration.
   */
  const vio_gpio_conf_t         *gpioconf;
#endif
} vio_conf_t;

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

#endif /* SB_CFG_ENABLE_VIO == TRUE */

#endif /* SBVIO_H */

/** @} */
