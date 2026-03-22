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
 * @file    httpd_vfs.c
 * @brief   HTTPD VFS bindings code.
 * @addtogroup LWIP_HTTPD_VFS
 * @{
 */

#include <string.h>
#include <fcntl.h>

#include "ch.h"
#include "hal.h"

#include "lwip/apps/fs.h"
#include "lwip/opt.h"
#include "lwip/mem.h"
#include "lwip/apps/httpd.h"

#include "httpd_vfs.h"
#include "vfs.h"

static const char httpd_vfs_prefix[] = HTTPD_VFS_PREFIX;

static msg_t build_httpd_path(char *path, size_t size, const char *name) {
  size_t prefix_len, name_len, needed;
  bool prefix_slash, name_slash;

  osalDbgCheck(path != NULL);
  osalDbgCheck(name != NULL);

  prefix_len = strlen(httpd_vfs_prefix);
  name_len = strlen(name);
  if ((prefix_len == 0U) || (httpd_vfs_prefix[0] != '/')) {
    return CH_RET_EINVAL;
  }

  prefix_slash = httpd_vfs_prefix[prefix_len - 1U] == '/';
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

  memcpy(path, httpd_vfs_prefix, prefix_len);
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

void httpd_vfs_init(void) {

  osalDbgCheck(httpd_vfs_prefix[0] == '/');
}

int fs_open_custom(struct fs_file *file, const char *name) {
  vfs_file_node_c *vfnp;
  vfs_stat_t statbuf;
  char path[VFS_CFG_PATHLEN_MAX + 1U];
  msg_t ret;

  ret = build_httpd_path(path, sizeof(path), name);
  if (CH_RET_IS_ERROR(ret)) {
    return 0;
  }

  memset(file, '\0', sizeof(*file));

  ret = vfsOpenFile(path, O_RDONLY, &vfnp);
  if (CH_RET_IS_ERROR(ret)) {
    return 0;
  }

  ret = vfsGetNodeStat((vfs_node_c *)vfnp, &statbuf);
  if (CH_RET_IS_ERROR(ret)) {
    vfsClose((vfs_node_c *)vfnp);
    return 0;
  }

  file->data = NULL;
  file->len = (int)statbuf.size;
  file->index = 0;
  file->pextension = (fs_file_extension *)vfnp;

  return 1;
}

void fs_close_custom(struct fs_file *file) {

  if ((file != NULL) && (file->pextension != NULL)) {
    vfsClose((vfs_node_c *)file->pextension);
    file->pextension = NULL;
  }
}

int fs_read_custom(struct fs_file *file, char *buffer, int count) {
  ssize_t ret;

  if ((file == NULL) || (file->pextension == NULL)) {
    return FS_READ_EOF;
  }

  ret = vfsReadFile((vfs_file_node_c *)file->pextension, (uint8_t *)buffer,
                    (size_t)count);
  if (CH_RET_IS_ERROR(ret)) {
    return 0;
  }
  if (ret == 0) {
    return FS_READ_EOF;
  }

  file->index += (int)ret;

  return (int)ret;
}

/** @} */
