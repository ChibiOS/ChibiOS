/*
    ChibiOS/RT - Copyright (C) 2006,2007,2008,2009,2010,
                 2011 Giovanni Di Sirio.

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
 * @file    STM32/ext_lld.c
 * @brief   STM32 EXT subsystem low level driver source.
 *
 * @addtogroup EXT
 * @{
 */

#include "ch.h"
#include "hal.h"

#if HAL_USE_EXT || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver local definitions.                                                 */
/*===========================================================================*/

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

/**
 * @brief   EXTD1 driver identifier.
 */
EXTDriver EXTD1;

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
CH_IRQ_HANDLER(EXTI0_IRQHandler) {

  CH_IRQ_PROLOGUE();

  EXTI->PR = (1 << 0);
  EXTD1.config->channels[0].cb(&EXTD1, 0);

  CH_IRQ_EPILOGUE();
}

/**
 * @brief   EXTI[1] interrupt handler.
 *
 * @isr
 */
CH_IRQ_HANDLER(EXTI1_IRQHandler) {

  CH_IRQ_PROLOGUE();

  EXTI->PR = (1 << 1);
  EXTD1.config->channels[1].cb(&EXTD1, 1);

  CH_IRQ_EPILOGUE();
}

/**
 * @brief   EXTI[2] interrupt handler.
 *
 * @isr
 */
CH_IRQ_HANDLER(EXTI2_IRQHandler) {

  CH_IRQ_PROLOGUE();

  EXTI->PR = (1 << 2);
  EXTD1.config->channels[2].cb(&EXTD1, 2);

  CH_IRQ_EPILOGUE();
}

/**
 * @brief   EXTI[3] interrupt handler.
 *
 * @isr
 */
CH_IRQ_HANDLER(EXTI3_IRQHandler) {

  CH_IRQ_PROLOGUE();

  EXTI->PR = (1 << 3);
  EXTD1.config->channels[3].cb(&EXTD1, 3);

  CH_IRQ_EPILOGUE();
}

/**
 * @brief   EXTI[4] interrupt handler.
 *
 * @isr
 */
CH_IRQ_HANDLER(EXTI4_IRQHandler) {

  CH_IRQ_PROLOGUE();

  EXTI->PR = (1 << 4);
  EXTD1.config->channels[4].cb(&EXTD1, 4);

  CH_IRQ_EPILOGUE();
}

/**
 * @brief   EXTI[5]...EXTI[9] interrupt handler.
 *
 * @isr
 */
CH_IRQ_HANDLER(EXTI9_5_IRQHandler) {
  uint32_t pr;

  CH_IRQ_PROLOGUE();

  pr = EXTI->PR;
  EXTI->PR = pr;
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

  CH_IRQ_EPILOGUE();
}

/**
 * @brief   EXTI[10]...EXTI[15] interrupt handler.
 *
 * @isr
 */
CH_IRQ_HANDLER(EXTI15_10_IRQHandler) {
  uint32_t pr;

  CH_IRQ_PROLOGUE();

  pr = EXTI->PR;
  EXTI->PR = pr;
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
CH_IRQ_HANDLER(RTCAlarm_IRQHandler) {

  CH_IRQ_PROLOGUE();

  EXTI->PR = (1 << 17);
  EXTD1.config->channels[17].cb(&EXTD1, 17);

  CH_IRQ_EPILOGUE();
}

#if STM32_HAS_USB || defined(__DOXYGEN__)
/**
 * @brief   EXTI[18] interrupt handler (USB).
 *
 * @isr
 */
CH_IRQ_HANDLER(USBWakeUp_IRQHandler) {

  CH_IRQ_PROLOGUE();

  EXTI->PR = (1 << 18);
  EXTD1.config->channels[18].cb(&EXTD1, 18);

  CH_IRQ_EPILOGUE();
}
#endif /* STM32_HAS_USB */

#if STM32_HAS_OTG1 || defined(__DOXYGEN__)
/**
 * @brief   EXTI[18] interrupt handler (OTG1).
 *
 * @isr
 */
CH_IRQ_HANDLER(OTG_FS_WKUP_IRQHandler) {

  CH_IRQ_PROLOGUE();

  EXTI->PR = (1 << 18);
  EXTD1.config->channels[18].cb(&EXTD1, 18);

  CH_IRQ_EPILOGUE();
}
#endif /* STM32_HAS_OTG1 */

#if STM32_HAS_ETH || defined(__DOXYGEN__)
/**
 * @brief   EXTI[19] interrupt handler (ETH).
 *
 * @isr
 */
CH_IRQ_HANDLER(ETH_WKUP_IRQHandler) {

  CH_IRQ_PROLOGUE();

  EXTI->PR = (1 << 19);
  EXTD1.config->channels[19].cb(&EXTD1, 19);

  CH_IRQ_EPILOGUE();
}
#endif /* STM32_HAS_ETH */

/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

/**
 * @brief   Low level EXT driver initialization.
 *
 * @notapi
 */
void ext_lld_init(void) {

  /* Driver initialization.*/
  extObjectInit(&EXTD1);
}

/**
 * @brief   Configures and activates the EXT peripheral.
 *
 * @param[in] extp      pointer to the @p EXTDriver object
 *
 * @notapi
 */
void ext_lld_start(EXTDriver *extp) {
  unsigned i;
  uint32_t imr, emr, rtsr, ftsr;

  if (extp->state == EXT_STOP) {
    /* Clock activation.*/
    NVICEnableVector(EXTI0_IRQn,
                     CORTEX_PRIORITY_MASK(STM32_EXT_EXTI0_IRQ_PRIORITY));
    NVICEnableVector(EXTI1_IRQn,
                     CORTEX_PRIORITY_MASK(STM32_EXT_EXTI1_IRQ_PRIORITY));
    NVICEnableVector(EXTI2_IRQn,
                     CORTEX_PRIORITY_MASK(STM32_EXT_EXTI2_IRQ_PRIORITY));
    NVICEnableVector(EXTI3_IRQn,
                     CORTEX_PRIORITY_MASK(STM32_EXT_EXTI3_IRQ_PRIORITY));
    NVICEnableVector(EXTI4_IRQn,
                     CORTEX_PRIORITY_MASK(STM32_EXT_EXTI4_IRQ_PRIORITY));
    NVICEnableVector(EXTI9_5_IRQn,
                     CORTEX_PRIORITY_MASK(STM32_EXT_EXTI5_9_IRQ_PRIORITY));
    NVICEnableVector(EXTI15_10_IRQn,
                     CORTEX_PRIORITY_MASK(STM32_EXT_EXTI10_15_IRQ_PRIORITY));
    NVICEnableVector(PVD_IRQn,
                     CORTEX_PRIORITY_MASK(STM32_EXT_EXTI16_IRQ_PRIORITY));
    NVICEnableVector(RTCAlarm_IRQn,
                     CORTEX_PRIORITY_MASK(STM32_EXT_EXTI17_IRQ_PRIORITY));
#if STM32_HAS_USB
    NVICEnableVector(USBWakeUp_IRQn,
                     CORTEX_PRIORITY_MASK(STM32_EXT_EXTI18_IRQ_PRIORITY));
#endif
#if STM32_HAS_OTG1
    NVICEnableVector(OTG_FS_WKUP_IRQn,
                     CORTEX_PRIORITY_MASK(STM32_EXT_EXTI18_IRQ_PRIORITY));
#endif
#if STM32_HAS_ETH
    NVICEnableVector(ETH_WKUP_IRQn,
                     CORTEX_PRIORITY_MASK(STM32_EXT_EXTI19_IRQ_PRIORITY));
#endif
  }
  /* Configuration.*/
  imr = emr = rtsr = ftsr = 0;
  for (i = 0; i < EXT_MAX_CHANNELS; i++) {
    if (extp->config->channels[i].mode & EXT_CH_MODE_AUTOSTART) {
      if (extp->config->channels[i].cb != NULL)
        imr |= (1 << i);
      else
        emr |= (1 << i);
      if (extp->config->channels[i].mode & EXT_CH_MODE_RISING_EDGE)
        rtsr |= (1 << i);
      if (extp->config->channels[i].mode & EXT_CH_MODE_FALLING_EDGE)
        ftsr |= (1 << i);
    }
  }
  AFIO->EXTICR[0] = extp->config->exti[0];
  AFIO->EXTICR[1] = extp->config->exti[1];
  AFIO->EXTICR[2] = extp->config->exti[2];
  AFIO->EXTICR[3] = extp->config->exti[3];
  EXTI->SWIER = 0;
  EXTI->RTSR  = rtsr;
  EXTI->FTSR  = ftsr;
  EXTI->PR    = EXT_CHANNELS_MASK;
  EXTI->EMR   = emr;
  EXTI->IMR   = imr;
}

/**
 * @brief   Deactivates the EXT peripheral.
 *
 * @param[in] extp      pointer to the @p EXTDriver object
 *
 * @notapi
 */
void ext_lld_stop(EXTDriver *extp) {

  if (extp->state == EXT_ACTIVE) {
    NVICDisableVector(EXTI0_IRQn);
    NVICDisableVector(EXTI1_IRQn);
    NVICDisableVector(EXTI2_IRQn);
    NVICDisableVector(EXTI3_IRQn);
    NVICDisableVector(EXTI4_IRQn);
    NVICDisableVector(EXTI9_5_IRQn);
    NVICDisableVector(EXTI15_10_IRQn);
    NVICDisableVector(PVD_IRQn);
    NVICDisableVector(RTCAlarm_IRQn);
#if STM32_HAS_USB
    NVICDisableVector(USBWakeUp_IRQn);
#endif
#if STM32_HAS_OTG1
    NVICDisableVector(OTG_FS_WKUP_IRQn);
#endif
#if STM32_HAS_ETH
    NVICDisableVector(ETH_WKUP_IRQn);
#endif
  }
  EXTI->EMR = 0;
  EXTI->IMR = 0;
  EXTI->PR = EXT_CHANNELS_MASK;
}

/**
 * @brief   Enables an EXT channel.
 *
 * @param[in] extp      pointer to the @p EXTDriver object
 * @param[in] channel   channel to be enabled
 *
 * @notapi
 */
void ext_lld_channel_enable(EXTDriver *extp, expchannel_t channel) {

  if (extp->config->channels[channel].cb != NULL)
    EXTI->IMR |= (1 << channel);
  else
    EXTI->EMR |= (1 << channel);
  if (extp->config->channels[channel].mode & EXT_CH_MODE_RISING_EDGE)
    EXTI->RTSR |= (1 << channel);
  if (extp->config->channels[channel].mode & EXT_CH_MODE_FALLING_EDGE)
    EXTI->FTSR |= (1 << channel);
}

/**
 * @brief   Disables an EXT channel.
 *
 * @param[in] extp      pointer to the @p EXTDriver object
 * @param[in] channel   channel to be disabled
 *
 * @notapi
 */
void ext_lld_channel_disable(EXTDriver *extp, expchannel_t channel) {

  (void)extp;

  EXTI->IMR  &= ~(1 << channel);
  EXTI->EMR  &= ~(1 << channel);
  EXTI->RTSR &= ~(1 << channel);
  EXTI->FTSR &= ~(1 << channel);
  EXTI->PR    = (1 << channel);
}

#endif /* HAL_USE_EXT */

/** @} */
