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
 * @file    SPC56ELxx/ivor.s
 * @brief   SPC56ELxx IVORx handlers.
 *
 * @addtogroup PPC_CORE
 * @{
 */

/*
 * Imports the PPC configuration headers.
 */
#define _FROM_ASM_
#include "chconf.h"
#include "chcore.h"

#if !defined(__DOXYGEN__)
        /*
         * INTC registers address.
         */
        .equ  INTC_IACKR, 0xfff48010
        .equ  INTC_EOIR,  0xfff48018

        .section    .handlers, "ax"

        .align      2
        .globl      _ivorinit
        .type       _ivorinit, @function
_ivorinit:
        /*
         * IVPR initialization.
         */
        lis         %r4, __ivpr_base__@h
        ori         %r4, %r4, __ivpr_base__@l
        mtIVPR      %r4

        /*
         * IVOR default settings.
         */
        lis         %r4, _unhandled_exception@h
        ori         %r4, %r4, _unhandled_exception@l
        mtspr       400, %r4    /* IVOR0-15 */
        mtspr       401, %r4
        mtspr       402, %r4
        mtspr       403, %r4
        mtspr       404, %r4
        mtspr       405, %r4
        mtspr       406, %r4
        mtspr       407, %r4
        mtspr       408, %r4
        mtspr       409, %r4
        mtspr       410, %r4
        mtspr       411, %r4
        mtspr       412, %r4
        mtspr       413, %r4
        mtspr       414, %r4
        mtspr       415, %r4
        mtspr       528, %r4    /* IVOR32-34 */
        mtspr       529, %r4
        mtspr       530, %r4

        blr

        /*
         * Unhandled exceptions handler.
         */
         .weak      _IVOR0
_IVOR0:
         .weak      _IVOR1
_IVOR1:
         .weak      _IVOR2
_IVOR2:
         .weak      _IVOR3
_IVOR3:
         .weak      _IVOR5
_IVOR5:
         .weak      _IVOR6
_IVOR6:
         .weak      _IVOR7
_IVOR7:
         .weak      _IVOR8
_IVOR8:
         .weak      _IVOR9
_IVOR9:
         .weak      _IVOR11
_IVOR11:
         .weak      _IVOR12
_IVOR12:
         .weak      _IVOR13
_IVOR13:
         .weak      _IVOR14
_IVOR14:
         .weak      _IVOR15
_IVOR15:
        .weak      _unhandled_exception
        .type       _unhandled_exception, @function
_unhandled_exception:
        b       _unhandled_exception

#endif /* !defined(__DOXYGEN__) */

/** @} */
