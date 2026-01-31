/*
    Copyright (C) 2006-2026 Giovanni Di Sirio.

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
 * @brief   ARM SandBox host API code.
 *
 * @addtogroup ARM_SANDBOX_HOSTAPI
 * @{
 */

#include "ch.h"
#include "sb.h"

#if defined(SB_INCLUDE_USERAPI)
#include "sbuserapi.h"
#endif

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

static thread_t *sb_msg_wait_timeout_s(sysinterval_t timeout) {
  thread_t *currtp = chThdGetSelfX();
  thread_t *tp;

  chDbgCheckClassS();

  /* The sender thread could have timed out in sbSendMessageTimeout() so
     repeating the wait if it did.*/
  do {
    if (!chMsgIsPendingI(currtp)) {
      if (chSchGoSleepTimeoutS(CH_STATE_WTMSG, timeout) != MSG_OK) {
        return NULL;
      }
    }
  } while(ch_queue_isempty(&currtp->msgqueue));

  /* Dequeuing the sender thread and returning it.*/
  tp = threadref(ch_queue_fifo_remove(&currtp->msgqueue));
  tp->state = CH_STATE_SNDMSG;

  return tp;
}

/*===========================================================================*/
/* Module exported functions.                                                */
/*===========================================================================*/

void sb_fastc_get_systime(sb_class_t *sbp, struct port_extctx *ectxp) {

  (void)sbp;

  ectxp->r0 = (uint32_t)chVTGetSystemTimeX();
}

void sb_fastc_get_frequency(sb_class_t *sbp, struct port_extctx *ectxp) {

  (void)sbp;

  ectxp->r0 = (uint32_t)CH_CFG_ST_FREQUENCY;
}

void sb_sysc_exit(sb_class_t *sbp, struct port_extctx *ectxp) {

  (void)sbp;

#if SB_CFG_ENABLE_VFS == TRUE
  __sb_io_cleanup(sbp);
#endif

  chSysLock();
#if SB_CFG_ENABLE_VRQ == TRUE
  chVTResetI(&sbp->vrq.alarm_vt);
#endif
#if CH_CFG_USE_EVENTS == TRUE
  chEvtBroadcastI(&sb.termination_es);
#endif
  chThdExitS((msg_t )ectxp->r0);
  chSysUnlock();

  /* Cannot get here.*/
  ectxp->r0 = CH_RET_ENOSYS;
}

void sb_sysc_sleep(sb_class_t *sbp, struct port_extctx *ectxp) {
  sysinterval_t interval = (sysinterval_t )ectxp->r0;

  (void)sbp;

  if (interval != TIME_IMMEDIATE) {
    chThdSleep(interval);
  }

  ectxp->r0 = CH_RET_SUCCESS;
}

void sb_sysc_sleep_until_windowed(sb_class_t *sbp, struct port_extctx *ectxp) {

  (void)sbp;

  chThdSleepUntilWindowed((systime_t )ectxp->r0, (systime_t )ectxp->r1);

  ectxp->r0 = CH_RET_SUCCESS;
}

void sb_sysc_wait_message(sb_class_t *sbp, struct port_extctx *ectxp) {
#if CH_CFG_USE_MESSAGES == TRUE

  chSysLock();

  if (sbp->base.msg_tp == NULL) {
    sbp->base.msg_tp = sb_msg_wait_timeout_s(TIME_INFINITE);
    ectxp->r0 = (uint32_t)chMsgGet(sbp->base.msg_tp);
  }
  else {
    thread_t *tp = sbp->base.msg_tp;
    sbp->base.msg_tp = NULL;
    chMsgReleaseS(tp, MSG_RESET);
    ectxp->r0 = MSG_RESET;
  }

  chSysUnlock();
#else
  (void)sbp;

  ectxp->r0 = CH_RET_ENOSYS;
#endif
}

void sb_sysc_reply_message(sb_class_t *sbp, struct port_extctx *ectxp) {
#if CH_CFG_USE_MESSAGES == TRUE

  chSysLock();

  if (sbp->base.msg_tp != NULL) {
    thread_t *tp = sbp->base.msg_tp;
    sbp->base.msg_tp = NULL;
    chMsgReleaseS(tp, (msg_t )ectxp->r0);
    ectxp->r0 = CH_RET_SUCCESS;
  }
  else {
    ectxp->r0 = MSG_RESET;
  }

  chSysUnlock();
#else
  (void)sbp;

  ectxp->r0 = CH_RET_ENOSYS;
#endif
}

void sb_sysc_wait_one_timeout(sb_class_t *sbp, struct port_extctx *ectxp) {

#if CH_CFG_USE_EVENTS == TRUE
  (void)sbp;

  ectxp->r0 = (uint32_t)chEvtWaitOneTimeout((eventmask_t )ectxp->r0,
                                            (sysinterval_t )ectxp->r1);
#else
  (void)sbp;

  ectxp->r0 =  CH_RET_ENOSYS;
#endif
}

void sb_sysc_wait_any_timeout(sb_class_t *sbp, struct port_extctx *ectxp) {

#if CH_CFG_USE_EVENTS == TRUE
  (void)sbp;

  ectxp->r0 = (uint32_t)chEvtWaitAnyTimeout((eventmask_t )ectxp->r0,
                                            (sysinterval_t )ectxp->r1);
#else
  (void)sbp;

  ectxp->r0 =  CH_RET_ENOSYS;
#endif
}

void sb_sysc_wait_all_timeout(sb_class_t *sbp, struct port_extctx *ectxp) {

#if CH_CFG_USE_EVENTS == TRUE
  (void)sbp;

  ectxp->r0 = (uint32_t)chEvtWaitAllTimeout((eventmask_t )ectxp->r0,
                                            (sysinterval_t )ectxp->r1);
#else
  (void)sbp;

  ectxp->r0 =  CH_RET_ENOSYS;
#endif
}

void sb_sysc_broadcast_flags(sb_class_t *sbp, struct port_extctx *ectxp) {
#if CH_CFG_USE_EVENTS == TRUE

  chEvtBroadcastFlags(&sbp->base.es, (eventflags_t )ectxp->r0);
  ectxp->r0 = CH_RET_SUCCESS;
#else
  (void)sbp;

  ectxp->r0 = CH_RET_ENOSYS;
#endif
}

void sb_sysc_loadelf(sb_class_t *sbp, struct port_extctx *ectxp) {
#if (SB_CFG_ENABLE_VFS == TRUE) || defined(__DOXYGEN__)
  const char *fname = (const char *)ectxp->r0;
  uint8_t *buf = (uint8_t *)ectxp->r1;
  size_t size = (size_t)ectxp->r2;

  if (sbp->io.vfs_driver == NULL) {
    ectxp->r0 = CH_RET_ENOSYS;
    return;
  }

  if ((sb_check_string(sbp, (void *)fname, VFS_CFG_PATHLEN_MAX + 1) == (size_t)0) ||
       !MEM_IS_ALIGNED(buf, MEM_NATURAL_ALIGN) ||
       !MEM_IS_ALIGNED(size, MEM_NATURAL_ALIGN) ||
       !sb_is_valid_write_range(sbp, buf, size)) {
    ectxp->r0 = CH_RET_EFAULT;
  }
  else {
    memory_area_t ma = {buf, size};
    ectxp->r0 = (uint32_t)sbElfLoadFile(sbp->io.vfs_driver, fname, &ma);
  }
#else
  (void)sbp;

  ectxp->r0 = CH_RET_ENOSYS;
#endif
}

/** @} */
