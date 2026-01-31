/*
    ChibiOS - Copyright (C) 2006-2026 Giovanni Di Sirio.

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
 * RAM sandbox memory setup.
 */

ENTRY(__crt0_entry)

SECTIONS
{
    /* Not elegant but a single section named ".text" makes it easier to debug
       a file that has been loaded and relocated, see the documentation of the
       GDB command "add-symbol-file".*/
    .text 0x00000000 : ALIGN(4)
    {
        /* Making sure startup segment is first.*/
        KEEP(*(.sandbox))

        /* Text segment.*/
        __text_base__ = .;
        *(.text)
        *(.text.*)
        *(.glue_7t)
        *(.glue_7)
        *(.gcc*)
        __text_end__ = .;

        /* Read-only data segment.*/
        . = ALIGN(4);
       __rodata_base__ = .;
        *(.rodata)
        *(.rodata.*)
        __rodata_end__ = .;

        /* constructors and destructors segment.*/
        __init_array_base__ = .;
        KEEP(*(SORT(.init_array.*)))
        KEEP(*(.init_array))
        __init_array_end__ = .;
        __fini_array_base__ = .;
        KEEP(*(.fini_array))
        KEEP(*(SORT(.fini_array.*)))
        __fini_array_end__ = .;

        /* ARM-specific segments.*/
        *(.eh_frame*)
        *(.ARM.extab* .gnu.linkonce.armextab.*)

        . = ALIGN(4);
         __exidx_base__ = .;
        __exidx_start = .;
        *(.ARM.exidx* .gnu.linkonce.armexidx.*)
        __exidx_end__ = .;
        __exidx_end = .;

        /* Data segment.*/
        . = ALIGN(4);
         PROVIDE(_data = .);
       __data_base__ = .;
        *(.data)
        *(.data.*)
        . = ALIGN(4);
        PROVIDE(_edata = .);
        __data_end__ = .;

        /* BSS segment.*/
        . = ALIGN(4);
         __bss_base__ = .;
        *(.bss)
        *(.bss.*)
        *(COMMON)
        . = ALIGN(4);
        __bss_end__ = .;
        PROVIDE(end = .);

        /* Stack segment.*/
        . = ALIGN(8);
        __user_psp_base__ = .;
        . += __process_stack_size__;
        . = ALIGN(8);
        __user_psp_end__ = .;

        /* Reserved heap segment.*/
        . = ALIGN(4);
        __heap_base__ = .;
        __heap_rsvd_base__ = .;
        *(.heap_rsvd)
        __heap_rsvd_end__ = .;
    }
}
