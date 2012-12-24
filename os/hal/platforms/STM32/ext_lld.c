/*
    ChibiOS/RT - Copyright (C) 2006,2007,2008,2009,2010,
                 2011,2012 Giovanni Di Sirio.

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

#include "ext_lld_isr.h"

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

  if (extp->state == EXT_STOP)
    ext_lld_exti_irq_enable();

  /* Configuration of automatic channels.*/
  for (i = 0; i < EXT_MAX_CHANNELS; i++)
    if (extp->config->channels[i].mode & EXT_CH_MODE_AUTOSTART)
      ext_lld_channel_enable(extp, i);
    else
      ext_lld_channel_disable(extp, i);
}

/**
 * @brief   Deactivates the EXT peripheral.
 *
 * @param[in] extp      pointer to the @p EXTDriver object
 *
 * @notapi
 */
void ext_lld_stop(EXTDriver *extp) {

  if (extp->state == EXT_ACTIVE)
    ext_lld_exti_irq_disable();

  EXTI->EMR = 0;
  EXTI->IMR = 0;
  EXTI->PR  = 0xFFFFFFFF;
#if STM32_EXTI_NUM_CHANNELS > 32
  EXTI->PR2 = 0xFFFFFFFF;
#endif
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

#if STM32_EXTI_NUM_CHANNELS > 32
  if (channel < 32) {
#endif
    /* Programming edge registers.*/
    if (extp->config->channels[channel].mode & EXT_CH_MODE_RISING_EDGE)
      EXTI->RTSR |= (1 << channel);
    else
      EXTI->RTSR &= ~(1 << channel);
    if (extp->config->channels[channel].mode & EXT_CH_MODE_FALLING_EDGE)
      EXTI->FTSR |= (1 << channel);
    else
      EXTI->FTSR &= ~(1 << channel);

    /* Programming interrupt and event registers.*/
    if (extp->config->channels[channel].cb != NULL) {
      EXTI->IMR |= (1 << channel);
      EXTI->EMR &= ~(1 << channel);
    }
    else {
      EXTI->EMR |= (1 << channel);
      EXTI->IMR &= ~(1 << channel);
    }
#if STM32_EXTI_NUM_CHANNELS > 32
  }
  else {
    /* Programming edge registers.*/
    if (extp->config->channels[channel].mode & EXT_CH_MODE_RISING_EDGE)
      EXTI->RTSR2 |= (1 << (32 - channel));
    else
      EXTI->RTSR2 &= ~(1 << (32 - channel));
    if (extp->config->channels[channel].mode & EXT_CH_MODE_FALLING_EDGE)
      EXTI->FTSR2 |= (1 << (32 - channel));
    else
      EXTI->FTSR2 &= ~(1 << (32 - channel));

    /* Programming interrupt and event registers.*/
    if (extp->config->channels[channel].cb != NULL) {
      EXTI->IMR2 |= (1 << (32 - channel));
      EXTI->EMR2 &= ~(1 << (32 - channel));
    }
    else {
      EXTI->EMR2 |= (1 << (32 - channel));
      EXTI->IMR2 &= ~(1 << (32 - channel));
    }
  }
#endif

  /* Setting the associated GPIO for external channels.*/
  if (channel < 16) {
    uint32_t n = channel >> 2;
    uint32_t mask = ~(0xF << ((channel & 3) * 4));
    uint32_t port = ((extp->config->channels[channel].mode &
                      EXT_MODE_GPIO_MASK) >>
                     EXT_MODE_GPIO_OFF) << ((channel & 3) * 4);

#if defined(STM32L1XX_MD) || defined(STM32F0XX) || defined(STM32F2XX) ||    \
    defined(STM32F30X) || defined(STM32F4XX)
  SYSCFG->EXTICR[n] = (SYSCFG->EXTICR[n] & mask) | port;
#else /* STM32F1XX */
  AFIO->EXTICR[n] = (AFIO->EXTICR[n] & mask) | port;
#endif /* STM32F1XX */
  }
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

#if STM32_EXTI_NUM_CHANNELS > 32
  if (channel < 32) {
#endif
    EXTI->IMR   &= ~(1 << channel);
    EXTI->EMR   &= ~(1 << channel);
    EXTI->RTSR  &= ~(1 << channel);
    EXTI->FTSR  &= ~(1 << channel);
    EXTI->PR     =  (1 << channel);
#if STM32_EXTI_NUM_CHANNELS > 32
  }
  else {
    EXTI->IMR2  &= ~(1 << (32 - channel));
    EXTI->EMR2  &= ~(1 << (32 - channel));
    EXTI->RTSR2 &= ~(1 << (32 - channel));
    EXTI->FTSR2 &= ~(1 << (32 - channel));
    EXTI->PR2    =  (1 << (32 - channel));
  }
#endif
}

#endif /* HAL_USE_EXT */

/** @} */
