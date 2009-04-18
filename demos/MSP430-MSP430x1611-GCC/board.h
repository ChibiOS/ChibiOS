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

#ifndef _BOARD_H_
#define _BOARD_H_

#include <msp430x16x.h>

/*
 * Clock settings.
 */
//#define MSP_USE_XT2CLK
#define MSP_USE_DCOCLK

#if defined(MSP_USE_XT2CLK) && defined(MSP_USE_DCOCLK)
#error "Define MSP_USE_XT2CLK or MSP_USE_DCOCLK, not both"
#endif

#define LFXT1CLK        32768
#define XT2CLK          8000000
#define DCOCLK          750000

#define ACLK            LFXT1CLK
#if defined(MSP_USE_XT2CLK)
#define MCLK            XT2CLK
#define SMCLK           (XT2CLK / 8)
#elif defined(MSP_USE_DCOCLK)
#define MCLK            DCOCLK
#define SMCLK           DCOCLK
#else
#error "Default clock source not selected"
#endif

#define VAL_DCOCTL      (DCO0 | DCO1)
#if defined(MSP_USE_XT2CLK)
#define VAL_BCSCTL1     (RSEL2)
#define VAL_BCSCTL2     (SELM_2 | DIVM_0 | DIVS_3 | SELS)
#endif
#if defined(MSP_USE_DCOCLK)
#define VAL_BCSCTL1     (XT2OFF | RSEL2)
#define VAL_BCSCTL2     (SELM_0 | DIVM_0 | DIVS_0)
#endif

/*
 * Pin definitionsfor the Olimex MSP430-P1611 board.
 */
#define P3_O_TXD0       (1 << 4)
#define P3_I_RXD0       (1 << 5)
#define P6_O_LED        (1 << 0)
#define P6_I_BUTTON     (1 << 1)

/*
 * Initial I/O ports settings.
 */
#define VAL_P1OUT       0x00
#define VAL_P1DIR       0xFF
#define VAL_P1SEL       0x00

#define VAL_P2OUT       0x00
#define VAL_P2DIR       0xFF
#define VAL_P2SEL       0x00

#define VAL_P3OUT       P3_O_TXD0
#define VAL_P3DIR       ~P3_I_RXD0
#define VAL_P3SEL       0x00

#define VAL_P4OUT       0x00
#define VAL_P4DIR       0xFF
#define VAL_P4SEL       0x00

#define VAL_P5OUT       0x00
#define VAL_P5DIR       0xFF
#define VAL_P5SEL       0x00

#define VAL_P6OUT       P6_O_LED
#define VAL_P6DIR       ~P6_I_BUTTON
#define VAL_P6SEL       0x00

void hwinit(void);

#endif /* _BOARD_H_ */
