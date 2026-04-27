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
 * @file    sb/vhal/sbvio_i2c.h
 * @brief   ARM SandBox host Virtual I2C macros and structures.
 *
 * @addtogroup ARM_SANDBOX_HOST_VIO_I2C
 * @{
 */

#ifndef SBVIO_I2C_H
#define SBVIO_I2C_H

#if (VIO_CFG_ENABLE_I2C == TRUE) || defined(__DOXYGEN__)

#if (HAL_USE_I2C != TRUE) && !defined(__DOXYGEN__)
#error "VIO_CFG_ENABLE_I2C requires HAL_USE_I2C"
#endif

/*===========================================================================*/
/* Module constants.                                                         */
/*===========================================================================*/

/**
 * @brief   Fast API handler for VIO I2C driver.
 */
#define SB_SVC102_HANDLER       sb_fastc_vio_i2c

/**
 * @brief   API handler for VIO I2C driver.
 */
#define SB_SVC230_HANDLER       sb_sysc_vio_i2c

/*===========================================================================*/
/* Module data structures and types.                                         */
/*===========================================================================*/

/**
 * @brief   Type of a VIO I2C unit representation.
 */
typedef struct vio_i2c_unit {
  hal_i2c_driver_c          *i2cp;
  const hal_i2c_config_t    *config;
  sb_class_t                *vrqsb;
  sb_vrqnum_t               vrqn;
} vio_i2c_unit_t;

/**
 * @brief   Type of a VIO I2C units structure.
 */
typedef struct vio_i2c_units {
  uint32_t                  n;
  vio_i2c_unit_t            units[];
} vio_i2c_units_t;

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#ifdef __cplusplus
extern "C" {
#endif
void sb_sysc_vio_i2c(sb_class_t *sbp, struct port_extctx *ectxp);
void sb_fastc_vio_i2c(sb_class_t *sbp, struct port_extctx *ectxp);
#ifdef __cplusplus
}
#endif

#endif /* VIO_CFG_ENABLE_I2C == TRUE */

#endif /* SBVIO_I2C_H */

/** @} */
