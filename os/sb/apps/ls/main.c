/*
    ChibiOS - Copyright (C) 2006..2025 Giovanni Di Sirio

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

        http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.
*/

#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <dirent.h>

#define NEWLINE_STR         "\r\n"
#define COLUMNS             6
#define COLUMN_SIZE         "11"

/* Option flags, all false initially.*/
struct {
  bool              lflg;
} options;

struct diritem {
  char              ftype;
  ino_t             fnum;
  nlink_t           fnlink;
  mode_t            fflags;
  off_t             fsize;
  char              *fname;
};

struct dirlist {
  bool              free_items;
  size_t            maxlen;
  int               max;
  int               n;
  struct diritem    items[];
};

static struct dirlist *toplist;

static void usage(void) {
  fprintf(stderr, "Usage: ls [<opts>] [<file>]..." NEWLINE_STR);
  fprintf(stderr, "Options:" NEWLINE_STR);
  fprintf(stderr, "  -l                 use a long listing format" NEWLINE_STR);
  exit(1);
}

static void error(const char *s) {

  fprintf(stderr, "ls: %s" NEWLINE_STR, s);
  exit(1);
}

static struct dirlist *dirlist_new(unsigned max, bool free_items) {
  struct dirlist *dlp;
  size_t size;

  size = sizeof (struct dirlist) + ((sizeof (struct diritem)) * max);
  dlp = malloc(size);
//  memset (dlp, 0, size);
  if (dlp == NULL) {
    error("out of memory");
  }

  dlp->free_items = free_items;
  dlp->maxlen = (size_t)0;
  dlp->max = max;
  dlp->n = 0;

  return dlp;
}

static void dirlist_free(struct dirlist *dlp) {

  if (dlp->free_items) {
    int i;

    for (i = 0; i < dlp->n; i++) {
      free((void *)dlp->items[i].fname);
    }
  }
  free(dlp);
}

static struct diritem *dirlist_add(struct dirlist **dlpp, char *fname) {
  struct dirlist *dlp = *dlpp, *newdlp;
  struct diritem *dip;
  size_t len;

  if (dlp->n >= dlp->max) {
    size_t newsize;

    newsize = sizeof (struct dirlist) +
              ((sizeof (struct diritem)) * dlp->max * 2);
    newdlp = realloc(dlp, newsize);
    if (newdlp == NULL) {
      goto memfail;
    }
    else {
      newdlp->max *= 2;
      *dlpp = dlp = newdlp;
    }
  }

  dip = &dlp->items[dlp->n];
  memset(dip, 0, sizeof (struct diritem));

  len = strlen(fname);
  if (dlp->free_items) {
    char *p = malloc(len + 1);
    if (p == NULL) {
      goto memfail;
    }
    strcpy(p, fname);
    dip->fname = p;
  }
  else {
    dip->fname = fname;
  }
  if (len > dlp->maxlen) {
    dlp->maxlen = len;
  }
  dlp->n++;

  return dip;

memfail:
  dirlist_free(dlp);
  error("out of memory");
  return NULL;
}

static void dostat(struct diritem *dip) {

  (void) dip;
}

static void build_list(const char *path, struct dirlist *dlp) {
  DIR *dirp;
  struct dirent *dep;

  dirp = opendir(path);
   if (dirp == NULL) {
     error(strerror(errno));
   }

   while ((dep = readdir(dirp)) != NULL) {
     struct diritem *dip = dirlist_add(&dlp, dep->d_name);
     dostat(dip);
   }

   closedir(dirp);
}

/*
 * Application entry point.
 */
int main(int argc, char *argv[], char *envp[]) {
  int i, col;

  (void)envp;

  /* Parsing arguments.*/
  argv++;
  argc--;
  while ((argc > 0) && (*argv[0] == '-')) {
    argv[0]++;
    while (*argv[0] != '\0') {
      switch (*argv[0]) {
      case 'l':
        options.lflg = true;
        break;
      default:
        usage();
      }
    }
    argv[0]++;
  }

  /* Case where no paths are specified.*/
//  if (argc == 0) {
//    argc++;
//    argv = &dotp;
//  }

  if (argc > 0) {
    /* Allocating the top level list.*/
    toplist = dirlist_new(8, false);
    if (toplist == NULL) {
      error("out of memory");
    }

    /* Scanning all arguments and populating the array.*/
    for (i = 0; i < argc; i++) {
      struct diritem *dip = dirlist_add(&toplist, *argv++);
      dostat(dip);
    }
  }
  else {
    /* Allocating the top level list.*/
    toplist = dirlist_new(8, true);
    if (toplist == NULL) {
      error("out of memory");
    }

    build_list(".", toplist);
  }

  /* Printing the top level.*/
  col = COLUMNS;
  for (i = 0; i < toplist->n; i++) {
    printf(" %-"COLUMN_SIZE"."COLUMN_SIZE"s", toplist->items[i].fname);
    if (--col <= 0) {
      printf(NEWLINE_STR);
      col = COLUMNS;
    }
  }
  if (col > 0) {
    printf(NEWLINE_STR);
  }

  /* Flushing the standard files.*/
  fflush(NULL);

  /* Not really required, freeing memory.*/
  dirlist_free(toplist);

  exit(0);
}

#if 0
/*
 * Application entry point.
 */
int main(int argc, char *argv[], char *envp[]) {
  const char *path;
  DIR *dirp;
  struct dirent *dep;
  int i;

  (void)envp;

  if (argc > 2) {
    fprintf(stderr, "Usage: ls [<dirpath>]" NEWLINE_STR);
    return 1;
  }

  if (argc == 1) {
    path = ".";
  }
  else {
    path = argv[1];
  }

  dirp = opendir(path);
  if (dirp == NULL) {
    fprintf(stderr, "ls: %s" NEWLINE_STR, strerror(errno));
    return 1;
  }

  i = COLUMNS;
  while ((dep = readdir(dirp)) != NULL) {
    printf(" %-"COLUMN_SIZE"."COLUMN_SIZE"s", dep->d_name);
    if (--i == 0) {
      printf(NEWLINE_STR);
      i = COLUMNS;
    }
  }
  if (i > 0) {
    printf(NEWLINE_STR);
  }
  fflush(NULL);

  closedir(dirp);

  return 0;
}
#endif
