/*
    ChibiOS - Copyright (C) 2006..2022 Giovanni Di Sirio

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

static char *dotp = ".";
static void *bufp = NULL;

static bool aflg = false;
static bool dflg = false;
static bool lflg = false;
static bool Uflg = false;

struct afile {
  char          ftype;
  short         fnlink;
  mode_t        fflags;
  off_t         fsize;
  char          *fname;
};

static void usage(void) {
  fprintf(stderr, "Usage: ls [<opts>] [<file>]..." NEWLINE_STR);
  fprintf(stderr, "Options:" NEWLINE_STR);
  fprintf(stderr, "  -a                 do not ignore entries starting with ." NEWLINE_STR);
  fprintf(stderr, "  -d                 list directories themselves, not their contents" NEWLINE_STR);
  fprintf(stderr, "  -U                 do not sort; list entries in directory order" NEWLINE_STR);
  fprintf(stderr, "  -l                 use a long listing format" NEWLINE_STR);
}

static void freeall(void) {

  if (bufp != NULL) {
    free(bufp);
  }
}

static void error(const char *s) {

  fprintf(stderr, "ls: %s" NEWLINE_STR, s);
  exit(1);
}

static bool gstat(struct afile *fp, const char *file, bool flag) {

  memset(fp, 0, sizeof (struct afile));
  fp->ftype = '-';

  if (flag || lflg) {
    int ret;
    struct stat stb;

    ret = stat(file, &stb);
    if (ret < 0) {
      fprintf(stderr, "%s not found\n", file);
      return true;
    }

    fp->fsize  = stb.st_size;
    fp->fflags = stb.st_mode & ~S_IFMT;
    fp->fnlink = stb.st_nlink;
    switch (fp->fflags) {
    case S_IFDIR:
      fp->ftype = 'd';
      break;
    case S_IFBLK:
      fp->ftype = 'b';
      fp->fsize = stb.st_rdev;
      break;
    case S_IFCHR:
      fp->ftype = 'c';
      fp->fsize = stb.st_rdev;
      break;
    case S_IFSOCK:
      fp->ftype = 's';
      fp->fsize = 0;
      break;
    }
  }

  return false;
}

static void formatf(const struct afile *fp0, const struct afile *fplast) {

  (void)fp0;
  (void)fplast;

#if 0
    register struct afile *fp;
    long width = 0, w, nentry = fplast - fp0;
    int i, j, columns, lines;
    char *cp;

    if (fp0 == fplast)
        return;
    if (lflg || Cflg == 0)
        columns = 1;
    else {
        for (fp = fp0; fp < fplast; fp++) {
            long len = strlen(fmtentry(fp));

            if (len > width)
                width = len;
        }
        if (usetabs)
            width = (width + 8) &~ 7;
        else
            width += 2;
        columns = twidth / (int)width;
        if (columns == 0)
            columns = 1;
    }
    lines = ((int)nentry + columns - 1) / columns;
    for (i = 0; i < lines; i++) {
        for (j = 0; j < columns; j++) {
            fp = fp0 + j * lines + i;
            cp = fmtentry(fp);
            printf("%s", cp);
            if (fp + lines >= fplast) {
                printf("\n");
                break;
            }
            w = strlen(cp);
            while (w < width)
                if (usetabs) {
                    w = (w + 8) &~ 7;
                    putchar('\t');
                } else {
                    w++;
                    putchar(' ');
                }
        }
    }
#endif
}

static int fcmp(const void *a, const void *b) {
  const struct afile *f1 = a, *f2 = b;

  /* Directories first then alphabetic order.*/
  if ((f1->ftype == 'd') && (f2->ftype != 'd')) {
    return -1;
  }
  if ((f1->ftype != 'd') && (f2->ftype == 'd')) {
    return 1;
  }

  return strcmp(f1->fname, f2->fname);
}

/*
 * Application entry point.
 */
int main(int argc, char *argv[], char *envp[]) {
  int i;
  struct afile *fp, *fp0, *fplast;

  (void)envp;

  /* Parsing arguments.*/
  argv++;
  argc--;
  while ((argc > 0) && (*argv[0] == '-')) {
    argv[0]++;
    while (*argv[0] != '\0') {
      switch (*argv[0]) {
      case 'a':
        aflg = true;
        break;
      case 'd':
        dflg = true;
        break;
      case 'l':
        lflg = true;
        break;
      case 'U':
        Uflg = true;
        break;
      default:
        usage();
        return 1;
      }
    }
    argv[0]++;
  }

  /* Case where no paths are specified.*/
  if (argc == 0) {
    argc++;
    argv = &dotp;
  }

  /* Allocating a single big buffer for all files.*/
  bufp = calloc(argc, sizeof (struct afile));
  fp0 = (struct afile *)bufp;
  if (bufp == NULL) {
    error("out of memory");
  }

  /* Scanning all arguments and populating the array.*/
  fp = fp0;
  for (i = 0; i < argc; i++) {
      if (gstat(fp, *argv, true)) {
          fp->fname = *argv;
          fp++;
      }
      argv++;
  }
  fplast = fp;

  /* Sorting the array, if not disabled.*/
  if (!Uflg) {
    qsort(fp0, fplast - fp0, sizeof (struct afile), fcmp);
  }

  if (dflg) {
    /* Not entering directories.*/
    formatf(fp0, fplast);
  }
  else {
    /* Entering directories.*/
  }

  freeall();
  return 0;
#if 0
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

  while ((dep = readdir(dirp)) != NULL) {
    printf("%s" NEWLINE_STR, dep->d_name);
  }

  closedir(dirp);
#endif
}
