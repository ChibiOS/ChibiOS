/*
    ChibiOS - Copyright (C) 2006..2018 Giovanni Di Sirio

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

        http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.
*/

/**
 * @file    tsdaemonskel.c
 * @brief   Common skeletons daemon for trusted clients.
 *
 */

#include "ch.h"
#include "chobjfifos.h"
#include "tsclient.h"
#include "tsdaemonskels.h"
#include <string.h>

/*===========================================================================*/
/* Module local definitions.                                                 */
/*===========================================================================*/

/*===========================================================================*/
/* Module exported variables.                                                */
/*===========================================================================*/

/*===========================================================================*/
/* Module local types.                                                       */
/*===========================================================================*/

/*===========================================================================*/
/* Module local variables.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Module local functions.                                                   */
/*===========================================================================*/
static inline msg_t invokeStubService(skel_req_t *skreqp) {
  msg_t r;

  chMtxLock(&skreqp->scp->stub_svc_mtx);
  r = tsInvokeServiceNoYield(skreqp->scp->stub_svc,
          (ts_params_area_t)skreqp, sizeof *skreqp);
  chDbgAssert(r != SMC_SVC_BUSY, "Unexpected SMC_SVC_BUSY");
  chMtxUnlock(&skreqp->scp->stub_svc_mtx);
  return r;
}

/*===========================================================================*/
/* Module exported functions.                                                */
/*===========================================================================*/

/**
 * @brief Invoke the stubs service in order to copy the 'in'
 *        parameters in the non secure memory space.
 */
void paramsInFromRemote(skel_req_t *skreqp) {
  skreqp->req = SKEL_REQ_CPYPRMS;
  (void) invokeStubService(skreqp);
}

/**
 * @brief Invoke the stubs service in order to copy the 'out'
 *        parameters in the secure memory space and set the
 *        remote call result.
 */
void returnToRemote(skel_req_t *skreqp, uint32_t res) {
  skreqp->stub_op_result = res;
  skreqp->req = SKEL_REQ_PUTRES;
  (void) invokeStubService(skreqp);
  chFifoReturnObject(&skreqp->scp->skel_req_fifo, skreqp);
}

/**
 * @brief   Dispatch a request to a skeleton worker thread.
 */
THD_FUNCTION(TsSkelsDaemon, arg) {
  skel_ctx_t *skel_ctx = (skel_ctx_t *)arg;
  event_listener_t  el;
  skel_req_t        *skreqp;
  msg_t             r;

  chEvtRegisterMaskWithFlags(&stubsEventSource, &el, ALL_EVENTS,
      skel_ctx->skel_eventflag);
  chMtxObjectInit(&skel_ctx->stub_svc_mtx);
  skel_ctx->stub_svc = (ts_service_t)tsInvokeServiceNoYield(TS_HND_DISCOVERY,
        (ts_params_area_t)skel_ctx->stub_svc_name,
        strlen(skel_ctx->stub_svc_name) + 1);

  /* Tell to stubs service that we are ready.*/
  skreqp = chFifoTakeObjectTimeout(&skel_ctx->skel_req_fifo, TIME_INFINITE);
  skreqp->req = SKEL_REQ_READY;
  skreqp->stub_op = skel_ctx->skel_eventflag;
  tsInvokeServiceNoYield(skel_ctx->stub_svc, (ts_params_area_t)skreqp,
      sizeof *skreqp);
  chFifoReturnObject(&skel_ctx->skel_req_fifo, skreqp);

  /* Start to receive ops from stubs.*/
  for (;/* ever */;) {
    chEvtWaitAny(ALL_EVENTS);
    (void)chEvtGetAndClearFlags(&el);
    while (true) {
      skreqp = chFifoTakeObjectTimeout(&skel_ctx->skel_req_fifo, TIME_INFINITE);
      skreqp->req = SKEL_REQ_GETOP;
      skreqp->scp = skel_ctx;
      r = invokeStubService(skreqp);
      if (r == SMC_SVC_NHND)
        break;
      chFifoSendObject(&skel_ctx->skel_req_fifo, skreqp);
    }
    chFifoReturnObject(&skel_ctx->skel_req_fifo, skreqp);
  }
}
