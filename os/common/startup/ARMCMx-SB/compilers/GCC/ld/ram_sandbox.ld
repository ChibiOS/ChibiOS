/*
    ChibiOS - Copyright (C) 2006..2018 Giovanni Di Sirio

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
    .text 0x00000000 : ALIGN(4)
    {
       KEEP(*(.sandbox))
       
        __text_base__ = .;
        *(.text)
        *(.text.*)
        *(.glue_7t)
        *(.glue_7)
        *(.gcc*)
        __text_end__ = .;
    }
    
    .rodata : ALIGN(4) ONLY_IF_RO
    {
        __rodata_base__ = .;
        *(.rodata)
        *(.rodata.*)
        __rodata_end__ = .;

        __init_array_base__ = .;
        KEEP(*(SORT(.init_array.*)))
        KEEP(*(.init_array))
        __init_array_end__ = .;
        __fini_array_base__ = .;
        KEEP(*(.fini_array))
        KEEP(*(SORT(.fini_array.*)))
        __fini_array_end__ = .;

        *(.eh_frame*)
        *(.ARM.extab* .gnu.linkonce.armextab.*)
    }

    .ARM.exidx : ALIGN(4)
    {
        __exidx_base__ = .;
        __exidx_start = .;
        *(.ARM.exidx* .gnu.linkonce.armexidx.*)
        __exidx_end__ = .;
        __exidx_end = .;
     }

    .data : ALIGN(4)
    {
        PROVIDE(_data = .);
       __data_base__ = .;
        *(.data)
        *(.data.*)
        . = ALIGN(4);
        PROVIDE(_edata = .);
        __data_end__ = .;
    }

    .bss (NOLOAD) : ALIGN(4)
    {
        __bss_base__ = .;
        *(.bss)
        *(.bss.*)
        *(COMMON)
        . = ALIGN(4);
        __bss_end__ = .;
        PROVIDE(end = .);
    }
    
    .stack (NOLOAD) : ALIGN(8)
    {
        . = ALIGN(8);
        __user_psp_base__ = .;
        . += __process_stack_size__;
        . = ALIGN(8);
        __user_psp_end__ = .;
        __heap_base__ = .;
    }
}
