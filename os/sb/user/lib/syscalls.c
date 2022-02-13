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

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "errcodes.h"
#include "sbuser.h"
#include "syscalls.h"

uint8_t *__sbrk_next = &__heap_base__;

__attribute__((used))
int _open_r(struct _reent *r, const char *p, int oflag, int mode) {
  int err;

  (void)mode;

  err = sbOpen(p, oflag);
  if (CH_RET_IS_ERROR(err)) {
    __errno_r(r) = CH_DECODE_ERROR(err);
    return -1;
  }

  return err;
}

__attribute__((used))
int _close_r(struct _reent *r, int file) {
  int err;

  err = sbClose(file);
  if (CH_RET_IS_ERROR(err)) {
    __errno_r(r) = CH_DECODE_ERROR(err);
    return -1;
  }

  return 0;
}

__attribute__((used))
int _write_r(struct _reent *r, int file, char *ptr, int len) {
  int err;

  err = sbWrite(file, (const void *)ptr, (size_t)len);
  if (CH_RET_IS_ERROR(err)) {
    __errno_r(r) = CH_DECODE_ERROR(err);
    return -1;
  }

  return (int)err;
}

__attribute__((used))
int _read_r(struct _reent *r, int file, char *ptr, int len) {
  int err;

  err = sbRead(file, (void *)ptr, (size_t)len);
  if (CH_RET_IS_ERROR(err)) {
    __errno_r(r) = CH_DECODE_ERROR(err);
    return -1;
  }

  return (int)err;
}

__attribute__((used))
int _lseek_r(struct _reent *r, int file, int ptr, int dir) {
  int err;

  err = sbSeek(file, ptr, dir);
  if (CH_RET_IS_ERROR(err)) {
    __errno_r(r) = CH_DECODE_ERROR(err);
    return -1;
  }

  return (int)err;
}

__attribute__((used))
int _fstat_r(struct _reent *r, int file, struct stat *pstat) {
  int err;

  err = sbFstat(file, pstat);
  if (CH_RET_IS_ERROR(err)) {
    __errno_r(r) = CH_DECODE_ERROR(err);
    return -1;
  }

  return (int)err;
}

__attribute__((used))
int _stat_r(struct _reent *r, const char *file, struct stat *pstat) {
  int err;

  err = sbStat(file, pstat);
  if (CH_RET_IS_ERROR(err)) {
    __errno_r(r) = CH_DECODE_ERROR(err);
    return -1;
  }

  return (int)err;
}

__attribute__((used))
int _isatty_r(struct _reent *r, int fd) {
  int err;
  struct stat s;

  err = sbFstat(fd, &s);
  if (CH_RET_IS_ERROR(err)) {
    __errno_r(r) = CH_DECODE_ERROR(err);
    return 0;
  }

  if (!S_ISCHR(s.st_mode)) {
    __errno_r(r) = ENOTTY;
    return 0;
  }

  return 1;
}

__attribute__((used))
caddr_t _sbrk_r(struct _reent *r, int incr) {
  uint8_t *prevp;

  prevp = __sbrk_next;
  if ((prevp + incr > __sb_parameters.heap_end) ||
      (prevp + incr < &__heap_base__)) {
    __errno_r(r) = ENOMEM;
    return (caddr_t)-1;
  }
  (void)r;

  __sbrk_next += incr;
  return (caddr_t)prevp;
}

__attribute__((used))
int _kill(int pid, int sig) {

  (void) pid;
  (void) sig;

  errno = EINVAL;
  return -1;
}

__attribute__((used))
int _getpid(void) {

  return 1;
  abort();
}

/* Additional functions not part of newlib.*/

int _getdents_r(struct _reent *r, int fd, void *dp, int count) {
  ssize_t n;

  n = sbGetdents(fd, dp, count);
  if (CH_RET_IS_ERROR(n)) {
    __errno_r(r) = CH_DECODE_ERROR(n);
    return -1;
  }

  return n;
}

int _chdir_r(struct _reent *r, const char *path) {
  int err;

  err = sbChdir(path);
  if (CH_RET_IS_ERROR(err)) {
    __errno_r(r) = CH_DECODE_ERROR(err);
    return -1;
  }

  return 0;
}

char *_getcwd_r(struct _reent *r, char *buf, size_t size) {
  int err;

  err = sbGetcwd(buf, size);
  if (CH_RET_IS_ERROR(err)) {
    __errno_r(r) = CH_DECODE_ERROR(err);
    return NULL;
  }

  return buf;
}

int _unlink_r(struct _reent *r, const char *path) {
  int err;

  err = sbUnlink(path);
  if (CH_RET_IS_ERROR(err)) {
    __errno_r(r) = CH_DECODE_ERROR(err);
    return -1;
  }

  return 0;
}

int _rename_r(struct _reent *r,
              const char *oldpath,
              const char *newpath) {
  int err;

  err = sbRename(oldpath, newpath);
  if (CH_RET_IS_ERROR(err)) {
    __errno_r(r) = CH_DECODE_ERROR(err);
    return -1;
  }

  return 0;
}

int _mkdir_r(struct _reent *r, const char *path, mode_t mode) {
  int err;

  err = sbMkdir(path, mode);
  if (CH_RET_IS_ERROR(err)) {
    __errno_r(r) = CH_DECODE_ERROR(err);
    return -1;
  }

  return 0;
}

int _rmdir_r(struct _reent *r, const char *path) {
  int err;

  err = sbRmdir(path);
  if (CH_RET_IS_ERROR(err)) {
    __errno_r(r) = CH_DECODE_ERROR(err);
    return -1;
  }

  return 0;
}

#ifdef __cplusplus
extern "C" {
  void __cxa_pure_virtual(void) {
    osalSysHalt("pure virtual");
  }
}
#endif

/*** EOF ***/
