/*
    ChibiOS - Copyright (C) 2006-2026 Giovanni Di Sirio.

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
 * @name    Fastcall codes
 * @{
 */
#define SB_FASTC_GET_SYSTIME    1
#define SB_FASTC_GET_FREQUENCY  2
#define SB_FASTC_VIO_VGPIO      96
#define SB_FASTC_VIO_VUART      97
#define SB_FASTC_VIO_VSPI       98
#define SB_FASTC_VRQ_GCSTS      119
#define SB_FASTC_VRQ_SETWT      120
#define SB_FASTC_VRQ_CLRWT      121
#define SB_FASTC_VRQ_SETEN      122
#define SB_FASTC_VRQ_CLREN      123
#define SB_FASTC_VRQ_DISABLE    124
#define SB_FASTC_VRQ_ENABLE     125
#define SB_FASTC_VRQ_GETISR     126
#define SB_FASTC_VRQ_RETURN     127
/** @} */

/**
 * @name    Syscall codes
 * @{
 */
#define SB_SYSC_POSIX           128
#define SB_SYSC_EXIT            129
#define SB_SYSC_SLEEP           130
#define SB_SYSC_SLEEP_UNTIL     131
#define SB_SYSC_MESSAGE_WAIT    132
#define SB_SYSC_MESSAGE_REPLY   133
#define SB_SYSC_EVENT_WAIT_ONE  134
#define SB_SYSC_EVENT_WAIT_ANY  135
#define SB_SYSC_EVENT_WAIT_ALL  136
#define SB_SYSC_EVENT_BROADCAST 137
#define SB_SYSC_LOADELF         138
#define SB_SYSC_VIO_VUART       225
#define SB_SYSC_VIO_VSPI        226
#define SB_SYSC_VRQ_SET_ALARM   253
#define SB_SYSC_VRQ_RESET_ALARM 254
#define SB_SYSC_VRQ_WAIT        255
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
 * @name    Virtual SPI syscall sub-codes
 * @{
 */
#define SB_VSPI_INIT            0
#define SB_VSPI_DEINIT          1
#define SB_VSPI_PULSES          2
#define SB_VSPI_RECEIVE         3
#define SB_VSPI_SEND            4
#define SB_VSPI_EXCHANGE        5
#define SB_VSPI_STOP            6
#define SB_VSPI_SELCFG          0
#define SB_VSPI_SELECT          1
#define SB_VSPI_UNSELECT        2
/** @} */

/**
 * @name    Virtual UART syscall sub-codes
 * @{
 */
#define SB_VUART_INIT           0
#define SB_VUART_DEINIT         1
#define SB_VUART_SELCFG         0
#define SB_VUART_ISRXE          1
#define SB_VUART_ISRXI          2
#define SB_VUART_ISTXF          3
#define SB_VUART_ISTXO          4
#define SB_VUART_HASERR         5
#define SB_VUART_READ           6
#define SB_VUART_WRITE          7
#define SB_VUART_GET            8
#define SB_VUART_PUT            9
#define SB_VUART_WREN           10
#define SB_VUART_GCERR          11
#define SB_VUART_GCEVT          12
#define SB_VUART_GEVT           13
#define SB_VUART_CTL            14
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
