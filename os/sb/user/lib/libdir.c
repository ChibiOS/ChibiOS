/*
    ChibiOS - Copyright (C) 2006..2018 Giovanni Di Sirio

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
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <dirent.h>

#include <reent.h>

DIR *fdopendir(int fd) {
  DIR *dirp;

  if ((dirp = (DIR *)malloc(sizeof (DIR))) == NULL) {
    close (fd);
    return NULL;
  }

  dirp->fd   = fd;
  dirp->next  = 0;

  return dirp;
}

DIR *opendir(const char *name) {
  int fd;

  if ((fd = open(name, O_RDONLY | O_DIRECTORY | O_CLOEXEC)) == -1) {
    return NULL;
  }

  return fdopendir(fd);
}

int closedir (DIR *dirp) {
  int fd = dirp->fd;

  free((void *)dirp);

  return close(fd);
}

struct dirent *readdir (DIR *dirp) {
  extern int _getdents_r(struct _reent *r, int fd, void *dp, int count);

  while (true) {
    struct dirent *dep;

    if (dirp->next == 0) {
      dirp->size = _getdents_r(_REENT, dirp->fd, dirp->buf, DIR_BUF_SIZE);
    }

    if (dirp->size <= 0) {
      return NULL;
    }

    if (dirp->next >= dirp->size) {
      dirp->next = 0;
      continue;
    }

    dep = (struct dirent *)(dirp->buf + dirp->next);
    dirp->next += dep->d_reclen;

    return dep;
  }
}

int chdir(const char *path) {
  extern int _chdir_r(struct _reent *r, const char *path);

  return _chdir_r(_REENT, path);
}

char *getcwd(char *buf, size_t size) {
  extern char *_getcwd_r(struct _reent *r, char *buf, size_t size);

  return _getcwd_r(_REENT, buf, size);
}

int unlink(const char *path) {

  return _unlink_r(_REENT, path);
}

int rename(const char *oldpath,
           const char *newpath) {

  return _rename_r(_REENT, oldpath, newpath);
}

int mkdir(const char *path, mode_t mode) {

  return _mkdir_r(_REENT, path, mode);
}

int rmdir(const char *path) {
  extern int _rmdir_r(struct _reent *r, const char *path);

  return _rmdir_r(_REENT, path);
}

/*** EOF ***/
