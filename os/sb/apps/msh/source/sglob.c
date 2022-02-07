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
 * @file    sglob.c
 * @brief   Simplified glob code.
 *
 * @addtogroup SGLOB
 * @{
 */

#include <stdbool.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <dirent.h>

#include "sglob.h"

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

static bool isdots(const char *p) {

  if (p[0] == '.') {
    if (p[1] == '.') {
      if (p[2] == '\0') {
        return true;
      }
    }
    else if (p[1] == '\0') {
      return true;
    }
  }
  return false;
}

static char *lsalloc(sglob_t *psglob, size_t len) {
  lstring_t *lsp;

  lsp = malloc(len + sizeof (lstring_t));
  if (lsp != NULL) {
    lsp->next = psglob->gl_next;
    psglob->gl_next = lsp;
  }

  return lsp->string;
}

/* Copyright not specified so public domain code, not my thing.*/
bool match(const char *pattern, const char *text) {

  if (*pattern == '\0' && *text == '\0')
    return true;

  if (*pattern == '*' && *(pattern + 1) != '\0' && *text == '\0')
    return false;

  if (*pattern == '?' || *pattern == *text)
    return match(pattern + 1, text + 1);

  if (*pattern == '*')
    return match(pattern + 1, text) || match(pattern, text + 1);

  return false;
}

/*===========================================================================*/
/* Module exported functions.                                                */
/*===========================================================================*/

void sglob_init(sglob_t *psglob, size_t offs) {

  psglob->gl_offs  = offs;
  psglob->gl_buf   = NULL;
  psglob->gl_next  = NULL;
  psglob->gl_pathc = 0;
  psglob->gl_pathv = NULL;
}

void sglob_free(sglob_t *psglob) {
  lstring_t *lsp;

  if (psglob->gl_buf != NULL) {
    free(psglob->gl_buf);
  }

  while ((lsp = psglob->gl_next) != NULL) {
    psglob->gl_next = lsp->next;
    free(lsp);
  }
}

int sglob_add(const char *pattern, sglob_t *psglob) {
  int ret;
  DIR *dbp;

  dbp = NULL;
  ret = GLOB_NOSPACE;
  do {
    char *lastp, *dirp;
    size_t plen, dirlen;
    bool no_spec;
    struct dirent *dp;

    /* Checking the path length.*/
    plen = strnlen(pattern, PATH_MAX);
    if (plen >= PATH_MAX) {
      break;
    }

    /* Allocating a path buffer if not already done.*/
    if (psglob->gl_buf == NULL) {
      psglob->gl_buf = malloc(PATH_MAX);
      if (psglob->gl_buf == NULL) {
        break;
      }
    }

    /* We need the path pattern in a writable buffer.*/
    strcpy(psglob->gl_buf, pattern);

    /* Finding the last element, the one to be expanded.*/
    lastp = rindex(psglob->gl_buf, '/');
    if (lastp == NULL) {
      /* It is all last element.*/
      lastp    = psglob->gl_buf;
      dirp     = NULL;
      dirlen   = (size_t)0;
    }
    else {
      *lastp++ = '\0';
      dirp     = psglob->gl_buf;
      dirlen   = strlen(dirp);
    }

    /* Case with no wildcards.*/
    no_spec = (bool)((index(lastp, '?') == NULL) && (index(lastp, '*') == NULL));
    if (no_spec) {
      char *p;

      p = lsalloc(psglob, plen);
      if (p != NULL) {
        strcpy(p, pattern);
 //       psglob->gl_pathv[psglob->gl_offs + psglob->gl_pathc]     = p;
 //       psglob->gl_pathv[psglob->gl_offs + psglob->gl_pathc + 1] = NULL;
        psglob->gl_pathc++;
        ret = 0;
      }
      break;
    }

    /* Opening the directory part of the path. If it fauls then it is not
       reported as an error but an empty sglob is returned, this can be
       detected from outside.*/
    dbp = opendir(dirp);
    if (dbp == NULL) {
      ret = 0;
      break;
    }

    /* Scanning the directory for matches.*/
    while ((dp = readdir(dbp)) != NULL) {
      char *p;

      if (isdots(dp->d_name) || !match(lastp, dp->d_name)) {
        continue;
      }

      /* Inserting the new found match into the list.*/
      p = lsalloc(psglob, dirlen + strlen(dp->d_name) + 1);
      if (p == NULL) {
        break;
      }
      *p = '\0';
      if (dirlen > 0) {
        strcat(p, dirp);
        strcat(p, "/");
      }
      strcat(p, dp->d_name);
//      psglob->gl_pathv[psglob->gl_offs + psglob->gl_pathc]     = p;
//      psglob->gl_pathv[psglob->gl_offs + psglob->gl_pathc + 1] = NULL;
      psglob->gl_pathc++;
    }

    closedir(dbp);
  }
  while (false);

  return ret;
}

/** @} */
