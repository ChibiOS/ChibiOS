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
 * @file    vfs/src/chpaths.c
 * @brief   VFS path utilities code.
 *
 * @addtogroup VFS_PATHS
 * @{
 */

#include "vfs.h"

/*===========================================================================*/
/* Module local definitions.                                                 */
/*===========================================================================*/

/*===========================================================================*/
/* Module exported variables.                                                */
/*===========================================================================*/

/*===========================================================================*/
/* Module local types.                                                       */
/*===========================================================================*/

/*===========================================================================*/
/* Module local variables.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Module local functions.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Module exported functions.                                                */
/*===========================================================================*/

/**
 * @brief   Copies a path into a destination buffer.
 * @details Up to @p VFS_CFG_PATHLEN_MAX characters are copied. A path
 *          separator is added to the end of the path if not present.
 *
 * @param[out] dst              The destination buffer.
 * @param[in] src               The source path.
 * @return                      The copied path size not including the final
 *                              zero.
 * @retval 0                    If the path size exceeded @p VFS_CFG_PATHLEN_MAX.
 */
size_t vfs_path_copy_with_separator(char *dst, const char *src) {
  size_t n = 0U;
  char lc = '\0';

  /* Copying the path.*/
  while ((*dst = *src) != '\0') {

    if (n > VFS_CFG_PATHLEN_MAX) {
      return 0U;
    }

    lc = *src++;
    dst++;
    n++;
  }

  /* Checking if it is terminated by a separator, if not then adding it.*/
  if (lc != '/') {

    if (n >= VFS_CFG_PATHLEN_MAX) {
      return 0U;
    }

    *dst++ = '/';
    *dst = '\0';
    n++;
  }

  return n;
}

msg_t vfs_path_append(char *dst, const char *src) {
  size_t n;

  /* Current path length.*/
  n = strnlen(dst, VFS_CFG_PATHLEN_MAX);
  if (n >= VFS_CFG_PATHLEN_MAX) {
    return ENAMETOOLONG;
  }

  /* Making sure to start with a separator in place.*/
  if (n == 0U) {
    *dst++ = '/';
    n++;
  }
  else {
    dst = dst + n;
    if (*(dst - 1) != '/') {
      *dst++ = '/';
      n++;
    }
  }

  /* The appended part needs to not begin with a separator.*/
  if (*src == '/') {
    src++;
  }

  /* Appending.*/
  while ((*dst++ = *src++) != '\0') {
    n++;

    if (n > VFS_CFG_PATHLEN_MAX) {
      return ENAMETOOLONG;
    }
  }

  *dst = '\0';

  return VFS_RET_SUCCESS;
}

/** @} */
