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
    if (rp->used && chMemIsSpaceWithinX(&rp->area, start, size)) {
      return true;
    }
    rp++;
  } while (rp < &sbcp->config->regions[SB_CFG_NUM_REGIONS]);

  return false;
}

bool sb_is_valid_write_range(sb_class_t *sbcp, void *start, size_t size) {
  const sb_memory_region_t *rp = &sbcp->config->regions[0];

  do {
    if (rp->used && chMemIsSpaceWithinX(&rp->area, start, size)) {
      return rp->writeable;
    }
    rp++;
  } while (rp < &sbcp->config->regions[SB_CFG_NUM_REGIONS]);

  return false;
}

size_t sb_check_string(sb_class_t *sbcp, const char *s, size_t max) {
  const sb_memory_region_t *rp = &sbcp->config->regions[0];

  do {
    if (rp->used) {
      size_t n = chMemIsStringWithinX(&rp->area, s, max);
      if (n > (size_t)0) {
        return n;
      }
    }
    rp++;
  } while (rp < &sbcp->config->regions[SB_CFG_NUM_REGIONS]);

  return (size_t)0;
}

size_t sb_check_pointers_array(sb_class_t *sbcp, const void *pp[], size_t max) {
  const sb_memory_region_t *rp = &sbcp->config->regions[0];

  do {
    if (rp->used) {
      size_t an = chMemIsPointersArrayWithinX(&rp->area, pp, max);
      if (an > (size_t)0) {
        return an;
      }
    }
    rp++;
  } while (rp < &sbcp->config->regions[SB_CFG_NUM_REGIONS]);

  return (size_t)0;
}

size_t sb_check_strings_array(sb_class_t *sbcp, const char *pp[], size_t max) {
  const char *s;
  size_t n;

  n = sb_check_pointers_array(sbcp, (const void **)pp, max);
  if (n > (size_t)0) {
    while ((s = *pp++) != NULL) {
      size_t sn;

      sn = sb_check_string(sbcp, s, max - n);
      if (sn == (size_t)0) {
        return (size_t)0;
      }

      n += sn;
    }
  }

  return n;
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
 * @param[in] argc      number of parameters for the sandbox
 * @param[in] argv      array of parameters for the sandbox
 * @param[in] envp      array of environment variables for the sandbox
 * @return              The thread pointer.
 * @retval NULL         if the sandbox thread creation failed.
 */
thread_t *sbStartThread(sb_class_t *sbcp, const char *name,
                        void *wsp, size_t size, tprio_t prio,
                        int argc, char *argv[], char *envp[]) {
  thread_t *utp;
  const sb_header_t *sbhp;
  const sb_config_t *config = sbcp->config;
  uint32_t *sp;

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

  /* Checking header entry point.*/
  if (!chMemIsSpaceWithinX(&config->regions[config->code_region].area,
                           (const void *)sbhp->hdr_entry,
                           (size_t)2)) {
    return NULL;
  }

  /* Setting up an initial stack for the sandbox.*/
  sp = (uint32_t *)(void *)(config->regions[config->data_region].area.base +
                            config->regions[config->data_region].area.size);
  sp -= 3 * sizeof (uint32_t);
  sp[0] = (uint32_t)argc;
  sp[1] = (uint32_t)argv;
  sp[2] = (uint32_t)envp;

  unprivileged_thread_descriptor_t utd = {
    .name       = name,
    .wbase      = (stkalign_t *)wsp,
    .wend       = (stkalign_t *)wsp + (size / sizeof (stkalign_t)),
    .prio       = prio,
    .u_pc       = sbhp->hdr_entry,
    .u_psp      = (uint32_t)sp,
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

/**
 * @brief   Verifies if the sandbox thread is running.
 *
 * @param[in] sbcp      pointer to a @p sb_class_t structure
 * @return              The thread status.
 *
 * @api
 */
bool sbIsThreadRunningX(sb_class_t *sbcp) {

  if (sbcp->tp == NULL) {
    return false;
  }

  return !chThdTerminatedX(sbcp->tp);
}

#if (CH_CFG_USE_WAITEXIT == TRUE) || defined(__DOXYGEN__)
/**
 * @brief   Blocks the execution of the invoking thread until the specified
 *          sandbox thread terminates then the exit code is returned.
 * @pre     The configuration option @p CH_CFG_USE_WAITEXIT must be enabled in
 *          order to use this function.
 *
 * @param[in] sbcp      pointer to a @p sb_class_t structure
 * @return              The exit code from the terminated sandbox thread.
 * @retval MSG_RESET    Sandbox thread not started.
 *
 * @api
 */
msg_t sbWaitThread(sb_class_t *sbcp) {
  msg_t msg;

  if (sbcp->tp == NULL) {
    return MSG_RESET;
  }

  msg = chThdWait(sbcp->tp);
  sbcp->tp = NULL;

  return msg;
}
#endif

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
