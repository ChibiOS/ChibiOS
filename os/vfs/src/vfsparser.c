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
 * @addtogroup VFS_PARSE
 * @{
 */

#include <string.h>
#include <ctype.h>

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

  if (*p++ != '/') {
    err = VFS_RET_INVALID_PATH;
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
    err = VFS_RET_INVALID_PATH;
  }
  else {
    err = VFS_RET_SUCCESS;
  }

  return err;
}

/**
 * @brief   Parses a filename element using the restricted Posix set.
 * @note    Consumes the next path separator, if any.
 *
 * @param[in, out]  pathp       pointer to the path under parsing
 * @param[out]      fname       extracted file name
 */
msg_t vfs_parse_filename(const char **pathp, char *fname) {
  msg_t err = VFS_RET_INVALID_PATH;
  size_t n;
  const char *p = *pathp;

  n = 0U;
  while (true) {
    char c = *p++;

    /* Valid characters for path names.*/
    if (!isalnum(c) && (c != '_') && (c != '-') && (c != '.')) {
      break;
    }

    if (c == '/') {
      *pathp = p + 1;
      *fname = '\0';
      err = VFS_RET_SUCCESS;
      break;
    }

    if (c == '\0') {
      *pathp = p;
      *fname = '\0';
      err = VFS_RET_SUCCESS;
      break;
    }

    if (n > VFS_CFG_MAX_NAMELEN) {
      break;
    }

    *fname++ = c;
    n++;
  }

  return err;
}

/** @} */
