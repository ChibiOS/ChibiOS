/*
    ChibiOS/RT - Copyright (C) 2009 Giovanni Di Sirio.

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

.section vectors
.code 32
.balign 4
/*
 * System entry points.
 */
_start:
        ldr     pc, _reset
        ldr     pc, _undefined
        ldr     pc, _swi
        ldr     pc, _prefetch
        ldr     pc, _abort
        nop
        ldr     pc, [pc,#-0xF20]        /* AIC - AIC_IVR */
        ldr     pc, [pc,#-0xF20]        /* AIC - AIC_FVR */

_reset:
        .word   ResetHandler            /* In crt0.s */
_undefined:
        .word   UndHandler
_swi:
        .word   SwiHandler
_prefetch:
        .word   PrefetchHandler
_abort:
        .word   AbortHandler
        .word   0
        .word   0
        .word   0

.text
.code 32
.balign 4

/*
 * Default exceptions handlers. The handlers are declared weak in order to be
 * replaced by the real handling code. Everything is defaulted to an infinite
 * loop.
 */
.weak UndHandler
UndHandler:

.weak SwiHandler
SwiHandler:

.weak PrefetchHandler
PrefetchHandler:

.weak AbortHandler
AbortHandler:

.weak FiqHandler
FiqHandler:

.loop:  b       .loop
