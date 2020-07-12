/*
    ChibiOS - Copyright (C) 2006..2018 Giovanni Di Sirio.

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
 * @file    rt/include/chport.h
 * @brief   Port wrapper header.
 *
 * @addtogroup port_wrapper
 * @details This module performs checks on the information exported by
 *          the port layer.
 * @{
 */

#ifndef CHPORT_H
#define CHPORT_H

/* Inclusion of the port layer.*/
#include "chcore.h"

/*===========================================================================*/
/* Module constants.                                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Module pre-compile time settings.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

/* Just in case the port layer does not export the following definitions.*/
#if !defined(PORT_CORES_NUMBER)
#define PORT_CORES_NUMBER                   1
#endif

#if (PORT_CORES_NUMBER < 1) || (PORT_CORES_NUMBER > 64)
#error "invalid PORT_CORES_NUMBER value"
#endif

#if PORT_CORES_NUMBER == 1
//#if CH_CFG_SMP_MODE != FALSE
//#error "this port does not support SMP"
//#endif
#endif

#if !defined(PORT_INSTANCE_ACCESS)
#define PORT_INSTANCE_ACCESS                (&ch)
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

/*===========================================================================*/
/* Module inline functions.                                                  */
/*===========================================================================*/

#endif /* CHPORT_H */

/** @} */
