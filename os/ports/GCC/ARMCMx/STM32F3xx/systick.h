/*
    Nil RTOS - Copyright (C) 2012 Giovanni Di Sirio.

    This file is part of Nil RTOS.

    Nil RTOS is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 3 of the License, or
    (at your option) any later version.

    Nil RTOS is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

/**
 * @file    STM32F3xx/niltimer.h
 * @brief   System timer header file.
 *
 * @addtogroup STM32F3_TIMER
 * @{
 */

#ifndef _CHTIMER_H_
#define _CHTIMER_H_

/*===========================================================================*/
/* Module constants.                                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Module pre-compile time settings.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

/*===========================================================================*/
/* Module data structures and types.                                         */
/*===========================================================================*/

typedef struct {
  volatile uint32_t     CR1;
  volatile uint32_t     CR2;
  volatile uint32_t     SMCR;
  volatile uint32_t     DIER;
  volatile uint32_t     SR;
  volatile uint32_t     EGR;
  volatile uint32_t     CCMR1;
  volatile uint32_t     CCMR2;
  volatile uint32_t     CCER;
  volatile uint32_t     CNT;
  volatile uint32_t     PSC;
  volatile uint32_t     ARR;
  volatile uint32_t     RCR;
  volatile uint32_t     CCR[4];
  volatile uint32_t     BDTR;
  volatile uint32_t     DCR;
  volatile uint32_t     DMAR;
  volatile uint32_t     OR;
  volatile uint32_t     CCMR3;
  volatile uint32_t     CCR5;
  volatile uint32_t     CCR6;
} local_stm32_tim_t;

/*===========================================================================*/
/* Module macros.                                                            */
/*===========================================================================*/

#define STM32F3_TIM2    ((local_stm32_tim_t *)0x40000000)

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

/*===========================================================================*/
/* Module inline functions.                                                  */
/*===========================================================================*/

/**
 * @brief   Timer unit initialization.
 *
 * @notapi
 */
static inline void port_timer_init(void) {

  STM32F3_TIM2->ARR     = 0xFFFFFFFF;
  STM32F3_TIM2->CCMR1   = 0;
  STM32F3_TIM2->CCR[0]  = 0;
  STM32F3_TIM2->DIER    = 0;
  STM32F3_TIM2->CR2     = 0;
  STM32F3_TIM2->EGR     = 1;            /* UG, CNT initialized.             */
  STM32F3_TIM2->CR1     = 1;            /* CEN */
}

/**
 * @brief   Returns the system time.
 *
 * @return              The system time.
 *
 * @notapi
 */
static inline systime_t port_timer_get_time(void) {

  return STM32F3_TIM2->CNT;
}

/**
 * @brief   Starts the alarm.
 * @note    Makes sure that no spurious alarms are triggered after
 *          this call.
 *
 * @param[in] time      the time to be set for the first alarm
 *
 * @notapi
 */
static inline void port_timer_start_alarm(systime_t time) {

  chDbgAssert((STM32F3_TIM2->DIER & 2) == 0,
              "port_timer_start_alarm(), #1",
              "already started");

  STM32F3_TIM2->CCR[0]  = time;
  STM32F3_TIM2->SR      = 0;
  STM32F3_TIM2->DIER    = 2;            /* CC1IE */
}

/**
 * @brief   Stops the alarm interrupt.
 *
 * @notapi
 */
static inline void port_timer_stop_alarm(void) {

  chDbgAssert((STM32F3_TIM2->DIER & 2) != 0,
              "port_timer_stop_alarm(), #1",
              "not started");

  STM32F3_TIM2->DIER    = 0;
}

/**
 * @brief   Sets the alarm time.
 *
 * @param[in] time      the time to be set for the next alarm
 *
 * @notapi
 */
static inline void port_timer_set_alarm(systime_t time) {

  chDbgAssert((STM32F3_TIM2->DIER & 2) != 0,
              "port_timer_set_alarm(), #1",
              "not started");

  STM32F3_TIM2->CCR[0]  = time;
}

/**
 * @brief   Returns the current alarm time.
 *
 * @return              The currently set alarm time.
 *
 * @notapi
 */
static inline systime_t port_timer_get_alarm(void) {

  chDbgAssert((STM32F3_TIM2->DIER & 2) != 0,
              "port_timer_get_alarm(), #1",
              "not started");

  return STM32F3_TIM2->CCR[0];
}

#endif /* _CHTIMER_H_ */

/** @} */
