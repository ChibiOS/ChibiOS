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
 * @file    xshell.c
 * @brief   Extended CLI shell code.
 *
 * @addtogroup XSHELL
 * @{
 */

#include <string.h>
#include <stdlib.h>

#include "ch.h"
#include "hal.h"

#include "xshell.h"
#include "xshell_cmd.h"

static void *alloc_thread(size_t size, unsigned align);

/*===========================================================================*/
/* Module local definitions.                                                 */
/*===========================================================================*/

#define CTRL(c) (char)((c) - 0x40)

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
static MEMORYPOOL_DECL(shells_pool, sizeof (xshell_t),
                       MEM_NATURAL_ALIGN, alloc_thread);

/*===========================================================================*/
/* Module local functions.                                                   */
/*===========================================================================*/

static const char *xshell_get_prompt(xshell_t *xshp) {
  xshell_manager_t *smp = xshellGetManager(xshp);

#if XSHELL_PROMPT_STR_LENGTH > 0
  return smp->prompt;
#else
  if (smp->config->prompt == NULL) {
    return XSHELL_DEFAULT_PROMPT_STR;
  }

  return smp->config->prompt;
#endif
}

static void *alloc_thread(size_t size, unsigned align) {

  return chCoreAllocFromTopI(size, align, 0U);
}

static char *fetch_argument(char **pp) {
  char *p, *ap;

  /* Skipping white space.*/
  ap = *pp;
  ap += strspn(ap, " \t");

  if (*ap == '\0') {

    /* Argument is empty.*/
    return  NULL;
  }

  if (*ap == '"') {
    /* If an argument starts with a double quote then its delimiter is another
       quote.*/
    ap++;
    p = strpbrk(ap, "\"");
#if XSHELL_MULTI_COMMAND_LINE == TRUE
    if (p != NULL) {

      /* If the string is quoted terminate the string and point at the
         next delimiter.*/
      *p++ = '\0';
    }
#endif
  }
  else {
#if XSHELL_MULTI_COMMAND_LINE == TRUE
    /* Look for white space delimiter or separator.*/
    p = strpbrk(ap, " \t;");
#else
    /* Look for white space delimiter.*/
    p = strpbrk(ap, " \t");
#endif
  }

  if (p != NULL) {
#if XSHELL_MULTI_COMMAND_LINE != TRUE
    /* Replace the delimiter with a string terminator.*/
    *p++ = '\0';
#endif
  }
  else {
    /* Final one, pointing on the final zero.*/
    p = ap + strlen(ap);
  }

  /* Update the token pointer.*/
  *pp = p;

  return ap;
}

static void list_commands(shell_stream_i *chp, const xshell_command_t *scp) {

  while (scp->name != NULL) {
    chprintf(chp, "%s ", scp->name);
    scp++;
  }
}

static bool xshell_cmdexec(xshell_t *xshp, const xshell_command_t *scp,
                           char *name, int argc, char *argv[], char *envp[]) {

  while (scp->name != NULL) {
    if (strcmp(scp->name, name) == 0) {
      scp->fn(xshp, argc, argv, envp);
      return false;
    }
    scp++;
  }
  return true;
}

/**
 * @brief   Shell thread function.
 *
 * @param[in] p         pointer to a @p shell_stream_i object
 */
static THD_FUNCTION(xshell_thread, p) {
  int n;
  xshell_manager_t *smp = chThdGetSelfX()->object;
  xshell_t *xshp = p;
  char *ap, *tokp;

  /* Shell banner, if defined.*/
  if (smp->config->banner != NULL) {
    chprintf(xshp->stream, "%s", smp->config->banner);
  }

  /* Shell command loop.*/
  while (!chThdShouldTerminateX()) {

    /* Shell prompt.*/
    chprintf(xshp->stream, "%s", xshell_get_prompt(xshp));

    /* Getting input line.*/
    if (xshellGetLine(xshp, xshp->line, sizeof xshp->line)) {

      /* Logout.*/
      chprintf(xshp->stream, "%s", XSHELL_LOGOUT_STR);
      break;
    }

    /* Fetching arguments.*/
    tokp = xshp->line;
#if XSHELL_MULTI_COMMAND_LINE == TRUE
    char delim = '\0';
    do {
#endif
      n = 0;
      while ((ap = fetch_argument(&tokp)) != NULL) {
        if (n < XSHELL_MAX_ARGUMENTS) {
          xshp->args[n++] = ap;
#if XSHELL_MULTI_COMMAND_LINE == TRUE
          delim = *tokp;

          /* Terminate the argument string.*/
          *tokp++ = '\0';
          if (delim == ';' || delim == '\0') {

            /* End of command arguments.*/
            break;
          }
#endif
        }
        else {
          n = 0;
          chprintf(xshp->stream, "%s: too many arguments" XSHELL_NEWLINE_STR, xshp->args[0]);
          break;
        }
      }

      /* End of arguments.*/
      xshp->args[n] = NULL;

      /* Process command if parsed.*/
      if (n > 0) {

        bool exec = false;
        /* Built-in commands, just "help" currently.*/
        if (strcmp(xshp->args[0], "help") == 0) {
          if (n > 1) {
            xshellUsage(xshp, "");
            continue;
          }

          /* Printing the commands list.*/
          chprintf(xshp->stream, "Commands: help ");
          list_commands(xshp->stream, xshell_local_commands);
          if (smp->config->commands != NULL) {
            list_commands(xshp->stream, smp->config->commands);
          }
          chprintf(xshp->stream, XSHELL_NEWLINE_STR);
        }
        else {

          /* Trying local commands.*/
          if ((exec = xshell_cmdexec(xshp, xshell_local_commands, xshp->args[0],
                                     n, xshp->args, xshp->envp))) {

            /* Failed, trying user commands (if defined).*/
            if ((smp->config->commands == NULL) ||
                (exec = xshell_cmdexec(xshp, smp->config->commands, xshp->args[0],
                                       n, xshp->args, xshp->envp))) {

              /* Failed, command not found.*/
              chprintf(xshp->stream, "%s?" XSHELL_NEWLINE_STR, xshp->args[0]);
            }
          }
        }
        /* Shell execution hook.*/
#if defined(XSHELL_EXEC_HOOK)
        XSHELL_EXEC_HOOK(xshp, exec, n, xshp->args, xshp->envp);
#endif
      }
#if XSHELL_MULTI_COMMAND_LINE == TRUE
      /* If argument parsing OK then process next command.*/
    } while (n != 0 && delim == ';');
#endif
  }

  /* Shell exit hook.*/
#if defined(XSHELL_EXIT_HOOK)
  XSHELL_EXIT_HOOK(xshp);
#endif

  /* Atomically broadcasting the event source and terminating the thread,
     there is not a chSysUnlock() because the thread terminates upon return.*/
  chSysLock();
  chEvtBroadcastI(&smp->events);
  chThdExitS(MSG_OK);
}

static void xshell_free(thread_t *tp) {
  xshell_t *xshp = __CH_OWNEROF(tp, xshell_t, thread);
  xshell_manager_t *smp = xshellGetManager(xshp);

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
  chPoolFree(&shells_pool, (void *)xshp);
}

#if (XSHELL_HISTORY_DEPTH > 0) || defined(__DOXYGEN__)
static void xshell_save_history(xshell_t *xshp, char *line) {
  char *history_base = &xshp->history.history_buffer[0][0];
  char *history_end = history_base +
                      (XSHELL_HISTORY_DEPTH * XSHELL_LINE_LENGTH);

  strcpy(xshp->history.history_head, line);
  xshp->history.history_head += XSHELL_LINE_LENGTH;
  if (xshp->history.history_head >= history_end) {
    xshp->history.history_head = history_base;
  }
}

static size_t xshell_get_history_prev(xshell_t *xshp, char *line) {
  size_t len;
  char *p;

  if (xshell_is_line_empty(xshp->history.history_buffer[0])) {
    return (size_t)0;
  }

  p = xshp->history.history_current - XSHELL_LINE_LENGTH;
    if (p < xshp->history.history_buffer[0]) {
      p = xshp->history.history_buffer[XSHELL_HISTORY_DEPTH - 1];
  }
  if ((len = strlen(p)) > (size_t)0) {
    xshp->history.history_current = p;
  }
  strcpy(line, xshp->history.history_current);

  return len;
}

static size_t xshell_get_history_next(xshell_t *xshp, char *line) {
  size_t len;
  char *p;

  if (xshell_is_line_empty(xshp->history.history_buffer[0])) {
    return (size_t)0;
  }

  p = xshp->history.history_current + XSHELL_LINE_LENGTH;
    if (p > xshp->history.history_buffer[XSHELL_HISTORY_DEPTH - 1]) {
      p = xshp->history.history_buffer[0];
  }
  if ((len = strlen(p)) > (size_t)0) {
    xshp->history.history_current = p;
  }
  strcpy(line, xshp->history.history_current);

  return len;
}

static void xshell_reset_history(xshell_t *xshp) {

  xshp->history.history_head = xshp->history.history_buffer[0];
  xshp->history.history_current = xshp->history.history_head;
  memset(xshp->history.history_buffer, 0, sizeof xshp->history.history_buffer);
}

static bool xshell_is_line_empty(const char *str) {

  while (*str != '\0') {
    if (*str != ' ') {
      return false;
    }
    str++;
  }

  return true;
}
#endif /* (XSHELL_HISTORY_DEPTH > 0) || defined(__DOXYGEN__) */

#if (XSHELL_LINE_EDITING == TRUE) || (XSHELL_HISTORY_DEPTH > 0) ||            \
                                      defined(__DOXYGEN__)
static void xshell_reset_line(xshell_t *xshp) {
  const char *prompt = xshell_get_prompt(xshp);

  chprintf(xshp->stream, "\033[%dD%s\033[K",
           XSHELL_LINE_LENGTH + strlen(prompt) + 2, prompt);
}
#endif /* (XSHELL_LINE_EDITING == TRUE) || (XSHELL_HISTORY_DEPTH > 0) ||
                                    defined(__DOXYGEN__) */

#if (XSHELL_LINE_EDITING == TRUE)  || defined(__DOXYGEN__)
static void xshell_move_cursor(xshell_t *xshp, int pos) {

  if (pos < 0) {
    pos = abs(pos);
    chprintf(xshp->stream, "\033[%dD", pos);
  }
  else if (pos > 0) {
    chprintf(xshp->stream, "\033[%dC", pos);
  }
}
#endif /* (XSHELL_LINE_EDITING == TRUE)  || defined(__DOXYGEN__) */

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

  /* Set default prompt if enabled.*/
#if XSHELL_PROMPT_STR_LENGTH > 0
  /* Set the default prompt from config.*/
  if (smp->config->prompt == NULL) {
    strcpy(smp->prompt, XSHELL_DEFAULT_PROMPT_STR);
  }
  else {
    strncpy(smp->prompt, config->prompt, XSHELL_PROMPT_STR_LENGTH);
    smp->prompt[XSHELL_PROMPT_STR_LENGTH] = '\0';
  }
#endif

  /* Shell events.*/
  chEvtObjectInit(&smp->events);

  /* Shell manager initialization hook.*/
#if defined(XSHELL_MGR_INIT_HOOK)
  XSHELL_MGR_INIT_HOOK(smp);
#endif
}

/**
 * @brief   Spawns a new shell using the specified stream for I/O.
 *
 * @param[in,out] smp           pointer to the @p xshell_manager_t object
 * @param[in]     stream        pointer to a stream interface
 * @param[in]     prio          shell priority
 * @param[in]     envp          pointer to shell environment (or NULL)
 *
 * @api
 */
xshell_t *xshellSpawn(xshell_manager_t *smp,
                      shell_stream_i *stream,
                      tprio_t prio,
                      char *envp[]) {
  xshell_t *xshp;
  thread_t *tp;

  /* Getting a xshell structure from the pool.*/
  xshp = chPoolAlloc(&shells_pool);
  if (xshp != NULL) {
    void *sbase;

    /* Object initialization.*/
    memset(xshp, 0, sizeof (xshell_t));
    xshp->stream = stream;
    xshp->envp = envp;
#if XSHELL_HISTORY_DEPTH > 0
    xshell_reset_history(xshp);
#endif

    /* Getting a stack area from this manager.*/
#if CH_CFG_USE_HEAP == TRUE
    if (smp->config->use_heap) {
      /* Using heap allocator.*/
      sbase = chHeapAllocAligned(NULL, smp->config->stack.size,
                                                    PORT_WORKING_AREA_ALIGN);
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
                                               sbase, send, prio,
                                               xshell_thread, (void *)xshp,
                                               NULL);
      tp = chThdSpawnSuspended(&xshp->thread, &td);
      chThdSetCallbackX(tp, xshell_free, (void *)smp);

#if defined(XSHELL_INIT_HOOK)
    	/* Instance initialisation hook.*/
    	XSHELL_INIT_HOOK(xshp);
#endif

      tp = chThdStart(tp);

    }
    else {
      chPoolFree(&shells_pool, (void *)xshp);
      return NULL;
    }
  }

  return xshp;
}

/**
 * @brief   Shells garbage collection.
 * @details This function scans the registry to locate all terminated shells
 *          associated to the specified shell manager, for each terminated
 *          shell a reference is released causing, if the reference counter
 *          drops to zero, the memory to be released.
 * @note    This function assumes that the shell thread has at least one
 *          reference because @p xshellSpawn() does not release it so the
 *          first reference logically belongs to the shell manager itself.
 *
 * @param[in] smp               pointer to the @p xshell_manager_t object
 * @oaram[in] cb                shell release callback or @p NULL
 * @param[in] par               parameter to be passed to callback
 * @return                      The number of cleared terminated shells.
 *
 * @api
 */
ucnt_t xshellGarbageCollect(xshell_manager_t *smp, xshell_callback_t cb, void *par) {
  thread_t *tp;
  ucnt_t n = (ucnt_t)0;

  tp = chRegFirstThread();
  do {
    if (chThdGetObjectX(tp) == (void *)smp) {

      /* Shell threads need to have at least 2 references, one kept by
         the shell manager and the other added by the registry scan
         functions.*/
      chDbgAssert(tp->refs >= (trefs_t)2, "unreferenced shell");

      /* Only releasing terminated shells.*/
      if (chThdTerminatedX(tp)) {

        /* Found terminated shells.*/
        n++;

        /* The found shell is reported before freeing memory.*/
        if (cb != NULL) {
          cb((xshell_t *)__CH_OWNEROF(tp, xshell_t, thread), par);
        }

        /* Releasing the shell manager reference, this will cause the shell
           memory to be freed on the next call to chRegNextThread().*/
        chThdRelease(tp);
      }
    }
    tp = chRegNextThread(tp);
  } while (tp != NULL);

  return n;
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
 * @param[in,out] xshp          pointer to the @p xshell_t object
 * @param[in] line              pointer to the line buffer
 * @param[in] size              buffer maximum length
 * @return                      The operation status.
 * @retval true                 if the channel was reset or CTRL-D pressed.
 * @retval false                if operation successful.
 *
 * @api
 */
bool xshellGetLine(xshell_t *xshp, char *line, size_t size) {
  char *p;

#if (XSHELL_LINE_EDITING == TRUE) || (XSHELL_HISTORY_DEPTH > 0)
  bool escape = false;
  bool bracket = false;

#if XSHELL_LINE_EDITING == TRUE
  bool vt      = false;
#endif
#if XSHELL_HISTORY_DEPTH > 0
  xshp->history.history_current = xshp->history.history_head;
#endif
#endif

  memset(line, '\0', size);
  p = line;
  while (true) {
    char c;

    if (streamRead(xshp->stream, (uint8_t *)&c, 1) == 0)
      return true;
#if (XSHELL_LINE_EDITING == TRUE) && (XSHELL_HISTORY_DEPTH == 0)
    /* Escape sequences decoding.*/
    if (c == 27) {
      escape = true;
      continue;
    }
    if (escape) {
      if (c == '[') {
        bracket = true;
        continue;
      }
      else {
        escape = false;
      }
      if (bracket) {
        escape = false;
        bracket = false;
        if (c == 'D') {
          /* Cursor back.*/
          if (p != line) {
            xshell_move_cursor(xshp, (int)-1);
            p--;
          }
          continue;
        }
        if (c == 'C') {
          /* Cursor forward. Check if already at end of input.*/
          if (*p != '\0') {

            /* Not already at current end of input.*/
            xshell_move_cursor(xshp, (int)1);
            p++;
          }
          continue;
        }
        if (c == '3') {
          /* VT sequence. Followed by ~ for delete.*/
          vt = true;
          continue;
        }
      }
    }
    if (c == '~' && vt == true) {
      /* Do delete at cursor. TODO: Add support INSERT/OVERWRITE mode.*/
      vt = false;
      memmove(p, p + 1, strlen(p + 1) + 1);
      xshell_reset_line(xshp);
      chprintf(xshp->stream, "%s", line);
      xshell_move_cursor(xshp, (int)(-strlen(p)));
      continue;
    }
    vt = false;
    if ((c == CTRL('H')) || (c == 127)) {
      if (p != line) {
        p--;
        memmove(p, p + 1, strlen(p) + 1);
        xshell_reset_line(xshp);
        chprintf(xshp->stream, "%s", line);
        xshell_move_cursor(xshp, (int)(-strlen(p)));
      }
      continue;
    }
#endif /* (XSHELL_LINE_EDITING == TRUE) && (XSHELL_HISTORY_DEPTH == 0) */
#if (XSHELL_LINE_EDITING == TRUE) && (XSHELL_HISTORY_DEPTH > 0)
    /* Escape sequences decoding.*/
    if (c == 27) {
      escape = true;
      continue;
    }
    if (escape) {
      if (c == '[') {
        bracket = true;
        continue;
      }
      else {
        escape = false;
      }
      if (bracket) {
        escape = false;
        bracket = false;
        if (c == 'A') {
          /* Cursor up.*/
          size_t len = xshell_get_history_prev(xshp, line);

          if (len > (size_t)0) {
            xshell_reset_line(xshp);
            chprintf(xshp->stream, "%s", line);
            p = line + len;
          }
          continue;
        }
        if (c == 'B') {
          /* Cursor down.*/
          size_t len = xshell_get_history_next(xshp, line);

          if (len > (size_t)0) {
            xshell_reset_line(xshp);
            chprintf(xshp->stream, "%s", line);
            p = line + len;
          }
          continue;
        }
        if (c == 'C') {
          /* Cursor forward. Check if already at end of input.*/
          if (*p != '\0') {

            /* Not already at current end of input.*/
            xshell_move_cursor(xshp, (int)1);
            p++;
          }
          continue;
        }
        if (c == 'D') {
          /* Cursor back.*/
          if (p != line) {
            xshell_move_cursor(xshp, (int)-1);
            p--;
          }
          continue;
        }
        if (c == '3') {
          /* VT sequence. Followed by ~ for delete.*/
          vt = true;
          continue;
        }
      }
    }
    if (c == '~' && vt == true) {
      /* Do delete at cursor. TODO: Add support INSERT/OVERWRITE mode.*/
      vt = false;
      memmove(p, p + 1, strlen(p + 1) + 1);
      xshell_reset_line(xshp);
      chprintf(xshp->stream, "%s", line);
      xshell_move_cursor(xshp, (int)(-strlen(p)));
      continue;
    }
    vt = false;
    if (c == CTRL('U')) {
      xshp->history.history_current = xshp->history.history_head;
      p = line;
      xshell_reset_line(xshp);
      continue;
    }
    if ((c == CTRL('H')) || (c == 127)) {
      if (p != line) {
        p--;
        memmove(p, p + 1, strlen(p) + 1);
        xshell_reset_line(xshp);
        chprintf(xshp->stream, "%s", line);
        xshell_move_cursor(xshp, (int)(-strlen(p)));
      }
      continue;
    }
#endif /* (XSHELL_LINE_EDITING == TRUE) && (XSHELL_HISTORY_DEPTH > 0) */
#if (XSHELL_HISTORY_DEPTH > 0) && (XSHELL_LINE_EDITING == FALSE)
    /* Escape sequences decoding.*/
    if (c == 27) {
      escape = true;
      continue;
    }
    if (escape) {
      if (c == '[') {
        bracket = true;
        continue;
      }
      else {
        escape = false;
      }
      if (bracket) {
        escape = false;
        bracket = false;
        if (c == 'A') {
          /* Cursor up.*/
          size_t len = xshell_get_history_prev(xshp, line);

          if (len > (size_t)0) {
            xshell_reset_line(xshp);
            chprintf(xshp->stream, "%s", line);
            p = line + len;
          }
          continue;
        }
        if (c == 'B') {
          /* Cursor down.*/
          size_t len = xshell_get_history_next(xshp, line);

          if (len > (size_t)0) {
            xshell_reset_line(xshp);
            chprintf(xshp->stream, "%s", line);
            p = line + len;
          }
          continue;
        }
      }
    }
    if (c == CTRL('U')) {
      xshp->history.history_current = xshp->history.history_head;
      p = line;
      xshell_reset_line(xshp);
      continue;
    }
    if ((c == CTRL('H')) || (c == 127)) {
      if (p != line) {
        streamWrite(xshp->stream, (const uint8_t *)"\010 \010", 3);
        p--;
      }
      continue;
    }
#endif /* (XSHELL_HISTORY_DEPTH > 0) && (XSHELL_LINE_EDITING == FALSE) */
#if (XSHELL_HISTORY_DEPTH == 0) && (XSHELL_LINE_EDITING == FALSE)
    if ((c == CTRL('H')) || (c == 127)) {
      if (p != line) {
        streamWrite(xshp->stream, (const uint8_t *)"\010 \010", 3);
        p--;
      }
      continue;
    }
#endif /* (XSHELL_HISTORY_DEPTH == 0) && (XSHELL_LINE_EDITING == FALSE) */

    /* Check for session close.*/
    if (c == CTRL('D')) {
      return true;
    }
    /* Check for execute line.*/
    if (strchr(XSHELL_EXECUTE_CHARS, c) != NULL) {
#if XSHELL_LINE_EDITING == TRUE
      /* Redraw the input line.*/
      xshell_reset_line(xshp);
      chprintf(xshp->stream, "%s", line);
#endif
      chprintf(xshp->stream, XSHELL_NEWLINE_STR);
#if XSHELL_HISTORY_DEPTH > 0
      if (!xshell_is_line_empty(line)) {
        xshell_save_history(xshp, line);
      }
#endif
      return false;
    }
    if (c < 0x20)
      continue;
#if XSHELL_LINE_EDITING == TRUE
    if (p < line + size - 1) {

      /* Check if cursor at current EOL.*/
      if (*p != '\0') {

        /* This is an insert so move line data and insert new character.*/
        memmove(p + 1, p, strlen(p) + 1);
        *p = (char)c;

        /* Redraw line. Cursor will be at end of output.*/
        xshell_reset_line(xshp);
        chprintf(xshp->stream, "%s", line);

        /* Restore cursor position.*/
        xshell_move_cursor(xshp, -strlen(p) + 1);
      }
      else {

        /* At end of line so just output and store character.*/
        streamPut(xshp->stream, c);
        *p = (char)c;
      }
      p++;
    }
#else
    if (p < line + size - 1) {
      streamPut(xshp->stream, c);
      *p++ = (char)c;
    }
#endif
  }
}

#if XSHELL_HISTORY_DEPTH > 0
/**
 * @brief   Clears the history of a shell.
 *
 * @param[in] xshp    pointer to a @p xshell_t object
 *
 * @api
 */
void xshellClearHistory(xshell_t *xshp) {

  xshell_reset_history(xshp);
}
#endif

/**
 * @brief   Prints out formatted usage message
 *
 * @param[in] xshp              pointer to a @p xshell_t object
 * @param[in] message           pointer to formatted message string
 * @param[in] ...               optional message parameters
 *
 * @api
 */
void xshellUsage(xshell_t *xshp, const char *message, ...) {

  va_list ap;

  chprintf(xshp->stream, "Usage: %s " , xshp->args[0]);
  va_start(ap, message);
  (void) chvprintf(xshp->stream, message, ap);
  va_end(ap);
  chprintf(xshp->stream, XSHELL_NEWLINE_STR);

  return;
}

/** @} */
