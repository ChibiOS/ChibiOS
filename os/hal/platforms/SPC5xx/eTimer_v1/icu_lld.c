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
 * @file
 * @brief   SPC5xx low level icu driver header.
 *
 * @addtogroup ICU
 * @{
 */

#include "ch.h"
#include "hal.h"

#if HAL_USE_ICU || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver local definitions.                                                 */
/*===========================================================================*/

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

/**
 * @brief   ICUD1 driver identifier.
 * @note    The driver ICUD1 allocates the complex timer SMD0 when enabled.
 */
#if SPC5_ICU_USE_SMOD0 || defined(__DOXYGEN__)
ICUDriver ICUD1;
#endif

/**
 * @brief   ICUD2 driver identifier.
 * @note    The driver ICUD2 allocates the complex timer SMD1 when enabled.
 */
#if SPC5_ICU_USE_SMOD1 || defined(__DOXYGEN__)
ICUDriver ICUD2;
#endif

/**
 * @brief   ICUD3 driver identifier.
 * @note    The driver ICUD3 allocates the complex timer SMD2 when enabled.
 */
#if SPC5_ICU_USE_SMOD2 || defined(__DOXYGEN__)
ICUDriver ICUD3;
#endif

/**
 * @brief   ICUD4 driver identifier.
 * @note    The driver ICUD4 allocates the complex timer SMD3 when enabled.
 */
#if SPC5_ICU_USE_SMD3 || defined(__DOXYGEN__)
ICUDriver ICUD4;
#endif

/**
 * @brief   ICUD5 driver identifier.
 * @note    The driver ICUD5 allocates the complex timer SMD4 when enabled.
 */
#if SPC5_ICU_USE_SMOD4 || defined(__DOXYGEN__)
ICUDriver ICUD5;
#endif

/**
 * @brief   ICUD6 driver identifier.
 * @note    The driver ICUD6 allocates the complex timer SMD5 when enabled.
 */
#if SPC5_ICU_USE_SMOD5 || defined(__DOXYGEN__)
ICUDriver ICUD6;
#endif

/**
 * @brief   ICUD7 driver identifier.
 * @note    The driver ICUD7 allocates the complex timer SMD6 when enabled.
 */
#if SPC5_ICU_USE_SMOD6 || defined(__DOXYGEN__)
ICUDriver ICUD7;
#endif

/**
 * @brief   ICUD8 driver identifier.
 * @note    The driver ICUD8 allocates the complex timer SMD7 when enabled.
 */
#if SPC5_ICU_USE_SMOD7 || defined(__DOXYGEN__)
ICUDriver ICUD8;
#endif

/**
 * @brief   ICUD9 driver identifier.
 * @note    The driver ICUD9 allocates the complex timer SMD8 when enabled.
 */
#if SPC5_ICU_USE_SMOD8 || defined(__DOXYGEN__)
ICUDriver ICUD9;
#endif

/**
 * @brief   ICUD10 driver identifier.
 * @note    The driver ICUD10 allocates the complex timer SMD9 when enabled.
 */
#if SPC5_ICU_USE_SMOD9 || defined(__DOXYGEN__)
ICUDriver ICUD10;
#endif

/**
 * @brief   ICUD11 driver identifier.
 * @note    The driver ICUD11 allocates the complex timer SMD10 when enabled.
 */
#if SPC5_ICU_USE_SMOD10 || defined(__DOXYGEN__)
ICUDriver ICUD11;
#endif

/**
 * @brief   ICUD12 driver identifier.
 * @note    The driver ICUD12 allocates the complex timer SMD11 when enabled.
 */
#if SPC5_ICU_USE_SMOD11 || defined(__DOXYGEN__)
ICUDriver ICUD12;
#endif

/*===========================================================================*/
/* Driver local variables.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

/**
 * @brief   ICU eTimer channel enable.
 *
 * @param[in] icup      pointer to the @p ICUDriver object
 * @param[in] index      ICU channel index
 */
static void spc5_icu_channel_enable(ICUDriver *icup, uint8_t index) {
  /* Clear pending IRQs (if any).*/
  icup->etimerp->CHANNEL[index].STS.R = 0xFFFF;
  /* Set Capture 1 and Capture 2 Mode.*/
  icup->etimerp->CHANNEL[index].CCCTRL.B.CPT1MODE = 0b10;
  icup->etimerp->CHANNEL[index].CCCTRL.B.CPT2MODE = 0b01;
  /* Active interrupts.*/
  if (icup->config->period_cb != NULL || icup->config->width_cb != NULL) {
    icup->etimerp->CHANNEL[index].INTDMA.B.ICF1IE = 1U;
    icup->etimerp->CHANNEL[index].INTDMA.B.ICF2IE = 1U;
  }
  if (icup->config->overflow_cb != NULL) {
    icup->etimerp->CHANNEL[index].INTDMA.B.TOFIE = 1U;
  }
  /* Set Capture FIFO Water Mark.*/
  icup->etimerp->CHANNEL[index].CCCTRL.B.CFWM = 0b00;
  /* Enable Counter.*/
  if (ICU_SKIP_FIRST_CAPTURE) {
    icup->etimerp->CHANNEL[index].CTRL.B.CNTMODE = 0b011;
  }
  else {
    icup->etimerp->CHANNEL[index].CTRL.B.CNTMODE = 0b001;
  }
  /* Enable Capture process.*/
  icup->etimerp->CHANNEL[index].CCCTRL.B.ARM = 1U;
}

/**
 * @brief   ICU eTimer channel disable.
 *
 * @param[in] icup      pointer to the @p ICUDriver object
 * @param[in] index      ICU channel index
 */
static void spc5_icu_channel_disable(ICUDriver *icup, uint8_t index) {
  /* Disable Capture process.*/
  icup->etimerp->CHANNEL[index].CCCTRL.B.ARM = 0;
  /* Clear pending IRQs (if any).*/
  icup->etimerp->CHANNEL[index].STS.R = 0xFFFF;
  /* Set Capture 1 and Capture 2 Mode to Disabled.*/
  icup->etimerp->CHANNEL[index].CCCTRL.B.CPT1MODE = 0b00;
  icup->etimerp->CHANNEL[index].CCCTRL.B.CPT2MODE = 0b00;
  /* Disable interrupts.*/
  if (icup->config->period_cb != NULL || icup->config->width_cb != NULL) {
    icup->etimerp->CHANNEL[index].INTDMA.B.ICF1IE = 0;
    icup->etimerp->CHANNEL[index].INTDMA.B.ICF2IE = 0;
  }
  if (icup->config->overflow_cb != NULL)
    icup->etimerp->CHANNEL[index].INTDMA.B.TOFIE = 0;
}

/**
 * @brief   ICU eTimer channel start.
 *
 * @param[in] icup      pointer to the @p ICUDriver object
 * @param[in] index      ICU channel index
 */
static void spc5_icu_channel_start(ICUDriver *icup, uint8_t index) {
  /* Timer disabled.*/
  icup->etimerp->CHANNEL[index].CTRL.B.CNTMODE = 0b000;
  /* Clear pending IRQs (if any).*/
  icup->etimerp->CHANNEL[index].STS.R = 0xFFFF;
  /* All IRQs and DMA requests disabled.*/
  icup->etimerp->CHANNEL[index].INTDMA.R = 0x0000;
  /* Compare Load 1 disabled.*/
  icup->etimerp->CHANNEL[index].CCCTRL.B.CLC1 = 0b000;
  /* Compare Load 2 disabled.*/
  icup->etimerp->CHANNEL[index].CCCTRL.B.CLC2 = 0b000;
  /* Capture 1 disabled.*/
  icup->etimerp->CHANNEL[index].CCCTRL.B.CPT1MODE = 0b00;
  /* Capture 2 disabled.*/
  icup->etimerp->CHANNEL[index].CCCTRL.B.CPT2MODE = 0b00;
  /* Counter reset to zero.*/
  icup->etimerp->CHANNEL[index].CNTR.R = 0x0000;
}

/**
 * @brief   ICU eTimer channel initialization.
 *
 * @param[in] icup      pointer to the @p ICUDriver object
 * @param[in] index      ICU channel index
 */
static void spc5_icu_channel_init(ICUDriver *icup, uint8_t index) {
#if !defined(psc)
  uint32_t psc;
#endif
  psc = (icup->clock / icup->config->frequency);
  chDbgAssert(
      (psc <= 0xFFFF) && (((psc) * icup->config->frequency) == icup->clock) &&
      ((psc == 1) || (psc == 2) || (psc == 4) || (psc == 8) || (psc == 16) ||
          (psc == 32) || (psc == 64) || (psc == 128)),
      "icu_lld_start(), #1", "invalid frequency");

  /* Set primary source and clock prescaler.*/
  switch (psc) {
  case 1:
    icup->etimerp->CHANNEL[index].CTRL.B.PRISRC = 0b11000;
    break;
  case 2:
    icup->etimerp->CHANNEL[index].CTRL.B.PRISRC = 0b11001;
    break;
  case 4:
    icup->etimerp->CHANNEL[index].CTRL.B.PRISRC = 0b11010;
    break;
  case 8:
    icup->etimerp->CHANNEL[index].CTRL.B.PRISRC = 0b11011;
    break;
  case 16:
    icup->etimerp->CHANNEL[index].CTRL.B.PRISRC = 0b11100;
    break;
  case 32:
    icup->etimerp->CHANNEL[index].CTRL.B.PRISRC = 0b11101;
    break;
  case 64:
    icup->etimerp->CHANNEL[index].CTRL.B.PRISRC = 0b11110;
    break;
  case 128:
    icup->etimerp->CHANNEL[index].CTRL.B.PRISRC = 0b11111;
    break;
  }
  /* Set control registers.*/
  icup->etimerp->CHANNEL[index].CTRL.B.ONCE = 0;
  icup->etimerp->CHANNEL[index].CTRL.B.LENGTH = 0;
  icup->etimerp->CHANNEL[index].CTRL.B.DIR = 0;
  icup->etimerp->CHANNEL[index].CTRL2.B.PIPS = 0;

  /* Set secondary source.*/
  switch (icup->config->channel) {
  case ICU_CHANNEL_1:
    icup->etimerp->CHANNEL[index].CTRL.B.SECSRC = 0b00000;
    break;
  case ICU_CHANNEL_2:
    icup->etimerp->CHANNEL[index].CTRL.B.SECSRC = 0b00001;
    break;
  case ICU_CHANNEL_3:
    icup->etimerp->CHANNEL[index].CTRL.B.SECSRC = 0b00010;
    break;
  case ICU_CHANNEL_4:
    icup->etimerp->CHANNEL[index].CTRL.B.SECSRC = 0b00011;
    break;
  case ICU_CHANNEL_5:
    icup->etimerp->CHANNEL[index].CTRL.B.SECSRC = 0b00100;
    break;
  case ICU_CHANNEL_6:
    icup->etimerp->CHANNEL[index].CTRL.B.SECSRC = 0b00101;
    break;
  }

  /* Set secondary source polarity.*/
  if (icup->config->mode == ICU_INPUT_ACTIVE_HIGH) {
    icup->etimerp->CHANNEL[index].CTRL2.B.SIPS = 0;
  }
  else {
    icup->etimerp->CHANNEL[index].CTRL2.B.SIPS = 1U;
  }

  /* Direct pointers to the capture registers in order to make reading
   data faster from within callbacks.*/
  icup->pccrp = &icup->etimerp->CHANNEL[index].CAPT1.R;
  icup->wccrp = &icup->etimerp->CHANNEL[index].CAPT2.R;

  /* Enable channel.*/
  icup->etimerp->ENBL.B.ENBL |= 1U << index;
}

/**
 * @brief   Interrupt Management.
 *
 * @param[in] icup      pointer to the @p ICUDriver object
 * @param[in] index      ICU channel index
 */
static void icu_lld_interrupt_management(ICUDriver *icup, uint8_t index) {
#if !defined(sr)
  uint16_t sr;
#endif
  sr = icup->etimerp->CHANNEL[index].STS.R
      & icup->etimerp->CHANNEL[index].INTDMA.R;
  if (ICU_SKIP_FIRST_CAPTURE) {
    if ((sr & 0x0008) != 0) { /* TOF */
      icup->etimerp->CHANNEL[index].STS.B.TOF = 1U;
      _icu_isr_invoke_overflow_cb(icup);
    }
    if ((sr & 0x0040) != 0) { /* ICF1 */
      if (icup->etimerp->CHANNEL[index].CTRL.B.CNTMODE == 0b011) {
        icup->etimerp->CHANNEL[index].STS.B.ICF1 = 1U;
        icup->etimerp->CHANNEL[index].CTRL.B.CNTMODE = 0b001;
      }
      else {
        icup->etimerp->CHANNEL[index].STS.B.ICF1 = 1U;
        icup->etimerp->CHANNEL[index].CNTR.R = 0x0000;
        _icu_isr_invoke_period_cb(icup);
      }
    }
    else if ((sr & 0x0080) != 0) { /* ICF2 */
      if (icup->etimerp->CHANNEL[index].CTRL.B.CNTMODE == 0b011) {
        icup->etimerp->CHANNEL[index].STS.B.ICF2 = 1U;
        icup->etimerp->CHANNEL[index].CNTR.R = 0x0000;
      }
      else {
        icup->etimerp->CHANNEL[index].STS.B.ICF2 = 1U;
        _icu_isr_invoke_width_cb(icup);
      }
    }
  } else { /* End ICU_SKIP_FIRST_CAPTURE = TRUE*/
    if ((sr & 0x0008) != 0) { /* TOF */
      icup->etimerp->CHANNEL[index].STS.B.TOF = 1U;
      _icu_isr_invoke_overflow_cb(icup);
    }
    if ((sr & 0x0040) != 0) { /* ICF1 */
      icup->etimerp->CHANNEL[index].STS.B.ICF1 = 1U;
      icup->etimerp->CHANNEL[index].CNTR.R = 0x0000;
      _icu_isr_invoke_period_cb(icup);
    }
    else if ((sr & 0x0080) != 0) { /* ICF2 */
      icup->etimerp->CHANNEL[index].STS.B.ICF2 = 1U;
      _icu_isr_invoke_width_cb(icup);
    }
  } /* End ICU_SKIP_FIRST_CAPTURE = FALSE*/
}

/**
 * @brief   Shared IRQ handler.
 *
 * @param[in] icup      pointer to the @p ICUDriver object
 */
static void icu_lld_serve_interrupt(ICUDriver *icup) {

#if SPC5_ICU_USE_SMOD0
  if (&ICUD1 == icup) {
    icu_lld_interrupt_management(icup, 0);
  }
#endif

#if SPC5_ICU_USE_SMOD1
  if (&ICUD2 == icup) {
    icu_lld_interrupt_management(icup, 1);
  }
#endif

#if SPC5_ICU_USE_SMOD2
  if (&ICUD3 == icup) {
    icu_lld_interrupt_management(icup, 2);
  }
#endif

#if SPC5_ICU_USE_SMOD3
  if (&ICUD4 == icup) {
    icu_lld_interrupt_management(icup, 3);
  }
#endif

#if SPC5_ICU_USE_SMOD4
  if (&ICUD5 == icup) {
    icu_lld_interrupt_management(icup, 4);
  }
#endif

#if SPC5_ICU_USE_SMOD5
  if (&ICUD6 == icup) {
    icu_lld_interrupt_management(icup, 5);
  }
#endif

#if SPC5_ICU_USE_SMOD6
  if (&ICUD7 == icup) {
    icu_lld_interrupt_management(icup, 0);
  }
#endif

#if SPC5_ICU_USE_SMOD7
  if (&ICUD8 == icup) {
    icu_lld_interrupt_management(icup, 1);
  }
#endif

#if SPC5_ICU_USE_SMOD8
  if (&ICUD9 == icup) {
    icu_lld_interrupt_management(icup, 2);
  }
#endif

#if SPC5_ICU_USE_SMOD9
  if (&ICUD10 == icup) {
    icu_lld_interrupt_management(icup, 3);
  }
#endif

#if SPC5_ICU_USE_SMOD10
  if (&ICUD11 == icup) {
    icu_lld_interrupt_management(icup, 4);
  }
#endif

#if SPC5_ICU_USE_SMOD11
  if (&ICUD12 == icup) {
    icu_lld_interrupt_management(icup, 5);
  }
#endif
}

/**
 * @brief   eTimer SubModules initialization.
 * @details This function must be invoked with interrupts disabled.
 *
 * @param[in] sdp       pointer to a @p ICUDriver object
 * @param[in] config    the architecture-dependent ICU driver configuration
 */
static void spc5_icu_smod_init(ICUDriver *icup) {

#if SPC5_ICU_USE_SMOD0
  if (&ICUD1 == icup) {
    icup->clock = SPC5_ETIMER0_CLK;
    spc5_icu_channel_init(icup, 0);
  }
#endif

#if SPC5_ICU_USE_SMOD1
  if (&ICUD2 == icup) {
    icup->clock = SPC5_ETIMER0_CLK;
    spc5_icu_channel_init(icup, 1);
  }
#endif

#if SPC5_ICU_USE_SMOD2
  if (&ICUD3 == icup) {
    icup->clock = SPC5_ETIMER0_CLK;
    spc5_icu_channel_init(icup, 2);
  }
#endif

#if SPC5_ICU_USE_SMOD3
  if (&ICUD4 == icup) {
    icup->clock = SPC5_ETIMER0_CLK;
    spc5_icu_channel_init(icup, 3);
  }
#endif

#if SPC5_ICU_USE_SMOD4
  if (&ICUD5 == icup) {
    icup->clock = SPC5_ETIMER0_CLK;
    spc5_icu_channel_init(icup, 4);
  }
#endif

#if SPC5_ICU_USE_SMOD5
  if (&ICUD6 == icup) {
    icup->clock = SPC5_ETIMER0_CLK;
    spc5_icu_channel_init(icup, 5);
  }
#endif

#if SPC5_ICU_USE_SMOD6
  if (&ICUD7 == icup) {
    icup->clock = SPC5_ETIMER1_CLK;
    spc5_icu_channel_init(icup, 0);
  }
#endif

#if SPC5_ICU_USE_SMOD7
  if (&ICUD8 == icup) {
    icup->clock = SPC5_ETIMER1_CLK;
    spc5_icu_channel_init(icup, 1);
  }
#endif

#if SPC5_ICU_USE_SMOD8
  if (&ICUD9 == icup) {
    icup->clock = SPC5_ETIMER1_CLK;
    spc5_icu_channel_init(icup, 2);
  }
#endif

#if SPC5_ICU_USE_SMOD9
  if (&ICUD10 == icup) {
    icup->clock = SPC5_ETIMER1_CLK;
    spc5_icu_channel_init(icup, 3);
  }
#endif

#if SPC5_ICU_USE_SMOD10
  if (&ICUD11 == icup) {
    icup->clock = SPC5_ETIMER1_CLK;
    spc5_icu_channel_init(icup, 4);
  }
#endif

#if SPC5_ICU_USE_SMOD11
  if (&ICUD12 == icup) {
    icup->clock = SPC5_ETIMER1_CLK;
    spc5_icu_channel_init(icup, 5);
  }
#endif
}

/*===========================================================================*/
/* Driver interrupt handlers.                                                */
/*===========================================================================*/

#if SPC5_ICU_USE_SMOD0
#if !defined(SPC5_ETIMER0_TC0IR_HANDLER)
#error "SPC5_ETIMER0_TC0IR_HANDLER not defined"
#endif
/**
 * @brief   eTimer0 Channel 0 interrupt handler.
 * @note    It is assumed that the various sources are only activated if the
 *          associated callback pointer is not equal to @p NULL in order to not
 *          perform an extra check in a potentially critical interrupt handler.
 *
 * @isr
 */CH_IRQ_HANDLER(SPC5_ETIMER0_TC0IR_HANDLER) {

  CH_IRQ_PROLOGUE();

  icu_lld_serve_interrupt(&ICUD1);

  CH_IRQ_EPILOGUE();
}
#endif /* SPC5_ICU_USE_SMOD0 */

#if SPC5_ICU_USE_SMOD1
#if !defined(SPC5_ETIMER0_TC1IR_HANDLER)
#error "SPC5_ETIMER0_TC1IR_HANDLER not defined"
#endif
/**
 * @brief   eTimer0 Channel 1 interrupt handler.
 * @note    It is assumed that the various sources are only activated if the
 *          associated callback pointer is not equal to @p NULL in order to not
 *          perform an extra check in a potentially critical interrupt handler.
 *
 * @isr
 */
CH_IRQ_HANDLER(SPC5_ETIMER0_TC1IR_HANDLER) {

  CH_IRQ_PROLOGUE();

  icu_lld_serve_interrupt(&ICUD2);

  CH_IRQ_EPILOGUE();
}
#endif /* SPC5_ICU_USE_SMOD1 */

#if SPC5_ICU_USE_SMOD2
#if !defined(SPC5_ETIMER0_TC2IR_HANDLER)
#error "SPC5_ETIMER0_TC2IR_HANDLER not defined"
#endif
/**
 * @brief   eTimer0 Channel 2 interrupt handler.
 * @note    It is assumed that the various sources are only activated if the
 *          associated callback pointer is not equal to @p NULL in order to not
 *          perform an extra check in a potentially critical interrupt handler.
 *
 * @isr
 */
CH_IRQ_HANDLER(SPC5_ETIMER0_TC2IR_HANDLER) {

  CH_IRQ_PROLOGUE();

  icu_lld_serve_interrupt(&ICUD3);

  CH_IRQ_EPILOGUE();
}
#endif /* SPC5_ICU_USE_SMOD2 */

#if SPC5_ICU_USE_SMOD3
#if !defined(SPC5_ETIMER0_TC3IR_HANDLER)
#error "SPC5_ETIMER0_TC3IR_HANDLER not defined"
#endif
/**
 * @brief   eTimer0 Channel 3 interrupt handler.
 * @note    It is assumed that the various sources are only activated if the
 *          associated callback pointer is not equal to @p NULL in order to not
 *          perform an extra check in a potentially critical interrupt handler.
 *
 * @isr
 */
CH_IRQ_HANDLER(SPC5_ETIMER0_TC3IR_HANDLER) {

  CH_IRQ_PROLOGUE();

  icu_lld_serve_interrupt(&ICUD4);

  CH_IRQ_EPILOGUE();
}
#endif /* SPC5_ICU_USE_SMOD3 */

#if SPC5_ICU_USE_SMOD4
#if !defined(SPC5_ETIMER0_TC4IR_HANDLER)
#error "SPC5_ETIMER0_TC4IR_HANDLER not defined"
#endif
/**
 * @brief   eTimer0 Channel 4 interrupt handler.
 * @note    It is assumed that the various sources are only activated if the
 *          associated callback pointer is not equal to @p NULL in order to not
 *          perform an extra check in a potentially critical interrupt handler.
 *
 * @isr
 */
CH_IRQ_HANDLER(SPC5_ETIMER0_TC4IR_HANDLER) {

  CH_IRQ_PROLOGUE();

  icu_lld_serve_interrupt(&ICUD5);

  CH_IRQ_EPILOGUE();
}
#endif /* SPC5_ICU_USE_SMOD4 */

#if SPC5_ICU_USE_SMOD5
#if !defined(SPC5_ETIMER0_TC5IR_HANDLER)
#error "SPC5_ETIMER0_TC5IR_HANDLER not defined"
#endif
/**
 * @brief   eTimer0 Channel 5 interrupt handler.
 * @note    It is assumed that the various sources are only activated if the
 *          associated callback pointer is not equal to @p NULL in order to not
 *          perform an extra check in a potentially critical interrupt handler.
 *
 * @isr
 */
CH_IRQ_HANDLER(SPC5_ETIMER0_TC5IR_HANDLER) {

  CH_IRQ_PROLOGUE();

  icu_lld_serve_interrupt(&ICUD6);

  CH_IRQ_EPILOGUE();
}
#endif /* SPC5_ICU_USE_SMOD5 */

#if SPC5_ICU_USE_SMOD6
#if !defined(SPC5_ETIMER1_TC0IR_HANDLER)
#error "SPC5_ETIMER1_TC0IR_HANDLER not defined"
#endif
/**
 * @brief   eTimer1 Channel 0 interrupt handler.
 * @note    It is assumed that the various sources are only activated if the
 *          associated callback pointer is not equal to @p NULL in order to not
 *          perform an extra check in a potentially critical interrupt handler.
 *
 * @isr
 */
CH_IRQ_HANDLER(SPC5_ETIMER1_TC0IR_HANDLER) {

//SIU.PCR[3*16+5].R = 556;
//SIU.GPDO[3*16+5].R = 0;

  CH_IRQ_PROLOGUE();

  icu_lld_serve_interrupt(&ICUD7);

  CH_IRQ_EPILOGUE();
}
#endif /* SPC5_ICU_USE_SMOD6 */

#if SPC5_ICU_USE_SMOD7
#if !defined(SPC5_ETIMER1_TC1IR_HANDLER)
#error "SPC5_ETIMER1_TC1IR_HANDLER not defined"
#endif
/**
 * @brief   eTimer1 Channel 1 interrupt handler.
 * @note    It is assumed that the various sources are only activated if the
 *          associated callback pointer is not equal to @p NULL in order to not
 *          perform an extra check in a potentially critical interrupt handler.
 *
 * @isr
 */
CH_IRQ_HANDLER(SPC5_ETIMER1_TC1IR_HANDLER) {

  CH_IRQ_PROLOGUE();

  icu_lld_serve_interrupt(&ICUD8);

  CH_IRQ_EPILOGUE();
}
#endif /* SPC5_ICU_USE_SMOD7 */

#if SPC5_ICU_USE_SMOD8
#if !defined(SPC5_ETIMER1_TC2IR_HANDLER)
#error "SPC5_ETIMER1_TC2IR_HANDLER not defined"
#endif
/**
 * @brief   eTimer1 Channel 2 interrupt handler.
 * @note    It is assumed that the various sources are only activated if the
 *          associated callback pointer is not equal to @p NULL in order to not
 *          perform an extra check in a potentially critical interrupt handler.
 *
 * @isr
 */
CH_IRQ_HANDLER(SPC5_ETIMER1_TC2IR_HANDLER) {

  CH_IRQ_PROLOGUE();

  icu_lld_serve_interrupt(&ICUD9);

  CH_IRQ_EPILOGUE();
}
#endif /* SPC5_ICU_USE_SMOD8 */

#if SPC5_ICU_USE_SMOD9
#if !defined(SPC5_ETIMER1_TC3IR_HANDLER)
#error "SPC5_ETIMER0_TC3IR_HANDLER not defined"
#endif
/**
 * @brief   eTimer1 Channel 3 interrupt handler.
 * @note    It is assumed that the various sources are only activated if the
 *          associated callback pointer is not equal to @p NULL in order to not
 *          perform an extra check in a potentially critical interrupt handler.
 *
 * @isr
 */
CH_IRQ_HANDLER(SPC5_ETIMER1_TC3IR_HANDLER) {

  CH_IRQ_PROLOGUE();

  icu_lld_serve_interrupt(&ICUD10);

  CH_IRQ_EPILOGUE();
}
#endif /* SPC5_ICU_USE_SMOD9 */

#if SPC5_ICU_USE_SMOD10
#if !defined(SPC5_ETIMER1_TC4IR_HANDLER)
#error "SPC5_ETIMER1_TC4IR_HANDLER not defined"
#endif
/**
 * @brief   eTimer1 Channel 4 interrupt handler.
 * @note    It is assumed that the various sources are only activated if the
 *          associated callback pointer is not equal to @p NULL in order to not
 *          perform an extra check in a potentially critical interrupt handler.
 *
 * @isr
 */
CH_IRQ_HANDLER(SPC5_ETIMER1_TC4IR_HANDLER) {

  CH_IRQ_PROLOGUE();

  icu_lld_serve_interrupt(&ICUD11);

  CH_IRQ_EPILOGUE();
}
#endif /* SPC5_ICU_USE_SMOD10 */

#if SPC5_ICU_USE_SMOD11
#if !defined(SPC5_ETIMER1_TC5IR_HANDLER)
#error "SPC5_ETIMER1_TC5IR_HANDLER not defined"
#endif
/**
 * @brief   eTimer1 Channel 5 interrupt handler.
 * @note    It is assumed that the various sources are only activated if the
 *          associated callback pointer is not equal to @p NULL in order to not
 *          perform an extra check in a potentially critical interrupt handler.
 *
 * @isr
 */
CH_IRQ_HANDLER(SPC5_ETIMER1_TC5IR_HANDLER) {

  CH_IRQ_PROLOGUE();

  icu_lld_serve_interrupt(&ICUD12);

  CH_IRQ_EPILOGUE();
}
#endif /* SPC5_ICU_USE_SMOD11 */

/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

/**
 * @brief   Low level ICU driver initialization.
 *
 * @notapi
 */
void icu_lld_init(void) {

#if SPC5_ICU_USE_SMOD0
  /* Driver initialization.*/
  icuObjectInit(&ICUD1);
  ICUD1.etimerp = &ETIMER_0;
#endif

#if SPC5_ICU_USE_SMOD1
  /* Driver initialization.*/
  icuObjectInit(&ICUD2);
  ICUD2.etimerp = &ETIMER_0;
#endif

#if SPC5_ICU_USE_SMOD2
  /* Driver initialization.*/
  icuObjectInit(&ICUD3);
  ICUD3.etimerp = &ETIMER_0;
#endif

#if SPC5_ICU_USE_SMOD3
  /* Driver initialization.*/
  icuObjectInit(&ICUD4);
  ICUD4.etimerp = &ETIMER_0;
#endif

#if SPC5_ICU_USE_SMOD4
  /* Driver initialization.*/
  icuObjectInit(&ICUD5);
  ICUD5.etimerp = &ETIMER_0;
#endif

#if SPC5_ICU_USE_SMOD5
  /* Driver initialization.*/
  icuObjectInit(&ICUD6);
  ICUD6.etimerp = &ETIMER_0;
#endif

#if SPC5_ICU_USE_SMOD6
  /* Driver initialization.*/
  icuObjectInit(&ICUD7);
  ICUD7.etimerp = &ETIMER_1;
#endif

#if SPC5_ICU_USE_SMOD7
  /* Driver initialization.*/
  icuObjectInit(&ICUD8);
  ICUD8.etimerp = &ETIMER_1;
#endif

#if SPC5_ICU_USE_SMOD8
  /* Driver initialization.*/
  icuObjectInit(&ICUD9);
  ICUD9.etimerp = &ETIMER_1;
#endif

#if SPC5_ICU_USE_SMOD9
  /* Driver initialization.*/
  icuObjectInit(&ICUD10);
  ICUD10.etimerp = &ETIMER_1;
#endif

#if SPC5_ICU_USE_SMOD10
  /* Driver initialization.*/
  icuObjectInit(&ICUD11);
  ICUD11.etimerp = &ETIMER_1;
#endif

#if SPC5_ICU_USE_SMOD11
  /* Driver initialization.*/
  icuObjectInit(&ICUD12);
  ICUD12.etimerp = &ETIMER_1;
#endif

#if SPC5_ICU_USE_SMOD0 || SPC5_ICU_USE_SMOD1 ||								\
  SPC5_ICU_USE_SMOD2 || SPC5_ICU_USE_SMOD3 ||								\
  SPC5_ICU_USE_SMOD4 || SPC5_ICU_USE_SMOD5

  INTC.PSR[SPC5_ETIMER0_TC0IR_NUMBER].R = SPC5_ICU_ETIMER0_PRIORITY;
  INTC.PSR[SPC5_ETIMER0_TC1IR_NUMBER].R = SPC5_ICU_ETIMER0_PRIORITY;
  INTC.PSR[SPC5_ETIMER0_TC2IR_NUMBER].R = SPC5_ICU_ETIMER0_PRIORITY;
  INTC.PSR[SPC5_ETIMER0_TC3IR_NUMBER].R = SPC5_ICU_ETIMER0_PRIORITY;
  INTC.PSR[SPC5_ETIMER0_TC4IR_NUMBER].R = SPC5_ICU_ETIMER0_PRIORITY;
  INTC.PSR[SPC5_ETIMER0_TC5IR_NUMBER].R = SPC5_ICU_ETIMER0_PRIORITY;
  INTC.PSR[SPC5_ETIMER0_WTIF_NUMBER].R = SPC5_ICU_ETIMER0_PRIORITY;
  INTC.PSR[SPC5_ETIMER0_RCF_NUMBER].R = SPC5_ICU_ETIMER0_PRIORITY;

#endif

#if SPC5_ICU_USE_SMOD6 || SPC5_ICU_USE_SMOD7 ||								\
  SPC5_ICU_USE_SMOD8 || SPC5_ICU_USE_SMOD9 ||								\
  SPC5_ICU_USE_SMOD10 || SPC5_ICU_USE_SMOD11

  INTC.PSR[SPC5_ETIMER1_TC0IR_NUMBER].R = SPC5_ICU_ETIMER1_PRIORITY;
  INTC.PSR[SPC5_ETIMER1_TC1IR_NUMBER].R = SPC5_ICU_ETIMER1_PRIORITY;
  INTC.PSR[SPC5_ETIMER1_TC2IR_NUMBER].R = SPC5_ICU_ETIMER1_PRIORITY;
  INTC.PSR[SPC5_ETIMER1_TC3IR_NUMBER].R = SPC5_ICU_ETIMER1_PRIORITY;
  INTC.PSR[SPC5_ETIMER1_TC4IR_NUMBER].R = SPC5_ICU_ETIMER1_PRIORITY;
  INTC.PSR[SPC5_ETIMER1_TC5IR_NUMBER].R = SPC5_ICU_ETIMER1_PRIORITY;
  INTC.PSR[SPC5_ETIMER1_RCF_NUMBER].R = SPC5_ICU_ETIMER1_PRIORITY;

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
  chDbgAssert(
      (icup->config->channel == ICU_CHANNEL_1) ||
      (icup->config->channel == ICU_CHANNEL_2) ||
      (icup->config->channel == ICU_CHANNEL_3) ||
      (icup->config->channel == ICU_CHANNEL_4) ||
      (icup->config->channel == ICU_CHANNEL_5) ||
      (icup->config->channel == ICU_CHANNEL_6),
      "icu_lld_start(), #1", "invalid input");

#if SPC5_ICU_USE_SMOD0 || SPC5_ICU_USE_SMOD1 || SPC5_ICU_USE_SMOD2 ||        \
    SPC5_ICU_USE_SMOD3 || SPC5_ICU_USE_SMOD4 || SPC5_ICU_USE_SMOD5
  uint8_t SMOD0 = 0;
  uint8_t SMOD1 = 0;
  uint8_t SMOD2 = 0;
  uint8_t SMOD3 = 0;
  uint8_t SMOD4 = 0;
  uint8_t SMOD5 = 0;
#endif

#if SPC5_ICU_USE_SMOD6 || SPC5_ICU_USE_SMOD7 || SPC5_ICU_USE_SMOD8 ||        \
    SPC5_ICU_USE_SMOD9 || SPC5_ICU_USE_SMOD10 || SPC5_ICU_USE_SMOD11
  uint8_t SMOD6 = 0;
  uint8_t SMOD7 = 0;
  uint8_t SMOD8 = 0;
  uint8_t SMOD9 = 0;
  uint8_t SMOD10 = 0;
  uint8_t SMOD11 = 0;
#endif

#if SPC5_ICU_USE_SMOD0
  if (ICUD1.state == ICU_READY)
    SMOD0 = 1U;
#endif
#if SPC5_ICU_USE_SMOD1
  if (ICUD2.state == ICU_READY)
  SMOD1 = 1U;
#endif
#if SPC5_ICU_USE_SMOD2
  if (ICUD3.state == ICU_READY)
  SMOD2 = 1U;
#endif
#if SPC5_ICU_USE_SMOD3
  if (ICUD4.state == ICU_READY)
  SMOD3 = 1U;
#endif
#if SPC5_ICU_USE_SMOD4
  if (ICUD5.state == ICU_READY)
  SMOD4 = 1U;
#endif
#if SPC5_ICU_USE_SMOD5
  if (ICUD6.state == ICU_READY)
  SMOD5 = 1U;
#endif
#if SPC5_ICU_USE_SMOD6
  if (ICUD7.state == ICU_READY)
  SMOD6 = 1U;
#endif
#if SPC5_ICU_USE_SMOD7
  if (ICUD8.state == ICU_READY)
  SMOD7 = 1U;
#endif
#if SPC5_ICU_USE_SMOD8
  if (ICUD9.state == ICU_READY)
  SMOD8 = 1U;
#endif
#if SPC5_ICU_USE_SMOD9
  if (ICUD10.state == ICU_READY)
  SMOD9 = 1U;
#endif
#if SPC5_ICU_USE_SMOD10
  if (ICUD11.state == ICU_READY)
  SMOD10 = 1U;
#endif
#if SPC5_ICU_USE_SMOD11
  if (ICUD12.state == ICU_READY)
  SMOD11 = 1U;
#endif

#if SPC5_ICU_USE_SMOD0 || SPC5_ICU_USE_SMOD1 || SPC5_ICU_USE_SMOD2 ||        \
    SPC5_ICU_USE_SMOD3 || SPC5_ICU_USE_SMOD4 || SPC5_ICU_USE_SMOD5
  /* Set Peripheral Clock.*/
  if (!(SMOD0 || SMOD1 || SMOD2 || SMOD3 || SMOD4 || SMOD5)) {
    halSPCSetPeripheralClockMode(SPC5_ETIMER0_PCTL,
                                 SPC5_ICU_ETIMER0_START_PCTL);
  }
#endif
#if SPC5_ICU_USE_SMOD6 || SPC5_ICU_USE_SMOD7 || SPC5_ICU_USE_SMOD8 ||        \
    SPC5_ICU_USE_SMOD9 || SPC5_ICU_USE_SMOD10 || SPC5_ICU_USE_SMOD11
  /* Set Peripheral Clock.*/
  if (!(SMOD6 || SMOD7 || SMOD8 || SMOD9 || SMOD10 || SMOD11)) {
    halSPCSetPeripheralClockMode(SPC5_ETIMER1_PCTL,
                                 SPC5_ICU_ETIMER1_START_PCTL);
  }
#endif

  /* Disable all channels.*/
  //icup->etimerp->ENBL.B.ENBL = 0x00;

  if (icup->state == ICU_STOP) {
#if SPC5_ICU_USE_SMOD0
    if (&ICUD1 == icup) {
      spc5_icu_channel_start(icup, 0);
    }
#endif
#if SPC5_ICU_USE_SMOD1
    if (&ICUD2 == icup) {
      spc5_icu_channel_start(icup, 1);
    }
#endif
#if SPC5_ICU_USE_SMOD2
    if (&ICUD3 == icup) {
      spc5_icu_channel_start(icup, 2);
    }
#endif
#if SPC5_ICU_USE_SMOD3
    if (&ICUD4 == icup) {
      spc5_icu_channel_start(icup, 3);
    }
#endif
#if SPC5_ICU_USE_SMOD4
    if (&ICUD5 == icup) {
      spc5_icu_channel_start(icup, 4);
    }
#endif
#if SPC5_ICU_USE_SMOD5
    if (&ICUD6 == icup) {
      spc5_icu_channel_start(icup, 5);
    }
#endif
#if SPC5_ICU_USE_SMOD6
    if (&ICUD7 == icup) {
      spc5_icu_channel_start(icup, 0);
    }
#endif
#if SPC5_ICU_USE_SMOD7
    if (&ICUD8 == icup) {
      spc5_icu_channel_start(icup, 1);
    }
#endif
#if SPC5_ICU_USE_SMOD8
    if (&ICUD9 == icup) {
      spc5_icu_channel_start(icup, 2);
    }
#endif
#if SPC5_ICU_USE_SMOD9
    if (&ICUD10 == icup) {
      spc5_icu_channel_start(icup, 3);
    }
#endif
#if SPC5_ICU_USE_SMOD10
    if (&ICUD11 == icup) {
      spc5_icu_channel_start(icup, 4);
    }
#endif
#if SPC5_ICU_USE_SMOD11
    if (&ICUD12 == icup) {
      spc5_icu_channel_start(icup, 5);
    }
#endif
  }
  /* Configuration.*/
  spc5_icu_smod_init(icup);
}

/**
 * @brief   Deactivates the ICU peripheral.
 *
 * @param[in] icup      pointer to the @p ICUDriver object
 *
 * @notapi
 */
void icu_lld_stop(ICUDriver *icup) {

  if (icup->state == ICU_READY) {

#if SPC5_ICU_USE_SMOD0 || SPC5_ICU_USE_SMOD1 || SPC5_ICU_USE_SMOD2 ||        \
    SPC5_ICU_USE_SMOD3 || SPC5_ICU_USE_SMOD4 || SPC5_ICU_USE_SMOD5
    uint8_t SMOD0 = 0;
    uint8_t SMOD1 = 0;
    uint8_t SMOD2 = 0;
    uint8_t SMOD3 = 0;
    uint8_t SMOD4 = 0;
    uint8_t SMOD5 = 0;
#endif

#if SPC5_ICU_USE_SMOD6 || SPC5_ICU_USE_SMOD7 || SPC5_ICU_USE_SMOD8 ||        \
    SPC5_ICU_USE_SMOD9 || SPC5_ICU_USE_SMOD10 || SPC5_ICU_USE_SMOD11
    uint8_t SMOD6 = 0;
    uint8_t SMOD7 = 0;
    uint8_t SMOD8 = 0;
    uint8_t SMOD9 = 0;
    uint8_t SMOD10 = 0;
    uint8_t SMOD11 = 0;
#endif

#if SPC5_ICU_USE_SMOD0
    if (&ICUD1 == icup) {
      /* Disable channel.*/
      icup->etimerp->ENBL.B.ENBL &= 0b11111110;
      SMOD0 = 1U;
    }
#endif
#if SPC5_ICU_USE_SMOD1
    if (&ICUD2 == icup) {
      /* Disable channel.*/
      icup->etimerp->ENBL.B.ENBL &= 0b11111101;
      SMOD1 = 1U;
    }
#endif
#if SPC5_ICU_USE_SMOD2
    if (&ICUD3 == icup) {
      /* Disable channel.*/
      icup->etimerp->ENBL.B.ENBL &= 0b11111011;
      SMOD2 = 1U;
    }
#endif
#if SPC5_ICU_USE_SMOD3
    if (&ICUD4 == icup) {
      /* Disable channel.*/
      icup->etimerp->ENBL.B.ENBL &= 0b11110111;
      SMOD3 = 1U;
    }
#endif
#if SPC5_ICU_USE_SMOD4
    if (&ICUD5 == icup) {
      /* Disable channel.*/
      icup->etimerp->ENBL.B.ENBL &= 0b11101111;
      SMOD4 = 1U;
    }
#endif
#if SPC5_ICU_USE_SMOD5
    if (&ICUD6 == icup) {
      /* Disable channel.*/
      icup->etimerp->ENBL.B.ENBL &= 0b11011111;
      SMOD5 = 1U;
    }
#endif
#if SPC5_ICU_USE_SMOD6
    if (&ICUD7 == icup) {
      /* Disable channel.*/
      icup->etimerp->ENBL.B.ENBL &= 0b11111110;
      SMOD6 = 1U;
    }
#endif
#if SPC5_ICU_USE_SMOD7
    if (&ICUD8 == icup) {
      /* Disable channel.*/
      icup->etimerp->ENBL.B.ENBL &= 0b11111101;
      SMOD7 = 1U;
    }
#endif
#if SPC5_ICU_USE_SMOD8
    if (&ICUD9 == icup) {
      /* Disable channel.*/
      icup->etimerp->ENBL.B.ENBL &= 0b11111011;
      SMOD8 = 1U;
    }
#endif
#if SPC5_ICU_USE_SMOD9
    if (&ICUD10 == icup) {
      /* Disable channel.*/
      icup->etimerp->ENBL.B.ENBL &= 0b11110111;
      SMOD9 = 1U;
    }
#endif
#if SPC5_ICU_USE_SMOD10
    if (&ICUD11 == icup) {
      /* Disable channel.*/
      icup->etimerp->ENBL.B.ENBL &= 0b11101111;
      SMOD10 = 1U;
    }
#endif
#if SPC5_ICU_USE_SMOD11
    if (&ICUD12 == icup) {
      /* Disable channel.*/
      icup->etimerp->ENBL.B.ENBL &= 0b11011111;
      SMOD11 = 1U;
    }
#endif

#if SPC5_ICU_USE_SMOD0 || SPC5_ICU_USE_SMOD1 || SPC5_ICU_USE_SMOD2 ||        \
    SPC5_ICU_USE_SMOD3 || SPC5_ICU_USE_SMOD4 || SPC5_ICU_USE_SMOD5
    if (SMOD0 || SMOD1 || SMOD2 || SMOD3 || SMOD4 || SMOD5) {
      /* Clock deactivation.*/
      if (icup->etimerp->ENBL.B.ENBL == 0x00) {
        halSPCSetPeripheralClockMode(SPC5_ETIMER0_PCTL,
                                     SPC5_ICU_ETIMER0_STOP_PCTL);
      }
    }
#endif

#if SPC5_ICU_USE_SMOD6 || SPC5_ICU_USE_SMOD7 || SPC5_ICU_USE_SMOD8 ||        \
    SPC5_ICU_USE_SMOD9 || SPC5_ICU_USE_SMOD10 || SPC5_ICU_USE_SMOD11
    if (SMOD6 || SMOD7 || SMOD8 || SMOD9 || SMOD10 || SMOD11) {
      /* Clock deactivation.*/
      if (icup->etimerp->ENBL.B.ENBL == 0x00) {
        halSPCSetPeripheralClockMode(SPC5_ETIMER1_PCTL,
                                     SPC5_ICU_ETIMER1_STOP_PCTL);
      }
    }
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
#if SPC5_ICU_USE_SMOD0
  if (&ICUD1 == icup) {
    spc5_icu_channel_enable(icup, 0);
  }
#endif

#if SPC5_ICU_USE_SMOD1
  if (&ICUD2 == icup) {
    spc5_icu_channel_enable(icup, 1);
  }
#endif

#if SPC5_ICU_USE_SMOD2
  if (&ICUD3 == icup) {
    spc5_icu_channel_enable(icup, 2);
  }
#endif

#if SPC5_ICU_USE_SMOD3
  if (&ICUD4 == icup) {
    spc5_icu_channel_enable(icup, 3);
  }
#endif

#if SPC5_ICU_USE_SMOD4
  if (&ICUD5 == icup) {
    spc5_icu_channel_enable(icup, 4);
  }
#endif

#if SPC5_ICU_USE_SMOD5
  if (&ICUD6 == icup) {
    spc5_icu_channel_enable(icup, 5);
  }
#endif

#if SPC5_ICU_USE_SMOD6
  if (&ICUD7 == icup) {
    spc5_icu_channel_enable(icup, 0);
  }
#endif

#if SPC5_ICU_USE_SMOD7
  if (&ICUD8 == icup) {
    spc5_icu_channel_enable(icup, 1);
  }
#endif

#if SPC5_ICU_USE_SMOD8
  if (&ICUD9 == icup) {
    spc5_icu_channel_enable(icup, 2);
  }
#endif

#if SPC5_ICU_USE_SMOD9
  if (&ICUD10 == icup) {
    spc5_icu_channel_enable(icup, 3);
  }
#endif

#if SPC5_ICU_USE_SMOD10
  if (&ICUD11 == icup) {
    spc5_icu_channel_enable(icup, 4);
  }
#endif

#if SPC5_ICU_USE_SMOD11
  if (&ICUD12 == icup) {
    spc5_icu_channel_enable(icup, 5);
  }
#endif
}

/**
 * @brief   Disables the input capture.
 *
 * @param[in] icup      pointer to the @p ICUDriver object
 *
 * @notapi
 */
void icu_lld_disable(ICUDriver *icup) {
#if SPC5_ICU_USE_SMOD0
  if (&ICUD1 == icup) {
    spc5_icu_channel_disable(icup, 0);
  }
#endif

#if SPC5_ICU_USE_SMOD1
  if (&ICUD2 == icup) {
    spc5_icu_channel_disable(icup, 1);
  }
#endif

#if SPC5_ICU_USE_SMOD2
  if (&ICUD3 == icup) {
    spc5_icu_channel_disable(icup, 2);
  }
#endif

#if SPC5_ICU_USE_SMOD3
  if (&ICUD4 == icup) {
    spc5_icu_channel_disable(icup, 3);
  }
#endif

#if SPC5_ICU_USE_SMOD4
  if (&ICUD5 == icup) {
    spc5_icu_channel_disable(icup, 4);
  }
#endif

#if SPC5_ICU_USE_SMOD5
  if (&ICUD6 == icup) {
    spc5_icu_channel_disable(icup, 5);
  }
#endif

#if SPC5_ICU_USE_SMOD6
  if (&ICUD7 == icup) {
    spc5_icu_channel_disable(icup, 0);
  }
#endif

#if SPC5_ICU_USE_SMOD7
  if (&ICUD8 == icup) {
    spc5_icu_channel_disable(icup, 1);
  }
#endif

#if SPC5_ICU_USE_SMOD8
  if (&ICUD9 == icup) {
    spc5_icu_channel_disable(icup, 2);
  }
#endif

#if SPC5_ICU_USE_SMOD9
  if (&ICUD10 == icup) {
    spc5_icu_channel_disable(icup, 3);
  }
#endif

#if SPC5_ICU_USE_SMOD10
  if (&ICUD11 == icup) {
    spc5_icu_channel_disable(icup, 4);
  }
#endif

#if SPC5_ICU_USE_SMOD11
  if (&ICUD12 == icup) {
    spc5_icu_channel_disable(icup, 5);
  }
#endif
}

/**
 * @brief   Returns the width of the latest pulse.
 * @details The pulse width is defined as number of ticks between the start
 *          edge and the stop edge.
 *
 * @param[in] icup      pointer to the @p ICUDriver object
 * @return              The number of ticks.
 *
 * @notapi
 */
icucnt_t icu_lld_get_width(ICUDriver *icup) {
  return (icucnt_t)*icup->wccrp + 1;
}

/**
 * @brief   Returns the width of the latest cycle.
 * @details The cycle width is defined as number of ticks between a start
 *          edge and the next start edge.
 *
 * @param[in] icup      pointer to the @p ICUDriver object
 * @return              The number of ticks.
 *
 * @notapi
 */
icucnt_t icu_lld_get_period(ICUDriver *icup) {
  return (icucnt_t)*icup->pccrp + 1;
}

#endif /* HAL_USE_ICU */

/** @} */
