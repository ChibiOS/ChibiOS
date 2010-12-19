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
 * @brief   PAL setup.
 * @details Digital I/O ports static configuration as defined in @p board.h.
 */
#if HAL_USE_PAL || defined(__DOXYGEN__)
ROMCONST PALConfig pal_default_config =
{
  {
    {VAL_GPIOAODR, 0, VAL_GPIOADDR, VAL_GPIOACR1, VAL_GPIOACR2},
    {VAL_GPIOBODR, 0, VAL_GPIOBDDR, VAL_GPIOBCR1, VAL_GPIOBCR2},
    {VAL_GPIOCODR, 0, VAL_GPIOCDDR, VAL_GPIOCCR1, VAL_GPIOCCR2},
    {VAL_GPIODODR, 0, VAL_GPIODDDR, VAL_GPIODCR1, VAL_GPIODCR2},
    {VAL_GPIOEODR, 0, VAL_GPIOEDDR, VAL_GPIOECR1, VAL_GPIOECR2},
    {VAL_GPIOFODR, 0, VAL_GPIOFDDR, VAL_GPIOFCR1, VAL_GPIOFCR2},
    {VAL_GPIOGODR, 0, VAL_GPIOGDDR, VAL_GPIOGCR1, VAL_GPIOGCR2},
  }
};
#endif

/*
 * TIM 2 clock after the prescaler.
 */
#define TIM2_CLOCK  (SYSCLK / 16)
#define TIM2_ARR    ((TIM2_CLOCK / CH_FREQUENCY) - 1)

/*
 * TIM2 interrupt handler.
 */
CH_IRQ_HANDLER(13) {

  CH_IRQ_PROLOGUE();

  chSysLockFromIsr();
  chSysTimerHandlerI();
  chSysUnlockFromIsr();

  TIM2->SR1 = 0;

  CH_IRQ_EPILOGUE();
}

/*
 * Board-specific initialization code.
 */
void boardInit(void) {

  /*
   * TIM2 initialization as system tick.
   */
  CLK->PCKENR1 |= CLK_PCKENR1_TIM2;
  TIM2->PSCR    = 4;            /* Prescaler divide by 2^4=16.*/
  TIM2->ARRH    = (uint8_t)(TIM2_ARR >> 8);
  TIM2->ARRL    = (uint8_t)(TIM2_ARR);
  TIM2->CNTRH   = 0;
  TIM2->CNTRL   = 0;
  TIM2->SR1     = 0;
  TIM2->IER     = TIM2_IER_UIE;
  TIM2->CR1     = TIM2_CR1_CEN;
}
