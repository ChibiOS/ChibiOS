/*
    ChibiOS - Copyright (C) 2006,2007,2008,2009,2010,2011,2012,2013,2014,
              2015,2016,2017,2018,2019,2020,2021 Giovanni Di Sirio.

    This file is part of ChibiOS.

    ChibiOS is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation version 3 of the License.

    ChibiOS is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

/**
 * @file    sb/host/sbhost.c
 * @brief   ARM SandBox host code.
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

/**
 * @brief   Global sandbox managed state variable.
 */
sb_t sb;

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
    if (chMemIsAreaContainedX(&rp->area, start, size)) {
      return true;
    }
    rp++;
  } while (rp < &sbcp->config->regions[SB_CFG_NUM_REGIONS]);

  return false;
}

bool sb_is_valid_write_range(sb_class_t *sbcp, void *start, size_t size) {
  const sb_memory_region_t *rp = &sbcp->config->regions[0];

  do {
    if (chMemIsAreaWithinX(&rp->area, start, size)) {
      return rp->writeable;
    }
    rp++;
  } while (rp < &sbcp->config->regions[SB_CFG_NUM_REGIONS]);

  return false;
}

bool sb_is_valid_string_range(sb_class_t *sbcp, const char *s, size_t n) {
  const sb_memory_region_t *rp = &sbcp->config->regions[0];

  do {
    if (chMemIsStringWithinX(&rp->area, s, n)) {
      return true;
    }
    rp++;
  } while (rp < &sbcp->config->regions[SB_CFG_NUM_REGIONS]);

  return false;
}

/**
 * @brief   Sandbox object initialization.
 *
 * @param[out] sbcp     pointer to the sandbox object
 * @param[in] config    pointer to the sandbox configuration
 *
 * @init
 */
void sbObjectInit(sb_class_t *sbcp, const sb_config_t *config) {

  sbcp->config = config;
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
 * @param[in] sbcp      pointer to a @p sb_class_t structure
 * @param[in] name      name to be assigned to the thread
 * @param[out] wsp      pointer to a working area dedicated to the thread stack
 * @param[in] size      size of the working area
 * @param[in] prio      the priority level for the new thread
 * @return              The thread pointer.
 * @retval NULL         if the sandbox thread creation failed.
 */
thread_t *sbStartThread(sb_class_t *sbcp, const char *name,
                        void *wsp, size_t size,
                        tprio_t prio) {
  thread_t *utp;
  const sb_header_t *sbhp;
  const sb_config_t *config = sbcp->config;

  /* Header location.*/
  sbhp = (const sb_header_t *)(void *)config->regions[config->code_region].area.base;

  /* Checking header magic numbers.*/
  if ((sbhp->hdr_magic1 != SB_MAGIC1) || (sbhp->hdr_magic2 != SB_MAGIC2)) {
    return NULL;
  }

  /* Checking header size and alignment.*/
  if (sbhp->hdr_size != sizeof (sb_header_t)) {
    return NULL;
  }

  unprivileged_thread_descriptor_t utd = {
    .name       = name,
    .wbase      = (stkalign_t *)wsp,
    .wend       = (stkalign_t *)wsp + (size / sizeof (stkalign_t)),
    .prio       = prio,
    .u_pc       = (uint32_t)(config->regions[config->code_region].area.base +
                             sizeof (sb_header_t)) | 1U,
    .u_psp      = (uint32_t)(config->regions[config->data_region].area.base +
                             config->regions[config->data_region].area.size),
    .arg        = (void *)sbcp
  };
#if PORT_SWITCHED_REGIONS_NUMBER > 0
  for (unsigned i = 0U; i < PORT_SWITCHED_REGIONS_NUMBER; i++) {
    utd.regions[i] = config->mpuregs[i];
  }
#endif

  utp = chThdCreateUnprivileged(&utd);

  /* For messages exchange.*/
  sbcp->tp      = utp;

  return utp;
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
  thread_t *ctp = __sch_get_currthread();

  chDbgCheck(sbcp != NULL);

  chSysLock();

  /* Sending the message.*/
  ctp->u.sentmsg = msg;
  __ch_msg_insert(&sbcp->tp->msgqueue, ctp);
  if (sbcp->tp->state == CH_STATE_WTMSG) {
    (void) chSchReadyI(sbcp->tp);
  }
  msg = chSchGoSleepTimeoutS(CH_STATE_SNDMSGQ, timeout);

  /* If a timeout occurred while the boxed thread already received the message
     then this thread needs to "unregister" as sender, the boxed error will
     get SB_ERR_EBUSY when/if trying to reply.*/
  if ((msg == MSG_TIMEOUT) && (sbcp->msg_tp == ctp)) {
    sbcp->msg_tp = NULL;
  }

  chSysUnlock();

  return msg;
}
#endif /* CH_CFG_USE_MESSAGES == TRUE */

/** @} */
