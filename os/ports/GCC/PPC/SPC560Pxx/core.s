/*
    ChibiOS/RT - Copyright (C) 2006,2007,2008,2009,2010,
                 2011,2012,2013 Giovanni Di Sirio.

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
*/

/**
 * @file    SPC560Pxx/core.s
 * @brief   e200z0 core configuration.
 *
 * @addtogroup PPC_CORE
 * @{
 */

/**
 * @name    BUCSR registers definitions
 * @{
 */
#define BUCSR_BPEN              0x00000001
#define BUCSR_BALLOC_BFI        0x00000200
/** @} */

/**
 * @name    BUCSR default settings
 * @{
 */
#define BUCSR_DEFAULT           (BUCSR_BPEN | BUCSR_BALLOC_BFI)
/** @} */

/**
 * @name   MSR register definitions
 * @{
 */
#define MSR_WE                  0x00040000
#define MSR_CE                  0x00020000
#define MSR_EE                  0x00008000
#define MSR_PR                  0x00004000
#define MSR_ME                  0x00001000
#define MSR_DE                  0x00000200
#define MSR_IS                  0x00000020
#define MSR_DS                  0x00000010
#define MSR_RI                  0x00000002
/** @} */

/**
 * @name   MSR default settings
 * @{
 */
#define MSR_DEFAULT             (MSR_WE | MSR_CE | MSR_ME)
/** @} */

#if !defined(__DOXYGEN__)

        .section    .coreinit, "ax"

        .align      2
        .globl      _coreinit
        .type       _coreinit, @function
_coreinit:
        /*
         * Branch prediction enabled.
         */
        li          %r3, BUCSR_DEFAULT
        mtspr       1013, %r3       /* BUCSR */

        blr

        /*
         * Exception vectors initialization.
         */
        .global     _ivinit
        .type       _ivinit, @function
_ivinit:
        /* MSR initialization.*/
        lis         %r3, MSR_DEFAULT@h
        ori         %r3, %r3, MSR_DEFAULT@l
        mtMSR       %r3

        /* IVPR initialization.*/
        lis         %r3, __ivpr_base__@h
        ori         %r3, %r3, __ivpr_base__@l
        mtIVPR      %r3

        blr

        .section    .handlers, "ax"

        .globl      IVORS
IVORS:
IVOR0:  b           IVOR0
        .align      4
IVOR1:  b           _IVOR1
        .align      4
IVOR2:  b           _IVOR2
        .align      4
IVOR3:  b           _IVOR3
        .align      4
IVOR4:  b           _IVOR4
        .align      4
IVOR5:  b           _IVOR5
        .align      4
IVOR6:  b           _IVOR6
        .align      4
IVOR7:  b           _IVOR7
        .align      4
IVOR8:  b           _IVOR8
        .align      4
IVOR9:  b           _IVOR9
        .align      4
IVOR10: b           _IVOR10
        .align      4
IVOR11: b           _IVOR11
        .align      4
IVOR12: b           _IVOR12
        .align      4
IVOR13: b           _IVOR13
        .align      4
IVOR14: b           _IVOR14
        .align      4
IVOR15: b           _IVOR15

        /*
         * Unhandled exceptions handler.
         */
        .weak       _IVOR0,  _IVOR1,  _IVOR2,  _IVOR3,  _IVOR4,  _IVOR5
        .weak       _IVOR6,  _IVOR7,  _IVOR8,  _IVOR9,  _IVOR10, _IVOR11
        .weak       _IVOR12, _IVOR13, _IVOR14, _IVOR15, _IVOR32, _IVOR33
        .weak       _IVOR34
        .weak       _unhandled_exception
_IVOR0:
_IVOR1:
_IVOR2:
_IVOR3:
_IVOR5:
_IVOR6:
_IVOR7:
_IVOR8:
_IVOR9:
_IVOR11:
_IVOR12:
_IVOR13:
_IVOR14:
_IVOR15:
_IVOR32:
_IVOR33:
_IVOR34:
        .type       _unhandled_exception, @function
_unhandled_exception:
        b           _unhandled_exception

#endif /* !defined(__DOXYGEN__) */

/** @} */
