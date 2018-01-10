/*
    ChibiOS - Copyright (C) 2006..2016 Giovanni Di Sirio

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

/* PIOBU pin attribute */
static PIOBUConfig piocfg[] = {
  {
    .pinIndex = 0, /* PIOBU0 */
    .afv = 5,
    .rfv = 5,
    .mode = 1,
    .outputLevel = 0,
    .pullUpState = 0,
    .scheduled  = 0,
    .inputDefaultLevel = 0,
    .dynamic = 1,
    .filter3_5 = 0,
  },
  {
    .pinIndex = 1, /* PIOBU1 */
    .afv = 5,
    .rfv = 5,
    .mode = 0,
    .outputLevel = 0,
    .pullUpState = 0,
    .scheduled = 0,
    .inputDefaultLevel = 0,
    .dynamic = 1,
    .filter3_5 = 0,
  },
  {
    .pinIndex = 3, /* PIOBU3 */
    .afv = 6,
    .rfv = 6,
    .mode = 0,
    .outputLevel = 0,
    .pullUpState = 1,
    .scheduled = 0,
    .inputDefaultLevel = 1,
    .dynamic = 0,
    .filter3_5 = 0,
  },
  {
    .pinIndex = 4, /* PIOBU4 */
    .afv = 5,
    .rfv = 5,
    .mode = 1,
    .outputLevel = 0,
    .pullUpState = 0,
    .scheduled  = 0,
    .inputDefaultLevel = 0,
    .dynamic = 1,
    .filter3_5 = 0,
  },
  {
    .pinIndex = 5, /* PIOBU5 */
    .afv = 5,
    .rfv = 5,
    .mode = 0,
    .outputLevel = 0,
    .pullUpState = 0,
    .scheduled = 0,
    .inputDefaultLevel = 0,
    .dynamic = 1,
    .filter3_5 = 0,
  },
  {
    .pinIndex = 7, /* PIOBU7 */
    .afv = 8,
    .rfv = 8,
    .mode = 0,
    .outputLevel = 0,
    .pullUpState = 2,
    .scheduled = 0,
    .inputDefaultLevel = 0,
    .dynamic = 0,
    .filter3_5 = 0,
  },
};

static void secram_callback(SECDriver *secp) {
  (void)secp;
  palToggleLine(LINE_LED_RED);
}

static void erase_callback(SECDriver *secp) {
  (void)secp;
  palToggleLine(LINE_LED_GREEN);
}

static void secmod_callback(SECDriver *secp,  secevent_t event) {
  (void)secp;
  chSysLockFromISR();
  switch (event) {
  case SEC_EVENT_PIOBU:
    /* Erasing memories on intrusion */
    secumodSoftwareProtection();
    break;
  case SEC_EVENT_SHLDM:
    ;
    break;
  case SEC_EVENT_DBLFM:
    ;
    break;
  case SEC_EVENT_TST:
    ;
    break;
  case SEC_EVENT_JTAG:
    ;
    break;
  case SEC_EVENT_MCKM:
    ;
    break;
  case SEC_EVENT_TPML:
    ;
    break;
  case SEC_EVENT_TPMH:
    ;
    break;
  case SEC_EVENT_VDDBUL:
    ;
    break;
  case SEC_EVENT_VDDBUH:
    ;
    break;
  case SEC_EVENT_VDDCOREL:
    ;
    break;
  case SEC_EVENT_VDDCOREH:
    ;
    break;
  }
  chSysUnlockFromISR();
}

static SECConfig secumod_cfg = {
  secram_callback,                          /* securam callback        */
  erase_callback,                           /* erased callback         */
  6,                                        /* Pads to configure       */
  piocfg,                                   /* PIOBU config            */
  {
    {RAMACC_RD_ACCESS},                     /* RAM region 0 permission */
    {RAMACC_WR_RD_ACCESS},                  /* RAM region 1 permission */
    {RAMACC_WR_RD_ACCESS},                  /* RAM region 2 permission */
    {RAMACC_WR_RD_ACCESS},                  /* RAM region 3 permission */
    {RAMACC_WR_RD_ACCESS},                  /* RAM region 4 permission */
    {RAMACC_WR_RD_ACCESS}                   /* RAM region 5 permission */
  },
  SECUMOD_CR_NORMAL |                       /* CR register             */
  SECUMOD_CR_SCRAMB(SCRAMB_DISABLE),
  SECUMOD_JTAGCR_CA5_DEBUG_MODE_FULL_DEBUG  /* JTAGCR register         */
};

/*
 * LED blinker thread, times are in milliseconds.
 */
static THD_WORKING_AREA(waThread1, 512);
static THD_FUNCTION(Thread1, arg) {

  (void)arg;
  chRegSetThreadName("blinker");

  while (true) {
    palToggleLine(LINE_LED_BLUE);
    chThdSleepMilliseconds(80);
    palToggleLine(LINE_LED_BLUE);
    chThdSleepMilliseconds(120);
    palToggleLine(LINE_LED_BLUE);
    chThdSleepMilliseconds(120);
    palToggleLine(LINE_LED_BLUE);
    chThdSleepMilliseconds(120);
    palToggleLine(LINE_LED_BLUE);
    chThdSleepMilliseconds(160);
    palToggleLine(LINE_LED_BLUE);
    chThdSleepMilliseconds(600);
  }
}

/*
 * Application entry point.
 */
int main(void) {

  /*
   * System initializations.
   * - HAL initialization, this also initializes the configured device drivers
   *   and performs the board-specific initializations.
   * - Kernel initialization, the main() function becomes a thread and the
   *   RTOS is active.
   */
  halInit();
  chSysInit();
  secInit();

  /* REGION_0 location */
  uint32_t *region0 = (uint32_t *) 0xF8044000;

  /*
   * Starts SECUMOD Driver
   */
  secStart(&SECD0, &secumod_cfg);

  /*
   * Creates the blinker thread.
   */
  chThdCreateStatic(waThread1, sizeof(waThread1), NORMALPRIO, Thread1, NULL);

  /* Intrusion on PIOBU pad 0 causes interrupt but not automating erase */
  secSetCallback(&SECD0, SECUMOD_DET0, &secmod_callback);

  /*
   * Normal main() thread activity, in this demo it does nothing except
   * sleeping in a loop and check the button state.
   */
  while (true) {
    if(!palReadPad(PIOB, PIOB_USER_PB)) {
      /* Write access in REGION_0 causes SECURAM interrupt */
      *region0 = 0xAA55AA55;
    }
    chThdSleepMilliseconds(500);
  }
}
