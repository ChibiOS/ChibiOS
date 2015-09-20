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
 * @file    pc_lld.h
 * @brief   AVR PC subsystem low level driver header.
 *
 * @addtogroup PC
 * @{
 */

#ifndef _PC_LLD_H_
#define _PC_LLD_H_

#if (HAL_USE_PC == TRUE) || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver constants.                                                         */
/*===========================================================================*/
#if defined(PORTA)
  #define PORTA_PRESENT 1
  #define PORTA_INDEX   0
#else
  #define PORTA_PRESENT 0
  #define PORTA_INDEX   -1
#endif

#if defined(PORTB)
  #define PORTB_PRESENT 1
  #define PORTB_INDEX   (PORTA_INDEX + 1)
#else
  #define PORTB_PRESENT 0
  #define PORTB_INDEX   PORTA_INDEX
#endif

#if defined(PORTC)
  #define PORTC_PRESENT 1
  #define PORTC_INDEX   (PORTB_INDEX + 1)
#else
  #define PORTC_PRESENT 0
  #define PORTC_INDEX   PORTB_INDEX
#endif

#if defined(PORTD)
  #define PORTD_PRESENT 1
  #define PORTD_INDEX   (PORTC_INDEX + 1)
#else
  #define PORTD_PRESENT 0
  #define PORTD_INDEX   PORTC_INDEX
#endif

#if defined(PORTE)
  #define PORTE_PRESENT 1
  #define PORTE_INDEX   (PORTD_INDEX + 1)
#else
  #define PORTE_PRESENT 0
  #define PORTE_INDEX   PORTD_INDEX
#endif

#if defined(PORTF)
  #define PORTF_PRESENT 1
  #define PORTF_INDEX   (PORTE_INDEX + 1)
#else
  #define PORTF_PRESENT 0
  #define PORTF_INDEX   PORTE_INDEX
#endif

#if defined(PORTG)
  #define PORTG_PRESENT 1
  #define PORTG_INDEX   (PORTF_INDEX + 1)
#else
  #define PORTG_PRESENT 0
  #define PORTG_INDEX   PORTF_INDEX
#endif

#if defined(PORTH)
  #define PORTH_PRESENT 1
  #define PORTH_INDEX   (PORTG_INDEX + 1)
#else
  #define PORTH_PRESENT 0
  #define PORTH_INDEX   PORTG_INDEX
#endif

#if defined(PORTI)
  #define PORTI_PRESENT 1
  #define PORTI_INDEX   (PORTH_INDEX + 1)
#else
  #define PORTI_PRESENT 0
  #define PORTI_INDEX   PORTH_INDEX
#endif

#if defined(PORTJ)
  #define PORTJ_PRESENT 1
  #define PORTJ_INDEX   (PORTI_INDEX + 1)
#else
  #define PORTJ_PRESENT 0
  #define PORTJ_INDEX   PORTI_INDEX
#endif

#if defined(PORTK)
  #define PORTK_PRESENT 1
  #define PORTK_INDEX   (PORTJ_INDEX + 1)
#else
  #define PORTK_PRESENT 0
  #define PORTK_INDEX   PORTJ_INDEX
#endif

/**
 * @brief   Available number of PC channels.
 */
#define PC_MAX_PORTS \
    (PORTA_PRESENT + PORTB_PRESENT + PORTC_PRESENT + PORTD_PRESENT +\
     PORTE_PRESENT + PORTF_PRESENT + PORTG_PRESENT + PORTH_PRESENT +\
     PORTI_PRESENT + PORTJ_PRESENT + PORTK_PRESENT)

/**
 * @brief   Available number of PC channels.
 */
#define PC_INT0_CHANNEL    0
#define PC_INT1_CHANNEL    1
#define PC_INT2_CHANNEL    2
#define PC_INT3_CHANNEL    3
#define PC_INT4_CHANNEL    4
#define PC_INT5_CHANNEL    5
#define PC_INT6_CHANNEL    6
#define PC_INT7_CHANNEL    7
#define PC_INT8_CHANNEL    8
#define PC_INT9_CHANNEL    9
#define PC_MAX_CHANNELS    PC_MAX_PORTS

/*===========================================================================*/
/* Driver pre-compile time settings.                                         */
/*===========================================================================*/

/**
 * @name    AVR configuration options
 * @{
 */
/**
 * @brief   PC driver enable switch.
 * @details If set to @p TRUE the support for PC1 is included.
 * @note    The default is @p FALSE.
 */
#if !defined(AVR_PC_USE_PC1) || defined(__DOXYGEN__)
#define AVR_PC_USE_PC1               FALSE
#endif
/** @} */

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

/*===========================================================================*/
/* Driver data structures and types.                                         */
/*===========================================================================*/

/**
 * @brief   PC channel identifier.
 */
typedef uint8_t pcchannel_t;


/**
 * @brief   PC port identifier.
 */
typedef uint8_t pcport_t;

/**
 * @brief   Type of an PC generic notification callback.
 *
 * @param[in] pcp      pointer to the @p EXPDriver object triggering the
 *                     callback
 */
typedef void (*pccallback_t)(PCDriver *pcp, pcchannel_t channel);


/**
 * @brief   Driver configuration structure.
 */
typedef struct {
  /**
   * @brief Channel configurations.
   */
  uint8_t          enabled[PC_MAX_CHANNELS];
  pccallback_t     cb[PC_MAX_CHANNELS];
  /* End of the mandatory fields.*/
} PCConfig;

/**
 * @brief   Structure representing an PC driver.
 */
struct PCDriver {
  /**
   * @brief Driver state.
   */
  pcstate_t        state;
  /**
   * @brief Current configuration data.
   */
  const PCConfig   *config;
  /* End of the mandatory fields.*/
  /**
   * @brief previous values.
   */
  uint8_t           current_values[PC_MAX_PORTS];
  uint8_t           old_values[PC_MAX_PORTS];
};

/*===========================================================================*/
/* Driver macros.                                                            */
/*===========================================================================*/

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#if (AVR_PC_USE_PC1 == TRUE) && !defined(__DOXYGEN__)
extern PCDriver PCD1;
volatile uint8_t *const PINS[PC_MAX_PORTS];
volatile uint8_t *const PCMSK[PC_MAX_PORTS];
#endif

#ifdef __cplusplus
extcern "C" {
#endif
  void pc_lld_init(void);
  void pc_lld_start(PCDriver *pcp);
  void pc_lld_stop(PCDriver *pcp);
  void pc_lld_channel_enable(PCDriver *pcp, pcchannel_t channel);
  void pc_lld_channel_disable(PCDriver *pcp, pcchannel_t channel);
#ifdef __cplusplus
}
#endif

#endif /* HAL_USE_PC == TRUE */

#endif /* _PC_LLD_H_ */

/** @} */
