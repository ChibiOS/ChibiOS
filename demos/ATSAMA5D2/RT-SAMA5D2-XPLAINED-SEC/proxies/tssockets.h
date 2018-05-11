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
 * @file    tssockets.h
 */

#ifndef TSSOCKETS_H
#define TSSOCKETS_H

/*===========================================================================*/
/* Module constants.                                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Module pre-compile time settings.                                         */
/*===========================================================================*/
#define socket        ts_socket
#define connect       ts_connect
#define send          ts_send
#define recv          ts_recv
#define select        ts_select
#define close         ts_close
#define bind          ts_bind
#define listen        ts_listen
#define write         ts_write
#define read          ts_read
#define getaddrinfo   ts_getaddrinfo
#define freeaddrinfo  ts_freeaddrinfo

#define fd_set        ts_fd_set
#define timeval       ts_timeval

#define FD_ZERO       TS_FD_ZERO
#define FD_SET        TS_FD_SET
#define FD_CLR        TS_FD_CLR
#define FD_ISSET      TS_FD_ISSET
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

/*===========================================================================*/
/* Module inline functions.                                                  */
/*===========================================================================*/

#endif /* TSSOCKETS_H */
