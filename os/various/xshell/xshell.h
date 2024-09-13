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
#define XSHELL_HISTORY_DEPTH                0
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
 * @brief   Prompt string.
 */
#if !defined(XSHELL_DEFAULT_PROMPT_STR) || defined(__DOXYGEN__)
#define XSHELL_DEFAULT_PROMPT_STR           "ch> "
#endif

/**
 * @brief   Shell default banner.
 */
#if !defined(XSHELL_DEFAULT_BANNER_STR) || defined(__DOXYGEN__)
#define XSHELL_DEFAULT_BANNER_STR           "\r\nChibiOS/RT XShell\r\n"
#endif

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

/*===========================================================================*/
/* Module data structures and types.                                         */
/*===========================================================================*/

/* Forward.*/
typedef struct xshell_manager xshell_manager_t;

/**
 * @brief   Type of a command handler function.
 */
typedef void (*xshellcmd_t)(xshell_manager_t *smp, BaseSequentialStream *chp,
                            int argc, char *argv[]);

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
   * @brief   Shells prompt string.
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
} xshell_manager_config_t;

/**
 * @brief   Type of an XShell Manager.
 */
typedef struct xshell_manager {
  /**
   * @brief   Associated configuration.
   */
  const xshell_manager_config_t *config;
  /**
   * @brief   Shell events;
   */
  event_source_t                events;
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
   * @brief   History buffer.
   */
  char                          history_buffer[XSHELL_HISTORY_DEPTH][XSHELL_LINE_LENGTH];
#endif
} xshell_manager_t;

/*===========================================================================*/
/* Module macros.                                                            */
/*===========================================================================*/

/* https://en.wikipedia.org/wiki/ANSI_escape_code */
/**
 * @brief   Send escape codes to move cursor to the beginning of the line
 *
 * @param[in] stream    pointer to a @p BaseSequentialStream object
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
 * @param[in] stream    pointer to a @p BaseSequentialStream object
 *
 * @notapi
 */
#define _shell_clr_line(stream)   chprintf(stream, "\033[K")

/**
 * @brief   Prints out usage message
 *
 * @param[in] stream            pointer to a stream interface
 * @param[in] message           pointer to message string
 *
 * @api
 */
#define xshellUsage(stream, message)                                        \
  chprintf(stream, "Usage: %s" XSHELL_NEWLINE_STR, message)

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#ifdef __cplusplus
extern "C" {
#endif
  void xshellObjectInit(xshell_manager_t *smp,
                        const xshell_manager_config_t *config);
  thread_t *xshellSpawn(xshell_manager_t *smp, BaseSequentialStream *stp);
  void xshellGarbageCollect(xshell_manager_t *smp);
  void xshellExit(xshell_manager_t *smp, msg_t msg);
  bool xshellGetLine(xshell_manager_t *smp, BaseSequentialStream *stp,
                     char *line, size_t size);
#ifdef __cplusplus
}
#endif

/*===========================================================================*/
/* Module inline functions.                                                  */
/*===========================================================================*/

#endif /* XSHELL_H */

/** @} */
