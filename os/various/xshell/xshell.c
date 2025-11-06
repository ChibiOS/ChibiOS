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

static bool cmdexec(xshell_manager_t *smp, const xshell_command_t *scp,
                    shell_stream_i *chp,
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
 * @param[in] p         pointer to a @p shell_stream_i object
 */
static THD_FUNCTION(xshell_thread, p) {
  int n;
  xshell_manager_t *smp = chThdGetSelfX()->object;
  shell_stream_i *stream = p;
  char *ap, *tokp, line[XSHELL_LINE_LENGTH];
  char *args[XSHELL_MAX_ARGUMENTS + 2];

  /* Shell banner, if defined.*/
  if (smp->config->banner != NULL) {
    chprintf(stream, "%s", smp->config->banner);
  }

  /* Shell command loop.*/
  while (!chThdShouldTerminateX()) {

    /* Shell prompt.*/
#if XSHELL_PROMPT_STR_LENGTH > 0
    chprintf(stream, "%s", smp->prompt);
#else
    chprintf(stream, "%s", smp->config->prompt);
#endif

    /* Getting input line.*/
    if (xshellGetLine(smp, stream, line, sizeof line)) {

      /* Logout.*/
      chprintf(stream, "%s", XSHELL_LOGOUT_STR);
      break;
    }

    /* Fetching arguments.*/
    tokp = line;
#if XSHELL_MULTI_COMMAND_LINE == TRUE
    char delim = '\0';
    do {
#endif
      n = 0;
      while ((ap = fetch_argument(&tokp)) != NULL) {
        if (n < XSHELL_MAX_ARGUMENTS) {
          args[n++] = ap;
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
          chprintf(stream, "%s: too many arguments" XSHELL_NEWLINE_STR, args[0]);
          break;
        }
      }

      /* End of arguments.*/
      args[n] = NULL;

      /* Process command if parsed.*/
      if (n > 0) {

        bool exec = false;
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
          if ((exec = cmdexec(smp, xshell_local_commands, stream, args[0],
                                                                  n, args))) {

            /* Failed, trying user commands (if defined).*/
            if ((smp->config->commands == NULL) ||
                (exec = cmdexec(smp, smp->config->commands, stream, args[0],
                                                                  n, args))) {

              /* Failed, command not found.*/
              chprintf(stream, "%s?" XSHELL_NEWLINE_STR, args[0]);
            }
          }
        }
        /* Shell execution hook.*/
#if defined(XSHELL_EXEC_HOOK)
        XSHELL_EXEC_HOOK(smp, stream, exec, n, args);
#endif
      }
#if XSHELL_MULTI_COMMAND_LINE == TRUE
      /* If argument parsing OK then process next command.*/
    } while (n != 0 && delim == ';');
#endif
  }

  /* Atomically broadcasting the event source and terminating the thread,
     there is not a chSysUnlock() because the thread terminates upon return.*/
  chSysLock();
  chEvtBroadcastI(&smp->events);
  chThdExitS(MSG_OK);
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

#if (XSHELL_HISTORY_DEPTH > 0) || defined(__DOXYGEN__)
static void xshell_save_history(xshell_manager_t *smp, char *line) {
  char *history_base = &smp->history_buffer[0][0];
  char *history_end = history_base +
                      (XSHELL_HISTORY_DEPTH * XSHELL_LINE_LENGTH);

  chMtxLock(&smp->history_mutex);

  strcpy(smp->history_head, line);
  smp->history_head += XSHELL_LINE_LENGTH;
  if (smp->history_head >= history_end) {
    smp->history_head = history_base;
  }

  chMtxUnlock(&smp->history_mutex);
}

static size_t xshell_get_history_prev(xshell_manager_t *smp, char *line) {
  size_t len;
  char *p;

  chMtxLock(&smp->history_mutex);

  p = smp->history_current - XSHELL_LINE_LENGTH;
    if (p < smp->history_buffer[0]) {
      p = smp->history_buffer[XSHELL_HISTORY_DEPTH - 1];
  }
  if ((len = strlen(p)) > (size_t)0) {
    smp->history_current = p;
  }
  strcpy(line, p);

  chMtxUnlock(&smp->history_mutex);

  return len;
}

static size_t xshell_get_history_next(xshell_manager_t *smp, char *line) {
  size_t len;
  char *p;

  chMtxLock(&smp->history_mutex);

  p = smp->history_current + XSHELL_LINE_LENGTH;
    if (p > smp->history_buffer[XSHELL_HISTORY_DEPTH - 1]) {
      p = smp->history_buffer[0];
  }
  if ((len = strlen(p)) > (size_t)0) {
    smp->history_current = p;
  }
  strcpy(line, p);

  chMtxUnlock(&smp->history_mutex);

  return len;
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
static void xshell_reset_line(xshell_manager_t *smp,
                              shell_stream_i *stream) {
#if XSHELL_PROMPT_STR_LENGTH > 0
  chprintf(stream, "\033[%dD%s\033[K",
           XSHELL_LINE_LENGTH + strlen(smp->prompt) + 2,
           smp->prompt);
#else
  chprintf(stream, "\033[%dD%s\033[K",
           XSHELL_LINE_LENGTH + strlen(smp->config->prompt) + 2,
           smp->config->prompt);
#endif
}
#endif /* (XSHELL_LINE_EDITING == TRUE) || (XSHELL_HISTORY_DEPTH > 0) ||
                                    defined(__DOXYGEN__) */

#if (XSHELL_LINE_EDITING == TRUE)  || defined(__DOXYGEN__)
static void xshell_move_cursor(shell_stream_i *stream, int pos) {

  if (pos < 0) {
    pos = abs(pos);
    chprintf(stream, "\033[%dD", pos);
  }
  else if (pos > 0) {
    chprintf(stream, "\033[%dC", pos);
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

#if XSHELL_PROMPT_STR_LENGTH > 0
  /* Set the default prompt from config.*/
  smp->prompt[XSHELL_PROMPT_STR_LENGTH - 1] = '\0';
  strncpy(smp->prompt, config->prompt, XSHELL_PROMPT_STR_LENGTH);
#endif
  
  /* Shell events.*/
  chEvtObjectInit(&smp->events);

#if XSHELL_HISTORY_DEPTH > 0
  chMtxObjectInit(&smp->history_mutex);
  smp->history_head = smp->history_buffer[0];
  memset(smp->history_buffer, 0, sizeof smp->history_buffer);
#endif

  /* Shell initialization hook.*/
#if defined(XSHELL_INIT_HOOK)
  XSHELL_INIT_HOOK(smp);
#endif
}

/**
 * @brief   Spawns a new shell using the specified stream for I/O.
 *
 * @param[in,out] smp           pointer to the @p xshell_manager_t object
 * @param[in] stp               pointer to a stream interface
 * @param[in] prio              shell priority
 *
 * @api
 */
thread_t *xshellSpawn(xshell_manager_t *smp,
                      shell_stream_i *stream,
                      tprio_t prio) {
  thread_t *tp;

  /* Getting a thread structure from the pool.*/
  tp = chPoolAlloc(&threads_pool);
  if (tp != NULL) {
    void *sbase;

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
                                               xshell_thread, (void *)stream,
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
 * @param[in] stream            pointer to a stream interface
 * @param[in] line              pointer to the line buffer
 * @param[in] size              buffer maximum length
 * @return                      The operation status.
 * @retval true                 if the channel was reset or CTRL-D pressed.
 * @retval false                if operation successful.
 *
 * @api
 */
bool xshellGetLine(xshell_manager_t *smp, shell_stream_i *stream,
                   char *line, size_t size) {
  char *p;

#if (XSHELL_LINE_EDITING == TRUE) || (XSHELL_HISTORY_DEPTH > 0)
  bool escape = false;
  bool bracket = false;

#if XSHELL_LINE_EDITING == TRUE
  bool vt      = false;
#endif
#if XSHELL_HISTORY_DEPTH > 0
  smp->history_current = smp->history_head;
#endif
#else

  (void)smp;
#endif

  memset(line, '\0', size);
  p = line;
  while (true) {
    char c;

    if (streamRead(stream, (uint8_t *)&c, 1) == 0)
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
            xshell_move_cursor(stream, (int)-1);
            p--;
          }
          continue;
        }
        if (c == 'C') {
          /* Cursor forward. Check if already at end of input.*/
          if (*p != '\0') {

            /* Not already at current end of input.*/
            xshell_move_cursor(stream, (int)1);
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
      xshell_reset_line(smp, stream);
      chprintf(stream, "%s", line);
      xshell_move_cursor(stream, (int)(-strlen(p)));
      continue;
    }
    vt = false;
    if ((c == CTRL('H')) || (c == 127)) {
      if (p != line) {
        p--;
        memmove(p, p + 1, strlen(p) + 1);
        xshell_reset_line(smp, stream);
        chprintf(stream, "%s", line);
        xshell_move_cursor(stream, (int)(-strlen(p)));
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
          size_t len = xshell_get_history_prev(smp, line);

          if (len > (size_t)0) {
            xshell_reset_line(smp, stream);
            chprintf(stream, "%s", line);
            p = line + len;
          }
          continue;
        }
        if (c == 'B') {
          /* Cursor down.*/
          size_t len = xshell_get_history_next(smp, line);

          if (len > (size_t)0) {
            xshell_reset_line(smp, stream);
            chprintf(stream, "%s", line);
            p = line + len;
          }
          continue;
        }
        if (c == 'C') {
          /* Cursor forward. Check if already at end of input.*/
          if (*p != '\0') {

            /* Not already at current end of input.*/
            xshell_move_cursor(stream, (int)1);
            p++;
          }
          continue;
        }
        if (c == 'D') {
          /* Cursor back.*/
          if (p != line) {
            xshell_move_cursor(stream, (int)-1);
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
      xshell_reset_line(smp, stream);
      chprintf(stream, "%s", line);
      xshell_move_cursor(stream, (int)(-strlen(p)));
      continue;
    }
    vt = false;
    if (c == CTRL('U')) {
      smp->history_current = smp->history_head;
      p = line;
      xshell_reset_line(smp, stream);
      continue;
    }
    if ((c == CTRL('H')) || (c == 127)) {
      if (p != line) {
        p--;
        memmove(p, p + 1, strlen(p) + 1);
        xshell_reset_line(smp, stream);
        chprintf(stream, "%s", line);
        xshell_move_cursor(stream, (int)(-strlen(p)));
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
          size_t len = xshell_get_history_prev(smp, line);

          if (len > (size_t)0) {
            xshell_reset_line(smp, stream);
            chprintf(stream, "%s", line);
            p = line + len;
          }
          continue;
        }
        if (c == 'B') {
          /* Cursor down.*/
          size_t len = xshell_get_history_next(smp, line);

          if (len > (size_t)0) {
            xshell_reset_line(smp, stream);
            chprintf(stream, "%s", line);
            p = line + len;
          }
          continue;
        }
      }
    }
    if (c == CTRL('U')) {
      smp->history_current = smp->history_head;
      p = line;
      xshell_reset_line(smp, stream);
      continue;
    }
    if ((c == CTRL('H')) || (c == 127)) {
      if (p != line) {
        streamWrite(stream, (const uint8_t *)"\010 \010", 3);
        p--;
      }
      continue;
    }
#endif /* (XSHELL_HISTORY_DEPTH > 0) && (XSHELL_LINE_EDITING == FALSE) */
#if (XSHELL_HISTORY_DEPTH == 0) && (XSHELL_LINE_EDITING == FALSE)
    if ((c == CTRL('H')) || (c == 127)) {
      if (p != line) {
        streamWrite(stream, (const uint8_t *)"\010 \010", 3);
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
      xshell_reset_line(smp, stream);
      chprintf(stream, "%s", line);
#endif
      chprintf(stream, XSHELL_NEWLINE_STR);
#if XSHELL_HISTORY_DEPTH > 0
      if (!xshell_is_line_empty(line)) {
        xshell_save_history(smp, line);
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
        xshell_reset_line(smp, stream);
        chprintf(stream, "%s", line);

        /* Restore cursor position.*/
        xshell_move_cursor(stream, -strlen(p) + 1);
      }
      else {

        /* At end of line so just output and store character.*/
        streamPut(stream, c);
        *p = (char)c;
      }
      p++;
    }
#else
    if (p < line + size - 1) {
      streamPut(stream, c);
      *p++ = (char)c;
    }
#endif
  }
}

/** @} */
