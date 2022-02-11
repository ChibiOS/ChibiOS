/*
    ChibiOS - Copyright (C) 2006,2007,2008,2009,2010,2011,2012,2013,2014,
              2015,2016,2017,2018,2019,2020,2021,2022 Giovanni Di Sirio.

    This file is part of ChibiOS.

    ChibiOS is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation version 3 of the License.

    ChibiOS is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

/**
 * @file    sb/common/sbsysc.h
 * @brief   Sandbox syscalls common macros and structures.
 *
 * @addtogroup ARM_SANDBOX_SYSCALLS
 * @{
 */

#ifndef SBSYSC_H
#define SBSYSC_H

/*===========================================================================*/
/* Module constants.                                                         */
/*===========================================================================*/

/**
 * @name    Syscall codes
 * @{
 */
#define SB_SYSC_POSIX           0
#define SB_SYSC_EXIT            1
#define SB_SYSC_GET_SYSTIME     2
#define SB_SYSC_GET_FREQUENCY   3
#define SB_SYSC_SLEEP           4
#define SB_SYSC_SLEEP_UNTIL     5
#define SB_SYSC_MESSAGE_WAIT    6
#define SB_SYSC_MESSAGE_REPLY   7
#define SB_SYSC_EVENT_WAIT_ONE  8
#define SB_SYSC_EVENT_WAIT_ANY  9
#define SB_SYSC_EVENT_WAIT_ALL  10
#define SB_SYSC_EVENT_BROADCAST 11
/** @} */

/**
 * @name    Posix syscall sub-codes
 * @{
 */
#define SB_POSIX_OPEN           1
#define SB_POSIX_CLOSE          2
#define SB_POSIX_DUP            3
#define SB_POSIX_DUP2           4
#define SB_POSIX_FSTAT          5
#define SB_POSIX_READ           6
#define SB_POSIX_WRITE          7
#define SB_POSIX_LSEEK          8
#define SB_POSIX_GETDENTS       9
#define SB_POSIX_CHDIR          10
#define SB_POSIX_GETCWD         11
#define SB_POSIX_UNLINK         12
#define SB_POSIX_RENAME         13
#define SB_POSIX_MKDIR          14
#define SB_POSIX_RMDIR          15
#define SB_POSIX_STAT           16
/** @} */

/*===========================================================================*/
/* Module pre-compile time settings.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

/*===========================================================================*/
/* Module data structures and types.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Module macros.                                                            */
/*===========================================================================*/

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __cplusplus
}
#endif

/*===========================================================================*/
/* Module inline functions.                                                  */
/*===========================================================================*/

#endif /* SBSYSC_H */

/** @} */
