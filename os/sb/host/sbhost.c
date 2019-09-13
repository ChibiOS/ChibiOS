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
 * @brief   ARMv7-M sandbox code.
 *
 * @addtogroup ARMV7M_SANDBOX
 * @{
 */

#include "ch.h"
#include "sbhost.h"

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

  /* The header is conventionally placed at base of region zero.*/
  sbhp = (const sb_header_t *)config->r0_base;

  /* Checking header magic numbers.*/
  if ((sbhp->hdr_magic1 != SB_MAGIC1) || (sbhp->hdr_magic2 != SB_MAGIC2)) {
    return;
  }

  /* Checking header size and alignment.*/
  if (sbhp->hdr_size != sizeof (sb_header_t)) {
    return;
  }

  /* Checking regions, applet regions and sandbox regions must match.*/
  if ((sbhp->r0_base != config->r0_base) || (sbhp->r0_end != config->r0_end) ||
      (sbhp->r1_base != config->r1_base) || (sbhp->r1_end != config->r1_end)) {
    return;
  }

  /* PC initial address, by convention it is immediately after the header.*/
  pc = (sbhp->r0_base + sbhp->hdr_size) | 1U;

  /* PSP initial address, it is placed at end of the last region.*/
  if (config->r1_base == 0U) {
    /* Must be in region 1.*/
    psp = config->r0_end;
  }
  else {
    /* Must be in region 2.*/
    psp = config->r1_end;
  }

  /* Additional context information.*/
  sbcp->config = config;
  chThdGetSelfX()->ctx.syscall.p    = (const void *)sbcp;
  chThdGetSelfX()->ctx.syscall.psp  = __get_PSP();

  /* Jumping to the unprivileged code.*/
  port_unprivileged_jump(pc, psp);

  /* Must never happen condition.*/
  chSysHalt("returned");
}

/** @} */
