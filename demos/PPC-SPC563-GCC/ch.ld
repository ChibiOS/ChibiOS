/*
    ChibiOS/RT - Copyright (C) 2006,2007,2008,2009,2010 Giovanni Di Sirio.

    This file is part of ChibiOS/RT.

    ChibiOS/RT is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 3 of the License, or
    (at your option) any later version.

    ChibiOS/RT is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program. If not, see <http://www.gnu.org/licenses/>.

                                      ---

    A special exception to the GPL can be applied should you wish to distribute
    a combined work that includes ChibiOS/RT, without being obliged to provide
    the source code for any proprietary components. See the file exception.txt
    for full details of how and when the exception can be applied.
*/

/*
 * SPC563M64 memory setup.
 */
__irq_stack_size__      = 0x0400;
__process_stack_size__  = 0x0800;
__stacks_total_size__   = __irq_stack_size__ + __process_stack_size__;

MEMORY
{
    flash : org = 0x00000000, len = 1536k
    ram   : org = 0x40000000, len = 94k
}

/*
 * Derived constants.
 */
__flash_size__          = LENGTH(flash);
__flash_start__         = ORIGIN(flash);
__flash_end__           = ORIGIN(flash) + LENGTH(flash);

__ram_size__            = LENGTH(ram);
__ram_start__           = ORIGIN(ram);
__ram_end__             = ORIGIN(ram) + LENGTH(ram);

SECTIONS
{
    . = ORIGIN(flash);

    .text : ALIGN(16) SUBALIGN(16)
    {
        __ivpr_base__ = .;
        KEEP(*(.bam))
        KEEP(*(.handlers))
        . = ALIGN(0x800);
        KEEP(*(.vectors))
        *(.text .stub .text.* .gnu.linkonce.t.*)
        *(.glue_7t);
        *(.glue_7);
        *(.ctors);
        *(.dtors);
    } > flash

    .rodata : ALIGN(16) SUBALIGN(16)
    {
        *(.rodata .rodata.* .gnu.linkonce.r.*)
        *(.rodata1)
    } > flash
    
    .sdata2 : ALIGN(16) SUBALIGN(16)
    {
        __sdata2_start__ = . + 0x8000;
        *(.sdata2 .sdata2.* .gnu.linkonce.s2.*)
        *(.sbss2 .sbss2.* .gnu.linkonce.sb2.*)
    } > flash
    
    .romdata : ALIGN(16) SUBALIGN(16)
    {
        __romdata_start__ = .;
    } > flash

    .data : AT(__romdata_start__)
    {
        . = ALIGN(4);
        __data_start__ = .;
        *(.data .data.* .gnu.linkonce.d.*)
        __sdata_start__ = . + 0x8000;
        *(.sdata .sdata.* .gnu.linkonce.s.*)
        __data_end__ = .;
    } > ram

    .sbss :
    {
        __bss_start__ = .;
        *(.sbss .sbss.* .gnu.linkonce.sb.*)
        *(.scommon)
    } > ram

    .bss :
    {
        *(.bss .bss.* .gnu.linkonce.b.*)
        *(COMMON)
        __bss_end__ = .;
    } > ram
    
    /DISCARD/ :
    {
        *(.eh_*)
    }

    __heap_base__   = __bss_end__;
    __heap_end__    = __ram_end__ - __stacks_total_size__;
}
