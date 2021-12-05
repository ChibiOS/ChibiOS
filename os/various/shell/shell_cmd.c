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

/**
 * @file    shell_cmd.c
 * @brief   Simple CLI shell common commands code.
 *
 * @addtogroup SHELL
 * @{
 */

#include <string.h>

#include "ch.h"
#include "hal.h"
#include "shell.h"
#include "shell_cmd.h"
#include "chprintf.h"

#if (SHELL_CMD_FILES_ENABLED == TRUE) || defined(__DOXYGEN__)
#include "vfs.h"
#endif

#if (SHELL_CMD_TEST_ENABLED == TRUE) || defined(__DOXYGEN__)
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

#if ((SHELL_CMD_EXIT_ENABLED == TRUE) && !defined(__CHIBIOS_NIL__)) ||        \
    defined(__DOXYGEN__)
static void cmd_exit(BaseSequentialStream *chp, int argc, char *argv[]) {

  (void)argv;
  if (argc > 0) {
    shellUsage(chp, "exit");
    return;
  }

  shellExit(MSG_OK);
}
#endif

#if (SHELL_CMD_INFO_ENABLED == TRUE) || defined(__DOXYGEN__)
static void cmd_info(BaseSequentialStream *chp, int argc, char *argv[]) {

  (void)argv;
  if (argc > 0) {
    shellUsage(chp, "info");
    return;
  }

  chprintf(chp, "Kernel:       %s" SHELL_NEWLINE_STR, CH_KERNEL_VERSION);
#ifdef PORT_COMPILER_NAME
  chprintf(chp, "Compiler:     %s" SHELL_NEWLINE_STR, PORT_COMPILER_NAME);
#endif
  chprintf(chp, "Architecture: %s" SHELL_NEWLINE_STR, PORT_ARCHITECTURE_NAME);
#ifdef PORT_CORE_VARIANT_NAME
  chprintf(chp, "Core Variant: %s" SHELL_NEWLINE_STR, PORT_CORE_VARIANT_NAME);
#endif
#ifdef PORT_INFO
  chprintf(chp, "Port Info:    %s" SHELL_NEWLINE_STR, PORT_INFO);
#endif
#ifdef PLATFORM_NAME
  chprintf(chp, "Platform:     %s" SHELL_NEWLINE_STR, PLATFORM_NAME);
#endif
#ifdef BOARD_NAME
  chprintf(chp, "Board:        %s" SHELL_NEWLINE_STR, BOARD_NAME);
#endif
#ifdef __DATE__
#ifdef __TIME__
  chprintf(chp, "Build time:   %s%s%s" SHELL_NEWLINE_STR, __DATE__, " - ", __TIME__);
#endif
#endif
}
#endif

#if (SHELL_CMD_ECHO_ENABLED == TRUE) || defined(__DOXYGEN__)
static void cmd_echo(BaseSequentialStream *chp, int argc, char *argv[]) {

  (void)argv;
  if (argc != 1) {
    shellUsage(chp, "echo \"message\"");
    return;
  }
  chprintf(chp, "%s" SHELL_NEWLINE_STR, argv[0]);
}
#endif

#if (SHELL_CMD_SYSTIME_ENABLED == TRUE) || defined(__DOXYGEN__)
static void cmd_systime(BaseSequentialStream *chp, int argc, char *argv[]) {

  (void)argv;
  if (argc > 0) {
    shellUsage(chp, "systime");
    return;
  }
  chprintf(chp, "%lu" SHELL_NEWLINE_STR, (unsigned long)chVTGetSystemTimeX());
}
#endif

#if (SHELL_CMD_MEM_ENABLED == TRUE) || defined(__DOXYGEN__)
static void cmd_mem(BaseSequentialStream *chp, int argc, char *argv[]) {
  size_t n, total, largest;
  memory_area_t area;

  (void)argv;
  if (argc > 0) {
    shellUsage(chp, "mem");
    return;
  }
  n = chHeapStatus(NULL, &total, &largest);
  chCoreGetStatusX(&area);
  chprintf(chp, "core free memory : %u bytes" SHELL_NEWLINE_STR, area.size);
  chprintf(chp, "heap fragments   : %u" SHELL_NEWLINE_STR, n);
  chprintf(chp, "heap free total  : %u bytes" SHELL_NEWLINE_STR, total);
  chprintf(chp, "heap free largest: %u bytes" SHELL_NEWLINE_STR, largest);
}
#endif

#if (SHELL_CMD_THREADS_ENABLED == TRUE) || defined(__DOXYGEN__)
static void cmd_threads(BaseSequentialStream *chp, int argc, char *argv[]) {
  static const char *states[] = {CH_STATE_NAMES};
  thread_t *tp;

  (void)argv;
  if (argc > 0) {
    shellUsage(chp, "threads");
    return;
  }
  chprintf(chp, "core stklimit    stack     addr refs prio     state         name" SHELL_NEWLINE_STR);
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
    chprintf(chp, "%4lu %08lx %08lx %08lx %4lu %4lu %9s %12s" SHELL_NEWLINE_STR,
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

#if (SHELL_CMD_TEST_ENABLED == TRUE) || defined(__DOXYGEN__)
static THD_FUNCTION(test_rt, arg) {
  BaseSequentialStream *chp = (BaseSequentialStream *)arg;
  test_execute(chp, &rt_test_suite);
}

static THD_FUNCTION(test_oslib, arg) {
  BaseSequentialStream *chp = (BaseSequentialStream *)arg;
  test_execute(chp, &oslib_test_suite);
}

static void cmd_test(BaseSequentialStream *chp, int argc, char *argv[]) {
  thread_t *tp;
  tfunc_t tfp;

  (void)argv;
  if (argc != 1) {
    shellUsage(chp, "test rt|oslib");
    return;
  }
  if (!strcmp(argv[0], "rt")) {
    tfp = test_rt;
  }
  else if (!strcmp(argv[0], "oslib")) {
    tfp = test_oslib;
  }
  else {
    shellUsage(chp, "test rt|oslib");
    return;
  }
  tp = chThdCreateFromHeap(NULL, SHELL_CMD_TEST_WA_SIZE,
                           "test", chThdGetPriorityX(),
                           tfp, chp);
  if (tp == NULL) {
    chprintf(chp, "out of memory" SHELL_NEWLINE_STR);
    return;
  }
  chThdWait(tp);
}
#endif

#if (SHELL_CMD_FILES_ENABLED == TRUE) || defined(__DOXYGEN__)
static void scan_nodes(BaseSequentialStream *chp,
                       char *path,
                       vfs_node_info_t *nip) {
  msg_t res;
  vfs_directory_node_c *dirp;

  chprintf(chp, "%s\r\n", path);
  res = vfsOpenDirectory(path, &dirp);
  if (res == VFS_RET_SUCCESS) {
    size_t i = strlen(path);

    while (true) {
      char *fn = nip->name;
      res = vfsReadDirectoryNext(dirp, nip);
      if (res != VFS_RET_SUCCESS) {
        break;
      }

      fn = nip->name;
      if (nip->attr & VFS_NODE_ATTR_ISDIR) {
        strcpy(path + i, fn);
        strcat(path + i, "/");
        scan_nodes(chp, path, nip);
        path[i] = '\0';
      }
      else {
        chprintf(chp, "%s%s\r\n", path, fn);
      }
    }

    vfsCloseDirectory(dirp);
  }
}

static void cmd_tree(BaseSequentialStream *chp, int argc, char *argv[]) {
  char *pathbuf = NULL;
  vfs_node_info_t *nip = NULL;

  (void)argv;

  if (argc > 0) {
    chprintf(chp, "Usage: tree\r\n");
    return;
  }

  do {
    pathbuf = (char *)chHeapAlloc(NULL, 1024);
    nip = (vfs_node_info_t *)chHeapAlloc(NULL, 1024);
    if ((pathbuf == NULL) || (nip == NULL)) {
      chprintf(chp, "Out of memory\r\n");
     break;
    }

    strcpy(pathbuf, "/");
    scan_nodes(chp, pathbuf, nip);
  }
  while (false);

  if (pathbuf != NULL) {
    chHeapFree((void *)pathbuf);
  }

  if (nip != NULL) {
    chHeapFree((void *)nip);
  }
}
#endif

/*===========================================================================*/
/* Module exported functions.                                                */
/*===========================================================================*/

/**
 * @brief   Array of the default commands.
 */
const ShellCommand shell_local_commands[] = {
#if (SHELL_CMD_EXIT_ENABLED == TRUE) && !defined(__CHIBIOS_NIL__)
  {"exit", cmd_exit},
#endif
#if SHELL_CMD_INFO_ENABLED == TRUE
  {"info", cmd_info},
#endif
#if SHELL_CMD_ECHO_ENABLED == TRUE
  {"echo", cmd_echo},
#endif
#if SHELL_CMD_SYSTIME_ENABLED == TRUE
  {"systime", cmd_systime},
#endif
#if SHELL_CMD_MEM_ENABLED == TRUE
  {"mem", cmd_mem},
#endif
#if SHELL_CMD_THREADS_ENABLED == TRUE
  {"threads", cmd_threads},
#endif
#if SHELL_CMD_FILES_ENABLED == TRUE
  {"tree", cmd_tree},
#endif
#if SHELL_CMD_TEST_ENABLED == TRUE
  {"test", cmd_test},
#endif
  {NULL, NULL}
};

/** @} */
