/*
    ChibiOS/RT - Copyright (C) 2006-2013 Giovanni Di Sirio

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

#include "ch.h"
#include "hal.h"

/**
 * @brief   PAL setup.
 * @details Digital I/O ports static configuration as defined in @p board.h.
 *          This variable is used by the HAL when initializing the PAL driver.
 */
#if HAL_USE_PAL || defined(__DOXYGEN__)
const PALConfig pal_default_config = {
 {VAL_PORT0DATA, VAL_PORT0DIR, VAL_PORT0ICR, VAL_PORT0ODR, 0},
 {VAL_PORT1DATA, VAL_PORT1DIR, VAL_PORT1ICR, VAL_PORT1ODR, 0},
 {VAL_PORT2DATA, VAL_PORT2DIR, VAL_PORT2ICR, VAL_PORT2ODR, 0},
 {VAL_PORT3DATA, VAL_PORT3DIR, VAL_PORT3ICR, VAL_PORT3ODR, 0},
 {VAL_PORT4DATA, VAL_PORT4DIR, VAL_PORT4ICR, 0           , 0},
 {VAL_PORT5DATA, VAL_PORT5DIR, VAL_PORT5ICR, 0           , 0},
 {VAL_PORTADATA, VAL_PORTADIR, VAL_PORTAICR, 0           , VAL_PORTAPCR},
 {VAL_PORTBDATA, VAL_PORTBDIR, VAL_PORTBICR, 0           , VAL_PORTBPCR},
 {VAL_PORTCDATA, VAL_PORTCDIR, VAL_PORTCICR, VAL_PORTCODR, VAL_PORTCPCR},
 {VAL_PORTDDATA, VAL_PORTDDIR, VAL_PORTDICR, 0           , VAL_PORTDPCR},
 {VAL_PORTEDATA, VAL_PORTEDIR, VAL_PORTEICR, 0           , VAL_PORTEPCR},
};
#endif

/**
 * @brief   CMT3 interrupt handler.
 */
CH_IRQ_HANDLER(Excep_CMTU1_CMT3) {

  CH_IRQ_PROLOGUE();

  chSysLockFromIsr();
  chSysTimerHandlerI();
  chSysUnlockFromIsr();

  CH_IRQ_EPILOGUE();
}

/*
 * Early initialization code.
 * This initialization must be performed just after stack setup and before
 * any other initialization.
 */
void __early_init(void) {

  rx62n_clock_init();
}

/*
 * Board-specific initialization code.
 */
void boardInit(void) {

  /* Setup tick timer */
  MSTP(CMT3) = 0;             /* Enable CMT3 timer (cancel module stop state) */
  CMT.CMSTR1.BIT.STR3 = 0;    /* Stop timer */
  CMT3.CMCR.BIT.CKS = 2;      /* PCLK/128 */
  CMT3.CMCNT = 0;
  CMT3.CMCOR = RX62N_PERCLK / 128 / CH_FREQUENCY;
  CMT3.CMCR.BIT.CMIE = 1;     /* Enable compare match interrupts */
  IEN(CMT3,CMI3) = 1;         /* Set Interrupt Enable Register */
  IPR(CMT3,CMI3) = 8;         /* Setup interrupt priority */
  CMT.CMSTR1.BIT.STR3 = 1;    /* Start timer */
}
