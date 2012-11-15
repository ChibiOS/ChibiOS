/*
    ChibiOS/RT - Copyright (C) 2006,2007,2008,2009,2010,
                 2011,2012 Giovanni Di Sirio.

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
 * @file    SPC560BCxx/ivor.s
 * @brief   SPC560BCxx IVORx handlers.
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

        /*
         * Fixed IVOR offset table.
         */
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
        .weak       _IVOR0
_IVOR0:
        .weak       _IVOR1
_IVOR1:
        .weak       _IVOR2
_IVOR2:
        .weak       _IVOR3
_IVOR3:
        .weak       _IVOR5
_IVOR5:
        .weak       _IVOR6
_IVOR6:
        .weak       _IVOR7
_IVOR7:
        .weak       _IVOR8
_IVOR8:
        .weak       _IVOR9
_IVOR9:
        .weak       _IVOR10
_IVOR10:
        .weak       _IVOR11
_IVOR11:
        .weak       _IVOR12
_IVOR12:
        .weak       _IVOR13
_IVOR13:
        .weak       _IVOR14
_IVOR14:
        .weak       _IVOR15
_IVOR15:
        .weak       _unhandled_exception
        .type       _unhandled_exception, @function
_unhandled_exception:
        b           _unhandled_exception

        /*
         * IVOR4 handler (Book-E external interrupt).
         */
        .align		4
        .globl      _IVOR4
        .type       _IVOR4, @function
_IVOR4:
        /* Creation of the external stack frame (extctx structure).*/
        stwu        %sp, -80(%sp)           /* Size of the extctx structure.*/
#if PPC_USE_VLE && PPC_SUPPORTS_VLE_MULTI
        e_stmvsrrw  8(%sp)                  /* Saves PC, MSR.               */
        e_stmvsprw  16(%sp)                 /* Saves CR, LR, CTR, XER.      */
        e_stmvgprw  32(%sp)                 /* Saves GPR0, GPR3...GPR12.    */
#else /* !(PPC_USE_VLE && PPC_SUPPORTS_VLE_MULTI) */
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
#endif /* !(PPC_USE_VLE && PPC_SUPPORTS_VLE_MULTI) */

        /* Software vector address from the INTC register.*/
        lis         %r3, INTC_IACKR@h
        ori         %r3, %r3, INTC_IACKR@l  /* IACKR register address.      */
        lwz         %r3, 0(%r3)             /* IACKR register value.        */
        lwz         %r3, 0(%r3)
        mtCTR       %r3                     /* Software handler address.    */

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

        /* Verifies if a reschedule is required.*/
#if CH_DBG_SYSTEM_STATE_CHECK
        bl          dbg_check_lock
#endif
        bl          chSchIsPreemptionRequired
        cmpli       cr0, %r3, 0
        beq         cr0, _ivor_exit
        bl          chSchDoReschedule

        /* Context restore.*/
        .globl      _ivor_exit
_ivor_exit:
#if CH_DBG_SYSTEM_STATE_CHECK
        bl          dbg_check_unlock
#endif
#if PPC_USE_VLE && PPC_SUPPORTS_VLE_MULTI
        e_lmvgprw  32(%sp)                  /* Restores GPR0, GPR3...GPR12. */
        e_lmvsprw  16(%sp)                  /* Restores CR, LR, CTR, XER.   */
        e_lmvsrrw  8(%sp)                   /* Restores PC, MSR.            */
#else /*!(PPC_USE_VLE && PPC_SUPPORTS_VLE_MULTI) */
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
#endif /* !(PPC_USE_VLE && PPC_SUPPORTS_VLE_MULTI) */
        addi        %sp, %sp, 80            /* Back to the previous frame.  */
        rfi

#endif /* !defined(__DOXYGEN__) */

/** @} */
