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
  uint32_t sub  = ectxp->r0;
  uint32_t unit = ectxp->r1;
  ectxp->r0 = (uint32_t)CH_RET_INNER_ERROR;
  ectxp->r1 = (uint32_t)0;
  const vio_uart_unit_t *unitp;

  if (unit >= sbp->config->vioconf->uarts->n) {
    ectxp->r0 = (uint32_t)HAL_RET_NO_RESOURCE;
    return;
  }

  unitp = &sbp->config->vioconf->uarts->units[unit];

  switch (sub) {
  case SB_VUART_INIT:
    {
      uint32_t conf = ectxp->r2;
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
  case SB_VUART_ISRXE:
    {
      ectxp->r1 = (uint32_t)sioIsRXEmptyX(unitp->siop);
      ectxp->r0 = (uint32_t)HAL_RET_SUCCESS;
      break;
    }
  case SB_VUART_ISRXI:
    {
      ectxp->r1 = (uint32_t)sioIsRXIdleX(unitp->siop);
      ectxp->r0 = (uint32_t)HAL_RET_SUCCESS;
      break;
    }
  case SB_VUART_ISTXF:
    {
      ectxp->r1 = (uint32_t)sioIsTXFullX(unitp->siop);
      ectxp->r0 = (uint32_t)HAL_RET_SUCCESS;
      break;
    }
  case SB_VUART_ISTXO:
    {
      ectxp->r1 = (uint32_t)sioIsTXOngoingX(unitp->siop);
      ectxp->r0 = (uint32_t)HAL_RET_SUCCESS;
      break;
    }
  case SB_VUART_HASERR:
    {
      ectxp->r1 = (uint32_t)sioHasRXErrorsX(unitp->siop);
      ectxp->r0 = (uint32_t)HAL_RET_SUCCESS;
      break;
    }
  case SB_VUART_READ:
    {
      uint8_t *buffer = (uint8_t *)ectxp->r2;
      size_t n = (size_t)ectxp->r3;

      if (unitp->siop->state != SIO_READY) {
        ectxp->r0 = (uint32_t)CH_RET_EIO;
        break;
      }

      if (!sb_is_valid_write_range(sbp, buffer, n)) {
        ectxp->r0 = (uint32_t)CH_RET_EFAULT;
        break;
      }

      ectxp->r1 = sioAsyncRead(unitp->siop, buffer, n);
      ectxp->r0 = (uint32_t)HAL_RET_SUCCESS;
      break;
    }
  case SB_VUART_WRITE:
    {
      const uint8_t *buffer = (const uint8_t *)ectxp->r2;
      size_t n = (size_t)ectxp->r3;

      if (unitp->siop->state != SIO_READY) {
        ectxp->r0 = (uint32_t)CH_RET_EIO;
        break;
      }

      if (!sb_is_valid_read_range(sbp, buffer, n)) {
        ectxp->r0 = (uint32_t)CH_RET_EFAULT;
        break;
      }

      ectxp->r1 = sioAsyncWrite(unitp->siop, buffer, n);
      ectxp->r0 = (uint32_t)HAL_RET_SUCCESS;
      break;
    }
  case SB_VUART_GET:
    {
      if (unitp->siop->state != SIO_READY) {
        ectxp->r0 = (uint32_t)CH_RET_EIO;
        break;
      }

      ectxp->r1 = sioGetX(unitp->siop);
      ectxp->r0 = (uint32_t)HAL_RET_SUCCESS;
      break;
    }
  case SB_VUART_PUT:
    {
      uint_fast16_t data = (uint_fast16_t)ectxp->r2;

      if (unitp->siop->state != SIO_READY) {
        ectxp->r0 = (uint32_t)CH_RET_EIO;
        break;
      }

      sioPutX(unitp->siop, data);
      ectxp->r0 = (uint32_t)HAL_RET_SUCCESS;
      break;
    }
  case SB_VUART_WREN:
    {
      sioflags_t flags = (sioflags_t)ectxp->r2;

      if (unitp->siop->state != SIO_READY) {
        ectxp->r0 = (uint32_t)CH_RET_EIO;
        break;
      }

      sioWriteEnableFlags(unitp->siop, flags);
      ectxp->r0 = (uint32_t)HAL_RET_SUCCESS;
      break;
    }
  case SB_VUART_GCERR:
    {
      if (unitp->siop->state != SIO_READY) {
        ectxp->r0 = (uint32_t)CH_RET_EIO;
        break;
      }

      ectxp->r1 = (uint32_t)sioGetAndClearErrors(unitp->siop);
      ectxp->r0 = (uint32_t)HAL_RET_SUCCESS;
      break;
    }
  case SB_VUART_GCEVT:
    {
      if (unitp->siop->state != SIO_READY) {
        ectxp->r0 = (uint32_t)CH_RET_EIO;
        break;
      }

      ectxp->r1 = (uint32_t)sioGetAndClearEvents(unitp->siop);
      ectxp->r0 = (uint32_t)HAL_RET_SUCCESS;
      break;
    }
  case SB_VUART_CTL:
    /* falls through */
  default:
    ectxp->r0 = (uint32_t)CH_RET_ENOSYS;
    break;
  }
}

#endif /* VIO_CFG_ENABLE_UART == TRUE */

/** @} */
