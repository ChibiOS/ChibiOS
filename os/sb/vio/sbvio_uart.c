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
 * @file    sb/vhal/sbvio_uart.c
 * @brief   ARM SandBox host Virtual UART code.
 *
 * @addtogroup ARM_SANDBOX_HOST_VIO_UART
 * @{
 */

#include "sb.h"

#if (VIO_CFG_ENABLE_UART == TRUE) || defined(__DOXYGEN__)

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

static void vuart_cb(SIODriver *siop) {

  (void)siop;
}

/*===========================================================================*/
/* Module exported functions.                                                */
/*===========================================================================*/

void sb_api_vio_uart(struct port_extctx *ectxp) {
  sb_class_t *sbp = (sb_class_t *)chThdGetSelfX()->ctx.syscall.p;
  ectxp->r0 = (uint32_t)CH_RET_INNER_ERROR;
  uint32_t sub = (unsigned)ectxp->r0;
  uint32_t unit = (unsigned)ectxp->r1;
  const vio_uart_unit_t *unitp;

  if (unit >= sbp->config->vioconf->uarts->n) {
    ectxp->r0 = (uint32_t)HAL_RET_NO_RESOURCE;
    return;
  }

  unitp = &sbp->config->vioconf->uarts->units[unit];

  switch (sub) {
  case SB_VUART_INIT:
    {
      uint32_t conf = sbp->config->vioconf->uartconfs->n;
      const vio_uart_config_t *confp;
      msg_t msg;

      if (conf >= sbp->config->vioconf->uarts->n) {
        ectxp->r0 = (uint32_t)HAL_RET_CONFIG_ERROR;
        return;
      }

      confp = &sbp->config->vioconf->uartconfs->cfgs[conf];

      msg = sioStart(unitp->siop, confp->siocfgp);
      if (msg == HAL_RET_SUCCESS) {
        sioSetCallbackX(unitp->siop, vuart_cb);
      }

      ectxp->r0 = (uint32_t)msg;
      break;
    }
  case SB_VUART_DEINIT:
    {
      sioStop(unitp->siop);

      ectxp->r0 = (uint32_t)HAL_RET_SUCCESS;
      break;
    }
  default:
    return;
  }
}

#endif /* VIO_CFG_ENABLE_UART == TRUE */

/** @} */
