/*
    Pin Change Low Level Driver for ChibiOS
    Copyright (C) 2015 Igor Stoppa <igor.stoppa@gmail.com>

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
 * @file    pc_lld.c
 * @brief   AVR PC subsystem low level driver source.
 *
 * @addtogroup PC
 * @{
 */

#include "hal.h"

#if (HAL_USE_PC == TRUE) || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver local definitions.                                                 */
/*===========================================================================*/

#define declare_pcint_isr(channel)                             \
  OSAL_IRQ_HANDLER(PCINT##channel##_vect) {                    \
    OSAL_IRQ_PROLOGUE();                                       \
    PCD1.current_values[channel] =                             \
      (*(PINS[channel])) & (*(PCMSK[channel]));                \
    PCD1.config->cb[PC_INT##channel##_CHANNEL]                 \
      (&PCD1, PC_INT##channel##_CHANNEL);                      \
    PCD1.old_values[channel] = PCD1.current_values[channel];   \
    OSAL_IRQ_EPILOGUE();                                       \
  }

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

/**
 * @brief   PC1 driver identifier.
 */
#if (AVR_PC_USE_PC1 == TRUE) || defined(__DOXYGEN__)
PCDriver PCD1;
#endif

/*===========================================================================*/
/* Driver local variables and types.                                         */
/*===========================================================================*/

volatile uint8_t * const PINS[PC_MAX_PORTS] = {
#if defined(PORTA)
  (volatile uint8_t *const)&PINA,
#endif
#if defined(PORTB)
  (volatile uint8_t *const)&PINB,
#endif
#if defined(PORTC)
  (volatile uint8_t *const)&PINC,
#endif
#if defined(PORTD)
  (volatile uint8_t *const)&PIND,
#endif
#if defined(PORTE)
  (volatile uint8_t *const)&PINE,
#endif
#if defined(PORTF)
  (volatile uint8_t *const)&PINF,
#endif
#if defined(PORTG)
  (volatile uint8_t *const)&PING,
#endif
#if defined(PORTH)
  (volatile uint8_t *const)&PINH,
#endif
#if defined(PORTI)
  (volatile uint8_t *const)&PINI,
#endif
#if defined(PORTJ)
  (volatile uint8_t *const)&PINJ,
#endif
};

volatile uint8_t * const PCMSK[PC_MAX_PORTS] = {
  #if 0 < PC_MAX_CHANNELS
    (volatile uint8_t *const)&PCMSK0,
  #endif
  #if 1 < PC_MAX_CHANNELS
    (volatile uint8_t *const)&PCMSK1,
  #endif
  #if 2 < PC_MAX_CHANNELS
    (volatile uint8_t *const)&PCMSK2,
  #endif
  #if 3 < PC_MAX_CHANNELS
    (volatile uint8_t *const)&PCMSK3,
  #endif
  #if 4 < PC_MAX_CHANNELS
    (volatile uint8_t *const)&PCMSK4,
  #endif
  #if 5 < PC_MAX_CHANNELS
    (volatile uint8_t *const)&PCMSK5,
  #endif
  #if 6 < PC_MAX_CHANNELS
    (volatile uint8_t *const)&PCMSK6,
  #endif
  #if 7 < PC_MAX_CHANNELS
    (volatile uint8_t *const)&PCMSK7,
  #endif
  #if 8 < PC_MAX_CHANNELS
    (volatile uint8_t *const)&PCMSK8,
  #endif
  #if 9 < PC_MAX_CHANNELS
    (volatile uint8_t *const)&PCMSK9,
  #endif
};
/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Driver interrupt handlers.                                                */
/*===========================================================================*/

#if AVR_PC_USE_PC1 || defined(__DOXYGEN__)
  #if 0 < PC_MAX_CHANNELS
    declare_pcint_isr(0);
  #endif
  #if 1 < PC_MAX_CHANNELS
    declare_pcint_isr(1);
  #endif
  #if 2 < PC_MAX_CHANNELS
    declare_pcint_isr(2);
  #endif
  #if 3 < PC_MAX_CHANNELS
    declare_pcint_isr(3);
  #endif
  #if 4 < PC_MAX_CHANNELS
    declare_pcint_isr(4);
  #endif
  #if 5 < PC_MAX_CHANNELS
    declare_pcint_isr(5);
  #endif
  #if 6 < PC_MAX_CHANNELS
    declare_pcint_isr(6);
  #endif
  #if 7 < PC_MAX_CHANNELS
    declare_pcint_isr(7);
  #endif
  #if 8 < PC_MAX_CHANNELS
    declare_pcint_isr(8);
  #endif
  #if 9 < PC_MAX_CHANNELS
    declare_pcint_isr(9);
  #endif
#endif


/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

/**
 * @brief   Low level PC driver initialization.
 *
 * @notapi
 */
void pc_lld_init(void) {
#if AVR_PC_USE_PC1 == TRUE
  int i;
  /* Driver initialization.*/
  pcObjectInit(&PCD1);
  for (i = 0; i < PC_MAX_PORTS; i++)
    PCD1.old_values[i] = 0;
#endif
}

/**
 * @brief   Configures and activates the PC peripheral.
 *
 * @param[in] pcp      pointer to the @p PCDriver object
 *
 * @notapi
 */
void pc_lld_start(PCDriver *pcp) {

#if AVR_PC_USE_PC1 == TRUE
  /* Configures the peripheral. */
  uint8_t new_PCICR = 0;
  uint8_t i;
  for (i = 0; i < PC_MAX_CHANNELS; i++)
    if (pcp->config->enabled[i]) {
        new_PCICR |= _BV(i);
        (*(PCMSK[i])) = pcp->config->enabled[i];
    }
  /* Enables/disables the peripheral, as requested. */
  PCICR = new_PCICR;
#endif

}

/**
 * @brief   Deactivates the PC peripheral.
 *
 * @param[in] pcp   pointer to the @p PCDriver object
 *
 * @notapi
 */
void pc_lld_stop(PCDriver *pcp) {

  if (pcp->state == PC_ACTIVE) {
    /* Resets the peripheral.*/

    /* Disables the peripheral.*/
#if AVR_PC_USE_PC1 == TRUE
    if (&PCD1 == pcp) {
      EIMSK = 0;
    }
#endif
  }
}

/**
 * @brief   Enables an PC channel.
 *
 * @param[in] pcp       pointer to the @p PCDriver object
 * @param[in] channel   channel to be enabled
 *
 * @notapi
 */
void pc_lld_channel_enable(PCDriver *pcp, pcchannel_t channel) {
  uint8_t index = channel / 8;
  ((uint8_t *)(pcp->config->enabled))[index] |= _BV(channel % 8);
  (*(PCMSK[index])) = pcp->config->enabled[index];
  PCICR |= _BV(index);
}

/**
 * @brief   Disables a PC channel.
 *
 * @param[in] pcp      pointer to the @p PCDriver object
 * @param[in] channel  channel to be disabled
 *
 * @notapi
 */
void pc_lld_channel_disable(PCDriver *pcp, expchannel_t channel) {
  uint8_t index = channel / 8;
  ((uint8_t *)(pcp->config->enabled))[index] &= ~_BV(channel % 8);
  (*(PCMSK[index])) = pcp->config->enabled[index];
  if (!(*(PCMSK[index])))
    PCICR &= ~_BV(index);
}

#endif /* HAL_USE_PC == TRUE */

/** @} */
