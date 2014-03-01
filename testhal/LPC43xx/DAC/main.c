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

#define NSAMPLES    255

const uint32_t sine_wave[NSAMPLES] = {
 512 << 6,  524 << 6,  537 << 6,  549 << 6,  562 << 6,  574 << 6,  587 << 6,  599 << 6,  612 << 6,  624 << 6,
 636 << 6,  649 << 6,  661 << 6,  673 << 6,  685 << 6,  696 << 6,  708 << 6,  720 << 6,  731 << 6,  743 << 6,
 754 << 6,  765 << 6,  776 << 6,  786 << 6,  797 << 6,  807 << 6,  818 << 6,  828 << 6,  837 << 6,  847 << 6,
 856 << 6,  866 << 6,  875 << 6,  883 << 6,  892 << 6,  900 << 6,  908 << 6,  916 << 6,  924 << 6,  931 << 6,
 938 << 6,  945 << 6,  952 << 6,  958 << 6,  964 << 6,  970 << 6,  975 << 6,  981 << 6,  985 << 6,  990 << 6,
 994 << 6,  998 << 6, 1002 << 6, 1006 << 6, 1009 << 6, 1012 << 6, 1014 << 6, 1016 << 6, 1018 << 6, 1020 << 6,
1021 << 6, 1022 << 6, 1023 << 6, 1023 << 6, 1023 << 6, 1023 << 6, 1023 << 6, 1022 << 6, 1021 << 6, 1019 << 6,
1017 << 6, 1015 << 6, 1013 << 6, 1010 << 6, 1007 << 6, 1004 << 6, 1000 << 6,  996 << 6,  992 << 6,  988 << 6,
 983 << 6,  978 << 6,  973 << 6,  967 << 6,  961 << 6,  955 << 6,  948 << 6,  942 << 6,  935 << 6,  928 << 6,
 920 << 6,  912 << 6,  904 << 6,  896 << 6,  888 << 6,  879 << 6,  870 << 6,  861 << 6,  852 << 6,  842 << 6,
 832 << 6,  823 << 6,  812 << 6,  802 << 6,  792 << 6,  781 << 6,  770 << 6,  759 << 6,  748 << 6,  737 << 6,
 725 << 6,  714 << 6,  702 << 6,  691 << 6,  679 << 6,  667 << 6,  655 << 6,  642 << 6,  630 << 6,  618 << 6,
 606 << 6,  593 << 6,  581 << 6,  568 << 6,  556 << 6,  543 << 6,  530 << 6,  518 << 6,  505 << 6,  493 << 6,
 480 << 6,  467 << 6,  455 << 6,  442 << 6,  430 << 6,  417 << 6,  405 << 6,  393 << 6,  381 << 6,  368 << 6,
 356 << 6,  344 << 6,  332 << 6,  321 << 6,  309 << 6,  298 << 6,  286 << 6,  275 << 6,  264 << 6,  253 << 6,
 242 << 6,  231 << 6,  221 << 6,  211 << 6,  200 << 6,  191 << 6,  181 << 6,  171 << 6,  162 << 6,  153 << 6,
 144 << 6,  135 << 6,  127 << 6,  119 << 6,  111 << 6,  103 << 6,   95 << 6,   88 << 6,   81 << 6,   75 << 6,
  68 << 6,   62 << 6,   56 << 6,   50 << 6,   45 << 6,   40 << 6,   35 << 6,   31 << 6,   27 << 6,   23 << 6,
  19 << 6,   16 << 6,   13 << 6,   10 << 6,    8 << 6,    6 << 6,    4 << 6,    2 << 6,    1 << 6,    0 << 6,
   0 << 6,    0 << 6,    0 << 6,    0 << 6,    1 << 6,    2 << 6,    3 << 6,    5 << 6,    7 << 6,    9 << 6,
  11 << 6,   14 << 6,   17 << 6,   21 << 6,   25 << 6,   29 << 6,   33 << 6,   38 << 6,   42 << 6,   48 << 6,
  53 << 6,   59 << 6,   65 << 6,   71 << 6,   78 << 6,   85 << 6,   92 << 6,   99 << 6,  107 << 6,  115 << 6,
 123 << 6,  131 << 6,  140 << 6,  148 << 6,  157 << 6,  167 << 6,  176 << 6,  186 << 6,  195 << 6,  205 << 6,
 216 << 6,  226 << 6,  237 << 6,  247 << 6,  258 << 6,  269 << 6,  280 << 6,  292 << 6,  303 << 6,  315 << 6,
 327 << 6,  338 << 6,  350 << 6,  362 << 6,  374 << 6,  387 << 6,  399 << 6,  411 << 6,  424 << 6,  436 << 6,
 449 << 6,  461 << 6,  474 << 6,  486 << 6,  499 << 6};

/*
 * Red LEDs blinker thread, times are in milliseconds.
 */
static WORKING_AREA(waThread1, 128);
static msg_t Thread1(void *arg) {

  (void)arg;
  chRegSetThreadName("blinker");
  while (TRUE) {
    palTogglePad(GPIO2, GPIO2_LED);
    chThdSleepMilliseconds(500);
  }
}

/*
 * DAC conversion groups, with callbacks.
 */
static const DACConversionGroup dacconvgrp1 = {
  1, /* Channels */
  NULL, /* End of transfer callback */
  NULL, /* Error callback */
  true /*circular mode */
};

/*
 * DAC config
 */
static const DACConfig daccfg1 = {
  1000*NSAMPLES, /* Multiply the buffer size to the desired frequency in Hz */
};

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

  /*
   * Creates the blinker thread.
   */
  chThdCreateStatic(waThread1, sizeof(waThread1), NORMALPRIO, Thread1, NULL);

  /*
   * Starting the DAC driver.
   */
  dacStart(&DACD1, &daccfg1);

  /*
   * Sending the dac_buffer
   */
  dacStartConversion(&DACD1, &dacconvgrp1, sine_wave, NSAMPLES);

  while (TRUE) {

  chThdSleepMilliseconds(1000);

  }
}
