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
 * @file    sb/vhal/sbvio_eth.h
 * @brief   ARM SandBox host Virtual ETH macros and structures.
 *
 * @addtogroup ARM_SANDBOX_HOST_VIO_ETH
 * @{
 */

#ifndef SBVIO_ETH_H
#define SBVIO_ETH_H

#if (VIO_CFG_ENABLE_ETH == TRUE) || defined(__DOXYGEN__)

#if (HAL_USE_ETH != TRUE) && !defined(__DOXYGEN__)
#error "VIO_CFG_ENABLE_ETH requires HAL_USE_ETH"
#endif

/*===========================================================================*/
/* Module constants.                                                         */
/*===========================================================================*/

/**
 * @brief   Fast API handler for VIO ETH driver.
 */
#define SB_SVC99_HANDLER        sb_fastc_vio_eth

/**
 * @brief   API handler for VIO ETH driver.
 */
#define SB_SVC227_HANDLER       sb_sysc_vio_eth

/**
 * @name    VIO ETH configuration flags
 * @{
 */
#define VIO_ETH_CFGF_MAC_ADDRESS   (1U << 0)
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
 * @brief   Type of a VIO ETH configuration representation.
 */
typedef struct vio_eth_config {
  uint32_t          flags;
  uint8_t           mac_address[6];
  uint8_t           reserved[2];
} vio_eth_config_t;

/**
 * @brief   Type of a VIO ETH unit representation.
 */
typedef struct vio_eth_unit {
  hal_eth_driver_c        *ethp;
  sb_class_t              *vrqsb;
  sb_vrqnum_t             vrqn;
} vio_eth_unit_t;

/**
 * @brief   Type of a VIO ETH units structure.
 */
typedef struct vio_eth_units {
  uint32_t          n;
  vio_eth_unit_t    units[];
} vio_eth_units_t;

/*===========================================================================*/
/* Module macros.                                                            */
/*===========================================================================*/

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#ifdef __cplusplus
extern "C" {
#endif
void sb_sysc_vio_eth(sb_class_t *sbp, struct port_extctx *ectxp);
void sb_fastc_vio_eth(sb_class_t *sbp, struct port_extctx *ectxp);
#ifdef __cplusplus
}
#endif

/*===========================================================================*/
/* Module inline functions.                                                  */
/*===========================================================================*/

#endif /* VIO_CFG_ENABLE_ETH == TRUE */

#endif /* SBVIO_ETH_H */

/** @} */
