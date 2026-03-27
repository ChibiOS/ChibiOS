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
 * @file    xshell_cmd.c
 * @brief   Extended CLI shell common commands code.
 *
 * @addtogroup XSHELL
 * @{
 */

#include <inttypes.h>
#include <errno.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

#include "ch.h"
#include "hal.h"

#include "xshell.h"
#include "xshell_cmd.h"

#if (XSHELL_CMD_FILES_ENABLED == TRUE) || defined(__DOXYGEN__)
#include <fcntl.h>
#include "vfs.h"
#endif

#if (XSHELL_CMD_TEST_ENABLED == TRUE) || defined(__DOXYGEN__)
#include "rt_test_root.h"
#include "oslib_test_root.h"
#endif

/*===========================================================================*/
/* Module local definitions.                                                 */
/*===========================================================================*/

/*===========================================================================*/
/* Module exported variables.                                                */
/*===========================================================================*/

/*===========================================================================*/
/* Module local types.                                                       */
/*===========================================================================*/

#if (XSHELL_CMD_FILES_ENABLED == TRUE) || defined(__DOXYGEN__)
typedef intptr_t xshell_fd_t;
#endif

/*===========================================================================*/
/* Module local variables.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Module local functions.                                                   */
/*===========================================================================*/

#if (XSHELL_CMD_FILES_ENABLED == TRUE) || defined(__DOXYGEN__)
static const xshell_fd_t xshell_invalid_fd = (xshell_fd_t)-1;

#if XSHELL_CMD_FILES_USE_VFS == TRUE
static int __ret_to_errno(msg_t ret) {

  if (CH_RET_IS_ERROR(ret)) {
    return CH_DECODE_ERROR(ret);
  }

  return EIO;
}

static mode_t __vfs_mode_to_stat_mode(vfs_mode_t mode) {
  mode_t stat_mode = 0U;

  if (VFS_MODE_S_ISREG(mode)) {
    stat_mode |= S_IFREG;
  }
  if (VFS_MODE_S_ISDIR(mode)) {
    stat_mode |= S_IFDIR;
  }
  if (VFS_MODE_S_ISCHR(mode)) {
    stat_mode |= S_IFCHR;
  }
  if (VFS_MODE_S_ISFIFO(mode)) {
    stat_mode |= S_IFIFO;
  }

  return stat_mode;
}
#endif

static int __stat(const char *path, struct stat *sp) {

#if XSHELL_CMD_FILES_USE_VFS == TRUE
  vfs_stat_t statbuf;
  msg_t ret;

  ret = vfsStat(path, &statbuf);
  if (CH_RET_IS_ERROR(ret)) {
    errno = __ret_to_errno(ret);
    return -1;
  }

  memset(sp, 0, sizeof(*sp));
  sp->st_mode = __vfs_mode_to_stat_mode(statbuf.mode);
  sp->st_size = (off_t)statbuf.size;

  return 0;
#else
  return stat(path, sp);
#endif
}

static xshell_fd_t __open(const char *path, int flags) {

#if XSHELL_CMD_FILES_USE_VFS == TRUE
  vfs_file_node_c *vfp;
  msg_t ret;

  ret = vfsOpenFile(path, flags, &vfp);
  if (CH_RET_IS_ERROR(ret)) {
    errno = __ret_to_errno(ret);
    return xshell_invalid_fd;
  }

  return (xshell_fd_t)(intptr_t)vfp;
#else
  return (xshell_fd_t)open(path, flags, 0666);
#endif
}

static int __close(xshell_fd_t fd) {

#if XSHELL_CMD_FILES_USE_VFS == TRUE
  if (fd != xshell_invalid_fd) {
    vfsClose((vfs_node_c *)(intptr_t)fd);
  }

  return 0;
#else
  if (fd != xshell_invalid_fd) {
    return close((int)fd);
  }

  return 0;
#endif
}

static ssize_t __read(xshell_fd_t fd, uint8_t *buf, size_t n) {

#if XSHELL_CMD_FILES_USE_VFS == TRUE
  ssize_t ret;

  ret = vfsReadFile((vfs_file_node_c *)(intptr_t)fd, buf, n);
  if (CH_RET_IS_ERROR(ret)) {
    errno = __ret_to_errno((msg_t)ret);
    return -1;
  }

  return ret;
#else
  return read((int)fd, buf, n);
#endif
}

static ssize_t __write(xshell_fd_t fd, const uint8_t *buf, size_t n) {

#if XSHELL_CMD_FILES_USE_VFS == TRUE
  ssize_t ret;

  ret = vfsWriteFile((vfs_file_node_c *)(intptr_t)fd, buf, n);
  if (CH_RET_IS_ERROR(ret)) {
    errno = __ret_to_errno((msg_t)ret);
    return -1;
  }

  return ret;
#else
  return write((int)fd, buf, n);
#endif
}
#endif

#if (XSHELL_CMD_EXIT_ENABLED == TRUE) || defined(__DOXYGEN__)
static void cmd_exit(xshell_t *xshp, int argc, char *argv[], char *envp[]) {

  (void)argv;
  (void)envp;

  if (argc > 1) {
    xshellUsage(xshp, "");
    return;
  }
  
  chThdTerminate(chThdGetSelfX());
}
#endif

#if (XSHELL_CMD_INFO_ENABLED == TRUE) || defined(__DOXYGEN__)
static void cmd_info(xshell_t *xshp, int argc, char *argv[], char *envp[]) {

  (void)argv;
  (void)envp;

  if (argc > 1) {
    xshellUsage(xshp, "");
    return;
  }

  chprintf(xshp->stream, "Kernel:       %s" XSHELL_NEWLINE_STR, CH_KERNEL_VERSION);
#ifdef PORT_COMPILER_NAME
  chprintf(xshp->stream, "Compiler:     %s" XSHELL_NEWLINE_STR, PORT_COMPILER_NAME);
#endif
  chprintf(xshp->stream, "Architecture: %s" XSHELL_NEWLINE_STR, PORT_ARCHITECTURE_NAME);
#ifdef PORT_CORE_VARIANT_NAME
  chprintf(xshp->stream, "Core Variant: %s" XSHELL_NEWLINE_STR, PORT_CORE_VARIANT_NAME);
#endif
#ifdef PORT_INFO
  chprintf(xshp->stream, "Port Info:    %s" XSHELL_NEWLINE_STR, PORT_INFO);
#endif
#ifdef PLATFORM_NAME
  chprintf(xshp->stream, "Platform:     %s" XSHELL_NEWLINE_STR, PLATFORM_NAME);
#endif
#ifdef BOARD_NAME
  chprintf(xshp->stream, "Board:        %s" XSHELL_NEWLINE_STR, BOARD_NAME);
#endif
#ifdef __DATE__
#ifdef __TIME__
  chprintf(xshp->stream, "Build time:   %s%s%s" XSHELL_NEWLINE_STR, __DATE__, " - ", __TIME__);
#endif
#endif
}
#endif

#if (XSHELL_CMD_ECHO_ENABLED == TRUE) || defined(__DOXYGEN__)
static void cmd_echo(xshell_t *xshp, int argc, char *argv[], char *envp[]) {

  (void)envp;

  if (argc != 2) {
    xshellUsage(xshp, "\"message\"");
    return;
  }

  chprintf(xshp->stream, "%s" XSHELL_NEWLINE_STR, argv[1]);
}
#endif

#if (XSHELL_CMD_SYSTIME_ENABLED == TRUE) || defined(__DOXYGEN__)
static void cmd_systime(xshell_t *xshp, int argc, char *argv[], char *envp[]) {

  (void)argv;
  (void)envp;

  if (argc > 1) {
    xshellUsage(xshp, "");
    return;
  }

  chprintf(xshp->stream, "%lu" XSHELL_NEWLINE_STR, (unsigned long)chVTGetSystemTimeX());
}
#endif

#if (XSHELL_CMD_MEM_ENABLED == TRUE) || defined(__DOXYGEN__)
static void cmd_mem(xshell_t *xshp, int argc, char *argv[], char *envp[]) {
  size_t n, total, largest;
  memory_area_t area;

  (void)argv;
  (void)envp;

  if (argc > 1) {
    xshellUsage(xshp, "");
    return;
  }
  n = chHeapStatus(NULL, &total, &largest);
  chCoreGetStatusX(&area);
  chprintf(xshp->stream, "core free memory : %u bytes" XSHELL_NEWLINE_STR, area.size);
  chprintf(xshp->stream, "heap fragments   : %u" XSHELL_NEWLINE_STR, n);
  chprintf(xshp->stream, "heap free total  : %u bytes" XSHELL_NEWLINE_STR, total);
  chprintf(xshp->stream, "heap free largest: %u bytes" XSHELL_NEWLINE_STR, largest);
}
#endif

#if (XSHELL_CMD_THREADS_ENABLED == TRUE) || defined(__DOXYGEN__)
static void cmd_threads(xshell_t *xshp, int argc, char *argv[], char *envp[]) {
  static const char *states[] = {CH_STATE_NAMES};
  thread_t *tp;

  (void)argv;
  (void)envp;

  if (argc > 1) {
    xshellUsage(xshp, "");
    return;
  }
  chprintf(xshp->stream, "core stklimit    stack     addr refs prio     state         name" XSHELL_NEWLINE_STR);
  tp = chRegFirstThread();
  do {
    core_id_t core_id;

#if !defined(__CHIBIOS_NIL__)
    core_id = tp->owner->core_id;
#else
    core_id = 0U;
#endif
#if (CH_DBG_ENABLE_STACK_CHECK == TRUE) || (CH_CFG_USE_DYNAMIC == TRUE)
    uintptr_t stklimit = (uintptr_t)tp->wabase;
#else
    uintptr_t stklimit = (uintptr_t)0;
#endif
    chprintf(xshp->stream,
             "%4lu %08" PRIxPTR " %08" PRIxPTR " %08" PRIxPTR
             " %4lu %4lu %9s %12s" XSHELL_NEWLINE_STR,
             (unsigned long)core_id,
             stklimit,
             (uintptr_t)tp->ctx.sp,
             (uintptr_t)tp,
             (uint32_t)tp->refs - 1,
             (uint32_t)tp->hdr.pqueue.prio,
             states[tp->state],
             tp->name == NULL ? "" : tp->name);
    tp = chRegNextThread(tp);
  } while (tp != NULL);
}
#endif

#if (XSHELL_CMD_TEST_ENABLED == TRUE) || defined(__DOXYGEN__)
static void cmd_test(xshell_t *xshp, int argc, char *argv[], char *envp[]) {

  (void)argv;
  (void)envp;

  if (argc > 2) {
    xshellUsage(xshp, "[rt|oslib]");
    return;
  }
  else if (argc == 1) {
    test_execute(xshp->stream, &rt_test_suite);
    test_execute(xshp->stream, &oslib_test_suite);
  }
  else if (!strcmp(argv[1], "rt")) {
    test_execute(xshp->stream, &rt_test_suite);
  }
  else if (!strcmp(argv[1], "oslib")) {
    test_execute(xshp->stream, &oslib_test_suite);
  }
  else {
    xshellUsage(xshp, "rt|oslib");
    return;
  }
}
#endif

#if (XSHELL_PROMPT_STR_LENGTH > 0)  || defined(__DOXYGEN__)
static void cmd_prompt(xshell_t *xshp, int argc, char *argv[], char *envp[]) {

  (void)envp;

  if (argc != 2) {
    xshellUsage(xshp, "\"string\"");
    return;
  }

  if (strlen(argv[1]) > XSHELL_PROMPT_STR_LENGTH) {
    chprintf(xshp->stream, "string too long" XSHELL_NEWLINE_STR);
    return;
  }
  strncpy(xshp->prompt, argv[1], XSHELL_PROMPT_STR_LENGTH);
  xshp->prompt[XSHELL_PROMPT_STR_LENGTH] = '\0';
}
#endif

#if (XSHELL_CMD_FILES_ENABLED == TRUE) || defined(__DOXYGEN__)
static void scan_nodes(xshell_t *xshp, char *path, size_t path_capacity, vfs_direntry_info_t *dip) {
  msg_t res;
  vfs_directory_node_c *dirp;

  chprintf(xshp->stream, "%s" XSHELL_NEWLINE_STR, path);
  res = vfsOpenDirectory(path, &dirp);
  if (res == CH_RET_SUCCESS) {
    size_t i = strlen(path);

    while (true) {
      char *fn = dip->name;
      size_t fn_len;
      size_t needed;
      res = vfsReadDirectoryNext(dirp, dip);
      if (res < (msg_t)1) {
        break;
      }

      fn = dip->name;
      fn_len = strlen(fn);
      needed = i + fn_len + 1U;

      if (VFS_MODE_S_ISDIR(dip->mode)) {
        needed++;
      }

      if (needed > path_capacity) {
        chprintf(xshp->stream, "%s%s [path too long]" XSHELL_NEWLINE_STR, path, fn);
        continue;
      }

      if (VFS_MODE_S_ISDIR(dip->mode)) {
        size_t cursor = i;

        memcpy(path + cursor, fn, fn_len);
        cursor += fn_len;
        path[cursor++] = '/';
        path[cursor] = '\0';
        scan_nodes(xshp, path, path_capacity, dip);
        path[i] = '\0';
      }
      else {
        chprintf(xshp->stream, "%s%s" XSHELL_NEWLINE_STR, path, fn);
      }
    }

    vfsClose((vfs_node_c *)dirp);
  }
}

static void cmd_tree(xshell_t *xshp, int argc, char *argv[], char *envp[]) {

  char *pathbuf = NULL;
  vfs_direntry_info_t *dip = NULL;
  const size_t path_capacity = VFS_CFG_PATHLEN_MAX + 1U;

  (void)argv;
  (void)envp;

  if (argc > 1) {
    xshellUsage(xshp, "");
    return;
  }

  do {
    pathbuf = (char *)chHeapAlloc(NULL, path_capacity);
    dip = (vfs_direntry_info_t *)chHeapAlloc(NULL, sizeof (vfs_direntry_info_t));
    if ((pathbuf == NULL) || (dip == NULL)) {
      chprintf(xshp->stream, "Out of memory" XSHELL_NEWLINE_STR);
     break;
    }

    strcpy(pathbuf, "/");
    scan_nodes(xshp, pathbuf, path_capacity, dip);
  }
  while (false);

  if (pathbuf != NULL) {
    chHeapFree((void *)pathbuf);
  }

  if (dip != NULL) {
    chHeapFree((void *)dip);
  }
}

static void cmd_cat(xshell_t *xshp, int argc, char *argv[], char *envp[]) {
  char *buf = NULL;
  xshell_fd_t fd = xshell_invalid_fd;

  (void)envp;

  if (argc != 2) {
    xshellUsage(xshp, "<source>");
    return;
  }

  do {
    struct stat statbuf;
    ssize_t n;

    buf = (char *)chHeapAlloc(NULL, XSHELL_CMD_FILES_BUFFER_SIZE);
    if (buf == NULL) {
      chprintf(xshp->stream, "Out of memory" XSHELL_NEWLINE_STR);
     break;
    }

    if (__stat(argv[1], &statbuf) < 0) {
      chprintf(xshp->stream, "stat failed (%d)" XSHELL_NEWLINE_STR, errno);
      break;
    }

    if (!(S_ISREG(statbuf.st_mode) || S_ISCHR(statbuf.st_mode) || S_ISFIFO(statbuf.st_mode))) {
      chprintf(xshp->stream, "Not a valid source type" XSHELL_NEWLINE_STR);
      break;
    }

    fd = __open(argv[1], O_RDONLY);
    if (fd == xshell_invalid_fd) {
      chprintf(xshp->stream, "Cannot open source" XSHELL_NEWLINE_STR);
      break;
    }

    while ((n = __read(fd, (uint8_t *)buf, XSHELL_CMD_FILES_BUFFER_SIZE)) > 0) {
      streamWrite(xshp->stream, (const uint8_t *)buf, n);
#if CH_HAL_MAJOR >= 10
#error "TODO: Handle new streams/channels in XHAL here"
#else
      if (chnGetTimeout((BaseChannel*)xshp->stream, TIME_IMMEDIATE) != STM_TIMEOUT) {
        break;
      }
#endif
    }
    if (n < 0) {
      chprintf(xshp->stream, "Error reading source" XSHELL_NEWLINE_STR);
    }
    chprintf(xshp->stream, XSHELL_NEWLINE_STR);
  }
  while (false);

  (void)__close(fd);
  if (buf != NULL) {
    chHeapFree((void *)buf);
  }
}

static void cmd_cp(xshell_t *xshp, int argc, char *argv[], char *envp[]) {
  char *buf = NULL;
  xshell_fd_t src = xshell_invalid_fd;
  xshell_fd_t dst = xshell_invalid_fd;

  (void)envp;

  if (argc != 3) {
    xshellUsage(xshp, "<source file> <destination file>");
    return;
  }

  do {
    struct stat statbuf;
    ssize_t n;

    buf = (char *)chHeapAlloc(NULL, XSHELL_CMD_FILES_BUFFER_SIZE);
    if (buf == NULL) {
      chprintf(xshp->stream, "Out of memory" XSHELL_NEWLINE_STR);
     break;
    }

    if (__stat(argv[1], &statbuf) < 0) {
      chprintf(xshp->stream, "stat failed (%d)" XSHELL_NEWLINE_STR, errno);
      break;
    }

    if (!S_ISREG(statbuf.st_mode)) {
      chprintf(xshp->stream, "Not a file type" XSHELL_NEWLINE_STR);
      break;
    }

    src = __open(argv[1], O_RDONLY);
    if (src == xshell_invalid_fd) {
      chprintf(xshp->stream, "Cannot open source file" XSHELL_NEWLINE_STR);
      break;
    }

    dst = __open(argv[2], O_CREAT | O_WRONLY | O_TRUNC);
    if (dst == xshell_invalid_fd) {
      chprintf(xshp->stream, "Cannot open destination file" XSHELL_NEWLINE_STR);
      break;
    }

    while ((n = __read(src, (uint8_t *)buf, XSHELL_CMD_FILES_BUFFER_SIZE)) > 0) {
      bool write_error = false;
      size_t total_written = 0;

      while (total_written < (size_t)n) {
        ssize_t written_this_call;

        written_this_call = __write(dst, (const uint8_t *)buf + total_written,
                                    (size_t)n - total_written);
        if (written_this_call < 0) {
          chprintf(xshp->stream, "Error writing destination file" XSHELL_NEWLINE_STR);
          write_error = true;
          break;
        }
        total_written += (size_t)written_this_call;
      }

      if (write_error) {
        break;
      }

      /* Check for abort.*/
#if CH_HAL_MAJOR >= 10
#error "TODO: Handle new streams/channels in XHAL here"
#else
      if (chnGetTimeout((BaseChannel*)xshp->stream, TIME_IMMEDIATE) != STM_TIMEOUT) {
        break;
      }
#endif
    }
    if (n < 0) {
      chprintf(xshp->stream, "Error reading source file" XSHELL_NEWLINE_STR);
    }
    chprintf(xshp->stream, XSHELL_NEWLINE_STR);
  }
  while (false);

  (void)__close(dst);
  (void)__close(src);
  if (buf != NULL) {
    chHeapFree((void *)buf);
  }
}

static void cmd_cd(xshell_t *xshp, int argc, char *argv[], char *envp[]) {

  (void)envp;

  if (argc != 2) {
    xshellUsage(xshp, "<dirpath>");
    return;
  }

  do {
    msg_t ret;

    ret = vfsChangeCurrentDirectory(argv[1]);
    if (CH_RET_IS_ERROR(ret)) {
      chprintf(xshp->stream, "failed (%d)" XSHELL_NEWLINE_STR, CH_DECODE_ERROR(ret));
    }
  }
  while (false);
}

static void cmd_ls(xshell_t *xshp, int argc, char *argv[], char *envp[]) {
  vfs_direntry_info_t *dip = NULL;

  (void)envp;

  if (argc > 2) {
    xshellUsage(xshp, "[<dirpath>]");
    return;
  }

  do {
    msg_t ret;
    vfs_directory_node_c *dirp;

    dip = (vfs_direntry_info_t *)chHeapAlloc(NULL, sizeof (vfs_direntry_info_t));
    if (dip == NULL) {
      chprintf(xshp->stream, "Out of memory" XSHELL_NEWLINE_STR);
     break;
    }

    /* Opening the (un)specified directory.*/
    ret = vfsOpenDirectory(argc == 2 ? argv[1] : ".", &dirp);
    if (!CH_RET_IS_ERROR(ret)) {

      while (vfsReadDirectoryNext(dirp, dip) > (msg_t)0) {
        chprintf(xshp->stream, "%s" XSHELL_NEWLINE_STR, dip->name);
      }

      vfsClose((vfs_node_c *)dirp);
    }
    else {
      chprintf(xshp->stream, "Failed (%d)" XSHELL_NEWLINE_STR, CH_DECODE_ERROR(ret));
    }

  } while (false);

  if (dip != NULL) {
    chHeapFree((void *)dip);
  }
}

static void cmd_mkdir(xshell_t *xshp, int argc, char *argv[], char *envp[]) {
  msg_t ret;

  (void)envp;

  if (argc != 2) {
    xshellUsage(xshp, "<dirpath>");
    return;
  }

  ret = vfsMkdir(argv[1], 0777U);
  if (CH_RET_IS_ERROR(ret)) {
    chprintf(xshp->stream, "Failed (%d)" XSHELL_NEWLINE_STR, CH_DECODE_ERROR(ret));
  }
}

static void cmd_mv(xshell_t *xshp, int argc, char *argv[], char *envp[]) {
  msg_t ret;

  (void)envp;

  if (argc != 3) {
    xshellUsage(xshp, "<oldpath> <newpath>");
    return;
  }

  ret = vfsRename(argv[1], argv[2]);
  if (CH_RET_IS_ERROR(ret)) {
    chprintf(xshp->stream, "Failed (%d)" XSHELL_NEWLINE_STR, CH_DECODE_ERROR(ret));
  }
}

static void cmd_pwd(xshell_t *xshp, int argc, char *argv[], char *envp[]) {
  char *buf = NULL;

  (void)argv;
  (void)envp;

  if (argc != 1) {
    xshellUsage(xshp, "");
    return;
  }

  do {
    msg_t ret;

    buf = (char *)chHeapAlloc(NULL, VFS_CFG_PATHLEN_MAX + 1);
    if (buf == NULL) {
      chprintf(xshp->stream, "Out of memory" XSHELL_NEWLINE_STR);
     break;
    }

    ret = vfsGetCurrentDirectory(buf, VFS_CFG_PATHLEN_MAX + 1);
    if (CH_RET_IS_ERROR(ret)) {
      chprintf(xshp->stream, "Failed (%d)" XSHELL_NEWLINE_STR, CH_DECODE_ERROR(ret));
    }
    else {
      chprintf(xshp->stream, "%s" XSHELL_NEWLINE_STR, buf);
    }
  }
  while (false);

  if (buf != NULL) {
    chHeapFree((void *)buf);
  }
}

static void cmd_rm(xshell_t *xshp, int argc, char *argv[], char *envp[]) {
  msg_t ret;

  (void)envp;

  if (argc != 2) {
    xshellUsage(xshp, "<filepath>");
    return;
  }

  ret = vfsUnlink(argv[1]);
  if (CH_RET_IS_ERROR(ret)) {
    chprintf(xshp->stream, "Failed (%d)" XSHELL_NEWLINE_STR, CH_DECODE_ERROR(ret));
  }
}

static void cmd_rmdir(xshell_t *xshp, int argc, char *argv[], char *envp[]) {
  msg_t ret;

  (void)envp;

  if (argc != 2) {
    xshellUsage(xshp, "<dirpath>");
    return;
  }

  ret = vfsRmdir(argv[1]);
  if (CH_RET_IS_ERROR(ret)) {
    chprintf(xshp->stream, "Failed (%d)" XSHELL_NEWLINE_STR, CH_DECODE_ERROR(ret));
  }
}

static void cmd_stat(xshell_t *xshp, int argc, char *argv[], char *envp[]) {
  msg_t ret;
  vfs_stat_t statbuf;

  (void)envp;

  if (argc != 2) {
    xshellUsage(xshp, "<path>");
    return;
  }

  ret = vfsStat(argv[1], &statbuf);
  if (CH_RET_IS_ERROR(ret)) {
    chprintf(xshp->stream, "Failed (%d)" XSHELL_NEWLINE_STR, CH_DECODE_ERROR(ret));
    return;
  }

  chprintf(xshp->stream, "Mode 0x%04lx Size %d" XSHELL_NEWLINE_STR, statbuf.mode,
                                                              statbuf.size);
}
#endif

/*===========================================================================*/
/* Module exported functions.                                                */
/*===========================================================================*/

#if XSHELL_PROMPT_STR_LENGTH > 0
/**
 * @brief   Sets the prompt.
 *
 * @param[in,out] xshp          pointer to the @p xshell_t object
 * @param[in]     str           pointer to a prompt string
 *
 * @return  status
 * @retval  true on success else false
 *
 * @api
 */
bool xshellSetPrompt(xshell_t *xshp, const char *str) {

  chDbgCheck(xshp != NULL);

  if (strlen(str) > XSHELL_PROMPT_STR_LENGTH) {
    return false;
  }
  strncpy(xshp->prompt, str, XSHELL_PROMPT_STR_LENGTH);
  xshp->prompt[XSHELL_PROMPT_STR_LENGTH] = '\0';

  return true;
}
#endif

/**
 * @brief   Array of the default commands.
 */
const xshell_command_t xshell_local_commands[] = {
#if (XSHELL_CMD_EXIT_ENABLED == TRUE) && !defined(__CHIBIOS_NIL__)
  {"exit",      cmd_exit},
#endif
#if XSHELL_CMD_INFO_ENABLED == TRUE
  {"info",      cmd_info},
#endif
#if XSHELL_CMD_ECHO_ENABLED == TRUE
  {"echo",      cmd_echo},
#endif
#if XSHELL_CMD_SYSTIME_ENABLED == TRUE
  {"systime",   cmd_systime},
#endif
#if XSHELL_CMD_MEM_ENABLED == TRUE
  {"mem",       cmd_mem},
#endif
#if XSHELL_CMD_THREADS_ENABLED == TRUE
  {"threads",   cmd_threads},
#endif
#if XSHELL_CMD_FILES_ENABLED == TRUE
  {"cat",       cmd_cat},
  {"cd",        cmd_cd},
  {"ls",        cmd_ls},
  {"mkdir",     cmd_mkdir},
  {"mv",        cmd_mv},
  {"pwd",       cmd_pwd},
  {"rm",        cmd_rm},
  {"rmdir",     cmd_rmdir},
  {"stat",      cmd_stat},
  {"tree",      cmd_tree},
  {"cp",        cmd_cp},
#endif
#if XSHELL_CMD_TEST_ENABLED == TRUE
  {"test",      cmd_test},
#endif
#if XSHELL_PROMPT_STR_LENGTH > 0
  {"prompt",    cmd_prompt},
#endif
  {NULL, NULL}
};

/** @} */
