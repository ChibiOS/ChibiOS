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

/* Sockets stub defines.*/
#define SOCK_OP_SOCKET    0
#define SOCK_OP_CLOSE     1
#define SOCK_OP_CONNECT   2
#define SOCK_OP_RECV      3
#define SOCK_OP_SEND      4
#define SOCK_OP_SELECT    5
#define SOCK_OP_BIND      6
#define SOCK_OP_LISTEN    7

/* Socket new op event.*/
#define EVT_F_SOCK_NEW_OP   1

/* Sockets stub service name.*/
#define SOCKS_SVC_NAME    "TsSocksStubService"

/* IOBlocks stub defines.*/
#define IOBLKS_OP_OPEN    0
#define IOBLKS_OP_CLOSE   1
#define IOBLKS_OP_READ    2
#define IOBLKS_OP_WRITE   3
#define IOBLKS_OP_FLUSH   4

/* IOBlock new op event.*/
#define EVT_F_IOBLK_NEW_OP  2

/* IOBlock stub service name.*/
#define IOBLKS_SVC_NAME   "TsIOBlksStubService"

/* Sector size.*/
#define IOBLKS_SECT_SIZE  512U

/* Remote Partition size, in sectors.*/
#define IOBLKS_PART_SIZE  96256U

/* Remote partition offset, in sectors.*/
#define IOBLKS_PART_OFFS  952320U

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
typedef struct skel_ctx skel_ctx_t;

typedef struct skel_req {
  uint32_t    req;        /* getop, cpyprms, putres */
  uint32_t    stub_op;
  uint32_t    stub_op_code;
  uint32_t    stub_op_result;
  uint32_t    stub_op_p_sz[METHOD_MAX_PARAMS];
  uint32_t    stub_op_p[METHOD_MAX_PARAMS];
  skel_ctx_t *scp;  /* the skeleton context this req come from.*/
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
