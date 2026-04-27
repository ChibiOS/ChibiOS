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
 * @file    sb/vhal/sbvio_adc.h
 * @brief   ARM SandBox host Virtual ADC macros and structures.
 *
 * @addtogroup ARM_SANDBOX_HOST_VIO_ADC
 * @{
 */

#ifndef SBVIO_ADC_H
#define SBVIO_ADC_H

#if (VIO_CFG_ENABLE_ADC == TRUE) || defined(__DOXYGEN__)

#if (HAL_USE_ADC != TRUE) && !defined(__DOXYGEN__)
#error "VIO_CFG_ENABLE_ADC requires HAL_USE_ADC"
#endif

/*===========================================================================*/
/* Module constants.                                                         */
/*===========================================================================*/

/**
 * @brief   Fast API handler for VIO ADC driver.
 */
#define SB_SVC100_HANDLER       sb_fastc_vio_adc

/**
 * @brief   API handler for VIO ADC driver.
 */
#define SB_SVC228_HANDLER       sb_sysc_vio_adc

/*===========================================================================*/
/* Module data structures and types.                                         */
/*===========================================================================*/

/**
 * @brief   Type of a VIO ADC unit representation.
 */
typedef struct vio_adc_unit {
  hal_adc_driver_c          *adcp;
  const hal_adc_config_t    *config;
  sb_class_t                *vrqsb;
  sb_vrqnum_t               vrqn;
} vio_adc_unit_t;

/**
 * @brief   Type of a VIO ADC units structure.
 */
typedef struct vio_adc_units {
  uint32_t                  n;
  vio_adc_unit_t            units[];
} vio_adc_units_t;

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#ifdef __cplusplus
extern "C" {
#endif
void sb_sysc_vio_adc(sb_class_t *sbp, struct port_extctx *ectxp);
void sb_fastc_vio_adc(sb_class_t *sbp, struct port_extctx *ectxp);
#ifdef __cplusplus
}
#endif

#endif /* VIO_CFG_ENABLE_ADC == TRUE */

#endif /* SBVIO_ADC_H */

/** @} */
