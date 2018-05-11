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
 * @file    tsdaemonskels.h
 * @brief   Common skeletons daemon macros and structures.
 *
 */

#ifndef TSDAEMONSKELS_H
#define TSDAEMONSKELS_H

#include "ch.h"
#include "ccportab.h"
#include "tscommon.h"

/*===========================================================================*/
/* Module constants.                                                         */
/*===========================================================================*/
#define N_MAX_SKEL_REQS   4

/*===========================================================================*/
/* Module pre-compile time settings.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

/*===========================================================================*/
/* Module data structures and types.                                         */
/*===========================================================================*/

typedef struct skel_ctx {
  objects_fifo_t  skel_req_fifo;
  msg_t           skel_req_msgs[N_MAX_SKEL_REQS];
  skel_req_t      skel_reqs[N_MAX_SKEL_REQS];
  eventflags_t    skel_eventflag;
  ts_service_t    stub_svc;
  mutex_t         stub_svc_mtx;
  const char     *stub_svc_name;
} skel_ctx_t;

/*===========================================================================*/
/* Module macros.                                                            */
/*===========================================================================*/

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#ifdef __cplusplus
extern "C" {
#endif
  void paramsInFromRemote(skel_req_t *skreqp);
  void returnToRemote(skel_req_t *skreqp, uint32_t res);
  THD_FUNCTION(TsSkelsDaemon, arg);
#ifdef __cplusplus
}
#endif

/*===========================================================================*/
/* Module inline functions.                                                  */
/*===========================================================================*/

#endif /* TSDAEMONSKELS_H */
