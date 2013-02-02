/*
    ChibiOS/RT - Copyright (C) 2006,2007,2008,2009,2010,
                 2011,2012,2013 Giovanni Di Sirio.

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

/**
 * @file    STM32F0xx/ext_lld_isr.c
 * @brief   STM32F0xx EXT subsystem low level driver ISR code.
 *
 * @addtogroup EXT
 * @{
 */

#include "ch.h"
#include "hal.h"

#if HAL_USE_EXT || defined(__DOXYGEN__)

#include "ext_lld_isr.h"

/*===========================================================================*/
/* Driver local definitions.                                                 */
/*===========================================================================*/

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local variables.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Driver interrupt handlers.                                                */
/*===========================================================================*/

/**
 * @brief   EXTI[0] interrupt handler.
 *
 * @isr
 */
CH_IRQ_HANDLER(EXTI0_1_IRQHandler) {
  uint32_t pr;

  CH_IRQ_PROLOGUE();

  pr = EXTI->PR & ((1 << 0) | (1 << 1));
  EXTI->PR = pr;
  if (pr & (1 << 0))
    EXTD1.config->channels[0].cb(&EXTD1, 0);
  if (pr & (1 << 1))
    EXTD1.config->channels[1].cb(&EXTD1, 1);

  CH_IRQ_EPILOGUE();
}

/**
 * @brief   EXTI[1] interrupt handler.
 *
 * @isr
 */
CH_IRQ_HANDLER(EXTI2_3_IRQHandler) {
  uint32_t pr;

  CH_IRQ_PROLOGUE();

  pr = EXTI->PR & ((1 << 2) | (1 << 3));
  EXTI->PR = pr;
  if (pr & (1 << 2))
    EXTD1.config->channels[2].cb(&EXTD1, 2);
  if (pr & (1 << 3))
    EXTD1.config->channels[3].cb(&EXTD1, 3);

  CH_IRQ_EPILOGUE();
}

/**
 * @brief   EXTI[4]...EXTI[15] interrupt handler.
 *
 * @isr
 */
CH_IRQ_HANDLER(EXTI4_15_IRQHandler) {
  uint32_t pr;

  CH_IRQ_PROLOGUE();

  pr = EXTI->PR & ((1 << 4)  | (1 << 5)  | (1 << 6)  | (1 << 7)  | (1 << 8)  |
                   (1 << 9)  | (1 << 10) | (1 << 11) | (1 << 12) | (1 << 13) |
                   (1 << 14) | (1 << 15));
  EXTI->PR = pr;
  if (pr & (1 << 4))
    EXTD1.config->channels[4].cb(&EXTD1, 4);
  if (pr & (1 << 5))
    EXTD1.config->channels[5].cb(&EXTD1, 5);
  if (pr & (1 << 6))
    EXTD1.config->channels[6].cb(&EXTD1, 6);
  if (pr & (1 << 7))
    EXTD1.config->channels[7].cb(&EXTD1, 7);
  if (pr & (1 << 8))
    EXTD1.config->channels[8].cb(&EXTD1, 8);
  if (pr & (1 << 9))
    EXTD1.config->channels[9].cb(&EXTD1, 9);
  if (pr & (1 << 10))
    EXTD1.config->channels[10].cb(&EXTD1, 10);
  if (pr & (1 << 11))
    EXTD1.config->channels[11].cb(&EXTD1, 11);
  if (pr & (1 << 12))
    EXTD1.config->channels[12].cb(&EXTD1, 12);
  if (pr & (1 << 13))
    EXTD1.config->channels[13].cb(&EXTD1, 13);
  if (pr & (1 << 14))
    EXTD1.config->channels[14].cb(&EXTD1, 14);
  if (pr & (1 << 15))
    EXTD1.config->channels[15].cb(&EXTD1, 15);

  CH_IRQ_EPILOGUE();
}

/**
 * @brief   EXTI[16] interrupt handler (PVD).
 *
 * @isr
 */
CH_IRQ_HANDLER(PVD_IRQHandler) {

  CH_IRQ_PROLOGUE();

  EXTI->PR = (1 << 16);
  EXTD1.config->channels[16].cb(&EXTD1, 16);

  CH_IRQ_EPILOGUE();
}

/**
 * @brief   EXTI[17] interrupt handler (RTC).
 *
 * @isr
 */
CH_IRQ_HANDLER(RTC_IRQHandler) {

  CH_IRQ_PROLOGUE();

  EXTI->PR = (1 << 17);
  EXTD1.config->channels[17].cb(&EXTD1, 17);

  CH_IRQ_EPILOGUE();
}

/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

/**
 * @brief   Enables EXTI IRQ sources.
 *
 * @notapi
 */
void ext_lld_exti_irq_enable(void) {

  nvicEnableVector(EXTI0_1_IRQn,
                   CORTEX_PRIORITY_MASK(STM32_EXT_EXTI0_1_IRQ_PRIORITY));
  nvicEnableVector(EXTI2_3_IRQn,
                   CORTEX_PRIORITY_MASK(STM32_EXT_EXTI2_3_IRQ_PRIORITY));
  nvicEnableVector(EXTI4_15_IRQn,
                   CORTEX_PRIORITY_MASK(STM32_EXT_EXTI4_15_IRQ_PRIORITY));
  nvicEnableVector(PVD_IRQn,
                   CORTEX_PRIORITY_MASK(STM32_EXT_EXTI16_IRQ_PRIORITY));
  nvicEnableVector(RTC_IRQn,
                   CORTEX_PRIORITY_MASK(STM32_EXT_EXTI17_IRQ_PRIORITY));
}

/**
 * @brief   Disables EXTI IRQ sources.
 *
 * @notapi
 */
void ext_lld_exti_irq_disable(void) {

  nvicDisableVector(EXTI0_1_IRQn);
  nvicDisableVector(EXTI2_3_IRQn);
  nvicDisableVector(EXTI4_15_IRQn);
  nvicDisableVector(PVD_IRQn);
  nvicDisableVector(RTC_IRQn);
}

#endif /* HAL_USE_EXT */

/** @} */
