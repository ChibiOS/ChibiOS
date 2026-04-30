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
 * @file    sb/vhal/sbvio_eth.c
 * @brief   ARM SandBox host Virtual ETH code.
 *
 * @addtogroup ARM_SANDBOX_HOST_VIO_ETH
 * @{
 */

#include <string.h>

#include "sb.h"

#if (VIO_CFG_ENABLE_ETH == TRUE) || defined(__DOXYGEN__)

/*===========================================================================*/
/* Module local functions.                                                   */
/*===========================================================================*/

static void veth_cb(void *ip) {
  hal_eth_driver_c *ethp = (hal_eth_driver_c *)ip;
  const vio_eth_unit_t *unitp = (const vio_eth_unit_t *)drvGetArgumentX(ethp);

  if (unitp == NULL) {
    return;
  }

  chSysLockFromISR();

  sbVRQSetFlagsI(unitp->vrqsb, unitp->vrqn, ethp->lastflags);
  sbVRQTriggerI(unitp->vrqsb, unitp->vrqn);

  chSysUnlockFromISR();
}

/*===========================================================================*/
/* Module exported functions.                                                */
/*===========================================================================*/

void sb_sysc_vio_eth(sb_class_t *sbp, struct port_extctx *ectxp) {
  uint32_t sub  = VIO_CALL_SUBCODE(ectxp->r0);
  uint32_t unit = VIO_CALL_UNIT(ectxp->r0);
  ectxp->r0 = (uint32_t)CH_RET_INNER_ERROR;

  /* VIO not associated.*/
  if ((sbp->vioconf == NULL) || (sbp->vioconf->eths == NULL)) {
    ectxp->r0 = (uint32_t)HAL_RET_NO_RESOURCE;
    return;
  }

  if (unit >= sbp->vioconf->eths->n) {
    ectxp->r0 = (uint32_t)HAL_RET_NO_RESOURCE;
    return;
  }

  /* API processing.*/
  {
    const vio_eth_unit_t *unitp = &sbp->vioconf->eths->units[unit];

    switch (sub) {
    case SB_VETH_INIT:
      {
        size_t n = (size_t)ectxp->r1;
        void *p = (void *)ectxp->r2;
        const hal_eth_config_t *confp;
        vio_eth_config_t cfg;
        msg_t msg;

        if ((sbp->vioconf->ethconfs == NULL) ||
            (sbp->vioconf->ethconfs->cfgsnum == 0U)) {
          ectxp->r0 = (uint32_t)HAL_RET_NO_RESOURCE;
          break;
        }

        if (n > sizeof (cfg)) {
          ectxp->r0 = (uint32_t)HAL_RET_CONFIG_ERROR;
          break;
        }

        if ((n > 0U) && !sb_is_valid_write_range(sbp, p, n)) {
          ectxp->r0 = (uint32_t)CH_RET_EFAULT;
          break;
        }

        confp = &sbp->vioconf->ethconfs->cfgs[0];

        drvSetArgumentX(unitp->ethp, (void *)unitp);

        msg = drvStart(unitp->ethp, confp);
        if (msg == HAL_RET_SUCCESS) {
          drvSetCallbackX(unitp->ethp, veth_cb);

          memset(&cfg, 0, sizeof (cfg));
          if (confp->mac_address != NULL) {
            cfg.flags |= VIO_ETH_CFGF_MAC_ADDRESS;
            memcpy(cfg.mac_address, confp->mac_address,
                   sizeof cfg.mac_address);
          }
          if (n > 0U) {
            memcpy(p, &cfg, n);
          }
        }

        ectxp->r0 = (uint32_t)msg;
        break;
      }
    case SB_VETH_DEINIT:
      {
        drvSetCallbackX(unitp->ethp, NULL);
        drvStop(unitp->ethp);
        drvSetArgumentX(unitp->ethp, NULL);

        ectxp->r0 = (uint32_t)HAL_RET_SUCCESS;
        break;
      }
    case SB_VETH_SELCFG:
      {
        uint32_t cfgnum = ectxp->r1;
        size_t n = (size_t)ectxp->r2;
        void *p = (void *)ectxp->r3;
        const hal_eth_config_t *confp;
        vio_eth_config_t cfg;
        msg_t msg;

        if (drvGetStateX(unitp->ethp) != HAL_DRV_STATE_READY) {
          ectxp->r0 = (uint32_t)HAL_RET_INV_STATE;
          break;
        }

        if ((sbp->vioconf->ethconfs == NULL) ||
            (cfgnum >= sbp->vioconf->ethconfs->cfgsnum)) {
          ectxp->r0 = (uint32_t)HAL_RET_NO_RESOURCE;
          break;
        }

        if (n > sizeof (cfg)) {
          ectxp->r0 = (uint32_t)HAL_RET_CONFIG_ERROR;
          break;
        }

        if ((n > 0U) && !sb_is_valid_write_range(sbp, p, n)) {
          ectxp->r0 = (uint32_t)CH_RET_EFAULT;
          break;
        }

        confp = &sbp->vioconf->ethconfs->cfgs[cfgnum];
        msg = drvSetCfgX(unitp->ethp, confp);
        if (msg != HAL_RET_SUCCESS) {
          ectxp->r0 = (uint32_t)HAL_RET_CONFIG_ERROR;
          break;
        }

        memset(&cfg, 0, sizeof (cfg));
        if (confp->mac_address != NULL) {
          cfg.flags |= VIO_ETH_CFGF_MAC_ADDRESS;
          memcpy(cfg.mac_address, confp->mac_address, sizeof cfg.mac_address);
        }

        if (n > 0U) {
          memcpy(p, &cfg, n);
        }
        ectxp->r0 = (uint32_t)HAL_RET_SUCCESS;
        break;
      }
    case SB_VETH_LINK:
      {
        ectxp->r0 = (uint32_t)ethPollLinkStatus(unitp->ethp);
        break;
      }
    case SB_VETH_RXREAD:
      {
        eth_receive_handle_t rxh = (eth_receive_handle_t)ectxp->r1;
        uint8_t *buffer = (uint8_t *)ectxp->r2;
        size_t n = (size_t)ectxp->r3;

        if (!sb_is_valid_write_range(sbp, buffer, n)) {
          ectxp->r0 = (uint32_t)CH_RET_EFAULT;
          break;
        }

        if (drvGetStateX(unitp->ethp) != HAL_DRV_STATE_READY) {
          ectxp->r0 = (uint32_t)HAL_RET_INV_STATE;
          break;
        }

        if (!ethIsRXHandleValidX(unitp->ethp, rxh)) {
          ectxp->r0 = (uint32_t)CH_RET_EINVAL;
          break;
        }

        ectxp->r0 = (uint32_t)ethReadReceiveHandle(unitp->ethp, rxh,
                                                   buffer, n);
        break;
      }
    case SB_VETH_TXWRITE:
      {
        eth_transmit_handle_t txh = (eth_transmit_handle_t)ectxp->r1;
        const uint8_t *buffer = (const uint8_t *)ectxp->r2;
        size_t n = (size_t)ectxp->r3;

        if (!sb_is_valid_read_range(sbp, buffer, n)) {
          ectxp->r0 = (uint32_t)CH_RET_EFAULT;
          break;
        }

        if (drvGetStateX(unitp->ethp) != HAL_DRV_STATE_READY) {
          ectxp->r0 = (uint32_t)HAL_RET_INV_STATE;
          break;
        }

        if (!ethIsTXHandleValidX(unitp->ethp, txh)) {
          ectxp->r0 = (uint32_t)CH_RET_EINVAL;
          break;
        }

        ectxp->r0 = (uint32_t)ethWriteTransmitHandle(unitp->ethp, txh,
                                                     buffer, n);
        break;
      }
    case SB_VETH_RXREL:
      {
        eth_receive_handle_t rxh = (eth_receive_handle_t)ectxp->r1;

        if (drvGetStateX(unitp->ethp) != HAL_DRV_STATE_READY) {
          ectxp->r0 = (uint32_t)HAL_RET_INV_STATE;
          break;
        }

        if (!ethIsRXHandleValidX(unitp->ethp, rxh)) {
          ectxp->r0 = (uint32_t)CH_RET_EINVAL;
          break;
        }

        ethReleaseReceiveHandle(unitp->ethp, rxh);
        ectxp->r0 = (uint32_t)HAL_RET_SUCCESS;
        break;
      }
    case SB_VETH_TXREL:
      {
        eth_transmit_handle_t txh = (eth_transmit_handle_t)ectxp->r1;

        if (drvGetStateX(unitp->ethp) != HAL_DRV_STATE_READY) {
          ectxp->r0 = (uint32_t)HAL_RET_INV_STATE;
          break;
        }

        if (!ethIsTXHandleValidX(unitp->ethp, txh)) {
          ectxp->r0 = (uint32_t)CH_RET_EINVAL;
          break;
        }

        ethReleaseTransmitHandle(unitp->ethp, txh);
        ectxp->r0 = (uint32_t)HAL_RET_SUCCESS;
        break;
      }
    default:
      ectxp->r0 = (uint32_t)CH_RET_ENOSYS;
      break;
    }
  }
}

void sb_fastc_vio_eth(sb_class_t *sbp, struct port_extctx *ectxp) {
  uint32_t sub  = VIO_CALL_SUBCODE(ectxp->r0);
  uint32_t unit = VIO_CALL_UNIT(ectxp->r0);

  /* VIO not associated.*/
  if ((sbp->vioconf == NULL) || (sbp->vioconf->eths == NULL)) {
    ectxp->r0 = (uint32_t)HAL_RET_NO_RESOURCE;
    return;
  }

  if (unit >= sbp->vioconf->eths->n) {
    ectxp->r0 = (uint32_t)HAL_RET_NO_RESOURCE;
    return;
  }

  /* API processing.*/
  {
    const vio_eth_unit_t *unitp = &sbp->vioconf->eths->units[unit];

    switch (sub) {
    case SB_VETH_RXGET:
      {
        if (drvGetStateX(unitp->ethp) != HAL_DRV_STATE_READY) {
          ectxp->r0 = (uint32_t)0U;
          return;
        }

        ectxp->r0 = (uint32_t)ethGetReceiveHandleX(unitp->ethp);
        return;
      }
    case SB_VETH_TXGET:
      {
        if (drvGetStateX(unitp->ethp) != HAL_DRV_STATE_READY) {
          ectxp->r0 = (uint32_t)0U;
          return;
        }

        ectxp->r0 = (uint32_t)ethGetTransmitHandleX(unitp->ethp);
        return;
      }
    default:
      ectxp->r0 = (uint32_t)CH_RET_ENOSYS;
      return;
    }
  }
}

#endif /* VIO_CFG_ENABLE_ETH == TRUE */

/** @} */
