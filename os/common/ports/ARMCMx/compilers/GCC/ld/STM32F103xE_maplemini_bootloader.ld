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
 * ST32F103xE memory setup for use with the maplemini bootloader.
 * You will have to
 * 	#define CORTEX_VTOR_INIT 0x5000
 * in your projects chconf.h
 */
MEMORY
{
    flash : org = 0x08005000, len = 512k - 0x5000
    ram : org = 0x20000C00, len = 64k - 0xC00
}

INCLUDE rules.ld
