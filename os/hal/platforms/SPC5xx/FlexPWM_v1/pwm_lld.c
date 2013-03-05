/*
 * Licensed under ST Liberty SW License Agreement V2, (the "License");
 * You may not use this file except in compliance with the License.
 * You may obtain a copy of the License at:
 *
 *        http://www.st.com/software_license_agreement_liberty_v2
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/**
 * @file    SPC560Pxx/pwm_lld.c
 * @brief   SPC560Pxx low level FlexPWM driver code.
 *
 * @addtogroup PWM
 * @{
 */

#include "ch.h"
#include "hal.h"

#if HAL_USE_PWM || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

/**
 * @brief   PWMD1 driver identifier.
 * @note    The driver PWMD1 allocates the complex timer TIM1 when enabled.
 */
#if SPC5_PWM_USE_SMOD0 || defined(__DOXYGEN__)
PWMDriver PWMD1;
#endif

/**
 * @brief   PWMD2 driver identifier.
 * @note    The driver PWMD2 allocates the timer TIM2 when enabled.
 */
#if SPC5_PWM_USE_SMOD1 || defined(__DOXYGEN__)
PWMDriver PWMD2;
#endif

/**
 * @brief   PWMD3 driver identifier.
 * @note    The driver PWMD3 allocates the timer TIM3 when enabled.
 */
#if SPC5_PWM_USE_SMOD2 || defined(__DOXYGEN__)
PWMDriver PWMD3;
#endif

/**
 * @brief   PWMD4 driver identifier.
 * @note    The driver PWMD4 allocates the timer TIM4 when enabled.
 */
#if SPC5_PWM_USE_SMOD3 || defined(__DOXYGEN__)
PWMDriver PWMD4;
#endif

/*===========================================================================*/
/* Driver local variables.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

/**
 * @brief   Configures and activates the PWM peripheral submodule.
 *
 * @param[in] pwmp      pointer to a @p PWMDriver object
 * @param[in] sid    PWM submodule identifier
 *
 * @notapi
 */
void pwm_lld_start_submodule(PWMDriver *pwmp, uint8_t sid) {

  pwmcnt_t pwmperiod;
  uint32_t psc;

  /* Clears Status Register.*/
  pwmp->flexpwmp->SUB[sid].STS.R = 0xFFFF;

  /* Clears LDOK and initializes the registers.*/
  pwmp->flexpwmp->MCTRL.B.CLDOK |= (0b0000 | (1U << sid));
  pwmp->flexpwmp->SUB[sid].OCTRL.R = 0x0000;
  pwmp->flexpwmp->SUB[sid].INTEN.R = 0x0000;

  /* Setting PWM clock frequency and submodule prescaler.*/
  psc = (SPC5_FLEXPWM0_CLK / pwmp->config->frequency);
  chDbgAssert((psc <= 0xFFFF) &&
      (((psc) * pwmp->config->frequency) == SPC5_FLEXPWM0_CLK) &&
      ((psc == 1) || (psc == 2) || (psc == 4) || (psc == 8) ||
          (psc == 16) || (psc == 32) ||
          (psc == 64) || (psc == 128)),
      "icu_lld_start(), #1", "invalid frequency");
  switch(psc) {
    case 1:
    pwmp->flexpwmp->SUB[sid].CTRL.B.PRSC = 0b000;
    break;
    case 2:
    pwmp->flexpwmp->SUB[sid].CTRL.B.PRSC = 0b001;
    break;
    case 4:
    pwmp->flexpwmp->SUB[sid].CTRL.B.PRSC = 0b010;
    break;
    case 8:
    pwmp->flexpwmp->SUB[sid].CTRL.B.PRSC = 0b011;
    break;
    case 16:
    pwmp->flexpwmp->SUB[sid].CTRL.B.PRSC = 0b100;
    break;
    case 32:
    pwmp->flexpwmp->SUB[sid].CTRL.B.PRSC = 0b101;
    break;
    case 64:
    pwmp->flexpwmp->SUB[sid].CTRL.B.PRSC = 0b110;
    break;
    case 128:
    pwmp->flexpwmp->SUB[sid].CTRL.B.PRSC = 0b111;
    break;
  }

  /* Disables PWM FAULT function. */
  pwmp->flexpwmp->SUB[sid].DISMAP.R = 0x0000;
  pwmp->flexpwmp->SUB[sid].CTRL2.R = 0x0000;
  pwmp->flexpwmp->SUB[sid].CTRL2.B.INDEP = 1;

  /* Sets PWM period.*/
  pwmperiod = pwmp->period;
  pwmp->flexpwmp->SUB[sid].INIT.R = ~(pwmperiod / 2) + 1U;
  pwmp->flexpwmp->SUB[sid].VAL[0].R = 0x0000;
  pwmp->flexpwmp->SUB[sid].VAL[1].R = pwmperiod / 2;

  /* Sets the submodule channels.*/
  switch (pwmp->config->mode & PWM_OUTPUT_MASK) {
    case EDGE_ALIGNED_PWM:
    /* Setting reloads.*/
    pwmp->flexpwmp->SUB[sid].CTRL.B.HALF = 0;
    pwmp->flexpwmp->SUB[sid].CTRL.B.FULL = 1;

    /* Setting active front of PWM channels.*/
    pwmp->flexpwmp->SUB[sid].VAL[2].R = ~(pwmperiod / 2) + 1U;
    pwmp->flexpwmp->SUB[sid].VAL[4].R = ~(pwmperiod / 2) + 1U;
    break;
    case CENTER_ALIGNED_PWM:
    /* Setting reloads.*/
    pwmp->flexpwmp->SUB[sid].CTRL.B.HALF = 1;
    pwmp->flexpwmp->SUB[sid].CTRL.B.FULL = 0;
    break;
    default:
    ;
  }

  /* Polarities setup.*/
  switch (pwmp->config->channels[0].mode & PWM_OUTPUT_MASK) {
    case PWM_OUTPUT_ACTIVE_LOW:
    pwmp->flexpwmp->SUB[sid].OCTRL.B.POLA = 1;
    /* Enables CHA mask.*/
    pwmp->flexpwmp->MASK.B.MASKA |= (0b0000 | (1U << sid));
    /* Enables CHA.*/
    pwmp->flexpwmp->OUTEN.B.PWMA_EN |= (0b0000 | (1U << sid));
    break;
    case PWM_OUTPUT_ACTIVE_HIGH:
    pwmp->flexpwmp->SUB[sid].OCTRL.B.POLA = 0;
    /* Enables CHA mask.*/
    pwmp->flexpwmp->MASK.B.MASKA |= (0b0000 | (1U << sid));
    /* Enables CHA.*/
    pwmp->flexpwmp->OUTEN.B.PWMA_EN |= (0b0000 | (1U << sid));
    break;
    case PWM_OUTPUT_DISABLED:
    /* Enables CHA mask.*/
    pwmp->flexpwmp->MASK.B.MASKA |= (0b0000 | (1U << sid));
    break;
    default:
    ;
  }
  switch (pwmp->config->channels[1].mode & PWM_OUTPUT_MASK) {
    case PWM_OUTPUT_ACTIVE_LOW:
    pwmp->flexpwmp->SUB[sid].OCTRL.B.POLB = 1;
    /* Enables CHB mask.*/
    pwmp->flexpwmp->MASK.B.MASKB |= (0b0000 | (1U << sid));
    /* Enables CHB.*/
    pwmp->flexpwmp->OUTEN.B.PWMB_EN |= (0b0000 | (1U << sid));
    break;
    case PWM_OUTPUT_ACTIVE_HIGH:
    pwmp->flexpwmp->SUB[sid].OCTRL.B.POLB = 0;
    /* Enables CHB mask.*/
    pwmp->flexpwmp->MASK.B.MASKB |= (0b0000 | (1U << sid));
    /* Enables CHB.*/
    pwmp->flexpwmp->OUTEN.B.PWMB_EN |= (0b0000 | (1U << sid));
    break;
    case PWM_OUTPUT_DISABLED:
    /* Enables CHB mask.*/
    pwmp->flexpwmp->MASK.B.MASKB |= (0b0000 | (1U << sid));
    break;
    default:
    ;
  }

  /* Complementary output setup.*/
  /*  switch (pwmp->config->channels[0].mode & PWM_COMPLEMENTARY_OUTPUT_MASK) {
   case PWM_COMPLEMENTARY_OUTPUT_ACTIVE_LOW:
   chDbgAssert(pwmp->config->channels[1].mode == PWM_OUTPUT_ACTIVE_LOW,
   "pwm_lld_start(), #1",
   "the PWM chB must be set in PWM_OUTPUT_ACTIVE_LOW");
   //pwmp->flexpwmp->SUB[sid].OCTRL.B.POLA = 1;
   pwmp->flexpwmp->SUB[sid].CTRL2.B.INDEP = 0;
   pwmp->flexpwmp->MCTRL.B.IPOL |= (0b0000 | (1U << sid));
   pwmp->flexpwmp->MASK.B.MASKA |= (0b0000 | (1U << sid));
   pwmp->flexpwmp->OUTEN.B.PWMA_EN |= (0b0000 | (1U << sid));
   //pwmp->flexpwmp->SUB[0].OCTRL.B.POLB = 0;
   break;
   case PWM_COMPLEMENTARY_OUTPUT_ACTIVE_HIGH:
   chDbgAssert(pwmp->config->channels[1].mode == PWM_OUTPUT_ACTIVE_HIGH,
   "pwm_lld_start(), #2",
   "the PWM chB must be set in PWM_OUTPUT_ACTIVE_HIGH");
   pwmp->flexpwmp->SUB[sid].CTRL2.B.INDEP = 0;
   pwmp->flexpwmp->MCTRL.B.IPOL |= (0b0000 | (0U << sid));
   pwmp->flexpwmp->MASK.B.MASKA |= (0b0000 | (1U << sid));
   pwmp->flexpwmp->OUTEN.B.PWMA_EN |= (0b0000 | (1U << sid));
   // pwmp->flexpwmp->SUB[0].OCTRL.B.POLA = 0;
   //pwmp->flexpwmp->SUB[0].OCTRL.B.POLB = 1;
   break;
   default:
   ;
   }

   switch (pwmp->config->channels[1].mode & PWM_COMPLEMENTARY_OUTPUT_MASK) {
   case PWM_COMPLEMENTARY_OUTPUT_ACTIVE_LOW:
   chDbgAssert(pwmp->config->channels[0].mode == PWM_OUTPUT_ACTIVE_LOW,
   "pwm_lld_start(), #3",
   "the PWM chA must be set in PWM_OUTPUT_ACTIVE_LOW");
   pwmp->flexpwmp->SUB[sid].CTRL2.B.INDEP = 0;
   pwmp->flexpwmp->MCTRL.B.IPOL &= ~ (0b0000 | (1U << sid));
   //  pwmp->flexpwmp->SUB[0].OCTRL.B.POLA = 0;
   pwmp->flexpwmp->SUB[sid].OCTRL.B.POLB = 1;
   pwmp->flexpwmp->MASK.B.MASKB |= (0b0000 | (1U << sid));
   pwmp->flexpwmp->OUTEN.B.PWMB_EN |= (0b0000 | (1U << sid));
   break;
   case PWM_COMPLEMENTARY_OUTPUT_ACTIVE_HIGH:
   chDbgAssert(pwmp->config->channels[0].mode == PWM_OUTPUT_ACTIVE_HIGH,
   "pwm_lld_start(), #4",
   "the PWM chA must be set in PWM_OUTPUT_ACTIVE_HIGH");
   pwmp->flexpwmp->SUB[sid].CTRL2.B.INDEP = 0;
   pwmp->flexpwmp->MCTRL.B.IPOL &= ~ (0b0000 | (1U << sid));

   pwmp->flexpwmp->MASK.B.MASKB |= (0b0000 | (1U << sid));
   pwmp->flexpwmp->OUTEN.B.PWMB_EN |= (0b0000 | (1U << sid));
   // pwmp->flexpwmp->SUB[0].OCTRL.B.POLA = 1;
   // pwmp->flexpwmp->SUB[0].OCTRL.B.POLB = 0;
   break;
   default:
   ;
   }
   */

  /* Sets the INIT and MASK registers.*/
  pwmp->flexpwmp->SUB[sid].CTRL2.B.FRCEN = 1U;
  pwmp->flexpwmp->SUB[sid].CTRL2.B.FORCE_SEL = 0b000;
  pwmp->flexpwmp->SUB[sid].CTRL2.B.FORCE = 1U;

  /* Updates SMOD registers and starts SMOD.*/
  pwmp->flexpwmp->MCTRL.B.LDOK |= (0b0000 | (1U << sid));
  pwmp->flexpwmp->MCTRL.B.RUN |= (0b0000 | (1U << sid));
}

/**
 * @brief   Enables a PWM channel of a submodule.
 *
 * @param[in] pwmp      pointer to a @p PWMDriver object
 * @param[in] channel   PWM channel identifier (0...PWM_CHANNELS-1)
 * @param[in] width     PWM pulse width as clock pulses number
 * @param[in] sid       PWM submodule id
 *
 * @notapi
 */
void pwm_lld_enable_submodule_channel(PWMDriver *pwmp,
    pwmchannel_t channel,
    pwmcnt_t width, uint8_t sid) {

  pwmcnt_t pwmperiod;
  int16_t nwidth;
  pwmperiod = pwmp->period;
  nwidth = width - (pwmperiod / 2);

  /* Clears LDOK.*/
  pwmp->flexpwmp->MCTRL.B.CLDOK |= (0b0000 | (1U << sid));

  /* Active the width interrupt.*/
  if (channel == 0) {
    if (pwmp->config->channels[0].callback != NULL) {
      if ((pwmp->flexpwmp->SUB[sid].INTEN.B.CMPIE & 0b001000) == 0) {
        pwmp->flexpwmp->SUB[sid].INTEN.B.CMPIE |= 0b001000;
      }
    }

    /* Sets the channel width.*/
    switch (pwmp->config->mode & PWM_OUTPUT_MASK) {
      case EDGE_ALIGNED_PWM:
      if(nwidth >= 0)
      pwmp->flexpwmp->SUB[sid].VAL[3].R = nwidth;
      else
      pwmp->flexpwmp->SUB[sid].VAL[3].R = ~((pwmperiod / 2) - width) + 1U;
      break;
      case CENTER_ALIGNED_PWM:
      pwmp->flexpwmp->SUB[sid].VAL[3].R = width / 2;
      pwmp->flexpwmp->SUB[sid].VAL[2].R = ~(width / 2) + 1U;
      break;
      default:
      ;
    }

    /* Removes the channel mask if it is necessary.*/
    if ((pwmp->flexpwmp->MASK.B.MASKA & (0b0000 | (1U << sid))) == 1)
    pwmp->flexpwmp->MASK.B.MASKA &= ~ (0b0000 | (1U << sid));
  }
  /* Active the width interrupt.*/
  else if (channel == 1) {
    if (pwmp->config->channels[1].callback != NULL) {
      if ((pwmp->flexpwmp->SUB[sid].INTEN.B.CMPIE & 0b100000) == 0) {
        pwmp->flexpwmp->SUB[sid].INTEN.B.CMPIE |= 0b100000;
      }
    }
    /* Sets the channel width.*/
    switch (pwmp->config->mode & PWM_OUTPUT_MASK) {
      case EDGE_ALIGNED_PWM:
      if(nwidth >= 0)
      pwmp->flexpwmp->SUB[sid].VAL[5].R = nwidth;
      else
      pwmp->flexpwmp->SUB[sid].VAL[5].R = ~((pwmperiod / 2) - width) + 1U;
      break;
      case CENTER_ALIGNED_PWM:
      pwmp->flexpwmp->SUB[sid].VAL[5].R = width / 2;
      pwmp->flexpwmp->SUB[sid].VAL[4].R = ~(width / 2) + 1U;
      break;
      default:
      ;
    }

    /* Removes the channel mask if it is necessary.*/
    if ((pwmp->flexpwmp->MASK.B.MASKB & (0b0000 | (1U << sid))) == 1)
    pwmp->flexpwmp->MASK.B.MASKB &= ~ (0b0000 | (1U << sid));
  }

  /* Active the periodic interrupt.*/
  if (pwmp->flexpwmp->SUB[sid].INTEN.B.RIE != 1U) {
    if (pwmp->config->callback != NULL) {
      pwmp->flexpwmp->SUB[sid].INTEN.B.RIE = 1;
    }
  }

  /* Sets the MASK registers.*/
  pwmp->flexpwmp->SUB[sid].CTRL2.B.FRCEN = 1U;
  pwmp->flexpwmp->SUB[sid].CTRL2.B.FORCE_SEL = 0b000;
  pwmp->flexpwmp->SUB[sid].CTRL2.B.FORCE = 1U;

  /* Forces reload of the VALUE registers.*/
  pwmp->flexpwmp->MCTRL.B.LDOK |= (0b0000 | (1U << sid));
}

/**
 * @brief   Disables a PWM channel of a submodule.
 *
 * @param[in] pwmp      pointer to a @p PWMDriver object
 * @param[in] channel   PWM channel identifier (0...PWM_CHANNELS-1)
 * @param[in] sid       PWM submodule id
 *
 * @notapi
 */
void pwm_lld_disable_submodule_channel(PWMDriver *pwmp,
    pwmchannel_t channel,
    uint8_t sid) {

  pwmp->flexpwmp->MCTRL.B.CLDOK |= (0b0000 | (1U << sid));

  /* Disable the width interrupt.*/
  if (channel == 0) {
    if (pwmp->config->channels[0].callback != NULL) {
      if ((pwmp->flexpwmp->SUB[sid].INTEN.B.CMPIE & 0b001000) == 1) {
        pwmp->flexpwmp->SUB[sid].INTEN.B.CMPIE &= 0b110111;
      }
    }

    /* Active the channel mask.*/
    pwmp->flexpwmp->MASK.B.MASKA |= (0b0000 | (1U << sid));
  }
  /* Disable the width interrupt.*/
  else if (channel == 1) {
    if (pwmp->config->channels[1].callback != NULL) {
      if ((pwmp->flexpwmp->SUB[sid].INTEN.B.CMPIE & 0b100000) == 1) {
        pwmp->flexpwmp->SUB[sid].INTEN.B.CMPIE &= 0b011111;
      }
    }

    /* Active the channel mask.*/
    pwmp->flexpwmp->MASK.B.MASKB |= (0b0000 | (1U << sid));
  }

  /* Sets the MASK registers.*/
  pwmp->flexpwmp->SUB[sid].CTRL2.B.FRCEN = 1U;
  pwmp->flexpwmp->SUB[sid].CTRL2.B.FORCE_SEL = 0b000;
  pwmp->flexpwmp->SUB[sid].CTRL2.B.FORCE = 1U;

  /* Disable RIE interrupt to prevent reload interrupt.*/
  if((pwmp->flexpwmp->MASK.B.MASKA & (0b0000 | (1U << sid))) &&
      (pwmp->flexpwmp->MASK.B.MASKB & (0b0000 | (1U << sid))) == 1) {
    pwmp->flexpwmp->SUB[sid].INTEN.B.RIE = 0;
    /* Clear the reload flag.*/
    pwmp->flexpwmp->SUB[sid].STS.B.RF = 1U;
  }

  pwmp->flexpwmp->MCTRL.B.LDOK |= (0b0000 | (1U << sid));
}

#if SPC5_PWM_USE_SMOD0 || SPC5_PWM_USE_SMOD1 || SPC5_PWM_USE_SMOD2 ||         \
    SPC5_PWM_USE_SMOD3 || defined(__DOXYGEN__)
/**
 * @brief   Common SMOD0...SMOD3 IRQ handler.
 * @note    It is assumed that the various sources are only activated if the
 *          associated callback pointer is not equal to @p NULL in order to not
 *          perform an extra check in a potentially critical interrupt handler.
 *
 * @param[in] pwmp      pointer to a @p PWMDriver object
 */
static void pwm_lld_serve_interrupt(PWMDriver *pwmp) {

  uint16_t sr;
#if SPC5_PWM_USE_SMOD0
  if (&PWMD1 == pwmp) {
    sr = pwmp->flexpwmp->SUB[0].STS.R & pwmp->flexpwmp->SUB[0].INTEN.R;
    if ((sr & SPC5_STS_CMPF3) != 0) {
      pwmp->flexpwmp->SUB[0].STS.B.CMPF |= 0b001000;
      pwmp->config->channels[0].callback(pwmp);
    }
    if ((sr & SPC5_STS_CMPF5) != 0) {
      pwmp->flexpwmp->SUB[0].STS.B.CMPF |= 0b100000;
      pwmp->config->channels[1].callback(pwmp);
    }
    if ((sr & SPC5_STS_RF) != 0) {
      pwmp->flexpwmp->SUB[0].STS.B.RF = 1U;
      pwmp->config->callback(pwmp);
    }
  }
#endif
#if SPC5_PWM_USE_SMOD1
  if (&PWMD2 == pwmp) {
    sr = pwmp->flexpwmp->SUB[1].STS.R & pwmp->flexpwmp->SUB[1].INTEN.R;
    if ((sr & SPC5_STS_CMPF3) != 0) {
      pwmp->flexpwmp->SUB[1].STS.B.CMPF |= 0b001000;
      pwmp->config->channels[0].callback(pwmp);
    }
    if ((sr & SPC5_STS_CMPF5) != 0) {
      pwmp->flexpwmp->SUB[1].STS.B.CMPF |= 0b100000;
      pwmp->config->channels[1].callback(pwmp);
    }
    if ((sr & SPC5_STS_RF) != 0) {
      pwmp->flexpwmp->SUB[1].STS.B.RF = 1U;
      pwmp->config->callback(pwmp);
    }
  }
#endif
#if SPC5_PWM_USE_SMOD2
  if (&PWMD3 == pwmp) {
    sr = pwmp->flexpwmp->SUB[2].STS.R & pwmp->flexpwmp->SUB[2].INTEN.R;
    if ((sr & SPC5_STS_CMPF3) != 0) {
      pwmp->flexpwmp->SUB[2].STS.B.CMPF |= 0b001000;
      pwmp->config->channels[0].callback(pwmp);
    }
    if ((sr & SPC5_STS_CMPF5) != 0) {
      pwmp->flexpwmp->SUB[2].STS.B.CMPF |= 0b100000;
      pwmp->config->channels[1].callback(pwmp);
    }
    if ((sr & SPC5_STS_RF) != 0) {
      pwmp->flexpwmp->SUB[2].STS.B.RF = 1U;
      pwmp->config->callback(pwmp);
    }
  }
#endif
#if SPC5_PWM_USE_SMOD3
  if (&PWMD4 == pwmp) {
    sr = pwmp->flexpwmp->SUB[3].STS.R & pwmp->flexpwmp->SUB[3].INTEN.R;
    if ((sr & SPC5_STS_CMPF3) != 0) {
      pwmp->flexpwmp->SUB[3].STS.B.CMPF |= 0b001000;
      pwmp->config->channels[0].callback(pwmp);
    }
    if ((sr & SPC5_STS_CMPF5) != 0) {
      pwmp->flexpwmp->SUB[3].STS.B.CMPF |= 0b100000;
      pwmp->config->channels[1].callback(pwmp);
    }
    if ((sr & SPC5_STS_RF) != 0) {
      pwmp->flexpwmp->SUB[3].STS.B.RF = 1U;
      pwmp->config->callback(pwmp);
    }
  }
#endif
}
#endif /* SPC5_PWM_USE_SMOD0 || ... || SPC5_PWM_USE_SMOD3 */

/*===========================================================================*/
/* Driver interrupt handlers.                                                */
/*===========================================================================*/

#if SPC5_PWM_USE_SMOD0 || defined(__DOXYGEN__)
#if !defined(SPC5_FLEXPWM0_RF0_HANDLER)
#error "SPC5_FLEXPWM0_RF0_HANDLER not defined"
#endif
/**
 * @brief   FlexPWM0-SMOD0 RF0 interrupt handler.
 *
 * @isr
 */
CH_IRQ_HANDLER(SPC5_FLEXPWM0_RF0_HANDLER) {

  CH_IRQ_PROLOGUE();

  pwm_lld_serve_interrupt(&PWMD1);

  CH_IRQ_EPILOGUE();
}

#if !defined(SPC5_FLEXPWM0_COF0_HANDLER)
#error "SPC5_FLEXPWM0_COF0_HANDLER not defined"
#endif
/**
 * @brief   FlexPWM0-SMOD0 COF0 interrupt handler.
 *
 * @isr
 */
CH_IRQ_HANDLER(SPC5_FLEXPWM0_COF0_HANDLER) {

  CH_IRQ_PROLOGUE();

  pwm_lld_serve_interrupt(&PWMD1);

  CH_IRQ_EPILOGUE();
}
#endif

#if SPC5_PWM_USE_SMOD1 || defined(__DOXYGEN__)
#if !defined(SPC5_FLEXPWM0_RF1_HANDLER)
#error "SPC5_FLEXPWM0_RF1_HANDLER not defined"
#endif
/**
 * @brief   FlexPWM0-SMOD1 RF1 interrupt handler.
 *
 * @isr
 */
CH_IRQ_HANDLER(SPC5_FLEXPWM0_RF1_HANDLER) {

  CH_IRQ_PROLOGUE();

  pwm_lld_serve_interrupt(&PWMD2);

  CH_IRQ_EPILOGUE();
}

#if !defined(SPC5_FLEXPWM0_COF1_HANDLER)
#error "SPC5_FLEXPWM0_COF1_HANDLER not defined"
#endif
/**
 * @brief   FlexPWM0-SMOD1 COF1 interrupt handler.
 *
 * @isr
 */
CH_IRQ_HANDLER(SPC5_FLEXPWM0_COF1_HANDLER) {

  CH_IRQ_PROLOGUE();

  pwm_lld_serve_interrupt(&PWMD2);

  CH_IRQ_EPILOGUE();
}
#endif

#if SPC5_PWM_USE_SMOD2 || defined(__DOXYGEN__)
#if !defined(SPC5_FLEXPWM0_RF2_HANDLER)
#error "SPC5_FLEXPWM0_RF2_HANDLER not defined"
#endif
/**
 * @brief   FlexPWM0-SMOD2 RF2 interrupt handler.
 *
 * @isr
 */
CH_IRQ_HANDLER(SPC5_FLEXPWM0_RF2_HANDLER) {

  CH_IRQ_PROLOGUE();

  pwm_lld_serve_interrupt(&PWMD3);

  CH_IRQ_EPILOGUE();
}

#if !defined(SPC5_FLEXPWM0_COF2_HANDLER)
#error "SPC5_FLEXPWM0_COF2_HANDLER not defined"
#endif
/**
 * @brief   FlexPWM0-SMOD2 COF2 interrupt handler.
 *
 * @isr
 */
CH_IRQ_HANDLER(SPC5_FLEXPWM0_COF2_HANDLER) {

  CH_IRQ_PROLOGUE();

  pwm_lld_serve_interrupt(&PWMD3);

  CH_IRQ_EPILOGUE();
}
#endif

#if SPC5_PWM_USE_SMOD3 || defined(__DOXYGEN__)
#if !defined(SPC5_FLEXPWM0_RF3_HANDLER)
#error "SPC5_FLEXPWM0_RF3_HANDLER not defined"
#endif
/**
 * @brief   FlexPWM0-SMOD1 RF3 interrupt handler.
 *
 * @isr
 */
CH_IRQ_HANDLER(SPC5_FLEXPWM0_RF3_HANDLER) {

  CH_IRQ_PROLOGUE();

  pwm_lld_serve_interrupt(&PWMD4);

  CH_IRQ_EPILOGUE();
}

#if !defined(SPC5_FLEXPWM0_COF3_HANDLER)
#error "SPC5_FLEXPWM0_COF3_HANDLER not defined"
#endif
/**
 * @brief   FlexPWM0-SMOD1 COF3 interrupt handler.
 *
 * @isr
 */
CH_IRQ_HANDLER(SPC5_FLEXPWM0_COF3_HANDLER) {

  CH_IRQ_PROLOGUE();

  pwm_lld_serve_interrupt(&PWMD4);

  CH_IRQ_EPILOGUE();
}
#endif

/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

/**
 * @brief   Low level PWM driver initialization.
 *
 * @notapi
 */
void pwm_lld_init(void) {

#if (SPC5_PWM_USE_SMOD0)
  /* Driver initialization.*/
  pwmObjectInit(&PWMD1);
  PWMD1.flexpwmp = &FLEXPWM_0;
  INTC.PSR[SPC5_FLEXPWM0_RF0_NUMBER].R = SPC5_PWM_SMOD0_PRIORITY;
  INTC.PSR[SPC5_FLEXPWM0_COF0_NUMBER].R = SPC5_PWM_SMOD0_PRIORITY;
  INTC.PSR[SPC5_FLEXPWM0_CAF0_NUMBER].R = SPC5_PWM_SMOD0_PRIORITY;
  INTC.PSR[SPC5_FLEXPWM0_FFLAG_NUMBER].R = SPC5_PWM_SMOD0_PRIORITY;
  INTC.PSR[SPC5_FLEXPWM0_REF_NUMBER].R = SPC5_PWM_SMOD0_PRIORITY;
#endif

#if (SPC5_PWM_USE_SMOD1)
  /* Driver initialization.*/
  pwmObjectInit(&PWMD2);
  PWMD2.flexpwmp = &FLEXPWM_0;
  INTC.PSR[SPC5_FLEXPWM0_RF1_NUMBER].R = SPC5_PWM_SMOD1_PRIORITY;
  INTC.PSR[SPC5_FLEXPWM0_COF1_NUMBER].R = SPC5_PWM_SMOD1_PRIORITY;
  INTC.PSR[SPC5_FLEXPWM0_CAF1_NUMBER].R = SPC5_PWM_SMOD1_PRIORITY;
  INTC.PSR[SPC5_FLEXPWM0_FFLAG_NUMBER].R = SPC5_PWM_SMOD1_PRIORITY;
  INTC.PSR[SPC5_FLEXPWM0_REF_NUMBER].R = SPC5_PWM_SMOD1_PRIORITY;
#endif

#if (SPC5_PWM_USE_SMOD2)
  /* Driver initialization.*/
  pwmObjectInit(&PWMD3);
  PWMD3.flexpwmp = &FLEXPWM_0;
  INTC.PSR[SPC5_FLEXPWM0_RF2_NUMBER].R = SPC5_PWM_SMOD2_PRIORITY;
  INTC.PSR[SPC5_FLEXPWM0_COF2_NUMBER].R = SPC5_PWM_SMOD2_PRIORITY;
  INTC.PSR[SPC5_FLEXPWM0_CAF2_NUMBER].R = SPC5_PWM_SMOD2_PRIORITY;
  INTC.PSR[SPC5_FLEXPWM0_FFLAG_NUMBER].R = SPC5_PWM_SMOD2_PRIORITY;
  INTC.PSR[SPC5_FLEXPWM0_REF_NUMBER].R = SPC5_PWM_SMOD2_PRIORITY;
#endif

#if (SPC5_PWM_USE_SMOD3)
  /* Driver initialization.*/
  pwmObjectInit(&PWMD4);
  PWMD4.flexpwmp = &FLEXPWM_0;
  INTC.PSR[SPC5_FLEXPWM0_RF3_NUMBER].R = SPC5_PWM_SMOD3_PRIORITY;
  INTC.PSR[SPC5_FLEXPWM0_COF3_NUMBER].R = SPC5_PWM_SMOD3_PRIORITY;
  INTC.PSR[SPC5_FLEXPWM0_CAF3_NUMBER].R = SPC5_PWM_SMOD3_PRIORITY;
  INTC.PSR[SPC5_FLEXPWM0_FFLAG_NUMBER].R = SPC5_PWM_SMOD3_PRIORITY;
  INTC.PSR[SPC5_FLEXPWM0_REF_NUMBER].R = SPC5_PWM_SMOD3_PRIORITY;
#endif
}

/**
 * @brief   Configures and activates the PWM peripheral.
 * @note    Starting a driver that is already in the @p PWM_READY state
 *          disables all the active channels.
 *
 * @param[in] pwmp      pointer to a @p PWMDriver object
 *
 * @notapi
 */
void pwm_lld_start(PWMDriver *pwmp) {

  if (pwmp->state == PWM_STOP) {
    uint8_t SMOD0 = 0;
    uint8_t SMOD1 = 0;
    uint8_t SMOD2 = 0;
    uint8_t SMOD3 = 0;

#if SPC5_PWM_USE_SMOD0
    if (PWMD1.state == PWM_READY)
    SMOD0 = 1U;
#endif
#if SPC5_PWM_USE_SMOD1
    if (PWMD2.state == PWM_READY)
    SMOD1 = 1U;
#endif
#if SPC5_PWM_USE_SMOD2
    if (PWMD3.state == PWM_READY)
    SMOD2 = 1U;
#endif
#if SPC5_PWM_USE_SMOD3
    if (PWMD4.state == PWM_READY)
    SMOD3 = 1U;
#endif

    /* Set Peripheral Clock.*/
    if(!(SMOD0 || SMOD1 || SMOD2 || SMOD3)) {
      halSPCSetPeripheralClockMode(SPC5_FLEXPWM0_PCTL,
          SPC5_PWM_FLEXPWM0_START_PCTL);
    }

#if SPC5_PWM_USE_SMOD0
    if (&PWMD1 == pwmp) {
      pwm_lld_start_submodule(pwmp, 0);
    }
#endif
#if SPC5_PWM_USE_SMOD1
    if (&PWMD2 == pwmp) {
      pwm_lld_start_submodule(pwmp, 1);
    }
#endif
#if SPC5_PWM_USE_SMOD2
    if (&PWMD3 == pwmp) {
      pwm_lld_start_submodule(pwmp, 2);
    }
#endif
#if SPC5_PWM_USE_SMOD3
    if (&PWMD4 == pwmp) {
      pwm_lld_start_submodule(pwmp, 3);
    }
#endif
  }
  else {
    /* Driver re-configuration scenario, it must be stopped first.*/
#if SPC5_PWM_USE_SMOD0
    if (&PWMD1 == pwmp) {
      /* Disable the interrupts.*/
      pwmp->flexpwmp->SUB[0].INTEN.R = 0x0000;

      /* Disable the submodule.*/
      pwmp->flexpwmp->OUTEN.B.PWMA_EN &= 0b1110;
      pwmp->flexpwmp->OUTEN.B.PWMB_EN &= 0b1110;

      /* Active the submodule masks.*/
      pwmp->flexpwmp->MASK.B.MASKA &= 0b1110;
      pwmp->flexpwmp->MASK.B.MASKB &= 0b1110;

      /* Sets the MASK registers.*/
      pwmp->flexpwmp->SUB[0].CTRL2.B.FRCEN = 1U;
      pwmp->flexpwmp->SUB[0].CTRL2.B.FORCE_SEL = 0b000;
      pwmp->flexpwmp->SUB[0].CTRL2.B.FORCE = 1U;
    }
#endif
#if SPC5_PWM_USE_SMOD1
    if (&PWMD2 == pwmp) {
      /* Disable the interrupts.*/
      pwmp->flexpwmp->SUB[1].INTEN.R = 0x0000;

      /* Disable the submodule.*/
      pwmp->flexpwmp->OUTEN.B.PWMA_EN &= 0b1101;
      pwmp->flexpwmp->OUTEN.B.PWMB_EN &= 0b1101;

      /* Active the submodule masks.*/
      pwmp->flexpwmp->MASK.B.MASKA &= 0b1101;
      pwmp->flexpwmp->MASK.B.MASKB &= 0b1101;

      /* Sets the MASK registers.*/
      pwmp->flexpwmp->SUB[1].CTRL2.B.FRCEN = 1U;
      pwmp->flexpwmp->SUB[1].CTRL2.B.FORCE_SEL = 0b000;
      pwmp->flexpwmp->SUB[1].CTRL2.B.FORCE = 1U;
    }
#endif
#if SPC5_PWM_USE_SMOD2
    if (&PWMD3 == pwmp) {
      /* Disable the interrupts.*/
      pwmp->flexpwmp->SUB[2].INTEN.R = 0x0000;

      /* Disable the submodule.*/
      pwmp->flexpwmp->OUTEN.B.PWMA_EN &= 0b1011;
      pwmp->flexpwmp->OUTEN.B.PWMB_EN &= 0b1011;

      /* Active the submodule masks.*/
      pwmp->flexpwmp->MASK.B.MASKA &= 0b1011;
      pwmp->flexpwmp->MASK.B.MASKB &= 0b1011;

      /* Sets the MASK registers.*/
      pwmp->flexpwmp->SUB[2].CTRL2.B.FRCEN = 1U;
      pwmp->flexpwmp->SUB[2].CTRL2.B.FORCE_SEL = 0b000;
      pwmp->flexpwmp->SUB[2].CTRL2.B.FORCE = 1U;
    }
#endif
#if SPC5_PWM_USE_SMOD3
    if (&PWMD4 == pwmp) {
      /* Disable the interrupts.*/
      pwmp->flexpwmp->SUB[3].INTEN.R = 0x0000;

      /* Disable the submodule.*/
      pwmp->flexpwmp->OUTEN.B.PWMA_EN &= 0b0111;
      pwmp->flexpwmp->OUTEN.B.PWMB_EN &= 0b0111;

      /* Active the submodule masks.*/
      pwmp->flexpwmp->MASK.B.MASKA &= 0b0111;
      pwmp->flexpwmp->MASK.B.MASKB &= 0b0111;

      /* Sets the MASK registers.*/
      pwmp->flexpwmp->SUB[3].CTRL2.B.FRCEN = 1U;
      pwmp->flexpwmp->SUB[3].CTRL2.B.FORCE_SEL = 0b000;
      pwmp->flexpwmp->SUB[3].CTRL2.B.FORCE = 1U;
    }
#endif
  }
}

/**
 * @brief   Deactivates the PWM peripheral.
 *
 * @param[in] pwmp      pointer to a @p PWMDriver object
 *
 * @notapi
 */
void pwm_lld_stop(PWMDriver *pwmp) {

  /* If in ready state then disables the PWM clock.*/
  if (pwmp->state == PWM_READY) {
#if SPC5_PWM_USE_SMOD0
    if (&PWMD1 == pwmp) {
      /* SMOD stop.*/
      pwmp->flexpwmp->MCTRL.B.CLDOK |= 0b0001;
      pwmp->flexpwmp->SUB[0].INTEN.R = 0x0000;
      pwmp->flexpwmp->SUB[0].STS.R = 0xFFFF;
      pwmp->flexpwmp->OUTEN.B.PWMA_EN &= 0b1110;
      pwmp->flexpwmp->OUTEN.B.PWMB_EN &= 0b1110;

      pwmp->flexpwmp->MCTRL.B.RUN &= 0b1110;
    }
#endif
#if SPC5_PWM_USE_SMOD1
    if (&PWMD2 == pwmp) {
      /* SMOD stop.*/
      pwmp->flexpwmp->MCTRL.B.CLDOK |= 0b0010;
      pwmp->flexpwmp->SUB[1].INTEN.R = 0x0000;
      pwmp->flexpwmp->SUB[1].STS.R = 0xFFFF;
      pwmp->flexpwmp->OUTEN.B.PWMA_EN &= 0b1101;
      pwmp->flexpwmp->OUTEN.B.PWMB_EN &= 0b1101;

      pwmp->flexpwmp->MCTRL.B.RUN &= 0b1101;
    }
#endif
#if SPC5_PWM_USE_SMOD2
    if (&PWMD3 == pwmp) {
      /* SMOD stop.*/
      pwmp->flexpwmp->MCTRL.B.CLDOK |= 0b0100;
      pwmp->flexpwmp->SUB[2].INTEN.R = 0x0000;
      pwmp->flexpwmp->SUB[2].STS.R = 0xFFFF;
      pwmp->flexpwmp->OUTEN.B.PWMA_EN &= 0b1011;
      pwmp->flexpwmp->OUTEN.B.PWMB_EN &= 0b1011;

      pwmp->flexpwmp->MCTRL.B.RUN &= 0b1011;
    }
#endif
#if SPC5_PWM_USE_SMOD3
    if (&PWMD4 == pwmp) {
      /* SMOD stop.*/
      pwmp->flexpwmp->MCTRL.B.CLDOK |= 0b1000;
      pwmp->flexpwmp->SUB[3].INTEN.R = 0x0000;
      pwmp->flexpwmp->SUB[3].STS.R = 0xFFFF;
      pwmp->flexpwmp->OUTEN.B.PWMA_EN &= 0b0111;
      pwmp->flexpwmp->OUTEN.B.PWMB_EN &= 0b0111;

      pwmp->flexpwmp->MCTRL.B.RUN &= 0b0111;
    }
#endif

    /* Disable peripheral clock if there is not an activated module.*/
    if ((pwmp->flexpwmp->MCTRL.B.RUN & 0b0001) ||
        (pwmp->flexpwmp->MCTRL.B.RUN & 0b0010) ||
        (pwmp->flexpwmp->MCTRL.B.RUN & 0b0100) ||
        (pwmp->flexpwmp->MCTRL.B.RUN & 0b1000) == 0) {
      halSPCSetPeripheralClockMode(SPC5_FLEXPWM0_PCTL,
          SPC5_PWM_FLEXPWM0_STOP_PCTL);
    }
  }
}

/**
 * @brief   Enables a PWM channel.
 * @pre     The PWM unit must have been activated using @p pwmStart().
 * @post    The channel is active using the specified configuration.
 * @note    The function has effect at the next cycle start.
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

#if SPC5_PWM_USE_SMOD0
  if (&PWMD1 == pwmp) {
    pwm_lld_enable_submodule_channel(pwmp, channel, width, 0);
  }
#endif
#if SPC5_PWM_USE_SMOD1
  if (&PWMD2 == pwmp) {
    pwm_lld_enable_submodule_channel(pwmp, channel, width, 1);
  }
#endif
#if SPC5_PWM_USE_SMOD2
  if (&PWMD3 == pwmp) {
    pwm_lld_enable_submodule_channel(pwmp, channel, width, 2);
  }
#endif
#if SPC5_PWM_USE_SMOD3
  if (&PWMD4 == pwmp) {
    pwm_lld_enable_submodule_channel(pwmp, channel, width, 3);
  }
#endif
}

/**
 * @brief   Disables a PWM channel.
 * @pre     The PWM unit must have been activated using @p pwmStart().
 * @post    The channel is disabled and its output line returned to the
 *          idle state.
 * @note    The function has effect at the next cycle start.
 *
 * @param[in] pwmp      pointer to a @p PWMDriver object
 * @param[in] channel   PWM channel identifier (0...PWM_CHANNELS-1)
 *
 * @notapi
 */
void pwm_lld_disable_channel(PWMDriver *pwmp, pwmchannel_t channel) {

#if SPC5_PWM_USE_SMOD0
  if (&PWMD1 == pwmp) {
    pwm_lld_disable_submodule_channel(pwmp, channel, 0);
  }
#endif
#if SPC5_PWM_USE_SMOD1
  if (&PWMD2 == pwmp) {
    pwm_lld_disable_submodule_channel(pwmp, channel, 1);
  }
#endif
#if SPC5_PWM_USE_SMOD2
  if (&PWMD3 == pwmp) {
    pwm_lld_disable_submodule_channel(pwmp, channel, 2);
  }
#endif
#if SPC5_PWM_USE_SMOD3
  if (&PWMD4 == pwmp) {
    pwm_lld_disable_submodule_channel(pwmp, channel, 3);
  }
#endif
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

  (void)period;
  pwmcnt_t pwmperiod;
  pwmperiod = period;
#if SPC5_PWM_USE_SMOD0
  if (&PWMD1 == pwmp) {
    pwmp->flexpwmp->MCTRL.B.CLDOK |= 0b0001;

    /* Setting PWM period.*/
    pwmp->flexpwmp->SUB[0].INIT.R = ~(pwmperiod / 2) + 1U;
    pwmp->flexpwmp->SUB[0].VAL[0].R = 0x0000;
    pwmp->flexpwmp->SUB[0].VAL[1].R = pwmperiod / 2;

    switch (pwmp->config->mode & PWM_OUTPUT_MASK) {
      case EDGE_ALIGNED_PWM:

      /* Setting active front of PWM channels.*/
      pwmp->flexpwmp->SUB[0].VAL[2].R = ~(pwmperiod / 2) + 1U;
      pwmp->flexpwmp->SUB[0].VAL[4].R = ~(pwmperiod / 2) + 1U;
      break;
      default:
      ;
    }
    pwmp->flexpwmp->MCTRL.B.LDOK |= 0b0001;
  }
#endif
#if SPC5_PWM_USE_SMOD1
  if (&PWMD2 == pwmp) {
    pwmp->flexpwmp->MCTRL.B.CLDOK |= 0b0010;

    /* Setting PWM period.*/
    pwmp->flexpwmp->SUB[1].INIT.R = ~(pwmperiod / 2) + 1U;
    pwmp->flexpwmp->SUB[1].VAL[0].R = 0x0000;
    pwmp->flexpwmp->SUB[1].VAL[1].R = pwmperiod / 2;

    switch (pwmp->config->mode & PWM_OUTPUT_MASK) {
      case EDGE_ALIGNED_PWM:

      /* Setting active front of PWM channels.*/
      pwmp->flexpwmp->SUB[1].VAL[2].R = ~(pwmperiod / 2) + 1U;
      pwmp->flexpwmp->SUB[1].VAL[4].R = ~(pwmperiod / 2) + 1U;
      break;
      default:
      ;
    }
    pwmp->flexpwmp->MCTRL.B.LDOK |= 0b0010;
  }
#endif
#if SPC5_PWM_USE_SMOD2
  if (&PWMD3 == pwmp) {
    pwmp->flexpwmp->MCTRL.B.CLDOK |= 0b0100;

    /* Setting PWM period.*/
    pwmp->flexpwmp->SUB[2].INIT.R = ~(pwmperiod / 2) + 1U;
    pwmp->flexpwmp->SUB[2].VAL[0].R = 0x0000;
    pwmp->flexpwmp->SUB[2].VAL[1].R = pwmperiod / 2;

    switch (pwmp->config->mode & PWM_OUTPUT_MASK) {
      case EDGE_ALIGNED_PWM:

      /* Setting active front of PWM channels.*/
      pwmp->flexpwmp->SUB[2].VAL[2].R = ~(pwmperiod / 2) + 1U;
      pwmp->flexpwmp->SUB[2].VAL[4].R = ~(pwmperiod / 2) + 1U;
      break;
      default:
      ;
    }
    pwmp->flexpwmp->MCTRL.B.LDOK |= 0b0100;
  }
#endif
#if SPC5_PWM_USE_SMOD3
  if (&PWMD4 == pwmp) {
    pwmp->flexpwmp->MCTRL.B.CLDOK |= 0b1000;

    /* Setting PWM period.*/
    pwmp->flexpwmp->SUB[3].INIT.R = ~(pwmperiod / 2) + 1U;
    pwmp->flexpwmp->SUB[3].VAL[0].R = 0x0000;
    pwmp->flexpwmp->SUB[3].VAL[1].R = pwmperiod / 2;

    switch (pwmp->config->mode & PWM_OUTPUT_MASK) {
      case EDGE_ALIGNED_PWM:

      /* Setting active front of PWM channels.*/
      pwmp->flexpwmp->SUB[3].VAL[2].R = ~(pwmperiod / 2) + 1U;
      pwmp->flexpwmp->SUB[3].VAL[4].R = ~(pwmperiod / 2) + 1U;
      break;
      default:
      ;
    }
    pwmp->flexpwmp->MCTRL.B.LDOK |= 0b1000;
  }
#endif
}

#endif /* HAL_USE_PWM */

/** @} */
