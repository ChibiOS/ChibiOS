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
/* Module local definitions.                                                 */
/*===========================================================================*/

#define VETH_TOKEN_SHIFT      16U
#define VETH_TOKEN_SLOT_MASK  0xFFFFU

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

static uint16_t veth_next_generation(uint16_t generation) {

  generation++;
  if (generation == 0U) {
    generation = 1U;
  }

  return generation;
}

static bool veth_validate_unit(const vio_eth_unit_t *unitp) {

  if ((unitp->ethp == NULL) ||
      (unitp->vrqsb == NULL) ||
      (unitp->rxslots == NULL) ||
      (unitp->txslots == NULL) ||
      (unitp->rxslots_num == 0U) ||
      (unitp->txslots_num == 0U) ||
      (unitp->rxslots_num > VETH_TOKEN_SLOT_MASK) ||
      (unitp->txslots_num > VETH_TOKEN_SLOT_MASK)) {
    return false;
  }

  return true;
}

static void veth_reset_rxslots(const vio_eth_unit_t *unitp) {
  uint32_t i;

  for (i = 0U; i < unitp->rxslots_num; i++) {
    unitp->rxslots[i].handle = (eth_receive_handle_t)0U;
    unitp->rxslots[i].generation =
      veth_next_generation(unitp->rxslots[i].generation);
  }
}

static void veth_reset_txslots(const vio_eth_unit_t *unitp) {
  uint32_t i;

  for (i = 0U; i < unitp->txslots_num; i++) {
    unitp->txslots[i].handle = (eth_transmit_handle_t)0U;
    unitp->txslots[i].generation =
      veth_next_generation(unitp->txslots[i].generation);
  }
}

static void veth_reset_slots(const vio_eth_unit_t *unitp) {

  veth_reset_rxslots(unitp);
  veth_reset_txslots(unitp);
}

static uint32_t veth_make_token(uint32_t index, uint16_t generation) {

  return (uint32_t)(((uint32_t)generation << VETH_TOKEN_SHIFT) |
                    ((index + 1U) & VETH_TOKEN_SLOT_MASK));
}

static int32_t veth_find_free_rxslot(const vio_eth_unit_t *unitp) {
  uint32_t i;

  for (i = 0U; i < unitp->rxslots_num; i++) {
    if (unitp->rxslots[i].handle == (eth_receive_handle_t)0U) {
      return (int32_t)i;
    }
  }

  return -1;
}

static int32_t veth_find_free_txslot(const vio_eth_unit_t *unitp) {
  uint32_t i;

  for (i = 0U; i < unitp->txslots_num; i++) {
    if (unitp->txslots[i].handle == (eth_transmit_handle_t)0U) {
      return (int32_t)i;
    }
  }

  return -1;
}

static vio_eth_rx_slot_t *veth_decode_rxslot(const vio_eth_unit_t *unitp,
                                             uint32_t token) {
  uint32_t slot;
  uint16_t generation;

  if (token == 0U) {
    return NULL;
  }

  slot = (token & VETH_TOKEN_SLOT_MASK);
  if (slot == 0U) {
    return NULL;
  }
  slot--;

  if (slot >= unitp->rxslots_num) {
    return NULL;
  }

  generation = (uint16_t)(token >> VETH_TOKEN_SHIFT);
  if (generation == 0U) {
    return NULL;
  }

  if ((unitp->rxslots[slot].handle == (eth_receive_handle_t)0U) ||
      (unitp->rxslots[slot].generation != generation)) {
    return NULL;
  }

  return &unitp->rxslots[slot];
}

static vio_eth_tx_slot_t *veth_decode_txslot(const vio_eth_unit_t *unitp,
                                             uint32_t token) {
  uint32_t slot;
  uint16_t generation;

  if (token == 0U) {
    return NULL;
  }

  slot = (token & VETH_TOKEN_SLOT_MASK);
  if (slot == 0U) {
    return NULL;
  }
  slot--;

  if (slot >= unitp->txslots_num) {
    return NULL;
  }

  generation = (uint16_t)(token >> VETH_TOKEN_SHIFT);
  if (generation == 0U) {
    return NULL;
  }

  if ((unitp->txslots[slot].handle == (eth_transmit_handle_t)0U) ||
      (unitp->txslots[slot].generation != generation)) {
    return NULL;
  }

  return &unitp->txslots[slot];
}

static void veth_release_rxslot(vio_eth_rx_slot_t *slotp) {

  slotp->handle = (eth_receive_handle_t)0U;
  slotp->generation = veth_next_generation(slotp->generation);
}

static void veth_release_txslot(vio_eth_tx_slot_t *slotp) {

  slotp->handle = (eth_transmit_handle_t)0U;
  slotp->generation = veth_next_generation(slotp->generation);
}

static void veth_cb(void *ip) {
  hal_eth_driver_c *ethp = (hal_eth_driver_c *)ip;
  const vio_eth_unit_t *unitp = (const vio_eth_unit_t *)drvGetArgumentX(ethp);

  if (unitp == NULL) {
    return;
  }

  sbVRQTriggerFromISR(unitp->vrqsb, unitp->vrqn);
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

  {
    const vio_eth_unit_t *unitp = &sbp->vioconf->eths->units[unit];

    if (!veth_validate_unit(unitp)) {
      ectxp->r0 = (uint32_t)HAL_RET_CONFIG_ERROR;
      return;
    }

    switch (sub) {
    case SB_VETH_INIT:
      {
        msg_t msg;

        veth_reset_slots(unitp);

        drvSetArgumentX(unitp->ethp, (void *)unitp);

        msg = drvStart(unitp->ethp);
        if (msg == HAL_RET_SUCCESS) {
          drvSetCallbackX(unitp->ethp, veth_cb);
        }

        ectxp->r0 = (uint32_t)msg;
        break;
      }
    case SB_VETH_DEINIT:
      {
        drvSetCallbackX(unitp->ethp, NULL);
        veth_reset_slots(unitp);
        drvStop(unitp->ethp);

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

        if (drvGetStateX(unitp->ethp) != HAL_DRV_STATE_READY) {
          ectxp->r0 = (uint32_t)HAL_RET_INV_STATE;
          break;
        }

        if (n > sizeof (cfg)) {
          ectxp->r0 = (uint32_t)HAL_RET_CONFIG_ERROR;
          break;
        }

        if (!sb_is_valid_write_range(sbp, p, n)) {
          ectxp->r0 = (uint32_t)CH_RET_EFAULT;
          break;
        }

        confp = (const hal_eth_config_t *)drvSelectCfgX(unitp->ethp, cfgnum);
        if (confp == NULL) {
          ectxp->r0 = (uint32_t)HAL_RET_CONFIG_ERROR;
          break;
        }

        memset(&cfg, 0, sizeof (cfg));
        if (confp->mac_address != NULL) {
          cfg.flags |= VIO_ETH_CFGF_MAC_ADDRESS;
          memcpy(cfg.mac_address, confp->mac_address, sizeof cfg.mac_address);
        }

        memcpy(p, &cfg, n);
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
        vio_eth_rx_slot_t *slotp;
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

        slotp = veth_decode_rxslot(unitp, ectxp->r1);
        if (slotp == NULL) {
          ectxp->r0 = (uint32_t)CH_RET_EINVAL;
          break;
        }

        ectxp->r0 = (uint32_t)ethReadReceiveHandle(unitp->ethp, slotp->handle,
                                                   buffer, n);
        break;
      }
    case SB_VETH_TXWRITE:
      {
        vio_eth_tx_slot_t *slotp;
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

        slotp = veth_decode_txslot(unitp, ectxp->r1);
        if (slotp == NULL) {
          ectxp->r0 = (uint32_t)CH_RET_EINVAL;
          break;
        }

        ectxp->r0 = (uint32_t)ethWriteTransmitHandle(unitp->ethp,
                                                     slotp->handle,
                                                     buffer, n);
        break;
      }
    case SB_VETH_RXREL:
      {
        vio_eth_rx_slot_t *slotp;

        if (drvGetStateX(unitp->ethp) != HAL_DRV_STATE_READY) {
          ectxp->r0 = (uint32_t)HAL_RET_INV_STATE;
          break;
        }

        slotp = veth_decode_rxslot(unitp, ectxp->r1);
        if (slotp == NULL) {
          ectxp->r0 = (uint32_t)CH_RET_EINVAL;
          break;
        }

        ethReleaseReceiveHandle(unitp->ethp, slotp->handle);
        veth_release_rxslot(slotp);
        ectxp->r0 = (uint32_t)HAL_RET_SUCCESS;
        break;
      }
    case SB_VETH_TXREL:
      {
        vio_eth_tx_slot_t *slotp;

        if (drvGetStateX(unitp->ethp) != HAL_DRV_STATE_READY) {
          ectxp->r0 = (uint32_t)HAL_RET_INV_STATE;
          break;
        }

        slotp = veth_decode_txslot(unitp, ectxp->r1);
        if (slotp == NULL) {
          ectxp->r0 = (uint32_t)CH_RET_EINVAL;
          break;
        }

        ethReleaseTransmitHandle(unitp->ethp, slotp->handle);
        veth_release_txslot(slotp);
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

  {
    const vio_eth_unit_t *unitp = &sbp->vioconf->eths->units[unit];

    if (!veth_validate_unit(unitp)) {
      ectxp->r0 = (uint32_t)HAL_RET_CONFIG_ERROR;
      return;
    }

    switch (sub) {
    case SB_VETH_RXGET:
      {
        int32_t slot;
        eth_receive_handle_t handle;

        if (drvGetStateX(unitp->ethp) != HAL_DRV_STATE_READY) {
          ectxp->r0 = (uint32_t)0U;
          return;
        }

        slot = veth_find_free_rxslot(unitp);
        if (slot < 0) {
          ectxp->r0 = (uint32_t)0U;
          return;
        }

        handle = ethGetReceiveHandleX(unitp->ethp);

        if (handle == (eth_receive_handle_t)0U) {
          ectxp->r0 = (uint32_t)0U;
          return;
        }

        unitp->rxslots[slot].handle = handle;
        ectxp->r0 = veth_make_token((uint32_t)slot,
                                    unitp->rxslots[slot].generation);
        return;
      }
    case SB_VETH_TXGET:
      {
        int32_t slot;
        eth_transmit_handle_t handle;

        if (drvGetStateX(unitp->ethp) != HAL_DRV_STATE_READY) {
          ectxp->r0 = (uint32_t)0U;
          return;
        }

        slot = veth_find_free_txslot(unitp);
        if (slot < 0) {
          ectxp->r0 = (uint32_t)0U;
          return;
        }

        handle = ethGetTransmitHandleX(unitp->ethp);

        if (handle == (eth_transmit_handle_t)0U) {
          ectxp->r0 = (uint32_t)0U;
          return;
        }

        unitp->txslots[slot].handle = handle;
        ectxp->r0 = veth_make_token((uint32_t)slot,
                                    unitp->txslots[slot].generation);
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
