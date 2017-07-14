/*
  EXT Low Level Driver for ChibiOS
  Copyright (C) 2015 Igor Stoppa <igor.stoppa@gmail.com>
  Copyright (C) 2016 Theodore Ateba

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
 * @file  hal_ext_lld.c
 * @brief AVR EXT subsystem low level driver source.
 *
 * @addtogroup EXT
 * @{
 */

#include "hal.h"

#if HAL_USE_EXT || defined(__DOXYGEN__)

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
#define declare_extint_isr(channel)                                   \
OSAL_IRQ_HANDLER(INT##channel##_vect)                                 \
{                                                                     \
  OSAL_IRQ_PROLOGUE();                                                \
  EXTD1.config->channels[EXT_INT##channel##_CHANNEL].                 \
    cb(&EXTD1, EXT_INT##channel##_CHANNEL);                           \
  OSAL_IRQ_EPILOGUE();                                                \
}

/**
 * @brief   Declares the isr for the pc channel specified
 *
 * @param[in] port      number of the port
 *
 * @notapi
 */
#define declare_pcint_isr(index)                                             \
OSAL_IRQ_HANDLER(PCINT##index##_vect) {                                      \
  uint8_t changed_pins;                                                      \
  uint8_t i;                                                                 \
                                                                             \
  OSAL_IRQ_PROLOGUE();                                                       \
  EXTD1.pc_current_values[index] = (*(PINS[index])) & (*(PCMSK[index]));     \
                                                                             \
  /* XOR to find the changed pin(s) */                                       \
  changed_pins = EXTD1.pc_current_values[index] ^ EXTD1.pc_old_values[index];\
                                                                             \
  for (i = 0; i < 8; i++) {                                                  \
    if (changed_pins & (1 << i)) {                                           \
      const EXTChannelConfig *chn =                                          \
        &EXTD1.config->channels[EXT_PCINT##index##_INDEX + i];               \
                                                                             \
      if (((chn->mode & EXT_CH_MODE_RISING_EDGE) &&                          \
           ((EXTD1.pc_current_values[index] & (1 << i)) > 0)) ||             \
          ((chn->mode & EXT_CH_MODE_FALLING_EDGE) &&                         \
           ((EXTD1.pc_current_values[index] & (1 << i)) == 0))) {            \
        chn->cb(&EXTD1, EXT_PCINT##index##_INDEX + i - EXT_PCINT_MIN_INDEX); \
      }                                                                      \
    }                                                                        \
  }                                                                          \
                                                                             \
  EXTD1.pc_old_values[index] = EXTD1.pc_current_values[index];               \
  OSAL_IRQ_EPILOGUE();                                                       \
}

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

/**
 * @brief   EXT1 driver identifier.
 */
EXTDriver EXTD1;

/*===========================================================================*/
/* Driver local variables and types.                                         */
/*===========================================================================*/

#if EXT_PC_NUM_PORTS > 0
/**
 * @brief   Vector with addresses of Ports available.
 */
volatile uint8_t * const PINS[EXT_PC_NUM_PORTS] = {
#if AVR_EXT_USE_PCINT0
  (volatile uint8_t *const)&PCINT0_PIN,
#endif
#if AVR_EXT_USE_PCINT1
  (volatile uint8_t *const)&PCINT1_PIN,
#endif
#if AVR_EXT_USE_PCINT2
(volatile uint8_t *const)&PCINT2_PIN,
#endif
#if AVR_EXT_USE_PCINT3
(volatile uint8_t *const)&PCINT3_PIN,
#endif
#if AVR_EXT_USE_PCINT4
(volatile uint8_t *const)&PCINT4_PIN,
#endif
#if AVR_EXT_USE_PCINT5
(volatile uint8_t *const)&PCINT5_PIN,
#endif
#if AVR_EXT_USE_PCINT6
(volatile uint8_t *const)&PCINT6_PIN,
#endif
#if AVR_EXT_USE_PCINT7
(volatile uint8_t *const)&PCINT7_PIN,
#endif
#if AVR_EXT_USE_PCINT8
(volatile uint8_t *const)&PCINT8_PIN,
#endif
#if AVR_EXT_USE_PCINT9
(volatile uint8_t *const)&PCINT9_PIN,
#endif
#if AVR_EXT_USE_PCINT10
(volatile uint8_t *const)&PCINT10_PIN,
#endif
};

/**
 * @brief   Vector with addresses of Port Masks available.
 */
volatile uint8_t * const PCMSK[EXT_PC_NUM_PORTS] = {
#if AVR_EXT_USE_PCINT0
  (volatile uint8_t *const)&PCMSK0,
#endif
#if AVR_EXT_USE_PCINT1
  (volatile uint8_t *const)&PCMSK1,
#endif
#if AVR_EXT_USE_PCINT2
  (volatile uint8_t *const)&PCMSK2,
#endif
#if AVR_EXT_USE_PCINT3
  (volatile uint8_t *const)&PCMSK3,
#endif
#if AVR_EXT_USE_PCINT4
  (volatile uint8_t *const)&PCMSK4,
#endif
#if AVR_EXT_USE_PCINT5
  (volatile uint8_t *const)&PCMSK5,
#endif
#if AVR_EXT_USE_PCINT6
  (volatile uint8_t *const)&PCMSK6,
#endif
#if AVR_EXT_USE_PCINT7
  (volatile uint8_t *const)&PCMSK7,
#endif
#if AVR_EXT_USE_PCINT8
  (volatile uint8_t *const)&PCMSK8,
#endif
#if AVR_EXT_USE_PCINT9
  (volatile uint8_t *const)&PCMSK9,
#endif
#if AVR_EXT_USE_PCINT10
  (volatile uint8_t *const)&PCMSK10,
#endif
};
#endif

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

#if EXT_PC_NUM_PORTS > 0
/**
 * @brief   Configures and activates the Pin Change inputs.
 *
 * @param[in] extp      pointer to the @p EXTDriver object
 *
 * @notapi
 */
static void start_pc(EXTDriver *extp) {
  uint8_t icr = 0;
  uint8_t i;

  /* For every pin */
  for (i = 0; i < EXT_PC_NUM_CHANNELS; i++) {
    uint8_t mode = extp->config->channels[i + EXT_PC_MIN_CHANNEL].mode;

    /* Only start if autostart and not disabled */
    if ((mode & EXT_CH_MODE_AUTOSTART) && ((mode & EXT_CH_MODE_EDGES_MASK) != EXT_CH_MODE_DISABLED)) {
      (*(PCMSK[i/8])) |= _BV(i & 0x07);
    }
  }

  /* For every port */
  for (i = 0; i < EXT_PC_NUM_PORTS; i++) {
    /* Only enable interrupt if at least 1 bit in the mask is set */
    if ((*(PCMSK[i])) != 0) {
      /* Enable interrupt */
      icr |= (_BV(i));
    }
  }

  /* Enables/disables the peripheral, as requested. */
#if defined(__AVR_ATmega162__)
  GICR &= ~(0x03 << 3);
  GICR |= (icr << 3);
#else
  PCICR = icr;
#endif
}

/**
 * @brief   Deactivates the PC interrupts.
 * 
 * @param[in] extp      pointer to the @p EXTDriver object   
 */
static void stop_pc(EXTDriver *extp) {
  uint8_t i;
  (void)extp;

  /* Disable pin change interrupts */
#if defined(__AVR_ATmega162__)
  GICR &= ~(0x03 << 3);
#else
  PCICR = 0;
#endif

  /* Clear masks */
  for (i = 0; i < EXT_PC_NUM_PORTS; i++) {
    (*(PCMSK[i])) = 0;
  }
}
#endif

#if EXT_INT_NUM_CHANNELS > 0
/**
 * @brief   Configures and activates the INT inputs.
 *
 * @param[in] extp      pointer to the @p EXTDriver object
 *
 * @notapi
 */
static void start_ext(EXTDriver *extp) {
#if EXT_INT_NUM_CHANNELS < 4
  uint8_t icr = 0;
#else
  uint16_t icr = 0;
#endif
  uint8_t msk = 0;
  for (expchannel_t channel = EXT_INT_MIN_CHANNEL;
       channel <= EXT_INT_MAX_CHANNEL; channel++) {
    /* Determines the triggering condition for each channel. */
    switch(extp->config->channels[channel].mode &
           ~(EXT_CH_MODE_AUTOSTART | EXT_CH_MODE_INTERNAL_PULLUP)) {
    case EXT_CH_MODE_LOW_LEVEL:
      icr |= (EXT_EICRA_LOW_LEVEL << (2 * (channel - EXT_INT_MIN_CHANNEL)));
      break;
    case EXT_CH_MODE_BOTH_EDGES:
      icr |= (EXT_EICRA_BOTH_EDGES << (2 * (channel - EXT_INT_MIN_CHANNEL)));
      break;
    case EXT_CH_MODE_RISING_EDGE:
      icr |= (EXT_EICRA_RISING_EDGE << (2 * (channel - EXT_INT_MIN_CHANNEL)));
      break;
    case EXT_CH_MODE_FALLING_EDGE:
      icr |= (EXT_EICRA_FALLING_EDGE << (2 * (channel - EXT_INT_MIN_CHANNEL)));
      break;
    default: osalDbgAssert(FALSE, "unsupported mode");
    }

    /* Determines which channel must be started right away. */
    if (extp->config->channels[channel].mode & EXT_CH_MODE_AUTOSTART) {
      msk |= (1 << (channel - EXT_INT_MIN_CHANNEL));
    }
  }
  /* Configures the peripheral. */
#if defined(__AVR_ATmega162__)
  MCUCR |= (icr & 0x0f);

  icr >>=  4;
  osalDbgAssert(((icr & 0x02) == EXT_EICRA_RISING_EDGE) || ((icr & 0x02) == EXT_EICRA_FALLING_EDGE), "INT2 only supports rising or falling edge, not both.");
  EMCUCR |= icr & 0x01;
  
  GICR |= ((msk & 0x03) << 6);
  if (icr & 0x01) {
    /* Enable INT2 */
    GICR |= (1 << 5);
  }
#else
  EICRA = icr & 0xff;
#if EXT_INT_NUM_CHANNELS > 4
  EICRB = icr >> 8;
#endif
  /* Enables/disables the peripheral, as requested. */
  EIMSK = msk;
#endif
}

/**
 * @brief   Deactivates the INT interrupts.
 * 
 * @param[in] extp      pointer to the @p EXTDriver object
 */
static void stop_ext(EXTDriver *extp) {
  (void)extp;
#if defined(__AVR_ATmega162__)
  MCUCR &= ~(0x0f);
  EMCUCR &= ~(0x01);
  GICR |= ~(0x07 << 5);
#else
  EICRA = 0;
#if EXT_INT_NUM_CHANNELS > 4
  EICRB = 0;
#endif
  /* Enables/disables the peripheral, as requested. */
  EIMSK = 0;
#endif
}
#endif

/*===========================================================================*/
/* Driver interrupt handlers.                                                */
/*===========================================================================*/

/*
 * Interrupt handlers for PC-type interrupts.
 */
#define EXT_PCINT_MIN_INDEX    EXT_PC_MIN_PORT

#if 0 < EXT_PC_NUM_PORTS
#define EXT_PCINT0_INDEX     EXT_PCINT_MIN_INDEX
declare_pcint_isr(0);
#endif
#if 1 < EXT_PC_NUM_PORTS
#define EXT_PCINT1_INDEX    (EXT_PCINT0_INDEX  + 1)
declare_pcint_isr(1);
#endif
#if 2 < EXT_PC_NUM_PORTS
#define EXT_PCINT2_INDEX    (EXT_PCINT1_INDEX  + 1)
declare_pcint_isr(2);
#endif
#if 3 < EXT_PC_NUM_PORTS
#define EXT_PCINT3_INDEX    (EXT_PCINT2_INDEX  + 1)
declare_pcint_isr(3);
#endif
#if 4 < EXT_PC_NUM_PORTS
#define EXT_PCINT4_INDEX    (EXT_PCINT3_INDEX  + 1)
declare_pcint_isr(4);
#endif
#if 5 < EXT_PC_NUM_PORTS
#define EXT_PCINT5_INDEX    (EXT_PCINT4_INDEX  + 1)
declare_pcint_isr(5);
#endif
#if 6 < EXT_PC_NUM_PORTS
#define EXT_PCINT6_INDEX    (EXT_PCINT5_INDEX  + 1)
declare_pcint_isr(6);
#endif
#if 7 < EXT_PC_NUM_PORTS
#define EXT_PCINT7_INDEX    (EXT_PCINT6_INDEX  + 1)
declare_pcint_isr(7);
#endif
#if 8 < EXT_PC_NUM_PORTS
#define EXT_PCINT8_INDEX    (EXT_PCINT7_INDEX  + 1)
declare_pcint_isr(8);
#endif
#if 9 < EXT_PC_NUM_PORTS
#define EXT_PCINT9_INDEX    (EXT_PCINT8_INDEX  + 1)
declare_pcint_isr(9);
#endif

/*
 * Interrupt handlers for INT-type interrupts.
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
#if 4 < EXT_INT_NUM_CHANNELS
declare_extint_isr(4);
#endif
#if 5 < EXT_INT_NUM_CHANNELS
declare_extint_isr(5);
#endif

/*===========================================================================*/
/* Driver functions.                                                         */
/*===========================================================================*/

/**
 * @brief   Low level EXT driver initialization.
 *
 * @notapi
 */
void ext_lld_init(void) {
  /* Driver initialization.*/
  extObjectInit(&EXTD1);
#if EXT_PC_NUM_PORTS > 0
  for (int i = 0; i < EXT_PC_NUM_PORTS; i++) {
    EXTD1.pc_old_values[i] = 0;
  }
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
#if EXT_INT_NUM_CHANNELS > 0
  start_ext(extp);
#endif
#if EXT_PC_NUM_PORTS > 0
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
#if EXT_INT_NUM_CHANNELS > 0
    stop_ext(extp);
#endif
#if EXT_PC_NUM_PORTS > 0
    stop_pc(extp);
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
  (void)extp;
#if EXT_PC_NUM_CHANNELS > 0
  if (EXT_PC_MIN_CHANNEL <= channel && channel <= EXT_PC_MAX_CHANNEL) {
    uint8_t port = (channel - EXT_PC_MIN_CHANNEL) / 8;

    /* Enable bit in mask */
    (*(PCMSK[port])) |= _BV((channel - EXT_PC_MIN_CHANNEL) % 8);

    /* Always enable interrupt */
#if defined(__AVR_ATmega162__)
    GICR |= (_BV(port) << 3);
#else
    PCICR |= _BV(port);
#endif
  }
#endif
#if EXT_PC_NUM_CHANNELS > 0 && EXT_INT_NUM_CHANNELS > 0
  else
#endif
#if EXT_INT_NUM_CHANNELS > 0
  if (channel <= EXT_INT_MAX_CHANNEL) {
#if defined(__AVR_ATmega162__)
    GICR |= ((1 << channel) << 5);
#else
    /* Enables/disables the peripheral, as requested. */
    EIMSK |= (1 << channel);
#endif
  }
#endif
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
#if EXT_PC_NUM_CHANNELS > 0
  if (EXT_PC_MIN_CHANNEL <= channel && channel <= EXT_PC_MAX_CHANNEL) {
    uint8_t port = (channel - EXT_PC_MIN_CHANNEL) / 8;

    /* Clear bit in mask */
    (*(PCMSK[port])) &= ~(_BV((channel - EXT_PC_MIN_CHANNEL) % 8));

    /* Disable interrupt if no bits are set */
    if ((*(PCMSK[port])) == 0) {
#if defined(__AVR_ATmega162__)
      GICR &= ~(_BV(port) << 3);
#else
      PCICR |= ~(_BV(port));
#endif
    }
  }
#endif
#if EXT_PC_NUM_CHANNELS > 0 && EXT_INT_NUM_CHANNELS > 0
  else
#endif
#if EXT_INT_NUM_CHANNELS > 0
  if (channel <= EXT_INT_MAX_CHANNEL) {
#if defined(__AVR_ATmega162__)
    GICR &= ~((1 << channel) << 5);
#else
    /* Enables/disables the peripheral, as requested. */
    EIMSK &= ~(1 << channel);
#endif
  }
#endif
}

#endif /* HAL_USE_EXT */

/** @} */
