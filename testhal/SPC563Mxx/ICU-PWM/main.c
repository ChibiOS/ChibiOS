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

/* Inclusion of the main header files of all the imported components in the
   order specified in the application wizard. The file is generated
   automatically.*/
#include "components.h"

static void pwmpcb(PWMDriver *pwmp) {

  (void)pwmp;
  palClearPad(PORT11, P11_LED1);
}

static void pwmc1cb(PWMDriver *pwmp) {

  (void)pwmp;
  palSetPad(PORT11, P11_LED1);
}

static PWMConfig pwmcfg = {
  100000,                                   /* 100kHz PWM clock frequency.*/
  20000,                                    /* Initial PWM period 0.2s.*/
  pwmpcb,
  {
   {PWM_OUTPUT_ACTIVE_HIGH, pwmc1cb}
  },
  PWM_ALIGN_EDGE
};

icucnt_t last_width, last_period;

static void icuwidthcb(ICUDriver *icup) {

  palSetPad(PORT11, P11_LED2);
  last_width = icuGetWidth(icup);
}

static void icuperiodcb(ICUDriver *icup) {

  palClearPad(PORT11, P11_LED2);
  last_period = icuGetPeriod(icup);
}

static ICUConfig icucfg = {
  ICU_INPUT_ACTIVE_HIGH,
  100000,                                   /* 100kHz ICU clock frequency.*/
  icuwidthcb,
  icuperiodcb,
  NULL
};

/*
 * Application entry point.
 */
int main(void) {

  /* Initialization of all the imported components in the order specified in
	 the application wizard. The function is generated automatically.*/
  componentsInit();

  palClearPad(PORT11, P11_LED4);

  /*
   * Initializes the PWM driver 8 and ICU driver 1.
   * PIN80 is the PWM output.
   * PIN63 is the ICU input.
   * The two pins have to be externally connected together.
   */

  /* Sets PIN63 alternative function.*/
  SIU.PCR[179].R = 0b0000010100001100;

  /* Sets PIN80 alternative function.*/
  SIU.PCR[202].R = 0b0000011000001100;

  icuStart(&ICUD1, &icucfg);
  icuEnable(&ICUD1);
  pwmStart(&PWMD8, &pwmcfg);

  chThdSleepMilliseconds(2000);

  /*
   * Starts the PWM channel 0 using 75% duty cycle.
   */
  pwmEnableChannel(&PWMD8, 0, PWM_PERCENTAGE_TO_WIDTH(&PWMD8, 7500));
  chThdSleepMilliseconds(5000);

  /*
   * Changes the PWM channel 0 to 50% duty cycle.
   */
  pwmEnableChannel(&PWMD8, 0, PWM_PERCENTAGE_TO_WIDTH(&PWMD8, 5000));
  chThdSleepMilliseconds(5000);

  /*
   * Changes the PWM channel 0 to 25% duty cycle.
   */
  pwmEnableChannel(&PWMD8, 0, PWM_PERCENTAGE_TO_WIDTH(&PWMD8, 2500));
  chThdSleepMilliseconds(5000);

  /*
   * Changes PWM period and the PWM channel 0 to 50% duty cycle.
   */
  pwmChangePeriod(&PWMD8, 25000);
  pwmEnableChannel(&PWMD8, 0, PWM_PERCENTAGE_TO_WIDTH(&PWMD8, 5000));
  chThdSleepMilliseconds(5000);

  /*
   * Disables PWM channel 0 and stops the drivers.
   */
  pwmDisableChannel(&PWMD8, 0);
  pwmStop(&PWMD8);

  /*
   * Disables and stops the ICU drivers.
   */

  icuDisable(&ICUD1);
  icuStop(&ICUD1);

  palClearPad(PORT11, P11_LED3);
  palClearPad(PORT11, P11_LED4);

  /*
   * Normal main() thread activity, in this demo it does nothing.
   */
  while (TRUE) {
    chThdSleepMilliseconds(500);
  }
  return 0;
}
