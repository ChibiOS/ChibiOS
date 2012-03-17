/*
    ChibiOS/RT - Copyright (C) 2006,2007,2008,2009,2010,
                 2011,2012 Giovanni Di Sirio.

    This file is part of ChibiOS/RT.

    ChibiOS/RT is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 3 of the License, or
    (at your option) any later version.

    ChibiOS/RT is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

                                      ---

    A special exception to the GPL can be applied should you wish to distribute
    a combined work that includes ChibiOS/RT, without being obliged to provide
    the source code for any proprietary components. See the file exception.txt
    for full details of how and when the exception can be applied.
*/

/**
 * @file shell.h
 * @brief Simple CLI shell header.
 * @addtogroup SHELL
 * @{
 */

#ifndef _SHELL_H_
#define _SHELL_H_

/**
 * @brief Shell maximum input line length.
 */
#if !defined(SHELL_MAX_LINE_LENGTH) || defined(__DOXYGEN__)
#define SHELL_MAX_LINE_LENGTH       64
#endif

/**
 * @brief Shell maximum arguments per command.
 */
#if !defined(SHELL_MAX_ARGUMENTS) || defined(__DOXYGEN__)
#define SHELL_MAX_ARGUMENTS         4
#endif

/**
 * @brief Enforces the use of iprintf() on newlib.
 */
#if !defined(SHELL_USE_IPRINTF) || defined(__DOXYGEN__)
#define SHELL_USE_IPRINTF           TRUE
#endif

/**
 * @brief Command handler function type.
 */
typedef void (*shellcmd_t)(BaseChannel *chp, int argc, char *argv[]);

/**
 * @brief Custom command entry type.
 */
typedef struct {
  const char            *sc_name;           /**< @brief Command name.       */
  shellcmd_t            sc_function;        /**< @brief Command function.   */
} ShellCommand;

/**
 * @brief Shell descriptor type.
 */
typedef struct {
  BaseChannel           *sc_channel;        /**< @brief I/O channel associated
                                                 to the shell.              */
  const ShellCommand    *sc_commands;       /**< @brief Shell extra commands
                                                 table.                     */
} ShellConfig;

extern EventSource shell_terminated;

#ifdef __cplusplus
extern "C" {
#endif
  void shellInit(void);
  Thread *shellCreate(const ShellConfig *scp, size_t size, tprio_t prio);
  void shellPrint(BaseChannel *chp, const char *msg);
  void shellPrintLine(BaseChannel *chp, const char *msg);
  bool_t shellGetLine(BaseChannel *chp, char *line, unsigned size);
#ifdef __cplusplus
}
#endif

#endif /* _SHELL_H_ */

/** @} */
