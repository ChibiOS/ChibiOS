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
#define SB_SYSC_LOADELF         12
#define SB_SYSC_VHAL_VGPIO      200
#define SB_SYSC_VHAL_VUART      201
#define SB_SYSC_VRQ_SET_ALARM   245
#define SB_SYSC_VRQ_RESET_ALARM 246
#define SB_SYSC_VRQ_WAIT        247
#define SB_SYSC_VRQ_SETWT       248
#define SB_SYSC_VRQ_CLRWT       249
#define SB_SYSC_VRQ_SETEN       250
#define SB_SYSC_VRQ_CLREN       251
#define SB_SYSC_VRQ_DISABLE     252
#define SB_SYSC_VRQ_ENABLE      253
#define SB_SYSC_VRQ_GETISR      254
#define SB_SYSC_VRQ_RETURN      255
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

/**
 * @name    Virtual GPIO syscall sub-codes
 * @{
 */
#define SB_VGPIO_WRITE          0
#define SB_VGPIO_SET            1
#define SB_VGPIO_CLEAR          2
#define SB_VGPIO_TOGGLE         3
#define SB_VGPIO_READLATCH      4
#define SB_VGPIO_READ           5
#define SB_VGPIO_SETMODE        6
/** @} */

/**
 * @name    Virtual UART syscall sub-codes
 * @{
 */
#define SB_VUART_INIT           0
#define SB_VUART_DEINIT         1
#define SB_VUART_READ           2
#define SB_VUART_WRITE          3
#define SB_VUART_GET            4
#define SB_VUART_PUT            5
#define SB_VUART_WREN           6
#define SB_VUART_GCERR          7
#define SB_VUART_GCEVT          8
#define SB_VUART_CTL            9
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
