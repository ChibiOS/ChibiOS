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
 * @file    sb/vhal/sbvio_gpt.c
 * @brief   ARM SandBox host Virtual GPT code.
 *
 * @addtogroup ARM_SANDBOX_HOST_VIO_GPT
 * @{
 */

#include "sb.h"

#include <string.h>

#if (VIO_CFG_ENABLE_GPT == TRUE) || defined(__DOXYGEN__)

/*===========================================================================*/
/* Module local functions.                                                   */
/*===========================================================================*/

static void vgpt_cb(void *ip) {
  hal_gpt_driver_c *gptp = (hal_gpt_driver_c *)ip;
  const vio_gpt_unit_t *unitp = (const vio_gpt_unit_t *)drvGetArgumentX(gptp);

  if (unitp == NULL) {
    return;
  }

  chSysLockFromISR();

  sbVRQSetFlagsI(unitp->vrqsb, unitp->vrqn, 1U << drvGetStateX(gptp));
  sbVRQTriggerI(unitp->vrqsb, unitp->vrqn);

  chSysUnlockFromISR();
}

/*===========================================================================*/
/* Module exported functions.                                                */
/*===========================================================================*/

void sb_sysc_vio_gpt(sb_class_t *sbp, struct port_extctx *ectxp) {
  uint32_t sub  = VIO_CALL_SUBCODE(ectxp->r0);
  uint32_t unit = VIO_CALL_UNIT(ectxp->r0);
  ectxp->r0 = (uint32_t)CH_RET_INNER_ERROR;

  /* VIO not associated.*/
  if ((sbp->vioconf == NULL) || (sbp->vioconf->gpts == NULL)) {
    ectxp->r0 = (uint32_t)HAL_RET_NO_RESOURCE;
    return;
  }

  if (unit >= sbp->vioconf->gpts->n) {
    ectxp->r0 = (uint32_t)HAL_RET_NO_RESOURCE;
    return;
  }

  /* API processing.*/
  {
    const vio_gpt_unit_t *unitp = &sbp->vioconf->gpts->units[unit];

    switch (sub) {
    case SB_VGPT_INIT:
      {
        size_t n = ectxp->r1;
        void *p = (void *)ectxp->r2;
        const void *confp;
        msg_t msg;

        if ((sbp->vioconf->gptconfs == NULL) ||
            (sbp->vioconf->gptconfs->cfgsnum == 0U)) {
          ectxp->r0 = (uint32_t)HAL_RET_NO_RESOURCE;
          break;
        }

        if (n > sizeof (hal_gpt_config_t)) {
          ectxp->r0 = (uint32_t)HAL_RET_CONFIG_ERROR;
          break;
        }

        if ((n > 0U) && !sb_is_valid_write_range(sbp, p, n)) {
          ectxp->r0 = (uint32_t)CH_RET_EFAULT;
          break;
        }

        confp = &sbp->vioconf->gptconfs->cfgs[0];

        drvSetArgumentX(unitp->gptp, (void *)unitp);

        msg = drvStart(unitp->gptp, confp);
        if (msg == HAL_RET_SUCCESS) {
          if (n > 0U) {
            memcpy(p, confp, n);
          }
        }

        ectxp->r0 = (uint32_t)msg;
        break;
      }
    case SB_VGPT_DEINIT:
      {
        drvSetCallbackX(unitp->gptp, NULL);
        drvStop(unitp->gptp);
        drvSetArgumentX(unitp->gptp, NULL);

        ectxp->r0 = (uint32_t)HAL_RET_SUCCESS;
        break;
      }
    case SB_VGPT_SETCB:
      {
        uint32_t enable = ectxp->r1;

        if (drvGetStateX(unitp->gptp) == HAL_DRV_STATE_STOP) {
          ectxp->r0 = (uint32_t)HAL_RET_INV_STATE;
          break;
        }

        drvSetCallbackX(unitp->gptp, enable != 0U ? vgpt_cb : NULL);

        ectxp->r0 = (uint32_t)HAL_RET_SUCCESS;
        break;
      }
    case SB_VGPT_SELCFG:
      {
        uint32_t cfgnum = ectxp->r1;
        size_t n = ectxp->r2;
        void *p = (void *)ectxp->r3;
        const void *confp;
        msg_t msg;

        if (drvGetStateX(unitp->gptp) != HAL_DRV_STATE_READY) {
          ectxp->r0 = (uint32_t)HAL_RET_INV_STATE;
          break;
        }

        if ((sbp->vioconf->gptconfs == NULL) ||
            (cfgnum >= sbp->vioconf->gptconfs->cfgsnum)) {
          ectxp->r0 = (uint32_t)HAL_RET_NO_RESOURCE;
          break;
        }

        if (n > sizeof (hal_gpt_config_t)) {
          ectxp->r0 = (uint32_t)HAL_RET_CONFIG_ERROR;
          break;
        }

        if ((n > 0U) && !sb_is_valid_write_range(sbp, p, n)) {
          ectxp->r0 = (uint32_t)CH_RET_EFAULT;
          break;
        }

        confp = &sbp->vioconf->gptconfs->cfgs[cfgnum];
        msg = drvSetCfgX(unitp->gptp, confp);
        if (msg == HAL_RET_SUCCESS) {
          if (n > 0U) {
            memcpy(p, confp, n);
          }
          ectxp->r0 = (uint32_t)HAL_RET_SUCCESS;
        }
        else {
          ectxp->r0 = (uint32_t)HAL_RET_CONFIG_ERROR;
        }

        break;
      }
    case SB_VGPT_PDELAY:
      {
        gptcnt_t interval = (gptcnt_t)ectxp->r1;

        if (interval == (gptcnt_t)0) {
          ectxp->r0 = (uint32_t)CH_RET_EINVAL;
          break;
        }

        if (drvGetStateX(unitp->gptp) != HAL_DRV_STATE_READY) {
          ectxp->r0 = (uint32_t)HAL_RET_INV_STATE;
          break;
        }

        gptPolledDelay(unitp->gptp, interval);
        ectxp->r0 = (uint32_t)HAL_RET_SUCCESS;
        break;
      }
    case SB_VGPT_START:
      {
        uint32_t mode = ectxp->r1;
        gptcnt_t interval = (gptcnt_t)ectxp->r2;
        msg_t msg;

        if (interval == (gptcnt_t)0) {
          ectxp->r0 = (uint32_t)CH_RET_EINVAL;
          break;
        }

        if (drvGetStateX(unitp->gptp) != HAL_DRV_STATE_READY) {
          msg = HAL_RET_INV_STATE;
        }
        else if (mode == SB_VGPT_CONTINUOUS) {
          gptStartContinuous(unitp->gptp, interval);
          msg = HAL_RET_SUCCESS;
        }
        else if (mode == SB_VGPT_ONESHOT) {
          gptStartOneShot(unitp->gptp, interval);
          msg = HAL_RET_SUCCESS;
        }
        else {
          msg = CH_RET_EINVAL;
        }

        ectxp->r0 = (uint32_t)msg;
        break;
      }
    case SB_VGPT_STOP:
      {
        driver_state_t state = drvGetStateX(unitp->gptp);

        if ((state != HAL_DRV_STATE_READY) &&
            (state != GPT_CONTINUOUS) &&
            (state != GPT_ONESHOT) &&
            (state != HAL_DRV_STATE_COMPLETE)) {
          ectxp->r0 = (uint32_t)HAL_RET_INV_STATE;
          break;
        }

        gptStopTimer(unitp->gptp);

        ectxp->r0 = (uint32_t)HAL_RET_SUCCESS;
        break;
      }
    case SB_VGPT_CHGI:
      {
        gptcnt_t interval = (gptcnt_t)ectxp->r1;

        if (interval == (gptcnt_t)0) {
          ectxp->r0 = (uint32_t)CH_RET_EINVAL;
          break;
        }

        if (drvGetStateX(unitp->gptp) != GPT_CONTINUOUS) {
          ectxp->r0 = (uint32_t)HAL_RET_INV_STATE;
          break;
        }

        gptChangeInterval(unitp->gptp, interval);

        ectxp->r0 = (uint32_t)HAL_RET_SUCCESS;
        break;
      }
    default:
      ectxp->r0 = (uint32_t)CH_RET_ENOSYS;
      break;
    }
  }
}

void sb_fastc_vio_gpt(sb_class_t *sbp, struct port_extctx *ectxp) {
  uint32_t sub  = VIO_CALL_SUBCODE(ectxp->r0);
  uint32_t unit = VIO_CALL_UNIT(ectxp->r0);

  /* VIO not associated.*/
  if ((sbp->vioconf == NULL) || (sbp->vioconf->gpts == NULL)) {
    ectxp->r0 = (uint32_t)HAL_RET_NO_RESOURCE;
    return;
  }

  if (unit >= sbp->vioconf->gpts->n) {
    ectxp->r0 = (uint32_t)HAL_RET_NO_RESOURCE;
    return;
  }

  /* API processing.*/
  {
    const vio_gpt_unit_t *unitp = &sbp->vioconf->gpts->units[unit];

    switch (sub) {
    case SB_VGPT_GETI:
      {
        ectxp->r0 = (uint32_t)gptGetIntervalX(unitp->gptp);
        break;
      }
    case SB_VGPT_GETC:
      {
        ectxp->r0 = (uint32_t)gptGetCounterX(unitp->gptp);
        break;
      }
    case SB_VGPT_GETFREQ:
      {
        const hal_gpt_config_t *config;

        config = (const hal_gpt_config_t *)unitp->gptp->config;
        ectxp->r0 = config != NULL ? (uint32_t)config->frequency : 0U;
        break;
      }
    default:
      ectxp->r0 = (uint32_t)CH_RET_ENOSYS;
      break;
    }
  }
}

#endif /* VIO_CFG_ENABLE_GPT == TRUE */

/** @} */
