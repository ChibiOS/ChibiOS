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
 * @file    sb/vhal/sbvio_gpio.h
 * @brief   ARM SandBox host Virtual GPIO macros and structures.
 *
 * @addtogroup ARM_SANDBOX_HOST_VIO_GPIO
 * @{
 */

#ifndef SBVIO_GPIO_H
#define SBVIO_GPIO_H

#if (VIO_CFG_ENABLE_GPIO == TRUE) || defined(__DOXYGEN__)

/*===========================================================================*/
/* Module constants.                                                         */
/*===========================================================================*/

/**
 * @brief   Fast API handler for VHAL PAL driver.
 */
#define SB_SVC96_HANDLER       sb_fastc_vio_gpio

/**
 * @name    VIO GPIO permissions
 * @{
 */
#define VIO_GPIO_PERM_WRITE     1U
#define VIO_GPIO_PERM_READ      2U
#define VIO_GPIO_PERM_SETMODE   4U
/** @} */

/*===========================================================================*/
/* Module pre-compile time settings.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

/*===========================================================================*/
/* Module data structures and types.                                         */
/*===========================================================================*/

/**
 * @brief   Type of a VIO GPIO unit representation.
 */
typedef struct vio_gpio_unit {
  uint32_t        permissions;
  ioportid_t      port;
  ioportmask_t    mask;
  uint32_t        offset;
} vio_gpio_unit_t;

/**
 * @brief   Type of a VIO GPIOs unit structure.
 */
typedef struct vio_gpio_units {
  uint32_t          n;
  vio_gpio_unit_t   units[];
} vio_gpio_units_t;

/*===========================================================================*/
/* Module macros.                                                            */
/*===========================================================================*/

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#ifdef __cplusplus
extern "C" {
#endif
  void sb_fastc_vio_gpio(sb_class_t *sbp, struct port_extctx *ectxp);
#ifdef __cplusplus
}
#endif

/*===========================================================================*/
/* Module inline functions.                                                  */
/*===========================================================================*/

#endif /* VIO_CFG_ENABLE_GPIO == TRUE */

#endif /* SBVIO_GPIO_H */

/** @} */
