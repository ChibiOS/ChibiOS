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
 * @file    tssockstub.c
 * @brief   Sockets stub for trusted services.
 *
 */

#include "ch.h"
#include "chobjfifos.h"
#include "chtssi.h"
#include "tsproxystubs.h"
#include <string.h>
#include <ctype.h>

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
static eventflags_t tsSkelIsReadyMask = 0;

/*===========================================================================*/
/* Module local functions.                                                   */
/*===========================================================================*/
static bool isOpValid(stub_ctx_t *scp, stub_op_t *op)
{
  if ((op < &(scp->ops[0])) || (op >= &(scp->ops[STUB_MAX_OPS])))
    return FALSE;
  if (((char *)op - (char *)&(scp->ops[0])) % sizeof scp->ops[0])
    return FALSE;
  return TRUE;
}

/*===========================================================================*/
/* Module exported functions.                                                */
/*===========================================================================*/

/**
 * @brief   Implement an a call to a NSEC function.
 * @details It activates the channel between the stubs service and
 *          the skels daemon running in the nsec world.
 *          To do it, it uses an event to signal the corresponding skel
 *          daemon that a new op request is ready to be executed.
 *          Behind the scenes, the skel daemon will then gets the op, calling
 *          the stub service via smc. The daemon executes it and then calls
 *          the stub service again to post the result and to wake up the
 *          calling thread of this function.
 *
 * @param[in] op    the 'remote' method description.
 *
 * @return          the return value of 'remote' method.
 */
uint32_t callRemote(stub_op_t *op) {
  uint32_t r;

  chSysLock();
  chFifoSendObjectI(&op->scp->ops_fifo, op);
  chEvtBroadcastFlagsI(&tsEventSource, op->scp->event_flag);
  chThdSuspendS(&op->op_wthdp);
  op->op_state = FREE;
  chSysUnlock();
  r = op->op_code;
  chFifoReturnObject(&op->scp->ops_fifo, op);
  return r;
}

stub_op_t *getNewOp(stub_ctx_t *scp) {
  stub_op_t *op = chFifoTakeObjectTimeout(&scp->ops_fifo, TIME_INFINITE);
  memset(op, 0, sizeof *op);
  op->op_state = CALLING;
  op->scp = scp;
  return op;
}

/**
 * @brief     The stubs service.
 * @details   And this is where the magic happens.
 */
void TsStubService(ts_state_t *svcp, stub_ctx_t *scp) {
  skel_req_t *skrp;
  stub_op_t *op;
  msg_t r;
  int i;

  chFifoObjectInit(&scp->ops_fifo, sizeof (stub_op_t), STUB_MAX_OPS,
      sizeof (uint8_t), scp->ops, scp->ops_msgs);
  for (;/* ever */;) {

    /* Wait a service request.*/
    (void)tssiWaitRequest(svcp);
    skrp = (skel_req_t *)TS_GET_DATA(svcp);
    r = SMC_SVC_OK;

    /* Process the request.*/
    if (TS_GET_DATALEN(svcp) != sizeof (skel_req_t)) {
      TS_SET_STATUS(svcp, SMC_SVC_INVALID);
      continue;
    }

    switch (skrp->req) {
    case SKEL_REQ_READY:
      tsSkelIsReadyMask |= (eventflags_t)skrp->stub_op;
      break;

    case SKEL_REQ_GETOP:

      /* The nsec skeleton calls us to get a new op ready to be executed.*/
      if (chFifoReceiveObjectTimeout(&scp->ops_fifo, (void **)&op,
          TIME_IMMEDIATE) == MSG_TIMEOUT) {

        /* no op ready to be executed.*/
        r = SMC_SVC_NHND;
        break;
      }
      skrp->stub_op = (uint32_t)op;
      skrp->stub_op_code = op->op_code;

      /* Pass all the 'by value' arguments from stub to skel.*/
      for (i = 0; i < METHOD_MAX_PARAMS; ++i) {
        if (op->op_p[i].dir == OP_PRMDIR_NONE)
          skrp->stub_op_p[i] = op->op_p[i].val;
      }
      op->op_state = PENDING;
      break;

    case SKEL_REQ_CPYPRMS:

      /* The nsec skel calls us to get a copy of the 'in' parameters of
         the specified op.
         An 'in' parameter is an indirect argument, that is an argument
         the value of which is a pointer to a memory buffer, that
         must be copied in a non secure memory buffer.
         It represents data to be consumed by the callee.*/
      op = (stub_op_t *)skrp->stub_op;
      if (!isOpValid(scp, op) || op->op_state != PENDING ||
            op->op_code != skrp->stub_op_code) {
        r = SMC_SVC_INVALID;
        break;
      }

      /* Copy all 'in' parameters.
         For each parameter check that the destination memory area
         is in the non secure memory arena.*/
      for (i = 0; i < METHOD_MAX_PARAMS; ++i) {
        if ((op->op_p[i].dir & OP_PRMDIR_IN) == 0)
          continue;
        if (!tsIsAddrSpaceValid((void *)skrp->stub_op_p[i], op->op_p[i].size)) {
          r = SMC_SVC_INVALID;
          break;
        }
        memcpy((void *)skrp->stub_op_p[i], (void *)op->op_p[i].val,
            op->op_p[i].size);
      }
      break;

    case SKEL_REQ_PUTRES:

      /* The nsec skel calls us to put a copy of the 'out' parameters of
         the specified op.
         An 'out' parameter is an indirect argument, that is an argument
         the value of which is a pointer to a memory buffer, that
         must be copied in a secure memory buffer.
         It represents data produced by the callee.*/
      op = (stub_op_t *)skrp->stub_op;
      if (!isOpValid(scp, op) || op->op_state != PENDING ||
            op->op_code != skrp->stub_op_code) {
        r = SMC_SVC_INVALID;
        break;
      }

      /* Copy all 'out' parameters.
         For each parameter check that the source memory area
         is in the non secure memory arena, and that the size returned
         fits in the caller buffer size.*/
      for (i = 0; i < METHOD_MAX_PARAMS; ++i) {
        if ((op->op_p[i].dir & OP_PRMDIR_OUT) == 0)
          continue;
        if (!tsIsAddrSpaceValid((void *)skrp->stub_op_p[i], skrp->stub_op_p_sz[i])
              || (skrp->stub_op_p_sz[i] > op->op_p[i].size)) {
          r = SMC_SVC_INVALID;
          break;
        }
        memcpy((void *)op->op_p[i].val, (void *)skrp->stub_op_p[i],
            skrp->stub_op_p_sz[i]);
      }
      if (r != SMC_SVC_OK)
        break;

      /* Set the return value of the 'remote' callee method,
         and wake up the caller.*/
      op->op_code = skrp->stub_op_result;
      chThdResume(&op->op_wthdp, MSG_OK);
      break;

    default:
      r = SMC_SVC_INVALID;
      break;
    }

    /* Set the response.*/
    TS_SET_STATUS(svcp, r);
  }
}

/**
 * @brief     Is the skeletons daemon ready to operate?
 * @details   It is used at the startup to synchronize the
 *            stub service with the skeleton daemon.
 */
void tsWaitStubSkelReady(eventflags_t mask) {
  while ((tsSkelIsReadyMask & mask) != mask) {
    chThdSleepMilliseconds(100);
  }
}

