/*
    EXT Low Level Driver for ChibiOS
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
 * @file    ext_lld.c
 * @brief   AVR EXT subsystem low level driver source.
 *
 * @addtogroup EXT
 * @{
 */

#include "hal.h"

#if (HAL_USE_EXT == TRUE) || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver local definitions.                                                 */
/*===========================================================================*/

#define EXT_EICRA_LOW_LEVEL    0
#define EXT_EICRA_BOTH_EDGES   1
#define EXT_EICRA_FALLING_EDGE 2
#define EXT_EICRA_RISING_EDGE  3

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

/**
 * @brief   EXT1 driver identifier.
 */
#if (AVR_EXT_USE_EXT1 == TRUE) || defined(__DOXYGEN__)
EXTDriver EXTD1;
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

#if AVR_EXT_USE_EXT1 || defined(__DOXYGEN__)
OSAL_IRQ_HANDLER(INT0_vect)
{
  OSAL_IRQ_PROLOGUE();
  EXTD1.config->channels[EXT_INT0_CHANNEL].cb(&EXTD1, EXT_INT0_CHANNEL);
  OSAL_IRQ_EPILOGUE();
}

OSAL_IRQ_HANDLER(INT1_vect)
{
  OSAL_IRQ_PROLOGUE();
  EXTD1.config->channels[EXT_INT1_CHANNEL].cb(&EXTD1, EXT_INT1_CHANNEL);
  OSAL_IRQ_EPILOGUE();
}
#endif


/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

/**
 * @brief   Low level EXT driver initialization.
 *
 * @notapi
 */
void ext_lld_init(void) {

#if AVR_EXT_USE_EXT1 == TRUE
  /* Driver initialization.*/
  extObjectInit(&EXTD1);
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

#if AVR_EXT_USE_EXT1 == TRUE
  uint8_t new_EICRA = 0;
  uint8_t new_EIMSK = 0;
  for (expchannel_t channel = 0; channel < EXT_MAX_CHANNELS; channel++) {
    DDRD &= ~_BV(channel + 2); /* Set line as input. */
    /* Determines the triggering condition for each channell. */
    switch(extp->config->channels[channel].mode &
           ~(EXT_CH_MODE_AUTOSTART | EXT_CH_MODE_INTERNAL_PULLUP)) {
      case EXT_CH_MODE_LOW_LEVEL:
        new_EICRA |= (EXT_EICRA_LOW_LEVEL << (2 * channel));
        break;
      case EXT_CH_MODE_BOTH_EDGES:
        new_EICRA |= (EXT_EICRA_BOTH_EDGES << (2 * channel));
        break;
      case EXT_CH_MODE_RISING_EDGE:
        new_EICRA |= (EXT_EICRA_RISING_EDGE << (2 * channel));
        break;
      case EXT_CH_MODE_FALLING_EDGE:
        new_EICRA |= (EXT_EICRA_FALLING_EDGE << (2 * channel));
        break;
      default: osalDbgAssert(FALSE, "unsupported mode");
    }
    if (extp->config->channels[channel].mode & EXT_CH_MODE_AUTOSTART)
      new_EIMSK |= (1 << channel);
    /* Determines which channel must be started right away. */
    if (extp->config->channels[channel].mode & EXT_CH_MODE_AUTOSTART)
      new_EIMSK |= (1 << channel);
    if (extp->config->channels[channel].mode & EXT_CH_MODE_INTERNAL_PULLUP) {
      PORTD |= _BV(channel + 2);  /* Enable intenal pullup. */
      MCUCR &= ~_BV(4);            /* Master pull-up enable. */
    }
  }
  /* Configures the peripheral. */
  EICRA = new_EICRA;
  /* Enables/disables the peripheral, as requested. */
  EIMSK = new_EIMSK;
#endif

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
    /* Resets the peripheral.*/

    /* Disables the peripheral.*/
#if AVR_EXT_USE_EXT1 == TRUE
    if (&EXTD1 == extp) {
      EIMSK = 0;
    }
#endif
  }
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
  EIMSK |= (1 << channel);
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
  EIMSK &= ~(1 << channel);
  PORTD &= ~(1 << (channel + 2));
}

#endif /* HAL_USE_EXT == TRUE */

/** @} */
