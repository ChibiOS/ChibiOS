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

/**
 * @file    httpd_posix.c
 * @brief   HTTPD POSIX bindings code.
 * @addtogroup LWIP_HTTPD_POSIX
 * @{
 */

#include <string.h>
#include <stdint.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>

#include "ch.h"
#include "hal.h"

#include "lwip/apps/fs.h"
#include "lwip/opt.h"
#include "lwip/mem.h"
#include "lwip/apps/httpd.h"

#include "httpd_posix.h"

static const char httpd_posix_prefix[] = HTTPD_POSIX_PREFIX;
static GUARDEDMEMORYPOOL_DECL(httpd_fd_pool, sizeof(int), sizeof(int));
static int httpd_fd_array[HTTPD_POSIX_OPEN_FILES];

static msg_t build_httpd_path(char *path, size_t size, const char *name) {
  size_t prefix_len, name_len, needed;
  bool prefix_slash, name_slash;

  osalDbgCheck(path != NULL);
  osalDbgCheck(name != NULL);

  prefix_len = strlen(httpd_posix_prefix);
  name_len = strlen(name);
  if ((prefix_len == 0U) || (httpd_posix_prefix[0] != '/')) {
    return CH_RET_EINVAL;
  }

  prefix_slash = httpd_posix_prefix[prefix_len - 1U] == '/';
  name_slash = (name_len > 0U) && (name[0] == '/');

  if (prefix_len == 1U) {
    if (name_slash) {
      if (name_len + 1U > size) {
        return CH_RET_ENAMETOOLONG;
      }
      memcpy(path, name, name_len + 1U);
    }
    else {
      if (name_len + 2U > size) {
        return CH_RET_ENAMETOOLONG;
      }
      path[0] = '/';
      memcpy(&path[1], name, name_len + 1U);
    }

    return CH_RET_SUCCESS;
  }

  needed = prefix_len + name_len + 1U;
  if (prefix_slash && name_slash) {
    needed--;
  }
  else if (!prefix_slash && !name_slash) {
    needed++;
  }

  if (needed > size) {
    return CH_RET_ENAMETOOLONG;
  }

  memcpy(path, httpd_posix_prefix, prefix_len);
  if (prefix_slash && name_slash) {
    memcpy(&path[prefix_len], &name[1], name_len);
    path[prefix_len + name_len - 1U] = '\0';
  }
  else if (!prefix_slash && !name_slash) {
    path[prefix_len] = '/';
    memcpy(&path[prefix_len + 1U], name, name_len + 1U);
  }
  else {
    memcpy(&path[prefix_len], name, name_len + 1U);
  }

  return CH_RET_SUCCESS;
}

void httpd_posix_init(void) {

  osalDbgCheck(httpd_posix_prefix[0] == '/');
  chGuardedPoolLoadArray(&httpd_fd_pool,
                         httpd_fd_array,
                         sizeof httpd_fd_array / sizeof httpd_fd_array[0]);
}

int fs_open_custom(struct fs_file *file, const char *name) {
  struct stat statbuf;
  char path[HTTPD_POSIX_PATH_MAX];
  int fd, *fdp;
  msg_t ret;

  ret = build_httpd_path(path, sizeof(path), name);
  if (CH_RET_IS_ERROR(ret)) {
    return 0;
  }

  fdp = chGuardedPoolAllocTimeout(&httpd_fd_pool, TIME_IMMEDIATE);
  if (fdp == NULL) {
    return 0;
  }

  memset(file, '\0', sizeof(*file));

  fd = open(path, O_RDONLY);
  if (fd < 0) {
    chGuardedPoolFree(&httpd_fd_pool, (void *)fdp);
    return 0;
  }

  if (fstat(fd, &statbuf) != 0) {
    (void)close(fd);
    chGuardedPoolFree(&httpd_fd_pool, (void *)fdp);
    return 0;
  }

  *fdp = fd;
  file->data = NULL;
  file->len = (int)statbuf.st_size;
  file->index = 0;
  file->pextension = (fs_file_extension *)fdp;

  return 1;
}

void fs_close_custom(struct fs_file *file) {

  if ((file != NULL) && (file->pextension != NULL)) {
    int *fdp = (int *)file->pextension;

    (void)close(*fdp);
    chGuardedPoolFree(&httpd_fd_pool, (void *)fdp);
    file->pextension = NULL;
  }
}

int fs_read_custom(struct fs_file *file, char *buffer, int count) {
  ssize_t ret;

  if ((file == NULL) || (file->pextension == NULL)) {
    return FS_READ_EOF;
  }

  ret = read(*(int *)file->pextension, buffer, (size_t)count);
  if (ret < 0) {
    return 0;
  }
  if (ret == 0) {
    return FS_READ_EOF;
  }

  file->index += (int)ret;

  return (int)ret;
}

/** @} */
