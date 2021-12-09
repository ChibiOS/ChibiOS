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
 * @file    vfs/include/vfsparser.h
 * @brief   VFS parser utilities header file.
 *
 * @addtogroup VFS_PARSER
 * @{
 */

#ifndef VFSPARSER_H
#define VFSPARSER_H

/*===========================================================================*/
/* Module constants.                                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Module pre-compile time settings.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

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
  msg_t vfs_parse_match_separator(const char **pathp);
  msg_t vfs_parse_match_end(const char **pathp);
  msg_t vfs_parse_get_fname(const char **pathp, char *fname);
  size_t vfs_parse_copy_with_separator(char *dst, const char *src);
#ifdef __cplusplus
}
#endif

/*===========================================================================*/
/* Module inline functions.                                                  */
/*===========================================================================*/

/**
 * @brief   Checks for a valid path separator.
 *
 * @param[in] c                 The character to be checked.
 * @return                      The operation result.
 */
CC_FORCE_INLINE
static inline bool vfs_parse_is_separator(char c) {

  return (bool)(c == '/');
}

/**
 * @brief   Checks for a valid path terminator.
 *
 * @param[in] c                 The character to be checked.
 * @return                      The operation result.
 */
CC_FORCE_INLINE
static inline bool vfs_parse_is_terminator(char c) {

  return (bool)(c == '\0');
}

/**
 * @brief   Checks for a path element character.
 *
 * @param[in] c                 The character to be checked.
 * @return                      The operation result.
 */
CC_FORCE_INLINE
static inline bool vfs_parse_is_filechar(char c) {

  /* Restrictive Posix set.*/
  return (bool)(isalnum(c) || (c == '_') || (c == '-') || (c == '.'));
}

#endif /* VFSPARSER_H */

/** @} */
