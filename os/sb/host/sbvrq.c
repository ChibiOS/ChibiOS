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

__STATIC_FORCEINLINE void vrq_makectx(sb_class_t *sbp,
                                      struct port_extctx *newctxp,
                                      uint32_t active_mask) {
  uint32_t irqn = __CLZ(__RBIT(active_mask));
  sbp->vrq_wtmask &= ~(1U << irqn);

  /* Disabling VRQs globally during processing.*/
  sbp->vrq_isr |= SB_VRQ_ISR_DISABLED;

  /* Building the return context.*/
  newctxp->r0     = irqn;
  newctxp->pc     = sbp->sbhp->hdr_vrq;
  newctxp->xpsr   = 0x01000000U;
#if CORTEX_USE_FPU == TRUE
  newctxp->fpscr  = FPU->FPDSCR;
#endif
}

static void vrq_check_trigger_s(sb_class_t *sbp, struct port_extctx *ectxp) {

  /* Triggering the VRQ if required.*/
  if ((sbp->vrq_isr & SB_VRQ_ISR_DISABLED) == 0U) {
    sb_vrqmask_t active_mask = sbp->vrq_wtmask & sbp->vrq_enmask;

    if (active_mask != 0U) {
      /* Creating a context for return.*/
      ectxp--;

      /* Checking if the new frame is within the sandbox else failure.*/
      if (!sb_is_valid_write_range(sbp,
                                   (void *)ectxp,
                                   sizeof (struct port_extctx))) {
        chSysUnlock();
        __sb_abort(CH_RET_EFAULT);
      }

      /* Building the return context.*/
      vrq_makectx(sbp, ectxp, active_mask);
      chThdResumeS(&sbp->vrq_trp, MSG_OK);
      __port_syscall_set_u_psp(sbp->tp, ectxp);
    }
  }
}

/**
 * @brief   Used as a known privileged address.
 */
static void vrq_privileged_code(void) {

  while (true) {
  }
}

static void delay_cb(virtual_timer_t *vtp, void *arg) {
  sb_class_t *sbp = (sb_class_t *)arg;

  (void)vtp;

  sbVRQTriggerFromISR(sbp, 1U << SB_CFG_ALARM_VRQ);
}

/*===========================================================================*/
/* Module exported functions.                                                */
/*===========================================================================*/

/**
 * @brief   Triggers VRQs on the specified sandbox.
 *
 * @param[in] sbp       pointer to a @p sb_class_t structure
 * @param[in] vmask     mask of VRQs to be activated
 *
 * @sclass
 */
void sbVRQTriggerS(sb_class_t *sbp, sb_vrqmask_t vmask) {

  chDbgCheckClassS();

  chDbgAssert(sbp->tp->state != CH_STATE_CURRENT, "current");

  /* Adding VRQ mask to the pending mask.*/
  sbp->vrq_wtmask |= vmask;

  /* Triggering the VRQ if required.*/
  if ((sbp->vrq_isr & SB_VRQ_ISR_DISABLED) == 0U) {
    sb_vrqmask_t active_mask = sbp->vrq_wtmask & sbp->vrq_enmask;

    if (active_mask != 0U) {
      struct port_extctx *ectxp, *newctxp;
      /* Getting the pointer from the context switch structure.*/
      ectxp   = sbp->tp->ctx.sp;
      newctxp = ectxp - 1;

      /* Checking if the new frame is within the sandbox else failure.*/
      if (!sb_is_valid_write_range(sbp,
                                   (void *)newctxp,
                                   sizeof (struct port_extctx))) {
        /* Making the sandbox return on a privileged address, this
           will cause a fault and sandbox termination.*/
        ectxp->pc = (uint32_t)vrq_privileged_code;
        return;
      }

      /* Building the return context.*/
      vrq_makectx(sbp, newctxp, active_mask);
      __port_syscall_set_u_psp(sbp->tp, newctxp);

      chThdResumeS(&sbp->vrq_trp, MSG_OK);
    }
  }
}

/**
 * @brief   Triggers VRQs on the specified sandbox.
 * @note    This function must be called from IRQ context because
 *          it manipulates exception stack frames.
 *
 * @param[in] sbp       pointer to a @p sb_class_t structure
 * @param[in] vmask     mask of VRQs to be activated
 *
 * @special
 */
void sbVRQTriggerFromISR(sb_class_t *sbp, sb_vrqmask_t vmask) {

  chSysLockFromISR();

  /* Adding VRQ mask to the pending mask.*/
  sbp->vrq_wtmask |= vmask;

  /* Triggering the VRQ if required.*/
  if ((sbp->vrq_isr & SB_VRQ_ISR_DISABLED) == 0U) {
    sb_vrqmask_t active_mask = sbp->vrq_wtmask & sbp->vrq_enmask;

    if (active_mask != 0U) {
      struct port_extctx *ectxp, *newctxp;

      /* A special case is when the IRQ preempted the sandbox while running
         in unprivileged mode (not processing a syscall).*/
      if ((sbp->tp->state == CH_STATE_CURRENT) &&
          ((__get_CONTROL() & 1U) != 0U)) {
        /* Getting the current PSP, it is U_PSP.*/
        ectxp = (struct port_extctx *)__get_PSP();

        /* Creating new context for the VRQ.*/
        newctxp = ectxp - 1;

        /* Checking if the new frame is within the sandbox else failure.*/
        if (!sb_is_valid_write_range(sbp,
                                     (void *)newctxp,
                                     sizeof (struct port_extctx))) {
          /* Making the sandbox return on a privileged address, this
             will cause a fault and sandbox termination.*/
          chSysUnlockFromISR();
          ectxp->pc = (uint32_t)vrq_privileged_code;
          return;
        }

        /* Building the return context.*/
        vrq_makectx(sbp, newctxp, active_mask);
        __set_PSP((uint32_t)newctxp);
      }
      else {
        /* Getting the stored U_PSP value.*/
        ectxp = (struct port_extctx *)__port_syscall_get_u_psp(sbp->tp);

        /* Creating new context for the VRQ.*/
        newctxp = ectxp - 1;

        /* Checking if the new frame is within the sandbox else failure.*/
        if (!sb_is_valid_write_range(sbp,
                                     (void *)newctxp,
                                     sizeof (struct port_extctx))) {
          /* Making the sandbox return on a privileged address, this
             will cause a fault and sandbox termination.*/
          chSysUnlockFromISR();
          ectxp->pc = (uint32_t)vrq_privileged_code;
          return;
        }

        /* Building the return context then waking up the sandbox thread.*/
        vrq_makectx(sbp, newctxp, active_mask);
        __port_syscall_set_u_psp(sbp->tp, newctxp);

        chThdResumeI(&sbp->vrq_trp, MSG_OK);
      }
    }
  }

  chSysUnlockFromISR();
}

void sb_api_vrq_set_alarm(struct port_extctx *ectxp) {
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

void sb_api_vrq_reset_alarm(struct port_extctx *ectxp) {
  sb_class_t *sbp = (sb_class_t *)chThdGetSelfX()->ctx.syscall.p;

  (void)ectxp;

  chVTReset(&sbp->alarm_vt);
}

void sb_api_vrq_wait(struct port_extctx *ectxp) {
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

void sb_api_vrq_setwt(struct port_extctx *ectxp) {
  sb_class_t *sbp = (sb_class_t *)chThdGetSelfX()->ctx.syscall.p;
  uint32_t m;

  chSysLock();

  m = ectxp->r0;
  ectxp->r0 = sbp->vrq_wtmask;
  sbp->vrq_wtmask |= m;

  vrq_check_trigger_s(sbp, ectxp);

  chSysUnlock();
}

void sb_api_vrq_clrwt(struct port_extctx *ectxp) {
  sb_class_t *sbp = (sb_class_t *)chThdGetSelfX()->ctx.syscall.p;
  uint32_t m;

  chSysLock();

  m = ectxp->r0;
  ectxp->r0 = sbp->vrq_wtmask;
  sbp->vrq_wtmask &= ~m;

  chSysUnlock();
}

void sb_api_vrq_seten(struct port_extctx *ectxp) {
  sb_class_t *sbp = (sb_class_t *)chThdGetSelfX()->ctx.syscall.p;
  uint32_t m;

  chSysLock();

  m = ectxp->r0;
  ectxp->r0 = sbp->vrq_enmask;
  sbp->vrq_enmask |= m;

  vrq_check_trigger_s(sbp, ectxp);

  chSysUnlock();
}

void sb_api_vrq_clren(struct port_extctx *ectxp) {
  sb_class_t *sbp = (sb_class_t *)chThdGetSelfX()->ctx.syscall.p;
  uint32_t m;

  chSysLock();

  m = ectxp->r0;
  ectxp->r0 = sbp->vrq_enmask;
  sbp->vrq_enmask &= ~m;

  chSysUnlock();
}

void sb_api_vrq_disable(struct port_extctx *ectxp) {
  sb_class_t *sbp = (sb_class_t *)chThdGetSelfX()->ctx.syscall.p;

  chSysLock();

  ectxp->r0 = sbp->vrq_isr;
  sbp->vrq_isr |= SB_VRQ_ISR_DISABLED;

  chSysUnlock();
}

void sb_api_vrq_enable(struct port_extctx *ectxp) {
  sb_class_t *sbp = (sb_class_t *)chThdGetSelfX()->ctx.syscall.p;

  chSysLock();

  ectxp->r0 = sbp->vrq_isr;
  sbp->vrq_isr &= ~SB_VRQ_ISR_DISABLED;

  vrq_check_trigger_s(sbp, ectxp);

  chSysUnlock();
}

void sb_api_vrq_getisr(struct port_extctx *ectxp) {
  sb_class_t *sbp = (sb_class_t *)chThdGetSelfX()->ctx.syscall.p;

  ectxp->r0 = sbp->vrq_isr;
}

void sb_api_vrq_return(struct port_extctx *ectxp) {
  sb_class_t *sbp = (sb_class_t *)chThdGetSelfX()->ctx.syscall.p;
  sb_vrqmask_t active_mask;

  chSysLock();

  /* VRQs must be disabled on return, sanity check.*/
  if (((sbp->vrq_isr & SB_VRQ_ISR_DISABLED) == 0U)) {
    __sb_abort(CH_RET_EFAULT);
  }

  /* Re-triggering the VRQ if required.*/
  active_mask = sbp->vrq_wtmask & sbp->vrq_enmask;
  if (active_mask != 0U) {
    /* Building the return context, reusing the current context structure.*/
    vrq_makectx(sbp, ectxp, active_mask);
  }
  else {
    /* Returning from VRQ.*/
    sbp->vrq_isr &= ~SB_VRQ_ISR_DISABLED;

    /* Discarding the return current context, returning on the previous one.*/
    ectxp++;
  }

  __port_syscall_set_u_psp(sbp->tp, ectxp);

  chSysUnlock();
}

#endif /* SB_CFG_ENABLE_VRQ == TRUE */

/** @} */
