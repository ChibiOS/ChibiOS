/*
    ChibiOS/RT - Copyright (C) 2006,2007,2008,2009,2010,2011 Giovanni Di Sirio.

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

#include "ch.h"
#include "hal.h"

/* This inclusion allows user ISR to be added to the HAL.*/
#if defined(_USER_ISR_)
#include "user_isr.h"
#endif

/**
 * @brief   Exception handler type.
 */
typedef void @far @interrupt (*interrupt_handler_t)(void);

/*
 * Various external symbols.
 */
void _stext(void);
@far @interrupt void vector_trap(void);
@far @interrupt void vector0(void);
@far @interrupt void vector1(void);
@far @interrupt void vector2(void);
@far @interrupt void vector3(void);
@far @interrupt void vector4(void);
@far @interrupt void vector5(void);
@far @interrupt void vector6(void);
@far @interrupt void vector7(void);
@far @interrupt void vector8(void);
@far @interrupt void vector9(void);
@far @interrupt void vector10(void);
@far @interrupt void vector11(void);
@far @interrupt void vector12(void);
@far @interrupt void vector13(void);
@far @interrupt void vector14(void);
@far @interrupt void vector15(void);
@far @interrupt void vector16(void);
@far @interrupt void vector17(void);
@far @interrupt void vector18(void);
@far @interrupt void vector19(void);
@far @interrupt void vector20(void);
@far @interrupt void vector21(void);
@far @interrupt void vector22(void);
@far @interrupt void vector23(void);
@far @interrupt void vector24(void);
@far @interrupt void vector25(void);
@far @interrupt void vector26(void);
@far @interrupt void vector27(void);
@far @interrupt void vector28(void);
@far @interrupt void vector29(void);

/**
 * @brief   Exception vector type.
 */
typedef struct {
  uint8_t               ev_instruction;
  interrupt_handler_t   ev_handler;
} exception_vector_t;

/**
 * @brief   Unhandled exception handler.
 * @default This function is the default handler for all unused entries
 *          in the vector table.
 */
@far @interrupt void _unhandled_exception (void)
{
  while (TRUE)
    ;
}

/**
 * @brief   Exceptions table.
 */
exception_vector_t const _vectab[] = {
  {0x82, (interrupt_handler_t)_stext},        /* reset */

#if defined(_TRAP_ISR)
  {0x82, vector_trap},
#else
  {0x82, _unhandled_exception},               /* trap */
#endif

#if defined(_TLI_ISR)
  {0x82, vector0},
#else
  {0x82, _unhandled_exception},               /* vector0 */
#endif

#if defined(_FLASH_ISR)
  {0x82, vector1},
#else
  {0x82, _unhandled_exception},               /* vector1 */
#endif

#if defined(_DMA10_ISR) || defined(_DMA11_ISR)
  {0x82, vector2},
#else
  {0x82, _unhandled_exception},               /* vector2 */
#endif

#if defined(_DMA12_ISR) || defined(_DMA13_ISR)
  {0x82, vector3},
#else
  {0x82, _unhandled_exception},               /* vector3 */
#endif

#if defined(_RTC_ISR) || defined(_LSE_CSS_ISR)
  {0x82, vector4},
#else
  {0x82, _unhandled_exception},               /* vector4 */
#endif

#if defined(_EXTIE_ISR) || defined(_EXTIF_ISR) || defined(_PVD_ISR)
  {0x82, vector5},
#else
  {0x82, _unhandled_exception},               /* vector5 */
#endif

#if defined(_EXTIB_ISR) || defined(_EXTIG_ISR)
  {0x82, vector6},
#else
  {0x82, _unhandled_exception},               /* vector6 */
#endif

#if defined(_EXTID_ISR) || defined(_EXTIH_ISR)
  {0x82, vector7},
#else
  {0x82, _unhandled_exception},               /* vector7 */
#endif

#if defined(_EXTI0_ISR)
  {0x82, vector8},
#else
  {0x82, _unhandled_exception},               /* vector8 */
#endif

#if defined(_EXTI1_ISR)
  {0x82, vector9},
#else
  {0x82, _unhandled_exception},               /* vector9 */
#endif

#if defined(_EXTI2_ISR)
  {0x82, vector10},
#else
  {0x82, _unhandled_exception},               /* vector10 */
#endif

#if defined(_EXTI3_ISR)
  {0x82, vector11},
#else
  {0x82, _unhandled_exception},               /* vector11 */
#endif

#if defined(_EXTI4_ISR)
  {0x82, vector12},
#else
  {0x82, _unhandled_exception},               /* vector12 */
#endif

#if defined(_EXTI5_ISR)
  {0x82, vector13},
#else
  {0x82, _unhandled_exception},               /* vector13 */
#endif

#if defined(_EXTI6_ISR)
  {0x82, vector14},
#else
  {0x82, _unhandled_exception},               /* vector14 */
#endif

#if defined(_EXTI7_ISR)
  {0x82, vector15},
#else
  {0x82, _unhandled_exception},               /* vector15 */
#endif

#if defined(_LCD_ISR) || defined(_AES_ISR)
  {0x82, vector16},
#else
  {0x82, _unhandled_exception},               /* vector16 */
#endif

#if defined(_CLK_ISR) || defined(_TIM1_BREAK_ISR) || defined(_DAC_ISR)
  {0x82, vector17},
#else
  {0x82, _unhandled_exception},               /* vector17 */
#endif

#if defined(_COMP1_ISR) || defined(_COMP2_ISR) || defined(_ADC1_ISR)
  {0x82, vector18},
#else
  {0x82, _unhandled_exception},               /* vector18 */
#endif

#if defined(_TIM2_UPDATE_ISR) || defined(_USART2_TRANSMIT_ISR)
  {0x82, vector19},
#else
  {0x82, _unhandled_exception},               /* vector19 */
#endif

#if defined(_TIM2_COMPARE_ISR) || defined(_USART2_RECEIVE_ISR)
  {0x82, vector20},
#else
  {0x82, _unhandled_exception},               /* vector20 */
#endif

#if defined(_TIM3_UPDATE_ISR) || defined(_USART3_TRANSMIT_ISR)
  {0x82, vector21},
#else
  {0x82, _unhandled_exception},               /* vector21 */
#endif

#if defined(_TIM3_COMPARE_ISR) || defined(_USART3_RECEIVE_ISR)
  {0x82, vector22},
#else
  {0x82, _unhandled_exception},               /* vector22 */
#endif

#if defined(_TIM1_UPDATE_ISR)
  {0x82, vector23},
#else
  {0x82, _unhandled_exception},               /* vector23 */
#endif

#if defined(_TIM1_COMPARE_ISR)
  {0x82, vector24},
#else
  {0x82, _unhandled_exception},               /* vector24 */
#endif

#if defined(_TIM4_UPDATE_ISR)
  {0x82, vector25},
#else
  {0x82, _unhandled_exception},               /* vector25 */
#endif

#if defined(_SPI1_ISR)
  {0x82, vector26},
#else
  {0x82, _unhandled_exception},               /* vector26 */
#endif

#if defined(_TIM5_UPDATE_ISR) || defined(_USART1_TRANSMIT_ISR)
  {0x82, vector27},
#else
  {0x82, _unhandled_exception},               /* vector27 */
#endif

#if defined(_TIM5_COMPARE_ISR) || defined(_USART1_RECEIVE_ISR)
  {0x82, vector28},
#else
  {0x82, _unhandled_exception},               /* vector28 */
#endif

#if defined(_SPI2_ISR) || defined(_I2C1_ISR)
  {0x82, vector29},
#else
  {0x82, _unhandled_exception},               /* vector29 */
#endif
};
