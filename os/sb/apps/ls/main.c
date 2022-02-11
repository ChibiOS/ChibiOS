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

#define S_ISARG             0x8000

static char *dotp = ".";
static void *bufp = NULL;

static bool lflag = false;
static bool aflag = false;

struct afile {
  char          ftype;
  short         nlink;
  mode_t        fflags;
  off_t         size;
  char          *fname;
};

static void usage(void) {
  fprintf(stderr, "Usage: ls [<opts>] [<file>]..." NEWLINE_STR);
  fprintf(stderr, "Options:" NEWLINE_STR);
  fprintf(stderr, "  -a                 do not ignore entries starting with ." NEWLINE_STR);
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

  if (flag) {

  }

  return false;
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
      case 'l':
        lflag = true;
        break;
      case 'a':
        aflag = true;
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
          fp->fflags |= S_ISARG;
          fp++;
      }
      argv++;
  }
  fplast = fp;

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
