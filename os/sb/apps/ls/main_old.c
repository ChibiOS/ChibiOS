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
#define TWIDTH              80

struct afile {
  char              ftype;
  long              fnum;
  short             fnlink;
  mode_t            fflags;
  off_t             fsize;
  char              *fname;
};

static char *dotp = ".";
static struct afile *bufp = NULL;

/* Option flags, all false initially.*/
struct {
  bool                      Rflg;
  bool                      aflg;
  bool                      dflg;
  bool                      fflg;
  bool                      lflg;
  bool                      qflg;
} options;

#define ISARG               0x8000      /* extra ``mode'' */
#define dbtob(n)            ((n) * 512)
#define kbytes(size)        (((size) + 1023L) / 1024L)

struct subdirs {
  char              *sd_name;
  struct  subdirs   *sd_next;
} *subdirs;

static void usage(void) {
  fprintf(stderr, "Usage: ls [<opts>] [<file>]..." NEWLINE_STR);
  fprintf(stderr, "Options:" NEWLINE_STR);
  fprintf(stderr, "  -R                 list subdirectories recursively" NEWLINE_STR);
  fprintf(stderr, "  -a                 do not ignore entries starting with ." NEWLINE_STR);
  fprintf(stderr, "  -d                 list directories themselves, not their contents" NEWLINE_STR);
  fprintf(stderr, "  -f                 do not sort, enable -a, disable -l" NEWLINE_STR);
  fprintf(stderr, "  -l                 use a long listing format" NEWLINE_STR);
  fprintf(stderr, "  -q                 print ? instead of nongraphic characters" NEWLINE_STR);
}

static void error(const char *s) {

  fprintf(stderr, "ls: %s" NEWLINE_STR, s);
  exit(1);
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

bool gstat(struct afile *fp, char *file, bool statarg) {
  static struct afile azerofile;

  *fp = azerofile;
  fp->fflags = 0;
  fp->fnum = 0;
  fp->ftype = '-';
  if (statarg || options.lflg) {
    struct stat stb;

    if (stat(file, &stb) < 0) {
      fprintf(stderr, "%s not found" NEWLINE_STR, file);
      return false;
    }
    fp->fsize = stb.st_size;
    switch (stb.st_mode & S_IFMT) {
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
    fp->fnum = stb.st_ino;
    fp->fflags = stb.st_mode & ~S_IFMT;
  }
  return true;
}

static char *fmtmode(char *p, mode_t flags) {

  if ((flags & S_IRUSR) != (mode_t)0) {
    *p++ = 'r';
  }
  if ((flags & S_IWUSR) != (mode_t)0) {
    *p++ = 'w';
  }
  if ((flags & S_IXUSR) != (mode_t)0) {
    *p++ = ((flags & S_ISUID) == (mode_t)0) ? 'x' : 's';
  }
  if ((flags & S_IRGRP) != (mode_t)0) {
    *p++ = 'r';
  }
  if ((flags & S_IWGRP) != (mode_t)0) {
    *p++ = 'w';
  }
  if ((flags & S_IXGRP) != (mode_t)0) {
    *p++ = ((flags & S_ISGID) == (mode_t)0) ? 'x' : 's';
  }
  if ((flags & S_IROTH) != (mode_t)0) {
    *p++ = 'r';
  }
  if ((flags & S_IWOTH) != (mode_t)0) {
    *p++ = 'w';
  }
  if ((flags & S_IXOTH) != (mode_t)0) {
    *p++ = 'x';
  }

  return p;
}

static char *fmtlstuff(const struct afile *p) {
  static char lstuffbuf[TWIDTH];
  static char gname[32], uname[32], fsize[32], ftime[32];
  char *lp = lstuffbuf;
  int n;

  n = TWIDTH;

  /* type mode uname gname fsize ftime */
  /* get uname */
  {
    char *cp = /*getname(p->fuid)*/"root";
    if (cp)
      n -= snprintf(uname, n, "%-9.9s", cp);
    else
      n -= snprintf(uname, n, "%-9d",/* p->fuid*/0);
  }

  /* get gname */
  {
    char *cp = /*getgroup(p->fgid)*/"root";
    if (cp)
      n -= snprintf(gname, n, "%-9.9s", cp);
    else
      n -= snprintf(gname, n, "%-9d", /*p->fgid*/0);
  }

  /* get fsize */
  if (p->ftype == 'b' || p->ftype == 'c') {
    n -= snprintf(fsize, n, "%3d,%4d", /*major(p->fsize)*/0, /*minor(p->fsize)*/0);
  }
  else if (p->ftype == 's') {
    n -= snprintf(fsize, n, "%8d", 0);
  }
  else {
    n -= snprintf(fsize, n, "%8d", (int)p->fsize);
  }

  /* get ftime */
  {
#if 0
    char *cp = ctime(&p->fmtime);
    if ((p->fmtime < sixmonthsago) || (p->fmtime > now))
      (void)sprintf(ftime, " %-7.7s %-4.4s ", cp + 4, cp + 20);
    else
      (void)sprintf(ftime, " %-12.12s ", cp + 4);
#endif
    n += snprintf(ftime, n, " %-7.7s %-4.4s ", "Jan   1", "1990");
  }

  /* splat */
  *lp++ = p->ftype;
  lp = fmtmode(lp, p->fflags);
  (void) snprintf(lp, n, "%3d %s%s%s%s", /*p->fnl*/0, uname, gname, fsize, ftime);

  return lstuffbuf;
}

static char *fmtentry(const struct afile *fp) {
  static char fmtres[TWIDTH];
  register char *cp, *dp;

  (void) snprintf(fmtres, TWIDTH, "%s%s%s",
                  /*iflg ? fmtinum(fp) :*/ "",
                  /*sflg ? fmtsize(fp) :*/ "",
                  options.lflg ? fmtlstuff(fp) : "");

  dp = &fmtres[strlen(fmtres)];
  for (cp = fp->fname; *cp; cp++) {
    if (options.qflg && ((*cp < ' ') || (*cp >= 0x7F))) {
      *dp++ = '?';
    }
    else {
      *dp++ = *cp;
    }
  }
  *dp++ = 0;

  return fmtres;
}

static void formatf(const struct afile *fp0, const struct afile *fplast) {
  int n;

  n = (int)(fplast - fp0);
  if (n > 0) {
    const struct afile *fp;
    int i, j, columns, lines, width;

    /* Determining number and size of columns.*/
    if (options.lflg) {
      columns = 1;
    }
    else {
      width = 0;
      for (fp = fp0; fp < fplast; fp++) {
        int len;

        len = (int)strlen(fmtentry(fp));
        if (len > width) {
          width = len;
        }
        width += 2;
        columns = TWIDTH / width;
        if (columns == 0) {
          columns = 1;
        }
      }
    }

    lines = (n + columns - 1) / columns;
    for (i = 0; i < lines; i++) {
      for (j = 0; j < columns; j++) {
        int w;
        char *cp;

        fp = fp0 + j * lines + i;
        cp = fmtentry(fp);
        printf("%s", cp);
        if (fp + lines >= fplast) {
          printf(NEWLINE_STR);
          break;
        }
        w = (int)strlen(cp);
        while (w < width) {
          w++;
          putchar(' ');
        }
      }
    }
  }
}

static char* cat(char *dir, char *file) {
  static char dfile[BUFSIZ];

  if (strlen(dir) + 1 + strlen(file) + 1 > BUFSIZ) {
    error("filename too long");
  }

  if (!strcmp(dir, "") || !strcmp(dir, ".")) {
    (void)strcpy(dfile, file);
    return dfile;
  }

  (void)strcpy(dfile, dir);
  if (dir[strlen(dir) - 1] != '/' && *file != '/') {
    (void)strcat(dfile, "/");
  }

  (void)strcat(dfile, file);

  return dfile;
}

static char *savestr(char *str) {
  char *cp = malloc(strlen(str) + 1);

  if (cp == NULL) {
    error("out of memory");
  }

  (void)strcpy(cp, str);
  return cp;
}

static long getdir(char *dir, struct afile **pfp0, struct afile **pfplast) {
  struct afile *fp;
  DIR *dirp;
  struct dirent *dp;
  long nb, nent = 20;

  dirp = opendir(dir);
  if (dirp == NULL) {
    *pfp0 = *pfplast = NULL;
    printf("%s unreadable" NEWLINE_STR, dir); /* not stderr! */
    return 0;
  }

  fp = *pfp0 = (struct afile*)calloc(nent, sizeof(struct afile));
  if (fp == NULL) {
    error("out of memory");
  }
  *pfplast = *pfp0 + nent;

  nb = 0;
  while ((dp = readdir(dirp)) != NULL) {
    if (dp->d_ino == 0) {
      continue;
    }

    if ((options.aflg == false) &&
        (dp->d_name[0] == '.') &&
        ((dp->d_name[1] == 0) || ((dp->d_name[1] == '.') &&
                                  (dp->d_name[2] == 0)))) {
        continue;
    }

    if (gstat(fp, cat(dir, dp->d_name), /*Fflg + */options.Rflg/*, &nb*/)) {
      continue;
    }

    fp->fnum = dp->d_ino;
    fp->fname = savestr(dp->d_name);
    fp++;
    if (fp == *pfplast) {
      *pfp0 = (struct afile*)realloc((char*)*pfp0,
                                     2 * nent * sizeof(struct afile));
      if (*pfp0 == NULL) {
        error("out of memory");
      }
      fp = *pfp0 + nent;
      *pfplast = fp + nent;
      nent *= 2;
    }
  }
  closedir(dirp);
  *pfplast = fp;

  return kbytes(dbtob(nb));
}

static void formatd(char *name, int title) {
  struct afile *fp;
  struct subdirs *dp;
  struct afile *dfp0, *dfplast;
  long nkb;

  nkb = getdir(name, &dfp0, &dfplast);
  if (dfp0 == NULL) {
    return;
  }

  if (options.fflg == 0) {
    qsort(dfp0, dfplast - dfp0, sizeof(struct afile), fcmp);
  }

  if (title) {
    printf("%s:" NEWLINE_STR, name);
  }

  if (options.lflg/* || sflg*/) {
    printf("total %ld" NEWLINE_STR, nkb);
  }

  formatf(dfp0, dfplast);
  if (options.Rflg)
    for (fp = dfplast - 1; fp >= dfp0; fp--) {
      if (fp->ftype != 'd' || !strcmp(fp->fname, ".") || !strcmp(fp->fname, "..")) {
        continue;
      }

      dp = (struct subdirs *)malloc(sizeof(struct subdirs));
      if (dp == NULL) { /*PATCH GIOV.*/
        error("out of memory");
      }

      dp->sd_name = savestr(cat(name, fp->fname));
      dp->sd_next = subdirs;
      subdirs = dp;
    }
  for (fp = dfp0; fp < dfplast; fp++) {
    if ((fp->fflags & ISARG) == 0 && fp->fname) {
      free(fp->fname);
//    if (fp->flinkto)
//      free(fp->flinkto);
    }
  }
  free((char*)dfp0);
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
      case 'R':
        options.Rflg = true;
        break;
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
      case 'q':
        options.qflg = true;
        break;
      default:
        usage();
        return 1;
      }
    }
    argv[0]++;
  }

  /* The "f" flag has side effects.*/
  if (options.fflg) {
    options.aflg = true;
    options.lflg = false;
    /*options.sflg = false;*/
    /*options.tflg = false*/;
  }

  /* Case where no paths are specified.*/
  if (argc == 0) {
    argc++;
    argv = &dotp;
  }

  /* Allocating a single big buffer for all files.*/
  bufp = calloc(argc, sizeof (struct afile));
  if (bufp == NULL) {
    error("out of memory");
  }

  /* Scanning all arguments and populating the array.*/
  fp0 = fp = bufp;
  for (i = 0; i < argc; i++) {
    if (!gstat(fp, *argv, true)) {
      fp->fname = *argv;
      fp->fflags |= ISARG;
      fp++;
    }
    argv++;
  }
  fplast = fp;

  /* Sorting the array, if not disabled.*/
  if (!options.fflg) {
    qsort(fp0, fplast - fp0, sizeof (struct afile), fcmp);
  }

  if (options.fflg) {
    fp = fp0;
  }
  else {
    for (fp = fp0; fp < fplast && fp->ftype != 'd'; fp++)
      continue;
    formatf(fp0, fp);
  }

  if (fp < fplast) {
    if (fp > fp0) {
      printf(NEWLINE_STR);
    }
    for (;;) {
      formatd(fp->fname, argc > 1);
      while (subdirs) {
        struct subdirs *t;

        t = subdirs;
        subdirs = t->sd_next;
        printf(NEWLINE_STR);
        formatd(t->sd_name, 1);
        free(t->sd_name);
        free(t);
      }
      if (++fp == fplast) {
        break;
      }
      printf(NEWLINE_STR);
    }
  }

  return 0;
}
