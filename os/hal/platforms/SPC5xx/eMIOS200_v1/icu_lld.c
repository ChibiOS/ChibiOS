/*
    SPC5 HAL - Copyright (C) 2013 STMicroelectronics

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
 * @file    SPC5xx/eMIOS200_v1/icu_lld.c
 * @brief   SPC5xx low level icu driver code.
 *
 * @addtogroup ICU
 * @{
 */

#include "ch.h"
#include "hal.h"

#if HAL_USE_ICU || defined(__DOXYGEN__)

#include "spc5_emios.h"

/*===========================================================================*/
/* Driver local definitions.                                                 */
/*===========================================================================*/

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

/**
 * @brief   ICUD1 driver identifier.
 * @note    The driver ICUD1 allocates the unified channel eMIOS_CH1
 *          when enabled.
 */
#if SPC5_ICU_USE_EMIOS_CH1 || defined(__DOXYGEN__)
ICUDriver ICUD1;
#endif

/**
 * @brief   ICUD2 driver identifier.
 * @note    The driver ICUD2 allocates the unified channel eMIOS_CH2
 *          when enabled.
 */
#if SPC5_ICU_USE_EMIOS_CH2 || defined(__DOXYGEN__)
ICUDriver ICUD2;
#endif

/**
 * @brief   ICUD3 driver identifier.
 * @note    The driver ICUD3 allocates the unified channel eMIOS_CH3
 *          when enabled.
 */
#if SPC5_ICU_USE_EMIOS_CH3 || defined(__DOXYGEN__)
ICUDriver ICUD3;
#endif


/**
 * @brief   ICUD4 driver identifier.
 * @note    The driver ICUD4 allocates the unified channel eMIOS_CH4
 *          when enabled.
 */
#if SPC5_ICU_USE_EMIOS_CH4 || defined(__DOXYGEN__)
ICUDriver ICUD4;
#endif

/**
 * @brief   ICUD5 driver identifier.
 * @note    The driver ICUD5 allocates the unified channel eMIOS_CH5
 *          when enabled.
 */
#if SPC5_ICU_USE_EMIOS_CH5 || defined(__DOXYGEN__)
ICUDriver ICUD5;
#endif

/**
 * @brief   ICUD6 driver identifier.
 * @note    The driver ICUD6 allocates the unified channel eMIOS_CH6
 *          when enabled.
 */
#if SPC5_ICU_USE_EMIOS_CH6 || defined(__DOXYGEN__)
ICUDriver ICUD6;
#endif

/**
 * @brief   ICUD7 driver identifier.
 * @note    The driver ICUD7 allocates the unified channel eMIOS_CH11
 *          when enabled.
 */
#if SPC5_ICU_USE_EMIOS_CH11 || defined(__DOXYGEN__)
ICUDriver ICUD7;
#endif

/**
 * @brief   ICUD8 driver identifier.
 * @note    The driver ICUD8 allocates the unified channel eMIOS_CH13
 *          when enabled.
 */
#if SPC5_ICU_USE_EMIOS_CH13 || defined(__DOXYGEN__)
ICUDriver ICUD8;
#endif

#if SPC5_EMIOS_NUM_CHANNELS == 24
/**
 * @brief   ICUD9 driver identifier.
 * @note    The driver ICUD9 allocates the unified channel eMIOS_CH7
 *          when enabled.
 */
#if SPC5_ICU_USE_EMIOS_CH7 || defined(__DOXYGEN__)
ICUDriver ICUD9;
#endif

/**
 * @brief   ICUD10 driver identifier.
 * @note    The driver ICUD10 allocates the unified channel eMIOS_CH16
 *          when enabled.
 */
#if SPC5_ICU_USE_EMIOS_CH16 || defined(__DOXYGEN__)
ICUDriver ICUD10;
#endif

/**
 * @brief   ICUD11 driver identifier.
 * @note    The driver ICUD11 allocates the unified channel eMIOS_CH17
 *          when enabled.
 */
#if SPC5_ICU_USE_EMIOS_CH17 || defined(__DOXYGEN__)
ICUDriver ICUD11;
#endif

/**
 * @brief   ICUD12 driver identifier.
 * @note    The driver ICUD12 allocates the unified channel eMIOS_CH18
 *          when enabled.
 */
#if SPC5_ICU_USE_EMIOS_CH18 || defined(__DOXYGEN__)
ICUDriver ICUD12;
#endif
#endif

/*===========================================================================*/
/* Driver local variables and types.                                         */
/*===========================================================================*/

/**
 * @brief   Width and Period registers.
 */
uint32_t width;
uint32_t period;

/**
 * @brief   A2 temp registers.
 */
uint32_t A2_1, A2_2, A2_3;

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

/**
 * @brief               ICU IRQ handler.
 *
 * @param[in] icup      pointer to the @p ICUDriver object
 */
static void icu_lld_serve_interrupt(ICUDriver *icup) {

  uint32_t sr = icup->emiosp->CH[icup->ch_number].CSR.R;

  if (sr && EMIOSS_OVFL && icup->config->overflow_cb != NULL) {
    icup->emiosp->CH[icup->ch_number].CSR.R |= EMIOSS_OVFLC;
    _icu_isr_invoke_overflow_cb(icup);
  }
  if (sr && EMIOSS_FLAG) {
    icup->emiosp->CH[icup->ch_number].CSR.R |= EMIOSS_FLAGC;
    if (icup->config->mode == ICU_INPUT_ACTIVE_HIGH) {
      if (icup->emiosp->CH[icup->ch_number].CSR.B.UCIN == 1U  &&            \
          icup->config->period_cb != NULL) {
        A2_3 = icup->emiosp->CH[icup->ch_number].CADR.R;
        period = A2_3 - A2_1;
        _icu_isr_invoke_period_cb(icup);
        A2_1 = A2_3;
      } else if (icup->emiosp->CH[icup->ch_number].CSR.B.UCIN == 0 &&       \
          icup->config->width_cb != NULL) {
        A2_2 = icup->emiosp->CH[icup->ch_number].CADR.R;
        width = A2_2 - A2_1;
        _icu_isr_invoke_width_cb(icup);
      }
    } else if (icup->config->mode == ICU_INPUT_ACTIVE_LOW) {
      if (icup->emiosp->CH[icup->ch_number].CSR.B.UCIN == 1U &&             \
          icup->config->width_cb != NULL) {
        A2_2 = icup->emiosp->CH[icup->ch_number].CADR.R;
        width = A2_2 - A2_1;
        _icu_isr_invoke_width_cb(icup);
      } else if (icup->emiosp->CH[icup->ch_number].CSR.B.UCIN == 0 &&       \
          icup->config->period_cb != NULL) {
        A2_3 = icup->emiosp->CH[icup->ch_number].CADR.R;
        period = A2_3 - A2_1;
        _icu_isr_invoke_period_cb(icup);
        A2_1 = A2_3;
      }
    }
  }
  if (sr && EMIOSS_OVR) {
    icup->emiosp->CH[icup->ch_number].CSR.R |= EMIOSS_OVRC;
  }

}

/*===========================================================================*/
/* Driver interrupt handlers.                                                */
/*===========================================================================*/

#if SPC5_ICU_USE_EMIOS_CH1
#if !defined(SPC5_EMIOS_FLAG_F1_HANDLER)
#error "SPC5_EMIOS_FLAG_F1_HANDLER not defined"
#endif
/**
 * @brief   eMIOS Channel 1 interrupt handler.
 * @note    It is assumed that the various sources are only activated if the
 *          associated callback pointer is not equal to @p NULL in order to not
 *          perform an extra check in a potentially critical interrupt handler.
 *
 * @isr
 */
CH_IRQ_HANDLER(SPC5_EMIOS_FLAG_F1_HANDLER) {

  CH_IRQ_PROLOGUE();

  icu_lld_serve_interrupt(&ICUD1);

  CH_IRQ_EPILOGUE();
}
#endif /* SPC5_ICU_USE_EMIOS_CH1 */

#if SPC5_ICU_USE_EMIOS_CH2
#if !defined(SPC5_EMIOS_FLAG_F2_HANDLER)
#error "SPC5_EMIOS_FLAG_F2_HANDLER not defined"
#endif
/**
 * @brief   eMIOS Channel 2 interrupt handler.
 * @note    It is assumed that the various sources are only activated if the
 *          associated callback pointer is not equal to @p NULL in order to not
 *          perform an extra check in a potentially critical interrupt handler.
 *
 * @isr
 */
CH_IRQ_HANDLER(SPC5_EMIOS_FLAG_F2_HANDLER) {

  CH_IRQ_PROLOGUE();

  icu_lld_serve_interrupt(&ICUD2);

  CH_IRQ_EPILOGUE();
}
#endif /* SPC5_ICU_USE_EMIOS_CH2 */

#if SPC5_ICU_USE_EMIOS_CH3
#if !defined(SPC5_EMIOS_FLAG_F3_HANDLER)
#error "SPC5_EMIOS_FLAG_F3_HANDLER not defined"
#endif
/**
 * @brief   eMIOS Channel 3 interrupt handler.
 * @note    It is assumed that the various sources are only activated if the
 *          associated callback pointer is not equal to @p NULL in order to not
 *          perform an extra check in a potentially critical interrupt handler.
 *
 * @isr
 */
CH_IRQ_HANDLER(SPC5_EMIOS_FLAG_F3_HANDLER) {

  CH_IRQ_PROLOGUE();

  icu_lld_serve_interrupt(&ICUD3);

  CH_IRQ_EPILOGUE();
}
#endif /* SPC5_ICU_USE_EMIOS_CH3 */

#if SPC5_ICU_USE_EMIOS_CH4
#if !defined(SPC5_EMIOS_FLAG_F4_HANDLER)
#error "SPC5_EMIOS_FLAG_F4_HANDLER not defined"
#endif
/**
 * @brief   eMIOS Channel 4 interrupt handler.
 * @note    It is assumed that the various sources are only activated if the
 *          associated callback pointer is not equal to @p NULL in order to not
 *          perform an extra check in a potentially critical interrupt handler.
 *
 * @isr
 */
CH_IRQ_HANDLER(SPC5_EMIOS_FLAG_F4_HANDLER) {

  CH_IRQ_PROLOGUE();

  icu_lld_serve_interrupt(&ICUD4);

  CH_IRQ_EPILOGUE();
}
#endif /* SPC5_ICU_USE_EMIOS_CH4 */

#if SPC5_ICU_USE_EMIOS_CH5
#if !defined(SPC5_EMIOS_FLAG_F5_HANDLER)
#error "SPC5_EMIOS_FLAG_F5_HANDLER not defined"
#endif
/**
 * @brief   eMIOS Channel 5 interrupt handler.
 * @note    It is assumed that the various sources are only activated if the
 *          associated callback pointer is not equal to @p NULL in order to not
 *          perform an extra check in a potentially critical interrupt handler.
 *
 * @isr
 */
CH_IRQ_HANDLER(SPC5_EMIOS_FLAG_F5_HANDLER) {

  CH_IRQ_PROLOGUE();

  icu_lld_serve_interrupt(&ICUD5);

  CH_IRQ_EPILOGUE();
}
#endif /* SPC5_ICU_USE_EMIOS_CH5 */

#if SPC5_ICU_USE_EMIOS_CH6
#if !defined(SPC5_EMIOS_FLAG_F6_HANDLER)
#error "SPC5_EMIOS_FLAG_F6_HANDLER not defined"
#endif
/**
 * @brief   eMIOS Channel 6 interrupt handler.
 * @note    It is assumed that the various sources are only activated if the
 *          associated callback pointer is not equal to @p NULL in order to not
 *          perform an extra check in a potentially critical interrupt handler.
 *
 * @isr
 */
CH_IRQ_HANDLER(SPC5_EMIOS_FLAG_F6_HANDLER) {

  CH_IRQ_PROLOGUE();

  icu_lld_serve_interrupt(&ICUD6);

  CH_IRQ_EPILOGUE();
}
#endif /* SPC5_ICU_USE_EMIOS_CH6 */

#if SPC5_ICU_USE_EMIOS_CH11
#if !defined(SPC5_EMIOS_FLAG_F11_HANDLER)
#error "SPC5_EMIOS_FLAG_F11_HANDLER not defined"
#endif
/**
 * @brief   eMIOS Channel 11 interrupt handler.
 * @note    It is assumed that the various sources are only activated if the
 *          associated callback pointer is not equal to @p NULL in order to not
 *          perform an extra check in a potentially critical interrupt handler.
 *
 * @isr
 */
CH_IRQ_HANDLER(SPC5_EMIOS_FLAG_F11_HANDLER) {

  CH_IRQ_PROLOGUE();

  icu_lld_serve_interrupt(&ICUD7);

  CH_IRQ_EPILOGUE();
}
#endif /* SPC5_ICU_USE_EMIOS_CH11 */

#if SPC5_ICU_USE_EMIOS_CH13
#if !defined(SPC5_EMIOS_FLAG_F13_HANDLER)
#error "SPC5_EMIOS_FLAG_F13_HANDLER not defined"
#endif
/**
 * @brief   eMIOS Channel 13 interrupt handler.
 * @note    It is assumed that the various sources are only activated if the
 *          associated callback pointer is not equal to @p NULL in order to not
 *          perform an extra check in a potentially critical interrupt handler.
 *
 * @isr
 */
CH_IRQ_HANDLER(SPC5_EMIOS_FLAG_F13_HANDLER) {

  CH_IRQ_PROLOGUE();

  icu_lld_serve_interrupt(&ICUD8);

  CH_IRQ_EPILOGUE();
}
#endif /* SPC5_ICU_USE_EMIOS_CH13 */

#if SPC5_EMIOS_NUM_CHANNELS == 24
#if SPC5_ICU_USE_EMIOS_CH7
#if !defined(SPC5_EMIOS_FLAG_F7_HANDLER)
#error "SPC5_EMIOS_FLAG_F7_HANDLER not defined"
#endif
/**
 * @brief   eMIOS Channel 7 interrupt handler.
 * @note    It is assumed that the various sources are only activated if the
 *          associated callback pointer is not equal to @p NULL in order to not
 *          perform an extra check in a potentially critical interrupt handler.
 *
 * @isr
 */
CH_IRQ_HANDLER(SPC5_EMIOS_FLAG_F7_HANDLER) {

  CH_IRQ_PROLOGUE();

  icu_lld_serve_interrupt(&ICUD9);

  CH_IRQ_EPILOGUE();
}
#endif /* SPC5_ICU_USE_EMIOS_CH7 */

#if SPC5_ICU_USE_EMIOS_CH16
#if !defined(SPC5_EMIOS_FLAG_F16_HANDLER)
#error "SPC5_EMIOS_FLAG_F16_HANDLER not defined"
#endif
/**
 * @brief   eMIOS Channel 16 interrupt handler.
 * @note    It is assumed that the various sources are only activated if the
 *          associated callback pointer is not equal to @p NULL in order to not
 *          perform an extra check in a potentially critical interrupt handler.
 *
 * @isr
 */
CH_IRQ_HANDLER(SPC5_EMIOS_FLAG_F16_HANDLER) {

  CH_IRQ_PROLOGUE();

  icu_lld_serve_interrupt(&ICUD10);

  CH_IRQ_EPILOGUE();
}
#endif /* SPC5_ICU_USE_EMIOS_CH16 */

#if SPC5_ICU_USE_EMIOS_CH17
#if !defined(SPC5_EMIOS_FLAG_F17_HANDLER)
#error "SPC5_EMIOS_FLAG_F17_HANDLER not defined"
#endif
/**
 * @brief   eMIOS Channel 17 interrupt handler.
 * @note    It is assumed that the various sources are only activated if the
 *          associated callback pointer is not equal to @p NULL in order to not
 *          perform an extra check in a potentially critical interrupt handler.
 *
 * @isr
 */
CH_IRQ_HANDLER(SPC5_EMIOS_FLAG_F17_HANDLER) {

  CH_IRQ_PROLOGUE();

  icu_lld_serve_interrupt(&ICUD11);

  CH_IRQ_EPILOGUE();
}
#endif /* SPC5_ICU_USE_EMIOS_CH17 */

#if SPC5_ICU_USE_EMIOS_CH18
#if !defined(SPC5_EMIOS_FLAG_F18_HANDLER)
#error "SPC5_EMIOS_FLAG_F18_HANDLER not defined"
#endif
/**
 * @brief   eMIOS Channel 18 interrupt handler.
 * @note    It is assumed that the various sources are only activated if the
 *          associated callback pointer is not equal to @p NULL in order to not
 *          perform an extra check in a potentially critical interrupt handler.
 *
 * @isr
 */
CH_IRQ_HANDLER(SPC5_EMIOS_FLAG_F18_HANDLER) {

  CH_IRQ_PROLOGUE();

  icu_lld_serve_interrupt(&ICUD12);

  CH_IRQ_EPILOGUE();
}
#endif /* SPC5_ICU_USE_EMIOS_CH18 */
#endif

/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

/**
 * @brief   Low level ICU driver initialization.
 *
 * @notapi
 */
void icu_lld_init(void) {

  /* Initialize A2 temp registers.*/
  A2_1 = 0U;
  A2_2 = 0U;
  A2_3 = 0U;

  /* eMIOSx channels initially all not in use.*/
  reset_emios_active_channels();

#if SPC5_ICU_USE_EMIOS_CH1
  /* Driver initialization.*/
  icuObjectInit(&ICUD1);
  ICUD1.emiosp = &EMIOS;
  ICUD1.ch_number = 1U;
  ICUD1.clock = SPC5_EMIOS_CLK;
#endif /* SPC5_ICU_USE_EMIOS_CH1 */

#if SPC5_ICU_USE_EMIOS_CH2
  /* Driver initialization.*/
  icuObjectInit(&ICUD2);
  ICUD2.emiosp = &EMIOS;
  ICUD2.ch_number = 2U;
  ICUD2.clock = SPC5_EMIOS_CLK;
#endif /* SPC5_ICU_USE_EMIOS_CH2 */

#if SPC5_ICU_USE_EMIOS_CH3
  /* Driver initialization.*/
  icuObjectInit(&ICUD3);
  ICUD3.emiosp = &EMIOS;
  ICUD3.ch_number = 3U;
  ICUD3.clock = SPC5_EMIOS_CLK;
#endif /* SPC5_ICU_USE_EMIOS_CH3 */

#if SPC5_ICU_USE_EMIOS_CH4
  /* Driver initialization.*/
  icuObjectInit(&ICUD4);
  ICUD4.emiosp = &EMIOS;
  ICUD4.ch_number = 4U;
  ICUD4.clock = SPC5_EMIOS_CLK;
#endif /* SPC5_ICU_USE_EMIOS_CH4 */

#if SPC5_ICU_USE_EMIOS_CH5
  /* Driver initialization.*/
  icuObjectInit(&ICUD5);
  ICUD5.emiosp = &EMIOS;
  ICUD5.ch_number = 5U;
  ICUD5.clock = SPC5_EMIOS_CLK;
#endif /* SPC5_ICU_USE_EMIOS_CH5 */

#if SPC5_ICU_USE_EMIOS_CH6
  /* Driver initialization.*/
  icuObjectInit(&ICUD6);
  ICUD6.emiosp = &EMIOS;
  ICUD6.ch_number = 6U;
  ICUD6.clock = SPC5_EMIOS_CLK;
#endif /* SPC5_ICU_USE_EMIOS_CH6 */

#if SPC5_ICU_USE_EMIOS_CH11
  /* Driver initialization.*/
  icuObjectInit(&ICUD7);
  ICUD7.emiosp = &EMIOS;
  ICUD7.ch_number = 11U;
  ICUD7.clock = SPC5_EMIOS_CLK;
#endif /* SPC5_ICU_USE_EMIOS_CH11 */

#if SPC5_ICU_USE_EMIOS_CH13
  /* Driver initialization.*/
  icuObjectInit(&ICUD8);
  ICUD8.emiosp = &EMIOS;
  ICUD8.ch_number = 13U;
  ICUD8.clock = SPC5_EMIOS_CLK;
#endif /* SPC5_ICU_USE_EMIOS_CH13 */

#if SPC5_EMIOS_NUM_CHANNELS == 24
#if SPC5_ICU_USE_EMIOS_CH7
  /* Driver initialization.*/
  icuObjectInit(&ICUD9);
  ICUD9.emiosp = &EMIOS;
  ICUD9.ch_number = 7U;
  ICUD9.clock = SPC5_EMIOS_CLK;
#endif /* SPC5_ICU_USE_EMIOS_CH7 */

#if SPC5_ICU_USE_EMIOS_CH16
  /* Driver initialization.*/
  icuObjectInit(&ICUD10);
  ICUD10.emiosp = &EMIOS;
  ICUD10.ch_number = 16U;
  ICUD10.clock = SPC5_EMIOS_CLK;
#endif /* SPC5_ICU_USE_EMIOS_CH16 */

#if SPC5_ICU_USE_EMIOS_CH17
  /* Driver initialization.*/
  icuObjectInit(&ICUD11);
  ICUD11.emiosp = &EMIOS;
  ICUD11.ch_number = 17U;
  ICUD11.clock = SPC5_EMIOS_CLK;
#endif /* SPC5_ICU_USE_EMIOS_CH17 */

#if SPC5_ICU_USE_EMIOS_CH18
  /* Driver initialization.*/
  icuObjectInit(&ICUD12);
  ICUD12.emiosp = &EMIOS;
  ICUD12.ch_number = 18U;
  ICUD12.clock = SPC5_EMIOS_CLK;
#endif /* SPC5_ICU_USE_EMIOS_CH18 */
#endif

#if SPC5_ICU_USE_EMIOS

#if SPC5_EMIOS_NUM_CHANNELS == 16
    INTC.PSR[SPC5_EMIOS_FLAG_F1_NUMBER].R = SPC5_EMIOS_FLAG_F1_PRIORITY;
    INTC.PSR[SPC5_EMIOS_FLAG_F2_NUMBER].R = SPC5_EMIOS_FLAG_F2_PRIORITY;
    INTC.PSR[SPC5_EMIOS_FLAG_F3_NUMBER].R = SPC5_EMIOS_FLAG_F3_PRIORITY;
    INTC.PSR[SPC5_EMIOS_FLAG_F4_NUMBER].R = SPC5_EMIOS_FLAG_F4_PRIORITY;
    INTC.PSR[SPC5_EMIOS_FLAG_F5_NUMBER].R = SPC5_EMIOS_FLAG_F5_PRIORITY;
    INTC.PSR[SPC5_EMIOS_FLAG_F6_NUMBER].R = SPC5_EMIOS_FLAG_F6_PRIORITY;
    INTC.PSR[SPC5_EMIOS_FLAG_F11_NUMBER].R = SPC5_EMIOS_FLAG_F11_PRIORITY;
    INTC.PSR[SPC5_EMIOS_FLAG_F13_NUMBER].R = SPC5_EMIOS_FLAG_F13_PRIORITY;
#endif

#if SPC5_EMIOS_NUM_CHANNELS == 24
    INTC.PSR[SPC5_EMIOS_FLAG_F1_NUMBER].R = SPC5_EMIOS_FLAG_F1_PRIORITY;
    INTC.PSR[SPC5_EMIOS_FLAG_F2_NUMBER].R = SPC5_EMIOS_FLAG_F2_PRIORITY;
    INTC.PSR[SPC5_EMIOS_FLAG_F3_NUMBER].R = SPC5_EMIOS_FLAG_F3_PRIORITY;
    INTC.PSR[SPC5_EMIOS_FLAG_F4_NUMBER].R = SPC5_EMIOS_FLAG_F4_PRIORITY;
    INTC.PSR[SPC5_EMIOS_FLAG_F5_NUMBER].R = SPC5_EMIOS_FLAG_F5_PRIORITY;
    INTC.PSR[SPC5_EMIOS_FLAG_F6_NUMBER].R = SPC5_EMIOS_FLAG_F6_PRIORITY;
    INTC.PSR[SPC5_EMIOS_FLAG_F7_NUMBER].R = SPC5_EMIOS_FLAG_F7_PRIORITY;
    INTC.PSR[SPC5_EMIOS_FLAG_F11_NUMBER].R = SPC5_EMIOS_FLAG_F11_PRIORITY;
    INTC.PSR[SPC5_EMIOS_FLAG_F13_NUMBER].R = SPC5_EMIOS_FLAG_F13_PRIORITY;
    INTC.PSR[SPC5_EMIOS_FLAG_F16_NUMBER].R = SPC5_EMIOS_FLAG_F16_PRIORITY;
    INTC.PSR[SPC5_EMIOS_FLAG_F17_NUMBER].R = SPC5_EMIOS_FLAG_F17_PRIORITY;
    INTC.PSR[SPC5_EMIOS_FLAG_F18_NUMBER].R = SPC5_EMIOS_FLAG_F18_PRIORITY;
#endif

#endif
}

/**
 * @brief   Configures and activates the ICU peripheral.
 *
 * @param[in] icup      pointer to the @p ICUDriver object
 *
 * @notapi
 */
void icu_lld_start(ICUDriver *icup) {

  chDbgAssert(get_emios_active_channels() < SPC5_EMIOS_NUM_CHANNELS,
		      "icu_lld_start(), #1", "too many channels");

  if (icup->state == ICU_STOP) {
    /* Enables the peripheral.*/
#if SPC5_ICU_USE_EMIOS_CH1
    if (&ICUD1 == icup)
      increase_emios_active_channels();
#endif /* SPC5_ICU_USE_EMIOS_CH1 */
#if SPC5_ICU_USE_EMIOS_CH2
    if (&ICUD2 == icup)
      increase_emios_active_channels();
#endif /* SPC5_ICU_USE_EMIOS_CH2 */
#if SPC5_ICU_USE_EMIOS_CH3
    if (&ICUD3 == icup)
      increase_emios_active_channels();
#endif /* SPC5_ICU_USE_EMIOS_CH3 */
#if SPC5_ICU_USE_EMIOS_CH4
    if (&ICUD4 == icup)
      increase_emios_active_channels();
#endif /* SPC5_ICU_USE_EMIOS_CH4 */
#if SPC5_ICU_USE_EMIOS_CH5
    if (&ICUD5 == icup)
      increase_emios_active_channels();
#endif /* SPC5_ICU_USE_EMIOS_CH5 */
#if SPC5_ICU_USE_EMIOS_CH6
    if (&ICUD6 == icup)
      increase_emios_active_channels();
#endif /* SPC5_ICU_USE_EMIOS_CH6 */
#if SPC5_ICU_USE_EMIOS_CH11
    if (&ICUD7 == icup)
      increase_emios_active_channels();
#endif /* SPC5_ICU_USE_EMIOS_CH11 */
#if SPC5_ICU_USE_EMIOS_CH13
    if (&ICUD8 == icup)
      increase_emios_active_channels();
#endif /* SPC5_ICU_USE_EMIOS_CH13 */

#if SPC5_EMIOS_NUM_CHANNELS == 24
#if SPC5_ICU_USE_EMIOS_CH7
    if (&ICUD9 == icup)
      increase_emios_active_channels();
#endif /* SPC5_ICU_USE_EMIOS_CH7 */
#if SPC5_ICU_USE_EMIOS_CH16
    if (&ICUD10 == icup)
      increase_emios_active_channels();
#endif /* SPC5_ICU_USE_EMIOS_CH16 */
#if SPC5_ICU_USE_EMIOS_CH17
    if (&ICUD11 == icup)
      increase_emios_active_channels();
#endif /* SPC5_ICU_USE_EMIOS_CH17 */
#if SPC5_ICU_USE_EMIOS_CH18
    if (&ICUD12 == icup)
      increase_emios_active_channels();
#endif /* SPC5_ICU_USE_EMIOS_CH18 */
#endif

    /* Set eMIOS Clock.*/
#if SPC5_ICU_USE_EMIOS
    active_emios_clock(icup, NULL);
#endif

  }
  /* Configures the peripheral.*/

  /* Channel enables.*/
  icup->emiosp->UCDIS.R &= ~(1 << icup->ch_number);

  /* Clear pending IRQs (if any).*/
  icup->emiosp->CH[icup->ch_number].CSR.R = EMIOSS_OVRC |
      EMIOSS_OVFLC | EMIOSS_FLAGC;

  /* Set clock prescaler and control register.*/
  uint32_t psc = (icup->clock / icup->config->frequency);
  chDbgAssert((psc <= 4) &&
              ((psc * icup->config->frequency) == icup->clock) &&
              ((psc == 1) || (psc == 2) || (psc == 3) || (psc == 4)),
              "icu_lld_start(), #1", "invalid frequency");

  icup->emiosp->CH[icup->ch_number].CCR.B.UCPREN = 0;
  icup->emiosp->CH[icup->ch_number].CCR.R |=
      EMIOSC_BSL(EMIOS_BSL_INTERNAL_COUNTER) |
      EMIOSC_EDSEL | EMIOS_CCR_MODE_SAIC;
  icup->emiosp->CH[icup->ch_number].CCR.B.UCPRE = psc - 1;
  icup->emiosp->CH[icup->ch_number].CCR.R |= EMIOSC_UCPREN;

  /* Set source polarity.*/
  if (icup->config->mode == ICU_INPUT_ACTIVE_HIGH) {
    icup->emiosp->CH[icup->ch_number].CCR.R |= EMIOSC_EDPOL;
  } else {
    icup->emiosp->CH[icup->ch_number].CCR.R &= ~EMIOSC_EDPOL;
  }

  /* Direct pointers to the period and width registers in order to make
     reading data faster from within callbacks.*/
  icup->pccrp = &period;
  icup->wccrp = &width;

  /* Channel disables.*/
  icup->emiosp->UCDIS.R |= (1 << icup->ch_number);

}

/**
 * @brief   Deactivates the ICU peripheral.
 *
 * @param[in] icup      pointer to the @p ICUDriver object
 *
 * @notapi
 */
void icu_lld_stop(ICUDriver *icup) {

  chDbgAssert(get_emios_active_channels() < SPC5_EMIOS_NUM_CHANNELS,
		  	  "icu_lld_stop(), #1", "too many channels");

  if (icup->state == ICU_READY) {

    /* Disables the peripheral.*/
#if SPC5_ICU_USE_EMIOS_CH1
    if (&ICUD1 == icup) {
      /* Reset UC Control Register.*/
      icup->emiosp->CH[icup->ch_number].CCR.R = 0;

      decrease_emios_active_channels();
    }
#endif /* SPC5_ICU_USE_EMIOS_CH1 */
#if SPC5_ICU_USE_EMIOS_CH2
    if (&ICUD2 == icup) {
      /* Reset UC Control Register.*/
      icup->emiosp->CH[icup->ch_number].CCR.R = 0;

      decrease_emios_active_channels();
    }
#endif /* SPC5_ICU_USE_EMIOS_CH2 */
#if SPC5_ICU_USE_EMIOS_CH3
    if (&ICUD3 == icup) {
      /* Reset UC Control Register.*/
      icup->emiosp->CH[icup->ch_number].CCR.R = 0;

      decrease_emios_active_channels();
    }
#endif /* SPC5_ICU_USE_EMIOS_CH3 */
#if SPC5_ICU_USE_EMIOS_CH4
    if (&ICUD4 == icup) {
      /* Reset UC Control Register.*/
      icup->emiosp->CH[icup->ch_number].CCR.R = 0;

      decrease_emios_active_channels();
    }
#endif /* SPC5_ICU_USE_EMIOS_CH4 */
#if SPC5_ICU_USE_EMIOS_CH5
    if (&ICUD5 == icup) {
      /* Reset UC Control Register.*/
      icup->emiosp->CH[icup->ch_number].CCR.R = 0;

      decrease_emios_active_channels();
    }
#endif /* SPC5_ICU_USE_EMIOS_CH5 */
#if SPC5_ICU_USE_EMIOS_CH6
    if (&ICUD6 == icup) {
      /* Reset UC Control Register.*/
      icup->emiosp->CH[icup->ch_number].CCR.R = 0;

      decrease_emios_active_channels();
    }
#endif /* SPC5_ICU_USE_EMIOS_CH6 */
#if SPC5_ICU_USE_EMIOS_CH11
    if (&ICUD7 == icup) {
      /* Reset UC Control Register.*/
      icup->emiosp->CH[icup->ch_number].CCR.R = 0;

      decrease_emios_active_channels();
    }
#endif /* SPC5_ICU_USE_EMIOS_CH11 */
#if SPC5_ICU_USE_EMIOS_CH13
    if (&ICUD8 == icup) {
      /* Reset UC Control Register.*/
      icup->emiosp->CH[icup->ch_number].CCR.R = 0;

      decrease_emios_active_channels();
    }
#endif /* SPC5_ICU_USE_EMIOS_CH13 */

#if SPC5_EMIOS_NUM_CHANNELS == 24
#if SPC5_ICU_USE_EMIOS_CH7
    if (&ICUD9 == icup) {
      /* Reset UC Control Register.*/
      icup->emiosp->CH[icup->ch_number].CCR.R = 0;

      decrease_emios_active_channels();
    }
#endif /* SPC5_ICU_USE_EMIOS_CH7 */
#if SPC5_ICU_USE_EMIOS_CH16
    if (&ICUD10 == icup) {
      /* Reset UC Control Register.*/
      icup->emiosp->CH[icup->ch_number].CCR.R = 0;

      decrease_emios_active_channels();
    }
#endif /* SPC5_ICU_USE_EMIOS_CH16 */
#if SPC5_ICU_USE_EMIOS_CH17
    if (&ICUD11 == icup) {
      /* Reset UC Control Register.*/
      icup->emiosp->CH[icup->ch_number].CCR.R = 0;

      decrease_emios_active_channels();
    }
#endif /* SPC5_ICU_USE_EMIOS_CH17 */
#if SPC5_ICU_USE_EMIOS_CH18
    if (&ICUD12 == icup) {
      /* Reset UC Control Register.*/
      icup->emiosp->CH[icup->ch_number].CCR.R = 0;

      decrease_emios_active_channels();
    }
#endif /* SPC5_ICU_USE_EMIOS_CH18 */
#endif

    /* eMIOS clock deactivation.*/
#if SPC5_ICU_USE_EMIOS
    deactive_emios_clock();
#endif

  }
}

/**
 * @brief   Enables the input capture.
 *
 * @param[in] icup      pointer to the @p ICUDriver object
 *
 * @notapi
 */
void icu_lld_enable(ICUDriver *icup) {

  /* Clear pending IRQs (if any).*/
  icup->emiosp->CH[icup->ch_number].CSR.R = EMIOSS_OVRC |
      EMIOSS_OVFLC | EMIOSS_FLAGC;

  /* Active interrupts.*/
  if (icup->config->period_cb != NULL || icup->config->width_cb != NULL ||  \
      icup->config->overflow_cb != NULL) {
    icup->emiosp->CH[icup->ch_number].CCR.B.FEN = 1U;
  }

  /* Channel enables.*/
  icup->emiosp->UCDIS.R &= ~(1 << icup->ch_number);

}

/**
 * @brief   Disables the input capture.
 *
 * @param[in] icup      pointer to the @p ICUDriver object
 *
 * @notapi
 */
void icu_lld_disable(ICUDriver *icup) {

  /* Clear pending IRQs (if any).*/
  icup->emiosp->CH[icup->ch_number].CSR.R = EMIOSS_OVRC |
        EMIOSS_OVFLC | EMIOSS_FLAGC;

  /* Disable interrupts.*/
  icup->emiosp->CH[icup->ch_number].CCR.B.FEN = 0;

  /* Channel disables.*/
  icup->emiosp->UCDIS.R |= (1 << icup->ch_number);

}

#endif /* HAL_USE_ICU */

/** @} */
