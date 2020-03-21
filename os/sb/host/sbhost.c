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
 * @file    sb/host/sbhost.c
 * @brief   ARM sandbox host code.
 *
 * @addtogroup ARM_SANDBOX
 * @{
 */

#include "ch.h"
#include "sb.h"

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

bool sb_is_valid_read_range(sb_class_t *sbcp, const void *start, size_t size) {
  const sb_memory_region_t *rp = &sbcp->config->regions[0];

  do {
    if (((uint32_t)start >= rp->base) && ((uint32_t)start < rp->end) &&
        (size <= ((size_t)rp->base - (size_t)start))) {
      return true;
    }
    rp++;
  } while (rp < &sbcp->config->regions[SB_NUM_REGIONS]);

  return false;
}

bool sb_is_valid_write_range(sb_class_t *sbcp, void *start, size_t size) {
  const sb_memory_region_t *rp = &sbcp->config->regions[0];

  do {
    if (((uint32_t)start >= rp->base) && ((uint32_t)start < rp->end) &&
        (size <= ((size_t)rp->base - (size_t)start))) {
      return rp->writeable;
    }
    rp++;
  } while (rp < &sbcp->config->regions[SB_NUM_REGIONS]);

  return false;
}

/**
 * @brief   Sandbox object initialization.
 *
 * @param[out] sbcp     pointer to the sandbox object
 *
 * @init
 */
void sbObjectInit(sb_class_t *sbcp) {

  sbcp->config = NULL;
  sbcp->tp     = NULL;
#if CH_CFG_USE_MESSAGES == TRUE
  sbcp->msg_tp = NULL;
#endif
#if CH_CFG_USE_EVENTS == TRUE
  chEvtObjectInit(&sbcp->es);
#endif
}

/**
 * @brief   Starts a sandboxed thread.
 *
 * @param[in] sbcp      pointer to the sandbox object
 * @return              The function returns only if the operation failed.
 *
 * @api
 */
void sbStart(sb_class_t *sbcp, const sb_config_t *config) {
  uint32_t pc, psp;
  const sb_header_t *sbhp;

  /* Header location.*/
  sbhp = (const sb_header_t *)config->regions[config->code_region].base;

  /* Checking header magic numbers.*/
  if ((sbhp->hdr_magic1 != SB_MAGIC1) || (sbhp->hdr_magic2 != SB_MAGIC2)) {
    return;
  }

  /* Checking header size and alignment.*/
  if (sbhp->hdr_size != sizeof (sb_header_t)) {
    return;
  }

  /* PC initial address, by convention it is immediately after the header.*/
  pc = (config->regions[config->code_region].base + sizeof (sb_header_t)) | 1U;

  /* PSP initial address.*/
  psp = config->regions[config->data_region].end;

  /* Additional context information.*/
  sbcp->config = config;
  sbcp->tp     = chThdGetSelfX();
  sbcp->tp->ctx.syscall.p    = (const void *)sbcp;
  sbcp->tp->ctx.syscall.psp  = __get_PSP();

  /* Jumping to the unprivileged code.*/
  port_unprivileged_jump(pc, psp);

  /* Must never happen condition.*/
  chSysHalt("returned");
}

#if (CH_CFG_USE_MESSAGES == TRUE) || defined(__DOXYGEN__)
/**
 * @brief   Sends a message to a sandboxed thread.
 *
 * @param[in] sbcp      pointer to the sandbox object
 * @param[in] msg       message to be sent
 * @param[in] timeout   the number of ticks before the operation timeouts,
 *                      the following special values are allowed:
 *                      - @a TIME_INFINITE no timeout.
 *                      .
 * @return              The returned message.
 * @retval MSG_TIMEOUT  if a timeout occurred.
 * @retval MSG_RESET    if the exchange aborted, sandboxed thread API usage
 *                      error.
 *
 * @api
 */
msg_t sbSendMessageTimeout(sb_class_t *sbcp,
                           msg_t msg,
                           sysinterval_t timeout) {
  thread_t *ctp = currp;

  chDbgCheck(sbcp != NULL);

  chSysLock();

  /* Sending the message.*/
  ctp->u.sentmsg = msg;
  __msg_insert(ctp, &sbcp->tp->msgqueue);
  if (sbcp->tp->state == CH_STATE_WTMSG) {
    (void) chSchReadyI(sbcp->tp);
  }
  msg = chSchGoSleepTimeoutS(CH_STATE_SNDMSGQ, timeout);

  /* If a timeout occurred while the boxed thread already received the message
     then this thread needs to "unregister" as sender, the boxed error will
     get SB_ERR_EBUSY when/if trying to reply.*/
  if (sbcp->msg_tp == ctp) {
    sbcp->msg_tp = NULL;
  }

  chSysUnlock();

  return msg;
}
#endif /* CH_CFG_USE_MESSAGES == TRUE */

/** @} */
