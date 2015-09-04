/*
    ChibiOS - Copyright (C) 2006..2015 Giovanni Di Sirio.

    This file is part of ChibiOS.

    ChibiOS is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 3 of the License, or
    (at your option) any later version.

    ChibiOS is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

/*
 * ST32F746xG memory setup.
 * Note: Use of ram1 and ram2 is mutually exclusive with use of ram0.
 */
MEMORY
{
    flash : org = 0x08000000, len = 1M
    ram0  : org = 0x20010000, len = 256k    /* SRAM1 + SRAM2 */
    ram1  : org = 0x20010000, len = 240k    /* SRAM1 */
    ram2  : org = 0x2004C000, len = 16k     /* SRAM2 */
    ram3  : org = 0x20000000, len = 64k     /* DTCM-RAM */
    ram4  : org = 0x00000000, len = 16k     /* ITCM-RAM */
    ram5  : org = 0x40024000, len = 4k      /* BCKP SRAM */
    ram6  : org = 0x00000000, len = 0
    ram7  : org = 0x00000000, len = 0
}

/* RAM region to be used for Main stack. This stack accommodates the processing
   of all exceptions and interrupts*/
REGION_ALIAS("MAIN_STACK_RAM", ram3);

/* RAM region to be used for the process stack. This is the stack used by
   the main() function.*/
REGION_ALIAS("PROCESS_STACK_RAM", ram3);

/* RAM region to be used for data segment.*/
REGION_ALIAS("DATA_RAM", ram1);

/* RAM region to be used for BSS segment.*/
REGION_ALIAS("BSS_RAM", ram1);

/* RAM region to be used for NOCACHE segment.*/
REGION_ALIAS("NOCACHE_RAM", ram3);

/* RAM region to be used for ETH segment.*/
REGION_ALIAS("ETH_RAM", ram2);

INCLUDE rules_dma.ld
