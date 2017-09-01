/*
    ChibiOS - Copyright (C) 2006..2016 Giovanni Di Sirio

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
 * @file    STM32L4xx/stm32_isr.h
 * @brief   STM32L4xx ISR handler code.
 *
 * @addtogroup SRM32L4xx_ISR
 * @{
 */

#include "hal.h"

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

#if HAL_USE_EXT
#define exti_serve_irq(pr, channel) {                                       \
                                                                            \
  if ((pr) & (1U << (channel))) {                                           \
    EXTD1.config->channels[channel].cb(&EXTD1, channel);                    \
  }                                                                         \
}
#elif HAL_USE_PAL
#define exti_serve_irq(pr, channel) {                                       \
                                                                            \
  if ((pr) & (1U << (channel))) {                                           \
    _pal_isr_code(channel);                                                 \
  }                                                                         \
}
#endif

/*===========================================================================*/
/* Driver interrupt handlers.                                                */
/*===========================================================================*/

#if HAL_USE_PAL || HAL_USE_EXT || defined(__DOXYGEN__)
/**
 * @brief   EXTI[0] interrupt handler.
 *
 * @isr
 */
OSAL_IRQ_HANDLER(Vector58) {
  uint32_t pr;

  OSAL_IRQ_PROLOGUE();

  pr = EXTI->PR;
  pr &= EXTI->IMR & (1U << 0);
  EXTI->PR = pr;

  exti_serve_irq(pr, 0);

  OSAL_IRQ_EPILOGUE();
}

/**
 * @brief   EXTI[1] interrupt handler.
 *
 * @isr
 */
OSAL_IRQ_HANDLER(Vector5C) {
  uint32_t pr;

  OSAL_IRQ_PROLOGUE();

  pr = EXTI->PR;
  pr &= EXTI->IMR & (1U << 1);
  EXTI->PR = pr;

  exti_serve_irq(pr, 1);

  OSAL_IRQ_EPILOGUE();
}

/**
 * @brief   EXTI[2] interrupt handler.
 *
 * @isr
 */
OSAL_IRQ_HANDLER(Vector60) {
  uint32_t pr;

  OSAL_IRQ_PROLOGUE();

  pr = EXTI->PR;
  pr &= EXTI->IMR & (1U << 2);
  EXTI->PR = pr;

  exti_serve_irq(pr, 2);

  OSAL_IRQ_EPILOGUE();
}

/**
 * @brief   EXTI[3] interrupt handler.
 *
 * @isr
 */
OSAL_IRQ_HANDLER(Vector64) {
  uint32_t pr;

  OSAL_IRQ_PROLOGUE();

  pr = EXTI->PR;
  pr &= EXTI->IMR & (1U << 3);
  EXTI->PR = pr;

  exti_serve_irq(pr, 3);

  OSAL_IRQ_EPILOGUE();
}

/**
 * @brief   EXTI[4] interrupt handler.
 *
 * @isr
 */
OSAL_IRQ_HANDLER(Vector68) {
  uint32_t pr;

  OSAL_IRQ_PROLOGUE();

  pr = EXTI->PR;
  pr &= EXTI->IMR & (1U << 4);
  EXTI->PR = pr;

  exti_serve_irq(pr, 4);

  OSAL_IRQ_EPILOGUE();
}

/**
 * @brief   EXTI[5]...EXTI[9] interrupt handler.
 *
 * @isr
 */
OSAL_IRQ_HANDLER(Vector9C) {
  uint32_t pr;

  OSAL_IRQ_PROLOGUE();

  pr = EXTI->PR;
  pr &= EXTI->IMR & ((1U << 5) | (1U << 6) | (1U << 7) | (1U << 8) |
                     (1U << 9));
  EXTI->PR = pr;

  exti_serve_irq(pr, 5);
  exti_serve_irq(pr, 6);
  exti_serve_irq(pr, 7);
  exti_serve_irq(pr, 8);
  exti_serve_irq(pr, 9);

  OSAL_IRQ_EPILOGUE();
}

/**
 * @brief   EXTI[10]...EXTI[15] interrupt handler.
 *
 * @isr
 */
OSAL_IRQ_HANDLER(VectorE0) {
  uint32_t pr;

  OSAL_IRQ_PROLOGUE();

  pr = EXTI->PR;
  pr &= EXTI->IMR & ((1U << 10) | (1U << 11) | (1U << 12) | (1U << 13) |
                     (1U << 14) | (1U << 15));
  EXTI->PR = pr;

  exti_serve_irq(pr, 10);
  exti_serve_irq(pr, 11);
  exti_serve_irq(pr, 12);
  exti_serve_irq(pr, 13);
  exti_serve_irq(pr, 14);
  exti_serve_irq(pr, 15);

  OSAL_IRQ_EPILOGUE();
}

#if HAL_USE_EXT || defined(__DOXYGEN__)
#if !defined(STM32_DISABLE_EXTI16_HANDLER)
/**
 * @brief   EXTI[16] interrupt handler (PVD).
 *
 * @isr
 */
OSAL_IRQ_HANDLER(Vector44) {
  uint32_t pr;

  OSAL_IRQ_PROLOGUE();

  pr = EXTI->PR;
  pr &= EXTI->IMR & (1U << 16);
  EXTI->PR = pr;
  if (pr & (1U << 16))
    EXTD1.config->channels[16].cb(&EXTD1, 16);

  OSAL_IRQ_EPILOGUE();
}
#endif

#if !defined(STM32_DISABLE_EXTI17_HANDLER)
/**
 * @brief   EXTI[17] interrupt handler (RTC Alarm).
 *
 * @isr
 */
OSAL_IRQ_HANDLER(VectorE4) {
  uint32_t pr;

  OSAL_IRQ_PROLOGUE();

  pr = EXTI->PR;
  pr &= EXTI->IMR & (1U << 17);
  EXTI->PR = pr;
  if (pr & (1U << 17))
    EXTD1.config->channels[17].cb(&EXTD1, 17);

  OSAL_IRQ_EPILOGUE();
}
#endif

#if !defined(STM32_DISABLE_EXTI18_HANDLER) && STM32_HAS_USB
/**
 * @brief   EXTI[18] interrupt handler (USB Wakeup).
 *
 * @isr
 */
OSAL_IRQ_HANDLER(VectorE8) {
  uint32_t pr;

  OSAL_IRQ_PROLOGUE();

  pr = EXTI->PR;
  pr &= EXTI->IMR & (1U << 18);
  EXTI->PR = pr;
  if (pr & (1U << 18))
    EXTD1.config->channels[18].cb(&EXTD1, 18);

  OSAL_IRQ_EPILOGUE();
}
#endif

#if !defined(STM32_DISABLE_EXTI19_HANDLER)
/**
 * @brief   EXTI[19] interrupt handler (Tamper TimeStamp).
 *
 * @isr
 */
OSAL_IRQ_HANDLER(Vector48) {
  uint32_t pr;

  OSAL_IRQ_PROLOGUE();

  pr = EXTI->PR;
  pr &= EXTI->IMR & (1U << 19);
  EXTI->PR = pr;
  if (pr & (1U << 19))
    EXTD1.config->channels[19].cb(&EXTD1, 19);

  OSAL_IRQ_EPILOGUE();
}
#endif

#if !defined(STM32_DISABLE_EXTI20_HANDLER)
/**
 * @brief   EXTI[20] interrupt handler (RTC Wakeup).
 *
 * @isr
 */
OSAL_IRQ_HANDLER(Vector4C) {
  uint32_t pr;

  OSAL_IRQ_PROLOGUE();

  pr = EXTI->PR;
  pr &= EXTI->IMR & (1U << 20);
  EXTI->PR = pr;
  if (pr & (1U << 20))
    EXTD1.config->channels[20].cb(&EXTD1, 20);

  OSAL_IRQ_EPILOGUE();
}
#endif

#if !defined(STM32_DISABLE_EXTI21_22_29_HANDLER)
/**
 * @brief   EXTI[21],EXTI[22],EXTI[29] interrupt handler (COMP1, COMP2, COMP3).
 *
 * @isr
 */
OSAL_IRQ_HANDLER(Vector140) {
  uint32_t pr;

  OSAL_IRQ_PROLOGUE();

  pr = EXTI->PR;
  pr &= EXTI->IMR & ((1U << 21) | (1U << 22) | (1U << 29));
  EXTI->PR = pr;
  if (pr & (1U << 21))
    EXTD1.config->channels[21].cb(&EXTD1, 21);
  if (pr & (1U << 22))
    EXTD1.config->channels[22].cb(&EXTD1, 22);
  if (pr & (1U << 29))
    EXTD1.config->channels[29].cb(&EXTD1, 29);

  OSAL_IRQ_EPILOGUE();
}
#endif

#if !defined(STM32_DISABLE_EXTI30_32_HANDLER)
/**
 * @brief   EXTI[30]...EXTI[32] interrupt handler (COMP4, COMP5, COMP6).
 *
 * @isr
 */
OSAL_IRQ_HANDLER(Vector144) {
  uint32_t pr;

  OSAL_IRQ_PROLOGUE();

  pr = EXTI->PR;
  pr &= EXTI->IMR & ((1U << 30) | (1U << 31));
  EXTI->PR = pr;
  if (pr & (1U << 30))
    EXTD1.config->channels[30].cb(&EXTD1, 30);
  if (pr & (1U << 31))
    EXTD1.config->channels[31].cb(&EXTD1, 31);

  pr = EXTI->PR2 & EXTI->IMR2 & (1U << 0);
  EXTI->PR2 = pr;
  if (pr & (1U << 0))
    EXTD1.config->channels[32].cb(&EXTD1, 32);

  OSAL_IRQ_EPILOGUE();
}
#endif

#if !defined(STM32_DISABLE_EXTI33_HANDLER)
/**
 * @brief   EXTI[33] interrupt handler (COMP7).
 *
 * @isr
 */
OSAL_IRQ_HANDLER(Vector148) {
  uint32_t pr2;

  OSAL_IRQ_PROLOGUE();

  pr2 = EXTI->PR2;
  pr2 = EXTI->IMR & (1U << 1);
  EXTI->PR2 = pr2;
  if (pr2 & (1U << 1))
    EXTD1.config->channels[33].cb(&EXTD1, 33);

  OSAL_IRQ_EPILOGUE();
}
#endif
#endif /* HAL_USE_EXT */

#endif /* HAL_USE_PAL || HAL_USE_EXT */

/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

/**
 * @brief   Enables IRQ sources.
 *
 * @notapi
 */
void irqInit(void) {

#if HAL_USE_PAL || HAL_USE_EXT
  nvicEnableVector(EXTI0_IRQn, STM32_IRQ_EXTI0_PRIORITY);
  nvicEnableVector(EXTI1_IRQn, STM32_IRQ_EXTI1_PRIORITY);
  nvicEnableVector(EXTI2_TSC_IRQn, STM32_IRQ_EXTI2_PRIORITY);
  nvicEnableVector(EXTI3_IRQn, STM32_IRQ_EXTI3_PRIORITY);
  nvicEnableVector(EXTI4_IRQn, STM32_IRQ_EXTI4_PRIORITY);
  nvicEnableVector(EXTI9_5_IRQn, STM32_IRQ_EXTI5_9_PRIORITY);
  nvicEnableVector(EXTI15_10_IRQn, STM32_IRQ_EXTI10_15_PRIORITY);
#if HAL_USE_EXT
  nvicEnableVector(PVD_PVM_IRQn, STM32_IRQ_EXTI1635_38_PRIORITY);
  nvicEnableVector(RTC_Alarm_IRQn, STM32_IRQ_EXTI18_PRIORITY);
  nvicEnableVector(TAMP_STAMP_IRQn, STM32_IRQ_EXTI19_PRIORITY);
  nvicEnableVector(RTC_WKUP_IRQn, STM32_IRQ_EXTI20_PRIORITY);
  nvicEnableVector(COMP_IRQn, STM32_IRQ_EXTI21_22_PRIORITY);
#endif
#endif
}

/**
 * @brief   Disables IRQ sources.
 *
 * @notapi
 */
void irqDeinit(void) {

#if HAL_USE_PAL || HAL_USE_EXT
  nvicDisableVector(EXTI0_IRQn);
  nvicDisableVector(EXTI1_IRQn);
  nvicDisableVector(EXTI2_TSC_IRQn);
  nvicDisableVector(EXTI3_IRQn);
  nvicDisableVector(EXTI4_IRQn);
  nvicDisableVector(EXTI9_5_IRQn);
  nvicDisableVector(EXTI15_10_IRQn);
#if HAL_USE_EXT
  nvicEnableVector(PVD_IRQn, STM32_IRQ_EXTI16_PRIORITY);
  nvicEnableVector(RTC_Alarm_IRQn, STM32_IRQ_EXTI17_PRIORITY);
#if STM32_HAS_USB
  nvicEnableVector(USBWakeUp_IRQn, STM32_IRQ_EXTI18_PRIORITY);
#endif
  nvicEnableVector(TAMP_STAMP_IRQn, STM32_IRQ_EXTI19_PRIORITY);
  nvicEnableVector(RTC_WKUP_IRQn, STM32_IRQ_EXTI20_PRIORITY);
  nvicEnableVector(COMP1_2_3_IRQn, STM32_IRQ_EXTI21_22_29_PRIORITY);
  nvicEnableVector(COMP4_5_6_IRQn, STM32_IRQ_EXTI30_32_PRIORITY);
#if STM32_EXTI_NUM_LINES >= 34
  nvicEnableVector(COMP7_IRQn, STM32_IRQ_EXTI33_PRIORITY);
#endif
#endif
#endif
}

/** @} */
