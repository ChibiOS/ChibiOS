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
 * @file    sb/vhal/sbvio_gpio.c
 * @brief   ARM SandBox host Virtual I/O code.
 *
 * @addtogroup ARM_SANDBOX_HOST_VIO_GPIO
 * @{
 */

#include "sb.h"

#if (VIO_CFG_ENABLE_GPIO == TRUE) || defined(__DOXYGEN__)

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

void sb_api_vio_gpio(struct port_extctx *ectxp) {
  sb_class_t *sbp = (sb_class_t *)chThdGetSelfX()->ctx.syscall.p;
  uint32_t sub = (unsigned)ectxp->r0;
  uint32_t vport = (unsigned)ectxp->r1;
  const vio_port_conf_t *vportp;
  ectxp->r0 = 0U;

  if (vport >= sbp->config->vioconf->gpioconf->n) {
    return;
  }

  vportp = &sbp->config->vioconf->gpioconf->ports[vport];

  switch (sub) {
  case SB_VPAL_WRITE:
    if ((vportp->permissions & VIO_GPIO_PERM_WRITE) != 0U) {
      palWriteGroup(vportp->port, vportp->mask, vportp->offset, ectxp->r2);
    }
    break;
  case SB_VPAL_SET:
    if ((vportp->permissions & VIO_GPIO_PERM_WRITE) != 0U) {
      uint32_t val = palReadGroup(vportp->port, vportp->mask, vportp->offset);
      palWriteGroup(vportp->port, vportp->mask, vportp->offset, ectxp->r2 | val);
    }
    break;
  case SB_VPAL_CLEAR:
    if ((vportp->permissions & VIO_GPIO_PERM_WRITE) != 0U) {
      uint32_t val = palReadGroup(vportp->port, vportp->mask, vportp->offset);
      palWriteGroup(vportp->port, vportp->mask, vportp->offset, ectxp->r2 & ~val);
    }
    break;
  case SB_VPAL_TOGGLE:
    if ((vportp->permissions & VIO_GPIO_PERM_WRITE) != 0U) {
      uint32_t val = palReadGroup(vportp->port, vportp->mask, vportp->offset);
      palWriteGroup(vportp->port, vportp->mask, vportp->offset, ectxp->r2 ^ val);
    }
    break;
  case SB_VPAL_READLATCH:
    if ((vportp->permissions & VIO_GPIO_PERM_WRITE) != 0U) {
      ectxp->r0 = palReadGroupLatch(vportp->port, vportp->mask, vportp->offset);
    }
    break;
  case SB_VPAL_READ:
    if ((vportp->permissions & VIO_GPIO_PERM_READ) != 0U) {
      ectxp->r0 = palReadGroup(vportp->port, vportp->mask, vportp->offset);
    }
    break;
  case SB_VPAL_SETMODE:
    if ((vportp->permissions & VIO_GPIO_PERM_SETMODE) != 0U) {
      /* TODO */
    }
    break;
  default:
    return;
  }
}

#endif /* VIO_CFG_ENABLE_GPIO == TRUE */

/** @} */
