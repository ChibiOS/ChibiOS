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
 * @file    sb/host/sbloader.h
 * @brief   ARM SandBox ELF loader macros and structures.
 *
 * @addtogroup sbhost.h
 * @{
 */

#ifndef SBELF_H
#define SBELF_H

#if (SB_CFG_ENABLE_VFS == TRUE) || defined(__DOXYGEN__)

/*===========================================================================*/
/* Module constants.                                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Module pre-compile time settings.                                         */
/*===========================================================================*/

/**
 * @brief   Maximum number of ELF regions that can be allocated by the loader.
 */
#if !defined(SB_CFG_ELF_MAX_ALLOCATED) || defined(__DOXYGEN__)
#define SB_CFG_ELF_MAX_ALLOCATED        6
#endif

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

#if (SB_CFG_ELF_MAX_ALLOCATED < 2) || (SB_CFG_ELF_MAX_ALLOCATED > 32)
#error "invalid SB_CFG_ELF_MAX_ALLOCATED value"
#endif

/*===========================================================================*/
/* Module data structures and types.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Module macros.                                                            */
/*===========================================================================*/

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#ifdef __cplusplus
extern "C" {
#endif
  msg_t sbElfLoad(vfs_file_node_c *fnp,
                  const memory_area_t *map);
  msg_t sbElfLoadFile(vfs_driver_c *drvp,
                      const char *path,
                      const memory_area_t *map);
#ifdef __cplusplus
}
#endif

/*===========================================================================*/
/* Module inline functions.                                                  */
/*===========================================================================*/

#endif /* SB_CFG_ENABLE_VFS == TRUE */

#endif /* SBELF_H */

/** @} */
