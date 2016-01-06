/*
    ChibiOS - Copyright (C) 2006..2015 Giovanni Di Sirio

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
 * @file    STM32L4xx/ext_lld_isr.c
 * @brief   STM32L4xx EXT subsystem low level driver ISR code.
 *
 * @addtogroup EXT
 * @{
 */

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
OSAL_IRQ_HANDLER(Vector58) {
  uint32_t pr;

  OSAL_IRQ_PROLOGUE();

  pr = EXTI->PR1 & EXTI->IMR1 & (1 << 0);
  EXTI->PR1 = pr;
  if (pr & (1 << 0))
    EXTD1.config->channels[0].cb(&EXTD1, 0);

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

  pr = EXTI->PR1 & EXTI->IMR1 & (1 << 1);
  EXTI->PR1 = pr;
  if (pr & (1 << 1))
    EXTD1.config->channels[1].cb(&EXTD1, 1);

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

  pr = EXTI->PR1 & EXTI->IMR1 & (1 << 2);
  EXTI->PR1 = pr;
  if (pr & (1 << 2))
    EXTD1.config->channels[2].cb(&EXTD1, 2);

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

  pr = EXTI->PR1 & EXTI->IMR1 & (1 << 3);
  EXTI->PR1 = pr;
  if (pr & (1 << 3))
    EXTD1.config->channels[3].cb(&EXTD1, 3);

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

  pr = EXTI->PR1 & EXTI->IMR1 & (1 << 4);
  EXTI->PR1 = pr;
  if (pr & (1 << 4))
    EXTD1.config->channels[4].cb(&EXTD1, 4);

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

  pr = EXTI->PR1 & EXTI->IMR1 & ((1 << 5) | (1 << 6) | (1 << 7) | (1 << 8) |
                               (1 << 9));
  EXTI->PR1 = pr;
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

  pr = EXTI->PR1 & EXTI->IMR1 & ((1 << 10) | (1 << 11) | (1 << 12) | (1 << 13) |
                               (1 << 14) | (1 << 15));
  EXTI->PR1 = pr;
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

  OSAL_IRQ_EPILOGUE();
}

/**
 * @brief   EXTI[16/35/36/37/38] interrupt handler (PVD/PVM1/PVM2/PVM3/PVM4)
 *
 * @isr
 */
OSAL_IRQ_HANDLER(Vector44) {
  uint32_t pr;

  OSAL_IRQ_PROLOGUE();

  pr = EXTI->PR1 & EXTI->IMR1 & (1 << 16);
  EXTI->PR1 = pr;
  if (pr & (1 << 16))
    EXTD1.config->channels[16].cb(&EXTD1, 16);

  pr = EXTI->PR2 & EXTI->IMR2 & ( (1 << (35-32)) | (1 << (36-32)) |
                                  (1 << (37-32)) | (1 << (38-32)) );
  EXTI->PR2 = pr;
  if (pr & (1 << (35-32)))
    EXTD1.config->channels[35].cb(&EXTD1, 35);
  if (pr & (1 << (36-32)))
    EXTD1.config->channels[36].cb(&EXTD1, 36);
  if (pr & (1 << (37-32)))
    EXTD1.config->channels[37].cb(&EXTD1, 37);
  if (pr & (1 << (38-32)))
    EXTD1.config->channels[38].cb(&EXTD1, 38);

  OSAL_IRQ_EPILOGUE();
}

/**
 * @brief   EXTI[18] interrupt handler (RTC_ALARM).
 *
 * @isr
 */
OSAL_IRQ_HANDLER(VectorE4) {
  uint32_t pr;

  OSAL_IRQ_PROLOGUE();

  pr = EXTI->PR1 & EXTI->IMR1 & (1 << 18);
  EXTI->PR1 = pr;
  if (pr & (1 << 18))
    EXTD1.config->channels[18].cb(&EXTD1, 18);

  OSAL_IRQ_EPILOGUE();
}

/**
 * @brief   EXTI[19] interrupt handler (RTC_TAMP_STAMP).
 *
 * @isr
 */
OSAL_IRQ_HANDLER(Vector48) {
  uint32_t pr;

  OSAL_IRQ_PROLOGUE();

  pr = EXTI->PR1 & EXTI->IMR1 & (1 << 19);
  EXTI->PR1 = pr;
  if (pr & (1 << 19))
    EXTD1.config->channels[19].cb(&EXTD1, 19);

  OSAL_IRQ_EPILOGUE();
}

/**
 * @brief   EXTI[20] interrupt handler (RTC_WKUP).
 *
 * @isr
 */
OSAL_IRQ_HANDLER(Vector4C) {
  uint32_t pr;

  OSAL_IRQ_PROLOGUE();

  pr = EXTI->PR1 & EXTI->IMR1 & (1 << 20);
  EXTI->PR1 = pr;
  if (pr & (1 << 20))
    EXTD1.config->channels[20].cb(&EXTD1, 20);

  OSAL_IRQ_EPILOGUE();
}

/**
 * @brief   EXTI[21/22] interrupt handler (COMP1,COMP2).
 *
 * @isr
 */
OSAL_IRQ_HANDLER(Vector140) {
  uint32_t pr;

  OSAL_IRQ_PROLOGUE();

  pr = EXTI->PR1 & EXTI->IMR1 & ( (1 << 21) | ( 1 << 22 ) );
  EXTI->PR1 = pr;
  if (pr & (1 << 21))
    EXTD1.config->channels[21].cb(&EXTD1, 21);
  if (pr & (1 << 22))
    EXTD1.config->channels[22].cb(&EXTD1, 22);

  OSAL_IRQ_EPILOGUE();
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

  nvicEnableVector(EXTI0_IRQn, STM32_EXT_EXTI0_IRQ_PRIORITY);
  nvicEnableVector(EXTI1_IRQn, STM32_EXT_EXTI1_IRQ_PRIORITY);
  nvicEnableVector(EXTI2_IRQn, STM32_EXT_EXTI2_IRQ_PRIORITY);
  nvicEnableVector(EXTI3_IRQn, STM32_EXT_EXTI3_IRQ_PRIORITY);
  nvicEnableVector(EXTI4_IRQn, STM32_EXT_EXTI4_IRQ_PRIORITY);
  nvicEnableVector(EXTI9_5_IRQn, STM32_EXT_EXTI5_9_IRQ_PRIORITY);
  nvicEnableVector(EXTI15_10_IRQn, STM32_EXT_EXTI10_15_IRQ_PRIORITY);
  nvicEnableVector(PVD_PVM_IRQn, STM32_EXT_EXTI16_IRQ_PRIORITY);
  nvicEnableVector(RTC_Alarm_IRQn, STM32_EXT_EXTI18_IRQ_PRIORITY);
  nvicEnableVector(TAMP_STAMP_IRQn, STM32_EXT_EXTI19_IRQ_PRIORITY);
  nvicEnableVector(RTC_WKUP_IRQn, STM32_EXT_EXTI20_IRQ_PRIORITY);
  nvicEnableVector(COMP_IRQn, STM32_EXT_EXTI21_22_IRQ_PRIORITY);
}

/**
 * @brief   Disables EXTI IRQ sources.
 *
 * @notapi
 */
void ext_lld_exti_irq_disable(void) {

  nvicDisableVector(EXTI0_IRQn);
  nvicDisableVector(EXTI1_IRQn);
  nvicDisableVector(EXTI2_IRQn);
  nvicDisableVector(EXTI3_IRQn);
  nvicDisableVector(EXTI4_IRQn);
  nvicDisableVector(EXTI9_5_IRQn);
  nvicDisableVector(EXTI15_10_IRQn);
  nvicDisableVector(PVD_PVM_IRQn);
  nvicDisableVector(RTC_Alarm_IRQn);
  nvicDisableVector(TAMP_STAMP_IRQn);
  nvicDisableVector(RTC_WKUP_IRQn);
  nvicDisableVector(COMP_IRQn);
}

#endif /* HAL_USE_EXT */

/** @} */
