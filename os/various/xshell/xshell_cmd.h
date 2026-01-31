/*
    Copyright (C) 2006-2026 Giovanni Di Sirio.

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
 * @file    xshell_cmd.h
 * @brief   Extended CLI shell common commands header.
 *
 * @addtogroup XSHELL
 * @{
 */

#ifndef XSHELLCMD_H
#define XSHELLCMD_H

/*===========================================================================*/
/* Module constants.                                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Module pre-compile time settings.                                         */
/*===========================================================================*/

#if !defined(XSHELL_CMD_EXIT_ENABLED) || defined(__DOXYGEN__)
#define XSHELL_CMD_EXIT_ENABLED             TRUE
#endif

#if !defined(XSHELL_CMD_INFO_ENABLED) || defined(__DOXYGEN__)
#define XSHELL_CMD_INFO_ENABLED             TRUE
#endif

#if !defined(XSHELL_CMD_ECHO_ENABLED) || defined(__DOXYGEN__)
#define XSHELL_CMD_ECHO_ENABLED             TRUE
#endif

#if !defined(XSHELL_CMD_SYSTIME_ENABLED) || defined(__DOXYGEN__)
#define XSHELL_CMD_SYSTIME_ENABLED          TRUE
#endif

#if !defined(XSHELL_CMD_MEM_ENABLED) || defined(__DOXYGEN__)
#define XSHELL_CMD_MEM_ENABLED              TRUE
#endif

#if !defined(XSHELL_CMD_THREADS_ENABLED) || defined(__DOXYGEN__)
#define XSHELL_CMD_THREADS_ENABLED          TRUE
#endif

#if !defined(XSHELL_CMD_TEST_ENABLED) || defined(__DOXYGEN__)
#define XSHELL_CMD_TEST_ENABLED             TRUE
#endif

#if !defined(XSHELL_CMD_FILES_ENABLED) || defined(__DOXYGEN__)
#define XSHELL_CMD_FILES_ENABLED            FALSE
#endif

#if !defined(XSHELL_CMD_FILES_BUFFER_SIZE)  || defined(__DOXYGEN__)
#define XSHELL_CMD_FILES_BUFFER_SIZE        2048U
#endif

#if !defined(XSHELL_CMD_TEST_WA_SIZE) || defined(__DOXYGEN__)
#define XSHELL_CMD_TEST_WA_SIZE             THD_WORKING_AREA_SIZE(512)
#endif

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

#if (XSHELL_CMD_MEM_ENABLED == TRUE) && (CH_CFG_USE_MEMCORE == FALSE)
#error "XSHELL_CMD_MEM_ENABLED requires CH_CFG_USE_MEMCORE"
#endif

#if (XSHELL_CMD_MEM_ENABLED == TRUE) && (CH_CFG_USE_HEAP == FALSE)
#error "XSHELL_CMD_MEM_ENABLED requires CH_CFG_USE_HEAP"
#endif

#if (XSHELL_CMD_THREADS_ENABLED == TRUE) && (CH_CFG_USE_REGISTRY == FALSE)
#error "XSHELL_CMD_THREADS_ENABLED requires CH_CFG_USE_REGISTRY"
#endif

#if (XSHELL_CMD_FILES_ENABLED == TRUE) && (CH_CFG_USE_HEAP == FALSE)
#error "XSHELL_CMD_FILES_ENABLED requires CH_CFG_USE_HEAP"
#endif

/*===========================================================================*/
/* Module data structures and types.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Module macros.                                                            */
/*===========================================================================*/

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#if !defined(__DOXYGEN__)
extern const xshell_command_t xshell_local_commands[];
#endif

#ifdef __cplusplus
extern "C" {
#endif
#if XSHELL_PROMPT_STR_LENGTH > 0
  bool xshellSetPrompt(xshell_manager_t *smp, const char *str);
#endif
#ifdef __cplusplus
}
#endif

/*===========================================================================*/
/* Module inline functions.                                                  */
/*===========================================================================*/

#endif /* XSHELLCMD_H */

/** @} */
