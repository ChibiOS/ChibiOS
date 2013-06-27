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
 * @file    AVR/hal_lld.h
 * @brief   AVR HAL subsystem low level driver header.
 *
 * @addtogroup HAL
 * @{
 */

#ifndef _HAL_LLD_H_
#define _HAL_LLD_H_

/*===========================================================================*/
/* Driver constants.                                                         */
/*===========================================================================*/

/**
 * @brief   Defines the support for realtime counters in the HAL.
 */
#define HAL_IMPLEMENTS_COUNTERS FALSE

/**
 * @brief   Platform name.
 */
#define PLATFORM_NAME   "AVR"

/**
 * @brief  Timer maximum value 
 */
#define AVR_TIMER_COUNTER_MAX 255

/*===========================================================================*/
/* Driver pre-compile time settings.                                         */
/*===========================================================================*/

/* Work out what the timer interrupt is called on this MCU */
#ifdef TIMER0_COMPA_vect
  #define AVR_TIMER_VECT TIMER0_COMPA_vect
#elif defined(TIMER_COMPA_vect)
  #define AVR_TIMER_VECT TIMER_COMPA_vect
#elif defined(TIMER0_COMP_vect)
  #define AVR_TIMER_VECT TIMER0_COMP_vect
#else
  #error "Cannot find interrupt vector name for timer"
#endif

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

/* Find the most suitable prescaler setting for the desired CH_FREQUENCY */
#if ((F_CPU / CH_FREQUENCY) <= AVR_TIMER_COUNTER_MAX)
  #define AVR_TIMER_PRESCALER 1
  #define AVR_TIMER_PRESCALER_BITS (0 << CS02)  | (0 << CS01)  | (1 << CS00); /* CLK      */
#elif ((F_CPU / CH_FREQUENCY / 8) <= AVR_TIMER_COUNTER_MAX)
  #define AVR_TIMER_PRESCALER 8
  #define AVR_TIMER_PRESCALER_BITS (0 << CS02)  | (1 << CS01)  | (0 << CS00); /* CLK/8    */
#elif ((F_CPU / CH_FREQUENCY / 64) <= AVR_TIMER_COUNTER_MAX)
  #define AVR_TIMER_PRESCALER 64
  #define AVR_TIMER_PRESCALER_BITS (0 << CS02)  | (1 << CS01)  | (1 << CS00); /* CLK/64   */
#elif ((F_CPU / CH_FREQUENCY / 256) <= AVR_TIMER_COUNTER_MAX)
  #define AVR_TIMER_PRESCALER 256
  #define AVR_TIMER_PRESCALER_BITS (1 << CS02)  | (0 << CS01)  | (0 << CS00); /* CLK/256  */
#elif ((F_CPU / CH_FREQUENCY / 1024) <= AVR_TIMER_COUNTER_MAX)
  #define AVR_TIMER_PRESCALER 1024
  #define AVR_TIMER_PRESCALER_BITS (1 << CS02)  | (0 << CS01)  | (1 << CS00); /* CLK/1024 */
#else
  #error "Frequency too low for timer, please set CH_FREQUENCY to a higher value"
#endif

#define AVR_TIMER_COUNTER (F_CPU / CH_FREQUENCY / AVR_TIMER_PRESCALER)

/* Test if CH_FREQUENCY can be matched exactly using this timer */
#define F_CPU_ (AVR_TIMER_COUNTER * AVR_TIMER_PRESCALER * CH_FREQUENCY)
#if (F_CPU_ != F_CPU)
  #warning "CH_FREQUENCY cannot be generated exactly using timer"
#endif
#undef F_CPU_

/*===========================================================================*/
/* Driver data structures and types.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Driver macros.                                                            */
/*===========================================================================*/

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#ifdef __cplusplus
extern "C" {
#endif
  void hal_lld_init(void);
#ifdef __cplusplus
}
#endif

#endif /* _HAL_LLD_H_ */

/** @} */
