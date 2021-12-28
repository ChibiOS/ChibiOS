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
 * @param[in, out]  pathp       Pointer to the path under parsing.
 */
msg_t vfs_parse_match_separator(const char **pathp) {
  msg_t err;
  const char *p = *pathp;

  if (!vfs_parse_is_separator(*p++)) {
    err = CH_RET_ENOENT;
  }
  else {
    err = CH_RET_SUCCESS;
    *pathp = p;
  }

  return err;
}

/**
 * @brief   Matches a string end.
 *
 * @param[in, out]  pathp       Pointer to the path under parsing.
 */
msg_t vfs_parse_match_end(const char **pathp) {
  msg_t err;

  if (**pathp != '\0') {
    err = CH_RET_ENOENT;
  }
  else {
    err = CH_RET_SUCCESS;
  }

  return err;
}

/**
 * @brief   Fetches the next path element.
 * @note    Does not consume the next separator, if any.
 * @note    Does not add a final zero to the fetched fname.
 * @note    It can return an empty element, it has to be detected outside.
 *
 * @param[in, out]  pathp       Pointer to the path under parsing.
 * @param[out]      fname       Extracted file name.
 * @param[in]       n           Maximum size in @fname.
 */
msg_t vfs_parse_copy_fname(const char **pathp, char *fname, size_t n) {
  size_t size;
  const char *p;

  p = *pathp;
  size = 0U;
  while (true) {
    char c = *p;

    /* Path elements must be terminated by a separator or an end-of-string.*/
    if (vfs_parse_is_separator(c) || vfs_parse_is_terminator(c)) {

      /* Advancing the path pointer past the file name in the path and
         closing the file name string.*/
      *pathp = p;
      return (msg_t)size;
    }

    /* Valid characters for path names.*/
    if (!vfs_parse_is_filechar(c)) {
      return CH_RET_EINVAL;
    }

    /* Exceeding the maximum length.*/
    if (size > n) {
      return CH_RET_ENAMETOOLONG;
    }

    *fname++ = c;
    p++;
    size++;
  }
}

/**
 * @brief   Fetches the next path element.
 * @note    Does not consume the next separator, if any.
 * @note    It can return an empty element, it has to be detected outside.
 *
 * @param[in, out]  pathp       Pointer to the path under parsing.
 * @param[out]      fname       Extracted file name.
 * @param[in]       n           Maximum size in @fname.
 */
msg_t vfs_parse_get_fname(const char **pathp, char *fname, size_t n) {
  msg_t ret;

  ret = vfs_parse_copy_fname(pathp, fname, n);
  CH_RETURN_ON_ERROR(ret);

  fname[ret] = '\0';

  return ret;
}

/** @} */
