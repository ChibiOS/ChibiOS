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
 * @file    sb/vhal/sbvio_adc.c
 * @brief   ARM SandBox host Virtual ADC code.
 *
 * @addtogroup ARM_SANDBOX_HOST_VIO_ADC
 * @{
 */

#include "sb.h"

#include <string.h>

#if (VIO_CFG_ENABLE_ADC == TRUE) || defined(__DOXYGEN__)

/*===========================================================================*/
/* Module local functions.                                                   */
/*===========================================================================*/

static void vadc_cb(void *ip) {
  hal_adc_driver_c *adcp = (hal_adc_driver_c *)ip;
  const vio_adc_unit_t *unitp = (const vio_adc_unit_t *)drvGetArgumentX(adcp);

  if (unitp == NULL) {
    return;
  }

  chSysLockFromISR();

  sbVRQSetFlagsI(unitp->vrqsb, unitp->vrqn, 1U << drvGetStateX(adcp));
  sbVRQTriggerI(unitp->vrqsb, unitp->vrqn);

  chSysUnlockFromISR();
}

static bool vadc_get_group(hal_adc_driver_c *adcp, unsigned grpnum,
                           const adc_conversion_group_t **grppp) {
  const hal_adc_config_t *config = (const hal_adc_config_t *)adcp->config;

  if ((config == NULL) || (config->grps == NULL) ||
      (grpnum >= config->grps->grpsnum)) {
    return false;
  }

  *grppp = &config->grps->grps[grpnum];

  return true;
}

static bool vadc_get_buffer_size(const adc_conversion_group_t *grpp,
                                 size_t depth, size_t *sizep) {
  size_t channels = (size_t)grpp->num_channels;
  size_t samples;

  if ((depth == 0U) || ((depth != 1U) && ((depth & 1U) != 0U)) ||
      (channels == 0U)) {
    return false;
  }

  if (depth > (size_t)-1 / channels) {
    return false;
  }

  samples = depth * channels;
  if (samples > (size_t)-1 / sizeof (adcsample_t)) {
    return false;
  }

  *sizep = samples * sizeof (adcsample_t);

  return true;
}

/*===========================================================================*/
/* Module exported functions.                                                */
/*===========================================================================*/

void sb_sysc_vio_adc(sb_class_t *sbp, struct port_extctx *ectxp) {
  uint32_t sub  = VIO_CALL_SUBCODE(ectxp->r0);
  uint32_t unit = VIO_CALL_UNIT(ectxp->r0);
  ectxp->r0 = (uint32_t)CH_RET_INNER_ERROR;

  /* VIO not associated.*/
  if ((sbp->vioconf == NULL) || (sbp->vioconf->adcs == NULL)) {
    ectxp->r0 = (uint32_t)HAL_RET_NO_RESOURCE;
    return;
  }

  if (unit >= sbp->vioconf->adcs->n) {
    ectxp->r0 = (uint32_t)HAL_RET_NO_RESOURCE;
    return;
  }

  /* API processing.*/
  {
    const vio_adc_unit_t *unitp = &sbp->vioconf->adcs->units[unit];

    switch (sub) {
    case SB_VADC_INIT:
      {
        size_t n = ectxp->r1;
        void *p = (void *)ectxp->r2;
        const void *confp;
        msg_t msg;

        if ((sbp->vioconf->adcconfs == NULL) ||
            (sbp->vioconf->adcconfs->cfgsnum == 0U)) {
          ectxp->r0 = (uint32_t)HAL_RET_NO_RESOURCE;
          break;
        }

        if (n > sizeof (hal_adc_config_t)) {
          ectxp->r0 = (uint32_t)HAL_RET_CONFIG_ERROR;
          break;
        }

        if ((n > 0U) && !sb_is_valid_write_range(sbp, p, n)) {
          ectxp->r0 = (uint32_t)CH_RET_EFAULT;
          break;
        }

        confp = &sbp->vioconf->adcconfs->cfgs[0];

        drvSetArgumentX(unitp->adcp, (void *)unitp);

        msg = drvStart(unitp->adcp, confp);
        if (msg == HAL_RET_SUCCESS) {
          drvSetCallbackX(unitp->adcp, vadc_cb);
          if (n > 0U) {
            memcpy(p, confp, n);
          }
        }

        ectxp->r0 = (uint32_t)msg;
        break;
      }
    case SB_VADC_DEINIT:
      {
        drvSetCallbackX(unitp->adcp, NULL);
        drvStop(unitp->adcp);
        drvSetArgumentX(unitp->adcp, NULL);

        ectxp->r0 = (uint32_t)HAL_RET_SUCCESS;
        break;
      }
    case SB_VADC_START_LINEAR:
    case SB_VADC_START_CIRCULAR:
      {
        const adc_conversion_group_t *grpp;
        adcsample_t *samples = (adcsample_t *)ectxp->r2;
        size_t depth = (size_t)ectxp->r3;
        size_t size;
        msg_t msg;
        unsigned grpnum = (unsigned)ectxp->r1;

        if ((drvGetStateX(unitp->adcp) != HAL_DRV_STATE_READY) &&
            (drvGetStateX(unitp->adcp) != HAL_DRV_STATE_ERROR)) {
          ectxp->r0 = (uint32_t)HAL_RET_INV_STATE;
          break;
        }

        if (!vadc_get_group(unitp->adcp, grpnum, &grpp)) {
          ectxp->r0 = (uint32_t)HAL_RET_CONFIG_ERROR;
          break;
        }

        if (!vadc_get_buffer_size(grpp, depth, &size)) {
          ectxp->r0 = (uint32_t)CH_RET_EINVAL;
          break;
        }

        if (!sb_is_valid_write_range(sbp, samples, size)) {
          ectxp->r0 = (uint32_t)CH_RET_EFAULT;
          break;
        }

        if (sub == SB_VADC_START_LINEAR) {
          msg = adcStartConversionLinear(unitp->adcp, grpnum, samples, depth);
        }
        else {
          msg = adcStartConversionCircular(unitp->adcp, grpnum, samples, depth);
        }
        ectxp->r0 = (uint32_t)msg;
        break;
      }
    case SB_VADC_STOP:
      {
        if (drvGetStateX(unitp->adcp) == HAL_DRV_STATE_STOP) {
          ectxp->r0 = (uint32_t)HAL_RET_INV_STATE;
          break;
        }

        if (drvGetStateX(unitp->adcp) != HAL_DRV_STATE_READY) {
          adcStopConversion(unitp->adcp);
        }

        ectxp->r0 = (uint32_t)HAL_RET_SUCCESS;
        break;
      }
    case SB_VADC_SELCFG:
      {
        uint32_t cfgnum = ectxp->r1;
        size_t n = ectxp->r2;
        void *p = (void *)ectxp->r3;
        const void *confp;
        msg_t msg;

        if (drvGetStateX(unitp->adcp) != HAL_DRV_STATE_READY) {
          ectxp->r0 = (uint32_t)HAL_RET_INV_STATE;
          break;
        }

        /* Check on configuration index.*/
        if ((sbp->vioconf->adcconfs == NULL) ||
            (cfgnum >= sbp->vioconf->adcconfs->cfgsnum)) {
          ectxp->r0 = (uint32_t)HAL_RET_NO_RESOURCE;
          break;
        }

        /* Check on configuration buffer size.*/
        if (n > sizeof (hal_adc_config_t)) {
          ectxp->r0 = (uint32_t)HAL_RET_CONFIG_ERROR;
          break;
        }

        /* Check on configuration buffer area.*/
        if ((n > 0U) && !sb_is_valid_write_range(sbp, p, n)) {
          ectxp->r0 = (uint32_t)CH_RET_EFAULT;
          break;
        }

        /* Specified VADC configuration.*/
        confp = &sbp->vioconf->adcconfs->cfgs[cfgnum];
        msg = drvSetCfgX(unitp->adcp, confp);

        /* Copying the standard part of the configuration into the sandbox
           space in the specified position.*/
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
    default:
      ectxp->r0 = (uint32_t)CH_RET_ENOSYS;
      break;
    }
  }
}

void sb_fastc_vio_adc(sb_class_t *sbp, struct port_extctx *ectxp) {
  uint32_t sub  = VIO_CALL_SUBCODE(ectxp->r0);
  uint32_t unit = VIO_CALL_UNIT(ectxp->r0);

  /* VIO not associated.*/
  if ((sbp->vioconf == NULL) || (sbp->vioconf->adcs == NULL)) {
    ectxp->r0 = (uint32_t)HAL_RET_NO_RESOURCE;
    return;
  }

  if (unit >= sbp->vioconf->adcs->n) {
    ectxp->r0 = (uint32_t)HAL_RET_NO_RESOURCE;
    return;
  }

  /* API processing.*/
  {
    const vio_adc_unit_t *unitp = &sbp->vioconf->adcs->units[unit];

    switch (sub) {
    case SB_VADC_GCERR:
      {
        ectxp->r0 = (uint32_t)adcGetAndClearErrorsX(unitp->adcp,
                                                    (adcerror_t)-1);
        break;
      }
    default:
      ectxp->r0 = (uint32_t)CH_RET_ENOSYS;
      break;
    }
  }
}

#endif /* VIO_CFG_ENABLE_ADC == TRUE */

/** @} */
