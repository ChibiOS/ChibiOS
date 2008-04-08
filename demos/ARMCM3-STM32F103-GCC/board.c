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

#include <ch.h>

#include "board.h"

/*
 * Hardware initialization goes here.
 * NOTE: Interrupts are still disabled.
 */
void hwinit(void) {

  /*
   * I/O ports initialization as specified in board.h.
   */
  GPIOA->CRL = VAL_GPIOACRL;
  GPIOA->CRH = VAL_GPIOACRH;
  GPIOA->ODR = VAL_GPIOAODR;

  GPIOB->CRL = VAL_GPIOBCRL;
  GPIOB->CRH = VAL_GPIOBCRH;
  GPIOB->ODR = VAL_GPIOBODR;

  GPIOC->CRL = VAL_GPIOCCRL;
  GPIOC->CRH = VAL_GPIOCCRH;
  GPIOC->ODR = VAL_GPIOCODR;

  GPIOD->CRL = VAL_GPIODCRL;
  GPIOD->CRH = VAL_GPIODCRH;
  GPIOD->ODR = VAL_GPIODODR;
}
