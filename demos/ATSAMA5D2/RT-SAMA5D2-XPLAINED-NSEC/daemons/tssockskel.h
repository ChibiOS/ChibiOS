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
 * @file    tssockskel.h
 * @brief   Sockets skeleton module macros and structures.
 *
 */

#ifndef TSSOCKSKEL_H
#define TSSOCKSKEL_H

#include "ch.h"
#include "ccportab.h"
#include "lwip/sockets.h"
#include "tscommon.h"

/*===========================================================================*/
/* Module constants.                                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Module pre-compile time settings.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/
#if (L_FD_SETSIZE) != (FD_SETSIZE)
#error "Configuration error of L_FD_SETSIZE, it must be set to FD_SETSIZE "
#define VALUE(x) #x
#define VAR_NAME_VALUE(var) #var "="  VALUE(var)
#pragma message(VAR_NAME_VALUE(FD_SETSIZE))
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
  THD_FUNCTION(TsSockSkelDaemon, tsstate);
  extern THD_WORKING_AREA(waTsSockSkelDaemon0, 2048);
  extern THD_WORKING_AREA(waTsSockSkelDaemon1, 2048);
  extern THD_WORKING_AREA(waTsSockSkelDaemon2, 2048);
#ifdef __cplusplus
}
#endif

/*===========================================================================*/
/* Module inline functions.                                                  */
/*===========================================================================*/

#endif /* TSSOCKSKEL_H */
