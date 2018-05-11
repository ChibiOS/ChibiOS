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
 * @file    tsproxystubs.h
 * @brief   Proxy stubs module macros and structures.
 *
 */

#ifndef TSPROXYSTUBS_H
#define TSPROXYSTUBS_H

#include "ch.h"
#include "ccportab.h"
#include "tscommon.h"

/*===========================================================================*/
/* Module constants.                                                         */
/*===========================================================================*/
#define METHOD_MAX_PARAMS   6
#define STUB_MAX_OPS        32

#define OP_PRMDIR_NONE      0
#define OP_PRMDIR_IN        1
#define OP_PRMDIR_OUT       2

/*===========================================================================*/
/* Module pre-compile time settings.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

/*===========================================================================*/
/* Module data structures and types.                                         */
/*===========================================================================*/

typedef enum {FREE=0, CALLING, PENDING} op_state_t;
typedef struct  stub_ctx stub_ctx_t;

typedef struct stub_param {
  uint32_t  dir;
  uint32_t  val;
  uint32_t  size;
} stub_parm_t;

typedef struct stub_op {
  uint32_t            op_code;  /* the stub method op code.*/
  op_state_t          op_state; /* calling, pending, free.*/
  stub_parm_t         op_p[METHOD_MAX_PARAMS];
  thread_reference_t  op_wthdp; /* TS internal client thread (the caller).*/
  stub_ctx_t         *scp;      /* the stub ctx this stub_op relates to.*/
} stub_op_t;

typedef struct stub_ctx {
  eventflags_t    event_flag;
  objects_fifo_t  ops_fifo;
  msg_t           ops_msgs[STUB_MAX_OPS];
  stub_op_t       ops[STUB_MAX_OPS];
} stub_ctx_t;

/*===========================================================================*/
/* Module macros.                                                            */
/*===========================================================================*/

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#ifdef __cplusplus
extern "C" {
#endif
  void tsWaitStubSkelReady(eventflags_t mask);
  void TsStubService(ts_state_t *svcp, stub_ctx_t *scp);
  uint32_t callRemote(stub_op_t *op);
  stub_op_t *getNewOp(stub_ctx_t *scp);
#ifdef __cplusplus
}
#endif

/*===========================================================================*/
/* Module inline functions.                                                  */
/*===========================================================================*/

#endif /* TSPROXYSTUBS_H */
