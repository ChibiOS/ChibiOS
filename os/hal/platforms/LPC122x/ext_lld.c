/*
    ChibiOS/RT - Copyright (C) 2006-2013 Giovanni Di Sirio
    LPC122x EXT driver - Copyright (C) 2013 Marcin Jokel

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
 * @file    LPC122x/ext_lld.c
 * @brief   LPC122x EXT subsystem low level driver source.
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
 * @brief   EXTD0 driver identifier.
 */
#if LPC122x_EXT_USE_EXT0 || defined(__DOXYGEN__)
EXTDriver EXTD0;
#endif

/**
 * @brief   EXTD1 driver identifier.
 */
#if LPC122x_EXT_USE_EXT1 || defined(__DOXYGEN__)
EXTDriver EXTD1;
#endif

/**
 * @brief   EXTD2 driver identifier.
 */
#if LPC122x_EXT_USE_EXT2 || defined(__DOXYGEN__)
EXTDriver EXTD2;
#endif

/*===========================================================================*/
/* Driver local variables and types.                                         */
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
#if LPC122x_EXT_USE_EXT0
  extObjectInit(&EXTD0);
  EXTD0.gpio = LPC_GPIO0;
#endif

#if LPC122x_EXT_USE_EXT1
  extObjectInit(&EXTD1);
  EXTD1.gpio = LPC_GPIO1;
#endif

#if LPC122x_EXT_USE_EXT2
  extObjectInit(&EXTD2);
  EXTD2.gpio = LPC_GPIO2;
#endif
}

/**
 * @brief   Configures and activates the EXT peripheral.
 *
 * @param[in] extp      pointer to the @p EXTDriver object
 *
 * @notapi
 */
void ext_lld_start(EXTDriver *extp) {
  int i;

  /* Configure all pins as edge sensitive */
#if LPC122x_EXT_USE_EXT0
  if (extp == &EXTD0) {
    LPC_GPIO0->IS = 0;
    ext_lld_exti_irq_enable(EXTI0_IRQ);
  }
#endif

#if LPC122x_EXT_USE_EXT1
  if (extp == &EXTD1) {
    LPC_GPIO1->IS = 0;
    ext_lld_exti_irq_enable(EXTI1_IRQ);
  }
#endif

#if LPC122x_EXT_USE_EXT2
  if (extp == &EXTD2) {
    LPC_GPIO2->IS = 0;
    ext_lld_exti_irq_enable(EXTI2_IRQ);
  }
#endif

  /* Configuration of autostart channels.*/
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

  LPC_GPIO_Type * gp = extp->gpio;

  if (extp->state == EXT_ACTIVE) {
#if LPC122x_EXT_USE_EXT0
    if (extp == &EXTD0) {
      ext_lld_exti_irq_disable(EXTI0_IRQ);
    }
#endif

#if LPC122x_EXT_USE_EXT1
    if (extp == &EXTD1) {
      ext_lld_exti_irq_disable(EXTI1_IRQ);
    }
#endif

#if LPC122x_EXT_USE_EXT2
    if (extp == &EXTD2) {
      ext_lld_exti_irq_disable(EXTI2_IRQ);
    }
#endif
  }

  gp->IE = 0;
  gp->IC = 0xFFFFFFFF;
  __NOP();
  __NOP();
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

  LPC_GPIO_Type * gp;

  gp = extp->gpio;
  
  /* Programming edge irq enables */
  if (extp->config->channels[channel].mode & EXT_CH_MODE_BOTH_EDGES)
    gp->IBE |=  (1 << channel);
  else {
    gp->IBE &=  ~(1 << channel);
    if (extp->config->channels[channel].mode & EXT_CH_MODE_RISING_EDGE)
      gp->IEV |= (1 << channel);
    else
      gp->IEV &= (1 << channel);
  }

  gp->IC = (1 << channel);    /* Clear interrupt on selected channel */
  __NOP();
  __NOP();

  gp->IE |= (1 << channel);  /* Interrupt on selected channel
                                    is not masked */
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

  LPC_GPIO_Type * gp;

  gp = extp->gpio;

  gp->IE &= ~(1 << channel);   /* Mask interrupt on selected channel */
  gp->IC = (1 << channel);     /* Clear interrupt on selected channel */
  __NOP();
  __NOP();
}

#endif /* HAL_USE_EXT */

/** @} */
