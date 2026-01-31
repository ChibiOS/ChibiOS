/*
    Copyright (C) 2006-2026 Giovanni Di Sirio.

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
 * @file    sb/vhal/sbvio_spi.c
 * @brief   ARM SandBox host Virtual SPI code.
 *
 * @addtogroup ARM_SANDBOX_HOST_VIO_SPI
 * @{
 */

#include "sb.h"

#if (VIO_CFG_ENABLE_SPI == TRUE) || defined(__DOXYGEN__)

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

static void vspi_cb(void *ip) {
  hal_spi_driver_c *spip = (hal_spi_driver_c *)ip;
  const vio_spi_unit_t *unitp = (const vio_spi_unit_t *)drvGetArgumentX(spip);

  chSysLockFromISR();

  /* Need to send the state as a flags mask.*/
  sbVRQSetFlagsI(unitp->vrqsb, unitp->vrqn, 1U << drvGetStateX(spip));
  sbVRQTriggerI(unitp->vrqsb, unitp->vrqn);

  chSysUnlockFromISR();
}

/*===========================================================================*/
/* Module exported functions.                                                */
/*===========================================================================*/

void sb_sysc_vio_spi(sb_class_t *sbp, struct port_extctx *ectxp) {
  uint32_t sub  = VIO_CALL_SUBCODE(ectxp->r0);
  uint32_t unit = VIO_CALL_UNIT(ectxp->r0);
  ectxp->r0 = (uint32_t)CH_RET_INNER_ERROR;
  const vio_spi_unit_t *unitp;

  /* VIO not associated.*/
  if (sbp->vioconf == NULL) {
    ectxp->r0 = (uint32_t)HAL_RET_NO_RESOURCE;
    return;
  }

  if (unit >= sbp->vioconf->spis->n) {
    ectxp->r0 = (uint32_t)HAL_RET_NO_RESOURCE;
    return;
  }

  unitp = &sbp->vioconf->spis->units[unit];

  switch (sub) {
  case SB_VSPI_INIT:
    {
      msg_t msg;

      /* Associating this virtual SPI to the SPI driver.*/
      drvSetArgumentX(unitp->spip, (void *)unitp);

      msg = drvStart(unitp->spip);
      if (msg == HAL_RET_SUCCESS) {

        /* Enabling the callback.*/
        drvSetCallbackX(unitp->spip, vspi_cb);
      }

      ectxp->r0 = (uint32_t)msg;
      break;
    }
  case SB_VSPI_DEINIT:
    {
      drvStop(unitp->spip);

      ectxp->r0 = (uint32_t)HAL_RET_SUCCESS;
      break;
    }
  case SB_VSPI_PULSES:
    {
      msg_t msg;
      uint32_t n = ectxp->r1;

      if (n == 0U) {
        ectxp->r0 = CH_RET_EINVAL;
        break;
      }

      chSysLock();
      if (drvGetStateX(unitp->spip) != HAL_DRV_STATE_READY) {
        msg = HAL_RET_INV_STATE;
      }
      else {
        msg = spiStartIgnoreI(unitp->spip, n);
      }
      chSysUnlock();

      ectxp->r0 = (uint32_t)msg;
      break;
    }
  case SB_VSPI_RECEIVE:
    {
      msg_t msg;
      uint32_t n = ectxp->r1;
      void *rxbuf = (void *)ectxp->r2;
      size_t bufsize;

      if (n == 0U) {
        ectxp->r0 = CH_RET_EINVAL;
        break;
      }

      bufsize = n * spiGetFrameSizeX(unitp->spip);

      if (!sb_is_valid_write_range(sbp, rxbuf, bufsize)) {
        ectxp->r0 = (uint32_t)CH_RET_EFAULT;
        /* TODO enforce fault instead.*/
        break;
      }

      chSysLock();
      if (drvGetStateX(unitp->spip) != HAL_DRV_STATE_READY) {
        msg = HAL_RET_INV_STATE;
      }
      else {
        msg = spiStartReceiveI(unitp->spip, n, rxbuf);
      }
      chSysUnlock();

      ectxp->r0 = (uint32_t)msg;
      break;
    }
  case SB_VSPI_SEND:
    {
      msg_t msg;
      uint32_t n = ectxp->r1;
      const void *txbuf = (const void *)ectxp->r2;
      size_t bufsize;

      if (n == 0U) {
        ectxp->r0 = CH_RET_EINVAL;
        break;
      }

      bufsize = n * spiGetFrameSizeX(unitp->spip);

      if (!sb_is_valid_read_range(sbp, txbuf, bufsize)) {
        ectxp->r0 = (uint32_t)CH_RET_EFAULT;
        /* TODO enforce fault instead.*/
        break;
      }

      chSysLock();
      if (drvGetStateX(unitp->spip) != HAL_DRV_STATE_READY) {
        msg = HAL_RET_INV_STATE;
      }
      else {
        msg = spiStartSendI(unitp->spip, n, txbuf);
      }
      chSysUnlock();

      ectxp->r0 = (uint32_t)msg;
      break;
    }
  case SB_VSPI_EXCHANGE:
    {
      msg_t msg;
      size_t n = (size_t)ectxp->r1;
      void *rxbuf = (void *)ectxp->r2;
      const void *txbuf = (const void *)ectxp->r3;
      size_t bufsize;

      if (n == 0U) {
        ectxp->r0 = CH_RET_EINVAL;
        break;
      }

      bufsize = n * spiGetFrameSizeX(unitp->spip);

      if (!sb_is_valid_write_range(sbp, rxbuf, bufsize)) {
        ectxp->r0 = (uint32_t)CH_RET_EFAULT;
        /* TODO enforce fault instead.*/
        break;
      }

      if (!sb_is_valid_read_range(sbp, txbuf, bufsize)) {
        ectxp->r0 = (uint32_t)CH_RET_EFAULT;
        /* TODO enforce fault instead.*/
        break;
      }

      chSysLock();
      if (drvGetStateX(unitp->spip) != HAL_DRV_STATE_READY) {
        msg = HAL_RET_INV_STATE;
      }
      else {
        msg = spiStartExchangeI(unitp->spip, n, txbuf, rxbuf);
      }
      chSysUnlock();

      ectxp->r0 = (uint32_t)msg;
      break;
    }
  case SB_VSPI_STOP:
    {
      msg_t msg;
      size_t *np = (size_t *)ectxp->r1;

      if (!sb_is_valid_write_range(sbp, np, sizeof (size_t))) {
        ectxp->r0 = (uint32_t)CH_RET_EFAULT;
        /* TODO enforce fault instead.*/
        break;
      }

      chSysLock();
      if (drvGetStateX(unitp->spip) == HAL_DRV_STATE_STOP) {
        msg = HAL_RET_INV_STATE;
      }
      else {
        msg = spiStopTransferI(unitp->spip, np);
      }
      chSysUnlock();

      ectxp->r0 = (uint32_t)msg;
      break;
    }
  default:
    ectxp->r0 = (uint32_t)CH_RET_ENOSYS;
    break;
  }
}

void sb_fastc_vio_spi(sb_class_t *sbp, struct port_extctx *ectxp) {
  uint32_t sub  = VIO_CALL_SUBCODE(ectxp->r0);
  uint32_t unit = VIO_CALL_UNIT(ectxp->r0);
  const vio_spi_unit_t *unitp;

  /* VIO not associated.*/
  if (sbp->vioconf == NULL) {
    ectxp->r0 = (uint32_t)HAL_RET_NO_RESOURCE;
    return;
  }

  /* Returned value in case of error or illegal sub-code.*/
  ectxp->r0 = (uint32_t)-1;

  if (unit >= sbp->vioconf->spis->n) {
    return;
  }

  unitp = &sbp->vioconf->spis->units[unit];

  /* We don't want assertion or errors to be caused in host, making sure
     all functions are called in the proper state.*/
  if (unitp->spip->state != HAL_DRV_STATE_READY) {
    return;
  }

  switch (sub) {
  case SB_VSPI_SELCFG:
    {
      uint32_t cfgnum = ectxp->r1;
      size_t n = ectxp->r2;
      void *p = (void *)ectxp->r3;
      const void *confp;

      /* Check on configuration index.*/
      if (cfgnum >= sbp->vioconf->spiconfs->cfgsnum) {
        ectxp->r0 = (uint32_t)HAL_RET_CONFIG_ERROR;
        return;
      }

      /* Check on configuration buffer size.*/
      if (n > sizeof (hal_spi_config_t)) {
        ectxp->r0 = (uint32_t)HAL_RET_CONFIG_ERROR;
        return;
      }

      /* Check on configuration buffer area.*/
      if (!sb_is_valid_write_range(sbp, p, n)) {
        ectxp->r0 = (uint32_t)CH_RET_EFAULT;
        /* TODO enforce fault instead.*/
        break;
      }

      /* Specified VSPI configuration.*/
      confp = drvSelectCfgX(unitp->spip, cfgnum);

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
  case SB_VSPI_SELECT:
    {
      spiSelectX(unitp->spip);
      ectxp->r0 = (uint32_t)HAL_RET_SUCCESS;
      break;
    }
  case SB_VSPI_UNSELECT:
    {
      spiUnselectX(unitp->spip);
      ectxp->r0 = (uint32_t)HAL_RET_SUCCESS;
      break;
    }
  default:
    /* Silently ignored.*/
    break;
  }
}

#endif /* VIO_CFG_ENABLE_SPI == TRUE */

/** @} */
