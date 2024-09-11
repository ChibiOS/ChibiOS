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
 * @file    xshell.c
 * @brief   Extended CLI shell code.
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

static char *parse_arguments(char *str, char **saveptr) {
  char *p;

  if (str != NULL)
    *saveptr = str;

  p = *saveptr;
  if (!p) {
    return NULL;
  }

  /* Skipping white space.*/
  p += strspn(p, " \t");

  if (*p == '"') {
    /* If an argument starts with a double quote then its delimiter is another
       quote.*/
    p++;
    *saveptr = strpbrk(p, "\"");
  }
  else {
    /* The delimiter is white space.*/
    *saveptr = strpbrk(p, " \t");
  }

  /* Replacing the delimiter with a zero.*/
  if (*saveptr != NULL) {
    *(*saveptr)++ = '\0';
  }

  return *p != '\0' ? p : NULL;
}

static void list_commands(BaseSequentialStream *chp, const xshell_command_t *scp) {

  while (scp->name != NULL) {
    chprintf(chp, "%s ", scp->name);
    scp++;
  }
}

static bool cmdexec(xshell_manager_t *smp, const xshell_command_t *scp,
                    BaseSequentialStream *chp,
                    char *name, int argc, char *argv[]) {

  while (scp->name != NULL) {
    if (strcmp(scp->name, name) == 0) {
      scp->fn(smp, chp, argc, argv);
      return false;
    }
    scp++;
  }
  return true;
}

/**
 * @brief   Shell thread function.
 *
 * @param[in] p         pointer to a @p BaseSequentialStream object
 */
static THD_FUNCTION(xshell_thread, p) {
  int n;
  xshell_manager_t *smp = chThdGetSelfX()->object;
  BaseSequentialStream *chp = p;
  const xshell_command_t *scp = smp->config->commands;
  char *lp, *cmd, *tokp, line[XSHELL_LINE_LENGTH];
  char *args[XSHELL_MAX_ARGUMENTS + 2];

  chprintf(chp, smp->config->banner);
  while (!chThdShouldTerminateX()) {
    chprintf(chp, smp->config->prompt);
    if (shellGetLine(scfg, line, sizeof(line), shp)) {
//      chprintf(chp, smp->config->newline);
//      chprintf(chp, "logout");
      break;
    }
    lp = parse_arguments(line, &tokp);
    cmd = lp;
    n = 0;
    while ((lp = parse_arguments(NULL, &tokp)) != NULL) {
      if (n >= XSHELL_MAX_ARGUMENTS) {
        chprintf(chp, "too many arguments%s", smp->config->newline);
        cmd = NULL;
        break;
      }
      args[n++] = lp;
    }
    args[n] = NULL;
    if (cmd != NULL) {
      if (strcmp(cmd, "help") == 0) {
        if (n > 0) {
          xshellUsage(chp, "help");
          continue;
        }
        chprintf(chp, "Commands: help ");
        list_commands(chp, shell_local_commands);
        if (scp != NULL)
          list_commands(chp, scp);
        chprintf(chp, smp->config->newline);
      }
      else if (cmdexec(smp, shell_local_commands, chp, cmd, n, args) &&
               ((scp == NULL) || cmdexec(smp, scp, chp, cmd, n, args))) {
        chprintf(chp, "%s?%s", cmd, smp->config->newline);
      }
    }
  }

  xshellExit(smp, MSG_OK);
}

static void xshell_free(thread_t *tp) {
  xshell_manager_t *smp = (xshell_manager_t *)tp->object;

  chPoolFree(smp->config->stacks_pool, (void *)tp->wabase);
  chPoolFree(smp->config->threads_pool, (void *)tp);
}

/*===========================================================================*/
/* Module exported functions.                                                */
/*===========================================================================*/

/**
 * @brief   XShell Manager object initialization.
 *
 * @param[out] smp              pointer to the @p xshell_manager_t object to
 *                              be initialized
 * @param[in] config            pointer to configuration data
 *
 * @api
 */
void xshellObjectInit(xshell_manager_t *smp,
                      const xshell_manager_config_t *config) {

  /* Keeping association with configuration data, it needs to be persistent.*/
  smp->config = config;

  /* Shell events.*/
  chEvtObjectInit(&smp->events);

#if XSHELL_HISTORY_DEPTH > 0
#endif
}

/**
 * @brief   Spawns a new shell using the specified stream for I/O.
 *
 * @param[in,out] smp           pointer to the @p xshell_manager_t object
 * @param[in] stp               pointer to a stream interface
 *
 * @api
 */
thread_t *xshellSpawn(xshell_manager_t *smp, BaseSequentialStream *stp) {
  thread_t *tp;

  /* Getting a thread structure from the pool.*/
  tp = chPoolAlloc(smp->config->threads_pool);
  if (tp != NULL) {

    /* Getting a stack area from this manager.*/
    void *sbase = chPoolAlloc(smp->config->stacks_pool);
    if (sbase != NULL) {
      void *send = (void *)((uint8_t *)sbase +
                            smp->config->stacks_pool->object_size);

      thread_descriptor_t td = __THD_DECL_DATA(smp->config->thread_name,
                                               sbase, send, NORMALPRIO,
                                               xshell_thread, (void *)stp,
                                               NULL, xshell_free);
      tp = chThdSpawnSuspended(tp, &td);
      tp->object = (void *)smp;
      tp = chThdStart(tp);

    }
    else {
      chPoolFree(smp->config->threads_pool, (void *)tp);
      return NULL;
    }
  }

  return tp;
}

/**
 * @brief   Terminates the shell.
 * @note    Must be invoked from the command handlers.
 * @note    Does not return.
 *
 * @param[in,out] smp           pointer to the @p xshell_manager_t object
 * @param[in] msg               shell exit code
 *
 * @api
 */
void xshellExit(xshell_manager_t *smp, msg_t msg) {

  /* Atomically broadcasting the event source and terminating the thread,
     there is not a chSysUnlock() because the thread terminates upon return.*/
  chSysLock();
  chEvtBroadcastI(&smp->events);
  chThdExitS(msg);
}

/**
 * @brief   Reads a whole line from the input channel.
 * @note    Input chars are echoed on the same stream object with the
 *          following exceptions:
 *          - DEL and BS are echoed as BS-SPACE-BS.
 *          - CR is echoed as the configured new line sequence.
 *          - 0x4 is echoed as "^D".
 *          - Other values below 0x20 are not echoed.
 *          .
 *
 * @param[in,out] smp           pointer to the @p xshell_manager_t object
 * @param[in] stp               pointer to a stream interface
 * @param[in] line              pointer to the line buffer
 * @param[in] size              buffer maximum length
 * @return                      The operation status.
 * @retval true                 if the channel was reset or CTRL-D pressed.
 * @retval false                if operation successful.
 *
 * @api
 */
bool xshellGetLine(xshell_manager_t *smp, BaseSequentialStream *stp,
                   char *line, size_t size) {
  char *p = line;

  while (true) {
    char c;

    if (streamRead(stp, (uint8_t *)&c, 1) == 0)
      return true;
    if (c == 4) {
      chprintf(stp, "^D");
      return true;
    }
    if ((c == 8) || (c == 127)) {
      if (p != line) {
        streamWrite(stp, (const uint8_t *)"\010 \010", 3);
//        streamPut(stp, 0x08);
//        streamPut(stp, 0x20);
//        streamPut(stp, 0x08);
        p--;
      }
      continue;
    }
    if (c == '\r') {
      chprintf(stp, smp->config->newline);
      *p = 0;
      return false;
    }
    if (c < 0x20)
      continue;
    if (p < line + size - 1) {
      streamPut(stp, c);
      *p++ = (char)c;
    }
  }
}

/** @} */
