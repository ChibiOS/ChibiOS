/*
    ChibiOS - Copyright (C) 2006..2024 Giovanni Di Sirio

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

#include <string.h>

#include "ch.h"
#include "hal.h"
#include "xshell.h"
#include "xshell_cmd.h"
#include "chprintf.h"

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

/*===========================================================================*/
/* Module local variables.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Module local functions.                                                   */
/*===========================================================================*/

#if ((XSHELL_CMD_EXIT_ENABLED == TRUE) && !defined(__CHIBIOS_NIL__)) ||     \
    defined(__DOXYGEN__)
static void cmd_exit(xshell_manager_t *smp, BaseSequentialStream *stream,
                     int argc, char *argv[]) {

  (void)argv;
  (void)smp;

  if (argc > 1) {
    xshellUsage(stream, "exit");
    return;
  }
  
  chThdTerminate(chThdGetSelfX());
}
#endif

#if (XSHELL_CMD_INFO_ENABLED == TRUE) || defined(__DOXYGEN__)
static void cmd_info(xshell_manager_t *smp, BaseSequentialStream *stream,
                     int argc, char *argv[]) {

  (void)smp;
  (void)argv;

  if (argc > 1) {
    xshellUsage(stream, "info");
    return;
  }

  chprintf(stream, "Kernel:       %s" XSHELL_NEWLINE_STR, CH_KERNEL_VERSION);
#ifdef PORT_COMPILER_NAME
  chprintf(stream, "Compiler:     %s" XSHELL_NEWLINE_STR, PORT_COMPILER_NAME);
#endif
  chprintf(stream, "Architecture: %s" XSHELL_NEWLINE_STR, PORT_ARCHITECTURE_NAME);
#ifdef PORT_CORE_VARIANT_NAME
  chprintf(stream, "Core Variant: %s" XSHELL_NEWLINE_STR, PORT_CORE_VARIANT_NAME);
#endif
#ifdef PORT_INFO
  chprintf(stream, "Port Info:    %s" XSHELL_NEWLINE_STR, PORT_INFO);
#endif
#ifdef PLATFORM_NAME
  chprintf(stream, "Platform:     %s" XSHELL_NEWLINE_STR, PLATFORM_NAME);
#endif
#ifdef BOARD_NAME
  chprintf(stream, "Board:        %s" XSHELL_NEWLINE_STR, BOARD_NAME);
#endif
#ifdef __DATE__
#ifdef __TIME__
  chprintf(stream, "Build time:   %s%s%s" XSHELL_NEWLINE_STR, __DATE__, " - ", __TIME__);
#endif
#endif
}
#endif

#if (XSHELL_CMD_ECHO_ENABLED == TRUE) || defined(__DOXYGEN__)
static void cmd_echo(xshell_manager_t *smp, BaseSequentialStream *stream,
                     int argc, char *argv[]) {

  (void)smp;
  (void)argv;

  if (argc != 2) {
    xshellUsage(stream, "echo \"message\"");
    return;
  }

  chprintf(stream, "%s" XSHELL_NEWLINE_STR, argv[1]);
}
#endif

#if (XSHELL_CMD_SYSTIME_ENABLED == TRUE) || defined(__DOXYGEN__)
static void cmd_systime(xshell_manager_t *smp, BaseSequentialStream *stream,
                        int argc, char *argv[]) {

  (void)smp;
  (void)argv;

  if (argc > 1) {
    xshellUsage(stream, "systime");
    return;
  }

  chprintf(stream, "%lu" XSHELL_NEWLINE_STR, (unsigned long)chVTGetSystemTimeX());
}
#endif

#if (XSHELL_CMD_MEM_ENABLED == TRUE) || defined(__DOXYGEN__)
static void cmd_mem(xshell_manager_t *smp, BaseSequentialStream *stream,
                    int argc, char *argv[]) {
  size_t n, total, largest;
  memory_area_t area;

  (void)smp;
  (void)argv;

  if (argc > 1) {
    xshellUsage(stream, "mem");
    return;
  }
  n = chHeapStatus(NULL, &total, &largest);
  chCoreGetStatusX(&area);
  chprintf(stream, "core free memory : %u bytes" XSHELL_NEWLINE_STR, area.size);
  chprintf(stream, "heap fragments   : %u" XSHELL_NEWLINE_STR, n);
  chprintf(stream, "heap free total  : %u bytes" XSHELL_NEWLINE_STR, total);
  chprintf(stream, "heap free largest: %u bytes" XSHELL_NEWLINE_STR, largest);
}
#endif

#if (XSHELL_CMD_THREADS_ENABLED == TRUE) || defined(__DOXYGEN__)
static void cmd_threads(xshell_manager_t *smp, BaseSequentialStream *stream,
                        int argc, char *argv[]) {
  static const char *states[] = {CH_STATE_NAMES};
  thread_t *tp;

  (void)smp;
  (void)argv;

  if (argc > 1) {
    xshellUsage(stream, "threads");
    return;
  }
  chprintf(stream, "core stklimit    stack     addr refs prio     state         name" XSHELL_NEWLINE_STR);
  tp = chRegFirstThread();
  do {
    core_id_t core_id;

#if !defined(__CHIBIOS_NIL__)
    core_id = tp->owner->core_id;
#else
    core_id = 0U;
#endif
#if (CH_DBG_ENABLE_STACK_CHECK == TRUE) || (CH_CFG_USE_DYNAMIC == TRUE)
    uint32_t stklimit = (uint32_t)tp->wabase;
#else
    uint32_t stklimit = 0U;
#endif
    chprintf(stream, "%4lu %08lx %08lx %08lx %4lu %4lu %9s %12s" XSHELL_NEWLINE_STR,
             core_id,
             stklimit,
             (uint32_t)tp->ctx.sp,
             (uint32_t)tp,
             (uint32_t)tp->refs - 1,
             (uint32_t)tp->hdr.pqueue.prio,
             states[tp->state],
             tp->name == NULL ? "" : tp->name);
    tp = chRegNextThread(tp);
  } while (tp != NULL);
}
#endif

#if (XSHELL_CMD_TEST_ENABLED == TRUE) || defined(__DOXYGEN__)
static void cmd_test(xshell_manager_t *smp, BaseSequentialStream *stream,
                     int argc, char *argv[]) {

  (void)smp;
  (void)argv;

  if (argc > 2) {
    xshellUsage(stream, "test [rt|oslib]");
    return;
  }
  else if (argc == 1) {
    test_execute(stream, &rt_test_suite);
    test_execute(stream, &oslib_test_suite);
  }
  else if (!strcmp(argv[1], "rt")) {
    test_execute(stream, &rt_test_suite);
  }
  else if (!strcmp(argv[1], "oslib")) {
    test_execute(stream, &oslib_test_suite);
  }
  else {
    xshellUsage(stream, "test rt|oslib");
    return;
  }
}
#endif

#if (XSHELL_PROMPT_STR_LENGTH > 0)  || defined(__DOXYGEN__)
static void cmd_prompt(xshell_manager_t *smp, BaseSequentialStream *stream,
                            int argc, char *argv[]) {
  if (argc != 2) {
    xshellUsage(stream, "prompt \"string\"");
    return;
  }
  if (strlen(argv[1]) > XSHELL_PROMPT_STR_LENGTH) {
    xshellUsage(stream, "prompt string too long");
    return;
  }
  strncpy(smp->prompt, argv[1], XSHELL_PROMPT_STR_LENGTH);
  smp->prompt[XSHELL_PROMPT_STR_LENGTH - 1] = '\0';
}
#endif

#if (XSHELL_CMD_FILES_ENABLED == TRUE) || defined(__DOXYGEN__)
static void scan_nodes(BaseSequentialStream *stream,
                       char *path,
                       vfs_direntry_info_t *dip) {
  msg_t res;
  vfs_directory_node_c *dirp;

  chprintf(stream, "%s" SHELL_NEWLINE_STR, path);
  res = vfsOpenDirectory(path, &dirp);
  if (res == CH_RET_SUCCESS) {
    size_t i = strlen(path);

    while (true) {
      char *fn = dip->name;
      res = vfsReadDirectoryNext(dirp, dip);
      if (res < (msg_t)1) {
        break;
      }

      fn = dip->name;
//      if (dip->attr & VFS_NODE_ATTR_ISDIR) {
      if (VFS_MODE_S_ISDIR(dip->mode)) {
        strcpy(path + i, fn);
        strcat(path + i, "/");
        scan_nodes(stream, path, dip);
        path[i] = '\0';
      }
      else {
        chprintf(stream, "%s%s" SHELL_NEWLINE_STR, path, fn);
      }
    }

    vfsClose((vfs_node_c *)dirp);
  }
}

static void cmd_tree(xshell_manager_t *smp, BaseSequentialStream *stream,
                     int argc, char *argv[]) {
  char *pathbuf = NULL;
  vfs_direntry_info_t *dip = NULL;

  (void)argv;

  if (argc > 1) {
    chprintf(stream, "Usage: tree" SHELL_NEWLINE_STR);
    return;
  }

  do {
    pathbuf = (char *)chHeapAlloc(NULL, 1024);
    dip = (vfs_direntry_info_t *)chHeapAlloc(NULL, 1024);
    if ((pathbuf == NULL) || (dip == NULL)) {
      chprintf(stream, "Out of memory" SHELL_NEWLINE_STR);
     break;
    }

    strcpy(pathbuf, "/");
    scan_nodes(stream, pathbuf, dip);
  }
  while (false);

  if (pathbuf != NULL) {
    chHeapFree((void *)pathbuf);
  }

  if (dip != NULL) {
    chHeapFree((void *)dip);
  }
}

static void cmd_cat(xshell_manager_t *smp, BaseSequentialStream *stream,
                    int argc, char *argv[]) {
  char *buf = NULL;

  if (argc != 2) {
    chprintf(stream, "Usage: cat <filename>" SHELL_NEWLINE_STR);
    return;
  }

  do {
    int fd, n;

    buf = (char *)chHeapAlloc(NULL, 2048);
    if (buf == NULL) {
      chprintf(stream, "Out of memory" SHELL_NEWLINE_STR);
     break;
    }

    fd = open(argv[1], O_RDONLY);
    if(fd == -1) {
      chprintf(stream, "Cannot open file" SHELL_NEWLINE_STR);
      break;
    }

    while ((n = read(fd, buf, sizeof (2048))) > 0) {
      streamWrite(stream, (const uint8_t *)buf, n);
    }
    chprintf(stream, SHELL_NEWLINE_STR);

    (void) close(fd);
  }
  while (false);

  if (buf != NULL) {
    chHeapFree((void *)buf);
  }
}

static void cmd_cd(xshell_manager_t *smp, BaseSequentialStream *stream,
                   int argc, char *argv[]) {

  if (argc != 2) {
    chprintf(stream, "Usage: cd <dirpath>" SHELL_NEWLINE_STR);
    return;
  }

  do {
    msg_t ret;

    ret = vfsChangeCurrentDirectory(argv[1]);
    if (CH_RET_IS_ERROR(ret)) {
      chprintf(stream, "failed (%d)" SHELL_NEWLINE_STR, ret);
    }
  }
  while (false);
}

static void cmd_ls(xshell_manager_t *smp, BaseSequentialStream *stream,
                   int argc, char *argv[]) {
  vfs_direntry_info_t *dip = NULL;

  if (argc > 2) {
    chprintf(stream, "Usage: ls [<dirpath>]" SHELL_NEWLINE_STR);
    return;
  }

  do {
    msg_t ret;
    vfs_directory_node_c *dirp;

    dip = (vfs_direntry_info_t *)chHeapAlloc(NULL, sizeof (vfs_direntry_info_t));
    if (dip == NULL) {
      chprintf(stream, "Out of memory" SHELL_NEWLINE_STR);
     break;
    }

    /* Opening the (un)specified directory.*/
    ret = vfsOpenDirectory(argc == 1 ? argv[1] : ".", &dirp);
    if (!CH_RET_IS_ERROR(ret)) {

      while (vfsReadDirectoryNext(dirp, dip) > (msg_t)0) {
        chprintf(stream, "%s" SHELL_NEWLINE_STR, dip->name);
      }

      vfsClose((vfs_node_c *)dirp);
    }
    else {
      chprintf(stream, "Failed (%d)" SHELL_NEWLINE_STR, ret);
    }

  } while (false);

  if (dip != NULL) {
    chHeapFree((void *)dip);
  }
}

static void cmd_mkdir(xshell_manager_t *smp, BaseSequentialStream *stream,
                      int argc, char *argv[]) {
  msg_t ret;

  if (argc != 2) {
    chprintf(stream, "Usage: mkdir <dirpath>" SHELL_NEWLINE_STR);
    return;
  }

  ret = vfsMkdir(argv[1], 0777U);
  if (CH_RET_IS_ERROR(ret)) {
    chprintf(stream, "Failed (%d)" SHELL_NEWLINE_STR, ret);
  }
}

static void cmd_mv(xshell_manager_t *smp, BaseSequentialStream *stream,
                   int argc, char *argv[]) {
  msg_t ret;

  if (argc != 3) {
    chprintf(stream, "Usage: mv <oldpath> <newpath>" SHELL_NEWLINE_STR);
    return;
  }

  ret = vfsRename(argv[1], argv[2]);
  if (CH_RET_IS_ERROR(ret)) {
    chprintf(stream, "Failed (%d)" SHELL_NEWLINE_STR, ret);
  }
}

static void cmd_pwd(xshell_manager_t *smp, BaseSequentialStream *stream,
                    int argc, char *argv[]) {
  char *buf = NULL;

  (void)argv;

  if (argc != 1) {
    chprintf(stream, "Usage: pwd" SHELL_NEWLINE_STR);
    return;
  }

  do {
    msg_t ret;

    buf = (char *)chHeapAlloc(NULL, VFS_CFG_PATHLEN_MAX + 1);
    if (buf == NULL) {
      chprintf(stream, "Out of memory" SHELL_NEWLINE_STR);
     break;
    }

    ret = vfsGetCurrentDirectory(buf, VFS_CFG_PATHLEN_MAX + 1);
    if (CH_RET_IS_ERROR(ret)) {
      chprintf(stream, "Failed (%d)" SHELL_NEWLINE_STR, ret);
    }
    else {
      chprintf(stream, "%s" SHELL_NEWLINE_STR, buf);
    }
  }
  while (false);

  if (buf != NULL) {
    chHeapFree((void *)buf);
  }
}

static void cmd_rm(xshell_manager_t *smp, BaseSequentialStream *stream,
                   int argc, char *argv[]) {
  msg_t ret;

  if (argc != 2) {
    chprintf(stream, "Usage: rm <filepath>" SHELL_NEWLINE_STR);
    return;
  }

  ret = vfsUnlink(argv[1]);
  if (CH_RET_IS_ERROR(ret)) {
    chprintf(stream, "Failed (%d)" SHELL_NEWLINE_STR, ret);
  }
}

static void cmd_rmdir(xshell_manager_t *smp, BaseSequentialStream *stream,
                      int argc, char *argv[]) {
  msg_t ret;

  if (argc != 2) {
    chprintf(stream, "Usage: rmdir <dirpath>" SHELL_NEWLINE_STR);
    return;
  }

  ret = vfsRmdir(argv[1]);
  if (CH_RET_IS_ERROR(ret)) {
    chprintf(stream, "Failed (%d)" SHELL_NEWLINE_STR, ret);
  }
}

static void cmd_stat(xshell_manager_t *smp, BaseSequentialStream *stream,
                     int argc, char *argv[]) {
  msg_t ret;
  vfs_stat_t statbuf;

  if (argc != 2) {
    chprintf(stream, "Usage: stat <path>" SHELL_NEWLINE_STR);
    return;
  }

  ret = vfsStat(argv[1], &statbuf);
  if (CH_RET_IS_ERROR(ret)) {
    chprintf(stream, "Failed (%d)" SHELL_NEWLINE_STR, ret);
  }

  chprintf(stream, "Mode 0x%04lx Size %d" SHELL_NEWLINE_STR, statbuf.mode, statbuf.size);
}
#endif

/*===========================================================================*/
/* Module exported functions.                                                */
/*===========================================================================*/

#if XSHELL_PROMPT_STR_LENGTH > 0
/**
 * @brief   Sets the prompt.
 *
 * @param[in,out] smp           pointer to the @p xshell_manager_t object
 * @param[in]     str           pointer to a prompt string
 *
 * @return  status
 * @retval  true on success else false
 *
 * @api
 */
bool xshellSetPrompt(xshell_manager_t *smp, const char *str) {

  if (strlen(str) > XSHELL_PROMPT_STR_LENGTH) {
    return false;
  }
  strncpy(smp->prompt, str, XSHELL_PROMPT_STR_LENGTH);
  smp->prompt[XSHELL_PROMPT_STR_LENGTH - 1] = '\0';

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
