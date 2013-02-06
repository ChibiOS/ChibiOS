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
 * @file    SPC56ELxx/core.s
 * @brief   e200z4 core configuration.
 *
 * @addtogroup PPC_CORE
 * @{
 */

/**
 * @name   MSR register definitions
 * @{
 */
#define MSR_UCLE                0x04000000
#define MSR_SPE                 0x02000000
#define MSR_WE                  0x00040000
#define MSR_CE                  0x00020000
#define MSR_EE                  0x00008000
#define MSR_PR                  0x00004000
#define MSR_FP                  0x00002000
#define MSR_ME                  0x00001000
#define MSR_FE0                 0x00000800
#define MSR_DE                  0x00000200
#define MSR_FE1                 0x00000100
#define MSR_IS                  0x00000020
#define MSR_DS                  0x00000010
#define MSR_RI                  0x00000002
/** @} */

/**
 * @name   MASx registers definitions
 * @{
 */
#define MAS0_TBLMAS_TBL         0x10000000
#define MAS0_ESEL_MASK          0x000F0000
#define MAS0_ESEL(n)            ((n) << 16)

#define MAS1_VALID              0x80000000
#define MAS1_IPROT              0x40000000
#define MAS1_TID_MASK           0x00FF0000
#define MAS1_TS                 0x00001000
#define MAS1_TSISE_MASK         0x00000F80
#define MAS1_TSISE_1K           0x00000000
#define MAS1_TSISE_2K           0x00000080
#define MAS1_TSISE_4K           0x00000100
#define MAS1_TSISE_8K           0x00000180
#define MAS1_TSISE_16K          0x00000200
#define MAS1_TSISE_32K          0x00000280
#define MAS1_TSISE_64K          0x00000300
#define MAS1_TSISE_128K         0x00000380
#define MAS1_TSISE_256K         0x00000400
#define MAS1_TSISE_512K         0x00000480
#define MAS1_TSISE_1M           0x00000500
#define MAS1_TSISE_2M           0x00000580
#define MAS1_TSISE_4M           0x00000600
#define MAS1_TSISE_8M           0x00000680
#define MAS1_TSISE_16M          0x00000700
#define MAS1_TSISE_32M          0x00000780
#define MAS1_TSISE_64M          0x00000800
#define MAS1_TSISE_128M         0x00000880
#define MAS1_TSISE_256M         0x00000900
#define MAS1_TSISE_512M         0x00000980
#define MAS1_TSISE_1G           0x00000A00
#define MAS1_TSISE_2G           0x00000A80
#define MAS1_TSISE_4G           0x00000B00

#define MAS2_EPN_MASK           0xFFFFFC00
#define MAS2_EPN(n)             ((n) & MAS2_EPN_MASK)
#define MAS2_EBOOK              0x00000000
#define MAS2_VLE                0x00000020
#define MAS2_W                  0x00000010
#define MAS2_I                  0x00000008
#define MAS2_M                  0x00000004
#define MAS2_G                  0x00000002
#define MAS2_E                  0x00000001

#define MAS3_RPN_MASK           0xFFFFFC00
#define MAS3_RPN(n)             ((n) & MAS3_RPN_MASK)
#define MAS3_U0                 0x00000200
#define MAS3_U1                 0x00000100
#define MAS3_U2                 0x00000080
#define MAS3_U3                 0x00000040
#define MAS3_UX                 0x00000020
#define MAS3_SX                 0x00000010
#define MAS3_UW                 0x00000008
#define MAS3_SW                 0x00000004
#define MAS3_UR                 0x00000002
#define MAS3_SR                 0x00000001
/** @} */

/**
 * @name    MSR default settings
 */
#define MSR_DEFAULT             (0x00001000)
/** @} */

/**
 * @name    TLB default settings
 * @{
 */
#define TLB0_MAS0               (MAS0_TBLMAS_TBL | MAS0_ESEL(0))
#define TLB0_MAS1               (MAS1_VALID | MAS1_IPROT | MAS1_TSISE_2M)
#define TLB0_MAS2               (MAS2_EPN(0x00000000) | MAS2_VLE)
#define TLB0_MAS3               (MAS3_RPN(0x00000000) |                     \
                                 MAS3_UX | MAS3_SX | MAS3_UW | MAS3_SW |    \
                                 MAS3_UR | MAS3_SR)

#define TLB1_MAS0               (MAS0_TBLMAS_TBL | MAS0_ESEL(1))
#define TLB1_MAS1               (MAS1_VALID | MAS1_IPROT | MAS1_TSISE_16M)
#define TLB1_MAS2               (MAS2_EPN(0x20000000) | MAS2_VLE)
#define TLB1_MAS3               (MAS3_RPN(0x20000000) |                     \
                                 MAS3_UX | MAS3_SX | MAS3_UW | MAS3_SW |    \
                                 MAS3_UR | MAS3_SR)

#define TLB2_MAS0               (MAS0_TBLMAS_TBL | MAS0_ESEL(2))
#define TLB2_MAS1               (MAS1_VALID | MAS1_IPROT | MAS1_TSISE_128K)
#define TLB2_MAS2               (MAS2_EPN(0x40000000) | MAS2_VLE | MAS2_I)
#define TLB2_MAS3               (MAS3_RPN(0x40000000) |                     \
                                 MAS3_UX | MAS3_SX | MAS3_UW | MAS3_SW |    \
                                 MAS3_UR | MAS3_SR)

#define TLB3_MAS0               (MAS0_TBLMAS_TBL | MAS0_ESEL(3))
#define TLB3_MAS1               (MAS1_VALID | MAS1_IPROT | MAS1_TSISE_1M)
#define TLB3_MAS2               (MAS2_EPN(0xC3F00000) | MAS2_I)
#define TLB3_MAS3               (MAS3_RPN(0xC3F00000) |                     \
                                 MAS3_UW | MAS3_SW | MAS3_UR | MAS3_SR)

#define TLB4_MAS0               (MAS0_TBLMAS_TBL | MAS0_ESEL(4))
#define TLB4_MAS1               (MAS1_VALID | MAS1_IPROT | MAS1_TSISE_2M)
#define TLB4_MAS2               (MAS2_EPN(0xFFE00000) | MAS2_I)
#define TLB4_MAS3               (MAS3_RPN(0xFFE00000) |                     \
                                 MAS3_UW | MAS3_SW | MAS3_UR | MAS3_SR)
/** @} */

#if !defined(__DOXYGEN__)

        .section    .handlers, "ax"

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

        .section    .coreinit, "ax"

        .align      2
        .globl      _coreinit
        .type       _coreinit, @function
_coreinit:
        /*
         * Special function registers clearing, required in order to avoid
         * possible problems with lockstep mode.
         */
        xor         %r31, %r31, %r31
        mtcrf       0xFF, %r31
        mtspr       8, %r31         /* LR      */
        mtspr       9, %r31         /* CTR     */
        mtspr       22, %r31        /* DEC     */
        mtspr       26, %r31        /* SRR0-1  */
        mtspr       27, %r31
        mtspr       54, %r31        /* DECAR   */
        mtspr       58, %r31        /* CSRR0-1 */
        mtspr       59, %r31
        mtspr       61, %r31        /* DEAR    */
        mtspr       256, %r31       /* USPRG0  */
        mtspr       272, %r31       /* SPRG1-7 */
        mtspr       273, %r31
        mtspr       274, %r31
        mtspr       275, %r31
        mtspr       276, %r31
        mtspr       277, %r31
        mtspr       278, %r31
        mtspr       279, %r31
        mtspr       285, %r31       /* TBU     */
        mtspr       284, %r31       /* TBL     */
        mtspr       318, %r31       /* DVC1-2  */
        mtspr       319, %r31
        mtspr       562, %r31       /* DBCNT */
        mtspr       570, %r31       /* MCSRR0  */
        mtspr       571, %r31       /* MCSRR1  */
        mtspr       604, %r31       /* SPRG8-9 */
        mtspr       605, %r31

        /* MSR initialization.*/
        lis         %r3, MSR_DEFAULT@h
        ori         %r3, %r3, MSR_DEFAULT@l
        mtMSR       %r3

        /*
         * IVPR initialization.
         */
        lis         %r3, __ivpr_base__@h
        ori         %r3, %r3, __ivpr_base__@l
        mtIVPR      %r3

        /*
         * IVORs initialization.
         */
        lis         %r3, _unhandled_exception@h
        ori         %r3, %r3, _unhandled_exception@l
        mtspr       400, %r3    /* IVOR0-15 */
        mtspr       401, %r3
        mtspr       402, %r3
        mtspr       403, %r3
        mtspr       404, %r3
        mtspr       405, %r3
        mtspr       406, %r3
        mtspr       407, %r3
        mtspr       408, %r3
        mtspr       409, %r3
        mtspr       410, %r3
        mtspr       411, %r3
        mtspr       412, %r3
        mtspr       413, %r3
        mtspr       414, %r3
        mtspr       415, %r3
        mtspr       528, %r3    /* IVOR32-34 */
        mtspr       529, %r3
        mtspr       530, %r3

        /*
         * TLB0 allocated to flash.
         */
        lis         %r3, TLB0_MAS0@h
        mtspr       624, %r3         /* MAS0 */
        lis         %r3, TLB0_MAS1@h
        ori         %r3, %r3, TLB0_MAS1@l
        mtspr       625, %r3         /* MAS1 */
        lis         %r3, TLB0_MAS2@h
        ori         %r3, %r3, TLB0_MAS2@l
        mtspr       626, %r3         /* MAS2 */
        lis         %r3, TLB0_MAS3@h
        ori         %r3, %r3, TLB0_MAS3@l
        mtspr       627, %r3         /* MAS3 */
        tlbwe

        /*
         * TLB1 allocated to external RAM, if any.
         */
        lis         %r3, TLB1_MAS0@h
        mtspr       624, %r3         /* MAS0 */
        lis         %r3, TLB1_MAS1@h
        ori         %r3, %r3, TLB1_MAS1@l
        mtspr       625, %r3         /* MAS1 */
        lis         %r3, TLB1_MAS2@h
        ori         %r3, %r3, TLB1_MAS2@l
        mtspr       626, %r3         /* MAS2 */
        lis         %r3, TLB1_MAS3@h
        ori         %r3, %r3, TLB1_MAS3@l
        mtspr       627, %r3         /* MAS3 */
        tlbwe

        /*
         * TLB2 allocated to internal RAM.
         */
        lis         %r3, TLB2_MAS0@h
        mtspr       624, %r3         /* MAS0 */
        lis         %r3, TLB2_MAS1@h
        ori         %r3, %r3, TLB2_MAS1@l
        mtspr       625, %r3         /* MAS1 */
        lis         %r3, TLB2_MAS2@h
        ori         %r3, %r3, TLB2_MAS2@l
        mtspr       626, %r3         /* MAS2 */
        lis         %r3, TLB2_MAS3@h
        ori         %r3, %r3, TLB2_MAS3@l
        mtspr       627, %r3         /* MAS3 */
        tlbwe

        /*
         * TLB3 allocated to internal Peripherals Bridge A.
         */
        lis         %r3, TLB3_MAS0@h
        mtspr       624, %r3         /* MAS0 */
        lis         %r3, TLB3_MAS1@h
        ori         %r3, %r3, TLB3_MAS1@l
        mtspr       625, %r3         /* MAS1 */
        lis         %r3, TLB3_MAS2@h
        ori         %r3, %r3, TLB3_MAS2@l
        mtspr       626, %r3         /* MAS2 */
        lis         %r3, TLB3_MAS3@h
        ori         %r3, %r3, TLB3_MAS3@l
        mtspr       627, %r3         /* MAS3 */
        tlbwe

        /*
         * TLB4 allocated to internal Peripherals Bridge B.
         */
        lis         %r3, TLB4_MAS0@h
        mtspr       624, %r3         /* MAS0 */
        lis         %r3, TLB4_MAS1@h
        ori         %r3, %r3, TLB4_MAS1@l
        mtspr       625, %r3         /* MAS1 */
        lis         %r3, TLB4_MAS2@h
        ori         %r3, %r3, TLB4_MAS2@l
        mtspr       626, %r3         /* MAS2 */
        lis         %r3, TLB4_MAS3@h
        ori         %r3, %r3, TLB4_MAS3@l
        mtspr       627, %r3         /* MAS3 */
        tlbwe

        blr

#endif /* !defined(__DOXYGEN__) */

/** @} */
