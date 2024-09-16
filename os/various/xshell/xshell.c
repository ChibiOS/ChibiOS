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

static void *alloc_thread(size_t size, unsigned align);

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

/* Shared pool of thread structures, all shell managers share the same pool.*/
static MEMORYPOOL_DECL(threads_pool, sizeof (thread_t),
                       MEM_NATURAL_ALIGN, alloc_thread);

/*===========================================================================*/
/* Module local functions.                                                   */
/*===========================================================================*/

static void *alloc_thread(size_t size, unsigned align) {

  return chCoreAllocFromTopI(size, align, 0U);
}

static char *fetch_argument(char **pp) {
  char *p, *ap;

  /* Skipping white space.*/
  ap = *pp;
  ap += strspn(ap, " \t");

  if (*ap == '\0') {
    return  NULL;
  }

  if (*ap == '"') {
    /* If an argument starts with a double quote then its delimiter is another
       quote.*/
    ap++;
    p = strpbrk(ap, "\"");
  }
  else {
    /* The delimiter is white space.*/
    p = strpbrk(ap, " \t");
  }

  if (p != NULL) {
    /* Replacing the delimiter with a zero.*/
    *p++ = '\0';
  }
  else {
    /* Final one, pointing on the final zero.*/
    p = ap + strlen(ap);
  }
  *pp = p;

  return ap;
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
  BaseSequentialStream *stream = p;
  char *ap, *tokp, line[XSHELL_LINE_LENGTH];
  char *args[XSHELL_MAX_ARGUMENTS + 2];

  /* Shell banner, if defined.*/
  if (smp->config->banner != NULL) {
    chprintf(stream, smp->config->banner);
  }

  /* Shell command loop.*/
  while (!chThdShouldTerminateX()) {

    /* Shell prompt.*/
    chprintf(stream, smp->config->prompt);

    /* Getting input line.*/
    if (xshellGetLine(smp, stream, line, sizeof line)) {
      break;
    }

    /* Fetching arguments.*/
    tokp = line;
    n = 0;
    while ((ap = fetch_argument(&tokp)) != NULL) {
      if (n < XSHELL_MAX_ARGUMENTS) {
        args[n++] = ap;
      }
      else {
        n = 0;
        chprintf(stream, "too many arguments" XSHELL_NEWLINE_STR);
        break;
      }
    }
    args[n] = NULL;

    /* Special case for empty lines.*/
    if (n == 0) {
      continue;
    }

    /* Built-in commands, just "help" currently.*/
    if (strcmp(args[0], "help") == 0) {
      if (n > 1) {
        xshellUsage(stream, "help");
        continue;
      }

      /* Printing the commands list.*/
      chprintf(stream, "Commands: help ");
      list_commands(stream, xshell_local_commands);
      if (smp->config->commands != NULL) {
        list_commands(stream, smp->config->commands);
      }
      chprintf(stream, XSHELL_NEWLINE_STR);
    }
    else {
      /* Trying local commands.*/
      if (cmdexec(smp, xshell_local_commands, stream, args[0], n, args)) {

        /* Failed, trying user commands (if defined).*/
        if ((smp->config->commands == NULL) ||
            cmdexec(smp, smp->config->commands, stream, args[0], n, args)) {

          /* Failed, command not found.*/
          chprintf(stream, "%s?" XSHELL_NEWLINE_STR, args[0]);
        }
      }

      /* Command executed.*/
    }
  }

  xshellExit(smp, MSG_OK);
}

static void xshell_free(thread_t *tp) {
  xshell_manager_t *smp = (xshell_manager_t *)tp->object;

#if CH_CFG_USE_HEAP == TRUE
  if (smp->config->use_heap) {
    chHeapFree((void *)tp->wabase);
  }
  else {
#endif
    chPoolFree(smp->config->stack.pool, (void *)tp->wabase);
#if CH_CFG_USE_HEAP == TRUE
  }
#endif
  chPoolFree(&threads_pool, (void *)tp);
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
  tp = chPoolAlloc(&threads_pool);
  if (tp != NULL) {
    void *sbase;

    /* Getting a stack area from this manager.*/
#if CH_CFG_USE_HEAP == TRUE
    if (smp->config->use_heap) {
      /* Using heap allocator.*/
      sbase = chHeapAllocAligned(NULL, smp->config->stack.size, PORT_WORKING_AREA_ALIGN);
    }
    else {
#endif
      /* Using pool allocator.*/
      sbase = chPoolAlloc(smp->config->stack.pool);
#if CH_CFG_USE_HEAP == TRUE
    }
#endif
    if (sbase != NULL) {
      size_t size;
#if CH_CFG_USE_HEAP == TRUE
      if (smp->config->use_heap) {
        size = smp->config->stack.size;
      }
      else {
#endif
        size = smp->config->stack.pool->object_size;
#if CH_CFG_USE_HEAP == TRUE
      }
#endif
      void *send = (void *)((uint8_t *)sbase + size);

      thread_descriptor_t td = __THD_DECL_DATA(smp->config->thread_name,
                                               sbase, send, NORMALPRIO,
                                               xshell_thread, (void *)stp,
                                               NULL);
      tp = chThdSpawnSuspended(tp, &td);
      chThdSetCallbackX(tp, xshell_free, (void *)smp);
      tp = chThdStart(tp);

    }
    else {
      chPoolFree(&threads_pool, (void *)tp);
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
  char *p;

  (void)smp;

  p = line;
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
        p--;
      }
      continue;
    }
    if (c == '\r') {
      chprintf(stp, XSHELL_NEWLINE_STR);
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
