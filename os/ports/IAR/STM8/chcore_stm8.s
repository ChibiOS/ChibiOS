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

; Get definitions for virtual registers used by the compiler
#include "vregs.inc"

	SECTION .near_func.text:code
	EXTERN rlist

/*
 * Performs a context switch between two threads.
 */
        PUBLIC _port_switch
_port_switch:
	push    ?b8
	push    ?b9
	push    ?b10
	push    ?b11
	push    ?b12
	push    ?b13
	push    ?b14
	push    ?b15

	ldw     y,sp
	ldw     (5,x),y
	ldw     x, rlist + 5
	ldw     x,(5,x)
	ldw     sp,x

	pop     ?b15
	pop     ?b14
	pop     ?b13
	pop     ?b12
	pop     ?b11
	pop     ?b10
	pop     ?b9
	pop     ?b8
	ret

        END
