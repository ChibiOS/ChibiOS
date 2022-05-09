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

#include <string.h>

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

__STATIC_FORCEINLINE void vfq_makectx(sb_class_t *sbp,
                                      struct port_extctx *ectxp,
                                      uint32_t active_mask) {
  uint32_t irqn = __CLZ(active_mask);
  sbp->vrq_wtmask &= ~(1U << irqn);

  /* Building the return context.*/
  ectxp->r0     = irqn;
  ectxp->pc     = sbp->sbhp->hdr_vfq; /* TODO validate or let it eventually crash? */
  ectxp->xpsr   = 0x01000000U;
#if CORTEX_USE_FPU == TRUE
  ectxp->fpscr  = FPU->FPDSCR;
#endif
}

/*===========================================================================*/
/* Module exported functions.                                                */
/*===========================================================================*/

/**
 * @brief   Triggers VRQs on the specified sandbox.
 * @note    This function must be called from IRQ context because
 *          it manipulates exception stack frames.
 *
 * @param[in] sbp       pointer to a @p sb_class_t structure
 * @param[in] vmask     mask of VRQs to be activated
 * @return              The operation status.
 * @retval false        if the activation has succeeded.
 * @retval true         in case of sandbox stack overflow.
 *
 * @special
 */
bool sbVRQTriggerFromISR(sb_class_t *sbp, sb_vrqmask_t vmask) {
  struct port_extctx *ectxp;

  chSysLockFromISR();

  /* Adding VRQ mask to the pending mask.*/
  sbp->vrq_wtmask |= vmask;

  /* Triggering the VRQ if required.*/
  if (sbp->vrq_isr == 0U) {
    sb_vrqmask_t active_mask = sbp->vrq_wtmask & sbp->vrq_enmask;

    if (active_mask != 0U) {

      /* This IRQ could have preempted the sandbox itself or some other thread,
         handling is different.*/
      if (sbp->tp->state == CH_STATE_CURRENT) {
        /* Sandbox case, getting the current exception frame.*/
        ectxp = (struct port_extctx *)__get_PSP() - 1;

        /* Checking if the new frame is within the sandbox else failure.*/
        if (!sb_is_valid_write_range(sbp,
                                     (void *)ectxp,
                                     sizeof (struct port_extctx))) {
          chSysUnlockFromISR();

          return true;
        }
      }
      else {
        ectxp = sbp->tp->ctx.sp - 1;

        /* Checking if the new frame is within the sandbox else failure.*/
        if (!sb_is_valid_write_range(sbp,
                                     (void *)ectxp,
                                     sizeof (struct port_extctx))) {
          chSysUnlockFromISR();

          return true;
        }

        /* Preventing leakage of information, clearing all register values, those
           would come from outside the sandbox.*/
        memset((void *)ectxp, 0, sizeof (struct port_extctx));
      }

      /* Building the return context.*/
      vfq_makectx(sbp, ectxp, active_mask);
    }
  }

  chSysUnlockFromISR();

  return false;
}

void sb_vrq_disable(struct port_extctx *ectxp) {
  sb_class_t *sbp = (sb_class_t *)chThdGetSelfX()->ctx.syscall.p;

  ectxp->r0 = sbp->vrq_isr;
  sbp->vrq_isr |= SB_VRQ_ISR_DISABLED;
}

void sb_vrq_enable(struct port_extctx *ectxp) {
  sb_class_t *sbp = (sb_class_t *)chThdGetSelfX()->ctx.syscall.p;

  ectxp->r0 = sbp->vrq_isr;
  sbp->vrq_isr &= ~SB_VRQ_ISR_DISABLED;

  /* Re-triggering the VRQ if required.*/
  if (sbp->vrq_isr == 0U) {
    sb_vrqmask_t active_mask = sbp->vrq_wtmask & sbp->vrq_enmask;

    if (active_mask != 0U) {
      /* Creating a context for return.*/
      ectxp--;

      /* Checking if the new frame is within the sandbox else failure.*/
      if (!sb_is_valid_write_range(sbp,
                                   (void *)ectxp,
                                   sizeof (struct port_extctx))) {
        __sb_abort(CH_RET_EFAULT);
      }

      /* Building the return context.*/
      vfq_makectx(sbp, ectxp, active_mask);
    }
  }
}

void sb_vrq_getisr(struct port_extctx *ectxp) {
  sb_class_t *sbp = (sb_class_t *)chThdGetSelfX()->ctx.syscall.p;

  ectxp->r0 = sbp->vrq_isr;
}

void sb_vrq_return(struct port_extctx *ectxp) {
  sb_class_t *sbp = (sb_class_t *)chThdGetSelfX()->ctx.syscall.p;

  if (((sbp->vrq_isr & SB_VRQ_ISR_IRQMODE) == 0U)) {
    __sb_abort(CH_RET_EFAULT);
  }

  /* Re-triggering the VRQ if required.*/
  if (sbp->vrq_isr == 0U) {
    sb_vrqmask_t active_mask = sbp->vrq_wtmask & sbp->vrq_enmask;

    if (active_mask != 0U) {
      /* Building the return context, reusing the current context structure.*/
      vfq_makectx(sbp, ectxp, active_mask);
    }
  }
  else {
    /* Ending IRQ mode.*/
    sbp->vrq_isr &= ~SB_VRQ_ISR_IRQMODE;

    /* Discarding the return current context, returning on the previous one.*/
    ectxp++;
  }
}

#endif /* SB_CFG_ENABLE_VRQ == TRUE */

/** @} */
