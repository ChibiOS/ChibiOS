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


/**
 * @brief   Declares the isr for the ext channel specified
 *
 * @param[in] channel      number of the channel
 *
 * @notapi
 */

#define declare_extint_isr(channel)                            \
  OSAL_IRQ_HANDLER(INT##channel##_vect)                        \
  {                                                            \
    OSAL_IRQ_PROLOGUE();                                       \
    EXTD1.config->channels[EXT_INT##channel##_CHANNEL].        \
      cb(&EXTD1, EXT_INT##channel##_CHANNEL);                  \
    OSAL_IRQ_EPILOGUE();                                       \
  }


/**
 * @brief   Declares the isr for the pc channel specified
 *
 * @param[in] port      number of the port
 *
 * @notapi
 */

#define declare_pcint_isr(index)                                       \
  OSAL_IRQ_HANDLER(PCINT##index##_vect) {                              \
    OSAL_IRQ_PROLOGUE();                                               \
    EXTD1.pc_current_values[index] =                                   \
      (*(PINS[index])) & (*(PCMSK[index]));                            \
    EXTD1.config->channels[EXT_PCINT##index##_INDEX]                   \
      .cb(&EXTD1, EXT_PCINT##index##_INDEX);                           \
    EXTD1.pc_old_values[index] = EXTD1.pc_current_values[index];       \
    OSAL_IRQ_EPILOGUE();                                               \
  }

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

/**
 * @brief   Vector with addresses of Ports available.
 */
volatile uint8_t * const PINS[EXT_PC_NUM_PORTS] = {
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

/**
 * @brief   Vector with addresses of Port Masks available.
 */
volatile uint8_t * const PCMSK[EXT_PC_NUM_PORTS] = {
  #if 0 < EXT_PC_NUM_PORTS
    (volatile uint8_t *const)&PCMSK0,
  #endif
  #if 1 < EXT_PC_NUM_PORTS
    (volatile uint8_t *const)&PCMSK1,
  #endif
  #if 2 < EXT_PC_NUM_PORTS
    (volatile uint8_t *const)&PCMSK2,
  #endif
  #if 3 < EXT_PC_NUM_PORTS
    (volatile uint8_t *const)&PCMSK3,
  #endif
  #if 4 < EXT_PC_NUM_PORTS
    (volatile uint8_t *const)&PCMSK4,
  #endif
  #if 5 < EXT_PC_NUM_PORTS
    (volatile uint8_t *const)&PCMSK5,
  #endif
  #if 6 < EXT_PC_NUM_PORTS
    (volatile uint8_t *const)&PCMSK6,
  #endif
  #if 7 < EXT_PC_NUM_PORTS
    (volatile uint8_t *const)&PCMSK7,
  #endif
  #if 8 < EXT_PC_NUM_PORTS
    (volatile uint8_t *const)&PCMSK8,
  #endif
  #if 9 < EXT_PC_NUM_PORTS
    (volatile uint8_t *const)&PCMSK9,
  #endif
};


/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

/**
 * @brief   Configures and activates the Pin Changed  inputs.
 *
 * @param[in] extp      pointer to the @p EXTDriver object
 *
 * @notapi
 */
inline void start_pc(EXTDriver *extp) {
  uint8_t new_PCICR = 0;
  uint8_t i;
  for (i = EXT_PC_MIN_PORT; i <= EXT_PC_MAX_PORT; i++)
    if (extp->config->channels[i].mode) {
        new_PCICR |= _BV(i - EXT_PC_MIN_PORT);
        (*(PCMSK[i - EXT_PC_MIN_PORT])) = extp->config->channels[i].mode;
    }
  /* Enables/disables the peripheral, as requested. */
  PCICR = new_PCICR;
}


 /**
 * @brief   Configures and activates the Pin Changed  inputs.
 *
 * @param[in] extp      pointer to the @p EXTDriver object
 *
 * @notapi
 */
inline void start_ext(EXTDriver *extp) {
  uint8_t new_EICRA = 0;
  uint8_t new_EIMSK = 0;
  for (expchannel_t channel = EXT_INT_MIN_CHANNEL;
       channel <= EXT_INT_MAX_CHANNEL; channel++) {
    /* Determines the triggering condition for each channell. */
    switch(extp->config->channels[channel].mode &
           ~(EXT_CH_MODE_AUTOSTART | EXT_CH_MODE_INTERNAL_PULLUP)) {
      case EXT_CH_MODE_LOW_LEVEL:
        new_EICRA |=
          (EXT_EICRA_LOW_LEVEL << (2 * (channel - EXT_INT_MIN_CHANNEL)));
        break;
      case EXT_CH_MODE_BOTH_EDGES:
        new_EICRA |=
          (EXT_EICRA_BOTH_EDGES << (2 * (channel - EXT_INT_MIN_CHANNEL)));
        break;
      case EXT_CH_MODE_RISING_EDGE:
        new_EICRA |=
          (EXT_EICRA_RISING_EDGE << (2 * (channel - EXT_INT_MIN_CHANNEL)));
        break;
      case EXT_CH_MODE_FALLING_EDGE:
        new_EICRA |=
          (EXT_EICRA_FALLING_EDGE << (2 * (channel - EXT_INT_MIN_CHANNEL)));
        break;
      default: osalDbgAssert(FALSE, "unsupported mode");
    }
    if (extp->config->channels[channel].mode & EXT_CH_MODE_AUTOSTART)
      new_EIMSK |= (1 << (channel - EXT_INT_MIN_CHANNEL));
    /* Determines which channel must be started right away. */
    if (extp->config->channels[channel].mode & EXT_CH_MODE_AUTOSTART)
      new_EIMSK |= (1 << (channel - EXT_INT_MIN_CHANNEL));
  }
  /* Configures the peripheral. */
  EICRA = new_EICRA;
  /* Enables/disables the peripheral, as requested. */
  EIMSK = new_EIMSK;
}


/*===========================================================================*/
/* Driver interrupt handlers.                                                */
/*===========================================================================*/

  /*
   * Interrupt handlers for PC-type interrupts.
   */
#if AVR_EXT_USE_EXT1 || defined(__DOXYGEN__)
  #define EXT_PCINT_MIN_INDEX    EXT_PC_MIN_PORT
  #if 0 < EXT_PC_NUM_PORTS
    #define  EXT_PCINT0_INDEX     EXT_PCINT_MIN_INDEX
    declare_pcint_isr(0);
  #endif
  #if 1 < EXT_PC_NUM_PORTS
    #define  EXT_PCINT1_INDEX    (EXT_PCINT0_INDEX  + 1)
    declare_pcint_isr(1);
  #endif
  #if 2 < EXT_PC_NUM_PORTS
    #define  EXT_PCINT2_INDEX    (EXT_PCINT1_INDEX  + 1)
    declare_pcint_isr(2);
  #endif
  #if 3 < EXT_PC_NUM_PORTS
    #define  EXT_PCINT3_INDEX    (EXT_PCINT2_INDEX  + 1)
    declare_pcint_isr(3);
  #endif
  #if 4 < EXT_PC_NUM_PORTS
    #define  EXT_PCINT4_INDEX    (EXT_PCINT3_INDEX  + 1)
    declare_pcint_isr(4);
  #endif
  #if 5 < EXT_PC_NUM_PORTS
    #define  EXT_PCINT5_INDEX    (EXT_PCINT4_INDEX  + 1)
    declare_pcint_isr(5);
  #endif
  #if 6 < EXT_PC_NUM_PORTS
    #define  EXT_PCINT6_INDEX    (EXT_PCINT5_INDEX  + 1)
    declare_pcint_isr(6);
  #endif
  #if 7 < EXT_PC_NUM_PORTS
    #define  EXT_PCINT7_INDEX    (EXT_PCINT6_INDEX  + 1)
    declare_pcint_isr(7);
  #endif
  #if 8 < EXT_PC_NUM_PORTS
    #define  EXT_PCINT8_INDEX    (EXT_PCINT7_INDEX  + 1)
    declare_pcint_isr(8);
  #endif
  #if 9 < EXT_PC_NUM_PORTS
    #define  EXT_PCINT9_INDEX    (EXT_PCINT8_INDEX  + 1)
    declare_pcint_isr(9);
  #endif

  /*
   * Interrupt handlers for EXT-type interrupts.
   */
  #if 0 < EXT_INT_NUM_CHANNELS
    declare_extint_isr(0);
  #endif
  #if 1 < EXT_INT_NUM_CHANNELS
    declare_extint_isr(1);
  #endif
  #if 2 < EXT_INT_NUM_CHANNELS
    declare_extint_isr(2);
  #endif
  #if 3 < EXT_INT_NUM_CHANNELS
    declare_extint_isr(3);
  #endif
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
  for (int i = 0; i < EXT_PC_NUM_PORTS; i++)
    EXTD1.pc_old_values[i] = 0;
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
  start_ext(extp);
  start_pc(extp);
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
    /* Disables the peripheral.*/
#if AVR_EXT_USE_EXT1 == TRUE
    if (&EXTD1 == extp) {
      EIMSK = 0;
      PCICR = 0;
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
  if (EXT_PC_MIN_CHANNEL <= channel && channel <= EXT_PC_MAX_CHANNEL) {
    uint8_t port = channel / 8;
    (*(uint8_t*)&(extp->config->channels[port].mode)) |= _BV(channel % 8);
    (*(PCMSK[port])) = extp->config->channels[port].mode;
    PCICR |= _BV(port);
  } else if (EXT_INT_MIN_CHANNEL <= channel &&
             EXT_INT_MAX_CHANNEL >= channel) {
    EIMSK |= (1 << (channel - EXT_INT_MIN_CHANNEL));
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
  if (EXT_PC_MIN_CHANNEL <= channel && channel <= EXT_PC_MAX_CHANNEL) {
    uint8_t port = channel / 8;
    (*(uint8_t*)&(extp->config->channels[port].mode)) &= ~_BV(channel % 8);
    (*(PCMSK[port])) = extp->config->channels[port].mode;
    if (extp->config->channels[port].mode == 0)
      PCICR &= ~_BV(port);
  } else if (EXT_INT_MIN_CHANNEL <= channel &&
             EXT_INT_MAX_CHANNEL >= channel) {
    EIMSK &= ~(1 << (channel - EXT_INT_MIN_CHANNEL));
  }
}
#endif /* HAL_USE_EXT == TRUE */

/** @} */
