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

/**
 * @file    AVR/hal_lld.c
 * @brief   AVR HAL subsystem low level driver code.
 *
 * @addtogroup HAL
 * @{
 */

#include "ch.h"
#include "hal.h"

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local variables and types.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Driver interrupt handlers.                                                */
/*===========================================================================*/

/**
 * @brief   Timer0 interrupt handler.
 */
CH_IRQ_HANDLER(AVR_TIMER_VECT) {

  CH_IRQ_PROLOGUE();

  chSysLockFromIsr();
  chSysTimerHandlerI();
  chSysUnlockFromIsr();

  CH_IRQ_EPILOGUE();
}


/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

/**
 * @brief   Low level HAL driver initialization.
 *
 * @notapi
 */
void hal_lld_init(void) {

  /*
   * Timer 0 setup.
   */
#ifdef TCCR0A /* Timer has multiple output comparators                       */
  TCCR0A  = (1 << WGM01) | (0 << WGM00) |                /* CTC mode.        */
            (0 << COM0A1) | (0 << COM0A0) |              /* OC0A disabled.   */
            (0 << COM0B1) | (0 << COM0B0);               /* OC0B disabled.   */
  TCCR0B  = (0 << WGM02) | AVR_TIMER_PRESCALER_BITS;     /* CTC mode.        */
  OCR0A   = AVR_TIMER_COUNTER - 1;
  TCNT0   = 0;                                           /* Reset counter.   */
  TIFR0   = (1 << OCF0A);                                /* Reset pending.   */
  TIMSK0  = (1 << OCIE0A);                               /* IRQ on compare.  */

#elif defined(TCCR0) /* Timer has single output comparator                   */
  TCCR0   = (1 << WGM01) | (0 << WGM00) |                /* CTC mode.        */
            (0 << COM01) | (0 << COM00) |                /* OC0A disabled.   */
            AVR_TIMER_PRESCALER_BITS;    
  OCR0    = AVR_TIMER_COUNTER - 1;
  TCNT0   = 0;                                           /* Reset counter.   */
  TIFR    = (1 << OCF0);                                 /* Reset pending.   */
  TIMSK   = (1 << OCIE0);                                /* IRQ on compare.  */
#else
	#error "Neither TCCR0A nor TCCRO registers are defined"
#endif
}

/** @} */
