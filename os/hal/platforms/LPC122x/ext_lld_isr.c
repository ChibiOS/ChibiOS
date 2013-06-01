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
 * @file    LPC122x/ext_lld_isr.c
 * @brief   LPC122x EXT subsystem low level driver ISR code.
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
/* Driver local variables and types.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

#if LPC122x_EXT_USE_EXT0 || LPC122x_EXT_USE_EXT1 || LPC122x_EXT_USE_EXT2 ||   \
  defined(__DOXYGEN__)
/**
 * @brief   I2C error handler.
 *
 * @param[in] i2cp      pointer to the @p I2CDriver object
 *
 * @notapi
 */
static void ext_lld_serve_interrupt(EXTDriver *extp) {
  uint32_t port_stat;
  uint8_t i;

  port_stat = extp->gpio->MIS;   /* Read interrupt status */
  extp->gpio->IC = port_stat;    /* Clear interrupt flags */

  for (i = 0; i < EXT_MAX_CHANNELS; i++) {
    if (port_stat & 0x01) {
      extp->config->channels[i].cb(extp, i);
    }
    port_stat  = port_stat >> 1;
  }
}
#endif
/*===========================================================================*/
/* Driver interrupt handlers.                                                */
/*===========================================================================*/

#if LPC122x_EXT_USE_EXT0 || defined(__DOXYGEN__)
/**
 * @brief   PIO0 interrupt handler.
 *
 * @isr
 */
CH_IRQ_HANDLER(VectorA4) {

  CH_IRQ_PROLOGUE();
  ext_lld_serve_interrupt(&EXTD0);
  CH_IRQ_EPILOGUE();
}
#endif

#if LPC122x_EXT_USE_EXT1 || defined(__DOXYGEN__)
/**
 * @brief   PIO1 interrupt handler.
 *
 * @isr
 */
CH_IRQ_HANDLER(VectorA8) {

  CH_IRQ_PROLOGUE();
  ext_lld_serve_interrupt(&EXTD1);
  CH_IRQ_EPILOGUE();
}
#endif

#if LPC122x_EXT_USE_EXT2 || defined(__DOXYGEN__)
/**
 * @brief   PIO2 interrupt handler.
 *
 * @isr
 */
CH_IRQ_HANDLER(VectorAC) {

  CH_IRQ_PROLOGUE();
  ext_lld_serve_interrupt(&EXTD2);
  CH_IRQ_EPILOGUE();
}
#endif

/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

/**
 * @brief   Enables EXTI IRQ sources.
 *
 * @notapi
 */
void ext_lld_exti_irq_enable(extirq_t irqn) {

  uint32_t pmask;

  switch (irqn) {
  case EXTI0_IRQ:
    pmask = LPC122x_EXT_EXTI0_IRQ_PRIORITY;
    break;
  case EXTI1_IRQ:
    pmask = LPC122x_EXT_EXTI1_IRQ_PRIORITY;
    break;
  case EXTI2_IRQ:
    pmask = LPC122x_EXT_EXTI2_IRQ_PRIORITY;
    break;
  }
  nvicEnableVector(EINT0_IRQn + irqn, CORTEX_PRIORITY_MASK(pmask));

}

/**
 * @brief   Disables EXTI IRQ sources.
 *
 * @notapi
 */
void ext_lld_exti_irq_disable(extirq_t irqn) {

  nvicDisableVector(EINT0_IRQn + irqn);
}

#endif /* HAL_USE_EXT */

/** @} */
