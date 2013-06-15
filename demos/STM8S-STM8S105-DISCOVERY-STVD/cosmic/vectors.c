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
@far @interrupt void vector10(void);
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
#if HAL_USE_SPI && STM8S_SPI_USE_SPI
  {0x82, vector10},
#else
  {0x82, vector},                             /* vector10 */
#endif
  {0x82, vector},                             /* vector11 */
  {0x82, vector},                             /* vector12 */
  {0x82, vector13},                           /* vector13 */
  {0x82, vector},                             /* vector14 */
  {0x82, vector},                             /* vector15 */
  {0x82, vector},                             /* vector16 */
#if HAL_USE_SERIAL && STM8S_SERIAL_USE_UART1
  {0x82, vector17},                           /* vector17 */
  {0x82, vector18},                           /* vector18 */
#else
  {0x82, vector},                             /* vector17 */
  {0x82, vector},                             /* vector18 */
#endif
  {0x82, vector},                             /* vector19 */
#if HAL_USE_SERIAL && (STM8S_SERIAL_USE_UART2 || STM8S_SERIAL_USE_UART3)
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
