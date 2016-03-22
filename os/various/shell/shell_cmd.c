/*
    ChibiOS - Copyright (C) 2006..2016 Giovanni Di Sirio

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

#if (SHELL_CMD_TEST_ENABLED == TRUE) || defined(__DOXYGEN__)
#include "test.h"
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

static void usage(BaseSequentialStream *chp, char *p) {

  chprintf(chp, "Usage: %s\r\n", p);
}

#if ((SHELL_CMD_EXIT_ENABLED == TRUE) && !defined(_CHIBIOS_NIL_)) ||        \
    defined(__DOXYGEN__)
static void cmd_exit(BaseSequentialStream *chp, int argc, char *argv[]) {

  (void)argv;
  if (argc > 0) {
    usage(chp, "exit");
    return;
  }

  shellExit(MSG_OK);
}
#endif

#if (SHELL_CMD_INFO_ENABLED == TRUE) || defined(__DOXYGEN__)
static void cmd_info(BaseSequentialStream *chp, int argc, char *argv[]) {

  (void)argv;
  if (argc > 0) {
    usage(chp, "info");
    return;
  }

  chprintf(chp, "Kernel:       %s\r\n", CH_KERNEL_VERSION);
#ifdef PORT_COMPILER_NAME
  chprintf(chp, "Compiler:     %s\r\n", PORT_COMPILER_NAME);
#endif
  chprintf(chp, "Architecture: %s\r\n", PORT_ARCHITECTURE_NAME);
#ifdef PORT_CORE_VARIANT_NAME
  chprintf(chp, "Core Variant: %s\r\n", PORT_CORE_VARIANT_NAME);
#endif
#ifdef PORT_INFO
  chprintf(chp, "Port Info:    %s\r\n", PORT_INFO);
#endif
#ifdef PLATFORM_NAME
  chprintf(chp, "Platform:     %s\r\n", PLATFORM_NAME);
#endif
#ifdef BOARD_NAME
  chprintf(chp, "Board:        %s\r\n", BOARD_NAME);
#endif
#ifdef __DATE__
#ifdef __TIME__
  chprintf(chp, "Build time:   %s%s%s\r\n", __DATE__, " - ", __TIME__);
#endif
#endif
}
#endif

#if (SHELL_CMD_ECHO_ENABLED == TRUE) || defined(__DOXYGEN__)
static void cmd_echo(BaseSequentialStream *chp, int argc, char *argv[]) {

  (void)argv;
  if (argc != 1) {
    usage(chp, "echo \"message\"");
    return;
  }
  chprintf(chp, "%s\r\n", argv[0]);
}
#endif

#if (SHELL_CMD_SYSTIME_ENABLED == TRUE) || defined(__DOXYGEN__)
static void cmd_systime(BaseSequentialStream *chp, int argc, char *argv[]) {

  (void)argv;
  if (argc > 0) {
    usage(chp, "systime");
    return;
  }
  chprintf(chp, "%lu\r\n", (unsigned long)chVTGetSystemTime());
}
#endif

#if (SHELL_CMD_MEM_ENABLED == TRUE) || defined(__DOXYGEN__)
static void cmd_mem(BaseSequentialStream *chp, int argc, char *argv[]) {
  size_t n, total, largest;

  (void)argv;
  if (argc > 0) {
    chprintf(chp, "Usage: mem\r\n");
    return;
  }
  n = chHeapStatus(NULL, &total, &largest);
  chprintf(chp, "core free memory : %u bytes\r\n", chCoreGetStatusX());
  chprintf(chp, "heap fragments   : %u\r\n", n);
  chprintf(chp, "heap free total  : %u bytes\r\n", total);
  chprintf(chp, "heap free largest: %u bytes\r\n", largest);
}
#endif

#if (SHELL_CMD_THREADS_ENABLED == TRUE) || defined(__DOXYGEN__)
static void cmd_threads(BaseSequentialStream *chp, int argc, char *argv[]) {
  static const char *states[] = {CH_STATE_NAMES};
  thread_t *tp;

  (void)argv;
  if (argc > 0) {
    chprintf(chp, "Usage: threads\r\n");
    return;
  }
  chprintf(chp, "stklimit    stack     addr refs prio     state         name\r\n");
  tp = chRegFirstThread();
  do {
    chprintf(chp, "%08lx %08lx %08lx %4lu %4lu %9s %12s\r\n",
             (uint32_t)tp->stklimit, (uint32_t)tp->ctx.sp, (uint32_t)tp,
             (uint32_t)tp->refs - 1, (uint32_t)tp->prio, states[tp->state],
             tp->name == NULL ? "" : tp->name);
    tp = chRegNextThread(tp);
  } while (tp != NULL);
}
#endif

#if (SHELL_CMD_TEST_ENABLED == TRUE) || defined(__DOXYGEN__)
static void cmd_test(BaseSequentialStream *chp, int argc, char *argv[]) {
  thread_t *tp;

  (void)argv;
  if (argc > 0) {
    chprintf(chp, "Usage: test\r\n");
    return;
  }
  tp = chThdCreateFromHeap(NULL, SHELL_CMD_TEST_WA_SIZE,
                           "test", chThdGetPriorityX(),
                           TestThread, chp);
  if (tp == NULL) {
    chprintf(chp, "out of memory\r\n");
    return;
  }
  chThdWait(tp);
}
#endif

/*===========================================================================*/
/* Module exported functions.                                                */
/*===========================================================================*/

/**
 * @brief   Array of the default commands.
 */
ShellCommand shell_local_commands[] = {
#if (SHELL_CMD_EXIT_ENABLED == TRUE) && !defined(_CHIBIOS_NIL_)
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
#if SHELL_CMD_TEST_ENABLED == TRUE
  {"test", cmd_test},
#endif
  {NULL, NULL}
};

/** @} */
