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
 * @file    xshell.h
 * @brief   Extended CLI shell header.
 *
 * @addtogroup XSHELL
 * @{
 */

#ifndef XSHELL_H
#define XSHELL_H

#if CH_HAL_MAJOR >= 10
#include "oop_chprintf.h"
#else
#include "chprintf.h"
#endif

#if defined(XSHELL_CONFIG_FILE)
#include "xshellconf.h"
#endif

/*===========================================================================*/
/* Module constants.                                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Module pre-compile time settings.                                         */
/*===========================================================================*/

/**
 * @brief   Shell maximum input line length inclusive or final zero.
 */
#if !defined(XSHELL_LINE_LENGTH) || defined(__DOXYGEN__)
#define XSHELL_LINE_LENGTH                  64
#endif

/**
 * @brief   History buffer depth, zero disables history.
 */
#if !defined(XSHELL_HISTORY_DEPTH) || defined(__DOXYGEN__)
#define XSHELL_HISTORY_DEPTH                4
#endif

/**
 * @brief   Shell maximum arguments per command.
 */
#if !defined(XSHELL_MAX_ARGUMENTS) || defined(__DOXYGEN__)
#define XSHELL_MAX_ARGUMENTS                4
#endif

/**
 * @brief   Newline string.
 */
#if !defined(XSHELL_NEWLINE_STR) || defined(__DOXYGEN__)
#define XSHELL_NEWLINE_STR                  "\r\n"
#endif

/**
 * @brief   Default prompt string.
 */
#if !defined(XSHELL_DEFAULT_PROMPT_STR) || defined(__DOXYGEN__)
#define XSHELL_DEFAULT_PROMPT_STR           "ch> "
#endif

/**
 * @brief   Set prompt string length.
 */
#if !defined(XSHELL_PROMPT_STR_LENGTH) || defined(__DOXYGEN__)
#define XSHELL_PROMPT_STR_LENGTH            20
#endif

/**
 * @brief   Shell default banner.
 */
#if !defined(XSHELL_DEFAULT_BANNER_STR) || defined(__DOXYGEN__)
#define XSHELL_DEFAULT_BANNER_STR           "\r\nChibiOS/RT XShell\r\n"
#endif

/**
 * @brief   Command line terminator character (any one of string).
 */
#if !defined(XSHELL_EXECUTE_CHARS) || defined(__DOXYGEN__)
#define XSHELL_EXECUTE_CHARS                "\r\n"
#endif

/**
 * @brief   Logout string.
 */
#if !defined(XSHELL_LOGOUT_STR) || defined(__DOXYGEN__)
#define XSHELL_LOGOUT_STR                   "\r\n"
#endif

/**
 * @brief   Multi-command line mode.
 */
#if !defined(XSHELL_MULTI_COMMAND_LINE) || defined(__DOXYGEN__)
#define XSHELL_MULTI_COMMAND_LINE           FALSE
#endif

/**
 * @brief   Line editing mode.
 */
#if !defined(XSHELL_LINE_EDITING) || defined(__DOXYGEN__)
#define XSHELL_LINE_EDITING                 TRUE
#endif

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

#if CH_CFG_USE_DYNAMIC == FALSE
#error "XSHELL requires CH_CFG_USE_DYNAMIC"
#endif

#if CH_CFG_USE_MEMPOOLS == FALSE
#error "XSHELL requires CH_CFG_USE_MEMPOOLS"
#endif

#if CH_HAL_MAJOR < 10
#define shell_stream_i                      BaseSequentialStream
#else
#define shell_stream_i                      sequential_stream_i
#endif

/*===========================================================================*/
/* Module data structures and types.                                         */
/*===========================================================================*/

/* Forward.*/
typedef struct xshell_manager xshell_manager_t;
typedef struct xshell xshell_t;

/**
 * @brief   Type of a command handler function.
 */
typedef void (*xshellcmd_t)(xshell_t *xshp, int argc, char *argv[], char *envp[]);

/**
 * @brief   Type of a shell custom command structure.
 */
typedef struct xshell_command {
  /**
   * @brief   Command name.
   */
  const char                    *name;
  /**
   * @brief   Command function.
   */
  xshellcmd_t                   fn;
} xshell_command_t;

/**
 * @brief   Type of a XShell manager configuration structure.
 */
typedef struct xshell_manager_config {
  /**
   * @brief   Shells name.
   */
  const char                    *thread_name;
  /**
   * @brief   Shells banner string.
   */
  const char                    *banner;
  /**
   * @brief   Shells default prompt string.
   */
  const char                    *prompt;
  /**
   * @brief   List of the associated shell commands.
   */
  const xshell_command_t        *commands;
#if (CH_CFG_USE_HEAP == TRUE) || defined(__DOXYGEN__)
  /**
   * @brief   Uses heap allocator instead of memory pools for stacks.
   */
  bool                          use_heap;
#endif
  union {
    /**
     * @brief   Pool of shell stack areas.
     */
    memory_pool_t               *pool;
#if (CH_CFG_USE_HEAP == TRUE) || defined(__DOXYGEN__)
    /**
     * @brief   Stack size for shells.
     */
    size_t                      size;
#endif
  } stack;
#if defined(XSHELL_CONFIG_EXTRA_FIELDS)
  /* Extra fields defined in xshellconf.h.*/
  XSHELL_CONFIG_EXTRA_FIELDS
#endif
} xshell_manager_config_t;

/**
 * @brief   Type of an XShell Manager.
 */
typedef struct xshell_manager {
  /**
   * @brief   Associated configuration.
   */
  const xshell_manager_config_t *config;
#if XSHELL_PROMPT_STR_LENGTH > 0
  /**
   * @brief   Shells current prompt string.
   */
  char                          prompt[XSHELL_PROMPT_STR_LENGTH + 1];
#endif
  /**
   * @brief   Shell events;
   */
  event_source_t                events;
#if 0
#if (XSHELL_HISTORY_DEPTH > 0) || defined(__DOXYGEN__)
  /**
   * @brief   Mutex protecting the history buffer.
   */
  mutex_t                       history_mutex;
  /**
   * @brief   Head of history circular buffer.
   */
  char                          *history_head;
  /**
   * @brief   Current position in history circular buffer.
   */
  char                          *history_current;
  /**
   * @brief   History buffer.
   */
  char                          history_buffer[XSHELL_HISTORY_DEPTH][XSHELL_LINE_LENGTH];
#endif
#endif
#if defined(XSHELL_MGR_EXTRA_FIELDS)
  /* Extra fields defined in xshellconf.h.*/
  XSHELL_MGR_EXTRA_FIELDS
#endif
} xshell_manager_t;

/**
 * @brief   Type of a shell instance.
 */
typedef struct xshell {
  /**
   * @brief   I/O stream associated to the shell.
   */
  shell_stream_i                *stream;
  /**
   * @brief   Shell environment or @p NULL.
   */
  char                          **envp;
  /**
   * @brief   Thread running this shell.
   */
  thread_t                      thread;
  /**
   * @brief   Shell command line buffer.
   */
  char                          line[XSHELL_LINE_LENGTH];
#if (XSHELL_HISTORY_DEPTH > 0) || defined(__DOXYGEN__)
  /**
   * @brief   Head of history circular buffer.
   */
  char                          *history_head;
  /**
   * @brief   Current position in history circular buffer.
   */
  char                          *history_current;
  /**
   * @brief   History buffer.
   */
  char                          history_buffer[XSHELL_HISTORY_DEPTH][XSHELL_LINE_LENGTH];
#endif
  /**
   * @brief   Command argument pointers array.
   * @note    1st argument is the command name itself, the array is
   *          NULL-terminated.
   */
  char *args[XSHELL_MAX_ARGUMENTS + 2];
#if defined(XSHELL_EXTRA_FIELDS)
  /* Extra fields defined in xshellconf.h.*/
  XSHELL_EXTRA_FIELDS
#endif
} xshell_t;

/*===========================================================================*/
/* Module macros.                                                            */
/*===========================================================================*/

/* https://en.wikipedia.org/wiki/ANSI_escape_code */
/**
 * @brief   Send escape codes to move cursor to the beginning of the line
 *
 * @param[in] stream    pointer to a @p shell_stream_i object
 *
 * @notapi
 */
#define _shell_reset_cur(stream) chprintf(stream, "\033[%dD\033[%dC",       \
                                          SHELL_MAX_LINE_LENGTH +           \
                                          strlen(SHELL_PROMPT_STR) + 2,     \
                                          strlen(SHELL_PROMPT_STR))

/**
 * @brief   Send escape codes to clear the rest of the line
 *
 * @param[in] stream    pointer to a @p shell_stream_i object
 *
 * @notapi
 */
#define _shell_clr_line(stream)   chprintf(stream, "\033[K")

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#ifdef __cplusplus
extern "C" {
#endif
  void xshellObjectInit(xshell_manager_t *smp,
                        const xshell_manager_config_t *config);
  xshell_t *xshellSpawn(xshell_manager_t *smp,
                        shell_stream_i *stream,
                        tprio_t prio,
                        char *envp[]);
  bool xshellGetLine(xshell_t *xshp, char *line, size_t size);
#ifdef __cplusplus
}
#endif

/*===========================================================================*/
/* Module inline functions.                                                  */
/*===========================================================================*/

/**
 * @brief   Prints out usage message
 *
 * @param[in] xshp              pointer to a @p xshell_t object
 * @param[in] message           pointer to message string
 *
 * @api
 */
static inline void xshellUsage(xshell_t *xshp, const char *message) {

  chprintf(xshp->stream, "Usage: %s" XSHELL_NEWLINE_STR, message);
}

/**
 * @brief   Returns the shell manager associated to a shell.
 *
 * @param[in] xshp              pointer to a @p xshell_t object
 * @return                      Pointer to a @p shell_manager_t object.
 *
 * @api
 */
static inline xshell_manager_t *xshellGetManager(xshell_t *xshp) {

  return xshp->thread.object;
}

/**
 * @brief   Waits for a shell to terminate.
 *
 * @param[in] xshp              pointer to a @p xshell_t object
 * @return                      The shell exit code.
 *
 * @api
 */
static inline msg_t xshellWait(xshell_t *xshp) {

  return chThdWait(&xshp->thread);
}

#endif /* XSHELL_H */

/** @} */
