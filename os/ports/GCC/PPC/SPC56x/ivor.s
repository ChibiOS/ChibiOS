/*
    ChibiOS/RT - Copyright (C) 2006-2007 Giovanni Di Sirio.

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
 * @file    PPC/ivor.s
 * @brief   PowerPC IVORx handlers.
 *
 * @addtogroup PPC_CORE
 * @{
 */
/** @cond never */

        /*
         * INTC registers address.
         */
        .equ  INTC_IACKR, 0xfff48010
        .equ  INTC_EOIR,  0xfff48018

        .section    .handlers

        /*
         * IVOR10 handler (Book-E decrementer).
         */
        .align		4
        .globl      IVOR10
IVOR10:
        /* Creation of the external stack frame (extctx structure).*/
        stwu        %sp, -80(%sp)           /* Size of the extctx structure.*/
        stw         %r0, 32(%sp)            /* Saves GPR0.                  */
        mfSRR0      %r0
        stw         %r0, 8(%sp)             /* Saves PC.                    */
        mfSRR1      %r0
        stw         %r0, 12(%sp)            /* Saves MSR.                   */
        mfCR        %r0
        stw         %r0, 16(%sp)            /* Saves CR.                    */
        mfLR        %r0
        stw         %r0, 20(%sp)            /* Saves LR.                    */
        mfCTR       %r0
        stw         %r0, 24(%sp)            /* Saves CTR.                   */
        mfXER       %r0
        stw         %r0, 28(%sp)            /* Saves XER.                   */
        stw         %r3, 36(%sp)            /* Saves GPR3...GPR12.          */
        stw         %r4, 40(%sp)
        stw         %r5, 44(%sp)
        stw         %r6, 48(%sp)
        stw         %r7, 52(%sp)
        stw         %r8, 56(%sp)
        stw         %r9, 60(%sp)
        stw         %r10, 64(%sp)
        stw         %r11, 68(%sp)
        stw         %r12, 72(%sp)

        /* Reset DIE bit in TSR register.*/
        lis         %r3, 0x0800             /* DIS bit mask.                */
        mtspr       336, %r3                /* TSR register.                */

        /* System tick handler invokation.*/
        bl          chSysTimerHandlerI
        bl          chSchIsRescRequiredExI
        cmpli       cr0, %r3, 0
        beq         cr0, .ctxrestore
        bl          chSchDoRescheduleI
        b           .ctxrestore

        /*
         * IVOR4 handler (Book-E external interrupt).
         */
        .align		4
        .globl      IVOR4
IVOR4:
        /* Creation of the external stack frame (extctx structure).*/
        stwu        %sp, -80(%sp)           /* Size of the extctx structure.*/
        stw         %r0, 32(%sp)            /* Saves GPR0.                  */
        mfSRR0      %r0
        stw         %r0, 8(%sp)             /* Saves PC.                    */
        mfSRR1      %r0
        stw         %r0, 12(%sp)            /* Saves MSR.                   */
        mfCR        %r0
        stw         %r0, 16(%sp)            /* Saves CR.                    */
        mfLR        %r0
        stw         %r0, 20(%sp)            /* Saves LR.                    */
        mfCTR       %r0
        stw         %r0, 24(%sp)            /* Saves CTR.                   */
        mfXER       %r0
        stw         %r0, 28(%sp)            /* Saves XER.                   */
        stw         %r3, 36(%sp)            /* Saves GPR3...GPR12.          */
        stw         %r4, 40(%sp)
        stw         %r5, 44(%sp)
        stw         %r6, 48(%sp)
        stw         %r7, 52(%sp)
        stw         %r8, 56(%sp)
        stw         %r9, 60(%sp)
        stw         %r10, 64(%sp)
        stw         %r11, 68(%sp)
        stw         %r12, 72(%sp)

        /* Software vector address from the INTC register.*/
        lis         %r3, INTC_IACKR@h
        ori         %r3, %r3, INTC_IACKR@l  /* IACKR register address.      */
        lwz         %r3, 0(%r3)             /* IACKR register value.        */
        lwz         %r3, 0(%r3)
        mtCTR        %r3                    /* Software handler address.    */

#if PPC_USE_IRQ_PREEMPTION
        /* Allows preemption while executing the software handler.*/
        wrteei      1
#endif

        /* Exectes the software handler.*/
        bctrl

#if PPC_USE_IRQ_PREEMPTION
        /* Prevents preemption again.*/
        wrteei      0
#endif

        /* Informs the INTC that the interrupt has been served.*/
        mbar        0
        lis         %r3, INTC_EOIR@h
        ori         %r3, %r3, INTC_EOIR@l
        stw         %r3, 0(%r3)             /* Writing any value should do. */

        /* Verifies if a reschedulation is required.*/
        bl          chSchIsRescRequiredExI
        cmpli       cr0, %r3, 0
        beq         cr0, .ctxrestore
        bl          chSchDoRescheduleI

        /* Context restore.*/
.ctxrestore:
        lwz         %r3, 36(%sp)            /* Restores GPR3...GPR12.       */
        lwz         %r4, 40(%sp)
        lwz         %r5, 44(%sp)
        lwz         %r6, 48(%sp)
        lwz         %r7, 52(%sp)
        lwz         %r8, 56(%sp)
        lwz         %r9, 60(%sp)
        lwz         %r10, 64(%sp)
        lwz         %r11, 68(%sp)
        lwz         %r12, 72(%sp)
        lwz         %r0, 8(%sp)
        mtSRR0      %r0                     /* Restores PC.                 */
        lwz         %r0, 12(%sp)
        mtSRR1      %r0                     /* Restores MSR.                */
        lwz         %r0, 16(%sp)
        mtCR        %r0                     /* Restores CR.                 */
        lwz         %r0, 20(%sp)
        mtLR        %r0                     /* Restores LR.                 */
        lwz         %r0, 24(%sp)
        mtCTR       %r0                     /* Restores CTR.                */
        lwz         %r0, 28(%sp)
        mtXER       %r0                     /* Restores XER.                */
        lwz         %r0, 32(%sp)            /* Restores GPR0.               */
        addi        %sp, %sp, 80            /* Back to the previous frame.  */
        rfi

/** @endcond */
/** @} */
