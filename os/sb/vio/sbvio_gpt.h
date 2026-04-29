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
 * @file    sb/vhal/sbvio_gpt.h
 * @brief   ARM SandBox host Virtual GPT macros and structures.
 *
 * @addtogroup ARM_SANDBOX_HOST_VIO_GPT
 * @{
 */

#ifndef SBVIO_GPT_H
#define SBVIO_GPT_H

#if (VIO_CFG_ENABLE_GPT == TRUE) || defined(__DOXYGEN__)

#if (HAL_USE_GPT != TRUE) && !defined(__DOXYGEN__)
#error "VIO_CFG_ENABLE_GPT requires HAL_USE_GPT"
#endif

/*===========================================================================*/
/* Module constants.                                                         */
/*===========================================================================*/

/**
 * @brief   Fast API handler for VIO GPT driver.
 */
#define SB_SVC101_HANDLER       sb_fastc_vio_gpt

/**
 * @brief   API handler for VIO GPT driver.
 */
#define SB_SVC229_HANDLER       sb_sysc_vio_gpt

/*===========================================================================*/
/* Module data structures and types.                                         */
/*===========================================================================*/

/**
 * @brief   Type of a VIO GPT unit representation.
 */
typedef struct vio_gpt_unit {
  hal_gpt_driver_c          *gptp;
  sb_class_t                *vrqsb;
  sb_vrqnum_t               vrqn;
} vio_gpt_unit_t;

/**
 * @brief   Type of a VIO GPT units structure.
 */
typedef struct vio_gpt_units {
  uint32_t                  n;
  vio_gpt_unit_t            units[];
} vio_gpt_units_t;

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#ifdef __cplusplus
extern "C" {
#endif
void sb_sysc_vio_gpt(sb_class_t *sbp, struct port_extctx *ectxp);
void sb_fastc_vio_gpt(sb_class_t *sbp, struct port_extctx *ectxp);
#ifdef __cplusplus
}
#endif

#endif /* VIO_CFG_ENABLE_GPT == TRUE */

#endif /* SBVIO_GPT_H */

/** @} */
