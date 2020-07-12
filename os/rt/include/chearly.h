/*
    ChibiOS - Copyright (C) 2006..2018 Giovanni Di Sirio.

    This file is part of ChibiOS.

    ChibiOS is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 3 of the License, or
    (at your option) any later version.

    ChibiOS is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

/**
 * @file    chearly.h
 * @brief   Early forward types declarations header.
 *
 * @addtogroup os_structures
 * @{
 */

#ifndef CHEARLY_H
#define CHEARLY_H

/*===========================================================================*/
/* Module constants.                                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Module pre-compile time settings.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

/*===========================================================================*/
/* Module data structures and types.                                         */
/*===========================================================================*/

/**
 * @extends threads_queue_t
 *
 * @brief   Type of a thread structure.
 */
typedef struct ch_thread thread_t;

/**
 * @brief   Type of an OS instance structure.
 */
typedef struct ch_os_instance os_instance_t;

/*===========================================================================*/
/* Module macros.                                                            */
/*===========================================================================*/

/**
 * @brief   Utility to make the parameter a quoted string.
 */
#define __CH_STRINGIFY(a) #a

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

/* Early function prototypes required by the following headers.*/
#ifdef __cplusplus
extern "C" {
#endif
  void chSysHalt(const char *reason);
#ifdef __cplusplus
}
#endif

/*===========================================================================*/
/* Module inline functions.                                                  */
/*===========================================================================*/

#endif /* CHEARLY_H */

/** @} */
