/*
    ChibiOS/RT - Copyright (C) 2006,2007,2008,2009,2010 Giovanni Di Sirio.

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

#include "ch.h"
#include "hal.h"

/**
 * @brief   Exception handler type.
 */
typedef void @far @interrupt (*interrupt_handler_t)(void);

/*
 * Various external symbols.
 */
void _stext(void);
@far @interrupt void vector13(void);
@far @interrupt void vector17(void);
@far @interrupt void vector18(void);
@far @interrupt void vector20(void);
@far @interrupt void vector21(void);

/**
 * @brief   Exception vector type.
 */
typedef struct {
	uint8_t               ev_instruction;
	interrupt_handler_t   ev_handler;
} exception_vector_t;

/**
 * @brief   Undefined interrupt handler.
 * @note    It should never be invoked.
 */
@far @interrupt static void vector (void)
{
	return;
}

/**
 * @brief   Exceptions table.
 */
exception_vector_t const _vectab[] = {
	{0x82, (interrupt_handler_t)_stext},        /* reset */
	{0x82, vector},                             /* trap  */
	{0x82, vector},                             /* vector0  */
	{0x82, vector},                             /* vector1  */
	{0x82, vector},                             /* vector2  */
	{0x82, vector},                             /* vector3  */
	{0x82, vector},                             /* vector4  */
	{0x82, vector},                             /* vector5  */
	{0x82, vector},                             /* vector6  */
	{0x82, vector},                             /* vector7  */
	{0x82, vector},                             /* vector8  */
	{0x82, vector},                             /* vector9  */
	{0x82, vector},                             /* vector10 */
	{0x82, vector},                             /* vector11 */
	{0x82, vector},                             /* vector12 */
	{0x82, vector13},                           /* vector13 */
	{0x82, vector},                             /* vector14 */
	{0x82, vector},                             /* vector15 */
	{0x82, vector},                             /* vector16 */
#if USE_STM8_UART1
	{0x82, vector17},                           /* vector17 */
	{0x82, vector18},                           /* vector18 */
#else
	{0x82, vector},                             /* vector17 */
	{0x82, vector},                             /* vector18 */
#endif
	{0x82, vector},                             /* vector19 */
#if USE_STM8_UART2 || USE_STM8_UART3
  {0x82, vector20},                           /* vector20 */
	{0x82, vector21},                           /* vector21 */
#else
  {0x82, vector},                             /* vector20 */
	{0x82, vector},                             /* vector21 */
#endif
	{0x82, vector},                             /* vector22 */
	{0x82, vector},                             /* vector23 */
	{0x82, vector},                             /* vector24 */
	{0x82, vector},                             /* vector25 */
	{0x82, vector},                             /* vector26 */
	{0x82, vector},                             /* vector27 */
	{0x82, vector},                             /* vector28 */
	{0x82, vector},                             /* vector29 */
};
