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
static inline struct port_extctx *vrq_makectx(sb_class_t *sbp,
                                              struct port_extctx *ectxp,
                                              sb_vrqnum_t nvrq) {
  struct port_extctx *newctxp = ectxp - 1;  /* TODO: Test FPCA, it could be short or long.*/

  /* Creating a stack frame for the VRQ.*/
  newctxp = ectxp - 1;

  /* Clearing served VRQ.*/
  sbp->vrq_wtmask &= ~(1U << nvrq);

  /* Disabling VRQs globally during processing.*/
  sbp->vrq_isr = SB_VRQ_ISR_DISABLED;

  /* Building the return context.*/
  newctxp->r0     = nvrq;
#if 0
  newctxp->r1     = 0U;
  newctxp->r2     = 0U;
  newctxp->r3     = 0U;
  newctxp->r12    = 0U;
  newctxp->lr_thd = 0U;
#endif
  newctxp->pc     = sbp->sbhp->hdr_vrq;
  newctxp->xpsr   = 0x01000000U;
#if CORTEX_USE_FPU == TRUE
  newctxp->fpscr  = FPU->FPDSCR;
#endif

  return newctxp;
}

CC_NO_INLINE
static struct port_extctx *vrq_writectx(sb_class_t *sbp,
                                        struct port_extctx *ectxp,
                                        sb_vrqnum_t nvrq) {

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
    ectxp = vrq_makectx(sbp, ectxp, nvrq);
  }

  return ectxp;
}

/* Note, this function may look an useless duplication of vrq_writectx() but
   the included __set_PSP() makes it a viable tail-call candidate for the
   compiler, this is a significant performance gain in several places.*/
CC_NO_INLINE
static void vrq_pushctx(sb_class_t *sbp,
                        struct port_extctx *ectxp,
                        sb_vrqnum_t nvrq) {

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
    ectxp = vrq_makectx(sbp, ectxp, nvrq);
    __set_PSP((uint32_t)ectxp);
#if PORT_SAVE_PSPLIM
    __set_PSPLIM(sbp->u_psplim);
#endif
  }
}

CC_NO_INLINE
static void vrq_fastc_check_pending(struct port_extctx *ectxp, sb_class_t *sbp) {

  /* Processing pending VRQs if enabled.*/
  if (((sbp->vrq_isr & SB_VRQ_ISR_DISABLED) == 0U)) {
    sb_vrqmask_t active_mask;

    active_mask = sbp->vrq_wtmask & sbp->vrq_enmask;
    if (active_mask != 0U) {

      /* Creating a return context.*/
      vrq_pushctx(sbp, ectxp, __CLZ(__RBIT(active_mask)));
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

  sbp->vrq_flags[nvrq] |= flags;
}

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

  chDbgAssert(sbp->thread.state != CH_STATE_CURRENT, "it is current");

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
        vrq_pushctx(sbp, (struct port_extctx *)__get_PSP(), __CLZ(__RBIT(active_mask)));
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

  /* Adding VRQ mask to the pending mask.*/
  sbp->vrq_wtmask |= (sb_vrqmask_t)(1U << nvrq);

  /* Only doing the following if VRQs are globally enabled.*/
  if ((sbp->vrq_isr & SB_VRQ_ISR_DISABLED) == 0U) {
    sb_vrqmask_t active_mask;

    /* Checking if there are VRQs to be served immediately.*/
    active_mask = sbp->vrq_wtmask & sbp->vrq_enmask;
    if (active_mask != 0U) {

      /* Checking if it happened to preempt this sandbox thread.*/
      if (sbp->thread.state == CH_STATE_CURRENT) {
        /* Checking if it is running in unprivileged mode, in this case we
           need to build a return context in its current PSP.*/
        if ((__get_CONTROL() & 1U) != 0U) {

          /* Creating a return context.*/
          vrq_pushctx(sbp, (struct port_extctx *)__get_PSP(), __CLZ(__RBIT(active_mask)));
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
        if ((sbp->thread.ctx.regs.control & 1U) != 0U) {
          struct port_extctx *ectxp;

          /* Getting the current PSP, it is stored in the thread context.*/
          ectxp = (struct port_extctx *)sbp->u_psp;

          /* Creating a return context.*/
          ectxp = vrq_writectx(sbp, ectxp, __CLZ(__RBIT(active_mask)));

          /* Updating stored PSP position.*/
          sbp->u_psp = (uint32_t)ectxp;
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
}

void sb_sysc_vrq_set_alarm(sb_class_t *sbp, struct port_extctx *ectxp) {
  sysinterval_t interval = (sysinterval_t )ectxp->r0;
  bool reload = (bool)ectxp->r1;

  if (reload) {
    chVTSetContinuous(&sbp->alarm_vt, interval, delay_cb, (void *)sbp);
  }
  else {
    chVTSet(&sbp->alarm_vt, interval, delay_cb, (void *)sbp);
  }
}

void sb_sysc_vrq_reset_alarm(sb_class_t *sbp, struct port_extctx *ectxp) {

  (void)ectxp;

  chVTReset(&sbp->alarm_vt);
}

void sb_sysc_vrq_wait(sb_class_t *sbp, struct port_extctx *ectxp) {
  sb_vrqmask_t active_mask;

  (void)ectxp;

  chSysLock();

  active_mask = sbp->vrq_wtmask & sbp->vrq_enmask;
  if (active_mask == 0U) {
    chThdSuspendS(&sbp->vrq_trp);
  }

  chSysUnlock();
}

void sb_fastc_vrq_gcsts(sb_class_t *sbp, struct port_extctx *ectxp) {
  uint32_t nvrq = ectxp->r0;

  /* Cast because vrq_flags[] could be configured to be a smaller type.*/
  ectxp->r0 = (uint32_t)sbp->vrq_flags[nvrq];
  sbp->vrq_flags[nvrq] = 0U;

  /* No need to check for pending VRQs.*/
}

void sb_fastc_vrq_setwt(sb_class_t *sbp, struct port_extctx *ectxp) {
  uint32_t m;

  m = ectxp->r0;
  ectxp->r0 = sbp->vrq_wtmask;
  sbp->vrq_wtmask |= m;

  vrq_fastc_check_pending(ectxp, sbp);
}

void sb_fastc_vrq_clrwt(sb_class_t *sbp, struct port_extctx *ectxp) {
  uint32_t m;

  m = ectxp->r0;
  ectxp->r0 = sbp->vrq_wtmask;
  sbp->vrq_wtmask &= ~m;

  /* No need to check for pending VRQs.*/
}

void sb_fastc_vrq_seten(sb_class_t *sbp, struct port_extctx *ectxp) {
  uint32_t m;

  m = ectxp->r0;
  ectxp->r0 = sbp->vrq_enmask;
  sbp->vrq_enmask |= m;

  vrq_fastc_check_pending(ectxp, sbp);
}

void sb_fastc_vrq_clren(sb_class_t *sbp, struct port_extctx *ectxp) {
  uint32_t m;

  m = ectxp->r0;
  ectxp->r0 = sbp->vrq_enmask;
  sbp->vrq_enmask &= ~m;

  /* No need to check for pending VRQs.*/
}

void sb_fastc_vrq_disable(sb_class_t *sbp, struct port_extctx *ectxp) {

  (void)ectxp;

  sbp->vrq_isr = SB_VRQ_ISR_DISABLED;

  /* No need to check for pending VRQs.*/
}

void sb_fastc_vrq_enable(sb_class_t *sbp, struct port_extctx *ectxp) {
  sb_vrqmask_t active_mask;

  active_mask = sbp->vrq_wtmask & sbp->vrq_enmask;
  if (active_mask != 0U) {
    sbp->vrq_isr = 0U;

    /* Creating a return context.*/
    vrq_pushctx(sbp, ectxp, __CLZ(__RBIT(active_mask)));
  }
  else {
    sbp->vrq_isr = 0U;
  }
}

void sb_fastc_vrq_getisr(sb_class_t *sbp, struct port_extctx *ectxp) {

  ectxp->r0 = sbp->vrq_isr;

  /* No need to check for pending VRQs.*/
}

void sb_fastc_vrq_return(sb_class_t *sbp, struct port_extctx *ectxp) {
  sb_vrqmask_t active_mask;

  /* Discarding the return current context, returning on the previous one.
     TODO: Check for overflows????*/
  ectxp++;

  active_mask = sbp->vrq_wtmask & sbp->vrq_enmask;
  if (active_mask != 0U) {

    /* Re-enabling VRQs globally.*/
//    sbp->vrq_isr = 0U; /* TODO interrupts should not be re-enabled, we are chaining here.*/

    /* Creating a new return context.*/
    vrq_pushctx(sbp, ectxp, __CLZ(__RBIT(active_mask)));
  }
  else {

    /* Re-enabling VRQs globally.*/
    sbp->vrq_isr = 0U;

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
  if (((sbp->vrq_isr & SB_VRQ_ISR_DISABLED) == 0U)) {
    sb_vrqmask_t active_mask;

    active_mask = sbp->vrq_wtmask & sbp->vrq_enmask;
    if (active_mask != 0U) {

      /* Creating a return context.*/
      vrq_pushctx(sbp, ectxp, __CLZ(__RBIT(active_mask)));
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
