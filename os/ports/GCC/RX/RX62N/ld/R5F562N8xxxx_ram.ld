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

                                      ---

    A special exception to the GPL can be applied should you wish to distribute
    a combined work that includes ChibiOS/RT, without being obliged to provide
    the source code for any proprietary components. See the file exception.txt
    for full details of how and when the exception can be applied.
*/

/*
 * R5F562N8xxxx memory setup (code executed from RAM)
 */
__user_stack_size__     = 0x0; /* not used */
__irq_stack_size__      = 0x0400;

MEMORY
{
    ramjumps : org = 0x00000000, len = 0x20
    flash :    org = 0x00000020, len = 64k-0x20
    ram :      org = 0x00010000, len = 32k
    fvectors : org = 0xFFFFFF80, len = 0x80
}

__ram_start__           = ORIGIN(ram);
__ram_size__            = LENGTH(ram);
__ram_end__             = __ram_start__ + __ram_size__;
__flash_start__         = ORIGIN(flash);
__flash_size__          = LENGTH(flash);

SECTIONS
{
    . = 0;
    _text = .;

    ramjumps : ALIGN(4)
    {
        KEEP(*(.ramjumps))
    } > ramjumps

    rvectors : ALIGN(4)
    {
        KEEP(*(.rvectors))
    } > flash

    constructors : ALIGN(4) SUBALIGN(4)
    {
        PROVIDE(__init_array_start = .);
        KEEP(*(SORT(.init_array.*)))
        KEEP(*(.init_array))
        PROVIDE(__init_array_end = .);
    } > flash

    destructors : ALIGN(4) SUBALIGN(4)
    {
        PROVIDE(__fini_array_start = .);
        KEEP(*(.fini_array))
        KEEP(*(SORT(.fini_array.*)))
        PROVIDE(__fini_array_end = .);
    } > flash

    .text : ALIGN(4)
    {
/*        KEEP(*(.text.startup.*))*/
        *(.text P .stub .text.* .gnu.linkonce.t.*)
        *(.rodata C C_2 C_1 .rodata.* .gnu.linkonce.r.*)
        *(.rodata1)
    } > flash
/*
    .eh_frame_hdr :
    {
        *(.eh_frame_hdr)
    } > flash

    .eh_frame : ONLY_IF_RO
    {
        *(.eh_frame)
    } > flash

    .textalign : ONLY_IF_RO
    {
        . = ALIGN(8);
    } > flash
*/
    . = ALIGN(4);
    _etext = .;
    _textdata = _etext;

    .noinit (NOLOAD) :
    {
    } > ram

    .stacks :
    {
        . = ALIGN(4);
        __user_stack_base__ = .;
        . += __user_stack_size__;
        . = ALIGN(4);
        __user_stack_end__ = .;
        __irq_stack_base__ = .;
        __main_thread_stack_base__ = .;
        . += __irq_stack_size__;
        . = ALIGN(4);
        __main_thread_stack_end__ = .;
        PROVIDE(__irq_stack_end__ = .);
    } > ram

    .data :
    {
        . = ALIGN(4);
        PROVIDE(_data = .);
        *(.data D .data.* .gnu.linkonce.d.*)
        . = ALIGN(4);
        *(.data1)
        . = ALIGN(4);
        *(.ramtext)
        . = ALIGN(4);
        *(.sdata .sdata.* .gnu.linkonce.s.* D_2 D_1)
        . = ALIGN(4);
        PROVIDE(_edata = .);
/*    } > flash*/
    } > ram AT > flash

    .bss :
    {
        . = ALIGN(4);
        PROVIDE(_bss_start = .);
        *(.sbss .sbss.*)
        *(.bss B B_2 B_1 .bss.* .gnu.linkonce.b.*)
        . = ALIGN(4);
        *(COMMON)
        . = ALIGN(4);

        . = ALIGN(32);
        *(.etherdesc)
        PROVIDE(_bss_end = .);
    } > ram

    PROVIDE(end = .);
    _end             = .;

    ___heap_base__   = _end;
    ___heap_end__    = __ram_end__;

    fvectors : ALIGN(4)
    {
        KEEP(*(.fvectors))
    } > fvectors

}
