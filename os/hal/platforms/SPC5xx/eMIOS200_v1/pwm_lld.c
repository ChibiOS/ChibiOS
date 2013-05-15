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
 * @file    SPC5xx/eMIOS200_v1/pwm_lld.c
 * @brief   SPC5xx low level pwm driver code.
 *
 * @addtogroup PWM
 * @{
 */

#include "ch.h"
#include "hal.h"

#if HAL_USE_PWM || defined(__DOXYGEN__)

#include "spc5_emios.h"

/*===========================================================================*/
/* Driver local definitions.                                                 */
/*===========================================================================*/

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

/**
 * @brief   PWMD1 driver identifier.
 * @note    The driver PWMD1 allocates the unified channel EMIOS_CH9
 *          when enabled.
 */
#if SPC5_PWM_USE_EMIOS_CH9 || defined(__DOXYGEN__)
PWMDriver PWMD1;
#endif

/**
 * @brief   PWMD2 driver identifier.
 * @note    The driver PWMD2 allocates the unified channel EMIOS_CH10
 *          when enabled.
 */
#if SPC5_PWM_USE_EMIOS_CH10 || defined(__DOXYGEN__)
PWMDriver PWMD2;
#endif

/**
 * @brief   PWMD3 driver identifier.
 * @note    The driver PWMD3 allocates the unified channel EMIOS_CH11
 *          when enabled.
 */
#if SPC5_PWM_USE_EMIOS_CH11 || defined(__DOXYGEN__)
PWMDriver PWMD3;
#endif

/**
 * @brief   PWMD4 driver identifier.
 * @note    The driver PWMD4 allocates the unified channel EMIOS_CH12
 *          when enabled.
 */
#if SPC5_PWM_USE_EMIOS_CH12 || defined(__DOXYGEN__)
PWMDriver PWMD4;
#endif

/**
 * @brief   PWMD5 driver identifier.
 * @note    The driver PWMD5 allocates the unified channel EMIOS_CH13
 *          when enabled.
 */
#if SPC5_PWM_USE_EMIOS_CH13 || defined(__DOXYGEN__)
PWMDriver PWMD5;
#endif

/**
 * @brief   PWMD6 driver identifier.
 * @note    The driver PWMD6 allocates the unified channel EMIOS_CH14
 *          when enabled.
 */
#if SPC5_PWM_USE_EMIOS_CH14 || defined(__DOXYGEN__)
PWMDriver PWMD6;
#endif

/**
 * @brief   PWMD7 driver identifier.
 * @note    The driver PWMD7 allocates the unified channel EMIOS_CH15
 *          when enabled.
 */
#if SPC5_PWM_USE_EMIOS_CH15 || defined(__DOXYGEN__)
PWMDriver PWMD7;
#endif

/**
 * @brief   PWMD8 driver identifier.
 * @note    The driver PWMD8 allocates the unified channel EMIOS_CH23
 *          when enabled.
 */
#if SPC5_PWM_USE_EMIOS_CH23 || defined(__DOXYGEN__)
PWMDriver PWMD8;
#endif

/**
 * @brief   PWMD9 driver identifier.
 * @note    The driver PWMD9 allocates the unified channel EMIOS_CH19
 *          when enabled.
 */
#if SPC5_PWM_USE_EMIOS_CH19 || defined(__DOXYGEN__)
PWMDriver PWMD9;
#endif

/**
 * @brief   PWMD10 driver identifier.
 * @note    The driver PWMD10 allocates the unified channel EMIOS_CH20
 *          when enabled.
 */
#if SPC5_PWM_USE_EMIOS_CH20 || defined(__DOXYGEN__)
PWMDriver PWMD10;
#endif

/**
 * @brief   PWMD11 driver identifier.
 * @note    The driver PWMD11 allocates the unified channel EMIOS_CH21
 *          when enabled.
 */
#if SPC5_PWM_USE_EMIOS_CH21 || defined(__DOXYGEN__)
PWMDriver PWMD11;
#endif

/**
 * @brief   PWMD12 driver identifier.
 * @note    The driver PWMD12 allocates the unified channel EMIOS_CH22
 *          when enabled.
 */
#if SPC5_PWM_USE_EMIOS_CH22 || defined(__DOXYGEN__)
PWMDriver PWMD12;
#endif

/*===========================================================================*/
/* Driver local variables and types.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

/**
 * @brief               PWM IRQ handler.
 *
 * @param[in] pwmp      pointer to the @p PWMDriver object
 */
static void pwm_lld_serve_interrupt(PWMDriver *pwmp) {

  uint32_t sr = pwmp->emiosp->CH[pwmp->ch_number].CSR.R;

  if(sr && EMIOSS_OVFL){
    pwmp->emiosp->CH[pwmp->ch_number].CSR.R |= EMIOSS_OVFLC;
  }
  if(sr && EMIOSS_OVR){
    pwmp->emiosp->CH[pwmp->ch_number].CSR.R |= EMIOSS_OVRC;
  }
  if (sr && EMIOSS_FLAG){
    pwmp->emiosp->CH[pwmp->ch_number].CSR.R |= EMIOSS_FLAGC;

    if (pwmp->config->channels[0].mode == PWM_OUTPUT_ACTIVE_HIGH) {
      if (pwmp->emiosp->CH[pwmp->ch_number].CSR.B.UCOUT == 1U  &&       \
          pwmp->config->callback != NULL) {
        pwmp->config->callback(pwmp);
      } else if (pwmp->emiosp->CH[pwmp->ch_number].CSR.B.UCOUT == 0 &&  \
          pwmp->config->channels[0].callback != NULL) {
        pwmp->config->channels[0].callback(pwmp);
      }
    } else if (pwmp->config->channels[0].mode == PWM_OUTPUT_ACTIVE_LOW) {
      if (pwmp->emiosp->CH[pwmp->ch_number].CSR.B.UCOUT == 0  &&        \
          pwmp->config->callback != NULL) {
        pwmp->config->callback(pwmp);
      } else if (pwmp->emiosp->CH[pwmp->ch_number].CSR.B.UCOUT == 1U && \
          pwmp->config->channels[0].callback != NULL) {
        pwmp->config->channels[0].callback(pwmp);
      }
    }
  }

}

/*===========================================================================*/
/* Driver interrupt handlers.                                                */
/*===========================================================================*/

#if SPC5_PWM_USE_EMIOS_CH9
#if !defined(SPC5_EMIOS_FLAG_F9_HANDLER)
#error "SPC5_EMIOS_FLAG_F9_HANDLER not defined"
#endif
/**
 * @brief   EMIOS Channel 9 interrupt handler.
 * @note    It is assumed that the various sources are only activated if the
 *          associated callback pointer is not equal to @p NULL in order to not
 *          perform an extra check in a potentially critical interrupt handler.
 *
 * @isr
 */
CH_IRQ_HANDLER(SPC5_EMIOS_FLAG_F9_HANDLER) {

  CH_IRQ_PROLOGUE();

  pwm_lld_serve_interrupt(&PWMD1);

  CH_IRQ_EPILOGUE();
}
#endif /* SPC5_PWM_USE_EMIOS_CH9 */

#if SPC5_PWM_USE_EMIOS_CH10
#if !defined(SPC5_EMIOS_FLAG_F10_HANDLER)
#error "SPC5_EMIOS_FLAG_F10_HANDLER not defined"
#endif
/**
 * @brief   EMIOS Channel 10 interrupt handler.
 * @note    It is assumed that the various sources are only activated if the
 *          associated callback pointer is not equal to @p NULL in order to not
 *          perform an extra check in a potentially critical interrupt handler.
 *
 * @isr
 */
CH_IRQ_HANDLER(SPC5_EMIOS_FLAG_F10_HANDLER) {

  CH_IRQ_PROLOGUE();

  pwm_lld_serve_interrupt(&PWMD2);

  CH_IRQ_EPILOGUE();
}
#endif /* SPC5_PWM_USE_EMIOS_CH10 */

#if SPC5_PWM_USE_EMIOS_CH11
#if !defined(SPC5_EMIOS_FLAG_F11_HANDLER)
#error "SPC5_EMIOS_FLAG_F11_HANDLER not defined"
#endif
/**
 * @brief   EMIOS Channel 11 interrupt handler.
 * @note    It is assumed that the various sources are only activated if the
 *          associated callback pointer is not equal to @p NULL in order to not
 *          perform an extra check in a potentially critical interrupt handler.
 *
 * @isr
 */
CH_IRQ_HANDLER(SPC5_EMIOS_FLAG_F11_HANDLER) {

  CH_IRQ_PROLOGUE();

  pwm_lld_serve_interrupt(&PWMD3);

  CH_IRQ_EPILOGUE();
}
#endif /* SPC5_PWM_USE_EMIOS_CH11 */

#if SPC5_PWM_USE_EMIOS_CH12
#if !defined(SPC5_EMIOS_FLAG_F12_HANDLER)
#error "SPC5_EMIOS_FLAG_F12_HANDLER not defined"
#endif
/**
 * @brief   EMIOS Channel 12 interrupt handler.
 * @note    It is assumed that the various sources are only activated if the
 *          associated callback pointer is not equal to @p NULL in order to not
 *          perform an extra check in a potentially critical interrupt handler.
 *
 * @isr
 */
CH_IRQ_HANDLER(SPC5_EMIOS_FLAG_F12_HANDLER) {

  CH_IRQ_PROLOGUE();

  pwm_lld_serve_interrupt(&PWMD4);

  CH_IRQ_EPILOGUE();
}
#endif /* SPC5_PWM_USE_EMIOS_CH12 */

#if SPC5_PWM_USE_EMIOS_CH13
#if !defined(SPC5_EMIOS_FLAG_F13_HANDLER)
#error "SPC5_EMIOS_FLAG_F13_HANDLER not defined"
#endif
/**
 * @brief   EMIOS Channel 13 interrupt handler.
 * @note    It is assumed that the various sources are only activated if the
 *          associated callback pointer is not equal to @p NULL in order to not
 *          perform an extra check in a potentially critical interrupt handler.
 *
 * @isr
 */
CH_IRQ_HANDLER(SPC5_EMIOS_FLAG_F13_HANDLER) {

  CH_IRQ_PROLOGUE();

  pwm_lld_serve_interrupt(&PWMD5);

  CH_IRQ_EPILOGUE();
}
#endif /* SPC5_PWM_USE_EMIOS_CH13 */

#if SPC5_PWM_USE_EMIOS_CH14
#if !defined(SPC5_EMIOS_FLAG_F14_HANDLER)
#error "SPC5_EMIOS_FLAG_F14_HANDLER not defined"
#endif
/**
 * @brief   EMIOS Channel 14 interrupt handler.
 * @note    It is assumed that the various sources are only activated if the
 *          associated callback pointer is not equal to @p NULL in order to not
 *          perform an extra check in a potentially critical interrupt handler.
 *
 * @isr
 */
CH_IRQ_HANDLER(SPC5_EMIOS_FLAG_F14_HANDLER) {

  CH_IRQ_PROLOGUE();

  pwm_lld_serve_interrupt(&PWMD6);

  CH_IRQ_EPILOGUE();
}
#endif /* SPC5_PWM_USE_EMIOS_CH14 */

#if SPC5_PWM_USE_EMIOS_CH15
#if !defined(SPC5_EMIOS_FLAG_F15_HANDLER)
#error "SPC5_EMIOS_FLAG_F15_HANDLER not defined"
#endif
/**
 * @brief   EMIOS Channel 15 interrupt handler.
 * @note    It is assumed that the various sources are only activated if the
 *          associated callback pointer is not equal to @p NULL in order to not
 *          perform an extra check in a potentially critical interrupt handler.
 *
 * @isr
 */
CH_IRQ_HANDLER(SPC5_EMIOS_FLAG_F15_HANDLER) {

  CH_IRQ_PROLOGUE();

  pwm_lld_serve_interrupt(&PWMD7);

  CH_IRQ_EPILOGUE();
}
#endif /* SPC5_PWM_USE_EMIOS_CH15 */

#if SPC5_PWM_USE_EMIOS_CH19
#if !defined(SPC5_EMIOS_FLAG_F19_HANDLER)
#error "SPC5_EMIOS_FLAG_F19_HANDLER not defined"
#endif
/**
 * @brief   EMIOS Channel 19 interrupt handler.
 * @note    It is assumed that the various sources are only activated if the
 *          associated callback pointer is not equal to @p NULL in order to not
 *          perform an extra check in a potentially critical interrupt handler.
 *
 * @isr
 */
CH_IRQ_HANDLER(SPC5_EMIOS_FLAG_F19_HANDLER) {

  CH_IRQ_PROLOGUE();

  pwm_lld_serve_interrupt(&PWMD9);

  CH_IRQ_EPILOGUE();
}
#endif /* SPC5_PWM_USE_EMIOS_CH19 */

#if SPC5_PWM_USE_EMIOS_CH20
#if !defined(SPC5_EMIOS_FLAG_F20_HANDLER)
#error "SPC5_EMIOS_FLAG_F20_HANDLER not defined"
#endif
/**
 * @brief   EMIOS Channel 20 interrupt handler.
 * @note    It is assumed that the various sources are only activated if the
 *          associated callback pointer is not equal to @p NULL in order to not
 *          perform an extra check in a potentially critical interrupt handler.
 *
 * @isr
 */
CH_IRQ_HANDLER(SPC5_EMIOS_FLAG_F20_HANDLER) {

  CH_IRQ_PROLOGUE();

  pwm_lld_serve_interrupt(&PWMD10);

  CH_IRQ_EPILOGUE();
}
#endif /* SPC5_PWM_USE_EMIOS_CH20 */

#if SPC5_PWM_USE_EMIOS_CH21
#if !defined(SPC5_EMIOS_FLAG_F21_HANDLER)
#error "SPC5_EMIOS_FLAG_F21_HANDLER not defined"
#endif
/**
 * @brief   EMIOS Channel 21 interrupt handler.
 * @note    It is assumed that the various sources are only activated if the
 *          associated callback pointer is not equal to @p NULL in order to not
 *          perform an extra check in a potentially critical interrupt handler.
 *
 * @isr
 */
CH_IRQ_HANDLER(SPC5_EMIOS_FLAG_F21_HANDLER) {

  CH_IRQ_PROLOGUE();

  pwm_lld_serve_interrupt(&PWMD11);

  CH_IRQ_EPILOGUE();
}
#endif /* SPC5_PWM_USE_EMIOS_CH21 */

#if SPC5_PWM_USE_EMIOS_CH22
#if !defined(SPC5_EMIOS_FLAG_F22_HANDLER)
#error "SPC5_EMIOS_FLAG_F22_HANDLER not defined"
#endif
/**
 * @brief   EMIOS Channel 22 interrupt handler.
 * @note    It is assumed that the various sources are only activated if the
 *          associated callback pointer is not equal to @p NULL in order to not
 *          perform an extra check in a potentially critical interrupt handler.
 *
 * @isr
 */
CH_IRQ_HANDLER(SPC5_EMIOS_FLAG_F22_HANDLER) {

  CH_IRQ_PROLOGUE();

  pwm_lld_serve_interrupt(&PWMD12);

  CH_IRQ_EPILOGUE();
}
#endif /* SPC5_PWM_USE_EMIOS_CH22 */

#if SPC5_PWM_USE_EMIOS_CH23
#if !defined(SPC5_EMIOS_FLAG_F23_HANDLER)
#error "SPC5_EMIOS_FLAG_F23_HANDLER not defined"
#endif
/**
 * @brief   EMIOS Channel 23 interrupt handler.
 * @note    It is assumed that the various sources are only activated if the
 *          associated callback pointer is not equal to @p NULL in order to not
 *          perform an extra check in a potentially critical interrupt handler.
 *
 * @isr
 */
CH_IRQ_HANDLER(SPC5_EMIOS_FLAG_F23_HANDLER) {

  CH_IRQ_PROLOGUE();

  pwm_lld_serve_interrupt(&PWMD8);

  CH_IRQ_EPILOGUE();
}
#endif /* SPC5_PWM_USE_EMIOS_CH23 */

/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

/**
 * @brief   Low level PWM driver initialization.
 *
 * @notapi
 */
void pwm_lld_init(void) {
  /* eMIOSx channels initially all not in use.*/
  reset_emios_active_channels();

#if SPC5_PWM_USE_EMIOS_CH9
  /* Driver initialization.*/
  pwmObjectInit(&PWMD1);
  PWMD1.emiosp = &EMIOS;
  PWMD1.ch_number = 9U;
#endif /* SPC5_PWM_USE_EMIOS_CH9 */

#if SPC5_PWM_USE_EMIOS_CH10
  /* Driver initialization.*/
  pwmObjectInit(&PWMD2);
  PWMD2.emiosp = &EMIOS;
  PWMD2.ch_number = 10U;
#endif /* SPC5_PWM_USE_EMIOS_CH10 */

#if SPC5_PWM_USE_EMIOS_CH11
  /* Driver initialization.*/
  pwmObjectInit(&PWMD3);
  PWMD3.emiosp = &EMIOS;
  PWMD3.ch_number = 11U;
#endif /* SPC5_PWM_USE_EMIOS_CH11 */

#if SPC5_PWM_USE_EMIOS_CH12
  /* Driver initialization.*/
  pwmObjectInit(&PWMD4);
  PWMD4.emiosp = &EMIOS;
  PWMD4.ch_number = 12U;
#endif /* SPC5_PWM_USE_EMIOS_CH12 */

#if SPC5_PWM_USE_EMIOS_CH13
  /* Driver initialization.*/
  pwmObjectInit(&PWMD5);
  PWMD5.emiosp = &EMIOS;
  PWMD5.ch_number = 13U;
#endif /* SPC5_PWM_USE_EMIOS_CH13 */

#if SPC5_PWM_USE_EMIOS_CH14
  /* Driver initialization.*/
  pwmObjectInit(&PWMD6);
  PWMD6.emiosp = &EMIOS;
  PWMD6.ch_number = 14U;
#endif /* SPC5_PWM_USE_EMIOS_CH14 */

#if SPC5_PWM_USE_EMIOS_CH15
  /* Driver initialization.*/
  pwmObjectInit(&PWMD7);
  PWMD7.emiosp = &EMIOS;
  PWMD7.ch_number = 15U;
#endif /* SPC5_PWM_USE_EMIOS_CH15 */

#if SPC5_PWM_USE_EMIOS_CH23
  /* Driver initialization.*/
  pwmObjectInit(&PWMD8);
  PWMD8.emiosp = &EMIOS;
  PWMD8.ch_number = 23U;
#endif /* SPC5_PWM_USE_EMIOS_CH23 */

#if SPC5_PWM_USE_EMIOS_CH19
  /* Driver initialization.*/
  pwmObjectInit(&PWMD9);
  PWMD9.emiosp = &EMIOS;
  PWMD9.ch_number = 19U;
#endif /* SPC5_PWM_USE_EMIOS_CH19 */

#if SPC5_PWM_USE_EMIOS_CH20
  /* Driver initialization.*/
  pwmObjectInit(&PWMD10);
  PWMD10.emiosp = &EMIOS;
  PWMD10.ch_number = 20U;
#endif /* SPC5_PWM_USE_EMIOS_CH20 */

#if SPC5_PWM_USE_EMIOS_CH21
  /* Driver initialization.*/
  pwmObjectInit(&PWMD11);
  PWMD11.emiosp = &EMIOS;
  PWMD11.ch_number = 21U;
#endif /* SPC5_PWM_USE_EMIOS_CH21 */

#if SPC5_PWM_USE_EMIOS_CH22
  /* Driver initialization.*/
  pwmObjectInit(&PWMD12);
  PWMD12.emiosp = &EMIOS;
  PWMD12.ch_number = 22U;
#endif /* SPC5_PWM_USE_EMIOS_CH22 */

#if SPC5_PWM_USE_EMIOS

#if SPC5_EMIOS_NUM_CHANNELS == 16
    INTC.PSR[SPC5_EMIOS_FLAG_F9_NUMBER].R = SPC5_EMIOS_FLAG_F9_PRIORITY;
    INTC.PSR[SPC5_EMIOS_FLAG_F10_NUMBER].R = SPC5_EMIOS_FLAG_F10_PRIORITY;
    INTC.PSR[SPC5_EMIOS_FLAG_F11_NUMBER].R = SPC5_EMIOS_FLAG_F11_PRIORITY;
    INTC.PSR[SPC5_EMIOS_FLAG_F12_NUMBER].R = SPC5_EMIOS_FLAG_F12_PRIORITY;
    INTC.PSR[SPC5_EMIOS_FLAG_F13_NUMBER].R = SPC5_EMIOS_FLAG_F13_PRIORITY;
    INTC.PSR[SPC5_EMIOS_FLAG_F14_NUMBER].R = SPC5_EMIOS_FLAG_F14_PRIORITY;
    INTC.PSR[SPC5_EMIOS_FLAG_F15_NUMBER].R = SPC5_EMIOS_FLAG_F15_PRIORITY;
    INTC.PSR[SPC5_EMIOS_FLAG_F23_NUMBER].R = SPC5_EMIOS_FLAG_F23_PRIORITY;
#endif

#if SPC5_EMIOS_NUM_CHANNELS == 24
    INTC.PSR[SPC5_EMIOS_FLAG_F9_NUMBER].R = SPC5_EMIOS_FLAG_F9_PRIORITY;
    INTC.PSR[SPC5_EMIOS_FLAG_F10_NUMBER].R = SPC5_EMIOS_FLAG_F10_PRIORITY;
    INTC.PSR[SPC5_EMIOS_FLAG_F11_NUMBER].R = SPC5_EMIOS_FLAG_F11_PRIORITY;
    INTC.PSR[SPC5_EMIOS_FLAG_F12_NUMBER].R = SPC5_EMIOS_FLAG_F12_PRIORITY;
    INTC.PSR[SPC5_EMIOS_FLAG_F13_NUMBER].R = SPC5_EMIOS_FLAG_F13_PRIORITY;
    INTC.PSR[SPC5_EMIOS_FLAG_F14_NUMBER].R = SPC5_EMIOS_FLAG_F14_PRIORITY;
    INTC.PSR[SPC5_EMIOS_FLAG_F15_NUMBER].R = SPC5_EMIOS_FLAG_F15_PRIORITY;
    INTC.PSR[SPC5_EMIOS_FLAG_F19_NUMBER].R = SPC5_EMIOS_FLAG_F19_PRIORITY;
    INTC.PSR[SPC5_EMIOS_FLAG_F20_NUMBER].R = SPC5_EMIOS_FLAG_F20_PRIORITY;
    INTC.PSR[SPC5_EMIOS_FLAG_F21_NUMBER].R = SPC5_EMIOS_FLAG_F21_PRIORITY;
    INTC.PSR[SPC5_EMIOS_FLAG_F22_NUMBER].R = SPC5_EMIOS_FLAG_F22_PRIORITY;
    INTC.PSR[SPC5_EMIOS_FLAG_F23_NUMBER].R = SPC5_EMIOS_FLAG_F23_PRIORITY;
#endif

#endif

}

/**
 * @brief   Configures and activates the PWM peripheral.
 *
 * @param[in] pwmp      pointer to the @p PWMDriver object
 *
 * @notapi
 */
void pwm_lld_start(PWMDriver *pwmp) {

  uint32_t psc = 0;

  chDbgAssert(get_emios_active_channels() < SPC5_EMIOS_NUM_CHANNELS,
              "pwm_lld_start(), #1", "too many channels");

  if (pwmp->state == PWM_STOP) {
#if SPC5_PWM_USE_EMIOS_CH9
    if (&PWMD1 == pwmp) {
      increase_emios_active_channels();
    }
#endif /* SPC5_PWM_USE_EMIOS_CH9 */

#if SPC5_PWM_USE_EMIOS_CH10
    if (&PWMD2 == pwmp) {
      increase_emios_active_channels();
    }
#endif /* SPC5_PWM_USE_EMIOS_CH10 */

#if SPC5_PWM_USE_EMIOS_CH11
    if (&PWMD3 == pwmp) {
      increase_emios_active_channels();
    }
#endif /* SPC5_PWM_USE_EMIOS_CH11 */

#if SPC5_PWM_USE_EMIOS_CH12
    if (&PWMD4 == pwmp) {
      increase_emios_active_channels();
    }
#endif /* SPC5_PWM_USE_EMIOS_CH12 */

#if SPC5_PWM_USE_EMIOS_CH13
    if (&PWMD5 == pwmp) {
      increase_emios_active_channels();
    }
#endif /* SPC5_PWM_USE_EMIOS_CH13 */

#if SPC5_PWM_USE_EMIOS_CH14
    if (&PWMD6 == pwmp) {
      increase_emios_active_channels();
    }
#endif /* SPC5_PWM_USE_EMIOS_CH14 */

#if SPC5_PWM_USE_EMIOS_CH15
    if (&PWMD7 == pwmp) {
      increase_emios_active_channels();
    }
#endif /* SPC5_PWM_USE_EMIOS_CH15 */

#if SPC5_PWM_USE_EMIOS_CH23
    if (&PWMD8 == pwmp) {
      increase_emios_active_channels();
    }
#endif /* SPC5_PWM_USE_EMIOS_CH23 */

#if SPC5_PWM_USE_EMIOS_CH19
    if (&PWMD9 == pwmp) {
      increase_emios_active_channels();
    }
#endif /* SPC5_PWM_USE_EMIOS_CH19 */

#if SPC5_PWM_USE_EMIOS_CH20
    if (&PWMD10 == pwmp) {
      increase_emios_active_channels();
    }
#endif /* SPC5_PWM_USE_EMIOS_CH20 */

#if SPC5_PWM_USE_EMIOS_CH21
    if (&PWMD11 == pwmp) {
      increase_emios_active_channels();
    }
#endif /* SPC5_PWM_USE_EMIOS_CH21 */

#if SPC5_PWM_USE_EMIOS_CH22
    if (&PWMD12 == pwmp) {
      increase_emios_active_channels();
    }
#endif /* SPC5_PWM_USE_EMIOS_CH22 */

    /* Set eMIOS Clock.*/
#if SPC5_PWM_USE_EMIOS
    active_emios_clock(NULL, pwmp);
#endif

  }
  /* Configures the peripheral.*/

  /* Channel enables.*/
  pwmp->emiosp->UCDIS.R &= ~(1 << pwmp->ch_number);

  /* Clear pending IRQs (if any).*/
  pwmp->emiosp->CH[pwmp->ch_number].CSR.R = EMIOSS_OVRC |
      EMIOSS_OVFLC | EMIOSS_FLAGC;

  /* Set clock prescaler and control register.*/
  psc = (SPC5_EMIOS_CLK / pwmp->config->frequency);
  chDbgAssert((psc <= 0xFFFF) &&
              (((psc) * pwmp->config->frequency) == SPC5_EMIOS_CLK) &&
              ((psc == 1) || (psc == 2) || (psc == 3) || (psc == 4)),
              "pwm_lld_start(), #1", "invalid frequency");

  if (pwmp->config->mode == PWM_ALIGN_EDGE) {
    pwmp->emiosp->CH[pwmp->ch_number].CCR.B.UCPREN = 0;
    pwmp->emiosp->CH[pwmp->ch_number].CCR.B.UCPRE = psc - 1U;
    pwmp->emiosp->CH[pwmp->ch_number].CCR.B.UCPREN = 1U;
    pwmp->emiosp->CH[pwmp->ch_number].CCNTR.R = 1U;
    pwmp->emiosp->CH[pwmp->ch_number].CADR.R = 0U;
    pwmp->emiosp->CH[pwmp->ch_number].CBDR.R = pwmp->config->period;
    pwmp->emiosp->CH[pwmp->ch_number].CCR.R |=
        EMIOSC_BSL(EMIOS_BSL_INTERNAL_COUNTER) | EMIOS_CCR_MODE_OPWFMB | 2U;
    pwmp->emiosp->CH[pwmp->ch_number].CCR.R |= EMIOSC_UCPREN;;

    /* Set output polarity.*/
    if(pwmp->config->channels[0].mode == PWM_OUTPUT_ACTIVE_LOW) {
      pwmp->emiosp->CH[pwmp->ch_number].CCR.R |= EMIOSC_EDPOL;
    } else if(pwmp->config->channels[0].mode == PWM_OUTPUT_ACTIVE_HIGH) {
      pwmp->emiosp->CH[pwmp->ch_number].CCR.R &= ~EMIOSC_EDPOL;
    }

    /* Channel disables.*/
    pwmp->emiosp->UCDIS.R |= (1 << pwmp->ch_number);

  } else if (pwmp->config->mode == PWM_ALIGN_CENTER){
    /* Not implemented.*/
  }

}

/**
 * @brief   Deactivates the PWM peripheral.
 *
 * @param[in] pwmp      pointer to the @p PWMDriver object
 *
 * @notapi
 */
void pwm_lld_stop(PWMDriver *pwmp) {

  chDbgAssert(get_emios_active_channels() < SPC5_EMIOS_NUM_CHANNELS,
		  	  "pwm_lld_stop(), #1", "too many channels");

  if (pwmp->state == PWM_READY) {

    /* Disables the peripheral.*/
#if SPC5_PWM_USE_EMIOS_CH9
    if (&PWMD1 == pwmp) {
      /* Reset UC Control Register.*/
      pwmp->emiosp->CH[pwmp->ch_number].CCR.R = 0;

      decrease_emios_active_channels();
    }
#endif /* SPC5_PWM_USE_EMIOS_CH9 */

#if SPC5_PWM_USE_EMIOS_CH10
    if (&PWMD2 == pwmp) {
      /* Reset UC Control Register.*/
      pwmp->emiosp->CH[pwmp->ch_number].CCR.R = 0;

      decrease_emios_active_channels();
    }
#endif /* SPC5_PWM_USE_EMIOS_CH10 */

#if SPC5_PWM_USE_EMIOS_CH11
    if (&PWMD3 == pwmp) {
      /* Reset UC Control Register.*/
      pwmp->emiosp->CH[pwmp->ch_number].CCR.R = 0;

      decrease_emios_active_channels();
    }
#endif /* SPC5_PWM_USE_EMIOS_CH11 */

#if SPC5_PWM_USE_EMIOS_CH12
    if (&PWMD4 == pwmp) {
      /* Reset UC Control Register.*/
      pwmp->emiosp->CH[pwmp->ch_number].CCR.R = 0;

      decrease_emios_active_channels();
    }
#endif /* SPC5_PWM_USE_EMIOS_CH12 */

#if SPC5_PWM_USE_EMIOS_CH13
    if (&PWMD5 == pwmp) {
      /* Reset UC Control Register.*/
      pwmp->emiosp->CH[pwmp->ch_number].CCR.R = 0;

      decrease_emios_active_channels();
    }
#endif /* SPC5_PWM_USE_EMIOS_CH13 */

#if SPC5_PWM_USE_EMIOS_CH14
    if (&PWMD6 == pwmp) {
      /* Reset UC Control Register.*/
      pwmp->emiosp->CH[pwmp->ch_number].CCR.R = 0;

      decrease_emios_active_channels();
    }
#endif /* SPC5_PWM_USE_EMIOS_CH14 */

#if SPC5_PWM_USE_EMIOS_CH15
    if (&PWMD7 == pwmp) {
      /* Reset UC Control Register.*/
      pwmp->emiosp->CH[pwmp->ch_number].CCR.R = 0;

      decrease_emios_active_channels();
    }
#endif /* SPC5_PWM_USE_EMIOS_CH15 */

#if SPC5_PWM_USE_EMIOS_CH23
    if (&PWMD8 == pwmp) {
      /* Reset UC Control Register.*/
      pwmp->emiosp->CH[pwmp->ch_number].CCR.R = 0;

      decrease_emios_active_channels();
    }
#endif /* SPC5_PWM_USE_EMIOS_CH23 */

#if SPC5_PWM_USE_EMIOS_CH19
    if (&PWMD9 == pwmp) {
      /* Reset UC Control Register.*/
      pwmp->emiosp->CH[pwmp->ch_number].CCR.R = 0;

      decrease_emios_active_channels();
    }
#endif /* SPC5_PWM_USE_EMIOS_CH19 */

#if SPC5_PWM_USE_EMIOS_CH20
    if (&PWMD10 == pwmp) {
      /* Reset UC Control Register.*/
      pwmp->emiosp->CH[pwmp->ch_number].CCR.R = 0;

      decrease_emios_active_channels();
    }
#endif /* SPC5_PWM_USE_EMIOS_CH20 */

#if SPC5_PWM_USE_EMIOS_CH21
    if (&PWMD11 == pwmp) {
      /* Reset UC Control Register.*/
      pwmp->emiosp->CH[pwmp->ch_number].CCR.R = 0;

      decrease_emios_active_channels();
    }
#endif /* SPC5_PWM_USE_EMIOS_CH21 */

#if SPC5_PWM_USE_EMIOS_CH22
    if (&PWMD12 == pwmp) {
      /* Reset UC Control Register.*/
      pwmp->emiosp->CH[pwmp->ch_number].CCR.R = 0;

      decrease_emios_active_channels();
    }
#endif /* SPC5_PWM_USE_EMIOS_CH22 */

    /* eMIOS clock deactivation.*/
#if SPC5_PWM_USE_EMIOS
    deactive_emios_clock();
#endif

  }
}

/**
 * @brief   Changes the period the PWM peripheral.
 * @details This function changes the period of a PWM unit that has already
 *          been activated using @p pwmStart().
 * @pre     The PWM unit must have been activated using @p pwmStart().
 * @post    The PWM unit period is changed to the new value.
 * @note    The function has effect at the next cycle start.
 * @note    If a period is specified that is shorter than the pulse width
 *          programmed in one of the channels then the behavior is not
 *          guaranteed.
 *
 * @param[in] pwmp      pointer to a @p PWMDriver object
 * @param[in] period    new cycle time in ticks
 *
 * @notapi
 */
void pwm_lld_change_period(PWMDriver *pwmp, pwmcnt_t period) {

  pwmp->period = period;
  pwmp->emiosp->CH[pwmp->ch_number].CBDR.R = period;

}

/**
 * @brief   Enables a PWM channel.
 * @pre     The PWM unit must have been activated using @p pwmStart().
 * @post    The channel is active using the specified configuration.
 * @note    Depending on the hardware implementation this function has
 *          effect starting on the next cycle (recommended implementation)
 *          or immediately (fallback implementation).
 *
 * @param[in] pwmp      pointer to a @p PWMDriver object
 * @param[in] channel   PWM channel identifier (0...PWM_CHANNELS-1)
 * @param[in] width     PWM pulse width as clock pulses number
 *
 * @notapi
 */
void pwm_lld_enable_channel(PWMDriver *pwmp,
                            pwmchannel_t channel,
                            pwmcnt_t width) {

  (void)channel;

  /* Clear pending IRQs (if any).*/
  pwmp->emiosp->CH[pwmp->ch_number].CSR.R = EMIOSS_OVRC |
      EMIOSS_OVFLC | EMIOSS_FLAGC;

  /* Set pwm width.*/
  pwmp->emiosp->CH[pwmp->ch_number].CADR.R = width;

  /* Active interrupts.*/
  if (pwmp->config->callback != NULL ||                                   \
      pwmp->config->channels[0].callback != NULL) {
    pwmp->emiosp->CH[pwmp->ch_number].CCR.B.FEN = 1U;
  }

  /* Channel enables.*/
  pwmp->emiosp->UCDIS.R &= ~(1 << pwmp->ch_number);

}

/**
 * @brief   Disables a PWM channel.
 * @pre     The PWM unit must have been activated using @p pwmStart().
 * @post    The channel is disabled and its output line returned to the
 *          idle state.
 * @note    Depending on the hardware implementation this function has
 *          effect starting on the next cycle (recommended implementation)
 *          or immediately (fallback implementation).
 *
 * @param[in] pwmp      pointer to a @p PWMDriver object
 * @param[in] channel   PWM channel identifier (0...PWM_CHANNELS-1)
 *
 * @notapi
 */
void pwm_lld_disable_channel(PWMDriver *pwmp, pwmchannel_t channel) {

  (void)channel;
  /* Clear pending IRQs (if any).*/
  pwmp->emiosp->CH[pwmp->ch_number].CSR.R = EMIOSS_OVRC |
        EMIOSS_OVFLC | EMIOSS_FLAGC;

  /* Disable interrupts.*/
  pwmp->emiosp->CH[pwmp->ch_number].CCR.B.FEN = 0;

  /* Channel disables.*/
  pwmp->emiosp->UCDIS.R |= (1 << pwmp->ch_number);

}

#endif /* HAL_USE_PWM */

/** @} */
