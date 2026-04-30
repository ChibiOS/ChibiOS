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
 * @file    sb/vhal/sbvio_i2c.c
 * @brief   ARM SandBox host Virtual I2C code.
 *
 * @addtogroup ARM_SANDBOX_HOST_VIO_I2C
 * @{
 */

#include "sb.h"

#if (VIO_CFG_ENABLE_I2C == TRUE) || defined(__DOXYGEN__)

/*===========================================================================*/
/* Module local functions.                                                   */
/*===========================================================================*/

static void vi2c_cb(void *ip) {
  hal_i2c_driver_c *i2cp = (hal_i2c_driver_c *)ip;
  const vio_i2c_unit_t *unitp = (const vio_i2c_unit_t *)drvGetArgumentX(i2cp);
  uint32_t flags;

  if (unitp == NULL) {
    return;
  }

  if (i2cGetErrorsX(i2cp) != I2C_NO_ERROR) {
    flags = 1U << HAL_DRV_STATE_ERROR;
  }
  else {
    flags = 1U << HAL_DRV_STATE_COMPLETE;
  }

  chSysLockFromISR();

  sbVRQSetFlagsI(unitp->vrqsb, unitp->vrqn, flags);
  sbVRQTriggerI(unitp->vrqsb, unitp->vrqn);

  chSysUnlockFromISR();
}

static bool vi2c_get_transfer(sb_class_t *sbp,
                              const sb_vi2c_transfer_t *transferp,
                              sb_vi2c_transfer_t *transferbufp) {

  if (!sb_is_valid_read_range(sbp, transferp, sizeof (*transferp))) {
    return false;
  }

  memcpy(transferbufp, transferp, sizeof (*transferp));
  return true;
}

/*===========================================================================*/
/* Module exported functions.                                                */
/*===========================================================================*/

void sb_sysc_vio_i2c(sb_class_t *sbp, struct port_extctx *ectxp) {
  uint32_t sub  = VIO_CALL_SUBCODE(ectxp->r0);
  uint32_t unit = VIO_CALL_UNIT(ectxp->r0);
  ectxp->r0 = (uint32_t)CH_RET_INNER_ERROR;

  /* VIO not associated.*/
  if ((sbp->vioconf == NULL) || (sbp->vioconf->i2cs == NULL)) {
    ectxp->r0 = (uint32_t)HAL_RET_NO_RESOURCE;
    return;
  }

  if (unit >= sbp->vioconf->i2cs->n) {
    ectxp->r0 = (uint32_t)HAL_RET_NO_RESOURCE;
    return;
  }

  /* API processing.*/
  {
    const vio_i2c_unit_t *unitp = &sbp->vioconf->i2cs->units[unit];

    switch (sub) {
    case SB_VI2C_INIT:
      {
        size_t n = ectxp->r1;
        void *p = (void *)ectxp->r2;
        const void *confp;
        msg_t msg;

        if ((sbp->vioconf->i2cconfs == NULL) ||
            (sbp->vioconf->i2cconfs->cfgsnum == 0U)) {
          ectxp->r0 = (uint32_t)HAL_RET_NO_RESOURCE;
          break;
        }

        if (n > sizeof (hal_i2c_config_t)) {
          ectxp->r0 = (uint32_t)HAL_RET_CONFIG_ERROR;
          break;
        }

        if ((n > 0U) && !sb_is_valid_write_range(sbp, p, n)) {
          ectxp->r0 = (uint32_t)CH_RET_EFAULT;
          break;
        }

        confp = &sbp->vioconf->i2cconfs->cfgs[0];

        drvSetArgumentX(unitp->i2cp, (void *)unitp);

        msg = drvStart(unitp->i2cp, confp);
        if (msg == HAL_RET_SUCCESS) {
          drvSetCallbackX(unitp->i2cp, vi2c_cb);
          if (n > 0U) {
            memcpy(p, confp, n);
          }
        }

        ectxp->r0 = (uint32_t)msg;
        break;
      }
    case SB_VI2C_DEINIT:
      {
        drvSetCallbackX(unitp->i2cp, NULL);
        drvStop(unitp->i2cp);
        drvSetArgumentX(unitp->i2cp, NULL);

        ectxp->r0 = (uint32_t)HAL_RET_SUCCESS;
        break;
      }
    case SB_VI2C_TX:
      {
        const sb_vi2c_transfer_t *transferp;
        sb_vi2c_transfer_t transfer;
        msg_t msg;

        transferp = (const sb_vi2c_transfer_t *)ectxp->r1;
        if (!vi2c_get_transfer(sbp, transferp, &transfer)) {
          ectxp->r0 = (uint32_t)CH_RET_EFAULT;
          break;
        }

        if ((transfer.txbytes == 0U) || (transfer.txbuf == NULL)) {
          ectxp->r0 = (uint32_t)CH_RET_EINVAL;
          break;
        }

        if ((transfer.rxbytes > 0U) && (transfer.rxbuf == NULL)) {
          ectxp->r0 = (uint32_t)CH_RET_EINVAL;
          break;
        }

        if (!sb_is_valid_read_range(sbp, transfer.txbuf,
                                    (size_t)transfer.txbytes)) {
          ectxp->r0 = (uint32_t)CH_RET_EFAULT;
          break;
        }

        if ((transfer.rxbytes > 0U) &&
            !sb_is_valid_write_range(sbp, transfer.rxbuf,
                                     (size_t)transfer.rxbytes)) {
          ectxp->r0 = (uint32_t)CH_RET_EFAULT;
          break;
        }

        chSysLock();
        if (drvGetStateX(unitp->i2cp) != HAL_DRV_STATE_READY) {
          msg = HAL_RET_INV_STATE;
        }
        else {
          msg = i2cStartMasterTransmitI(unitp->i2cp,
                                        (i2caddr_t)transfer.addr,
                                        transfer.txbuf,
                                        (size_t)transfer.txbytes,
                                        transfer.rxbuf,
                                        (size_t)transfer.rxbytes);
        }
        chSysUnlock();

        ectxp->r0 = (uint32_t)msg;
        break;
      }
    case SB_VI2C_RX:
      {
        const sb_vi2c_transfer_t *transferp;
        sb_vi2c_transfer_t transfer;
        msg_t msg;

        transferp = (const sb_vi2c_transfer_t *)ectxp->r1;
        if (!vi2c_get_transfer(sbp, transferp, &transfer)) {
          ectxp->r0 = (uint32_t)CH_RET_EFAULT;
          break;
        }

        if ((transfer.addr == 0U) ||
            (transfer.rxbytes == 0U) || (transfer.rxbuf == NULL)) {
          ectxp->r0 = (uint32_t)CH_RET_EINVAL;
          break;
        }

        if (!sb_is_valid_write_range(sbp, transfer.rxbuf,
                                     (size_t)transfer.rxbytes)) {
          ectxp->r0 = (uint32_t)CH_RET_EFAULT;
          break;
        }

        chSysLock();
        if (drvGetStateX(unitp->i2cp) != HAL_DRV_STATE_READY) {
          msg = HAL_RET_INV_STATE;
        }
        else {
          msg = i2cStartMasterReceiveI(unitp->i2cp,
                                       (i2caddr_t)transfer.addr,
                                       transfer.rxbuf,
                                       (size_t)transfer.rxbytes);
        }
        chSysUnlock();

        ectxp->r0 = (uint32_t)msg;
        break;
      }
    case SB_VI2C_STOP:
      {
        msg_t msg;

        chSysLock();
        if (drvGetStateX(unitp->i2cp) == HAL_DRV_STATE_STOP) {
          msg = HAL_RET_INV_STATE;
        }
        else {
          msg = i2cStopTransferI(unitp->i2cp);
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
}

void sb_fastc_vio_i2c(sb_class_t *sbp, struct port_extctx *ectxp) {
  uint32_t sub  = VIO_CALL_SUBCODE(ectxp->r0);
  uint32_t unit = VIO_CALL_UNIT(ectxp->r0);

  /* VIO not associated.*/
  if ((sbp->vioconf == NULL) || (sbp->vioconf->i2cs == NULL)) {
    ectxp->r0 = (uint32_t)HAL_RET_NO_RESOURCE;
    return;
  }

  if (unit >= sbp->vioconf->i2cs->n) {
    ectxp->r0 = (uint32_t)HAL_RET_NO_RESOURCE;
    return;
  }

  /* API processing.*/
  {
    const vio_i2c_unit_t *unitp = &sbp->vioconf->i2cs->units[unit];

    switch (sub) {
    case SB_VI2C_SELCFG:
      {
        uint32_t cfgnum = ectxp->r1;
        const void *confp;
        msg_t msg;

        if ((sbp->vioconf->i2cconfs == NULL) ||
            (cfgnum >= sbp->vioconf->i2cconfs->cfgsnum)) {
          ectxp->r0 = (uint32_t)HAL_RET_NO_RESOURCE;
          break;
        }

        confp = &sbp->vioconf->i2cconfs->cfgs[cfgnum];
        msg = drvSetCfgX(unitp->i2cp, confp);
        if (msg == HAL_RET_SUCCESS) {
          ectxp->r0 = (uint32_t)HAL_RET_SUCCESS;
        }
        else {
          ectxp->r0 = (uint32_t)HAL_RET_CONFIG_ERROR;
        }

        break;
      }
    case SB_VI2C_GCERR:
      {
        ectxp->r0 = (uint32_t)i2cGetAndClearErrorsX(unitp->i2cp);
        break;
      }
    default:
      ectxp->r0 = (uint32_t)CH_RET_ENOSYS;
      break;
    }
  }
}

#endif /* VIO_CFG_ENABLE_I2C == TRUE */

/** @} */
