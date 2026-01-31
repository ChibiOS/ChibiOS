/*
    ChibiOS - Copyright (C) 2006-2026 Giovanni Di Sirio.

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
 * @brief   ARM SandBox host Virtual GPIO code.
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

void sb_fastc_vio_gpio(sb_class_t *sbp, struct port_extctx *ectxp) {
  uint32_t sub  = VIO_CALL_SUBCODE(ectxp->r0);
  uint32_t unit = VIO_CALL_UNIT(ectxp->r0);
  const vio_gpio_unit_t *unitp;

  /* VIO not associated.*/
  if (sbp->vioconf == NULL) {
    ectxp->r0 = (uint32_t)HAL_RET_NO_RESOURCE;
    return;
  }

  ectxp->r0 = 0U;

  if (unit >= sbp->vioconf->gpios->n) {
    return;
  }

  unitp = &sbp->vioconf->gpios->units[unit];

  switch (sub) {
  case SB_VGPIO_WRITE:
    if ((unitp->permissions & VIO_GPIO_PERM_WRITE) != 0U) {
      palWriteGroup(unitp->port, unitp->mask, unitp->offset, ectxp->r1);
    }
    break;
  case SB_VGPIO_SET:
    if ((unitp->permissions & VIO_GPIO_PERM_WRITE) != 0U) {
      uint32_t val = palReadGroup(unitp->port, unitp->mask, unitp->offset);
      palWriteGroup(unitp->port, unitp->mask, unitp->offset, ectxp->r1 | val);
    }
    break;
  case SB_VGPIO_CLEAR:
    if ((unitp->permissions & VIO_GPIO_PERM_WRITE) != 0U) {
      uint32_t val = palReadGroup(unitp->port, unitp->mask, unitp->offset);
      palWriteGroup(unitp->port, unitp->mask, unitp->offset, ectxp->r1 & ~val);
    }
    break;
  case SB_VGPIO_TOGGLE:
    if ((unitp->permissions & VIO_GPIO_PERM_WRITE) != 0U) {
      uint32_t val = palReadGroup(unitp->port, unitp->mask, unitp->offset);
      palWriteGroup(unitp->port, unitp->mask, unitp->offset, ectxp->r1 ^ val);
    }
    break;
  case SB_VGPIO_READLATCH:
    if ((unitp->permissions & VIO_GPIO_PERM_WRITE) != 0U) {
      ectxp->r0 = palReadGroupLatch(unitp->port, unitp->mask, unitp->offset);
    }
    break;
  case SB_VGPIO_READ:
    if ((unitp->permissions & VIO_GPIO_PERM_READ) != 0U) {
      ectxp->r0 = palReadGroup(unitp->port, unitp->mask, unitp->offset);
    }
    break;
  case SB_VGPIO_SETMODE:
    if ((unitp->permissions & VIO_GPIO_PERM_SETMODE) != 0U) {
      /* TODO */
    }
    break;
  default:
    return;
  }
}

#endif /* VIO_CFG_ENABLE_GPIO == TRUE */

/** @} */
