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
 * @file    sb/vhal/sbvio_spi.h
 * @brief   ARM SandBox host Virtual SPI macros and structures.
 *
 * @addtogroup ARM_SANDBOX_HOST_VIO_SPI
 * @{
 */

#ifndef SBVIO_SPI_H
#define SBVIO_SPI_H

#if (VIO_CFG_ENABLE_SPI == TRUE) || defined(__DOXYGEN__)

/*===========================================================================*/
/* Module constants.                                                         */
/*===========================================================================*/

/**
 * @brief   Fast API handler for VHAL SPI driver.
 */
#define SB_SVC98_HANDLER        sb_fastc_vio_spi

/**
 * @brief   API handler for VHAL SPI driver.
 */
#define SB_SVC226_HANDLER       sb_sysc_vio_spi

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
 * @brief   Type of a VIO SPI unit representation.
 */
typedef struct vio_spi_unit {
  hal_spi_driver_c  *spip;
  sb_class_t        *vrqsb;
  sb_vrqnum_t       vrqn;
} vio_spi_unit_t;

/**
 * @brief   Type of a VIO SPI units structure.
 */
typedef struct vio_spi_units {
  uint32_t          n;
  vio_spi_unit_t    units[];
} vio_spi_units_t;

/*===========================================================================*/
/* Module macros.                                                            */
/*===========================================================================*/

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#ifdef __cplusplus
extern "C" {
#endif
void sb_sysc_vio_spi(sb_class_t *sbp, struct port_extctx *ectxp);
void sb_fastc_vio_spi(sb_class_t *sbp, struct port_extctx *ectxp);
#ifdef __cplusplus
}
#endif

/*===========================================================================*/
/* Module inline functions.                                                  */
/*===========================================================================*/

#endif /* VIO_CFG_ENABLE_SPI == TRUE */

#endif /* SBVIO_SPI_H */

/** @} */
