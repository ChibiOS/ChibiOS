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
 * @file    SPC56ELxx/bam.s
 * @brief   SPC56ELxx boot assistant record.
 *
 * @addtogroup PPC_CORE
 * @{
 */

#if !defined(__DOXYGEN__)

        /* BAM info, SWT off, WTE off, VLE from settings.*/
        .section    .bam, "ax"
        .long       0x015A0000
        .long       .clear_ecc

        /* RAM clearing, this device requires a write to all RAM location in
           order to initialize the ECC detection hardware, this is going to
           slow down the startup but there is no way around.
           Note all registers are cleared in order to avoid possible problems
           with lockstep mode.*/
.clear_ecc:
        xor         %r0, %r0, %r0
        xor         %r1, %r1, %r1
        xor         %r2, %r2, %r2
        xor         %r3, %r3, %r3
        xor         %r4, %r4, %r4
        xor         %r5, %r5, %r5
        xor         %r6, %r6, %r6
        xor         %r7, %r7, %r7
        xor         %r8, %r8, %r8
        xor         %r9, %r9, %r9
        xor         %r10, %r10, %r10
        xor         %r11, %r11, %r11
        xor         %r12, %r12, %r12
        xor         %r13, %r13, %r13
        xor         %r14, %r14, %r14
        xor         %r15, %r15, %r15
        xor         %r16, %r16, %r16
        xor         %r17, %r17, %r17
        xor         %r18, %r18, %r18
        xor         %r19, %r19, %r19
        xor         %r20, %r20, %r20
        xor         %r21, %r21, %r21
        xor         %r22, %r22, %r22
        xor         %r23, %r23, %r23
        xor         %r24, %r24, %r24
        xor         %r25, %r25, %r25
        xor         %r26, %r26, %r26
        xor         %r27, %r27, %r27
        xor         %r28, %r28, %r28
        xor         %r29, %r29, %r29
        xor         %r30, %r30, %r30
        xor         %r31, %r31, %r31
        lis         %r4, __ram_start__@h
        ori         %r4, %r4, __ram_start__@l
        lis         %r5, __ram_end__@h
        ori         %r5, %r5, __ram_end__@l
.cleareccloop:
        cmpl        cr0, %r4, %r5
        bge         cr0, .cleareccend
        stmw        %r16, 0(%r4)
        addi        %r4, %r4, 64
        b           .cleareccloop
.cleareccend:

        /* HW configuration.*/
        bl          _coreinit

        b           _boot_address

#endif /* !defined(__DOXYGEN__) */

/** @} */
