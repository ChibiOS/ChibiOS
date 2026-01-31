/*
    Copyright (C) 2006-2026 Giovanni Di Sirio.

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

/* Option flags, all false initially.*/
struct {
  bool              aflg;
  bool              dflg;
  bool              fflg;
  bool              lflg;
  bool              yflg;
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
  char              *path;
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
  fprintf(stderr, "  -d                 list directories themselves, not their contents" NEWLINE_STR);
  fprintf(stderr, "  -f                 do not sort, enable -a and -d, disable -l" NEWLINE_STR);
  fprintf(stderr, "  -l                 use a long listing format" NEWLINE_STR);
  fprintf(stderr, "  -y                 enforce plain text output (non standard option)" NEWLINE_STR);
  exit(1);
}

static void error(const char *s) {

  fprintf(stderr, "ls: fatal error; %s" NEWLINE_STR, s);
  exit(1);
}

static void newline(void) {

  printf(NEWLINE_STR);
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

  dlp->path = NULL;
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

  if (dlp->path != NULL) {
    free(dlp->path);
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

static void dirlist_undo(struct dirlist **dlpp) {

  (*dlpp)->n--;
}

static char *new_append_path(char *dir, char *file) {
  char *p;
  size_t ld, lf;

  ld = strlen(dir);
  lf = strlen(file);
  p = malloc(ld + 1 + lf + 1);
  if (p == NULL) {
    error("out of memory");
  }

  if (!strcmp(dir, "") || !strcmp(dir, ".")) {
    strcpy(p, file);
  }
  else {
    strcpy(p, dir);
    if ((dir[ld - 1] != '/') && (file[0] != '/')) {
      strcat(p, "/");
    }
    strcat(p, file);
  }

  return p;
}

static char *new_path(char *path) {
  char *p;
  size_t l;

  l = strlen(path);
  p = malloc(l + 1);
  if (p == NULL) {
    error("out of memory");
  }
  strcpy(p, path);

  return p;
}

static bool dostat( struct dirlist *dlp, struct diritem *dip) {
  struct stat stb;
  char *path;

  path = new_append_path(dlp->path, dip->fname);
  if (stat(path, &stb) < 0) {
    free(path);
    fprintf(stderr, "ls: cannot access '%s': %s" NEWLINE_STR, dip->fname, strerror(errno));
    return false;
  }

  free(path);

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

static bool ignore_item(char *p) {

  if (p[0] == '.') {
    /* . and .. always ignored.*/
    if ((p[1] == '\0') || ((p[1] == '.') && (p[2] == '\0'))) {
      return true;
    }

    /* Anything starting with . is ignore unless the -a option is specified.*/
    if (options.aflg == false) {
      return true;
    }
  }

  return false;
}

static void build_list_from_args(char *argv[], struct dirlist **dlpp) {

  (*dlpp)->path = new_path(".");

  while (*argv != NULL) {
    char *p = *argv++;
    struct diritem *dip = dirlist_add(dlpp,p);

    if (ignore_item(p)) {
      dirlist_undo(dlpp);
      continue;
    }

    if (!dostat(*dlpp, dip)) {
      dirlist_undo(dlpp);
    }
  }

  if (options.fflg == false) {
    struct dirlist *dlp = *dlpp;
    qsort(&dlp->items[0], dlp->n, sizeof(struct diritem), fcmp);
  }
}

static void build_list_from_path(char *path, struct dirlist **dlpp) {
  DIR *dirp;
  struct dirent *dep;

  (*dlpp)->path = path;

  dirp = opendir(path);
  if (dirp == NULL) {
    error(strerror(errno));
  }

  while ((dep = readdir(dirp)) != NULL) {
    struct diritem *dip = dirlist_add(dlpp, dep->d_name);

    if (ignore_item(dep->d_name)) {
      dirlist_undo(dlpp);
      continue;
    }

#ifndef __MINGW32__
    /* If the -l option is set or the readdir() is unable to return the
       file type a stat() is needed.*/
    if ((options.lflg == true) || (dep->d_type == DT_UNKNOWN))  {
      if (!dostat(*dlpp, dip)) {
        dirlist_undo(dlpp);
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
    if (!dostat(*dlpp, dip)) {
      dirlist_undo(dlpp);
    }
#endif
  }

  closedir(dirp);

  if (options.fflg == false) {
    struct dirlist *dlp = *dlpp;
    qsort(&dlp->items[0], dlp->n, sizeof(struct diritem), fcmp);
  }
}

static void printnames(struct dirlist *dlp, bool listdirs) {
  int i, j, cols, col;

  col = dlp->maxlen + 1;
  cols = TERMWIDTH / col;
  i = 0;
  while (i < dlp->n) {
    j = 0;
    while ((j < cols) && (i < dlp->n)) {
      struct diritem *dip = &dlp->items[i];
      if (dip->ftype == 'd') {
        if (listdirs) {
          /* Printing subdir.*/
          struct dirlist *sdlp;

          sdlp = dirlist_new(8, true);
          build_list_from_path(new_append_path(dlp->path, dip->fname), &sdlp);

          printf("%s:" NEWLINE_STR, dip->fname);
          printnames(sdlp, false);
          newline();
          dirlist_free(sdlp);
          i++;
          j = 0;
          continue;
        }
        else {
          /* Printing dir name only.*/
          if (options.yflg == false) {
            printf("\033[1m%-*.*s\033[0m ", col, col, dip->fname);
          }
          else  {
            printf("%-*.*s ", col, col, dip->fname);
          }
        }
      }
      else {
        printf("%-*.*s ", col, col, dip->fname);
      }
      j++, i++;
    }
    newline();
  }
}

static void printlist(struct dirlist *dlp, bool listdirs) {
  int i;

  (void)listdirs; /* Always disabled in current implementation.*/

  i = 0;
  while (i < dlp->n) {
    struct diritem *dip = &dlp->items[i];

    /* Type char.*/
    if (dip->ftype != 'r') {
      putchar(dip->ftype);
    }
    else {
      putchar('-');
    }

    /* Modes.*/
    if ((dip->fflags & S_IRUSR) != (mode_t)0) {
      putchar('r');
    }
    else {
      putchar('-');
    }
    if ((dip->fflags & S_IWUSR) != (mode_t)0) {
      putchar('w');
    }
    else {
      putchar('-');
    }
#ifndef __MINGW32__
    if ((dip->fflags & S_IXUSR) != (mode_t)0) {
      putchar(((dip->fflags & S_ISUID) == (mode_t)0) ? 'x' : 's');
    }
    else {
      putchar('-');
    }
    if ((dip->fflags & S_IRGRP) != (mode_t)0) {
      putchar('r');
    }
    else {
      putchar('-');
    }
    if ((dip->fflags & S_IWGRP) != (mode_t)0) {
      putchar('w');
    }
    else {
      putchar('-');
    }
    if ((dip->fflags & S_IXGRP) != (mode_t)0) {
      putchar(((dip->fflags & S_ISGID) == (mode_t)0) ? 'x' : 's');
    }
    else {
      putchar('-');
    }
    if ((dip->fflags & S_IROTH) != (mode_t)0) {
      putchar('r');
    }
    else {
      putchar('-');
    }
    if ((dip->fflags & S_IWOTH) != (mode_t)0) {
      putchar('w');
    }
    else {
      putchar('-');
    }
    if ((dip->fflags & S_IXOTH) != (mode_t)0) {
      putchar('x');
    }
    else {
      putchar('-');
    }
#else
    printf("-------");
#endif

    /* Links.*/
//    printf("%3d ", 1);

    /* User and group.*/
//    printf("root root ");

    /* Size field.*/
    if ((dip->ftype) == 'b' || (dip->ftype == 'c')) {
      printf("  %3u,%4u ", /*major(p->fsize)*/0, /*minor(p->fsize)*/0);
    }
    else if (dip->ftype == 's') {
      printf("%9u ", 0);
    }
    else {
      printf("%9u ", (unsigned)dip->fsize);
    }

    /* File name.*/
    if (options.yflg == false) {
      if (dip->ftype == 'd') {
        printf("\033[1m%s\033[0m" NEWLINE_STR, dip->fname);
      }
      else {
        printf("%s" NEWLINE_STR, dip->fname);
      }
    }
    else {
      printf("%s" NEWLINE_STR, dip->fname);
    }
    i++;
  }
}

/*
 * Application entry point.
 */
int main(int argc, char *argv[], char *envp[]) {

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
      case 'd':
        options.dflg = true;
        break;
      case 'f':
        options.fflg = true;
        break;
      case 'l':
        options.lflg = true;
        break;
      case 'y':
        options.yflg = true;
        break;
      default:
        usage();
      }
      argv[0]++;
    }
    argv++;
    argc--;
  }

  /* The "f" flag has side effects.*/
  if (options.fflg) {
    options.aflg = true;
    options.dflg = true;
    options.lflg = false;
  }

  /* The "l" flag also implies "d" for simplicity.*/
  if (options.lflg) {
    options.dflg = true;
  }

  if (argc > 0) {
    /* Allocating the top level list.*/
    toplist = dirlist_new(8, false);
    if (toplist == NULL) {
      error("out of memory");
    }

    build_list_from_args(argv, &toplist);
  }
  else {
    /* Allocating the top level list.*/
    toplist = dirlist_new(8, true);
    if (toplist == NULL) {
      error("out of memory");
    }

    build_list_from_path(new_path("."), &toplist);
    options.dflg = true;
  }

  /* Printing the top level.*/
  if (options.lflg) {
    printlist(toplist, !options.dflg);
  }
  else {
    printnames(toplist, !options.dflg);
  }

  /* Flushing the standard files.*/
  fflush(NULL);

  /* Not really required, freeing memory.*/
  dirlist_free(toplist);

  exit(0);
}
