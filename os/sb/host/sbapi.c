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
 * @file    sb/sbhost.c
 * @brief   ARMv7-M sandbox host API code.
 *
 * @addtogroup ARMV7M_SANDBOX_HOSTAPI
 * @{
 */

#include "ch.h"
#include "sbhost.h"

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

/*===========================================================================*/
/* Module exported functions.                                                */
/*===========================================================================*/

uint32_t sb_api_exit(struct port_extctx *ctxp) {

  chThdExit((msg_t)ctxp->r0);

  return SB_ERR_NOT_IMPLEMENTED;
}

uint32_t sb_api_get_systime(struct port_extctx *ctxp) {

  (void)ctxp;

  return (uint32_t)chVTGetSystemTimeX();
}

uint32_t sb_api_get_frequency(struct port_extctx *ctxp) {

  (void)ctxp;

  return (uint32_t)CH_CFG_ST_FREQUENCY;
}

uint32_t sb_api_sleep(struct port_extctx *ctxp) {

  chThdSleep((sysinterval_t)ctxp->r0);

  return SB_ERR_NOERROR;
}

uint32_t sb_api_sleep_until_windowed(struct port_extctx *ctxp) {

  chThdSleepUntilWindowed((systime_t)ctxp->r0, (systime_t)ctxp->r1);

  return SB_ERR_NOERROR;
}

uint32_t sb_api_wait_message(struct port_extctx *ctxp) {
  sb_class_t *sbcp = (sb_class_t *)chThdGetSelfX()->ctx.syscall.p;

  (void)ctxp;

  if (sbcp->msg_tp == NULL) {
    sbcp->msg_tp = chMsgWait();
    return (uint32_t)chMsgGet(sbcp->msg_tp);
  }
  else {
    chMsgRelease(sbcp->msg_tp, MSG_RESET);
    sbcp->msg_tp = NULL;
    return SB_ERR_API_USAGE;
  }
}

uint32_t sb_api_reply_message(struct port_extctx *ctxp) {
  sb_class_t *sbcp = (sb_class_t *)chThdGetSelfX()->ctx.syscall.p;

  if (sbcp->msg_tp != NULL) {
    chMsgRelease(sbcp->msg_tp, (msg_t)ctxp->r0);
    sbcp->msg_tp = NULL;
    return SB_ERR_API_USAGE;
  }
  else {
    return SB_ERR_API_USAGE;
  }
}

uint32_t sb_api_wait_one_timeout(struct port_extctx *ctxp) {

  return (uint32_t)chEvtWaitOneTimeout((eventmask_t)ctxp->r0,
                                       (sysinterval_t)ctxp->r1);
}

uint32_t sb_api_wait_any_timeout(struct port_extctx *ctxp) {

  return (uint32_t)chEvtWaitAnyTimeout((eventmask_t)ctxp->r0,
                                       (sysinterval_t)ctxp->r1);
}

uint32_t sb_api_wait_all_timeout(struct port_extctx *ctxp) {

  return (uint32_t)chEvtWaitAllTimeout((eventmask_t)ctxp->r0,
                                       (sysinterval_t)ctxp->r1);
}

/** @} */
