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
 * @file    sb/host/sbvrq.c
 * @brief   ARM SandBox host Virtual IRQs code.
 *
 * @addtogroup ARM_SANDBOX_VRQ
 * @{
 */

#include "sb.h"

#if (SB_CFG_ENABLE_VRQ == TRUE) || defined(__DOXYGEN__)

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

/**
 * @brief   Used as a known privileged address.
 */
static void vrq_privileged_code(void) {

  while (true) {
  }
}

CC_FORCE_INLINE
static inline void vrq_initctx(sb_class_t *sbp,
                               struct port_extctx *ectxp,
                               sb_vrqnum_t nvrq) {

  /* Clearing served VRQ.*/
  sbp->vrq.wtmask &= ~(1U << nvrq);

  /* Disabling VRQs globally during processing.*/
  sbp->vrq.isr = SB_VRQ_ISR_DISABLED;

  /* Building the return context.*/
  ectxp->r0     = nvrq;
#if 0
  ectxp->r1     = 0U;
  ectxp->r2     = 0U;
  ectxp->r3     = 0U;
  ectxp->r12    = 0U;
  ectxp->lr_thd = 0U;
#endif
  ectxp->pc     = ((const sb_header_t *)(void *)sbp->regions[0].area.base)->hdr_vrq;
  ectxp->xpsr   = 0x01000000U;
#if CORTEX_USE_FPU == TRUE
  ectxp->fpscr  = FPU->FPDSCR;
#endif
}

/* Encouraging a tail call on this function.*/
CC_NO_INLINE
static void vrq_pushctx_other(sb_class_t *sbp, sb_vrqnum_t nvrq) {
  struct port_extctx *ectxp;

  /* Current stack frame position.*/
  ectxp = (struct port_extctx *)sbp->u_psp;

  /* Position of the new stack frame, it depends on FPU settings and state.*/
  ectxp = (struct port_extctx *)(sbp->u_psp - sizeof (struct port_extctx));

  /* Checking if the new frame is within the sandbox else failure.*/
  if (!sb_is_valid_write_range(sbp, (void *)ectxp, sizeof (struct port_extctx))) {
    /* Making the sandbox return on a privileged address, this
       will cause a fault and sandbox termination.*/
    ectxp->pc = (uint32_t)vrq_privileged_code;
  }
  else {
    /* Creating a new context for the VRQ handler return.*/
    vrq_initctx(sbp, ectxp, nvrq);
  }

  sbp->u_psp = (uint32_t)ectxp;
}

/* Encouraging a tail call on this function.*/
CC_NO_INLINE
static void vrq_pushctx_this(sb_class_t *sbp, uint32_t psp, sb_vrqnum_t nvrq) {
  struct port_extctx *ectxp;

  /* Position of the new stack frame, it depends on FPU settings and state.*/
  ectxp = (struct port_extctx *)(psp - sizeof (struct port_extctx));

  /* Checking if the new frame is within the sandbox else failure.*/
  if (!sb_is_valid_write_range(sbp, (void *)ectxp, sizeof (struct port_extctx))) {
    /* Making the sandbox return on a privileged address, this
       will cause a fault and sandbox termination.*/
    ectxp->pc = (uint32_t)vrq_privileged_code;
  }
  else {
    /* Creating a new context for the VRQ handler return.*/
    vrq_initctx(sbp, ectxp, nvrq);
    __set_PSP((uint32_t)ectxp);
#if PORT_SAVE_PSPLIM
    __set_PSPLIM(sbp->u_psplim);
#endif
  }
}

CC_NO_INLINE
static void vrq_fastc_check_pending(struct port_extctx *ectxp, sb_class_t *sbp) {

  /* Processing pending VRQs if enabled.*/
  if (((sbp->vrq.isr & SB_VRQ_ISR_DISABLED) == 0U)) {
    sb_vrqmask_t active_mask;

    active_mask = sbp->vrq.wtmask & sbp->vrq.enmask;
    if (active_mask != 0U) {

      /* Creating a return context.*/
      vrq_pushctx_this(sbp, (uint32_t)ectxp, __CLZ(__RBIT(active_mask)));
    }
  }
}

static void delay_cb(virtual_timer_t *vtp, void *arg) {
  sb_class_t *sbp = (sb_class_t *)arg;

  (void)vtp;

  sbVRQTriggerFromISR(sbp, SB_CFG_ALARM_VRQ);
}

/*===========================================================================*/
/* Module exported functions.                                                */
/*===========================================================================*/

/**
 * @brief   Add flags to a VRQ.
 * @details Flags are sent in the VRQ context and cleared, it is a fast way
 *          to transmit a (virtual) peripheral status information when a
 *          VRQ is triggered.
 *
 * @param[in] sbp       pointer to a @p sb_class_t structure
 * @param[in] nvrq      number of VRQ to be activated
 * @param[in] flags     flags to be added to the VRQ
 *
 * @iclass
 */

void sbVRQSetFlagsI(sb_class_t *sbp, sb_vrqnum_t nvrq, uint32_t flags) {

  sbp->vrq.flags[nvrq] |= flags;
}

/**
 * @brief   Triggers VRQs on the specified sandbox.
 * @note    This function can only be used to send VRQs on sandboxes running
 *          on the same core.
 *
 * @param[in] sbp       pointer to a @p sb_class_t structure
 * @param[in] nvrq      number of VRQ to be activated
 *
 * @sclass
 */
void sbVRQTriggerS(sb_class_t *sbp, sb_vrqnum_t nvrq) {

  chDbgCheckClassS();

  chDbgAssert(sbp->thread.owner == currcore, "different core");

  /* Adding VRQ mask to the pending mask.*/
  sbp->vrq.wtmask |= (sb_vrqmask_t)(1U << nvrq);

  /* Only doing the following if VRQs are globally enabled.*/
  if ((sbp->vrq.isr & SB_VRQ_ISR_DISABLED) == 0U) {
    sb_vrqmask_t active_mask;

    /* Checking if there are VRQs to be served immediately.*/
    active_mask = sbp->vrq.wtmask & sbp->vrq.enmask;
    if (active_mask != 0U) {

      /* Checking if it has been called from this sandbox thread by
         a syscall handler.*/
      if (sbp->thread.state == CH_STATE_CURRENT) {
        /* Doing nothing, pending VRQs will be handled while exiting the
           current syscall.*/
      }
      else {
        /* Setting VRQs on some other thread. In this case the privilege
           information is stored in the internal sandbox thread context
           because it is switched-out.*/
        if ((sbp->thread.ctx.regs.control & 1U) != 0U) {

          /* Unprivileged mode, creating a return context on the sandbox
             thread.*/
          vrq_pushctx_other(sbp, __CLZ(__RBIT(active_mask)));
        }
        else {
          /* Privileged mode, so it will check for pending VRQs while
             exiting the syscall. Just trying to wake up the thread
             in case it is waiting for VRQs.*/
          chThdResumeS(&sbp->vrq.trp, MSG_OK);
        }
      }
    }
  }
}

/**
 * @brief   Triggers VRQs on the specified sandbox.
 * @note    This function can only be used to send VRQs on sandboxes running
 *          on the same core.
 * @note    This function must be called from IRQ context because
 *          it manipulates exception stack frames.
 *
 * @param[in] sbp       pointer to a @p sb_class_t structure
 * @param[in] nvrq      number of VRQ to be activated
 *
 * @iclass
 */
void sbVRQTriggerI(sb_class_t *sbp, sb_vrqnum_t nvrq) {

  chDbgCheckClassI();

  chDbgAssert(sbp->thread.owner == currcore, "different core");

  /* Adding VRQ mask to the pending mask.*/
  sbp->vrq.wtmask |= (sb_vrqmask_t)(1U << nvrq);

  /* Only doing the following if VRQs are globally enabled.*/
  if ((sbp->vrq.isr & SB_VRQ_ISR_DISABLED) == 0U) {
    sb_vrqmask_t active_mask;

    /* Checking if there are VRQs to be served immediately.*/
    active_mask = sbp->vrq.wtmask & sbp->vrq.enmask;
    if (active_mask != 0U) {

      /* Checking if it happened to preempt this sandbox thread.*/
      if (sbp->thread.state == CH_STATE_CURRENT) {
        /* Checking if it is running in unprivileged mode, in this case we
           need to build a return context in its current PSP.*/
        if ((__get_CONTROL() & 1U) != 0U) {

          /* Creating a return context.*/
          vrq_pushctx_this(sbp, __get_PSP(), __CLZ(__RBIT(active_mask)));
        }
        else {
          /* It is in privileged mode so it will check for pending VRQs
             while exiting the syscall. Just trying to wake up the thread
             in case it is waiting for VRQs.*/
          chThdResumeI(&sbp->vrq.trp, MSG_OK);
        }
      }
      else {
        /* We preempted some other thread. In this case the privilege
           information is stored in the internal sandbox thread context
           because it is switched-out.*/
        if ((sbp->thread.ctx.regs.control & 1U) != 0U) {

          /* Unprivileged mode, creating a return context on the sandbox
             thread.*/
          vrq_pushctx_other(sbp, __CLZ(__RBIT(active_mask)));
        }
        else {
          /* Privileged mode, so it will check for pending VRQs while
             exiting the syscall. Just trying to wake up the thread
             in case it is waiting for VRQs.*/
          chThdResumeI(&sbp->vrq.trp, MSG_OK);
        }
      }
    }
  }
}

void sb_sysc_vrq_set_alarm(sb_class_t *sbp, struct port_extctx *ectxp) {
  sysinterval_t interval = (sysinterval_t )ectxp->r0;
  bool reload = (bool)ectxp->r1;

  if (reload) {
    chVTSetContinuous(&sbp->vrq.alarm_vt, interval, delay_cb, (void *)sbp);
  }
  else {
    chVTSet(&sbp->vrq.alarm_vt, interval, delay_cb, (void *)sbp);
  }
}

void sb_sysc_vrq_reset_alarm(sb_class_t *sbp, struct port_extctx *ectxp) {

  (void)ectxp;

  chVTReset(&sbp->vrq.alarm_vt);
}

void sb_sysc_vrq_wait(sb_class_t *sbp, struct port_extctx *ectxp) {
  sb_vrqmask_t active_mask;

  (void)ectxp;

  chSysLock();

  active_mask = sbp->vrq.wtmask & sbp->vrq.enmask;
  if (active_mask == 0U) {
    chThdSuspendS(&sbp->vrq.trp);
  }

  chSysUnlock();
}

void sb_fastc_vrq_gcsts(sb_class_t *sbp, struct port_extctx *ectxp) {
  uint32_t nvrq = ectxp->r0;
  const uint32_t vrq_num =
    (uint32_t)(sizeof (sbp->vrq.flags) / sizeof (sbp->vrq.flags[0]));

  if (nvrq >= vrq_num) {
    ectxp->r0 = (uint32_t)CH_RET_EINVAL;
    return;
  }

  /* Cast because vrq.flags[] could be configured to be a smaller type.*/
  ectxp->r0 = (uint32_t)sbp->vrq.flags[nvrq];
  sbp->vrq.flags[nvrq] = 0U;

  /* No need to check for pending VRQs.*/
}

void sb_fastc_vrq_setwt(sb_class_t *sbp, struct port_extctx *ectxp) {
  uint32_t m;

  m = ectxp->r0;
  ectxp->r0 = sbp->vrq.wtmask;
  sbp->vrq.wtmask |= m;

  vrq_fastc_check_pending(ectxp, sbp);
}

void sb_fastc_vrq_clrwt(sb_class_t *sbp, struct port_extctx *ectxp) {
  uint32_t m;

  m = ectxp->r0;
  ectxp->r0 = sbp->vrq.wtmask;
  sbp->vrq.wtmask &= ~m;

  /* No need to check for pending VRQs.*/
}

void sb_fastc_vrq_seten(sb_class_t *sbp, struct port_extctx *ectxp) {
  uint32_t m;

  m = ectxp->r0;
  ectxp->r0 = sbp->vrq.enmask;
  sbp->vrq.enmask |= m;

  vrq_fastc_check_pending(ectxp, sbp);
}

void sb_fastc_vrq_clren(sb_class_t *sbp, struct port_extctx *ectxp) {
  uint32_t m;

  m = ectxp->r0;
  ectxp->r0 = sbp->vrq.enmask;
  sbp->vrq.enmask &= ~m;

  /* No need to check for pending VRQs.*/
}

void sb_fastc_vrq_disable(sb_class_t *sbp, struct port_extctx *ectxp) {

  (void)ectxp;

  sbp->vrq.isr = SB_VRQ_ISR_DISABLED;

  /* No need to check for pending VRQs.*/
}

void sb_fastc_vrq_enable(sb_class_t *sbp, struct port_extctx *ectxp) {
  sb_vrqmask_t active_mask;

  sbp->vrq.isr = 0U;
  asm ("" : : : "memory");
  active_mask = sbp->vrq.enmask & sbp->vrq.wtmask;
  if (unlikely(active_mask != 0U)) {

    /* Creating a return context.*/
    vrq_pushctx_this(sbp, (uint32_t)ectxp, __CLZ(__RBIT(active_mask)));
  }
}

void sb_fastc_vrq_getisr(sb_class_t *sbp, struct port_extctx *ectxp) {

  ectxp->r0 = sbp->vrq.isr;

  /* No need to check for pending VRQs.*/
}

void sb_fastc_vrq_return(sb_class_t *sbp, struct port_extctx *ectxp) {
  register sb_vrqmask_t active_mask;

  active_mask = sbp->vrq.wtmask & sbp->vrq.enmask;
  if (active_mask != 0U) {
    sb_vrqnum_t nvrq = __CLZ(__RBIT(active_mask));

    sbp->vrq.wtmask &= ~(1U << nvrq);

    /* Building the return context.*/
    ectxp->r0     = nvrq;
#if 0
    ectxp->r1     = 0U;
    ectxp->r2     = 0U;
    ectxp->r3     = 0U;
    ectxp->r12    = 0U;
    ectxp->lr_thd = 0U;
#endif
    ectxp->pc     = ((const sb_header_t *)(void *)sbp->regions[0].area.base)->hdr_vrq;
    ectxp->xpsr   = 0x01000000U;
#if CORTEX_USE_FPU == TRUE
    ectxp->fpscr  = FPU->FPDSCR;
#endif
  }
  else {

    /* Discarding the return current context, returning on the previous one.
       TODO: Check for overflows????*/
    ectxp++;

    /* Re-enabling VRQs globally.*/
    sbp->vrq.isr = 0U;

    /* Keeping the current return context.*/
    __set_PSP((uint32_t)ectxp);
#if PORT_SAVE_PSPLIM
    __set_PSPLIM(sbp->u_psplim);
#endif
  }
}

/**
 * @brief   Checks for pending VRQs, creates a return context if any.
 *
 * @param[in] sbp       pointer to a @p sb_class_t structure
 * @param[in] ectxp     current return context
 *
 * @notapi
 */
void __sb_vrq_check_pending(sb_class_t *sbp, struct port_extctx *ectxp) {

  /* Processing pending VRQs if enabled.*/
  if (((sbp->vrq.isr & SB_VRQ_ISR_DISABLED) == 0U)) {
    sb_vrqmask_t active_mask;

    active_mask = sbp->vrq.wtmask & sbp->vrq.enmask;
    if (active_mask != 0U) {

      /* Creating a return context.*/
      vrq_pushctx_this(sbp, (uint32_t)ectxp, __CLZ(__RBIT(active_mask)));
      return;
    }
  }

  __set_PSP((uint32_t)ectxp);
#if PORT_SAVE_PSPLIM
  __set_PSPLIM(sbp->u_psplim);
#endif
}

#endif /* SB_CFG_ENABLE_VRQ == TRUE */

/** @} */
