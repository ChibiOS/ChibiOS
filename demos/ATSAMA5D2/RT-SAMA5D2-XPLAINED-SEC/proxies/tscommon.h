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
 * @file    tscommon.h
 * @brief   Common, shared defines and macros between secure and non secure
 *          environment.
 *
 */

#ifndef TSCOMMON_H
#define TSCOMMON_H

#include "ch.h"
#include "ccportab.h"

/*===========================================================================*/
/* Module constants.                                                         */
/*===========================================================================*/
#define SKEL_REQ_GETOP    1
#define SKEL_REQ_CPYPRMS  2
#define SKEL_REQ_PUTRES   3
#define SKEL_REQ_READY    4

#define STUB_OP_SOCKET    0
#define STUB_OP_CLOSE     1
#define STUB_OP_CONNECT   2
#define STUB_OP_RECV      3
#define STUB_OP_SEND      4
#define STUB_OP_SELECT    5
#define STUB_OP_BIND      6

#define EVT_F_SOCK_NEW_OP 1

/*===========================================================================*/
/* Module pre-compile time settings.                                         */
/*===========================================================================*/
#define METHOD_MAX_PARAMS   6

#define L_FD_SETSIZE  64

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

/*===========================================================================*/
/* Module data structures and types.                                         */
/*===========================================================================*/
typedef struct skel_req {
  uint32_t  req;        /* getop, cpyprms, putres */
  uint32_t  stub_op;
  uint32_t  stub_op_code;
  uint32_t  stub_op_result;
  uint32_t  stub_op_p_sz[METHOD_MAX_PARAMS];
  uint32_t  stub_op_p[METHOD_MAX_PARAMS];
} skel_req_t;

/*===========================================================================*/
/* Module macros.                                                            */
/*===========================================================================*/

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

/*===========================================================================*/
/* Module inline functions.                                                  */
/*===========================================================================*/

#endif /* TSCOMMON_H */
