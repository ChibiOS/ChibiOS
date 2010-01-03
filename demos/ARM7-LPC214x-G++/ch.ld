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

/*
 * LPC2148 memory setup.
 */
__und_stack_size__	= 0x0004;
__abt_stack_size__	= 0x0004;
__fiq_stack_size__	= 0x0010;
__irq_stack_size__	= 0x0080;
__svc_stack_size__	= 0x0004;
__sys_stack_size__	= 0x0800;
__stacks_total_size__	= __und_stack_size__ + __abt_stack_size__ + __fiq_stack_size__ + __irq_stack_size__ + __svc_stack_size__ + __sys_stack_size__;

MEMORY
{
    flash : org = 0x00000000, len = 512k - 12k
    ram : org = 0x40000200, len = 32k - 0x200 - 288
}

__ram_start__       = ORIGIN(ram);
__ram_size__        = LENGTH(ram);
__ram_end__         = __ram_start__ + __ram_size__;
__dma_start__       = 0x7FD00000;
__dma_size__        = 8k;
__dma_end__         = 0x7FD00000 + __dma_size__;

SECTIONS
{
	. = 0;

	.text : ALIGN(16) SUBALIGN(16)
	{
        _text = .;
        KEEP(*(vectors))
        *(.text)
        *(.text.*);
        *(.rodata);
        *(.rodata.*);
        *(.glue_7t);
        *(.glue_7);
        *(.gcc*);
        *(.ctors);
        *(.dtors);
        . = ALIGN(4);
        _etext = .;
    } > flash

    _textdata = _etext;

    .data :
    {
        _data = .;
        *(.data)
        . = ALIGN(4);
        *(.data.*)
        . = ALIGN(4);
        *(.ramtext)
        . = ALIGN(4);
        _edata = .;
    } > ram AT > flash

    .bss :
    {
        _bss_start = .;
        *(.bss)
        . = ALIGN(4);
        *(.bss.*)
        . = ALIGN(4);
        *(COMMON)
        . = ALIGN(4);
        _bss_end = .;
    } > ram    
    
    /DISCARD/ :
    {
        *(.eh_*)
    }
}

PROVIDE(end = .);
_end =			.;

__heap_base__		= _end;
__heap_end__		= __ram_end__ - __stacks_total_size__;
