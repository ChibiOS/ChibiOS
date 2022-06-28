/*
    ChibiOS - Copyright (C) 2006,2007,2008,2009,2010,2011,2012,2013,2014,
              2015,2016,2017,2018,2019,2020,2021 Giovanni Di Sirio.

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
 * @file    sb/vhal/sbvhal_pal.h
 * @brief   ARM SandBox host Virtual HAL PAL macros and structures.
 *
 * @addtogroup ARM_SANDBOX_HOST_VHAL
 * @{
 */

#ifndef SBVHAL_PAL_H
#define SBVHAL_PAL_H

#if (SB_CFG_ENABLE_VHAL_PAL == TRUE) || defined(__DOXYGEN__)

/*===========================================================================*/
/* Module constants.                                                         */
/*===========================================================================*/

/**
 * @brief   API handler for VHAL PAL driver.
 */
#define SB_SVC200_HANDLER       sb_api_vhal_pal

/**
 * @name    VPIO permissions
 * @{
 */
#define VPIO_PERM_WRITE         1U
#define VPIO_PERM_READ          2U
#define VPIO_PERM_READLATCH     4U
#define VPIO_PERM_SETMODE       8U
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
 * @brief   Type of a VPIO configuration structure.
 */
typedef struct vhal_vpio_conf {
  uint32_t        permissions;
  ioportid_t      port;
  ioportmask_t    mask;
  uint32_t        offset;
} vhal_vpio_conf_t;

/**
 * @brief   Type of a VHAL PAL instance configuration structure.
 */
typedef struct vhal_pal_conf {
  uint32_t          n;
  vhal_vpio_conf_t  vpio[];
} vhal_pal_conf_t;

/*===========================================================================*/
/* Module macros.                                                            */
/*===========================================================================*/

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#ifdef __cplusplus
extern "C" {
#endif
  void sb_api_vhal_pal(struct port_extctx *ectxp);
#ifdef __cplusplus
}
#endif

/*===========================================================================*/
/* Module inline functions.                                                  */
/*===========================================================================*/

#endif /* SB_CFG_ENABLE_VHAL == TRUE */

#endif /* SBVHAL_PAL_H */

/** @} */
