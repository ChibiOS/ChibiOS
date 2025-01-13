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

#include <sys/stat.h>

#define NEWLINE_STR         "\r\n"
#define TERMWIDTH           80
#define COLUMNS             6
#define COLUMN_SIZE         "11"

/* Option flags, all false initially.*/
struct {
  bool              aflg;
  bool              fflg;
  bool              lflg;
} options;

struct diritem {
  char              ftype;
  ino_t             fnum;
//  nlink_t           fnlink;
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
  fprintf(stderr, "  -a                 do not ignore entries starting with ." NEWLINE_STR);
  fprintf(stderr, "  -f                 do not sort, enable -a, disable -l" NEWLINE_STR);
  fprintf(stderr, "  -l                 use a long listing format" NEWLINE_STR);
  exit(1);
}

static void error(const char *s) {

  fprintf(stderr, "ls: fatal error; %s" NEWLINE_STR, s);
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

static void dirlist_undo(struct dirlist *dlp) {

  dlp->n--;
}

static bool dostat(struct diritem *dip) {
  struct stat stb;

  if (stat(dip->fname, &stb) < 0) {
    fprintf(stderr, "ls: cannot access '%s': %s" NEWLINE_STR, dip->fname, strerror(errno));
    return false;
  }

  dip->fnum = stb.st_ino;
  dip->fflags = stb.st_mode & ~S_IFMT;
  dip->fsize = stb.st_size;
  switch (stb.st_mode & S_IFMT) {
#ifndef __MINGW32__
  case S_IFSOCK:
    dip->ftype = 's';
    dip->fsize = 0;
    break;
  case S_IFLNK:
    dip->ftype = 'l';
    dip->fsize = 0;
    break;
#endif
  case S_IFREG:
    dip->ftype = 'r';
    break;
  case S_IFBLK:
    dip->ftype = 'b';
    dip->fsize = 0; //stb.st_rdev;
    break;
  case S_IFDIR:
    dip->ftype = 'd';
    break;
  case S_IFCHR:
    dip->ftype = 'c';
    dip->fsize = 0; //stb.st_rdev;
    break;
  case S_IFIFO:
    dip->ftype = 'f';
    dip->fsize = 0;
    break;
  default:
    dip->ftype = '-';
    dip->fsize = 0;
  }

  return true;
}

static int fcmp(const void *a, const void *b) {
  const struct diritem *f1 = a, *f2 = b;

  /* Directories first then alphabetic order.*/
  if ((f1->ftype == 'd') && (f2->ftype != 'd')) {
    return -1;
  }
  if ((f1->ftype != 'd') && (f2->ftype == 'd')) {
    return 1;
  }

  return strcmp(f1->fname, f2->fname);
}

static void build_list_from_path(const char *path, struct dirlist *dlp) {
  DIR *dirp;
  struct dirent *dep;

  dirp = opendir(path);
  if (dirp == NULL) {
    error(strerror(errno));
  }

  while ((dep = readdir(dirp)) != NULL) {
    struct diritem *dip = dirlist_add(&dlp, dep->d_name);

    if ((options.aflg == false) &&
        (dep->d_name[0] == '.') &&
        ((dep->d_name[1] == 0) || ((dep->d_name[1] == '.') &&
                                    (dep->d_name[2] == 0)))) {
      dirlist_undo(dlp);
      continue;
    }

#ifndef __MINGW32__
     /* If the -l option is set or the readdir() is unable to return the
        file type a stat() is needed.*/
     if ((options.lflg == true) || (dep->d_type == DT_UNKNOWN))  {
       if (!dostat(dip)) {
         dirlist_undo(dlp);
       }
     }
     else{
       switch (dep->d_type) {
       case DT_LNK:
         dip->ftype = 'l';
         break;
       case DT_REG:
         dip->ftype = 'r';
         break;
       case DT_BLK:
         dip->ftype = 'b';
         break;
       case DT_DIR:
         dip->ftype = 'd';
         break;
       case DT_CHR:
         dip->ftype = 'c';
         break;
       case DT_FIFO:
         dip->ftype = 'f';
         break;
       default:
         dip->ftype = '-';
       }
     }
#else
    if (!dostat(dip)) {
      dirlist_undo(dlp);
    }
#endif
  }

  closedir(dirp);

  if (options.fflg == false) {
    qsort(&dlp->items[0], dlp->n, sizeof(struct diritem), fcmp);
  }
}

static void printlist(struct dirlist *dlp) {
  int i, j, cols, col;

  col = dlp->maxlen + 1;
  cols = TERMWIDTH / col;
  i = 0;
  while (i < dlp->n) {
    j = 0;
    while ((j < cols) && (i < dlp->n)) {
      printf(" %-*.*s", col, col, dlp->items[i].fname);
      j++, i++;
    }
    printf(NEWLINE_STR);
  }
}

/*
 * Application entry point.
 */
int main(int argc, char *argv[], char *envp[]) {
  int i; //, col;

  (void)envp;

  /* Parsing arguments.*/
  argv++;
  argc--;
  while ((argc > 0) && (*argv[0] == '-')) {
    argv[0]++;
    while (*argv[0] != '\0') {
      switch (*argv[0]) {
      case 'a':
        options.aflg = true;
        break;
      case 'f':
        options.fflg = true;
        break;
      case 'l':
        options.lflg = true;
        break;
      default:
        usage();
      }
    }
    argv[0]++;
  }

  /* The "f" flag has side effects.*/
  if (options.fflg) {
    options.aflg = true;
    options.lflg = false;
  }

  if (argc > 0) {
    /* Allocating the top level list.*/
    toplist = dirlist_new(8, false);
    if (toplist == NULL) {
      error("out of memory");
    }

    /* Scanning all arguments and populating the array.*/
    for (i = 0; i < argc; i++) {
      struct diritem *dip = dirlist_add(&toplist, *argv++);
      if (!dostat(dip)) {
        toplist->n--;
      }
    }
  }
  else {
    /* Allocating the top level list.*/
    toplist = dirlist_new(8, true);
    if (toplist == NULL) {
      error("out of memory");
    }

    build_list_from_path(".", toplist);
  }

  /* Printing the top level.*/
  printlist(toplist);

  /* Flushing the standard files.*/
  fflush(NULL);

  /* Not really required, freeing memory.*/
  dirlist_free(toplist);

  exit(0);
}
