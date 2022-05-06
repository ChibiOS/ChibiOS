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

/*===========================================================================*/
/* Module exported functions.                                                */
/*===========================================================================*/

/**
 * @brief   Activates VRQs on the specified sandbox.
 *
 * @param[in] sbp       pointer to a @p sb_class_t structure
 * @param[in] vmask     mask of VRQs to be activated
 * @return              The operation status.
 * @retval false        if the activation has succeeded.
 * @retval true         in case of sandbox stack overflow.
 */
bool sbVRQSignalMaskI(sb_class_t *sbp, sb_vrqmask_t vmask) {
  struct port_extctx *ectxp;
  const sb_header_t *sbhp;

  /* This IRQ could have preempted the sandbox itself or some other thread,
     handling is different.*/
  if (sbp->tp->state == CH_STATE_CURRENT) {
    /* Sandbox case, getting the current exception frame.*/
    ectxp = (struct port_extctx *)__get_PSP() - 1;

    /* Checking if the new frame is within the sandbox else failure.*/
    if (!sb_is_valid_write_range(sbp,
                                 (void *)ectxp,
                                 sizeof (struct port_extctx))) {
      return true;
    }
  }
  else {
    ectxp = sbp->tp->ctx.sp - 1;

    /* Checking if the new frame is within the sandbox else failure.*/
    if (!sb_is_valid_write_range(sbp,
                                 (void *)ectxp,
                                 sizeof (struct port_extctx))) {
      return true;
    }

    /* Preventing leakage of information, clearing all register values, those
       would come from outside the sandbox.*/
    memset((void *)ectxp, 0, sizeof (struct port_extctx));
  }

  /* Header location.*/
  sbhp = (const sb_header_t *)(void *)sbp->config->regions[sbp->config->code_region].area.base;

  return false;
}

#endif /* SB_CFG_ENABLE_VRQ == TRUE */

/** @} */
