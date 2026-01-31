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

static void vuart_cb(void *ip) {
  hal_sio_driver_c *siop = (hal_sio_driver_c *)ip;
  const vio_uart_unit_t *unitp = (const vio_uart_unit_t *)drvGetArgumentX(siop);

  sbVRQTriggerFromISR(unitp->vrqsb, unitp->vrqn);
}

/*===========================================================================*/
/* Module exported functions.                                                */
/*===========================================================================*/

void sb_sysc_vio_uart(sb_class_t *sbp, struct port_extctx *ectxp) {
  uint32_t sub  = VIO_CALL_SUBCODE(ectxp->r0);
  uint32_t unit = VIO_CALL_UNIT(ectxp->r0);
  ectxp->r0 = (uint32_t)CH_RET_INNER_ERROR;
  const vio_uart_unit_t *unitp;

  /* VIO not associated.*/
  if (sbp->vioconf == NULL) {
    ectxp->r0 = (uint32_t)HAL_RET_NO_RESOURCE;
    return;
  }

  if (unit >= sbp->vioconf->uarts->n) {
    ectxp->r0 = (uint32_t)HAL_RET_NO_RESOURCE;
    return;
  }

  unitp = &sbp->vioconf->uarts->units[unit];

  switch (sub) {
  case SB_VUART_INIT:
    {
      msg_t msg;

      /* Associating this virtual UART to the SIO driver.*/
      drvSetArgumentX(unitp->siop, (void *)unitp);

      msg = drvStart(unitp->siop);
      if (msg == HAL_RET_SUCCESS) {

        /* Starting with disabled events, enabling the callback.*/
        sioWriteEnableFlags(unitp->siop, SIO_EV_NONE);
        drvSetCallbackX(unitp->siop, vuart_cb);
      }

      ectxp->r0 = (uint32_t)msg;
      break;
    }
  case SB_VUART_DEINIT:
    {
      drvStop(unitp->siop);

      ectxp->r0 = (uint32_t)HAL_RET_SUCCESS;
      break;
    }
  default:
    ectxp->r0 = (uint32_t)CH_RET_ENOSYS;
    break;
  }
}

void sb_fastc_vio_uart(sb_class_t *sbp, struct port_extctx *ectxp) {
  uint32_t sub  = VIO_CALL_SUBCODE(ectxp->r0);
  uint32_t unit = VIO_CALL_UNIT(ectxp->r0);
  const vio_uart_unit_t *unitp;

  /* VIO not associated.*/
  if (sbp->vioconf == NULL) {
    ectxp->r0 = (uint32_t)HAL_RET_NO_RESOURCE;
    return;
  }

  /* Returned value in case of error or illegal sub-code.*/
  ectxp->r0 = (uint32_t)-1;

  if (unit >= sbp->vioconf->uarts->n) {
    return;
  }

  unitp = &sbp->vioconf->uarts->units[unit];

  /* We don't want assertion or errors to be caused in host, making sure
     all functions are called in the proper state.*/
  if (unitp->siop->state != HAL_DRV_STATE_READY) {
    return;
  }

  switch (sub) {
  case SB_VUART_SELCFG:
    {
      uint32_t cfgnum = ectxp->r1;
      size_t n = ectxp->r2;
      void *p = (void *)ectxp->r3;
      const void *confp;

      /* Check on configuration index.*/
      if (cfgnum >= sbp->vioconf->uartconfs->cfgsnum) {
        ectxp->r0 = (uint32_t)HAL_RET_CONFIG_ERROR;
        return;
      }

      /* Check on configuration buffer size.*/
      if (n > sizeof (hal_sio_config_t)) {
        ectxp->r0 = (uint32_t)HAL_RET_CONFIG_ERROR;
        return;
      }

      /* Check on configuration buffer area.*/
      if (!sb_is_valid_write_range(sbp, p, n)) {
        ectxp->r0 = (uint32_t)CH_RET_EFAULT;
        /* TODO enforce fault instead.*/
        break;
      }

      /* Specified VUART configuration.*/
      confp = drvSelectCfgX(unitp->siop, cfgnum);

      /* Copying the standard part of the configuration into the sandbox
         space in the specified position.*/
      if (confp != NULL) {
        memcpy(p, confp, n);
        ectxp->r0 = (uint32_t)HAL_RET_SUCCESS;
      }
      else {
        ectxp->r0 = (uint32_t)HAL_RET_CONFIG_ERROR;
      }

      break;
    }
  case SB_VUART_ISRXE:
    {
      ectxp->r0 = (uint32_t)sioIsRXEmptyX(unitp->siop);
      break;
    }
  case SB_VUART_ISRXI:
    {
      ectxp->r0 = (uint32_t)sioIsRXIdleX(unitp->siop);
      break;
    }
  case SB_VUART_ISTXF:
    {
      ectxp->r0 = (uint32_t)sioIsTXFullX(unitp->siop);
      break;
    }
  case SB_VUART_ISTXO:
    {
      ectxp->r0 = (uint32_t)sioIsTXOngoingX(unitp->siop);
      break;
    }
  case SB_VUART_HASERR:
    {
      ectxp->r0 = (uint32_t)sioHasRXErrorsX(unitp->siop);
      break;
    }
  case SB_VUART_READ:
    {
      uint8_t *buffer = (uint8_t *)ectxp->r1;
      size_t n = (size_t)ectxp->r2;

      if (!sb_is_valid_write_range(sbp, buffer, n)) {
        ectxp->r0 = (uint32_t)CH_RET_EFAULT;
        /* TODO enforce fault instead.*/
        break;
      }

      ectxp->r0 = sioAsyncReadX(unitp->siop, buffer, n);
      break;
    }
  case SB_VUART_WRITE:
    {
      const uint8_t *buffer = (const uint8_t *)ectxp->r1;
      size_t n = (size_t)ectxp->r2;

      if (!sb_is_valid_read_range(sbp, buffer, n)) {
        ectxp->r0 = (uint32_t)CH_RET_EFAULT;
        /* TODO enforce fault instead.*/
        break;
      }

      ectxp->r0 = sioAsyncWriteX(unitp->siop, buffer, n);
      break;
    }
  case SB_VUART_GET:
    {
      ectxp->r0 = sioGetX(unitp->siop);
      break;
    }
  case SB_VUART_PUT:
    {
      sioPutX(unitp->siop, (uint_fast16_t)ectxp->r1);
      ectxp->r0 = (uint32_t)0;
      break;
    }
  case SB_VUART_WREN:
    {
      sioWriteEnableFlagsX(unitp->siop, (sioevents_t)ectxp->r1);
      ectxp->r0 = (uint32_t)0;
      break;
    }
  case SB_VUART_GCERR:
    {
      ectxp->r0 = (uint32_t)sioGetAndClearErrorsX(unitp->siop);
      break;
    }
  case SB_VUART_GCEVT:
    {
      ectxp->r0 = (uint32_t)sioGetAndClearEventsX(unitp->siop,
                                                  (sioevents_t)ectxp->r1);
      break;
    }
  case SB_VUART_GEVT:
    {
      ectxp->r0 = (uint32_t)sioGetEventsX(unitp->siop);
      break;
    }
  case SB_VUART_CTL:
    /* falls through */
  default:
    /* Silently ignored.*/
    break;
  }
}

#endif /* VIO_CFG_ENABLE_UART == TRUE */

/** @} */
