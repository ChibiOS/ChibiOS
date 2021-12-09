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
 * @file    vfs/src/chparser.c
 * @brief   VFS parser utilities code.
 *
 * @addtogroup VFS_PARSER
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
 * @brief   Matches a path separator.
 *
 * @param[in, out]  pathp       pointer to the path under parsing
 */
msg_t vfs_parse_match_separator(const char **pathp) {
  msg_t err;
  const char *p = *pathp;

  if (vfs_parse_is_separator(*p++)) {
    err = VFS_RET_ENOENT;
  }
  else {
    err = VFS_RET_SUCCESS;
    *pathp = p;
  }

  return err;
}

/**
 * @brief   Matches a string end.
 *
 * @param[in, out]  pathp       pointer to the path under parsing
 */
msg_t vfs_parse_match_end(const char **pathp) {
  msg_t err;

  if (**pathp != '\0') {
    err = VFS_RET_ENOENT;
  }
  else {
    err = VFS_RET_SUCCESS;
  }

  return err;
}

/**
 * @brief   Fetches the next path element.
 * @note    Consumes the next path separator, if any.
 *
 * @param[in, out]  pathp       pointer to the path under parsing
 * @param[out]      fname       extracted file name
 */
msg_t vfs_parse_get_fname(const char **pathp, char *fname) {
  size_t size;
  const char *p;

  p = *pathp;
  size = 0U;
  while (true) {
    char c = *p;

    /* Path elements must be terminated by a separator or an end-of-string.*/
    if (vfs_parse_is_separator(c) || vfs_parse_is_terminator(c)) {

      /* Consecutive separators are not valid.*/
      if (size == 0U) {
        return VFS_RET_ENOENT;
      }

      /* Advancing the path pointer past the file name in the path and
         closing the file name string.*/
      *pathp = p;
      *fname = '\0';
      return VFS_RET_SUCCESS;
    }

    /* Valid characters for path names.*/
    if (!vfs_parse_is_filechar(c)) {
      return VFS_RET_ENOENT;
    }

    /* Exceeding the path element length.*/
    if (size > VFS_CFG_NAMELEN_MAX) {
      return VFS_RET_ENOENT;
    }

    *fname++ = c;
    p++;
    size++;
  }
}

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
size_t vfs_parse_copy_with_separator(char *dst, const char *src) {
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

    if (n > VFS_CFG_PATHLEN_MAX) {
      return 0U;
    }

    *dst++ = '/';
    *dst = '\0';
    n++;
  }

  return n;
}

/** @} */
