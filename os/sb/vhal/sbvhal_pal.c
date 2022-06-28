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
 * @file    sb/vhal/sbvhal_pal.c
 * @brief   ARM SandBox host Virtual HAL PAL code.
 *
 * @addtogroup ARM_SANDBOX_HOST_VHAL
 * @{
 */

#include "sb.h"

#if (SB_CFG_ENABLE_VHAL_PAL == TRUE) || defined(__DOXYGEN__)

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

void sb_api_vhal_pal(struct port_extctx *ectxp) {
  sb_class_t *sbp = (sb_class_t *)chThdGetSelfX()->ctx.syscall.p;
  uint32_t sub = (unsigned)ectxp->r0;
  uint32_t vport = (unsigned)ectxp->r1;
  const vhal_vpio_conf_t *vpiop;
  ectxp->r0 = 0U;

  if (vport >= sbp->config->vhalconf->vpalconf->n) {
    return;
  }

  vpiop = &sbp->config->vhalconf->vpalconf->vpio[vport];

  switch (sub) {
  case SB_VPAL_WRITE:
    if ((vpiop->permissions & VPIO_PERM_WRITE) != 0U) {
      palWriteGroup(vpiop->port, vpiop->mask, vpiop->offset, ectxp->r2);
    }
    break;
  case SB_VPAL_SET:
    if ((vpiop->permissions & VPIO_PERM_WRITE) != 0U) {
      uint32_t val = palReadGroup(vpiop->port, vpiop->mask, vpiop->offset);
      palWriteGroup(vpiop->port, vpiop->mask, vpiop->offset, ectxp->r2 | val);
    }
    break;
  case SB_VPAL_CLEAR:
    if ((vpiop->permissions & VPIO_PERM_WRITE) != 0U) {
      uint32_t val = palReadGroup(vpiop->port, vpiop->mask, vpiop->offset);
      palWriteGroup(vpiop->port, vpiop->mask, vpiop->offset, ectxp->r2 & ~val);
    }
    break;
  case SB_VPAL_TOGGLE:
    if ((vpiop->permissions & VPIO_PERM_WRITE) != 0U) {
      uint32_t val = palReadGroup(vpiop->port, vpiop->mask, vpiop->offset);
      palWriteGroup(vpiop->port, vpiop->mask, vpiop->offset, ectxp->r2 ^ val);
    }
    break;
  case SB_VPAL_READLATCH:
    if ((vpiop->permissions & VPIO_PERM_WRITE) != 0U) {
      ectxp->r0 = palReadGroupLatch(vpiop->port, vpiop->mask, vpiop->offset);
    }
    break;
  case SB_VPAL_READ:
    if ((vpiop->permissions & VPIO_PERM_READ) != 0U) {
      ectxp->r0 = palReadGroup(vpiop->port, vpiop->mask, vpiop->offset);
    }
    break;
  case SB_VPAL_SETMODE:
    if ((vpiop->permissions & VPIO_PERM_SETMODE) != 0U) {
      /* TODO */
    }
    break;
  default:
    return;
  }
}

#endif /* SB_CFG_ENABLE_VHAL_PAL == TRUE */

/** @} */
