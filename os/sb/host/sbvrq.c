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

#if 0
CC_FORCE_INLINE
static inline void vrq_makectx(struct port_extctx *newctxp,
                               sb_class_t *sbp,
                               sb_vrqmask_t active_mask) {
  uint32_t irqn = __CLZ(__RBIT(active_mask));
  sbp->vrq_wtmask &= ~(1U << irqn);

  /* Disabling VRQs globally during processing.*/
  sbp->vrq_isr = SB_VRQ_ISR_DISABLED;

  /* Building the return context.*/
  newctxp->r0     = irqn;
  newctxp->pc     = sbp->sbhp->hdr_vrq;
  newctxp->xpsr   = 0x01000000U;
#if CORTEX_USE_FPU == TRUE
  newctxp->fpscr  = FPU->FPDSCR;
#endif
}

CC_NO_INLINE
static struct port_extctx *vrq_writectx(struct port_extctx *ectxp,
                                        sb_class_t *sbp,
                                        sb_vrqmask_t active_mask) {

  /* Checking if the new frame is within the sandbox else failure.*/
  if (!sb_is_valid_write_range(sbp,
                               (void *)(ectxp - 1),
                               sizeof (struct port_extctx))) {
    /* Making the sandbox return on a privileged address, this
       will cause a fault and sandbox termination.*/
    ectxp->pc = (uint32_t)vrq_privileged_code;
  }
  else {
    /* Creating a new context for return the VRQ handler.*/
    ectxp--;
    vrq_makectx(ectxp, sbp, active_mask);
  }

  return ectxp;
}

/* Note, this function may look an useless duplication of vrq_writectx() but
   the included __set_PSP() makes it a viable tail-call candidate for the
   compiler, this is a significant performance gain is several places.*/
CC_NO_INLINE
static void vrq_pushctx(struct port_extctx *ectxp,
                        sb_class_t *sbp,
                        sb_vrqmask_t active_mask) {

  /* Checking if the new frame is within the sandbox else failure.*/
  if (!sb_is_valid_write_range(sbp,
                               (void *)(ectxp - 1),
                               sizeof (struct port_extctx))) {
    /* Making the sandbox return on a privileged address, this
       will cause a fault and sandbox termination.*/
    ectxp->pc = (uint32_t)vrq_privileged_code;
  }
  else {
    /* Creating a new context for return the VRQ handler.*/
    ectxp--;
    vrq_makectx(ectxp, sbp, active_mask);
    __set_PSP((uint32_t)ectxp);
  }
}
#endif

CC_FORCE_INLINE
static inline void vrq_makectx(struct port_extctx *newctxp,
                               sb_class_t *sbp,
                               uint32_t irqn) {
  sbp->vrq_wtmask &= ~(1U << irqn);

  /* Disabling VRQs globally during processing.*/
  sbp->vrq_isr = SB_VRQ_ISR_DISABLED;

  /* Building the return context.*/
  newctxp->r0     = irqn;
  newctxp->pc     = sbp->sbhp->hdr_vrq;
  newctxp->xpsr   = 0x01000000U;
#if CORTEX_USE_FPU == TRUE
  newctxp->fpscr  = FPU->FPDSCR;
#endif
}

CC_NO_INLINE
static struct port_extctx *vrq_writectx(struct port_extctx *ectxp,
                                        sb_class_t *sbp,
                                        uint32_t irqn) {

  /* Checking if the new frame is within the sandbox else failure.*/
  if (!sb_is_valid_write_range(sbp,
                               (void *)(ectxp - 1),
                               sizeof (struct port_extctx))) {
    /* Making the sandbox return on a privileged address, this
       will cause a fault and sandbox termination.*/
    ectxp->pc = (uint32_t)vrq_privileged_code;
  }
  else {
    /* Creating a new context for return the VRQ handler.*/
    ectxp--;
    vrq_makectx(ectxp, sbp, irqn);
  }

  return ectxp;
}

/* Note, this function may look an useless duplication of vrq_writectx() but
   the included __set_PSP() makes it a viable tail-call candidate for the
   compiler, this is a significant performance gain is several places.*/
CC_NO_INLINE
static void vrq_pushctx(struct port_extctx *ectxp,
                        sb_class_t *sbp,
                        uint32_t irqn) {

  /* Checking if the new frame is within the sandbox else failure.*/
  if (!sb_is_valid_write_range(sbp,
                               (void *)(ectxp - 1),
                               sizeof (struct port_extctx))) {
    /* Making the sandbox return on a privileged address, this
       will cause a fault and sandbox termination.*/
    ectxp->pc = (uint32_t)vrq_privileged_code;
  }
  else {
    /* Creating a new context for return the VRQ handler.*/
    ectxp--;
    vrq_makectx(ectxp, sbp, irqn);
    __set_PSP((uint32_t)ectxp);
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
 * @brief   Triggers VRQs on the specified sandbox.
 *
 * @param[in] sbp       pointer to a @p sb_class_t structure
 * @param[in] nvrq      number of VRQ to be activated
 *
 * @sclass
 */
void sbVRQTriggerS(sb_class_t *sbp, sb_vrqnum_t nvrq) {

  chDbgCheckClassS();

  chDbgAssert(sbp->tp->state != CH_STATE_CURRENT, "it is current");

  /* Adding VRQ mask to the pending mask.*/
  sbp->vrq_wtmask |= (sb_vrqmask_t)(1U << nvrq);

  /* Triggering the VRQ if enabled.*/
  if ((sbp->vrq_isr & SB_VRQ_ISR_DISABLED) == 0U) {
    sb_vrqmask_t active_mask;

    /* Checking if there are VRQs to be served immediately.*/
    active_mask = sbp->vrq_wtmask & sbp->vrq_enmask;
    if (active_mask != 0U) {
      /* Checking if it is running in unprivileged mode, in this case we
         need to build a return context in its current PSP.*/
      if ((__get_CONTROL() & 1U) != 0U) {

        /* Creating a return context.*/
        vrq_pushctx((struct port_extctx *)__get_PSP(), sbp, __CLZ(__RBIT(active_mask)));
      }
      else {
        /* It is in privileged mode so it will check for pending VRQs
           while exiting the syscall. Just trying to wake up the thread
           in case it is waiting for VRQs.*/
        chThdResumeS(&sbp->vrq_trp, MSG_OK);
      }
    }
  }
}

/**
 * @brief   Triggers VRQs on the specified sandbox.
 * @note    This function must be called from IRQ context because
 *          it manipulates exception stack frames.
 *
 * @param[in] sbp       pointer to a @p sb_class_t structure
 * @param[in] nvrq      number of VRQ to be activated
 *
 * @iclass
 */
void sbVRQTriggerI(sb_class_t *sbp, sb_vrqnum_t nvrq) {

  chSysLockFromISR();

  /* Adding VRQ mask to the pending mask.*/
  sbp->vrq_wtmask |= (sb_vrqmask_t)(1U << nvrq);

  /* Only doing the following if VRQs are globally enabled.*/
  if ((sbp->vrq_isr & SB_VRQ_ISR_DISABLED) == 0U) {
    sb_vrqmask_t active_mask;

    /* Checking if there are VRQs to be served immediately.*/
    active_mask = sbp->vrq_wtmask & sbp->vrq_enmask;
    if (active_mask != 0U) {

      /* Checking if it happened to preempt this sandbox thread.*/
      if (sbp->tp->state == CH_STATE_CURRENT) {
        /* Checking if it is running in unprivileged mode, in this case we
           need to build a return context in its current PSP.*/
        if ((__get_CONTROL() & 1U) != 0U) {

          /* Creating a return context.*/
          vrq_pushctx((struct port_extctx *)__get_PSP(), sbp, __CLZ(__RBIT(active_mask)));
        }
        else {
          /* It is in privileged mode so it will check for pending VRQs
             while exiting the syscall. Just trying to wake up the thread
             in case it is waiting for VRQs.*/
          chThdResumeI(&sbp->vrq_trp, MSG_OK);
        }
      }
      else {
        /* We preempted some other thread. In this case the privilege
           information is stored in the internal thread context because
           it is switched-out.*/
        if ((sbp->tp->ctx.regs.control & 1U) != 0U) {
          struct port_extctx *ectxp;

          /* Getting the current PSP, it is stored in the thread context.*/
          ectxp = (struct port_extctx *)sbp->tp->ctx.syscall.u_psp;

          /* Creating a return context.*/
          ectxp = vrq_writectx(ectxp, sbp, __CLZ(__RBIT(active_mask)));

          /* Updating stored PSP position.*/
          sbp->tp->ctx.syscall.u_psp = (uint32_t)ectxp;
        }
        else {
          /* It is in privileged mode so it will check for pending VRQs
             while exiting the syscall. Just trying to wake up the thread
             in case it is waiting for VRQs.*/
          chThdResumeI(&sbp->vrq_trp, MSG_OK);
        }
      }
    }
  }

  chSysUnlockFromISR();
}

void sb_sysc_vrq_set_alarm(struct port_extctx *ectxp) {
  sb_class_t *sbp = (sb_class_t *)chThdGetSelfX()->ctx.syscall.p;
  sysinterval_t interval = (sysinterval_t )ectxp->r0;
  bool reload = (bool)ectxp->r1;

  if (reload) {
    chVTSetContinuous(&sbp->alarm_vt, interval, delay_cb, (void *)sbp);
  }
  else {
    chVTSet(&sbp->alarm_vt, interval, delay_cb, (void *)sbp);
  }
}

void sb_sysc_vrq_reset_alarm(struct port_extctx *ectxp) {
  sb_class_t *sbp = (sb_class_t *)chThdGetSelfX()->ctx.syscall.p;

  (void)ectxp;

  chVTReset(&sbp->alarm_vt);
}

void sb_sysc_vrq_wait(struct port_extctx *ectxp) {
  sb_class_t *sbp = (sb_class_t *)chThdGetSelfX()->ctx.syscall.p;
  sb_vrqmask_t active_mask;

  (void)ectxp;

  chSysLock();

  active_mask = sbp->vrq_wtmask & sbp->vrq_enmask;
  if (active_mask == 0U) {
    chThdSuspendS(&sbp->vrq_trp);
  }

  chSysUnlock();
}

void sb_fastc_vrq_gcsts(struct port_extctx *ectxp) {
  sb_class_t *sbp = (sb_class_t *)chThdGetSelfX()->ctx.syscall.p;
  uint32_t sts;

  sts = sbp->vrq_flags[ectxp->r0] & ectxp->r1;
  sbp->vrq_flags[ectxp->r0] &= ~sts;
  ectxp->r0 = sts;
}

void sb_fastc_vrq_setwt(struct port_extctx *ectxp) {
  sb_class_t *sbp = (sb_class_t *)chThdGetSelfX()->ctx.syscall.p;
  uint32_t m;

  m = ectxp->r0;
  ectxp->r0 = sbp->vrq_wtmask;
  sbp->vrq_wtmask |= m;

  __sb_vrq_check_pending(ectxp, sbp);
}

void sb_fastc_vrq_clrwt(struct port_extctx *ectxp) {
  sb_class_t *sbp = (sb_class_t *)chThdGetSelfX()->ctx.syscall.p;
  uint32_t m;

  m = ectxp->r0;
  ectxp->r0 = sbp->vrq_wtmask;
  sbp->vrq_wtmask &= ~m;

  /* No need to check for pending VRQs.*/
}

void sb_fastc_vrq_seten(struct port_extctx *ectxp) {
  sb_class_t *sbp = (sb_class_t *)chThdGetSelfX()->ctx.syscall.p;
  uint32_t m;

  m = ectxp->r0;
  ectxp->r0 = sbp->vrq_enmask;
  sbp->vrq_enmask |= m;

  __sb_vrq_check_pending(ectxp, sbp);
}

void sb_fastc_vrq_clren(struct port_extctx *ectxp) {
  sb_class_t *sbp = (sb_class_t *)chThdGetSelfX()->ctx.syscall.p;
  uint32_t m;

  m = ectxp->r0;
  ectxp->r0 = sbp->vrq_enmask;
  sbp->vrq_enmask &= ~m;

  /* No need to check for pending VRQs.*/
}

void sb_fastc_vrq_disable(struct port_extctx *ectxp) {
  sb_class_t *sbp = (sb_class_t *)chThdGetSelfX()->ctx.syscall.p;

  (void)ectxp;

  sbp->vrq_isr = SB_VRQ_ISR_DISABLED;

  /* No need to check for pending VRQs.*/
}

void sb_fastc_vrq_enable(struct port_extctx *ectxp) {
  sb_class_t *sbp = (sb_class_t *)chThdGetSelfX()->ctx.syscall.p;
  sb_vrqmask_t active_mask;

  active_mask = sbp->vrq_wtmask & sbp->vrq_enmask;
  if (active_mask != 0U) {
    sbp->vrq_isr = 0U;

    /* Creating a return context.*/
    vrq_pushctx(ectxp, sbp, __CLZ(__RBIT(active_mask)));
  }
  else {
    sbp->vrq_isr = 0U;
  }
}

void sb_fastc_vrq_getisr(struct port_extctx *ectxp) {
  sb_class_t *sbp = (sb_class_t *)chThdGetSelfX()->ctx.syscall.p;

  ectxp->r0 = sbp->vrq_isr;

  /* No need to check for pending VRQs.*/
}

void sb_fastc_vrq_return(struct port_extctx *ectxp) {
  sb_class_t *sbp = (sb_class_t *)chThdGetSelfX()->ctx.syscall.p;
  sb_vrqmask_t active_mask;

  /* Discarding the return current context, returning on the previous one.
     TODO: Check for overflows????*/
  ectxp++;

  active_mask = sbp->vrq_wtmask & sbp->vrq_enmask;
  if (active_mask != 0U) {

    /* Re-enabling VRQs globally.*/
    sbp->vrq_isr = 0U;

    /* Creating a new return context.*/
    vrq_pushctx(ectxp, sbp, __CLZ(__RBIT(active_mask)));
  }
  else {

    /* Re-enabling VRQs globally.*/
    sbp->vrq_isr = 0U;

    /* Keeping the current return context.*/
    __set_PSP((uint32_t)ectxp);
  }
}

/**
 * @brief   Checks for pending VRQs, creates a return context if any.
 *
 * @param[in] ectxp     current return context
 * @param[in] sbp       pointer to a @p sb_class_t structure
 *
 * @notapi
 */
void __sb_vrq_check_pending(struct port_extctx *ectxp, sb_class_t *sbp) {

  /* Processing pending VRQs if enabled.*/
  if (((sbp->vrq_isr & SB_VRQ_ISR_DISABLED) == 0U)) {
    sb_vrqmask_t active_mask;

    active_mask = sbp->vrq_wtmask & sbp->vrq_enmask;
    if (active_mask != 0U) {

      /* Creating a return context.*/
      ectxp = vrq_writectx(ectxp, sbp, __CLZ(__RBIT(active_mask)));
    }
  }

  __set_PSP((uint32_t)ectxp);
}

#endif /* SB_CFG_ENABLE_VRQ == TRUE */

/** @} */
