/*
    ChibiOS - Copyright (C) 2006-2026 Giovanni Di Sirio.

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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>

#define NEWLINE_STR         "\r\n"

typedef struct {
  bool                    aflg;
  bool                    dflg;
  bool                    lflg;
} options_t;

static void usage(void) {

  fprintf(stderr, "Usage: ls [-adl] [<path>]..." NEWLINE_STR);
}

static void report_error(const char *path) {

  fprintf(stderr, "ls: %s: %s" NEWLINE_STR, path, strerror(errno));
}

static bool is_hidden_name(const char *name) {

  return name[0] == '.';
}

static char get_ftype(mode_t mode) {

  if (S_ISDIR(mode)) {
    return 'd';
  }
  if (S_ISCHR(mode)) {
    return 'c';
  }
  if (S_ISBLK(mode)) {
    return 'b';
  }
  if (S_ISFIFO(mode)) {
    return 'f';
  }
#ifndef __MINGW32__
  if (S_ISLNK(mode)) {
    return 'l';
  }
  if (S_ISSOCK(mode)) {
    return 's';
  }
#endif

  return '-';
}

static void format_mode(mode_t mode, char buf[10]) {

  buf[0] = ((mode & S_IRUSR) != 0) ? 'r' : '-';
  buf[1] = ((mode & S_IWUSR) != 0) ? 'w' : '-';
  buf[2] = ((mode & S_IXUSR) != 0) ? 'x' : '-';
  buf[3] = ((mode & S_IRGRP) != 0) ? 'r' : '-';
  buf[4] = ((mode & S_IWGRP) != 0) ? 'w' : '-';
  buf[5] = ((mode & S_IXGRP) != 0) ? 'x' : '-';
  buf[6] = ((mode & S_IROTH) != 0) ? 'r' : '-';
  buf[7] = ((mode & S_IWOTH) != 0) ? 'w' : '-';
  buf[8] = ((mode & S_IXOTH) != 0) ? 'x' : '-';
  buf[9] = '\0';
}

static void print_entry(const char *name, const struct stat *stp,
                        const options_t *optsp) {

  if (optsp->lflg) {
    char modebuf[10];

    format_mode(stp->st_mode & (mode_t)0777, modebuf);
    printf("%c%s %8ld %s" NEWLINE_STR,
           get_ftype(stp->st_mode),
           modebuf,
           (long)stp->st_size,
           name);
  }
  else {
    printf("%s" NEWLINE_STR, name);
  }
}

static char *join_path(const char *dir, const char *name) {
  size_t ldir, lname;
  char *path;
  bool needs_slash;

  ldir = strlen(dir);
  lname = strlen(name);
  needs_slash = (bool)((ldir > 0U) && (dir[ldir - 1U] != '/'));

  path = malloc(ldir + (needs_slash ? 1U : 0U) + lname + 1U);
  if (path == NULL) {
    fprintf(stderr, "ls: out of memory" NEWLINE_STR);
    return NULL;
  }

  memcpy(path, dir, ldir);
  if (needs_slash) {
    path[ldir++] = '/';
  }
  memcpy(&path[ldir], name, lname);
  path[ldir + lname] = '\0';

  return path;
}

static int list_directory(const char *path, const options_t *optsp) {
  DIR *dirp;
  struct dirent *dep;

  dirp = opendir(path);
  if (dirp == NULL) {
    report_error(path);
    return 1;
  }

  while ((dep = readdir(dirp)) != NULL) {
    struct stat st;
    char *fullpath;

    if (!optsp->aflg && is_hidden_name(dep->d_name)) {
      continue;
    }

    fullpath = join_path(path, dep->d_name);
    if (fullpath == NULL) {
      closedir(dirp);
      return 1;
    }

    if (stat(fullpath, &st) < 0) {
      report_error(fullpath);
      free(fullpath);
      continue;
    }

    print_entry(dep->d_name, &st, optsp);
    free(fullpath);
  }

  closedir(dirp);

  return 0;
}

static int handle_path(const char *path, const options_t *optsp) {
  struct stat st;

  if (stat(path, &st) < 0) {
    report_error(path);
    return 1;
  }

  if (!S_ISDIR(st.st_mode) || optsp->dflg) {
    print_entry(path, &st, optsp);
    return 0;
  }

  return list_directory(path, optsp);
}

static int parse_options(int argc, char *argv[], options_t *optsp, int *argip) {
  int i;

  memset(optsp, 0, sizeof (*optsp));

  for (i = 1; i < argc; i++) {
    const char *p = argv[i];

    if (p[0] != '-') {
      break;
    }
    if ((p[1] == '\0') || ((p[1] == '-') && (p[2] == '\0'))) {
      if (p[1] == '-') {
        i++;
      }
      break;
    }

    p++;
    while (*p != '\0') {
      switch (*p++) {
      case 'a':
        optsp->aflg = true;
        break;
      case 'd':
        optsp->dflg = true;
        break;
      case 'l':
        optsp->lflg = true;
        break;
      default:
        usage();
        return 1;
      }
    }
  }

  *argip = i;
  return 0;
}

int main(int argc, char *argv[], char *envp[]) {
  options_t options;
  int argi, ret, exsts;
  bool show_header, first_path;

  (void)envp;

  ret = parse_options(argc, argv, &options, &argi);
  if (ret != 0) {
    return ret;
  }

  exsts = 0;
  show_header = (bool)((argc - argi) > 1);
  first_path = true;

  if (argi >= argc) {
    return handle_path(".", &options);
  }

  for (; argi < argc; argi++) {
    if (show_header) {
      if (!first_path) {
        printf(NEWLINE_STR);
      }
      printf("%s:%s", argv[argi], NEWLINE_STR);
    }

    ret = handle_path(argv[argi], &options);
    if (ret != 0) {
      exsts = ret;
    }
    first_path = false;
  }

  return exsts;
}
