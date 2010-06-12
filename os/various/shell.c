/*
    ChibiOS/RT - Copyright (C) 2006,2007,2008,2009,2010 Giovanni Di Sirio.

    This file is part of ChibiOS/RT.

    ChibiOS/RT is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 3 of the License, or
    (at your option) any later version.

    ChibiOS/RT is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program. If not, see <http://www.gnu.org/licenses/>.

                                      ---

    A special exception to the GPL can be applied should you wish to distribute
    a combined work that includes ChibiOS/RT, without being obliged to provide
    the source code for any proprietary components. See the file exception.txt
    for full details of how and when the exception can be applied.
*/

/**
 * @file shell.c
 * @brief Simple CLI shell code.
 * @addtogroup SHELL
 * @{
 */

#include <stdio.h>
#include <string.h>

#include "ch.h"
#include "hal.h"
#include "shell.h"

#if SHELL_USE_IPRINTF
#define sprintf siprintf
#endif

/**
 * @brief Shell termination event source.
 */
EventSource shell_terminated;

#if defined(WIN32)
/*
 * MinGW does not seem to have this function...
 */
static char *strtok_r(char *str, const char *delim, char **saveptr) {
  char *token;
  if (str)
    *saveptr = str;
  token = *saveptr;

  if (!token)
    return NULL;

  token += strspn(token, delim);
  *saveptr = strpbrk(token, delim);
  if (*saveptr)
    *(*saveptr)++ = '\0';

  return *token ? token : NULL;
}
#endif

static void usage(BaseChannel *chp, char *p) {

  shellPrint(chp, "Usage: ");
  shellPrintLine(chp, p);
}

static void list_commands(BaseChannel *chp, const ShellCommand *scp) {

  while (scp->sc_name != NULL) {
    shellPrint(chp, scp->sc_name);
    shellPrint(chp, " ");
    scp++;
  }
}

static void cmd_info(BaseChannel *chp, int argc, char *argv[]) {

  (void)argv;
  if (argc > 0) {
    usage(chp, "info");
    return;
  }

  shellPrint(chp, "Kernel version: ");
  shellPrintLine(chp, CH_KERNEL_VERSION);
#ifdef __GNUC__
  shellPrint(chp, "GCC Version:    ");
  shellPrintLine(chp, __VERSION__);
#endif
  shellPrint(chp, "Architecture:   ");
  shellPrintLine(chp, CH_ARCHITECTURE_NAME);
#ifdef CH_CORE_VARIANT_NAME
  shellPrint(chp, "Core Variant:   ");
  shellPrintLine(chp, CH_CORE_VARIANT_NAME);
#endif
#ifdef PLATFORM_NAME
  shellPrint(chp, "Platform:       ");
  shellPrintLine(chp, PLATFORM_NAME);
#endif
#ifdef BOARD_NAME
  shellPrint(chp, "Board:          ");
  shellPrintLine(chp, BOARD_NAME);
#endif
}

static void cmd_systime(BaseChannel *chp, int argc, char *argv[]) {
  char buf[12];

  (void)argv;
  if (argc > 0) {
    usage(chp, "systime");
    return;
  }
  sprintf(buf, "%lu", (unsigned long)chTimeNow());
  shellPrintLine(chp, buf);
}

/**
 * @brief Array of the default commands.
 */
static ShellCommand local_commands[] = {
  {"info", cmd_info},
  {"systime", cmd_systime},
  {NULL, NULL}
};

static bool_t cmdexec(const ShellCommand *scp, BaseChannel *chp,
                      char *name, int argc, char *argv[]) {

  while (scp->sc_name != NULL) {
    if (strcasecmp(scp->sc_name, name) == 0) {
      scp->sc_function(chp, argc, argv);
      return FALSE;
    }
    scp++;
  }
  return TRUE;
}

/**
 * @brief Shell thread function.
 *
 * @param[in] p pointer to a @p BaseChannel object
 * @return Termination reason.
 * @retval RDY_OK terminated by command.
 * @retval RDY_RESET terminated by reset condition on the I/O channel.
 */
static msg_t shell_thread(void *p) {
  int n;
  msg_t msg = RDY_OK;
  BaseChannel *chp = ((ShellConfig *)p)->sc_channel;
  const ShellCommand *scp = ((ShellConfig *)p)->sc_commands;
  char *lp, *cmd, *tokp, line[SHELL_MAX_LINE_LENGTH];
  char *args[SHELL_MAX_ARGUMENTS + 1];

  shellPrintLine(chp, "");
  shellPrintLine(chp, "ChibiOS/RT Shell");
  while (TRUE) {
    shellPrint(chp, "ch> ");
    if (shellGetLine(chp, line, sizeof(line))) {
      shellPrint(chp, "\nlogout");
      break;
    }
    lp = strtok_r(line, " \009", &tokp);
    cmd = lp;
    n = 0;
    while ((lp = strtok_r(NULL, " \009", &tokp)) != NULL) {
      if (n >= SHELL_MAX_ARGUMENTS) {
        shellPrintLine(chp, "too many arguments");
        cmd = NULL;
        break;
      }
      args[n++] = lp;
    }
    args[n] = NULL;
    if (cmd != NULL) {
      if (strcasecmp(cmd, "exit") == 0) {
        if (n > 0)
          usage(chp, "exit");
        break;
      }
      else if (strcasecmp(cmd, "help") == 0) {
        if (n > 0)
          usage(chp, "help");
        shellPrint(chp, "Commands: help exit ");
        list_commands(chp, local_commands);
        if (scp != NULL)
          list_commands(chp, scp);
        shellPrintLine(chp, "");
      }
      else if (cmdexec(local_commands, chp, cmd, n, args) &&
          ((scp == NULL) || cmdexec(scp, chp, cmd, n, args))) {
        shellPrint(chp, cmd);
        shellPrintLine(chp, " ?");
      }
    }
  }
  chSysLock();
  chEvtBroadcastI(&shell_terminated);
  return msg;
}

/**
 * @brief Shell manager initialization.
 */
void shellInit(void) {

  chEvtInit(&shell_terminated);
}

/**
 * @brief Spawns a new shell.
 *
 * @param[in] scp pointer to a @p ShellConfig object
 * @param[in] size size of the shell working area to be allocated
 * @param[in] prio the priority level for the new shell
 *
 * @return A pointer to the shell thread.
 * @retval NULL thread creation failed because memory allocation.
 */
Thread *shellCreate(const ShellConfig *scp, size_t size, tprio_t prio) {

  return chThdCreateFromHeap(NULL, size, prio, shell_thread, (void *)scp);
}

/**
 * @brief Prints a string.
 *
 * @param[in] chp pointer to a @p BaseChannel object
 * @param[in] msg pointer to the string
 */
void shellPrint(BaseChannel *chp, const char *msg) {

  while (*msg)
    chIOPut(chp, *msg++);
}

/**
 * @brief Prints a string with a final newline.
 *
 * @param[in] chp pointer to a @p BaseChannel object
 * @param[in] msg pointer to the string
 */
void shellPrintLine(BaseChannel *chp, const char *msg) {

  shellPrint(chp, msg);
  shellPrint(chp, "\r\n");
}

/**
 * @brief Reads a whole line from the input channel.
 *
 * @param[in] chp pointer to a @p BaseChannel object
 * @param[in] line pointer to the line buffer
 * @param[in] size buffer maximum length
 *
 * @return The operation status.
 * @retval TRUE the channel was reset or CTRL-D pressed.
 * @retval FALSE operation successful.
 */
bool_t shellGetLine(BaseChannel *chp, char *line, unsigned size) {
  char *p = line;

  while (TRUE) {
    short c = (short)chIOGet(chp);
    if (c < 0)
      return TRUE;
    if (c == 4) {
      shellPrintLine(chp, "^D");
      return TRUE;
    }
    if (c == 8) {
      if (p != line) {
        chIOPut(chp, (uint8_t)c);
        chIOPut(chp, 0x20);
        chIOPut(chp, (uint8_t)c);
        p--;
      }
      continue;
    }
    if (c == '\r') {
      shellPrintLine(chp, "");
      *p = 0;
      return FALSE;
    }
    if (c < 0x20)
      continue;
    if (p < line + size - 1) {
      chIOPut(chp, (uint8_t)c);
      *p++ = (char)c;
    }
  }
}

/** @} */
