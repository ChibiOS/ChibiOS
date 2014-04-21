/*
    ChibiOS/RT - Copyright (C) 2006-2013 Giovanni Di Sirio

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

        http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.
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
