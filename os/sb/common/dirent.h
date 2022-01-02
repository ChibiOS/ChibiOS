/*
    ChibiOS - Copyright (C) 2006,2007,2008,2009,2010,2011,2012,2013,2014,
              2015,2016,2017,2018,2019,2020,2021,2022 Giovanni Di Sirio.

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
 * @file    sb/common/dirent.h
 * @brief   Replaces the default dirent.h file.
 *
 * @addtogroup ARM_SANDBOX_DIRENT
 * @{
 */

#ifndef DIRENT_H
#define DIRENT_H

#include <sys/types.h>

/*===========================================================================*/
/* Module constants.                                                         */
/*===========================================================================*/

/**
 * @name    Structure dirent defined fields
 * @{
 */
#define _DIRENT_HAVE_D_RECLEN
#define _DIRENT_HAVE_D_TYPE
/** @} */

/**
 * @name    File types
 * @{
 */
#define DT_REG              IFTODT(S_IFREG)
#define DT_DIR              IFTODT(S_IFDIR)
#define DT_CHR              IFTODT(S_IFCHR)
#define DT_FIFO             IFTODT(S_IFIFO)
/** @} */

#define DIR_BUF_SIZE        512

/*===========================================================================*/
/* Module pre-compile time settings.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

/*===========================================================================*/
/* Module data structures and types.                                         */
/*===========================================================================*/

struct dirent {
  ino_t             d_ino;
  unsigned short    d_reclen;
  unsigned char     d_type;
  char              d_name[];
};

typedef struct {
  int               fd;
  ssize_t           next;
  ssize_t           size;
  char              buf[DIR_BUF_SIZE];
} DIR;

/*===========================================================================*/
/* Module macros.                                                            */
/*===========================================================================*/

#define IFTODT(mode)        (((mode) & 0170000) >> 12)
#define DTTOIF(dirtype)     ((dirtype) << 12)

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#ifdef __cplusplus
extern "C" {
#endif
  DIR *fdopendir(int fd);
  DIR *opendir(const char *name);
  int closedir (DIR *dirp);
  struct dirent *readdir (DIR *dirp);
#ifdef __cplusplus
}
#endif

/*===========================================================================*/
/* Module inline functions.                                                  */
/*===========================================================================*/

#endif /* DIRENT_H */

/** @} */
